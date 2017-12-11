#pragma once
#include <DirectXMath.h>
#include <vector>
#include "LifeSpanTracker.h"
#include "wrl_util.h"
#include "trace.h"

using namespace Microsoft::WRL;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Diagnostics;
using namespace wrl_util;

namespace AudioVisualizer
{
	class SpectrumData : public RuntimeClass<ISpectrumData,
		IVectorView<IVectorView<float>*>, 
		IIterable<IVectorView<float> *>>, public LifespanTracker<SpectrumData>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_SpectrumData, BaseTrust);

		ScaleType _amplitudeScale;
		DirectX::XMVECTOR *_pData;
		size_t _vElementsCount;	// XMVector elements count per channel
		size_t _size;	// Elements count per channel
		size_t _channels;
		std::vector<ComPtr<IVectorView<float>>> _values;
		ScaleType _frequencyScale;
		float _minFrequency;
		float _maxFrequency;
		float _frequencyStep;
	public:
		SpectrumData();
		~SpectrumData();
		HRESULT RuntimeClassInitialize(size_t cChannels, size_t cElements, ScaleType ampScaleType, ScaleType fScaleType, float minFrequency, float maxFrequency, bool bInit);

		DirectX::XMVECTOR *GetBuffer() { return _pData; }

		const std::vector<ComPtr<IVectorView<float>>> & GetValues() { return _values; }

		STDMETHODIMP get_AmplitudeScale(ScaleType *pScale)
		{
			if (pScale == nullptr)
				return E_INVALIDARG;
			*pScale = _amplitudeScale;
			return S_OK;
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
		STDMETHODIMP get_FrequencyCount(UINT32 *pValue)
		{
			if (pValue == nullptr)
				return E_INVALIDARG;
			*pValue = _size;
			return S_OK;
		}
		STDMETHODIMP TransformLinearFrequency(UINT32 cElements, ISpectrumData **ppResult);
		STDMETHODIMP TransformLinearFrequencyWithRange(UINT32 cElements, float fromFrequency, float toFrequency, ISpectrumData **result);
		STDMETHODIMP TransformLogFrequency(UINT32 cElements, float minFrequency, float maxFrequency, ISpectrumData **ppResult);
		STDMETHODIMP ConvertToLogAmplitude(float minValue, float maxValue, ISpectrumData **ppResult);
		STDMETHODIMP ApplyRiseAndFall(ISpectrumData *pPrevious, TimeSpan riseTime, TimeSpan fallTime, TimeSpan timeDelta, ISpectrumData **ppResult);
		STDMETHODIMP CombineChannels(UINT32 elementCount, float *pMap, ISpectrumData **result);
		STDMETHODIMP GetFrequency(UINT32 elementIndex, float *pValue)
		{
			if (pValue == nullptr)
				return E_POINTER;
			*pValue = _frequencyScale == ScaleType::Linear ?
				_minFrequency + _frequencyStep * elementIndex :
				_minFrequency * powf(_frequencyStep, (float)elementIndex);
			return S_OK;
		}
		STDMETHODIMP GetCenterFrequency(UINT32 elementIndex, float *pValue)
		{
			if (pValue == nullptr)
				return E_POINTER;
			float baseFrequency = _frequencyScale == ScaleType::Linear ?
				_minFrequency + _frequencyStep / 2 :
				_minFrequency * sqrtf(_frequencyStep);
			*pValue = _frequencyScale == ScaleType::Linear ?
				baseFrequency + _frequencyStep * elementIndex :
				baseFrequency * powf(_frequencyStep, (float)elementIndex);
			return S_OK;
		}

		/* IVectorView interfaces */
		STDMETHODIMP GetAt(unsigned int index, IVectorView<float> **ppValue)
		{
			if (ppValue == nullptr)
				return E_POINTER;
			if (index >= _channels)
				return E_INVALIDARG;

			return _values[index].CopyTo(ppValue);
		}
		STDMETHODIMP get_Size(UINT32 *pSize)
		{
			if (pSize == nullptr)
				return E_INVALIDARG;
			*pSize = (UINT32) _channels;
			return S_OK;
		}
		STDMETHODIMP IndexOf(IVectorView<float> * value, unsigned int *, boolean *)
		{
			return E_NOTIMPL;
		}
		/* IIterable interfaces */
		STDMETHODIMP First(IIterator<IVectorView<float>*> **ppIterator);
	};
}

