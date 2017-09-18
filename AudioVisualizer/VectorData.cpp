#include "pch.h"
#include "VectorData.h"
#include <AudioMath.h>

namespace AudioVisualizer
{


	STDMETHODIMP VectorData::ConvertToLogAmplitude(float minValue, float maxValue, IVectorData **ppResult)
	{
		auto returnValue = Make<VectorData>(_channels, _size, ScaleType::Logarithmic);
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, _vElementsCount, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP VectorData::ApplyRiseAndFall(IVectorData * pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IVectorData ** ppResult)
	{
		return E_NOTIMPL;
	}

	VectorData::VectorData(size_t cChannels, size_t cElements, ScaleType scaleType) :
		_amplitudeScale(scaleType), _pData(nullptr), _vElementsCount(0), _size(0), _channels(0)
	{
		_vElementsCount = (cElements + 3) >> 2;	// Make sure channel data is aligned
		_size = cElements;
		_channels = cChannels;
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(_vElementsCount * cChannels * sizeof(DirectX::XMVECTOR), 16, __FILE__, __LINE__));
		_values = Make<VectorVectorView>(cChannels, cElements, _pData);
	}

	VectorData::~VectorData()
	{
		if (_pData != nullptr)
			_aligned_free(_pData);
	}

	STDMETHODIMP VectorData::CreateLinearDistribution(UINT32 cElements, IVectorData ** ppResult)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP VectorData::CreateLogDistribution(UINT32 cElements, float minFrequency, float maxFrequency, InterpolationType ipType, IVectorData ** ppResult)
	{
		return E_NOTIMPL;
	}

}

