#pragma once
#include "LoopbackVisualizationSource.g.h"
#include <AudioClient.h>
#include "mfengine.h"
#include "AudioAnalyzer.h"

namespace winrt::AudioVisualizer::implementation
{
    struct LoopbackVisualizationSource : LoopbackVisualizationSourceT<LoopbackVisualizationSource,::IMFAsyncCallback>
    {
		MFEngine __engine;	// Automatic MF engine init and shutdown
		winrt::com_ptr<::IAudioClient3> _audioClient;
		winrt::com_ptr<::IAudioCaptureClient> _captureClient;
		winrt::com_ptr<::IMFAsyncResult> _asyncResult;
		winrt::handle _callbackEvent;
		DWORD _workQueueId{ 0 };
		DWORD _workQueueTaskId{ 0 };
		uint32_t _inputSampleRate{ 0 };
		uint32_t _inputChannels{ 0 };
		AudioVisualizer::AudioAnalyzer _analyzer{ nullptr };
		bool _suspended { true };
		MFWORKITEM_KEY _workItemKey{ 0 };
		winrt::event_token _analyzerOutputEvent;
		AudioVisualizer::VisualizationDataFrame _outputFrame{ nullptr };
		std::mutex _outputFrameLock;
		Windows::Foundation::TimeSpan _streamPosition{ 0 };

		winrt::event<Windows::Foundation::TypedEventHandler<IVisualizationSource, hstring>> _configurationChangedEvent;

		void InitWorkQueue();
		void InitAnalyzer(uint32_t fftLength = 1024u, float overlap = 0.25f);
		void ScheduleWorkItem();
		void CancelWorkItem();

		void OnAnalyzerOutput(AudioVisualizer::AudioAnalyzer const & sender, AudioVisualizer::VisualizationDataFrame const &frame);

		LoopbackVisualizationSource(winrt::com_ptr<::IAudioClient3> const& audioClient);

        static Windows::Foundation::IAsyncOperation<AudioVisualizer::LoopbackVisualizationSource> CreateAsync();
        static Windows::Foundation::IAsyncOperation<AudioVisualizer::LoopbackVisualizationSource> CreateAsync(Windows::Devices::Enumeration::DeviceInformation renderDevice);
        
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
		Windows::Foundation::IReference<float> ActualFrequencyStep();
		Windows::Foundation::IReference<AudioVisualizer::ScaleType> ActualFrequencyScale();
        winrt::event_token ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler);
        void ConfigurationChanged(winrt::event_token const& token) noexcept;
		void ConfigureSpectrum(uint32_t fftLength, float overlap);

		void Start();
		void Stop();

		virtual HRESULT STDMETHODCALLTYPE GetParameters(DWORD* pdwFlags,DWORD* pdwQueue);
		virtual HRESULT STDMETHODCALLTYPE Invoke(IMFAsyncResult* pAsyncResult);
    };
}
namespace winrt::AudioVisualizer::factory_implementation
{
    struct LoopbackVisualizationSource : LoopbackVisualizationSourceT<LoopbackVisualizationSource, implementation::LoopbackVisualizationSource>
    {
    };
}
