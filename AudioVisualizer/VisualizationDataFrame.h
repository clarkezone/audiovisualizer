#pragma once

#include "VisualizationDataFrame.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct VisualizationDataFrame : VisualizationDataFrameT<VisualizationDataFrame>
    {
		Windows::Foundation::TimeSpan _time;
		Windows::Foundation::TimeSpan _duration;
		ScalarData _rms{ nullptr };
		ScalarData _peak{ nullptr };
		SpectrumData _spectrum{ nullptr };

        VisualizationDataFrame() = delete;
		VisualizationDataFrame(nullptr_t = nullptr) noexcept {};
        VisualizationDataFrame(Windows::Foundation::TimeSpan const& time, Windows::Foundation::TimeSpan const& duration, AudioVisualizer::ScalarData const& rms, AudioVisualizer::ScalarData const& peak, AudioVisualizer::SpectrumData const& spectrum);

        Windows::Foundation::TimeSpan Time();
        Windows::Foundation::TimeSpan Duration();
        AudioVisualizer::ScalarData RMS();
        AudioVisualizer::ScalarData Peak();
        AudioVisualizer::SpectrumData Spectrum();

		bool IsBefore(Windows::Foundation::TimeSpan time) {
			return _time < time;
		}
		bool IsAfter(Windows::Foundation::TimeSpan time) {
			return time.count() >= _time.count() + _duration.count() + 50;	// Add 5us to avoid any integer time math rounding errors
		}
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct VisualizationDataFrame : VisualizationDataFrameT<VisualizationDataFrame, implementation::VisualizationDataFrame>
    {
    };
}
