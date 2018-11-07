#pragma once

#include "SpectrumData.g.h"
#include "VectorData.h"

namespace winrt::AudioVisualizer::implementation
{

    struct SpectrumData : SpectrumDataT<SpectrumData>
    {
		ScaleType _amplitudeScale;
		ScaleType _frequencyScale;
		float _minimumFrequency;
		float _maximumFrequency;
		float _frequencyStep;

        DirectX::XMVECTOR *_pData = nullptr;
		uint32_t _vElementsCount = 0;	// XMVector elements count per channel
		uint32_t _size = 0;	// Elements count per channel
		uint32_t _channels = 0;
		//Windows::Foundation::Collections::IVectorView<AudioVisualizer::VectorData> _view;

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

		Windows::Foundation::Collections::IIterator<AudioVisualizer::VectorData> First();
		AudioVisualizer::VectorData GetAt(uint32_t index);
		uint32_t Size();
		bool IndexOf(AudioVisualizer::VectorData const& value, uint32_t& index);
		uint32_t GetMany(uint32_t startIndex, array_view<AudioVisualizer::VectorData> items);

        static AudioVisualizer::SpectrumData CreateEmpty(uint32_t cChannels, uint32_t cElements, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency);
        static AudioVisualizer::SpectrumData Create(array_view<float const> values, uint32_t cChannels, AudioVisualizer::ScaleType const& amplitudeScale, AudioVisualizer::ScaleType const& frequencyScale, float minFrequency, float maxFrequency);
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
