#pragma once

#include "VisualizationDataFrame.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct VisualizationDataFrame : VisualizationDataFrameT<VisualizationDataFrame>
    {
		Windows::Foundation::TimeSpan _time;
		Windows::Foundation::TimeSpan _duration;
		ScalarData _rms;
		ScalarData _peak;
		SpectrumData _spectrum;

        VisualizationDataFrame() = delete;
        VisualizationDataFrame(Windows::Foundation::TimeSpan const& time, Windows::Foundation::TimeSpan const& duration, AudioVisualizer::ScalarData const& rms, AudioVisualizer::ScalarData const& peak, AudioVisualizer::SpectrumData const& spectrum);

        Windows::Foundation::TimeSpan Time();
        Windows::Foundation::TimeSpan Duration();
        AudioVisualizer::ScalarData RMS();
        AudioVisualizer::ScalarData Peak();
        AudioVisualizer::SpectrumData Spectrum();

		bool IsBefore(REFERENCE_TIME time) {
			return _time.count() < time;
		}
		bool IsAfter(REFERENCE_TIME time) {
			return time >= _time.count() + _duration.count() + 50;	// Add 5us to avoid any integer time math rounding errors
		}
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct VisualizationDataFrame : VisualizationDataFrameT<VisualizationDataFrame, implementation::VisualizationDataFrame>
    {
    };
}
