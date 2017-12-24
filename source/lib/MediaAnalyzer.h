#pragma once

#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer.abi.h"
#include <DirectXMath.h>
#include <queue>
#include <memory>
#include <mutex>
#include <windows.system.threading.h>
#include <AudioAnalyzer.h>
#include "VisualizationDataFrame.h"
#include "wrl_util.h"

using namespace Microsoft::WRL;
using namespace ABI::Windows::System::Threading;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Foundation;

namespace AudioVisualizer
{
#define MFT_ANALYZER_PROPERTYSET_NAME L"Source"

	class MediaAnalyzer
		: public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
		ABI::Windows::Media::IMediaExtension,
		IMFTransform,
		IMFClockConsumer,
		IMFClockStateSink,
		ABI::AudioVisualizer::IVisualizationSource>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_MediaAnalyzer, BaseTrust)
		
#pragma region IMFTransform variables
		Microsoft::WRL::ComPtr<IMFMediaType> m_spOutputType;
		Microsoft::WRL::ComPtr<IMFMediaType> m_spInputType;
		Microsoft::WRL::ComPtr<IMFAttributes> m_spMftAttributes;
		Microsoft::WRL::ComPtr<IMFSample> m_spSample;
		Microsoft::WRL::Wrappers::CriticalSection m_csMft;	// Critical section lock to avoid multiple threads modifying this MFT at the same time
#pragma endregion

#pragma region Analyzer variables and methods
		UINT32 m_FramesPerSecond;
		UINT32 m_nChannels;

		bool m_bIsSuspended;	// Is analyzer in a suspended state
		ComPtr<IThreadPoolStatics> _threadPoolStatics;
		HANDLE _threadPoolSemaphore;	// Controls threadpool execution - schedule only one instance of execution
		bool _bFlushPending;

		const size_t cMaxOutputQueueSize = 600;	// Keep 10sec worth of data for 60fps output

		std::shared_ptr<Math::CAudioAnalyzer> _analyzer;
	
		Microsoft::WRL::Wrappers::CriticalSection m_csAnalyzerAccess;	// Critical section to lock analyzer configuration changing
		Microsoft::WRL::Wrappers::CriticalSection m_csOutputQueueAccess;
		std::queue<ComPtr<AudioVisualizer::VisualizationDataFrame>> m_AnalyzerOutput;

		size_t m_StepFrameCount;	// How many samples does calculate consume each step
		size_t m_StepFrameOverlap;
		size_t m_StepTotalFrames;
		size_t m_FftLength;
		size_t m_FftLengthLog2;

		float m_fOutputFps;
		float m_fInputOverlap;

		AnalyzerType  _analyzerTypes;

		SourcePlaybackState _playbackState;

		HRESULT Analyzer_TestInputType(IMFMediaType *pType);
		HRESULT Analyzer_SetMediaType(IMFMediaType *pType);
		HRESULT Analyzer_Initialize();
		HRESULT Analyzer_ProcessSample(IMFSample *pSample);
		HRESULT Analyzer_ScheduleProcessing();
		void Analyzer_ProcessData();
		HRESULT Analyzer_Flush(); 
		HRESULT Analyzer_CompactOutputQueue();
		HRESULT Analyzer_ClearOutputQueue();
		HRESULT Analyzer_Resume();
		HRESULT Analyzer_Suspend();
		HRESULT Analyzer_FFwdQueueTo(REFERENCE_TIME time, IVisualizationDataFrame **ppFrame);

		
		inline long time_to_samples(REFERENCE_TIME time) const { return m_nChannels * (long)((time * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline long time_to_frames(REFERENCE_TIME time) const { return (long)((time * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline long time_to_frames(float time) const { return (long)((((REFERENCE_TIME)(1e7 * time))  * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline REFERENCE_TIME frames_to_time(long frames) { return 10000000L * (long long)frames / m_FramesPerSecond; }
		inline REFERENCE_TIME samples_to_time(long samples) { return 10000000L * (long long)(samples / m_nChannels) / m_FramesPerSecond; }

		REFERENCE_TIME GetPresentationTime();
#pragma endregion


#pragma region IMediaExtension
		STDMETHODIMP SetProperties(ABI::Windows::Foundation::Collections::IPropertySet *pConfiguration);
#pragma endregion

#pragma region IMFTransform implementation
		STDMETHODIMP GetStreamLimits(DWORD *pdwInputMinimum,DWORD *pdwInputMaximum,DWORD *pdwOutputMinimum,DWORD *pdwOutputMaximum);
		STDMETHODIMP GetStreamCount(DWORD *pcInputStreams,DWORD *pcOutputStreams);
		STDMETHODIMP GetStreamIDs(DWORD dwInputIDArraySize,DWORD *pdwInputIDs,DWORD dwOutputIDArraySize,DWORD *pdwOutputIDs);
		STDMETHODIMP GetInputStreamInfo(DWORD dwInputStreamID,MFT_INPUT_STREAM_INFO *pStreamInfo);
		STDMETHODIMP GetOutputStreamInfo(DWORD dwOutputStreamID,MFT_OUTPUT_STREAM_INFO *  pStreamInfo);
		STDMETHODIMP GetAttributes(IMFAttributes** pAttributes);
		STDMETHODIMP GetInputStreamAttributes(DWORD dwInputStreamID,IMFAttributes **ppAttributes);
		STDMETHODIMP GetOutputStreamAttributes(DWORD dwOutputStreamID,IMFAttributes **ppAttributes);
		STDMETHODIMP DeleteInputStream(DWORD dwStreamID);
		STDMETHODIMP AddInputStreams(DWORD cStreams,DWORD *adwStreamIDs);
		STDMETHODIMP GetInputAvailableType(DWORD dwInputStreamID,DWORD dwTypeIndex,IMFMediaType **ppType);
		STDMETHODIMP GetOutputAvailableType(DWORD  dwOutputStreamID,DWORD dwTypeIndex,IMFMediaType **ppType);
		STDMETHODIMP SetInputType(DWORD dwInputStreamID,IMFMediaType *pType,DWORD dwFlags);
		STDMETHODIMP SetOutputType(DWORD dwOutputStreamID,IMFMediaType *pType,DWORD dwFlags);
		STDMETHODIMP GetInputCurrentType(DWORD dwInputStreamID,IMFMediaType **ppType);
		STDMETHODIMP GetOutputCurrentType(DWORD dwOutputStreamID,IMFMediaType **ppType);
		STDMETHODIMP GetInputStatus(DWORD dwInputStreamID,DWORD *pdwFlags);
		STDMETHODIMP GetOutputStatus(DWORD *pdwFlags);
		STDMETHODIMP SetOutputBounds(LONGLONG hnsLowerBound,LONGLONG hnsUpperBound);
		STDMETHODIMP ProcessEvent(DWORD dwInputStreamID,IMFMediaEvent *pEvent);
		STDMETHODIMP ProcessMessage(MFT_MESSAGE_TYPE eMessage,ULONG_PTR ulParam);
		STDMETHODIMP ProcessInput(DWORD dwInputStreamID,IMFSample *pSample,DWORD dwFlags);
		STDMETHODIMP ProcessOutput(DWORD dwFlags,DWORD cOutputBufferCount,MFT_OUTPUT_DATA_BUFFER  *pOutputSamples,DWORD *pdwStatus);
#pragma endregion

#pragma region IMFClockConsumer implementation
		Microsoft::WRL::ComPtr<IMFPresentationClock> m_spPresentationClock;
		HRESULT STDMETHODCALLTYPE SetPresentationClock(_In_opt_ IMFPresentationClock *pPresentationClock);
		HRESULT STDMETHODCALLTYPE GetPresentationClock(_COM_Outptr_opt_result_maybenull_ IMFPresentationClock **pPresentationClock)
		{
			return E_NOTIMPL;
		}
#pragma endregion

#pragma region IMFClockStateSink implementation
		virtual HRESULT STDMETHODCALLTYPE OnClockStart(
			/* [in] */ MFTIME hnsSystemTime,
			/* [in] */ LONGLONG llClockStartOffset)
		{
			_playbackState = SourcePlaybackState::Playing;
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE OnClockStop(
			/* [in] */ MFTIME hnsSystemTime)
		{
			_playbackState = SourcePlaybackState::Stopped;
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE OnClockPause(
			/* [in] */ MFTIME hnsSystemTime)
		{
			_playbackState = SourcePlaybackState::Paused;
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE OnClockRestart(
			/* [in] */ MFTIME hnsSystemTime)
		{
			_playbackState = SourcePlaybackState::Playing;
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE OnClockSetRate(
			/* [in] */ MFTIME hnsSystemTime,
			/* [in] */ float flRate)
		{
			return S_OK;
		}
#pragma endregion

		EventSource<ITypedEventHandler<IVisualizationSource*, HSTRING>> _configurationChangedList;
		HRESULT RaiseConfiguratonChanged(wchar_t *wszPropertyName)
		{
			return _configurationChangedList.InvokeAll(this, HStringReference(wszPropertyName).Get());
		}
	public:
		MediaAnalyzer();
		~MediaAnalyzer();
		HRESULT RuntimeClassInitialize();
#pragma region IVisualizationSource
		STDMETHODIMP GetData(IVisualizationDataFrame **pData);
		STDMETHODIMP get_IsSuspended(boolean *pbIsSuspended);
		STDMETHODIMP put_IsSuspended(boolean bIsSuspended);
		STDMETHODIMP get_Fps(float *pFrameRate);
		STDMETHODIMP put_Fps(float frameRate);
		STDMETHODIMP get_AnalyzerTypes(AnalyzerType *pResult);
		STDMETHODIMP put_AnalyzerTypes(AnalyzerType result);
		STDMETHODIMP get_PresentationTime(IReference<TimeSpan> **ppTime);
		STDMETHODIMP get_PlaybackState(SourcePlaybackState *pState)
		{
			if (pState == nullptr)
				return E_POINTER;
			*pState = _playbackState;
			return S_OK;
		}
		STDMETHODIMP get_ActualFrequencyCount(IReference<UINT32> **ppcElements)
		{
			using namespace wrl_util;
			if (ppcElements == nullptr)
				return E_POINTER;
			UINT32 frequencyCount = _analyzer->GetFftLength() / 2;
			auto value = Make<Nullable<UINT32>>(frequencyCount);
			return value.CopyTo(ppcElements);
		}
		STDMETHODIMP get_ActualChannelCount(IReference<UINT32> **ppcElements)
		{
			if (ppcElements == nullptr)
				return E_POINTER;
			auto lock = m_csMft.Lock();
			if (m_spOutputType != nullptr)
			{
				UINT32 channels;
				m_spOutputType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &channels);
				auto value = Make<Nullable<UINT32>>(channels);
				return value.CopyTo(ppcElements);
			}
			else
				*ppcElements = nullptr;
			return S_OK;
		}

		STDMETHODIMP get_ActualMinFrequency(IReference<float> **ppValue)
		{
			if (ppValue == nullptr)
				return E_POINTER;
			auto minFreq = Make<Nullable<float>>(0.0f);
			return minFreq.CopyTo(ppValue);
		}

		STDMETHODIMP get_ActualMaxFrequency(IReference<float> **ppValue)
		{
			if (ppValue == nullptr)
				return E_POINTER;
			
			auto lock = m_csMft.Lock();
			if (m_spOutputType == nullptr)
			{
				*ppValue = nullptr;
			}
			UINT32 sampleRate = 0;
			m_spOutputType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND,&sampleRate);

			auto maxFreq = Make<Nullable<float>>((float) sampleRate / (2.0f * (float)_analyzer->GetDownsampleRate() ) );
			return maxFreq.CopyTo(ppValue);
		}

		STDMETHODIMP get_ActualFrequencyScale(IReference<ScaleType> **ppValue)
		{
			if (ppValue == nullptr)
				return E_POINTER;
			auto scale = Make<Nullable<ScaleType>>(ScaleType::Linear);
			return scale.CopyTo(ppValue);
		}
		
		STDMETHODIMP add_ConfigurationChanged(
			ITypedEventHandler<IVisualizationSource *, HSTRING> *value,
			EventRegistrationToken *token)
		{
			return _configurationChangedList.Add(value, token);
		}
		STDMETHODIMP remove_ConfigurationChanged(
			EventRegistrationToken token)
		{
			return _configurationChangedList.Remove(token);
		}

#pragma endregion
		
		STDMETHODIMP ConfigureSpectrum(UINT32 fftLength, float overlap);
	};
}
