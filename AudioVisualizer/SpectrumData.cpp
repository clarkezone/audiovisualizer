#include "pch.h"
#include "SpectrumData.h"
#include "ScalarData.h"
#include "AudioMath.h"
#include "VectorData.h"

namespace winrt::AudioVisualizer::implementation
{
	struct VectorDataIterator : implements<VectorDataIterator, Windows::Foundation::Collections::IIterator<AudioVisualizer::VectorData>>
	{
		const float *_pData;
		size_t _size;
		size_t _currentIndex;
		size_t _elementStep;
		SpectrumData _data{ nullptr };

		VectorDataIterator(SpectrumData const &data) {
			winrt::copy_to_abi(data, _data);
			_currentIndex = 0;
		}

		uint32_t GetMany(array_view<AudioVisualizer::VectorData>)
		{
			throw hresult_not_implemented();
		}

		bool MoveNext()
		{
			if (_currentIndex < _data.Size())
			{
				_currentIndex++;
			}
			return  _currentIndex < _data.Size();
		}

		bool HasCurrent()
		{
			return _currentIndex < _data.Size();
		}

		AudioVisualizer::VectorData Current()
		{
			if (_currentIndex >= _data.Size()) {
				throw hresult_out_of_bounds();
			}
			return make<VectorData>((float *)(_data._pData + _currentIndex * _data._vElementsCount), _data._size, *this);
		}
	};

    AudioVisualizer::ScaleType SpectrumData::AmplitudeScale()
    {
		return _amplitudeScale;
    }

    AudioVisualizer::ScaleType SpectrumData::FrequencyScale()
    {
		return _frequencyScale;
    }

    float SpectrumData::MinFrequency()
    {
		return _minimumFrequency;
    }

    float SpectrumData::MaxFrequency()
    {
		return _maximumFrequency;
    }

    float SpectrumData::FrequencyStep()
    {
		return _frequencyStep;
    }

    uint32_t SpectrumData::FrequencyCount()
    {
		return _size;
    }

    AudioVisualizer::SpectrumData SpectrumData::LinearTransform(uint32_t elementCount, float fromFrequency, float toFrequency)
    {
		if (_amplitudeScale != ScaleType::Linear || _frequencyScale != ScaleType::Linear)
			throw hresult_error(E_NOT_VALID_STATE);

		if (fromFrequency >= toFrequency || elementCount < 1)
				throw hresult_invalid_argument();

		auto result = make_self<SpectrumData>(
			_channels,
			elementCount,
			_amplitudeScale,
			_frequencyScale,
			fromFrequency,
			toFrequency,
			false);

		float fromIndex = (fromFrequency - _minimumFrequency) / _frequencyStep;
		float toIndex = (toFrequency - _minimumFrequency) / _frequencyStep;

		for (size_t index = 0, vSrcIndex = 0, vDstIndex = 0; index < _channels; index++, vSrcIndex += _vElementsCount, vDstIndex += result->_vElementsCount)
		{
			float *pSource = (float *)(_pData + vSrcIndex);
			float *pDest = (float*)(result->_pData + vDstIndex);
			AudioMath::SpectrumTransform(pSource, _size, fromIndex, toIndex, pDest, result->_size, true);
		}

		return result.as<AudioVisualizer::SpectrumData>();
    }

    AudioVisualizer::SpectrumData SpectrumData::LogarithmicTransform(uint32_t elementCount, float fromFrequency, float toFrequency)
    {
		if (_amplitudeScale != ScaleType::Linear ||
			_frequencyScale != ScaleType::Linear)
			throw hresult_error(E_NOT_VALID_STATE);
		if (fromFrequency <= 0 || toFrequency < fromFrequency)
			throw hresult_invalid_argument();

		if (fromFrequency >= toFrequency || elementCount < 1)
			throw hresult_invalid_argument();

		auto result = make_self<SpectrumData>(
			_channels,
			elementCount,
			_amplitudeScale,
			ScaleType::Logarithmic,
			fromFrequency,
			toFrequency,
			false);


		float fromIndex = (fromFrequency - _minimumFrequency) / _frequencyStep;
		float toIndex = (toFrequency - _minimumFrequency) / _frequencyStep;

		for (size_t index = 0, vSrcIndex = 0, vDstIndex = 0; index < _channels; index++, vSrcIndex += _vElementsCount, vDstIndex += result->_vElementsCount)
		{
			float *pSource = (float *)(_pData + vSrcIndex);
			float *pDest = (float*)(result->_pData + vDstIndex);
			AudioMath::SpectrumTransform(pSource, _size, fromIndex, toIndex, pDest, result->_size, false);
		}

		return result.as<AudioVisualizer::SpectrumData>();
    }

    AudioVisualizer::SpectrumData SpectrumData::ApplyRiseAndFall(AudioVisualizer::SpectrumData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
		if (_amplitudeScale != ScaleType::Linear)
			throw hresult_error(E_NOT_VALID_STATE);

		if (previous && previous.AmplitudeScale() != ScaleType::Linear)
		{
			throw hresult_invalid_argument();
		}

		if (previous && (previous.Size() != Size() || previous.FrequencyCount() != FrequencyCount()))
		{
			throw hresult_invalid_argument(L"Previous data not the same size");
		}
		auto result = make_self<SpectrumData>(Size(), FrequencyCount(), AmplitudeScale(), FrequencyScale(), MinFrequency(), MaxFrequency(), false);
		DirectX::XMVECTOR *pLastData = (previous) ? winrt::from_abi<SpectrumData>(previous)->_pData : nullptr;

		float normalizedRiseTime = (float)timeFromPrevious.count() / (float)riseTime.count();
		float normalizedFallTime = (float)timeFromPrevious.count() / (float)fallTime.count();

		AudioMath::ApplyRiseAndFall(pLastData, _pData, result->_pData, _vElementsCount * _channels, normalizedRiseTime, normalizedFallTime);
		
		return result.as<AudioVisualizer::SpectrumData>();
    }

	AudioVisualizer::SpectrumData SpectrumData::ApplyRiseAndFallToEmpty(AudioVisualizer::SpectrumData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
	{
		if (!previous)
			throw hresult_invalid_argument();
		if (previous.AmplitudeScale() != ScaleType::Linear)
			throw hresult_invalid_argument();

		auto result = make_self<SpectrumData>(previous.Size(), previous.FrequencyCount(), previous.AmplitudeScale(), previous.FrequencyScale(), previous.MinFrequency(), previous.MaxFrequency(), false);

		size_t vSize = (previous.FrequencyCount() + 3) >> 2;

		float normalizedRiseTime = (float)timeFromPrevious.count() / (float)riseTime.count();
		float normalizedFallTime = (float)timeFromPrevious.count() / (float)fallTime.count();

		AudioMath::ApplyRiseAndFall(winrt::from_abi<SpectrumData>(previous)->_pData, nullptr, result->_pData, vSize * previous.Size(), normalizedRiseTime, normalizedFallTime);
		
		return result.as<AudioVisualizer::SpectrumData>();
	}
    AudioVisualizer::SpectrumData SpectrumData::ConvertToDecibels(float minValue, float maxValue)
    {
		if (_amplitudeScale == ScaleType::Logarithmic)
			throw hresult_error(E_NOT_VALID_STATE);

		if (minValue >= maxValue)
			throw hresult_invalid_argument();

		auto result = make_self<SpectrumData>(_channels, _size, ScaleType::Logarithmic, _frequencyScale, _minimumFrequency, _maximumFrequency, false);
		AudioMath::ConvertToLogarithmic(_pData, result->_pData, _vElementsCount * _channels, minValue, maxValue);
		return result.as<AudioVisualizer::SpectrumData>();
    }

    AudioVisualizer::SpectrumData SpectrumData::CombineChannels(array_view<float const> map)
    {
		if (map.data() == nullptr)
			throw hresult_error(E_POINTER);

		if (map.size() < _channels)
			throw hresult_invalid_argument();

		if (_amplitudeScale != ScaleType::Linear)
			throw hresult_error(E_NOT_VALID_STATE);

		UINT32 outputChannels = map.size() / _channels;
		auto result = make_self<SpectrumData>(
			outputChannels,
			_size,
			_amplitudeScale,
			_frequencyScale,
			_minimumFrequency,
			_maximumFrequency,
			false);

		std::vector<DirectX::XMVECTOR *> src(_channels);
		DirectX::XMVECTOR *pData = _pData;
		for (size_t channelIndex = 0; channelIndex < _channels; channelIndex++, pData += _vElementsCount)
		{
			src[channelIndex] = pData;
		}

		DirectX::XMVECTOR *pDest = result->_pData;
		for (size_t channelIndex = 0; channelIndex < outputChannels; channelIndex++, pDest += _vElementsCount)
		{
			AudioMath::CombineChannels(src.data(), _channels, _vElementsCount, ((float *) map.data()) + channelIndex * _channels, pDest);
		}

		return result.as<AudioVisualizer::SpectrumData>();

    }

    float SpectrumData::GetFrequency(uint32_t elementIndex)
    {
		return _frequencyScale == ScaleType::Linear ?
			_minimumFrequency + _frequencyStep * elementIndex :
			_minimumFrequency * powf(_frequencyStep, (float)elementIndex);
    }

    float SpectrumData::GetCenterFrequency(uint32_t elementIndex)
    {
		return _frequencyScale == ScaleType::Linear ?
			_minimumFrequency + _frequencyStep * ((float)elementIndex + 0.5f):
			_minimumFrequency * powf(_frequencyStep, (float)elementIndex + 0.5f);
    }

    Windows::Foundation::Collections::IIterator<AudioVisualizer::VectorData> SpectrumData::First()
    {
		return make<VectorDataIterator>(*this).as<Windows::Foundation::Collections::IIterator<AudioVisualizer::VectorData>>();
    }

	AudioVisualizer::VectorData SpectrumData::GetAt(uint32_t index)
    {
		if (index >= _channels)
			throw hresult_invalid_argument();
		return make<VectorData>((float *)(_pData + index *_vElementsCount), _size, *this);
    }

    uint32_t SpectrumData::Size()
    {
		return _channels;
    }

    bool SpectrumData::IndexOf(AudioVisualizer::VectorData const& value, uint32_t& index)
    {
		throw hresult_not_implemented();
    }

    uint32_t SpectrumData::GetMany(uint32_t startIndex, array_view<AudioVisualizer::VectorData> items)
    {
		throw hresult_not_implemented();
    }

    AudioVisualizer::SpectrumData SpectrumData::CreateEmpty(uint32_t cChannels, uint32_t cElements, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency)
    {
		if (cChannels == 0 || cElements == 0)
			throw hresult_invalid_argument();
		if (maxFrequency <= minFrequency || maxFrequency <= 0 || minFrequency < 0)
			throw hresult_invalid_argument();
		if (frequencyScale == ScaleType::Logarithmic && minFrequency <= 0)
				throw hresult_invalid_argument();

		return make<SpectrumData>(cChannels, cElements, amplitudeScale, frequencyScale, minFrequency, maxFrequency, true);
    }

    AudioVisualizer::SpectrumData SpectrumData::Create(Windows::Foundation::Collections::IVectorView<Windows::Foundation::Collections::IVectorView<float>> const& values, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency)
    {
		if (values == nullptr)
			throw hresult_error(E_POINTER);

		if (maxFrequency <= minFrequency || maxFrequency <= 0 || minFrequency < 0)
			throw hresult_invalid_argument();

		if (frequencyScale == ScaleType::Logarithmic && minFrequency <= 0)
			throw hresult_invalid_argument();

		uint32_t cChannels = values.Size();

		if (cChannels == 0)
			throw hresult_invalid_argument();

		uint32_t cElements = values.GetAt(0).Size();

		for (size_t channelIndex = 1; channelIndex < cChannels; channelIndex++)
		{
			if (values.GetAt(channelIndex).Size() != cElements) {
				throw hresult_invalid_argument(hstring(L"Need to have equal number of elements per channel"));	// Need to have same number of elements per channel
			}
		}

		auto result = make_self<SpectrumData>(cChannels, cElements, amplitudeScale, frequencyScale, minFrequency, maxFrequency, false);
		DirectX::XMVECTOR *pData = result->_pData;

		for (size_t channelIndex = 0; channelIndex < cChannels; channelIndex++,pData+=result->_vElementsCount)
		{
			auto sourceData = values.GetAt(channelIndex);
			for (size_t index = 0; index < sourceData.Size(); index++)
			{
				((float*)pData)[index] = sourceData.GetAt(index);
			}
		}
		return result.as<AudioVisualizer::SpectrumData>();
    }

	SpectrumData::SpectrumData(uint32_t cChannels, uint32_t cElements, AudioVisualizer::ScaleType const & amplitudeScale, AudioVisualizer::ScaleType const & frequencyScale, float minFrequency, float maxFrequency, bool bInitWithZeros)
	{
		_pData = nullptr;
		_amplitudeScale = amplitudeScale;
		_frequencyScale = frequencyScale;
		_minimumFrequency = minFrequency;
		_maximumFrequency = maxFrequency;
		_frequencyStep = _frequencyScale == ScaleType::Linear ?
			(maxFrequency - minFrequency) / cElements :
			powf(maxFrequency / minFrequency, 1.0f / cElements);

		_vElementsCount = (cElements + 3) >> 2;	// Make sure channel data is aligned
		_size = cElements;
		_channels = cChannels;
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc_dbg(_vElementsCount * cChannels * sizeof(DirectX::XMVECTOR), 16,__FILE__,__LINE__));
		if (!_pData) {
			throw std::bad_alloc();
		}
		if (bInitWithZeros)
		{
			memset(_pData, 0, _vElementsCount * cChannels * sizeof(DirectX::XMVECTOR));
		}
	}

	SpectrumData::~SpectrumData()
	{
		if (_pData != nullptr)
			_aligned_free_dbg(_pData);
		_pData = nullptr;
	}


}
