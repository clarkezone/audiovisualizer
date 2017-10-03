#include "pch.h"
#include "ScalarData.h"
#include <AudioMath.h>
#include "ErrorHandling.h"
#include "Utilities.h"

namespace AudioVisualizer
{
	class ScalarDataFactory : public AgileActivationFactory<IScalarDataFactory>
	{
	public:
		IFACEMETHODIMP Create(UINT32 channels,IScalarData **ppResult)
		{
			ComPtr<ScalarData> data = Make<ScalarData>(channels, ScaleType::Linear,true);
			return data.CopyTo(ppResult);
		}
	};

	ScalarData::ScalarData(size_t cElements, ScaleType scaleType,bool bInit)
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
		auto returnValue = Make<ScalarData>(_size, ScaleType::Logarithmic);
		size_t vSize = (_size + 3) >> 2;
		Math::ConvertToLogarithmic(_pData, returnValue->_pData, vSize, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP ScalarData::ApplyRiseAndFall(IScalarData * pPrevious, TimeSpan riseTime, TimeSpan fallTime, TimeSpan timeDelta, IScalarData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || timeDelta.Duration == 0)
			return E_INVALIDARG;

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

	ActivatableClassWithFactory(ScalarData, ScalarDataFactory);

}