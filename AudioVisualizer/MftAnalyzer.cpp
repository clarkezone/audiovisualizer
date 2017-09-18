#include "pch.h"
#include "MftAnalyzer.h"
#include "xdsp.h"
#include <cfloat>
#include <windows.media.core.interop.h>
#include <trace.h>
#include "Nullable.h"
#include <VisualizationDataFrame.h>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfplat.lib")

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;

namespace AudioVisualizer
{
	ActivatableClass(CAnalyzerEffect);

	const size_t CircleBufferSize = 960000;	// 10 sec worth of stereo audio at 48k

	CAnalyzerEffect::CAnalyzerEffect() :
		m_FramesPerSecond(0),
		m_nChannels(0),
		_bResetAnalyzer(false),
		_threadPoolSemaphore(NULL),
		m_StepFrameCount(0),
		m_StepFrameOverlap(0),
		m_StepTotalFrames(0),
		m_FftLength(0),
		m_FftLengthLog2(0),
		m_fOutputFps(0.0f),
		m_fInputOverlap(0.0f),
		m_bIsSuspended(false)
	{
		_analyzer = std::make_shared<AudioMath::CAudioAnalyzer>(CircleBufferSize);
#ifdef _TRACE
		AudioVisualizer::Diagnostics::Trace::Initialize();
#endif
	}

	CAnalyzerEffect::~CAnalyzerEffect()
	{
		CloseHandle(_threadPoolSemaphore);
#ifdef _TRACE
		AudioVisualizer::Diagnostics::Trace::Shutdown();
#endif
	}


	HRESULT CAnalyzerEffect::RuntimeClassInitialize()
	{
		HRESULT hr = MFCreateAttributes(&m_spMftAttributes, 4);
		if (FAILED(hr))
			return hr;

		hr = GetActivationFactory(HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).Get(), &_threadPoolStatics);
		if (FAILED(hr))
			return hr;
		
		_threadPoolSemaphore = CreateSemaphore(nullptr, 1, 1, nullptr);
		if (_threadPoolSemaphore == NULL)
			return E_FAIL;
	
		return S_OK;
	}

	STDMETHODIMP CAnalyzerEffect::Configure(ABI::AudioVisualizer::AnalyzerType types, float outputFps, unsigned fftLength,  float inputOverlap)
	{
		if ((fftLength & fftLength - 1) != 0)	// FFT length needs to be power of 2
			return E_INVALIDARG;

		if (outputFps < 1.0f || outputFps > 120.0f)	// Set some reasonable limitations
			return E_INVALIDARG;
		// Make sure that the length of output frame does not contain more input frames than fft length.
		// If input sample rate is not set then use 48k
		UINT32 framesPerSecond = m_FramesPerSecond != 0 ? m_FramesPerSecond : 48000;
		if (framesPerSecond / outputFps > fftLength)
			return E_INVALIDARG;
		if (inputOverlap < 0.0f || inputOverlap > 1.0f)	// Set some sensible overlap limits
			return E_INVALIDARG;

		_analyzisTypes = types;
		m_FftLength = fftLength;
		m_fOutputFps = outputFps;
		m_fInputOverlap = inputOverlap;

		// If input type is set then calculate the necessary variables and initialize
		if (m_FramesPerSecond != 0)
			return Analyzer_Initialize();

		return S_OK;
	}

	STDMETHODIMP CAnalyzerEffect::GetData(ABI::AudioVisualizer::IVisualizationDataFrame **ppData)
	{
		if (ppData == nullptr)
			return E_POINTER;
		*ppData = nullptr;

		MFTIME currentPosition = -1;
		HRESULT hr = S_OK;
		if (m_spPresentationClock != nullptr) {
			m_spPresentationClock->GetTime(&currentPosition);
		}

		if (currentPosition != -1)	// If no presentation position then return nullptr
		{
#ifdef _TRACE
			AudioVisualizer::Diagnostics::Trace::Trace_Lock(&m_csOutputQueueAccess,L"OutputQueue",
				[&hr,this,currentPosition,ppData] 
			{
				hr = Analyzer_FFwdQueueTo(currentPosition, ppData);
			}
			);
#else
			auto lock = m_csOutputQueueAccess.Lock();	
			hr = Analyzer_FFwdQueueTo(currentPosition, ppData);
#endif
		}

#ifdef _TRACE
		AudioVisualizer::Diagnostics::Trace::Log_GetData(currentPosition, *ppData,m_AnalyzerOutput.empty() ? nullptr : m_AnalyzerOutput.front().Get(), m_AnalyzerOutput.size(),hr);
#endif

		return hr;
	}

	STDMETHODIMP CAnalyzerEffect::get_IsSuspended(boolean * pbIsSuspended)
	{
		if (pbIsSuspended == nullptr)
			return E_POINTER;
		*pbIsSuspended = (boolean) m_bIsSuspended;
		return S_OK;
	}
	STDMETHODIMP CAnalyzerEffect::put_IsSuspended(boolean bNewValue)
	{
		if (m_bIsSuspended != (bool) bNewValue)
		{
			if (bNewValue)
				return Analyzer_Suspend();
			else
				return Analyzer_Resume();
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetStreamLimits
	// Returns the minum and maximum number of streams.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetStreamLimits(DWORD * pdwInputMinimum, DWORD * pdwInputMaximum, DWORD * pdwOutputMinimum, DWORD * pdwOutputMaximum)
	{
		if (pdwInputMaximum == nullptr || pdwInputMaximum == nullptr || pdwOutputMinimum == nullptr || pdwOutputMaximum == nullptr)
			return E_POINTER;
		// This MFT has a fixed number of streams
		*pdwInputMinimum = 1;
		*pdwInputMaximum = 1;
		*pdwOutputMinimum = 1;
		*pdwOutputMaximum = 1;
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetStreamCount
	// Returns the actual number of streams.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetStreamCount(DWORD * pcInputStreams, DWORD * pcOutputStreams)
	{
		if ((pcInputStreams == NULL) || (pcOutputStreams == NULL))
		{
			return E_POINTER;
		}

		// This MFT has a fixed number of streams.
		*pcInputStreams = 1;
		*pcOutputStreams = 1;
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetStreamIDs
	// Returns stream IDs for the input and output streams.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetStreamIDs(DWORD dwInputIDArraySize, DWORD * pdwInputIDs, DWORD dwOutputIDArraySize, DWORD * pdwOutputIDs)
	{
		// It is not required to implement this method if the MFT has a fixed number of
		// streams AND the stream IDs are numbered sequentially from zero (that is, the
		// stream IDs match the stream indexes).

		// In that case, it is OK to return E_NOTIMPL.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// GetInputStreamInfo
	// Returns information about an input stream.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetInputStreamInfo(DWORD dwInputStreamID, MFT_INPUT_STREAM_INFO * pStreamInfo)
	{
		if (pStreamInfo == NULL)
		{
			return E_POINTER;
		}
		if (dwInputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}
		UINT32 minFrameSize = 0;
		if (m_spInputType != nullptr)
			m_spInputType->GetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, &minFrameSize);
		pStreamInfo->cbSize = minFrameSize;
		pStreamInfo->cbAlignment = 0;
		pStreamInfo->hnsMaxLatency = 0;
		pStreamInfo->dwFlags = MFT_INPUT_STREAM_WHOLE_SAMPLES;
		pStreamInfo->cbMaxLookahead = 0;
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetOutputStreamInfo
	// Returns information about an output stream.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetOutputStreamInfo(DWORD dwOutputStreamID, MFT_OUTPUT_STREAM_INFO * pStreamInfo)
	{
		if (pStreamInfo == NULL)
		{
			return E_POINTER;
		}
		if (dwOutputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}
		// NOTE: This method should succeed even when there is no media type on the
		//       stream. If there is no media type, we only need to fill in the dwFlags
		//       member of MFT_OUTPUT_STREAM_INFO. The other members depend on having a
		//       a valid media type.


		// Flags
		pStreamInfo->dwFlags =
			MFT_OUTPUT_STREAM_WHOLE_SAMPLES |         // Output buffers contain complete audio frames.
			MFT_OUTPUT_STREAM_CAN_PROVIDE_SAMPLES |   // The MFT can allocate output buffers, or use caller-allocated buffers.
			MFT_OUTPUT_STREAM_FIXED_SAMPLE_SIZE;      // Samples (ie, audio frames) are fixed size.

		pStreamInfo->cbSize = 0;   // If no media type is set, use zero.
		pStreamInfo->cbAlignment = 0;

		auto lock = m_csMft.Lock();
		if (m_spOutputType != nullptr) {
			pStreamInfo->cbSize = MFGetAttributeUINT32(m_spOutputType.Get(), MF_MT_AUDIO_BLOCK_ALIGNMENT, 0);
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetAttributes
	// Returns the attributes for the MFT.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetAttributes(IMFAttributes ** ppAttributes)
	{
		if (ppAttributes == NULL)
		{
			return E_POINTER;
		}
		auto lock = m_csMft.Lock();
		m_spMftAttributes.CopyTo(ppAttributes);
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetInputStreamAttributes
	// Returns stream-level attributes for an input stream.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetInputStreamAttributes(DWORD dwInputStreamID, IMFAttributes ** ppAttributes)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// GetOutputStreamAttributes
	// Returns stream-level attributes for an output stream.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetOutputStreamAttributes(DWORD dwOutputStreamID, IMFAttributes ** ppAttributes)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// DeleteInputStream
	// Remove stream from processing
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::DeleteInputStream(DWORD dwStreamID)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// AddInputStreams
	// Add streams for processing
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::AddInputStreams(DWORD cStreams, DWORD * adwStreamIDs)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// GetInputAvailableType
	// Returns a preferred input type.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetInputAvailableType(DWORD dwInputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType)
	{
		if (ppType == NULL)
		{
			return E_INVALIDARG;
		}
		if (dwInputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}

		HRESULT hr = S_OK;
		auto lock = m_csMft.Lock();

		// If the output type is set, return that type as our preferred input type.
		if (m_spOutputType == nullptr)
		{
			// The output type is not set. Create a partial media type.

			Microsoft::WRL::ComPtr<IMFMediaType> spPartialMediaType;

			HRESULT hr = MFCreateMediaType(&spPartialMediaType);
			if (FAILED(hr))
			{
				return hr;
			}

			if (dwTypeIndex == 0) {
				hr = spPartialMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
				if (FAILED(hr))
				{
					return hr;
				}

				hr = spPartialMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_Float);
				if (FAILED(hr))
				{
					return hr;
				}
			}
			else
			{
				return MF_E_NO_MORE_TYPES;	// We really want float PCM, only 1 type (allow index==0)
			}

			spPartialMediaType.CopyTo(ppType);
			return S_OK;
		}
		else if (dwTypeIndex > 0)
		{
			hr = MF_E_NO_MORE_TYPES;
		}
		else
		{
			m_spOutputType.CopyTo(ppType);
		}
		return hr;
	}

	//-------------------------------------------------------------------
	// GetOutputAvailableType
	// Returns a preferred output type.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetOutputAvailableType(DWORD dwOutputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType)
	{
		if (ppType == NULL)
		{
			return E_INVALIDARG;
		}
		if (dwOutputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}

		HRESULT hr = S_OK;
		auto lock = m_csMft.Lock();

		if (m_spInputType == nullptr)
		{
			// The input type is not set. Request it to be set first
			return MF_E_TRANSFORM_TYPE_NOT_SET;
		}
		else if (dwTypeIndex > 0)
		{
			hr = MF_E_NO_MORE_TYPES; // Only one type (Audio_Float)
		}
		else
		{
			// Use input as output type
			m_spInputType.CopyTo(ppType);
		}
		return hr;
	}

	//-------------------------------------------------------------------
	// SetInputType
	// Test and set input data type
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::SetInputType(DWORD dwInputStreamID, IMFMediaType * pType, DWORD dwFlags)
	{
		// Validate flags.
		if (dwInputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}
		if (dwFlags & ~_MFT_SET_TYPE_FLAGS::MFT_SET_TYPE_TEST_ONLY)	// Any other flags than media test flag are invalid
		{
			return E_INVALIDARG;
		}

		auto lock = m_csMft.Lock();

		// If we have an input sample, the client cannot change the type now.
		if (m_spSample != nullptr)
		{
			return MF_E_TRANSFORM_CANNOT_CHANGE_MEDIATYPE_WHILE_PROCESSING;
		}

		// Validate the type, if non-NULL.
		if (pType != nullptr) {
			HRESULT hr = Analyzer_TestInputType(pType);
			if (FAILED(hr))
				return hr;
		}

		// The type is OK. Set the type, unless the caller was just testing.
		if ((dwFlags & MFT_SET_TYPE_TEST_ONLY) == 0)
		{
			m_spInputType = pType;
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// SetOutputType
	// Test and set output data type
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::SetOutputType(DWORD dwOutputStreamID, IMFMediaType * pType, DWORD dwFlags)
	{
		// Validate parameters
		if (dwOutputStreamID != 0)
			return MF_E_INVALIDSTREAMNUMBER;
		if (dwFlags & ~_MFT_SET_TYPE_FLAGS::MFT_SET_TYPE_TEST_ONLY)	// Any other flags than media test flag are invalid
			return E_INVALIDARG;
		if (m_spInputType == nullptr)
			return MF_E_TRANSFORM_TYPE_NOT_SET;
		if (m_spSample != nullptr)
		{
			return MF_E_TRANSFORM_CANNOT_CHANGE_MEDIATYPE_WHILE_PROCESSING;
		}
		DWORD dwTypeCompFlags = 0;
		if (m_spInputType->IsEqual(pType, &dwTypeCompFlags) != S_OK)
			return MF_E_INVALIDMEDIATYPE;


		// The type is OK. Set the type, unless the caller was just testing.
		if ((dwFlags & MFT_SET_TYPE_TEST_ONLY) == 0)
		{
			m_spOutputType = pType;
			return Analyzer_SetMediaType(pType);	// Using MFT output type to configure analyzer
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetInputCurrentType
	// Returns the current input type.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetInputCurrentType(DWORD dwInputStreamID, IMFMediaType ** ppType)
	{
		if (ppType == NULL)
		{
			return E_POINTER;
		}
		if (dwInputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}
		else if (!m_spInputType)
		{
			return MF_E_TRANSFORM_TYPE_NOT_SET;
		}
		else
		{
			m_spInputType.CopyTo(ppType);
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetOutputCurrentType
	// Returns the current output type.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetOutputCurrentType(DWORD dwOutputStreamID, IMFMediaType ** ppType)
	{
		if (ppType == NULL)
		{
			return E_POINTER;
		}
		if (dwOutputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}
		else if (!m_spOutputType)
		{
			return MF_E_TRANSFORM_TYPE_NOT_SET;
		}
		else
		{
			m_spOutputType.CopyTo(ppType);
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetInputStatus
	// Query if the MFT is accepting more input.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetInputStatus(
		DWORD           dwInputStreamID,
		DWORD           *pdwFlags
	)
	{
		if (pdwFlags == nullptr)
		{
			return E_POINTER;
		}
		if (dwInputStreamID == 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}

		// If an input sample is already queued, do not accept another sample until the 
		// client calls ProcessOutput or Flush.

		// NOTE: It is possible for an MFT to accept more than one input sample. For 
		// example, this might be required in a video decoder if the frames do not 
		// arrive in temporal order. In the case, the decoder must hold a queue of 
		// samples. For the video effect, each sample is transformed independently, so
		// there is no reason to queue multiple input samples.
		auto lock = m_csMft.Lock();

		if (m_spSample == nullptr)
		{
			*pdwFlags = MFT_INPUT_STATUS_ACCEPT_DATA;
		}
		else
		{
			*pdwFlags = 0;
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetOutputStatus
	// Query if the MFT can produce output.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::GetOutputStatus(DWORD *pdwFlags)
	{
		if (pdwFlags == nullptr)
		{
			return E_POINTER;
		}
		auto lock = m_csMft.Lock();

		// The MFT can produce an output sample if (and only if) there an input sample.
		if (m_spSample != nullptr)
		{
			*pdwFlags = MFT_OUTPUT_STATUS_SAMPLE_READY;
		}
		else
		{
			*pdwFlags = 0;
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// SetOutputBounds
	// Sets the range of time stamps that the MFT will output.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::SetOutputBounds(LONGLONG hnsLowerBound, LONGLONG hnsUpperBound)
	{
		return E_NOTIMPL;	// This method is not implemented for this MFT
	}

	//-------------------------------------------------------------------
	// ProcessEvent
	// Sends an event to an input stream.
	//-------------------------------------------------------------------
	STDMETHODIMP CAnalyzerEffect::ProcessEvent(
		DWORD              dwInputStreamID,
		IMFMediaEvent      *pEvent
	)
	{
		return S_OK;
	}


	//-------------------------------------------------------------------
	// ProcessMessage
	//-------------------------------------------------------------------
	HRESULT CAnalyzerEffect::ProcessMessage(
		MFT_MESSAGE_TYPE    eMessage,
		ULONG_PTR           ulParam
	)
	{
		auto lock = m_csMft.Lock();

		HRESULT hr = S_OK;

		switch (eMessage)
		{
		case MFT_MESSAGE_COMMAND_FLUSH:
			// Flush the MFT.
			hr = Analyzer_Flush();
			break;

		case MFT_MESSAGE_COMMAND_DRAIN:
			// Drain: Tells the MFT to reject further input until all pending samples are
			// processed. That is our default behavior already, so there is nothing to do.
			//
			// For a decoder that accepts a queue of samples, the MFT might need to drain
			// the queue in response to this command.
			break;

		case MFT_MESSAGE_SET_D3D_MANAGER:
			// Sets a pointer to the IDirect3DDeviceManager9 interface.

			// The pipeline should never send this message unless the MFT sets the MF_SA_D3D_AWARE 
			// attribute set to TRUE. Because this MFT does not set MF_SA_D3D_AWARE, it is an error
			// to send the MFT_MESSAGE_SET_D3D_MANAGER message to the MFT. Return an error code in
			// this case.

			// NOTE: If this MFT were D3D-enabled, it would cache the IDirect3DDeviceManager9 
			// pointer for use during streaming.

			hr = E_NOTIMPL;
			break;

		case MFT_MESSAGE_NOTIFY_BEGIN_STREAMING:
			break;

		case MFT_MESSAGE_NOTIFY_END_STREAMING:
			break;

			// The next two messages do not require any action from this MFT.

		case MFT_MESSAGE_NOTIFY_END_OF_STREAM:
			break;

		case MFT_MESSAGE_NOTIFY_START_OF_STREAM:
			hr = Analyzer_Reset();
			break;
		}

		return hr;
	}

	HRESULT CAnalyzerEffect::ProcessInput(DWORD dwInputStreamID, IMFSample * pSample, DWORD dwFlags)
	{
		// Check input parameters.
		if (pSample == NULL)
		{
			return E_POINTER;
		}
		if (dwFlags != 0)
		{
			return E_INVALIDARG; // dwFlags is reserved and must be zero.
		}
		// Validate the input stream number.
		if (dwInputStreamID != 0)
		{
			return MF_E_INVALIDSTREAMNUMBER;
		}
		// Check for valid media types.
		// The client must set input and output types before calling ProcessInput.
		if (!m_spInputType || !m_spOutputType)
		{
			return MF_E_NOTACCEPTING;
		}
		// Check if an input sample is already queued.
		if (m_spSample != nullptr)
		{
			return MF_E_NOTACCEPTING;   // We already have an input sample.
		}

		HRESULT hr = S_OK;

		auto lock = m_csMft.Lock();

		// Cache the sample. We do the actual work in ProcessOutput.
		m_spSample = pSample;

		return hr;
	}
	HRESULT CAnalyzerEffect::ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER * pOutputSamples, DWORD * pdwStatus)
	{
		if (dwFlags != 0)
		{
			return E_INVALIDARG;
		}
		if (pOutputSamples == nullptr || pdwStatus == nullptr)
		{
			return E_POINTER;
		}
		// There must be exactly one output buffer.
		if (cOutputBufferCount != 1)
		{
			return E_INVALIDARG;
		}

		HRESULT hr = S_OK;

		auto lock = m_csMft.Lock();

		if (m_spSample == nullptr)
		{
			return MF_E_TRANSFORM_NEED_MORE_INPUT;
		}

		hr = Analyzer_ProcessSample(m_spSample.Get());

		pOutputSamples->pSample = m_spSample.Detach();

		// Set status flags.
		pOutputSamples[0].dwStatus = 0;
		*pdwStatus = 0;

		return hr;
	}

	HRESULT CAnalyzerEffect::Analyzer_TestInputType(IMFMediaType * pMediaType)
	{
		if (pMediaType == nullptr)	// Allow nullptr
			return S_OK;
		GUID majorType;
		HRESULT hr = pMediaType->GetMajorType(&majorType);
		if (FAILED(hr))
			return hr;
		if (majorType != MFMediaType_Audio)
			return MF_E_INVALIDMEDIATYPE;
		GUID minorType;
		hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &minorType);
		if (FAILED(hr))
			return hr;
		if (minorType != MFAudioFormat_Float)
			return MF_E_INVALIDMEDIATYPE;
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_SetMediaType(IMFMediaType * pType)
	{
		m_FramesPerSecond = 0;
		HRESULT hr = pType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &m_FramesPerSecond);
		if (FAILED(hr))
			goto exit;
		m_nChannels = 0;
		hr = pType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &m_nChannels);
		if (FAILED(hr))
			goto exit;

		if (m_FftLength != 0)	// If FFT has been configured, then reconfigure analyzer as input type has changed
			hr = Analyzer_Initialize();
	exit:
#ifdef _TRACE
		Diagnostics::Trace::Log_SetMediaType(pType, hr);
#endif
		return hr;
	}

	HRESULT CAnalyzerEffect::Analyzer_Initialize()
	{
		
		m_FftLengthLog2 = 1;
		while ((size_t)1 << m_FftLengthLog2 != m_FftLength)
			m_FftLengthLog2++;

		m_StepFrameCount = time_to_frames(1.0f / m_fOutputFps);
		m_StepFrameOverlap = (size_t)(m_fInputOverlap * m_StepFrameCount);
		m_StepTotalFrames = m_StepFrameCount + m_StepFrameOverlap;

		_analyzer->ConfigureInput(m_nChannels);
		_analyzer->ConfigureAnalyzer(m_FftLength, m_StepTotalFrames, m_StepFrameOverlap);

#ifdef _TRACE
		Diagnostics::Trace::Log_Initialize(m_FftLength,m_StepTotalFrames,m_StepFrameOverlap);
#endif
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_ProcessSample(IMFSample * pSample)
	{
		if (m_FramesPerSecond == 0 || m_nChannels == 0)
			return E_NOT_VALID_STATE;
		if (m_FftLength == 0)
			return E_NOT_VALID_STATE;

#ifdef _TRACE
		Diagnostics::Trace::Log_ProcessSample(pSample);
#endif

		// Allow processing after a reset event.
		_bResetAnalyzer = false;

		HRESULT hr = S_OK;
		long position = -1;
		if (_analyzer->GetPosition() == -1)	// Sample index not set, get time from sample
		{
			REFERENCE_TIME sampleTime = 0;
			hr = pSample->GetSampleTime(&sampleTime);
			if (FAILED(hr))
				return hr;
			position = time_to_frames(sampleTime);
#ifdef _TRACE
			Diagnostics::Trace::Log_SetInputPosition(position);
#endif
		}

		ComPtr<IMFMediaBuffer> spBuffer;
		hr = pSample->ConvertToContiguousBuffer(&spBuffer);
		if (FAILED(hr))
			return hr;

		float *pBufferData = nullptr;
		DWORD cbCurrentLength = 0;
		hr = spBuffer->Lock((BYTE **)&pBufferData, nullptr, &cbCurrentLength);
		if (FAILED(hr))
			return hr;

		_analyzer->AddInput(pBufferData,cbCurrentLength / sizeof(float),position);

		spBuffer->Unlock();

		Analyzer_ScheduleProcessing();

		return S_OK;
	}

	//-----------------------------------------------
	// ScheduleInputProcessing
	// Validate if background processing is running and if not initiate
	//-----------------------------------------------
	HRESULT CAnalyzerEffect::Analyzer_ScheduleProcessing()
	{
		if (m_bIsSuspended)	// Do not schedule another work item when suspended
			return S_OK;

		// See if the access semaphore is signaled
		DWORD dwWaitResult = WaitForSingleObject(_threadPoolSemaphore, 0);
		if (dwWaitResult == WAIT_OBJECT_0)
		{
			// Execute data processing on threadpool
			ComPtr<IAsyncAction> action;
			return _threadPoolStatics->RunWithPriorityAsync(
			Callback<IWorkItemHandler>(
				[this](IAsyncAction *pAction) -> HRESULT
			{
				Analyzer_ProcessData();
				ReleaseSemaphore(_threadPoolSemaphore, 1, nullptr);
				return S_OK;
			}
				).Get(),
				WorkItemPriority::WorkItemPriority_High,
			&action);

			// return MFPutWorkItem2(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, 0, this, nullptr);
		}
		else if (dwWaitResult == WAIT_TIMEOUT)
		{
			return S_FALSE;	// Analysis is already running
		}
		else
			return E_FAIL;
	}

	void CAnalyzerEffect::Analyzer_ProcessData()
	{
		// Process data until not suspended, not in a reset and output is available
	
		while (!m_bIsSuspended && !_bResetAnalyzer && _analyzer->IsOutputAvailable())
		{
			ComPtr<ScalarData> rms;
			if ((int)_analyzisTypes & (int)AnalyzerType::RMS)
				rms = Make<ScalarData>(m_nChannels);

			ComPtr<ScalarData> peak;
			if ((int)_analyzisTypes & (int)AnalyzerType::Peak)
				peak = Make<ScalarData>(m_nChannels);

			ComPtr<VectorData> spectrum;
			if ((int)_analyzisTypes & (int)AnalyzerType::Spectrum)
				spectrum = Make<VectorData>(m_nChannels, m_FftLength >> 1);	// Spectrum is half fft length
			long position = -1;

			_analyzer->Step(&position, rms->GetBuffer(), peak->GetBuffer(), spectrum->GetBuffer());

			auto time = Make<Nullable<TimeSpan>>(TimeSpan() = { frames_to_time(position) });
			auto duration = Make<Nullable<TimeSpan>>(TimeSpan() = { (REFERENCE_TIME)(1e7 / m_fOutputFps) });

			ComPtr<VisualizationDataFrame> dataFrame = Make<VisualizationDataFrame>(
				time.Get(),
				duration.Get(),
				rms.Get(),
				peak.Get(),
				spectrum.Get()
				);
			

			// Only push the result if reset is not pending
			if (!_bResetAnalyzer)
			{
#ifdef _TRACE
				AudioVisualizer::Diagnostics::Trace::Trace_Lock(&m_csOutputQueueAccess,L"OutputQueue",
					[=] {
					Analyzer_CompactOutputQueue();
					m_AnalyzerOutput.push(dataFrame);
				});
				Diagnostics::Trace::Log_OutputQueuePush(dataFrame.Get(), m_AnalyzerOutput.size());
#else
				auto _lock = m_csOutputQueueAccess.Lock();
				Analyzer_CompactOutputQueue();
				m_AnalyzerOutput.push(dataFrame);
#endif
			
			}
		}
	}

	HRESULT CAnalyzerEffect::Analyzer_Reset()
	{
		_analyzer->Flush();
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_Flush()
	{
		// Dissallow processing and discard any output until new samples 
		_bResetAnalyzer = true;
		// Release input sample and reset the analyzer and queues
		// Clean up any state from buffer copying
		Analyzer_Reset();
		Analyzer_FlushOutputQueue();
		m_spSample.Reset();

#ifdef _TRACE
		Diagnostics::Trace::Log_Flush();
#endif
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_CompactOutputQueue()
	{
		Analyzer_FFwdQueueTo(GetPresentationTime(), nullptr);
		// Now manage queue size - remove items until the size is below limit
		while (m_AnalyzerOutput.size() > cMaxOutputQueueSize)
		{
#ifdef _TRACE
			AudioVisualizer::Diagnostics::Trace::Log_OutputQueuePop(m_AnalyzerOutput.front().Get(),m_AnalyzerOutput.size(), 1);
#endif
			//m_AnalyzerOutput.front() = nullptr;
			m_AnalyzerOutput.pop();
		}
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_FlushOutputQueue()
	{
		auto lock = m_csOutputQueueAccess.Lock();

		while (!m_AnalyzerOutput.empty())
		{
#ifdef _TRACE
			AudioVisualizer::Diagnostics::Trace::Log_OutputQueuePop(m_AnalyzerOutput.front().Get(),m_AnalyzerOutput.size(), 2);
#endif
			//m_AnalyzerOutput.front() = nullptr;
			m_AnalyzerOutput.pop();
		}
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_Resume()
	{
		m_bIsSuspended = false;
		return Analyzer_ScheduleProcessing();
	}

	HRESULT CAnalyzerEffect::Analyzer_Suspend()
	{
		m_bIsSuspended = true;
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_FFwdQueueTo(REFERENCE_TIME position, IVisualizationDataFrame **ppFrame)
	{
		if (position < 0)
			return S_FALSE;

		while (!m_AnalyzerOutput.empty())
		{
			TimeSpan time = { 0 }, duration = { 0 };
			ComPtr<IReference<TimeSpan>> frameTime, frameDuration;

			m_AnalyzerOutput.front()->get_Time(&frameTime);
			m_AnalyzerOutput.front()->get_Duration(&frameDuration);
			if (frameTime == nullptr || frameDuration == nullptr)
				return E_FAIL;
			frameTime->get_Value(&time);
			frameDuration->get_Value(&duration);

			if (position < time.Duration)
			{
				return S_FALSE; // Current position is before the frames in visualization queue - wait until we catch up
			}

			// Add 5uS (about half sample time @96k) to avoid int time math rounding errors
			if (position <= duration.Duration + time.Duration + 50L)
			{

				if (ppFrame != nullptr)	// If frame is requested, return the frame found
				{
					m_AnalyzerOutput.front().CopyTo(ppFrame);
				}
				return S_OK;
			}
			else
			{
				if (position == 0)	// Do not use position 0 to manage queue elements as position can be set to 0 for a long time after stream starts
				{
					return S_FALSE;
				}
				// Current position is after the item in the queue - remove and continue searching
				auto front = m_AnalyzerOutput.front().Get();
				AudioVisualizer::Diagnostics::Trace::Log_OutputQueuePop(m_AnalyzerOutput.front().Get(),m_AnalyzerOutput.size(), 0);
				//m_AnalyzerOutput.front() = nullptr; // Dereference the pointer
				m_AnalyzerOutput.pop();
			}
		}
		return S_FALSE;
	}

	STDMETHODIMP CAnalyzerEffect::SetProperties(ABI::Windows::Foundation::Collections::IPropertySet * pConfiguration)
	{
		using namespace ABI::Windows::Foundation::Collections;
		using namespace Microsoft::WRL;
		using namespace Microsoft::WRL::Wrappers;

		ComPtr<IMap<HSTRING, IInspectable *>> spMap;
		HRESULT hr = ComPtr<IPropertySet>(pConfiguration).As(&spMap);
		if (FAILED(hr))
			return hr;

		boolean bIsReplaced;
		IInspectable *pObj = reinterpret_cast<IInspectable *>(this);
		hr = spMap->Insert(HStringReference(MFT_ANALYZER_PROPERTYSET_NAME).Get(), pObj, &bIsReplaced);
		if (FAILED(hr))
			return hr;

		return S_OK;
	}


}


