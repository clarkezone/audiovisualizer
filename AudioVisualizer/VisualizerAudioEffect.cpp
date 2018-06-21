#include "pch.h"
#include "VisualizerAudioEffect.h"
#include <winrt/Windows.Media.Audio.h>

namespace winrt::AudioVisualizer::implementation
{
	VisualizerAudioEffect::VisualizerAudioEffect() :
		_analyzerTypes(AnalyzerType::All),
		_fOutputFps(60.0f),
		_fOverlap(0.5f),
		_fftLength(2048)
	{
		
	}

    bool VisualizerAudioEffect::UseInputFrameForOutput()
    {
		return true;
    }

	Windows::Foundation::Collections::IVectorView<Windows::Media::MediaProperties::AudioEncodingProperties> g_SupportedEncodings{ nullptr };

	Windows::Foundation::Collections::IVectorView<Windows::Media::MediaProperties::AudioEncodingProperties> VisualizerAudioEffect::SupportedEncodingProperties()
	{
		if (!g_SupportedEncodings) {
			std::vector<uint32_t> supportedSampleRates{ 8000,11025,16000,22050,32000,44100,48000,96000,192000};
			std::vector<uint32_t> supportedChannelCounts{ 1,2,5 };
			std::vector<Windows::Media::MediaProperties::AudioEncodingProperties> properties;
			for (uint32_t sampleRate : supportedSampleRates)
			{
				for (uint32_t channelCount : supportedChannelCounts)
				{
					auto encoding = Windows::Media::MediaProperties::AudioEncodingProperties::CreatePcm(sampleRate, channelCount, 32);
					encoding.Subtype(L"Float");
					properties.insert(std::end(properties),encoding);
				}
			}
			g_SupportedEncodings = single_threaded_vector(std::move(properties)).GetView();
		}
		return g_SupportedEncodings;
	}

	void VisualizerAudioEffect::SetEncodingProperties(Windows::Media::MediaProperties::AudioEncodingProperties const& encodingProperties)
	{
		if (!encodingProperties)
			throw hresult_invalid_argument();
		if (encodingProperties.Subtype() != L"Float" || encodingProperties.BitsPerSample() != 32)
			throw hresult_invalid_argument();

		_encoding = encodingProperties;
		CreateAnalyzer();
	}

	void VisualizerAudioEffect::ProcessFrame(Windows::Media::Effects::ProcessAudioFrameContext const& context)
	{
		throw hresult_not_implemented();
	}

	void VisualizerAudioEffect::Close(Windows::Media::Effects::MediaEffectClosedReason const& reason)
	{
		throw hresult_not_implemented();
	}

	void VisualizerAudioEffect::DiscardQueuedFrames()
	{
		throw hresult_not_implemented();
	}

	AudioVisualizer::VisualizationDataFrame VisualizerAudioEffect::GetData()
	{
		throw hresult_not_implemented();
	}

	bool VisualizerAudioEffect::IsSuspended()
	{
		throw hresult_not_implemented();
	}

	void VisualizerAudioEffect::IsSuspended(bool value)
	{
		throw hresult_not_implemented();
	}

	float VisualizerAudioEffect::Fps()
	{
		return _fOutputFps;
	}

	void VisualizerAudioEffect::Fps(float value)
	{
		throw hresult_not_implemented();
	}

	AudioVisualizer::AnalyzerType VisualizerAudioEffect::AnalyzerTypes()
	{
		return _analyzerTypes;
	}

	void VisualizerAudioEffect::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
	{
		_analyzerTypes = value;
		if (_analyzer) {
			_analyzer.AnalyzerTypes(value);
		}
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> VisualizerAudioEffect::PresentationTime()
	{
		throw hresult_not_implemented();
	}

	AudioVisualizer::SourcePlaybackState VisualizerAudioEffect::PlaybackState()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IReference<uint32_t> VisualizerAudioEffect::ActualFrequencyCount()
	{
		if (_analyzer) {
			return _analyzer.SpectrumElementCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<uint32_t> VisualizerAudioEffect::ActualChannelCount()
	{
		if (_encoding) {
			return _encoding.ChannelCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<float> VisualizerAudioEffect::ActualMinFrequency()
	{
		return 0.0f;
	}

	Windows::Foundation::IReference<float> VisualizerAudioEffect::ActualMaxFrequency()
	{
		if (_analyzer) {
			return _analyzer.SpectrumStep() * _analyzer.SpectrumElementCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> VisualizerAudioEffect::ActualFrequencyScale()
	{
		return ScaleType::Linear;
	}

	event_token VisualizerAudioEffect::ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler)
	{
		return _configurationChangedEvent.add(handler);
	}

	void VisualizerAudioEffect::ConfigurationChanged(event_token const& token)
	{
		_configurationChangedEvent.remove(token);
	}

    void VisualizerAudioEffect::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
		configuration.Insert(L"Source", *this);
		configuration.Insert(L"Type", winrt::box_value(name_of<AudioVisualizer::VisualizerAudioEffect>().data()));
	}

	void VisualizerAudioEffect::ConfigureSpectrum(uint32_t fftLength, float overlap)
	{
		if ((fftLength & fftLength - 1) != 0 || fftLength  < 256)	// FFT length needs to be power of 2
			throw hresult_invalid_argument();

		if (overlap < 0.0f || overlap > 1.0f)	// Set some sensible overlap limits
			throw hresult_invalid_argument();

		_fftLength = fftLength;
		_fOverlap = overlap;

		// If input type is set then calculate the necessary variables and initialize
		if (_encoding) {
			CreateAnalyzer();
		}
		_configurationChangedEvent(*this, hstring(L""));
	}

	void VisualizerAudioEffect::CreateAnalyzer()
	{
		uint32_t analyzerStep = (uint32_t)(_encoding.SampleRate() / _fOutputFps);
		uint32_t overlapSamples = (uint32_t)(analyzerStep * _fOverlap);
		_analyzer = AudioVisualizer::AudioAnalyzer(analyzerStep + overlapSamples, _encoding.ChannelCount(), _encoding.SampleRate(), analyzerStep, overlapSamples, _fftLength, true);
	}
	
}
