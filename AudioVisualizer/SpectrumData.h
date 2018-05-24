#pragma once

#include "SpectrumData.g.h"

namespace winrt::AudioVisualizer::implementation
{
	struct SpectrumChannelValues : implements<SpectrumChannelValues,Windows::Foundation::Collections::IVectorView<float>, Windows::Foundation::Collections::IIterable<float> >
	{
		uint32_t _size;
		float *_pData;
		SpectrumChannelValues(float *pData, uint32_t cElements)
		{
			_size = cElements;
			_pData = pData;
		}

		Windows::Foundation::Collections::IIterator<float> First();
		float GetAt(uint32_t index);
		uint32_t Size() { return _size; }
		bool IndexOf(float const& value, uint32_t& index);
		uint32_t GetMany(uint32_t startIndex, array_view<float> items);
	};

    struct SpectrumData : SpectrumDataT<SpectrumData>
    {
		ScaleType _amplitudeScale;
		ScaleType _frequencyScale;
		float _minimumFrequency;
		float _maximumFrequency;
		float _frequencyStep;

        DirectX::XMVECTOR *_pData = nullptr;
		size_t _vElementsCount = 0;	// XMVector elements count per channel
		size_t _size = 0;	// Elements count per channel
		size_t _channels = 0;
		Windows::Foundation::Collections::IVectorView<Windows::Foundation::Collections::IVectorView<float>> _view;

        SpectrumData() = delete;
		SpectrumData(nullptr_t = nullptr) noexcept {};

        AudioVisualizer::ScaleType AmplitudeScale();
        AudioVisualizer::ScaleType FrequencyScale();
        float MinFrequency();
        float MaxFrequency();
        float FrequencyStep();
        uint32_t FrequencyCount();
        AudioVisualizer::SpectrumData LinearTransform(uint32_t elementCount, float fromFrequency, float toFrequency);
        AudioVisualizer::SpectrumData LogarithmicTransform(uint32_t elementCount, float fromFrequency, float toFrequency);
        AudioVisualizer::SpectrumData ApplyRiseAndFall(AudioVisualizer::SpectrumData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);
        AudioVisualizer::SpectrumData ConvertToDecibels(float minValue, float maxValue);
        AudioVisualizer::SpectrumData CombineChannels(array_view<float const> map);
        float GetFrequency(uint32_t elementIndex);
        float GetCenterFrequency(uint32_t elementIndex);
        Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IVectorView<float>> First();
        Windows::Foundation::Collections::IVectorView<float> GetAt(uint32_t index);
        uint32_t Size();
        bool IndexOf(Windows::Foundation::Collections::IVectorView<float> const& value, uint32_t& index);
        uint32_t GetMany(uint32_t startIndex, array_view<Windows::Foundation::Collections::IVectorView<float>> items);

        static AudioVisualizer::SpectrumData CreateEmpty(uint32_t cChannels, uint32_t cElements, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency);
        static AudioVisualizer::SpectrumData Create(Windows::Foundation::Collections::IVectorView<Windows::Foundation::Collections::IVectorView<float>> const& values, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency);
        static AudioVisualizer::SpectrumData ApplyRiseAndFallToEmpty(AudioVisualizer::SpectrumData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);
    
		SpectrumData(uint32_t cChannels, uint32_t cElements, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency,bool bInitWithZeros=true);
		~SpectrumData();
		
	};
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct SpectrumData : SpectrumDataT<SpectrumData, implementation::SpectrumData>
    {
    };
}
