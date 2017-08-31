#include "pch.h"
#include "MftAnalyzer.h"
#include "xdsp.h"
#include <cfloat>
#include <windows.media.core.interop.h>
#include <trace.h>

#ifdef _DEBUG
	#define _TRACE
#endif


#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfplat.lib")

using namespace Microsoft::WRL;

namespace AudioAnalyzer
{
	ActivatableClass(CAnalyzerEffect);

	// {0B82B25D-E6E1-4B6B-92A1-7EEE99D02CFE}
	static const GUID g_PropKey_RMS_Data_Offset =
	{ 0xb82b25d, 0xe6e1, 0x4b6b,{ 0x92, 0xa1, 0x7e, 0xee, 0x99, 0xd0, 0x2c, 0xfe } };

	// {F4D65F78-CF5A-4949-88C1-76DAD605C313}
	static const GUID g_PropKey_Data_Step =
	{ 0xf4d65f78, 0xcf5a, 0x4949,{ 0x88, 0xc1, 0x76, 0xda, 0xd6, 0x5, 0xc3, 0x13 } };

	const size_t CircleBufferSize = 960000;	// 10 sec worth of stereo audio at 48k

	CAnalyzerEffect::CAnalyzerEffect() :
		m_InputBuffer(CircleBufferSize),
		m_FramesPerSecond(0),
		m_nChannels(0),
		m_hWQAccess(NULL),
		m_hResetWorkQueue(NULL),
		m_StepFrameCount(0),
		m_StepFrameOverlap(0),
		m_StepTotalFrames(0),
		m_FftLength(0),
		m_FftLengthLog2(0),
		m_OutElementsCount(0),
		m_fOutputFps(0.0f),
		m_fInputOverlap(0.0f),
		m_pWindow(nullptr),
		m_pFftReal(nullptr),
		m_pFftUnityTable(nullptr),
		m_pFftBuffers(nullptr),
		m_fFftScale(0.0f),
		m_bUseLogAmpScale(true),
		m_bUseLogFScale(false),
		m_vClampAmpLow(DirectX::XMVectorReplicate(-100.0f)),	// Clamp DB values between -100 and FLT_MAX
		m_vClampAmpHigh(DirectX::XMVectorReplicate(FLT_MAX)),
		m_bIsSuspended(false)
	{
#ifdef _TRACE
		AudioAnalyzer::Diagnostics::Trace::Initialize();
#endif
	}

	CAnalyzerEffect::~CAnalyzerEffect()
	{
		CloseHandle(m_hWQAccess);
		CloseHandle(m_hResetWorkQueue);
		Analyzer_FreeBuffers();
#ifdef _TRACE
		AudioAnalyzer::Diagnostics::Trace::Shutdown();
#endif
	}


	HRESULT CAnalyzerEffect::RuntimeClassInitialize()
	{
		HRESULT hr = MFCreateAttributes(&m_spMftAttributes, 4);
		if (FAILED(hr))
			return hr;

		m_hWQAccess = CreateSemaphore(nullptr, 1, 1, nullptr);
		if (m_hWQAccess == NULL)
			return E_FAIL;

		m_hResetWorkQueue = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);	// Manual reset event
		if (m_hResetWorkQueue == NULL)
			return E_FAIL;

		return S_OK;
	}

	STDMETHODIMP CAnalyzerEffect::Configure(unsigned long fftLength, float outputFps, float inputOverlap)
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

		m_FftLength = fftLength;
		m_fOutputFps = outputFps;
		m_fInputOverlap = inputOverlap;
		m_OutElementsCount = m_FftLength;

		// If input type is set then calculate the necessary variables and initialize
		if (m_FramesPerSecond != 0)
			return Analyzer_Initialize();

		return S_OK;
	}

	STDMETHODIMP CAnalyzerEffect::GetData(ABI::AudioAnalyzer::IVisualizationData **ppData)
	{
		ComPtr<CVisualizationData> spData;
		HRESULT hr = MakeAndInitialize<CVisualizationData>(&spData);
		spData.CopyTo(ppData);
		return S_OK;
	}

	/*
	STDMETHODIMP CAnalyzerEffect::GetFrame(ABI::Windows::Media::IAudioFrame ** ppAudioFrame)
	{
		using namespace Microsoft::WRL;
		using namespace Microsoft::WRL::Wrappers;
		using namespace ABI::Windows::Media;
		using namespace ABI::Windows::Foundation;

		// Get current presentation position
		MFTIME currentPosition = -1;
		HRESULT hr = S_OK;
		if (m_spPresentationClock != nullptr) {
			m_spPresentationClock->GetTime(&currentPosition);
		}

		auto lock = m_csOutputQueueAccess.Lock();

		if (currentPosition == -1)
		{
			*ppAudioFrame = nullptr;
			return S_OK;
		}

		ComPtr<IMFSample> spSample;

#ifdef _TRACE
		size_t queueSize = m_AnalyzerOutput.size();
		ComPtr<IMFSample> spFront;
		if (queueSize != 0)
			spFront = m_AnalyzerOutput.front();
#endif
		hr = Analyzer_FFwdQueueTo(currentPosition, &spSample);

		lock.Unlock();

		if (FAILED(hr))
			return hr;

#ifdef _TRACE
		Diagnostics::Trace::Log_GetData(currentPosition, spSample.Get(), spFront.Get(), queueSize, hr);
#endif
		// Position not found in the queue
		if (hr != S_OK || spSample == nullptr)
		{
			*ppAudioFrame = nullptr;
			return S_OK;
		}

		// Convert IMFSample into WinRT AudioFrame using IAudioFrameNativeFactory
		MULTI_QI qiFactory[1] = { { &__uuidof(IAudioFrameNativeFactory),nullptr,S_OK } };
		hr = CoCreateInstanceFromApp(CLSID_AudioFrameNativeFactory, nullptr, CLSCTX_INPROC_SERVER, nullptr, 1, qiFactory);
		if (FAILED(hr))
			return hr;
		if (FAILED(qiFactory[0].hr))
			return qiFactory[0].hr;

		ComPtr<IAudioFrameNativeFactory> spNativeFactory = (IAudioFrameNativeFactory *)qiFactory[0].pItf;

		// Now use the factory to create frame out of IMFSample
		hr = spNativeFactory->CreateFromMFSample(spSample.Get(), false, IID_PPV_ARGS(ppAudioFrame));

		return hr;

	}
	*/

	STDMETHODIMP CAnalyzerEffect::get_IsSuspended(boolean * pbIsSuspended)
	{
		if (pbIsSuspended == nullptr)
			return E_INVALIDARG;
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

		HRESULT hr = Analyzer_AllocateBuffers();
		if (FAILED(hr))
			return hr;

		// Initialize window, use Blackman-Nuttall window for low sidelobes
		float a0 = 0.3635819f, a1 = 0.4891775f, a2 = 0.1365995f, a3 = 0.0106411f;
		float pi = 3.14159f;
		for (size_t index = 0; index < m_StepTotalFrames; index++)
		{
			m_pWindow[index] = a0 - a1 * cosf(2 * pi * index / (m_StepTotalFrames - 1)) +
				a2 * cosf(4 * pi * index / (m_StepTotalFrames - 1)) -
				a3 * cosf(6 * pi * index / (m_StepTotalFrames - 1));
		}

		XDSP::FFTInitializeUnityTable(m_pFftUnityTable, m_FftLength);
		m_fFftScale = 2.0f / m_FftLength;

		// Configure the buffer
		m_InputBuffer.SetFrameSize(m_nChannels);
		m_InputBuffer.Configure(m_StepTotalFrames, m_StepFrameOverlap);

#ifdef _TRACE
		Diagnostics::Trace::Log_Initialize(m_FftLength,m_StepTotalFrames,m_StepFrameOverlap);
#endif
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_AllocateBuffers()
	{
		using namespace DirectX;

		Analyzer_FreeBuffers();	// First free any existing memory allocations

		m_pWindow = static_cast<float *>(malloc(m_StepTotalFrames * sizeof(float)));
		m_pFftReal = static_cast<XMVECTOR *>(_aligned_malloc(m_FftLength * sizeof(XMVECTOR) * m_nChannels, 16));	// For real data allocate space for all channels
		m_pFftUnityTable = static_cast<XMVECTOR *> (_aligned_malloc(2 * m_FftLength * sizeof(XMVECTOR), 16));	// Complex values 
		m_pFftBuffers = static_cast<XMVECTOR *>(_aligned_malloc(2 * m_FftLength * sizeof(XMVECTOR), 16));	// Preallocate buffers for FFT calculation 2*length of Fft

		if (m_pFftReal == nullptr || m_pWindow == nullptr || m_pFftUnityTable == nullptr || m_pFftBuffers == nullptr)
			return E_OUTOFMEMORY;

		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_FreeBuffers()
	{
		if (m_pFftReal != nullptr)
			_aligned_free(m_pFftReal);
		m_pFftReal = nullptr;
		if (m_pFftUnityTable != nullptr)
			_aligned_free(m_pFftUnityTable);
		m_pFftUnityTable = nullptr;
		if (m_pFftBuffers != nullptr)
			_aligned_free(m_pFftBuffers);
		m_pFftBuffers = nullptr;
		if (m_pWindow != nullptr)
			free(m_pWindow);
		m_pWindow = nullptr;

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

		// Allow processing after a reset event. No effect is processing is in an allowed state
		SetEvent(m_hResetWorkQueue);

		HRESULT hr = S_OK;
		if (m_InputBuffer.GetPosition() == -1)	// Sample index not set, get time from sample
		{
			REFERENCE_TIME sampleTime = 0;
			hr = pSample->GetSampleTime(&sampleTime);
			if (FAILED(hr))
				return hr;
			long position = time_to_frames(sampleTime);
			m_InputBuffer.SetPosition(position);
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

		m_InputBuffer.Add(pBufferData, cbCurrentLength / sizeof(float));

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
		DWORD dwWaitResult = WaitForSingleObject(m_hWQAccess, 0);
		if (dwWaitResult == WAIT_OBJECT_0) // 
		{
			return MFPutWorkItem2(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, 0, this, nullptr);
		}
		else if (dwWaitResult == WAIT_TIMEOUT)
		{
			return S_FALSE;	// Analysis is already running
		}
		else
			return E_FAIL;
	}

	HRESULT CAnalyzerEffect::Analyzer_Step(IMFAsyncResult * pResult)
	{
		// If in reset state do not even process anything
		DWORD dwWaitResult = WaitForSingleObject(m_hResetWorkQueue, 0);
		if (dwWaitResult != WAIT_OBJECT_0)
		{
			ReleaseSemaphore(m_hWQAccess, 1, nullptr);
			return S_OK;
		}

		ComPtr<IMFSample> spSample;
		HRESULT hr = Analyzer_Calculate(&spSample);

		auto lock = m_csOutputQueueAccess.Lock();
		dwWaitResult = WaitForSingleObject(m_hResetWorkQueue, 0);
		// Only push the result if reset is not pending
		if (hr == S_OK && dwWaitResult == WAIT_OBJECT_0)
		{
			// Manage queue size
			Analyzer_CompactOutputQueue();
			// TODO: Push sample
			// m_AnalyzerOutput.push(spSample);
#ifdef _TRACE
			Diagnostics::Trace::Log_OutputQueuePush(m_AnalyzerOutput.size());
#endif
		}

		if (hr != S_OK || dwWaitResult != WAIT_OBJECT_0 || m_bIsSuspended)
		{
			// Work is done, queue is empty, release the semaphore
			ReleaseSemaphore(m_hWQAccess, 1, nullptr);
			return S_OK;
		}

		// Schedule next work item
		return MFPutWorkItem2(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, 0, this, nullptr);
	}

	HRESULT CAnalyzerEffect::Analyzer_GetFromBuffer(float *pData, REFERENCE_TIME *pPosition)
	{
		auto readerLock = m_csInputIndexAccess.Lock();	// Get lock on modifying the reader pointer

		long currentPosition = m_InputBuffer.GetPosition();
		HRESULT hr = m_InputBuffer.Step(pData, m_FftLength, m_pWindow);
		if (FAILED(hr))
			return S_FALSE;

		*pPosition = frames_to_time(currentPosition);

		return S_OK;
	}

	// Calculate Fft and then the abs values for the first half of the elements (positive frequencies)
	// The results are placed back into pData
	void CAnalyzerEffect::Analyzer_CalculateFft(DirectX::XMVECTOR * pData, DirectX::XMVECTOR * pBuffers)
	{
		Diagnostics::Trace::Log_LineNumber(__LINE__);
		using namespace DirectX;
		using namespace XDSP;
		size_t fftVectorLength = m_FftLength >> 2;
		XMVECTOR *pImag = pBuffers;
		XMVECTOR *pRealUnswizzled = pBuffers + fftVectorLength;

		memset(pImag, 0, sizeof(float)*m_FftLength);	// Imaginary values are 0 for input

		Diagnostics::Trace::Log_LineNumber(__LINE__);
		FFT(pData, pImag, m_pFftUnityTable, m_FftLength);
		Diagnostics::Trace::Log_LineNumber(__LINE__);
		FFTUnswizzle(pRealUnswizzled, pData, m_FftLengthLog2);
		FFTUnswizzle(pData, pImag, m_FftLengthLog2); // Use input data for temporary buffer for reordered imaginary data

		XMVECTOR vRMS = XMVectorZero();
		size_t spectrLength = fftVectorLength >> 1;
		// Calculate abs value first half of FFT output and copy to output
		for (size_t vIndex = 0; vIndex < spectrLength; vIndex++)	// vector length is 4 times shorter, copy only positive frequency values
		{
			XMVECTOR vRR = XMVectorMultiply(pRealUnswizzled[vIndex], pRealUnswizzled[vIndex]);
			XMVECTOR vII = XMVectorMultiply(pData[vIndex], pData[vIndex]);	// pData is used as buffer for reordered imaginary values
			XMVECTOR vRRplusvII = XMVectorAdd(vRR, vII);
			XMVECTOR vAbs = XMVectorSqrtEst(vRRplusvII);
			XMVECTOR vScaledAbs = XMVectorScale(vAbs, m_fFftScale);
			pData[vIndex] = vScaledAbs;
			vRMS += vScaledAbs * vScaledAbs;
		}
		// Place RMS value after the output (which takes only half of the buffer)
		pData[spectrLength] = XMVectorSqrtEst(XMVectorSum(vRMS));
	}

	HRESULT CAnalyzerEffect::Analyzer_Calculate(IMFSample ** ppSample)
	{
		using namespace DirectX;

		// Not initialized
		if (m_nChannels == 0)
			return E_NOT_VALID_STATE;

		auto lock = m_csAnalyzerConfig.Lock();	// Lock object for config changes

#ifdef _TRACE
		ComPtr<ABI::Windows::Foundation::Diagnostics::ILoggingActivity> spActivity;
		Diagnostics::Trace::Log_StartCalculate(&spActivity,m_InputBuffer.GetPosition(),m_InputBuffer.GetLength());
		Diagnostics::CLogActivityHelper activity(spActivity.Get());
#endif

		REFERENCE_TIME hnsDataTime = 0;
		HRESULT hr = Analyzer_GetFromBuffer((float *)m_pFftReal, &hnsDataTime);
		if (hr != S_OK)
			return hr;
#ifdef _TRACE
		Diagnostics::Trace::Log_GetFromBuffer(hnsDataTime);
#endif
			// Create output media sample
		ComPtr<IMFMediaBuffer> spBuffer;
		// Create aligned buffer for vector math + 16 bytes for 8 floats for RMS values
		// In linear output length is going to be 1/2 of the FFT length
		size_t vOutputLength = (m_OutElementsCount + 3) >> 2;		// To use vector math allocate buffer for each channel which is rounded up to the next 16 byte boundary

		hr = MFCreateAlignedMemoryBuffer(DWORD((sizeof(XMVECTOR)*vOutputLength*m_nChannels) + 2 * sizeof(XMVECTOR)), 16, &spBuffer);
		if (FAILED(hr))
			return hr;

		spBuffer->SetCurrentLength(DWORD(sizeof(XMVECTOR)*(2 + m_nChannels * vOutputLength)));

		float *pBufferData;	// This is pointer to the media buffer data
		hr = spBuffer->Lock((BYTE **)&pBufferData, nullptr, nullptr);
		if (FAILED(hr))
			return hr;

		float *pRMSData = (float *)(pBufferData + m_nChannels * (vOutputLength << 2));
		memset(pRMSData, 0, sizeof(float) * 8);	// Zero the 8 RMS value slots

		// Process each channel now. FFT values are in the m_pFftReal
		for (size_t channelIndex = 0; channelIndex < m_nChannels; channelIndex++)
		{
			// Output data goes directly into the allocated buffer
			// One channel output is fftLength / 2, array vector size is fftLength / 4 hence fftLength >> 3
			float *pOutData = pBufferData + channelIndex * (vOutputLength << 2);

			DirectX::XMVECTOR *pFftData = m_pFftReal + channelIndex * (m_FftLength >> 2);
			Analyzer_CalculateFft(pFftData, m_pFftBuffers);
			// RMS data is placed right after the spectral data which is half the fft length
			pRMSData[channelIndex] = ((float *)(pFftData))[m_FftLength >> 1];

			Analyzer_LinearInterpolate((float*)pFftData, m_FftLength >> 1, pOutData, m_OutElementsCount);
		}
		// Scale the output value, we can use vector math as there is space after the buffer and it is aligned
		if (m_bUseLogAmpScale)
		{
			// Process all amplitude and RMS values in one pass
			Analyzer_ConvertToDb((XMVECTOR *)pBufferData, vOutputLength * m_nChannels + 2);
		}

		spBuffer->Unlock();

		hr = Analyzer_CreateOutputSample(ppSample,
			spBuffer.Get(),
			hnsDataTime,
			vOutputLength << 2);


		return hr;
	}

	// Creates media sample and sets necessary attributes
	HRESULT  CAnalyzerEffect::Analyzer_CreateOutputSample(
		IMFSample **ppSample, IMFMediaBuffer *pBuffer,
		REFERENCE_TIME time, size_t bufferStep)
	{
		HRESULT hr = MFCreateSample(ppSample);
		if (FAILED(hr))
			return hr;

		(*ppSample)->AddBuffer(pBuffer);
		(*ppSample)->SetSampleTime(time);
		(*ppSample)->SetSampleDuration(frames_to_time((long) m_StepFrameCount));

		(*ppSample)->SetUINT32(g_PropKey_RMS_Data_Offset,UINT32(m_nChannels * bufferStep));
		(*ppSample)->SetUINT32(g_PropKey_Data_Step, UINT32(bufferStep));
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_Reset()
	{
		auto inputLock = m_csInputIndexAccess.Lock();
		m_InputBuffer.Flush();
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_Flush()
	{
		// Dissallow processing and discard any output until new samples arrive
		ResetEvent(m_hResetWorkQueue);
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

	HRESULT CAnalyzerEffect::Analyzer_LinearInterpolate(const float *pInput, size_t inputSize, float *pOutput, size_t outputSize)
	{
		if (outputSize > inputSize)
			return E_INVALIDARG;
		if (inputSize == outputSize)
		{
			memcpy(pOutput, pInput, outputSize * sizeof(float));
		}

		float inStep = (float)inputSize / (float)outputSize;
		float inIndex = 0;
		float nextInIndex = inIndex + inStep;
		float scaler = 1 / inStep;

		for (size_t outIndex = 0; outIndex < outputSize; outIndex++)
		{
			int inValueIntIndex = (int)floor(inIndex);
			int inValueIntNextIndex = (int)floor(nextInIndex);

			float sum = 0;
			for (int index = inValueIntIndex + 1; index < inValueIntNextIndex && index < (int)inputSize; index++)
			{
				sum += pInput[index];
			}
			sum += pInput[inValueIntIndex] * (1 - inIndex + (float)inValueIntIndex);
			if (inValueIntNextIndex < (int)inputSize)
				sum += pInput[inValueIntNextIndex] * (nextInIndex - (float)inValueIntNextIndex);
			pOutput[outIndex] = sum * scaler;

			inIndex = nextInIndex;
			nextInIndex = inIndex + inStep;
		}
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_CompactOutputQueue()
	{
		Analyzer_FFwdQueueTo(GetPresentationTime(), nullptr);
		/* TODO 
		// Now manage queue size - remove items until the size is below limit
		while (m_AnalyzerOutput.size() > cMaxOutputQueueSize)
		{
			m_AnalyzerOutput.front() = nullptr;
			m_AnalyzerOutput.pop();
		}*/
		return S_OK;
	}

	HRESULT CAnalyzerEffect::Analyzer_FlushOutputQueue()
	{
		auto lock = m_csOutputQueueAccess.Lock();

		/* TODO
		while (!m_AnalyzerOutput.empty())
		{
			m_AnalyzerOutput.front() = nullptr;
			m_AnalyzerOutput.pop();
		}*/
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

	HRESULT CAnalyzerEffect::Analyzer_FFwdQueueTo(REFERENCE_TIME position, IMFSample ** ppSample)
	{
		if (position < 0)
			return S_FALSE;

		while (!m_AnalyzerOutput.empty())
		{
			REFERENCE_TIME sampleTime = 0, sampleDuration = 0;
			HRESULT hr = S_OK; // TODO: m_AnalyzerOutput.front()->GetSampleTime(&sampleTime);
			if (FAILED(hr))
				return hr;

			hr = S_OK; // TODO: m_AnalyzerOutput.front()->GetSampleDuration(&sampleDuration);
			if (FAILED(hr))
				return hr;

			if (position < sampleTime)
			{
				return S_FALSE; // Current position is before the frames in visualization queue - wait until we catch up
			}

			// Add 5uS (about half sample time @96k) to avoid int time math rounding errors
			if (position <= sampleDuration + sampleTime + 50L)
			{
				// TODO:
				/* if (ppSample != nullptr)	// If sample is requested, return the sample found
					m_AnalyzerOutput.front().CopyTo(ppSample);*/
				return S_OK;
			}
			else
			{
				if (position == 0)	// Do not use position 0 to manage queue elements as position can be set to 0 for a long time after stream starts
				{
					return S_FALSE;
				}
				// Current position is after the item in the queue - remove and continue searching
				// TODO:
				//m_AnalyzerOutput.front() = nullptr; // Dereference the pointer
				//m_AnalyzerOutput.pop();
			}
		}
		return S_FALSE;
	}

	DirectX::XMVECTOR g_vLog10DbScaler = DirectX::XMVectorReplicate(8.68588f); // This is 20.0/LogE(10)
																			   // {3F692E37-FC20-48DD-93D2-2234E1B1AA23}

	void CAnalyzerEffect::Analyzer_ConvertToDb(DirectX::XMVECTOR * pvData, size_t nElements)
	{
		using namespace DirectX;
		for (size_t vIndex = 0; vIndex < nElements; vIndex++)
		{
			XMVECTOR vClamped = XMVectorClamp(pvData[vIndex], DirectX::g_XMZero, XMVectorReplicate(FLT_MAX));
			XMVECTOR vLog = XMVectorLogE(vClamped) * g_vLog10DbScaler;
			pvData[vIndex] = XMVectorClamp(vLog, m_vClampAmpLow, m_vClampAmpHigh);
		}
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


