#include "pch.h"
#include "VisualizationData.h"
#include <AudioMath.h>
namespace AudioVisualizer
{

	STDMETHODIMP ScalarData::ConvertToLogAmplitude(float minValue, float maxValue, IVisualizationData **ppResult)
	{
		auto returnValue = Make<ScalarData>(_size, ScaleType::Logarithmic);
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, (_size + 3) >> 2, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP ScalarData::ApplyRiseAndFall(IVisualizationData * pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IVisualizationData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear)
			return E_INVALIDARG;
		return E_NOTIMPL;
	}


	STDMETHODIMP VectorData::ConvertToLogAmplitude(float minValue, float maxValue, IVisualizationData **ppResult)
	{
		auto returnValue = Make<VectorData>(_channels,_size, ScaleType::Logarithmic);
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, _vElementsCount, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP VectorData::ApplyRiseAndFall(IVisualizationData * pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IVisualizationData ** ppResult)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP VectorData::CreateLinearDistribution(UINT32 cElements, IVisualizationData ** ppResult)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP VectorData::CreateLogDistribution(UINT32 cElements, float minFrequency, float maxFrequency, InterpolationType ipType, IVisualizationData ** ppResult)
	{
		return E_NOTIMPL;
	}


}
