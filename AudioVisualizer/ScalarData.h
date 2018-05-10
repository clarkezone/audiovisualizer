#pragma once

#include "ScalarData.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct ScalarData : ScalarDataT<ScalarData>
    {
        ScalarData() = delete;

        AudioVisualizer::ScaleType AmplitudeScale();
        AudioVisualizer::ScalarData ConvertToDecibels(float minValue, float maxValue);
        AudioVisualizer::ScalarData ApplyRiseAndFall(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);

        static AudioVisualizer::ScalarData CreateEmpty(uint32_t channels);
        static AudioVisualizer::ScalarData Create(array_view<float const> values);
        static AudioVisualizer::ScalarData ApplyRiseAndFallToEmpty(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct ScalarData : ScalarDataT<ScalarData, implementation::ScalarData>
    {
    };
}
