#include "pch.h"
#include "ScalarData.h"
#include <AudioMath.h>

namespace AudioVisualizer
{
	class ScalarDataStatics : public AgileActivationFactory<IScalarDataStatics>
	{
		InspectableClassStatic(RuntimeClass_AudioVisualizer_ScalarData, BaseTrust);
	public:
		// IScalarDataStatics
		IFACEMETHODIMP CreateEmpty(UINT32 channels, IScalarData **ppResult)
		{
			ComPtr<ScalarData> data = Make<ScalarData>(channels, ScaleType::Linear, true);
			return data.CopyTo(ppResult);
		}
		STDMETHODIMP Create(UINT32 channels, float *pValues, IScalarData **ppResult)
		{
			ComPtr<ScalarData> data = Make<ScalarData>(channels, ScaleType::Linear, true);
			memcpy(data->GetBuffer(), pValues, channels * sizeof(float));
			return data.CopyTo(ppResult);
		}
		STDMETHODIMP ApplyRiseAndFallToEmpty(IScalarData * pPrevious, TimeSpan riseTime, TimeSpan fallTime, TimeSpan timeDelta, IScalarData ** ppResult)
		{
			if (timeDelta.Duration == 0 || pPrevious == nullptr)
				return E_INVALIDARG;
			ScaleType scale;
			pPrevious->get_AmplitudeScale(&scale);
			if (scale == ScaleType::Logarithmic)
				return E_INVALIDARG;
			ComPtr<IVectorView<float>> vector;
			ComPtr<IScalarData>(pPrevious).As(&vector);
			UINT32 size = 0;
			vector->get_Size(&size);

			ComPtr<ScalarData> result = Make<ScalarData>(size,ScaleType::Linear);

			size_t vSize = (size + 3) >> 2;
			DirectX::XMVECTOR *pLastData = nullptr;
			if (pPrevious != nullptr)
			{
				ScalarData *pPreviousData = dynamic_cast<ScalarData *>(pPrevious);
				pLastData = pPreviousData->GetBuffer();
			}
			float riseT = riseTime.Duration != 0 ? (float)timeDelta.Duration / riseTime.Duration : std::numeric_limits<float>::infinity();
			float fallT = fallTime.Duration != 0 ? (float)timeDelta.Duration / fallTime.Duration : std::numeric_limits<float>::infinity();

			Math::ApplyRiseAndFall(pLastData, nullptr, result->GetBuffer(), vSize, riseT, fallT);
			return result.CopyTo(ppResult);
		}

	};

	ActivatableStaticOnlyFactory(ScalarDataStatics);


	ScalarData::ScalarData(size_t cElements, ScaleType scaleType, bool bInit)
	{
		_amplitudeScale = scaleType;
		size_t vLength = (cElements + 3) >> 2;	// Vector size of the allocated buffer
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(vLength * sizeof(DirectX::XMVECTOR), 16, __FILE__, __LINE__));
		if (bInit)
		{
			memset(_pData, 0, vLength * sizeof(DirectX::XMVECTOR));
		}
		_size = cElements;
	}
	ScalarData::~ScalarData()
	{
		if (_pData != nullptr)
			_aligned_free(_pData);
	}
	STDMETHODIMP ScalarData::ConvertToLogAmplitude(float minValue, float maxValue, IScalarData **ppResult)
	{
		if (_amplitudeScale == ScaleType::Logarithmic)
			return E_NOT_VALID_STATE;
		if (maxValue <= minValue)
			return E_INVALIDARG;
		auto returnValue = Make<ScalarData>(_size, ScaleType::Logarithmic);
		size_t vSize = (_size + 3) >> 2;
		Math::ConvertToLogarithmic(_pData, returnValue->_pData, vSize, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP ScalarData::ApplyRiseAndFall(IScalarData * pPrevious, TimeSpan riseTime, TimeSpan fallTime, TimeSpan timeDelta, IScalarData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || timeDelta.Duration == 0)
			return E_INVALIDARG;
		if (pPrevious != nullptr)
		{
			ScaleType scale;
			pPrevious->get_AmplitudeScale(&scale);
			if (scale == ScaleType::Logarithmic)
				return E_INVALIDARG;

			ComPtr<IVectorView<float>> vector;
			ComPtr<IScalarData>(pPrevious).As(&vector);
			UINT32 size = 0;
			vector->get_Size(&size);
			if (size != _size)
				return E_INVALIDARG;
		}

		ComPtr<ScalarData> result = Make<ScalarData>(_size, _amplitudeScale);

		size_t vSize = (_size + 3) >> 2;
		DirectX::XMVECTOR *pLastData = nullptr;
		if (pPrevious != nullptr)
		{
			ScalarData *pPreviousData = dynamic_cast<ScalarData *>(pPrevious);
			pLastData = pPreviousData->GetBuffer();
		}
		float riseT = riseTime.Duration != 0 ? (float)timeDelta.Duration / riseTime.Duration : std::numeric_limits<float>::infinity();
		float fallT = fallTime.Duration != 0 ? (float)timeDelta.Duration / fallTime.Duration : std::numeric_limits<float>::infinity();

		Math::ApplyRiseAndFall(pLastData, GetBuffer(), result->GetBuffer(), vSize, riseT, fallT);
		return result.CopyTo(ppResult);
	}

}