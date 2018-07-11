#pragma once

#include "VisualizationDataFrame.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct VisualizationDataFrame : VisualizationDataFrameT<VisualizationDataFrame>
    {
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _time;
		Windows::Foundation::Collections::PropertySet _properties;
		Windows::Foundation::TimeSpan _duration;
		ScalarData _rms{ nullptr };
		ScalarData _peak{ nullptr };
		SpectrumData _spectrum{ nullptr };

        VisualizationDataFrame() = delete;
		VisualizationDataFrame(nullptr_t = nullptr) noexcept {};
        VisualizationDataFrame(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& time, Windows::Foundation::TimeSpan const& duration, AudioVisualizer::ScalarData const& rms, AudioVisualizer::ScalarData const& peak, AudioVisualizer::SpectrumData const& spectrum);

        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> Time();
        Windows::Foundation::TimeSpan Duration();
        AudioVisualizer::ScalarData RMS();
        AudioVisualizer::ScalarData Peak();
        AudioVisualizer::SpectrumData Spectrum();
		Windows::Foundation::Collections::PropertySet Properties()
		{
			return _properties;
		}
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct VisualizationDataFrame : VisualizationDataFrameT<VisualizationDataFrame, implementation::VisualizationDataFrame>
    {
    };
}
