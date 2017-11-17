#include "pch.h"
#include "ArrayData.h"
#include "AudioMath.h"
#include <Microsoft.Graphics.Canvas.h>

namespace AudioVisualizer
{
	class ArrayDataFactory : public AgileActivationFactory<IArrayDataFactory>
	{
	public:
		IFACEMETHODIMP CreateLinear(UINT32 channels, UINT32 cElements,float upperFrequency,IArrayData **ppResult)
		{
			ComPtr<ArrayData> data = Make<ArrayData>(channels, cElements, ScaleType::Linear,ScaleType::Linear,0.0f,upperFrequency,upperFrequency/cElements,true);
			return data.CopyTo(ppResult);
		}
	};


	ArrayData::ArrayData(size_t cChannels, size_t cElements, ScaleType ampScaleType, ScaleType fScaleType, float minFrequency, float maxFrequency, float frequencyStep,bool bInit) :
		_amplitudeScale(ampScaleType),
		_frequencyScale(fScaleType),
		_minFrequency(minFrequency),
		_maxFrequency(maxFrequency),
		_frequencyStep(frequencyStep),
		_pData(nullptr), 
		_vElementsCount(0), 
		_size(0), 
		_channels(0)
	{
		_vElementsCount = (cElements + 3) >> 2;	// Make sure channel data is aligned
		_size = cElements;
		_channels = cChannels;
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(_vElementsCount * cChannels * sizeof(DirectX::XMVECTOR), 16, __FILE__, __LINE__));
		if (bInit)
		{
			memset(_pData, 0, _vElementsCount * cChannels * sizeof(DirectX::XMVECTOR));
		}
		_values.resize(_channels);
		for (size_t index = 0,vIndex = 0; index < _channels; index++,vIndex += _vElementsCount)
		{
			_values[index] = Make<ArrayValueView>(_pData + vIndex,cElements);
		}
	}

	ArrayData::~ArrayData()
	{
		if (_pData != nullptr)
			_aligned_free(_pData);
	}

	STDMETHODIMP ArrayData::ConvertToLogAmplitude(float minValue, float maxValue, IArrayData **ppResult)
	{
		ComPtr<ArrayData> result = Make<ArrayData>(
			_channels,
			_size,
			ScaleType::Logarithmic,
			_frequencyScale,
			_minFrequency,
			_maxFrequency,
			_frequencyStep);

		Math::ConvertToLogarithmic(_pData, result->GetBuffer(), _vElementsCount * _channels, minValue, maxValue);
		return result.CopyTo(ppResult);
	}

	STDMETHODIMP ArrayData::ApplyRiseAndFall(IArrayData * pPrevious, TimeSpan riseTime, TimeSpan fallTime, TimeSpan timeDelta, IArrayData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || timeDelta.Duration == 0)
			return E_INVALIDARG;

		ArrayData *pPreviousData = dynamic_cast<ArrayData *>(pPrevious);

		if (pPreviousData != nullptr &&
				( pPreviousData->_size != _size ||
					pPreviousData->_channels != _channels))
		{
			return E_INVALIDARG;
		}

		ComPtr<ArrayData> result = Make<ArrayData>(
			_channels,
			_size,
			_amplitudeScale,
			_frequencyScale,
			_minFrequency,
			_maxFrequency,
			_frequencyStep);

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

	STDMETHODIMP ArrayData::TransformLinearFrequency(UINT32 cElements, IArrayData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || _frequencyScale != ScaleType::Linear || cElements < 1)
			return E_FAIL;

		ComPtr<ArrayData> result = Make<ArrayData>(_channels,
			cElements,
			ScaleType::Linear,
			ScaleType::Linear,
			_minFrequency,
			_maxFrequency,
			(_maxFrequency - _minFrequency) / (float) cElements);
		for (size_t index = 0,vSrcIndex = 0,vDstIndex = 0; index < _channels; index++,vSrcIndex+=_vElementsCount,vDstIndex+=result->_vElementsCount)
		{
			float *pSource = (float *)(GetBuffer() + vSrcIndex);
			float *pDest = (float*)(result->GetBuffer() + vDstIndex);
			Math::SpectrumTransform(pSource, _size, 0.0f, (float) _size, pDest, result->_size,true);
		}

		return result.CopyTo(ppResult);
	}

	STDMETHODIMP ArrayData::TransformLinearFrequencyWithRange(UINT32 cElements, float fromFrequency, float toFrequency, IArrayData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || _frequencyScale != ScaleType::Linear || cElements < 1)
			return E_FAIL;

		if (cElements < 1 || fromFrequency >= toFrequency)
			return E_INVALIDARG;

		ComPtr<ArrayData> result = Make<ArrayData>(_channels,
			cElements,
			ScaleType::Linear,
			ScaleType::Linear,
			fromFrequency,
			toFrequency,
			(fromFrequency - toFrequency) / cElements);

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

	STDMETHODIMP ArrayData::ConvertToLogFrequency(UINT32 cElements, float minFrequency, float maxFrequency, InterpolationType ipType, IArrayData ** ppResult)
	{
		if (_amplitudeScale != ScaleType::Linear || 
			_frequencyScale != ScaleType::Linear)
			return E_FAIL;
		if (minFrequency >= maxFrequency || cElements < 1)
			return E_INVALIDARG;

		ComPtr<ArrayData> result = Make<ArrayData>(_channels,
			cElements,
			ScaleType::Linear,
			ScaleType::Linear,
			minFrequency,
			maxFrequency,
			(_maxFrequency - _minFrequency) / cElements);

		return E_NOTIMPL;
	}

	ActivatableClassWithFactory(ArrayData, ArrayDataFactory);

}

