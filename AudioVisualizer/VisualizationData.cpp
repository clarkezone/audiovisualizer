#include "pch.h"
#include "VisualizationData.h"
#include <AudioMath.h>
namespace AudioVisualizer
{

	STDMETHODIMP ScalarData::ConvertToLogAmplitude(float minValue, float maxValue, IVisualizationData **ppResult)
	{
		auto returnValue = Make<ScalarData>(_size, ScaleType::Logarithmic);
		size_t vSize = (_size + 3) >> 2;
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, vSize, minValue, maxValue);
		return returnValue.CopyTo(ppResult);
	}

	STDMETHODIMP ScalarData::ApplyRiseAndFall(IVisualizationData * pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IVisualizationData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || riseTime.Duration == 0 || fallTime.Duration == 0)
			return E_INVALIDARG;

		ComPtr<ScalarData> data;
		size_t vSize = (_size + 3) >> 2;
		if (pPrevious != nullptr)
		{
			data = Make<ScalarData>(_size, _amplitudeScale);

			ComPtr<IScalarData> last;
			HRESULT hr = ComPtr<IVisualizationData>(pPrevious).As(&last);
			if (FAILED(hr))
				return E_FAIL;

			UINT32 cCount;
			DirectX::XMVECTOR *pLastData;

			last->get_Values(&cCount, (float **) &pLastData);
			
			AudioMath::ApplyRiseAndFall(pLastData, _pData, data->_pData, vSize, 1e7f * timeDelta.Duration / riseTime.Duration, 1e7f * timeDelta.Duration / fallTime.Duration);
		}
		else
		{
			data = Make<ScalarData>(_size,_amplitudeScale);
			memcpy(data->_pData, _pData, vSize * sizeof(DirectX::XMVECTOR));
			_CrtCheckMemory();
		}
		return data.CopyTo(ppResult);
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
