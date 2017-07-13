#pragma once
#include "AudioAnalyzer_h.h"
#include <DirectXMath.h>
#include <queue>
#include "ring_buffer.h"

namespace AudioAnalyzer
{
	class CAnalyzerEffect
		: public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
		ABI::Windows::Media::IMediaExtension,
		IMFTransform,
		IMFClockConsumer,
		ABI::AudioAnalyzer::IAudioAnalyzer,
		IMFAsyncCallback>
	{
		InspectableClass(RuntimeClass_AudioAnalyzer_AnalyzerEffect, BaseTrust)
		
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

		const size_t cMaxOutputQueueSize = 600;	// Keep 10sec worth of data for 60fps output

		buffers::ring_buffer<float, 960000> m_InputBuffer;	// Allocate input buffer for 10sec stereo data at 48k

		long m_InputSampleIndex;	// Current read position sample index in stream
		Microsoft::WRL::Wrappers::CriticalSection m_csInputIndexAccess;

		HANDLE m_hWQAccess;	// Semaphore that is used to detect is work queue processing is running
		HANDLE m_hResetWorkQueue;	// Event that is used to cancel and reset work queue processing

		Microsoft::WRL::Wrappers::CriticalSection m_csAnalyzerConfig;	// Critical section to lock analyzer configuration changing
		Microsoft::WRL::Wrappers::CriticalSection m_csOutputQueueAccess;
		std::queue<Microsoft::WRL::ComPtr<IMFSample>> m_AnalyzerOutput;

		size_t m_StepFrameCount;	// How many samples does calculate consume each step
		size_t m_StepFrameOverlap;
		size_t m_StepTotalFrames;
		size_t m_FftLength;
		size_t m_FftLengthLog2;

		float m_fOutputFps;
		float m_fInputOverlap;
		size_t m_OutElementsCount;
		bool m_bUseLogAmpScale;
		bool m_bUseLogFScale;

		float *m_pWindow;
		DirectX::XMVECTOR *m_pFftReal;
		DirectX::XMVECTOR *m_pFftUnityTable;
		DirectX::XMVECTOR *m_pFftBuffers;
		float m_fFftScale;	// 2/N scale factor for fft output
		DirectX::XMVECTOR m_vClampAmpLow;
		DirectX::XMVECTOR m_vClampAmpHigh;

		HRESULT Analyzer_TestInputType(IMFMediaType *pType);
		HRESULT Analyzer_SetMediaType(IMFMediaType *pType);
		HRESULT Analyzer_Initialize();
		HRESULT Analyzer_AllocateBuffers();
		HRESULT Analyzer_FreeBuffers();
		HRESULT Analyzer_ProcessSample(IMFSample *pSample);
		HRESULT Analyzer_ScheduleProcessing();
		HRESULT Analyzer_Step(IMFAsyncResult *pResult);
		HRESULT Analyzer_Calculate(IMFSample **ppSample);
		HRESULT Analyzer_GetFromBuffer(float *pData, REFERENCE_TIME *pPosition);
		void	Analyzer_CalculateFft(DirectX::XMVECTOR *pData, DirectX::XMVECTOR *pBuffers);
		HRESULT	Analyzer_LinearInterpolate(const float *pInput, size_t inputSize, float *pOutput, size_t outputSize);
		HRESULT Analyzer_CreateOutputSample(IMFSample **ppSample, IMFMediaBuffer *pBuffer, REFERENCE_TIME time,size_t bufferStep);
		HRESULT Analyzer_Reset();
		HRESULT Analyzer_Flush(); 
		HRESULT Analyzer_CompactOutputQueue();
		HRESULT Analyzer_FlushOutputQueue();
		HRESULT Analyzer_Resume();
		HRESULT Analyzer_Suspend();
		HRESULT Analyzer_FFwdQueueTo(REFERENCE_TIME time, IMFSample **ppSample);
		void Analyzer_ConvertToDb(DirectX::XMVECTOR *pvData, size_t nElements);	// Converts input values to db range

		inline long time_to_samples(REFERENCE_TIME time) const { return m_nChannels * (long)((time * m_FramesPerSecond + 5000000L) / 10000000L); }
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
		STDMETHODIMP GetParameters(DWORD*, DWORD*)
		{
			// Implementation of this method is optional.
			return E_NOTIMPL;
		}

		STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult)
		{
			return Analyzer_Step(pAsyncResult);
		}
#pragma endregion

	public:
		CAnalyzerEffect();
		~CAnalyzerEffect();
		HRESULT RuntimeClassInitialize();
#pragma region IAudioAnalyzer
		STDMETHODIMP Configure(unsigned long fftLength, float outputFps, float inputOverlap);
		STDMETHODIMP SetLogFScale(float fLow, float fHigh, unsigned long outputElements)	// TODO
		{
			return E_NOTIMPL;
		}
		STDMETHODIMP SetLinearFScale(unsigned long outputElements);
		STDMETHODIMP GetFrame(ABI::Windows::Media::IAudioFrame **ppAudioFrame);
		STDMETHODIMP get_FrequencyStep(float *pResult)
		{
			if (pResult == nullptr)
				return E_FAIL;

			if (m_FramesPerSecond == 0 || m_FftLength == 0 || m_OutElementsCount == 0)
			{
				*pResult = 0.0f;
			}
			else
			{
				*pResult = 0.5f * (float) m_FramesPerSecond / (float) m_OutElementsCount;

			}
			return S_OK;
		}
		STDMETHODIMP get_IsLogFrequencyScale(boolean *pbIsLogFScale);
		STDMETHODIMP get_LowFrequency(float *pfLow);
		STDMETHODIMP get_HighFrequency(float *pfHigh);
		STDMETHODIMP get_OutputElementsCount(unsigned long *pOutElements);
		STDMETHODIMP get_IsLogAmplitudeScale(boolean *pbIsLogAmpScale);
		STDMETHODIMP put_IsLogAmplitudeScale(boolean bIsLogAmpScale);
		STDMETHODIMP get_IsSuspended(boolean *pbIsSuspended);
		STDMETHODIMP put_IsSuspended(boolean bIsSuspended);

#pragma endregion
	};
}
