#include "pch.h"
#include "SpectrumData.h"
#include "ScalarData.h"


namespace winrt::AudioVisualizer::implementation
{
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
        throw hresult_not_implemented();
    }

    AudioVisualizer::SpectrumData SpectrumData::LogarithmicTransform(uint32_t elementCount, float fromFrequency, float toFrequency)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::SpectrumData SpectrumData::ApplyRiseAndFall(AudioVisualizer::SpectrumData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::SpectrumData SpectrumData::ConvertToDecibels(float minValue, float maxValue)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::SpectrumData SpectrumData::CombineChannels(array_view<float const> map)
    {
        throw hresult_not_implemented();
    }

    float SpectrumData::GetFrequency(uint32_t elementIndex)
    {
        throw hresult_not_implemented();
    }

    float SpectrumData::GetCenterFrequency(uint32_t elementIndex)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IVectorView<float>> SpectrumData::First()
    {
		return _view.First();
    }

    Windows::Foundation::Collections::IVectorView<float> SpectrumData::GetAt(uint32_t index)
    {
		return _view.GetAt(index);
    }

    uint32_t SpectrumData::Size()
    {
		return _view.Size();
    }

    bool SpectrumData::IndexOf(Windows::Foundation::Collections::IVectorView<float> const& value, uint32_t& index)
    {
		return _view.IndexOf(value, index);
    }

    uint32_t SpectrumData::GetMany(uint32_t startIndex, array_view<Windows::Foundation::Collections::IVectorView<float>> items)
    {
		return _view.GetMany(startIndex, items);
    }

    AudioVisualizer::SpectrumData SpectrumData::CreateEmpty(uint32_t cChannels, uint32_t cElements, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency)
    {
		if (cChannels == 0 || cElements == 0)
			throw hresult_invalid_argument();
		if (maxFrequency <= minFrequency || maxFrequency <= 0 || minFrequency < 0)
			throw hresult_invalid_argument();
		if (frequencyScale == ScaleType::Logarithmic && minFrequency <= 0)
				throw hresult_invalid_argument();

		float fStep = frequencyScale == ScaleType::Linear ?
			(maxFrequency - minFrequency) / cElements :
			powf(maxFrequency / minFrequency, 1.0f / cElements);

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

		float fStep = frequencyScale == ScaleType::Linear ?
			(maxFrequency - minFrequency) / cElements :
			powf(maxFrequency / minFrequency, 1.0f / cElements);

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

    AudioVisualizer::SpectrumData SpectrumData::ApplyRiseAndFallToEmpty(AudioVisualizer::SpectrumData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
        throw hresult_not_implemented();
    }

	SpectrumData::SpectrumData(uint32_t cChannels, uint32_t cElements, AudioVisualizer::ScaleType const & amplitudeScale, AudioVisualizer::ScaleType const & frequencyScale, float minFrequency, float maxFrequency, bool bInitWithZeros)
	{
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
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc(_vElementsCount * cChannels * sizeof(DirectX::XMVECTOR), 16));
		if (bInitWithZeros)
		{
			memset(_pData, 0, _vElementsCount * cChannels * sizeof(DirectX::XMVECTOR));
		}
		std::vector<Windows::Foundation::Collections::IVectorView<float>> values(_channels);

		for (size_t index = 0, vIndex = 0; index < _channels; index++, vIndex += _vElementsCount)
		{
			values[index] = make<SpectrumChannelValues>((float *)(_pData + vIndex), cElements).as<Windows::Foundation::Collections::IVectorView<float>>();
		}
		_view = single_threaded_vector(std::move(values)).GetView();
	}

	SpectrumData::~SpectrumData()
	{
		if (_pData != nullptr)
			_aligned_free(_pData);
		_pData = nullptr;
	}

	Windows::Foundation::Collections::IIterator<float> SpectrumChannelValues::First()
	{
		return make<VectorDataIterator>(_pData, _size);
	}

	float SpectrumChannelValues::GetAt(uint32_t index)
	{
		if (index >= _size)
			throw hresult_out_of_bounds();
		return _pData[index];
	}
	bool SpectrumChannelValues::IndexOf(float const & value, uint32_t & index)
	{
		throw hresult_not_implemented();
	}
	uint32_t SpectrumChannelValues::GetMany(uint32_t startIndex, array_view<float> items)
	{
		throw hresult_not_implemented();
	}
}
