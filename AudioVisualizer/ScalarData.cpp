#include "pch.h"
#include "ScalarData.h"

namespace winrt::AudioVisualizer::implementation
{
    AudioVisualizer::ScaleType ScalarData::AmplitudeScale()
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::ConvertToDecibels(float minValue, float maxValue)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFall(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::CreateEmpty(uint32_t channels)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::Create(array_view<float const> values)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFallToEmpty(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
        throw hresult_not_implemented();
    }
}
