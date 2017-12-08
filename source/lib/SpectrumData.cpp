#include "pch.h"
#include "SpectrumData.h"
#include "AudioMath.h"
#include "trace.h"
#include <Microsoft.Graphics.Canvas.h>


namespace AudioVisualizer
{
	class SpectrumDataIterator : public RuntimeClass<IIterator<IVectorView<float> *>>, public LifespanTracker<SpectrumDataIterator>
	{
		UINT32 _currentIndex;
		ComPtr<SpectrumData> _data;
		InspectableClass(IIterator<IVectorView<float>*>::z_get_rc_name_impl(), BaseTrust);
	public:
		SpectrumDataIterator(SpectrumData *pSpectrumData)
		{
			_currentIndex = 0;
			_data = pSpectrumData;
		}
		virtual /* propget */ HRESULT STDMETHODCALLTYPE get_Current(IVectorView<float> **ppCurrent)
		{
			if (_currentIndex >= _data->GetValues().size())
				return E_BOUNDS;
			return _data->GetValues()[_currentIndex].CopyTo(ppCurrent);
		};
		virtual /* propget */ HRESULT STDMETHODCALLTYPE get_HasCurrent(_Out_ boolean *hasCurrent)
		{
			if (hasCurrent == nullptr)
				return E_POINTER;
			*hasCurrent = _currentIndex < _data->GetValues().size();
			return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ boolean *hasCurrent)
		{
			if (hasCurrent == nullptr)
				return E_POINTER;
			if (_currentIndex < _data->GetValues().size())
			{
				_currentIndex++;
			}
			*hasCurrent = _currentIndex < _data->GetValues().size();
			return S_OK;
		}
	};

	class SpectrumValuesIterator : public RuntimeClass<IIterator<float>> , public LifespanTracker<SpectrumValuesIterator>
	{
		UINT32 _currentIndex;
		ComPtr<ISpectrumData> _data;
		UINT32 _size;
		float *_pData;

		InspectableClass(IIterator<float>::z_get_rc_name_impl(), BaseTrust);
	public:
		SpectrumValuesIterator(ISpectrumData *pSpectrumData,float *pData,UINT32 size)
		{
			_currentIndex = 0;
			_data = pSpectrumData;
			_pData = pData;
			_size = size;
		}
		virtual /* propget */ HRESULT STDMETHODCALLTYPE get_Current(float *pCurrent)
		{
			if (_currentIndex >= _size)
				return E_BOUNDS;
			*pCurrent = _pData[_currentIndex];
			return S_OK;
		};
		virtual /* propget */ HRESULT STDMETHODCALLTYPE get_HasCurrent(_Out_ boolean *hasCurrent)
		{
			if (hasCurrent == nullptr)
				return E_POINTER;
			*hasCurrent = _currentIndex < _size;
			return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ boolean *hasCurrent)
		{
			if (hasCurrent == nullptr)
				return E_POINTER;
			if (_currentIndex < _size)
			{
				_currentIndex++;
			}
			*hasCurrent = _currentIndex < _size;
			return S_OK;
		}
	};

	class SpectrumValuesView : public RuntimeClass<IVectorView<float>, IIterable<float>> , public LifespanTracker<SpectrumValuesView>
	{
		InspectableClass(IVectorView<float>::z_get_rc_name_impl(), BaseTrust);
		WeakRef _dataRef;
		UINT32 _size;
		float *_pData;
	public:
		SpectrumValuesView(ISpectrumData *pData,float *pBuffer,UINT32 size)
		{
			ComPtr<ISpectrumData> spectrumData = pData;
			spectrumData.AsWeak(&_dataRef);
			_size = size;
			_pData = pBuffer;
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
			ComPtr<ISpectrumData> spectrumData;
			_dataRef.As(&spectrumData);

			if (spectrumData == nullptr)
				return RO_E_CLOSED;

			auto iterator = Make<SpectrumValuesIterator>(spectrumData.Get(),_pData, _size);
			return iterator.CopyTo(ppIterator);
		}
	};

	class SpectrumDataFactory : public AgileActivationFactory<ISpectrumDataFactory>
	{
	public:
		STDMETHODIMP Create(	UINT32 channels,
								UINT32 cElements,
								ScaleType amplitudeScaleType,
								ScaleType frequencyScaleType,
								float minFrequency,
								float maxFrequency,
								ISpectrumData **ppResult
		)
		{
			if (channels == 0 || cElements == 0)
				return E_INVALIDARG;
			if (maxFrequency <= minFrequency || maxFrequency <= 0)
				return E_INVALIDARG;
			if (frequencyScaleType == ScaleType::Logarithmic && minFrequency <= 0)
				return E_INVALIDARG;

			float fStep = frequencyScaleType == ScaleType::Linear ?
				(maxFrequency - minFrequency) / cElements :
				powf(maxFrequency / minFrequency, 1.0f / cElements);

			ComPtr<SpectrumData> result;
			HRESULT hr = MakeAndInitialize<SpectrumData>(&result,
				channels,
				cElements,
				amplitudeScaleType,
				frequencyScaleType,
				minFrequency,
				maxFrequency,
				true);
			if (FAILED(hr))
				return hr;

			return result.CopyTo(ppResult);
		}
		STDMETHODIMP CreateWithValues(
			IVectorView<IVectorView<float> *> *pData,
			ScaleType amplitudeScaleType,
			ScaleType frequencyScaleType,
			float minFrequency,
			float maxFrequency,
			ISpectrumData **ppResult
		)
		{
			if (pData == nullptr)
				return E_POINTER;
			if (maxFrequency <= minFrequency || maxFrequency <= 0)
				return E_INVALIDARG;
			if (frequencyScaleType == ScaleType::Logarithmic && minFrequency <= 0)
				return E_INVALIDARG;

			UINT32 channels = 0;
			HRESULT hr = pData->get_Size(&channels);
			if (FAILED(hr))
				return hr;

			UINT32 cElements = 0;
			for (size_t channelIndex = 0; channelIndex < channels; channelIndex++)
			{
				ComPtr<IVectorView<float>> item;
				hr = pData->GetAt(channelIndex, &item);
				if (FAILED(hr))
					return hr;
				UINT32 itemCount = 0;
				hr = item->get_Size(&itemCount);
				if (FAILED(hr))
					return hr;
				if (channelIndex == 0)
					cElements = itemCount;
				else if (itemCount != cElements)
					return E_INVALIDARG;
			}

			if (channels == 0 || cElements == 0)
				return E_INVALIDARG;

			float fStep = frequencyScaleType == ScaleType::Linear ?
				(maxFrequency - minFrequency) / cElements :
				powf(maxFrequency / minFrequency, 1.0f / cElements);

			ComPtr<SpectrumData> result;
			
			hr = MakeAndInitialize<SpectrumData>(&result,
				channels,
				cElements,
				amplitudeScaleType,
				frequencyScaleType,
				minFrequency,
				maxFrequency,
				false);

			if (FAILED(hr))
				return hr;

			size_t vElementsCount = (cElements + 3) >> 2;
			DirectX::XMVECTOR *pValues = result->GetBuffer();

			for (size_t channelIndex = 0; channelIndex < channels; channelIndex++,pValues += vElementsCount)
			{
				ComPtr<IVectorView<float>> item;
				hr = pData->GetAt(channelIndex, &item);
				if (FAILED(hr))
					return hr;
				UINT32 cActual = 0;
				hr = item->GetMany(0, cElements, (float*)pValues, &cActual);
				if (FAILED(hr))
					return hr;
				if (cActual != cElements)
					return E_FAIL;
			}

			return result.CopyTo(ppResult);
		}
	};

	SpectrumData::SpectrumData() :
		_pData(nullptr), 
		_vElementsCount(0), 
		_size(0), 
		_channels(0)
	{
	}

	SpectrumData::~SpectrumData()
	{
		if (_pData != nullptr)
			_aligned_free(_pData);
	}

	HRESULT SpectrumData::RuntimeClassInitialize(size_t cChannels, size_t cElements, ScaleType ampScaleType, ScaleType fScaleType, float minFrequency, float maxFrequency, bool bInit)
	{
		if (cChannels == 0 || cElements == 0)
			return E_INVALIDARG;
		if (maxFrequency <= minFrequency || minFrequency < 0)
			return E_INVALIDARG;
		if (fScaleType == ScaleType::Logarithmic && minFrequency == 0)
			return E_INVALIDARG;

		_amplitudeScale = ampScaleType;
		_frequencyScale = fScaleType;
		_minFrequency = minFrequency;
		_maxFrequency = maxFrequency;
		_frequencyStep = _frequencyScale == ScaleType::Linear ?
			(maxFrequency - minFrequency) / cElements :
			powf(maxFrequency / minFrequency, 1.0f / cElements);

		_vElementsCount = (cElements + 3) >> 2;	// Make sure channel data is aligned
		_size = cElements;
		_channels = cChannels;
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(_vElementsCount * cChannels * sizeof(DirectX::XMVECTOR), 16, __FILE__, __LINE__));
		if (bInit)
		{
			memset(_pData, 0, _vElementsCount * cChannels * sizeof(DirectX::XMVECTOR));
		}
		_values.resize(_channels);
		for (size_t index = 0, vIndex = 0; index < _channels; index++, vIndex += _vElementsCount)
		{
			_values[index] = Make<SpectrumValuesView>(this, (float *)(_pData + vIndex), cElements);
		}

		return S_OK;
	}

	STDMETHODIMP SpectrumData::ConvertToLogAmplitude(float minValue, float maxValue, ISpectrumData **ppResult)
	{
		ComPtr<SpectrumData> result;
		HRESULT hr = MakeAndInitialize<SpectrumData>(
			&result,
			_channels,
			_size,
			ScaleType::Logarithmic,
			_frequencyScale,
			_minFrequency,
			_maxFrequency,
			false);
		if (FAILED(hr))
			return hr;

		Math::ConvertToLogarithmic(_pData, result->GetBuffer(), _vElementsCount * _channels, minValue, maxValue);
		return result.CopyTo(ppResult);
	}

	STDMETHODIMP SpectrumData::ApplyRiseAndFall(ISpectrumData * pPrevious, TimeSpan riseTime, TimeSpan fallTime, TimeSpan timeDelta, ISpectrumData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || timeDelta.Duration == 0)
			return E_INVALIDARG;

		SpectrumData *pPreviousData = dynamic_cast<SpectrumData *>(pPrevious);

		if (pPreviousData != nullptr &&
				( pPreviousData->_size != _size ||
					pPreviousData->_channels != _channels))
		{
			return E_INVALIDARG;
		}

		ComPtr<SpectrumData> result;
		HRESULT hr = MakeAndInitialize<SpectrumData>(
			&result,
			_channels,
			_size,
			_amplitudeScale,
			_frequencyScale,
			_minFrequency,
			_maxFrequency,
			false);
		if (FAILED(hr))
			return hr;

		size_t vSize = (_size + 3) >> 2;
		DirectX::XMVECTOR *pLastData = nullptr;
		if (pPrevious != nullptr)
		{
			pLastData = pPreviousData->GetBuffer();
		}
		float riseT = riseTime.Duration != 0 ? (float)timeDelta.Duration / riseTime.Duration : std::numeric_limits<float>::infinity();
		float fallT = fallTime.Duration != 0 ? (float)timeDelta.Duration / fallTime.Duration : std::numeric_limits<float>::infinity();

		Math::ApplyRiseAndFall(pLastData, GetBuffer(), result->GetBuffer(), _vElementsCount * _channels, riseT, fallT);

		return result.CopyTo(ppResult);
	}

	STDMETHODIMP SpectrumData::CombineChannels(UINT32 elementCount, float *pMap, ISpectrumData **ppResult)
	{
		if (pMap == nullptr)
			return E_POINTER;
		if (elementCount < _channels)
			return E_INVALIDARG;
		if (_amplitudeScale != ScaleType::Linear)
			return E_INVALIDARG;

		UINT32 outputChannels = elementCount / _channels;
		ComPtr<SpectrumData> result;
		HRESULT hr = MakeAndInitialize<SpectrumData>(
			&result,
			outputChannels,
			_size,
			_amplitudeScale,
			_frequencyScale,
			_minFrequency,
			_maxFrequency,
			false);
		if (FAILED(hr))
			return hr;


		std::vector<XMVECTOR *> src(_channels);
		XMVECTOR *pData = _pData;
		for (size_t channelIndex = 0; channelIndex < _channels; channelIndex++,pData+=_vElementsCount)
		{
			src[channelIndex] = pData;
		}

		XMVECTOR *pDest = result->GetBuffer();
		for (size_t channelIndex = 0; channelIndex < outputChannels; channelIndex++,pDest+=_vElementsCount)
		{
			Math::CombineChannels(src.data(), _channels, _vElementsCount, pMap + channelIndex * _channels, pDest);
		}

		return result.CopyTo(ppResult);
	}

	STDMETHODIMP SpectrumData::First(IIterator<IVectorView<float> *> **ppIterator)
	{
		auto iterator = Make<SpectrumDataIterator>(this);
		return iterator.CopyTo(ppIterator);
	}

	STDMETHODIMP SpectrumData::TransformLinearFrequency(UINT32 cElements, ISpectrumData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || _frequencyScale != ScaleType::Linear || cElements < 1)
			return E_FAIL;

		ComPtr<SpectrumData> result;
		HRESULT hr = MakeAndInitialize<SpectrumData>(
			&result,
			_channels,
			cElements,
			_amplitudeScale,
			_frequencyScale,
			_minFrequency,
			_maxFrequency,
			false);
		if (FAILED(hr))
			return hr;

		for (size_t index = 0,vSrcIndex = 0,vDstIndex = 0; index < _channels; index++,vSrcIndex+=_vElementsCount,vDstIndex+=result->_vElementsCount)
		{
			float *pSource = (float *)(GetBuffer() + vSrcIndex);
			float *pDest = (float*)(result->GetBuffer() + vDstIndex);
			Math::SpectrumTransform(pSource, _size, 0.0f, (float) _size, pDest, result->_size,true);
		}

		return result.CopyTo(ppResult);
	}

	STDMETHODIMP SpectrumData::TransformLinearFrequencyWithRange(UINT32 cElements, float fromFrequency, float toFrequency, ISpectrumData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || _frequencyScale != ScaleType::Linear || cElements < 1)
			return E_FAIL;

		if (cElements < 1 || fromFrequency >= toFrequency)
			return E_INVALIDARG;
		ComPtr<SpectrumData> result;
		HRESULT hr = MakeAndInitialize<SpectrumData>(
			&result,
			_channels,
			cElements,
			_amplitudeScale,
			_frequencyScale,
			fromFrequency,
			toFrequency,
			false);
		if (FAILED(hr))
			return hr;

		float fromIndex = (fromFrequency - _minFrequency) / _frequencyStep;
		float toIndex = (toFrequency - _minFrequency) / _frequencyStep;

		for (size_t index = 0, vSrcIndex = 0, vDstIndex = 0; index < _channels; index++, vSrcIndex += _vElementsCount, vDstIndex += result->_vElementsCount)
		{
			float *pSource = (float *)(GetBuffer() + vSrcIndex);
			float *pDest = (float*)(result->GetBuffer() + vDstIndex);
			Math::SpectrumTransform(pSource, _size, fromIndex, toIndex, pDest, result->_size, true);
		}

		return result.CopyTo(ppResult);
	}

	STDMETHODIMP SpectrumData::TransformLogFrequency(UINT32 cElements, float fromFrequency, float toFrequency, ISpectrumData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || 
			_frequencyScale != ScaleType::Linear)
			return E_FAIL;
		if (fromFrequency <= 0 || toFrequency < fromFrequency)
			return E_INVALIDARG;
		if (fromFrequency >= toFrequency || cElements < 1)
			return E_INVALIDARG;

		ComPtr<SpectrumData> result;
		HRESULT hr = MakeAndInitialize<SpectrumData>(
			&result,
			_channels,
			cElements,
			_amplitudeScale,
			ScaleType::Logarithmic,
			fromFrequency,
			toFrequency,
			false);
		if (FAILED(hr))
			return hr;

		float fromIndex = (fromFrequency - _minFrequency) / _frequencyStep;
		float toIndex = (toFrequency - _minFrequency) / _frequencyStep;

		for (size_t index = 0, vSrcIndex = 0, vDstIndex = 0; index < _channels; index++, vSrcIndex += _vElementsCount, vDstIndex += result->_vElementsCount)
		{
			float *pSource = (float *)(GetBuffer() + vSrcIndex);
			float *pDest = (float*)(result->GetBuffer() + vDstIndex);
			Math::SpectrumTransform(pSource, _size, fromIndex, toIndex, pDest, result->_size, false);
		}

		return result.CopyTo(ppResult);
	}

	ActivatableClassWithFactory(SpectrumData, SpectrumDataFactory);

}

