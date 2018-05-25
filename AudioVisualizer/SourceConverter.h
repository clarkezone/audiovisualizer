#pragma once

#include "SourceConverter.g.h"
#include <concrt.h>

namespace winrt::AudioVisualizer::implementation
{
    struct SourceConverter : SourceConverterT<SourceConverter>
    {

		IVisualizationSource _source;
		AnalyzerType _analyzerTypes;
		Windows::Foundation::IReference<UINT32> _frequencyCount;
		Windows::Foundation::IReference<UINT32> _channelCount;
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _rmsRiseTime;
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _rmsFallTime;
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _peakRiseTime;
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _peakFallTime;
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _spectrumRiseTime;
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _spectrumFallTime;
		Windows::Foundation::IReference<float> _minFrequency;
		Windows::Foundation::IReference<float> _maxFrequency;
		Windows::Foundation::IReference<ScaleType> _frequencyScale;
		AudioVisualizer::VisualizationDataFrame _cachedSourceFrame{ nullptr };
		AudioVisualizer::VisualizationDataFrame _cachedOutputFrame{ nullptr };
		bool _bCacheData;

		AudioVisualizer::SpectrumData _previousSpectrum{ nullptr };
		AudioVisualizer::ScalarData _previousRMS{ nullptr };
		AudioVisualizer::ScalarData _previousPeak{ nullptr };

		concurrency::reader_writer_lock _lock;
		Windows::Foundation::TimeSpan _timeFromPrevious;
		 
		winrt::event<Windows::Foundation::TypedEventHandler<IVisualizationSource, hstring>> _configurationChangedEvent;
		winrt::event_token _sourceChanged;

		AudioVisualizer::VisualizationDataFrame CloneFrame(AudioVisualizer::VisualizationDataFrame const &source);
		AudioVisualizer::VisualizationDataFrame ProcessFrame(AudioVisualizer::VisualizationDataFrame const& source);

		AudioVisualizer::SpectrumData ApplyFrequencyTransforms(AudioVisualizer::SpectrumData source);

		AudioVisualizer::SpectrumData ApplyRiseAndFall(AudioVisualizer::SpectrumData data, AudioVisualizer::SpectrumData previous, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> riseTime, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> fallTime);

		AudioVisualizer::ScalarData ApplyRiseAndFall(AudioVisualizer::ScalarData data, AudioVisualizer::ScalarData previous, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> riseTime, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> fallTime);

		Windows::Foundation::IReference<uint32_t> get_ActualFrequencyCount();
		Windows::Foundation::IReference<uint32_t> get_ActualChannelCount();
		Windows::Foundation::IReference<float> get_ActualMinFrequency();
		Windows::Foundation::IReference<float> get_ActualMaxFrequency();
		Windows::Foundation::IReference<AudioVisualizer::ScaleType> get_ActualFrequencyScale();

        SourceConverter();

        AudioVisualizer::IVisualizationSource Source();
        void Source(AudioVisualizer::IVisualizationSource const& value);
        Windows::Foundation::IReference<uint32_t> FrequencyCount();
        void FrequencyCount(Windows::Foundation::IReference<uint32_t> const& value);
        Windows::Foundation::IReference<uint32_t> ChannelCount();
        void ChannelCount(Windows::Foundation::IReference<uint32_t> const& value);
        com_array<float> ChannelMapping();
        void ChannelMapping(array_view<float const> value);
        Windows::Foundation::IReference<float> MinFrequency();
        void MinFrequency(Windows::Foundation::IReference<float> const& value);
        Windows::Foundation::IReference<float> MaxFrequency();
        void MaxFrequency(Windows::Foundation::IReference<float> const& value);
        Windows::Foundation::IReference<AudioVisualizer::ScaleType> FrequencyScale();
        void FrequencyScale(Windows::Foundation::IReference<AudioVisualizer::ScaleType> const& value);
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> RmsRiseTime();
        void RmsRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value);
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> RmsFallTime();
        void RmsFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value);
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> PeakRiseTime();
        void PeakRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value);
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> PeakFallTime();
        void PeakFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value);
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SpectrumRiseTime();
        void SpectrumRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value);
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SpectrumFallTime();
        void SpectrumFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value);
        AudioVisualizer::VisualizationDataFrame GetData();
        bool IsSuspended();
        void IsSuspended(bool value);
        float Fps();
        void Fps(float value);
        AudioVisualizer::AnalyzerType AnalyzerTypes();
        void AnalyzerTypes(AudioVisualizer::AnalyzerType const& value);
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> PresentationTime();
        AudioVisualizer::SourcePlaybackState PlaybackState();
        Windows::Foundation::IReference<uint32_t> ActualFrequencyCount();
        Windows::Foundation::IReference<uint32_t> ActualChannelCount();
        Windows::Foundation::IReference<float> ActualMinFrequency();
        Windows::Foundation::IReference<float> ActualMaxFrequency();
        Windows::Foundation::IReference<AudioVisualizer::ScaleType> ActualFrequencyScale();
        event_token ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler);
        void ConfigurationChanged(event_token const& token);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct SourceConverter : SourceConverterT<SourceConverter, implementation::SourceConverter>
    {
    };
}
