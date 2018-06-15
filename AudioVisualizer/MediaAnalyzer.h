#pragma once

#include "MediaAnalyzer.g.h"
#include <memory>
#include <queue>
#include <mutex>
#include "VisualizationDataFrame.h"
#include <windows.media.core.interop.h>
#include "AudioAnalyzer.h"
#include "Tracing.h"
#include <winrt/Windows.Foundation.Diagnostics.h>

namespace winrt::AudioVisualizer::implementation
{
    struct MediaAnalyzer : MediaAnalyzerT<MediaAnalyzer,IMFTransform, IMFClockConsumer,IMFClockStateSink>
    {
		class dataframe_queue
		{
		private:
			std::mutex _outputQueueAccessMutex;
			std::queue<AudioVisualizer::VisualizationDataFrame> _data;
			size_t _maxQueueSize;
			void compact();
		public:
			dataframe_queue(size_t maxQueueSize) : _maxQueueSize(maxQueueSize) {}
			void add(AudioVisualizer::VisualizationDataFrame const &frame);
			void clear();
			AudioVisualizer::VisualizationDataFrame get(Windows::Foundation::TimeSpan time);
		};

#pragma region IMFTransform variables
		com_ptr<IMFMediaType> m_spOutputType;
		com_ptr<IMFMediaType> m_spInputType;
		com_ptr<IMFAttributes> m_spMftAttributes;
		com_ptr<IMFSample> m_spSample;

		std::mutex _mtxMftAccess;	// Nutex to avoid multiple threads modifying this MFT at the same time
#pragma endregion

#pragma region Analyzer variables and methods
		UINT32 m_FramesPerSecond;
		UINT32 m_nChannels;

		const size_t cMaxOutputQueueSize = 600;	// Keep 10sec worth of data for 60fps output

		AudioVisualizer::AudioAnalyzer _analyzer{ nullptr };
		winrt::event_token _analyzerOutputEvent;
		com_ptr<IAudioFrameNativeFactory> _audioFrameFactory;
		dataframe_queue _analyzerOutput;

		size_t _fftLength;
		float _fOutputFps;
		float _fInputOverlap;

		AnalyzerType  _analyzerTypes;

		SourcePlaybackState _playbackState;

		winrt::event<Windows::Foundation::TypedEventHandler<IVisualizationSource,hstring>> _configurationChangedEvent;

		void CreateAnalyzer();
		Windows::Media::AudioFrame ConvertToAudioFrame(IMFSample *pSample);
		void OnAnalyzerOutput(AudioVisualizer::AudioAnalyzer analyzer, AudioVisualizer::VisualizationDataFrame frame);

		inline long time_to_samples(REFERENCE_TIME time) const { return m_nChannels * (long)((time * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline long time_to_frames(REFERENCE_TIME time) const { return (long)((time * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline long time_to_frames(float time) const { return (long)((((REFERENCE_TIME)(1e7 * time))  * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline REFERENCE_TIME frames_to_time(long frames) { return 10000000L * (long long)frames / m_FramesPerSecond; }
		inline REFERENCE_TIME samples_to_time(long samples) { return 10000000L * (long long)(samples / m_nChannels) / m_FramesPerSecond; }
#pragma endregion


#pragma region IMFTransform implementation
		STDMETHODIMP GetStreamLimits(DWORD *pdwInputMinimum, DWORD *pdwInputMaximum, DWORD *pdwOutputMinimum, DWORD *pdwOutputMaximum);
		STDMETHODIMP GetStreamCount(DWORD *pcInputStreams, DWORD *pcOutputStreams);
		STDMETHODIMP GetStreamIDs(DWORD dwInputIDArraySize, DWORD *pdwInputIDs, DWORD dwOutputIDArraySize, DWORD *pdwOutputIDs);
		STDMETHODIMP GetInputStreamInfo(DWORD dwInputStreamID, MFT_INPUT_STREAM_INFO *pStreamInfo);
		STDMETHODIMP GetOutputStreamInfo(DWORD dwOutputStreamID, MFT_OUTPUT_STREAM_INFO *  pStreamInfo);
		STDMETHODIMP GetAttributes(IMFAttributes** pAttributes);
		STDMETHODIMP GetInputStreamAttributes(DWORD dwInputStreamID, IMFAttributes **ppAttributes);
		STDMETHODIMP GetOutputStreamAttributes(DWORD dwOutputStreamID, IMFAttributes **ppAttributes);
		STDMETHODIMP DeleteInputStream(DWORD dwStreamID);
		STDMETHODIMP AddInputStreams(DWORD cStreams, DWORD *adwStreamIDs);
		STDMETHODIMP GetInputAvailableType(DWORD dwInputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType);
		STDMETHODIMP GetOutputAvailableType(DWORD  dwOutputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType);
		STDMETHODIMP SetInputType(DWORD dwInputStreamID, IMFMediaType *pType, DWORD dwFlags);
		STDMETHODIMP SetOutputType(DWORD dwOutputStreamID, IMFMediaType *pType, DWORD dwFlags);
		STDMETHODIMP GetInputCurrentType(DWORD dwInputStreamID, IMFMediaType **ppType);
		STDMETHODIMP GetOutputCurrentType(DWORD dwOutputStreamID, IMFMediaType **ppType);
		STDMETHODIMP GetInputStatus(DWORD dwInputStreamID, DWORD *pdwFlags);
		STDMETHODIMP GetOutputStatus(DWORD *pdwFlags);
		STDMETHODIMP SetOutputBounds(LONGLONG hnsLowerBound, LONGLONG hnsUpperBound);
		STDMETHODIMP ProcessEvent(DWORD dwInputStreamID, IMFMediaEvent *pEvent);
		STDMETHODIMP ProcessMessage(MFT_MESSAGE_TYPE eMessage, ULONG_PTR ulParam);
		STDMETHODIMP ProcessInput(DWORD dwInputStreamID, IMFSample *pSample, DWORD dwFlags);
		STDMETHODIMP ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER  *pOutputSamples, DWORD *pdwStatus);
#pragma endregion

#pragma region IMFClockConsumer implementation
		com_ptr<IMFPresentationClock> m_spPresentationClock;
		STDMETHODIMP SetPresentationClock(_In_opt_ IMFPresentationClock *pPresentationClock);
		STDMETHODIMP GetPresentationClock(_COM_Outptr_opt_result_maybenull_ IMFPresentationClock **)
		{
			return E_NOTIMPL;
		}
#pragma endregion

#pragma region IMFClockStateSink implementation
		virtual STDMETHODIMP OnClockStart(
			/* [in] hnsSystemTime */ MFTIME,
			/* [in] llClockStartOffset */ LONGLONG )
		{
			_playbackState = SourcePlaybackState::Playing;
			return S_OK;
		}

		virtual STDMETHODIMP OnClockStop(
			/* [in] hnsSystemTime */ MFTIME )
		{
			_playbackState = SourcePlaybackState::Stopped;
			return S_OK;
		}

		virtual STDMETHODIMP OnClockPause(
			/* [in] hnsSystemTime */ MFTIME )
		{
			_playbackState = SourcePlaybackState::Paused;
			return S_OK;
		}

		virtual STDMETHODIMP OnClockRestart(
			/* [in] hnsSystemTime */ MFTIME )
		{
			_playbackState = SourcePlaybackState::Playing;
			return S_OK;
		}

		virtual STDMETHODIMP OnClockSetRate(
			/* [in]  hnsSystemTime */ MFTIME ,
			/* [in] flRate */ float )
		{
			return S_OK;
		}
#pragma endregion


        MediaAnalyzer();
		~MediaAnalyzer();

        void ConfigureSpectrum(uint32_t fftLength, float overlap);
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
        event_token ConfigurationChanged(winrt::Windows::Foundation::TypedEventHandler<IVisualizationSource,hstring> const& handler);
        void ConfigurationChanged(event_token const& token);
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
	struct MediaAnalyzer : MediaAnalyzerT<MediaAnalyzer, implementation::MediaAnalyzer>
	{
	};
}
