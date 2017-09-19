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

using namespace Microsoft::WRL;
using namespace ABI::Windows::System::Threading;

namespace AudioVisualizer
{
#define MFT_ANALYZER_PROPERTYSET_NAME L"Source"

	class CAnalyzerEffect
		: public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
		ABI::Windows::Media::IMediaExtension,
		IMFTransform,
		IMFClockConsumer,
		ABI::AudioVisualizer::IVisualizationSource
		/*IMFAsyncCallback*/>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_MftAnalyzer, BaseTrust)
		
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
		bool _bResetAnalyzer;

		const size_t cMaxOutputQueueSize = 600;	// Keep 10sec worth of data for 60fps output

		std::shared_ptr<AudioMath::CAudioAnalyzer> _analyzer;
	
		Microsoft::WRL::Wrappers::CriticalSection m_csAnalyzerConfig;	// Critical section to lock analyzer configuration changing
		Microsoft::WRL::Wrappers::CriticalSection m_csOutputQueueAccess;
		std::queue<ComPtr<AudioMath::AnalyzerFrame>> m_AnalyzerOutput;

		size_t m_StepFrameCount;	// How many samples does calculate consume each step
		size_t m_StepFrameOverlap;
		size_t m_StepTotalFrames;
		size_t m_FftLength;
		size_t m_FftLengthLog2;

		float m_fOutputFps;
		float m_fInputOverlap;

		ABI::AudioVisualizer::AnalyzerType  _analyzisTypes;

		HRESULT Analyzer_TestInputType(IMFMediaType *pType);
		HRESULT Analyzer_SetMediaType(IMFMediaType *pType);
		HRESULT Analyzer_Initialize();
		HRESULT Analyzer_ProcessSample(IMFSample *pSample);
		HRESULT Analyzer_ScheduleProcessing();
		void Analyzer_ProcessData();
		HRESULT Analyzer_Reset();
		HRESULT Analyzer_Flush(); 
		HRESULT Analyzer_CompactOutputQueue();
		HRESULT Analyzer_FlushOutputQueue();
		HRESULT Analyzer_Resume();
		HRESULT Analyzer_Suspend();
		HRESULT Analyzer_FFwdQueueTo(REFERENCE_TIME time, AudioMath::AnalyzerFrame **ppFrame);

		
		inline long time_to_samples(REFERENCE_TIME time) const { return m_nChannels * (long)((time * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline long time_to_frames(REFERENCE_TIME time) const { return (long)((time * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline long time_to_frames(float time) const { return (long)((((REFERENCE_TIME)(1e7 * time))  * m_FramesPerSecond + 5000000L) / 10000000L); }
		inline REFERENCE_TIME frames_to_time(long frames) { return 10000000L * (long long)frames / m_FramesPerSecond; }
		inline REFERENCE_TIME samples_to_time(long samples) { return 10000000L * (long long)(samples / m_nChannels) / m_FramesPerSecond; }

		REFERENCE_TIME GetPresentationTime()
		{
			MFTIME presentationTime = -1;
			if (m_spPresentationClock != nullptr)
				m_spPresentationClock->GetTime(&presentationTime);
			return presentationTime;
		}
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
		HRESULT STDMETHODCALLTYPE SetPresentationClock(_In_opt_ IMFPresentationClock *pPresentationClock)
		{
			m_spPresentationClock = pPresentationClock;
			return S_OK;
		}
		HRESULT STDMETHODCALLTYPE GetPresentationClock(_COM_Outptr_opt_result_maybenull_ IMFPresentationClock **pPresentationClock)
		{
			return E_NOTIMPL;
		}
#pragma endregion

#pragma region IMFAsyncCallback implementation
		/*STDMETHODIMP GetParameters(DWORD*, DWORD*)
		{
			// Implementation of this method is optional.
			return E_NOTIMPL;
		}

		STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult)
		{
			return Analyzer_Step(pAsyncResult);
		}*/
#pragma endregion

	public:
		CAnalyzerEffect();
		~CAnalyzerEffect();
		HRESULT RuntimeClassInitialize();
#pragma region IAudioVisualizer
		STDMETHODIMP Configure(ABI::AudioVisualizer::AnalyzerType types, float outputFps,unsigned fftLength,  float inputOverlap);
		STDMETHODIMP GetData(ABI::AudioVisualizer::IVisualizationDataFrame **pData);
		STDMETHODIMP get_IsSuspended(boolean *pbIsSuspended);
		STDMETHODIMP put_IsSuspended(boolean bIsSuspended);

#pragma endregion
	};
}
