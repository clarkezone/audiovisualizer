#pragma once

#include "VisualizerAudioEffect.g.h"
#include "AudioAnalyzer.h"

namespace winrt::AudioVisualizer::implementation
{
    struct VisualizerAudioEffect : VisualizerAudioEffectT<VisualizerAudioEffect>
    {
		AudioVisualizer::AudioAnalyzer _analyzer{ nullptr };
		Windows::Media::MediaProperties::AudioEncodingProperties _encoding{ nullptr };
		float _fOutputFps;
		float _fOverlap;
		uint32_t _fftLength;
		winrt::event<Windows::Foundation::TypedEventHandler<IVisualizationSource, hstring>> _configurationChangedEvent;

		AnalyzerType _analyzerTypes;
		void CreateAnalyzer();

        VisualizerAudioEffect();
		bool UseInputFrameForOutput();
		Windows::Foundation::Collections::IVectorView<Windows::Media::MediaProperties::AudioEncodingProperties> SupportedEncodingProperties();
		void SetEncodingProperties(Windows::Media::MediaProperties::AudioEncodingProperties const& encodingProperties);
		void ProcessFrame(Windows::Media::Effects::ProcessAudioFrameContext const& context);
		void Close(Windows::Media::Effects::MediaEffectClosedReason const& reason);
		void DiscardQueuedFrames();
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
		void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
		void ConfigureSpectrum(uint32_t fftLength, float overlap);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
	struct VisualizerAudioEffect : VisualizerAudioEffectT<VisualizerAudioEffect, implementation::VisualizerAudioEffect>
	{
	};
}
