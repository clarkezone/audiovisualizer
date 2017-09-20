#include "pch.h"
#include "ArrayData.h"
#include <AudioMath.h>

namespace AudioVisualizer
{
	STDMETHODIMP ArrayData::ConvertToLogAmplitude(float minValue, float maxValue, IArrayData **ppResult)
	{
		auto returnValue = Make<ArrayData>(_channels, _size, ScaleType::Logarithmic);
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, _vElementsCount, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP ArrayData::ApplyRiseAndFall(IArrayData * pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IArrayData ** ppResult)
	{
		return E_NOTIMPL;
	}

	ArrayData::ArrayData(size_t cChannels, size_t cElements, ScaleType scaleType) :
		_amplitudeScale(scaleType), _pData(nullptr), _vElementsCount(0), _size(0), _channels(0)
	{
		_vElementsCount = (cElements + 3) >> 2;	// Make sure channel data is aligned
		_size = cElements;
		_channels = cChannels;
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(_vElementsCount * cChannels * sizeof(DirectX::XMVECTOR), 16, __FILE__, __LINE__));
		_values = Make<ArrayVectorView>(cChannels, cElements, _pData);
	}

	ArrayData::~ArrayData()
	{
		if (_pData != nullptr)
			_aligned_free(_pData);
	}

	STDMETHODIMP ArrayData::CreateLinearDistribution(UINT32 cElements, IArrayData ** ppResult)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP ArrayData::CreateLogDistribution(UINT32 cElements, float minFrequency, float maxFrequency, InterpolationType ipType, IArrayData ** ppResult)
	{
		return E_NOTIMPL;
	}

}

