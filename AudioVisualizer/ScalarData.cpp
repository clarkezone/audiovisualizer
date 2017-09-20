#include "pch.h"
#include "ScalarData.h"
#include <AudioMath.h>

namespace AudioVisualizer
{
	ScalarData::ScalarData(size_t cElements, ScaleType scaleType)
	{
		_amplitudeScale = scaleType;
		size_t vLength = (cElements + 3) >> 2;	// Vector size of the allocated buffer
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(vLength * sizeof(DirectX::XMVECTOR), 16, __FILE__, __LINE__));
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
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, vSize, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP ScalarData::ApplyRiseAndFall(IScalarData * pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IScalarData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || riseTime.Duration == 0 || fallTime.Duration == 0)
			return E_INVALIDARG;

		ComPtr<ScalarData> data;
		size_t vSize = (_size + 3) >> 2;
		if (pPrevious != nullptr)
		{
			data = Make<ScalarData>(_size, _amplitudeScale);

			UINT32 cCount;
			DirectX::XMVECTOR *pLastData;

			pPrevious->get_Values(&cCount, (float **)&pLastData);

			AudioMath::ApplyRiseAndFall(pLastData, _pData, data->_pData, vSize, (float) timeDelta.Duration / riseTime.Duration, (float) timeDelta.Duration / fallTime.Duration);
		}
		else
		{
			data = Make<ScalarData>(_size, _amplitudeScale);
			memcpy(data->_pData, _pData, vSize * sizeof(DirectX::XMVECTOR));
		}
		return data.CopyTo(ppResult);
	}
}