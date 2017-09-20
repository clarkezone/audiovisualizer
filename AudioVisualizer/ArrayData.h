#pragma once

#include "AudioVisualizer.abi.h"
#include <DirectXMath.h>
#include <vector>
#include "LifeSpanTracker.h"
#include "trace.h"

using namespace Microsoft::WRL;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Diagnostics;

namespace AudioVisualizer
{
	class ArrayData : public RuntimeClass<IArrayData>, LifespanTracker<ArrayData>
	{
		
		class ValueArrayView : public RuntimeClass<IVectorView<float>>,LifespanTracker<ValueArrayView>
		{
			size_t _size;
			float *_pData;
		public:
			ValueArrayView(DirectX::XMVECTOR *pBuffer, size_t cElements)
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
		};

		class ArrayVectorView : public RuntimeClass<IVectorView<IVectorView<float>*>>, LifespanTracker<ArrayVectorView>
		{
			std::vector<IVectorView<float> *> _vectors;
			size_t _size;
		public:
			ArrayVectorView(size_t channels,size_t cElements, DirectX::XMVECTOR *pData)
			{
				_size = channels;
				_vectors.resize(_size);
				size_t vElementsCount = (cElements + 3) >> 2;	// Make sure channel data is aligned
				for (size_t index = 0, vIndex = 0; index < _size; index++, vIndex += vElementsCount)
				{
					auto view = Make<ValueArrayView>(pData + vIndex, _size);
					view.CopyTo(&_vectors[index]);
				}
			}

			STDMETHODIMP GetAt(unsigned int index, IVectorView<float> **pResult)
			{
				if (pResult == nullptr)
					return E_INVALIDARG;
				if (index >= _size)
					return E_INVALIDARG;
				*pResult = _vectors[index];
				return S_OK;
			}
			STDMETHODIMP get_Size(unsigned int *pSize)
			{
				if (pSize == nullptr)
					return E_INVALIDARG;
				*pSize = (unsigned int)_size;
				return S_OK;
			}
			STDMETHODIMP IndexOf(IVectorView<float> *, unsigned int *, boolean *)
			{
				return E_NOTIMPL;
			}
		};

		InspectableClass(RuntimeClass_AudioVisualizer_ArrayData, BaseTrust);

		ScaleType _amplitudeScale;
		DirectX::XMVECTOR *_pData;
		size_t _vElementsCount;	// XMVector elements count per channel
		size_t _size;	// Elements count per channel
		size_t _channels;
		ComPtr<ArrayVectorView> _values;
		ScaleType _frequencyScale;
		float _minFrequency;
		float _maxFrequency;
		float _frequencyStep;
	public:
		ArrayData(size_t cChannels, size_t cElements, ScaleType scaleType = ScaleType::Linear);
		~ArrayData();

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
		STDMETHODIMP get_Values(IVectorView<IVectorView<float> *> **ppValues)
		{
			if (ppValues == nullptr)
				return E_INVALIDARG;
			_values.CopyTo(ppValues);
			return S_OK;
		}
		STDMETHODIMP CreateLinearDistribution(UINT32 cElements, IArrayData **ppResult);
		STDMETHODIMP CreateLogDistribution(UINT32 cElements, float minFrequency, float maxFrequency, InterpolationType ipType, IArrayData **ppResult);
		STDMETHODIMP ConvertToLogAmplitude(float minValue, float maxValue, IArrayData **ppResult);
		STDMETHODIMP ApplyRiseAndFall(IArrayData *pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IArrayData **ppResult);

	};
}

