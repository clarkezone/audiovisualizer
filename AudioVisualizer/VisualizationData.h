#pragma once

#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer_h.h"
#include <DirectXMath.h>
#include <vector>

using namespace Microsoft::WRL;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Foundation;

namespace AudioVisualizer
{
	class VisualizationData : public Implements<IVisualizationData>
	{
	protected:
		ScaleType _amplitudeScale;
	public:
		VisualizationData(ScaleType scaleType = ScaleType::Linear) : _amplitudeScale(scaleType)
		{
		}
	

		STDMETHODIMP get_AmplitudeScale(ScaleType *pScale)
		{
			if (pScale == nullptr)
				return E_INVALIDARG;
			*pScale = _amplitudeScale;
			return S_OK;
		}

	};

	class ScalarData : public RuntimeClass<MixIn<ScalarData,VisualizationData>,IScalarData>, public VisualizationData
	{
		InspectableClass(RuntimeClass_AudioVisualizer_ScalarData, BaseTrust);

		DirectX::XMVECTOR *_pData;
		size_t _size;

	public:
		ScalarData(size_t cElements, ScaleType scaleType = ScaleType::Linear) :
			VisualizationData(scaleType) 
		{
			size_t vLength = (cElements + 3) >> 2;	// Vector size of the allocated buffer
			_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(vLength * sizeof(DirectX::XMVECTOR), 16,__FILE__,__LINE__));
			_size = cElements;
		}
		~ScalarData()
		{
			if (_pData != nullptr)
				_aligned_free(_pData);
		}

		DirectX::XMVECTOR *GetBuffer() { return _pData; }

		STDMETHODIMP get_Values(UINT32 *pcElements, float **ppValues)
		{
			if (ppValues == nullptr)
				return E_INVALIDARG;
			*ppValues = (float *)_pData;
			*pcElements = _size;
			return S_OK;
		}
		STDMETHODIMP get_Size(UINT32 *pSize)
		{
			if (pSize == nullptr)
				return E_INVALIDARG;
			*pSize = _size;
			return S_OK;
		}
		STDMETHODIMP ConvertToLogAmplitude(float minValue, float maxValue, IVisualizationData **ppResult);
		STDMETHODIMP ApplyRiseAndFall(IVisualizationData *pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IVisualizationData **ppResult);
	};
	
	class VectorData : public RuntimeClass<MixIn<VectorData, VisualizationData>, IVectorView<IVectorView<float> *>,IVectorData>, public VisualizationData
	{
		class ValueVectorView : public RuntimeClass<IVectorView<float>>
		{
			ComPtr<VectorData> _vectorDataHost;
			size_t _size;
			float *_pData;
		public:
			ValueVectorView(VectorData *pHost, DirectX::XMVECTOR *pBuffer, size_t cElements)
			{
				_vectorDataHost = pHost;	// Keep the smart pointer to where data is kept for reference counting
				_pData = reinterpret_cast<float *>(pBuffer);
				_size = cElements;
			}
			STDMETHODIMP GetAt(unsigned int index, float *pResult)
			{
				if (pResult == nullptr)
					return E_INVALIDARG;
				if (index >= _size)
					return E_INVALIDARG;
				*pResult = _pData[index];
				return S_OK;
			}
			STDMETHODIMP get_Size(unsigned int *pSize)
			{
				if (pSize == nullptr)
					return E_INVALIDARG;
				*pSize = (unsigned int)_size;
				return S_OK;
			}
			STDMETHODIMP IndexOf(float, unsigned int *, boolean *)
			{
				return E_NOTIMPL;
			}

		};
		InspectableClass(RuntimeClass_AudioVisualizer_VectorData, BaseTrust);
		DirectX::XMVECTOR *_pData;
		size_t _vElementsCount;	// XMVector elements count per channel
		size_t _size;	// Elements count per channel
		size_t _channels;
		std::vector<ComPtr<ValueVectorView>> _channelData;
		ScaleType _frequencyScale;
		float _minFrequency;
		float _maxFrequency;
		float _frequencyStep;
	public:
		VectorData(size_t cChannels, size_t cElements, ScaleType scaleType = ScaleType::Linear) :
			VisualizationData(scaleType), _pData(nullptr), _vElementsCount(0),_size(0),_channels(0)
		{
			_vElementsCount = (cElements + 3) >> 2;	// Make sure channel data is aligned
			_size = cElements;
			_channels = cChannels;
			_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(_vElementsCount * cChannels * sizeof(DirectX::XMVECTOR), 16,__FILE__, __LINE__));
			_channelData.resize(_channels);
			for (size_t index = 0,vIndex = 0; index < _channels; index++,vIndex+=_vElementsCount)
			{
				_channelData[index] = Make<ValueVectorView>(this, _pData + vIndex, _size);
			}
		}
		~VectorData()
		{
			if (_pData != nullptr)
				_aligned_free(_pData);
		}

		DirectX::XMVECTOR *GetBuffer() { return _pData; }

		STDMETHODIMP GetAt(unsigned int index, IVectorView<float> **ppResult)
		{
			if (ppResult == nullptr)
				return E_INVALIDARG;
			if (index >= _channels)
				return E_INVALIDARG;
			HRESULT hr = _channelData[index].CopyTo(ppResult);
			return hr;
		}
		STDMETHODIMP get_Size(unsigned int *pSize)
		{
			if (pSize == nullptr)
				return E_INVALIDARG;
			*pSize = (unsigned int)_channels;
			return S_OK;
		}
		STDMETHODIMP IndexOf(IVectorView<float> *pSearch, unsigned int *, boolean *)
		{
			return E_NOTIMPL;
		}

		STDMETHODIMP get_FrequencyScale(ScaleType *pScale)
		{
			if (pScale == nullptr)
				return E_INVALIDARG;
			*pScale = _frequencyScale;
			return S_OK;
		}

		STDMETHODIMP get_MinFrequency(float *pValue)
		{
			if (pValue == nullptr)
				return E_INVALIDARG;
			*pValue = _minFrequency;
			return S_OK;
		}
		STDMETHODIMP get_MaxFrequency(float *pValue)
		{
			if (pValue == nullptr)
				return E_INVALIDARG;
			*pValue = _maxFrequency;
			return S_OK;
		}
		STDMETHODIMP get_FrequencyStep(float *pValue)
		{
			if (pValue == nullptr)
				return E_INVALIDARG;
			*pValue = _frequencyStep;
			return S_OK;
		}
		STDMETHODIMP CreateLinearDistribution(UINT32 cElements, IVisualizationData **ppResult);
		STDMETHODIMP CreateLogDistribution(UINT32 cElements, float minFrequency, float maxFrequency, InterpolationType ipType, IVisualizationData **ppResult);
		STDMETHODIMP ConvertToLogAmplitude(float minValue, float maxValue, IVisualizationData **ppResult);
		STDMETHODIMP ApplyRiseAndFall(IVisualizationData *pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IVisualizationData **ppResult);

	};
}

