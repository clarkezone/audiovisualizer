#include "pch.h"
#include "VisualizationDataFrame.h"

namespace winrt::AudioVisualizer::implementation
{
    VisualizationDataFrame::VisualizationDataFrame(Windows::Foundation::TimeSpan const& time, Windows::Foundation::TimeSpan const& duration, AudioVisualizer::ScalarData const& rms, AudioVisualizer::ScalarData const& peak, AudioVisualizer::SpectrumData const& spectrum) : _rms(rms), _peak(peak),_spectrum(spectrum)
    {
		_time = time;
		_duration = duration;
    }

    Windows::Foundation::TimeSpan VisualizationDataFrame::Time()
    {
		return _time;
    }

    Windows::Foundation::TimeSpan VisualizationDataFrame::Duration()
    {
		return _duration;
    }

    AudioVisualizer::ScalarData VisualizationDataFrame::RMS()
    {
		return _rms;
    }

    AudioVisualizer::ScalarData VisualizationDataFrame::Peak()
    {
		return _peak;
	}

    AudioVisualizer::SpectrumData VisualizationDataFrame::Spectrum()
    {
		return _spectrum;
    }
}
