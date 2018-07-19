#pragma once

#include "ScalarData.g.h"
#include <windows.foundation.h>
#include "ScalarValuesIterator.h"

namespace winrt::AudioVisualizer::implementation
{
    struct ScalarData : ScalarDataT<ScalarData>
    {
        ScalarData() = delete;
		ScalarData(std::nullptr_t = nullptr) noexcept {}

        AudioVisualizer::ScaleType AmplitudeScale();
        AudioVisualizer::ScalarData ConvertToDecibels(float minValue, float maxValue);
        AudioVisualizer::ScalarData ApplyRiseAndFall(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);
		AudioVisualizer::ScalarData CombineChannels(array_view<float const> map);

        Windows::Foundation::Collections::IIterator<float> First();
        float GetAt(uint32_t index);
		uint32_t Size() { return (uint32_t) _size; }
        bool IndexOf(float const& value, uint32_t& index);
        uint32_t GetMany(uint32_t startIndex, array_view<float> items);

        static AudioVisualizer::ScalarData CreateEmpty(uint32_t channels);
        static AudioVisualizer::ScalarData Create(array_view<float const> values);
        static AudioVisualizer::ScalarData ApplyRiseAndFallToEmpty(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);
    
		ScaleType _amplitudeScale;
		DirectX::XMVECTOR *_pData;
		uint32_t _size;

		ScalarData(uint32_t cElements, ScaleType scaleType = ScaleType::Linear,bool bInitToZero = true);
		ScalarData(array_view<float const> values, ScaleType scaleType = ScaleType::Linear);
		~ScalarData();
	};
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct ScalarData : ScalarDataT<ScalarData, implementation::ScalarData>
    {
    };
}
