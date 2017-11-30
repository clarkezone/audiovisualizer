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
	class ArrayValueView : public RuntimeClass<IVectorView<float>,IIterable<float>>
	{
		InspectableClass(IVectorView<float>::z_get_rc_name_impl(),BaseTrust)
		size_t _size;
		float *_pData;
	public:
		ArrayValueView(DirectX::XMVECTOR *pBuffer, size_t cElements)
		{
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
		STDMETHODIMP First(IIterator<float> **ppIterator)
		{
			auto iterator = Make<IteratorImpl<float>>((float *)_pData, _size);
			return iterator.CopyTo(ppIterator);
		}
	};

	class SpectrumData : public RuntimeClass<ISpectrumData,IVectorView<IVectorView<float>*>>, 
		public Implements<IVectorView<IVectorView<float>*>>
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
		SpectrumData(size_t cChannels, size_t cElements, ScaleType ampScaleType,ScaleType fScaleType,float minFrequency,float maxFrequency,float fStep,bool bInit=false);
		~SpectrumData();

		DirectX::XMVECTOR *GetBuffer() { return _pData; }

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
		STDMETHODIMP get_FrequencyCount(UINT32 *pCount)
		{
			if (pCount == nullptr)
				return E_INVALIDARG;
			*pCount = (UINT32) _size;
			return S_OK;
		}

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
		STDMETHODIMP TransformLinearFrequency(UINT32 cElements, ISpectrumData **ppResult);
		STDMETHODIMP TransformLinearFrequencyWithRange(UINT32 cElements,float fromFrequency, float toFrequency, ISpectrumData **result);
		STDMETHODIMP TransformLogFrequency(UINT32 cElements, float minFrequency, float maxFrequency, ISpectrumData **ppResult);
		STDMETHODIMP ConvertToLogAmplitude(float minValue, float maxValue, ISpectrumData **ppResult);
		STDMETHODIMP ApplyRiseAndFall(ISpectrumData *pPrevious, TimeSpan riseTime, TimeSpan fallTime, TimeSpan timeDelta, ISpectrumData **ppResult);

	};
}

