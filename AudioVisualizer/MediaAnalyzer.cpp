#include "pch.h"
#include "MediaAnalyzer.h"
#include "ScalarData.h"
#include "SpectrumData.h"
#include "VisualizationDataFrame.h"
#include <winrt/Windows.System.Threading.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfplat.lib")

namespace winrt::AudioVisualizer::implementation
{
	const size_t CircleBufferSize = 960000;	// 10 sec worth of stereo audio at 48k

	MediaAnalyzer::MediaAnalyzer() :
		m_FramesPerSecond(0),
		m_nChannels(0),
		_bFlushPending(false),
		_threadPoolSemaphore(NULL),
		m_StepFrameCount(0),
		m_StepFrameOverlap(0),
		m_StepTotalFrames(0),
		m_FftLength(2048),
		m_FftLengthLog2(11),
		m_fOutputFps(60.0f),
		m_fInputOverlap(0.5f),
		m_bIsSuspended(false),
		_playbackState(SourcePlaybackState::Stopped),
		_analyzerTypes(AnalyzerType::All)
	{
		_analyzer = std::make_shared<::AudioVisualizer::AudioMath::CAudioAnalyzer>(CircleBufferSize);

		HRESULT hr = MFCreateAttributes(m_spMftAttributes.put(), 4);
		if (FAILED(hr))
			throw_hresult(hr);

		_threadPoolSemaphore = CreateSemaphore(nullptr, 1, 1, nullptr);
		if (_threadPoolSemaphore == NULL)
			throw hresult_error(E_FAIL);
	}

	MediaAnalyzer::~MediaAnalyzer()
	{
		if (m_spPresentationClock != nullptr)
		{
			m_spPresentationClock->RemoveClockStateSink(this);
		}
		CloseHandle(_threadPoolSemaphore);
	}


	void MediaAnalyzer::ConfigureSpectrum(uint32_t fftLength, float overlap)
	{
		if ((fftLength & fftLength - 1) != 0)	// FFT length needs to be power of 2
			throw hresult_invalid_argument();

		if (overlap < 0.0f || overlap > 1.0f)	// Set some sensible overlap limits
			throw hresult_invalid_argument();

		m_FftLength = fftLength;
		m_fInputOverlap = overlap;

		// If input type is set then calculate the necessary variables and initialize
		if (m_spInputType != nullptr)
			Analyzer_Initialize();

		_configurationChangedEvent(*this, hstring(L""));
	}

	AudioVisualizer::VisualizationDataFrame MediaAnalyzer::GetData()
	{
		MFTIME currentPosition = GetPresentationTime();

		if (currentPosition != -1)	// If no presentation position then return nullptr
		{
			std::lock_guard lock(_outputQueueAccessMutex);
			return Analyzer_FFwdQueueTo(currentPosition);
		}
		return nullptr;
	}

	bool MediaAnalyzer::IsSuspended()
	{
		return m_bIsSuspended;
	}

	void MediaAnalyzer::IsSuspended(bool value)
	{
		if (m_bIsSuspended != value)
		{
			if (value)
				Analyzer_Suspend();
			else
				Analyzer_Resume();
		}
	}

	float MediaAnalyzer::Fps()
	{
		return m_fOutputFps;
	}

	void MediaAnalyzer::Fps(float)
	{
		throw hresult_not_implemented();
	}

	AudioVisualizer::AnalyzerType MediaAnalyzer::AnalyzerTypes()
	{
		return _analyzerTypes;
	}

	void MediaAnalyzer::AnalyzerTypes(AudioVisualizer::AnalyzerType const&)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> MediaAnalyzer::PresentationTime()
	{
		REFERENCE_TIME time = GetPresentationTime();
		if (time != -1)
		{
			auto ts = winrt::Windows::Foundation::TimeSpan(time);
			return ts;
		}
		else
		{
			return nullptr;
		}
	}

	AudioVisualizer::SourcePlaybackState MediaAnalyzer::PlaybackState()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IReference<uint32_t> MediaAnalyzer::ActualFrequencyCount()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IReference<uint32_t> MediaAnalyzer::ActualChannelCount()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IReference<float> MediaAnalyzer::ActualMinFrequency()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IReference<float> MediaAnalyzer::ActualMaxFrequency()
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> MediaAnalyzer::ActualFrequencyScale()
	{
		throw hresult_not_implemented();
	}

	event_token MediaAnalyzer::ConfigurationChanged(winrt::Windows::Foundation::TypedEventHandler<IVisualizationSource,hstring> const& handler)
	{
		return _configurationChangedEvent.add(handler);
	}

	void MediaAnalyzer::ConfigurationChanged(event_token const& token)
	{
		_configurationChangedEvent.remove(token);
	}

	void MediaAnalyzer::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
	{
		configuration.Insert(hstring(L"Source"), *this);
	}

	//-------------------------------------------------------------------
	// GetStreamLimits
	// Returns the minum and maximum number of streams.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetStreamLimits(DWORD * pdwInputMinimum, DWORD * pdwInputMaximum, DWORD * pdwOutputMinimum, DWORD * pdwOutputMaximum)
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
	STDMETHODIMP MediaAnalyzer::GetStreamCount(DWORD * pcInputStreams, DWORD * pcOutputStreams)
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
	STDMETHODIMP MediaAnalyzer::GetStreamIDs(DWORD, DWORD *, DWORD, DWORD *)
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
	STDMETHODIMP MediaAnalyzer::GetInputStreamInfo(DWORD dwInputStreamID, MFT_INPUT_STREAM_INFO * pStreamInfo)
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
	STDMETHODIMP MediaAnalyzer::GetOutputStreamInfo(DWORD dwOutputStreamID, MFT_OUTPUT_STREAM_INFO * pStreamInfo)
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

		std::lock_guard lock(_mtxMftAccess);
		if (m_spOutputType != nullptr) {
			pStreamInfo->cbSize = MFGetAttributeUINT32(m_spOutputType.get(), MF_MT_AUDIO_BLOCK_ALIGNMENT, 0);
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetAttributes
	// Returns the attributes for the MFT.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetAttributes(IMFAttributes ** ppAttributes)
	{
		if (ppAttributes == NULL)
		{
			return E_POINTER;
		}
		std::lock_guard lock(_mtxMftAccess);
		m_spMftAttributes.copy_to(ppAttributes);
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetInputStreamAttributes
	// Returns stream-level attributes for an input stream.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetInputStreamAttributes(DWORD, IMFAttributes **)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// GetOutputStreamAttributes
	// Returns stream-level attributes for an output stream.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetOutputStreamAttributes(DWORD, IMFAttributes **)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// DeleteInputStream
	// Remove stream from processing
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::DeleteInputStream(DWORD)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// AddInputStreams
	// Add streams for processing
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::AddInputStreams(DWORD, DWORD *)
	{
		// This MFT does not support any stream-level attributes, so the method is not implemented.
		return E_NOTIMPL;
	}

	//-------------------------------------------------------------------
	// GetInputAvailableType
	// Returns a preferred input type.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetInputAvailableType(DWORD dwInputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType)
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
		std::lock_guard lock(_mtxMftAccess);

		// If the output type is set, return that type as our preferred input type.
		if (m_spOutputType == nullptr)
		{
			// The output type is not set. Create a partial media type.

			Microsoft::WRL::ComPtr<IMFMediaType> spPartialMediaType;

			hr = MFCreateMediaType(&spPartialMediaType);
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
			m_spOutputType.copy_to(ppType);
		}
		return hr;
	}

	//-------------------------------------------------------------------
	// GetOutputAvailableType
	// Returns a preferred output type.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetOutputAvailableType(DWORD dwOutputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType)
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
		std::lock_guard lock(_mtxMftAccess);

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
			m_spInputType.copy_to(ppType);
		}
		return hr;
	}

	//-------------------------------------------------------------------
	// SetInputType
	// Test and set input data type
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::SetInputType(DWORD dwInputStreamID, IMFMediaType * pType, DWORD dwFlags)
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

		std::lock_guard lock(_mtxMftAccess);

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
			m_spInputType.copy_from(pType);
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// SetOutputType
	// Test and set output data type
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::SetOutputType(DWORD dwOutputStreamID, IMFMediaType * pType, DWORD dwFlags)
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
			m_spOutputType.copy_from(pType);
			HRESULT hr = Analyzer_SetMediaType(pType);	// Using MFT output type to configure analyzer
			_configurationChangedEvent(*this, hstring(L""));
			return hr;
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetInputCurrentType
	// Returns the current input type.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetInputCurrentType(DWORD dwInputStreamID, IMFMediaType ** ppType)
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
			m_spInputType.copy_to(ppType);
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetOutputCurrentType
	// Returns the current output type.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetOutputCurrentType(DWORD dwOutputStreamID, IMFMediaType ** ppType)
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
			m_spOutputType.copy_to(ppType);
		}
		return S_OK;
	}

	//-------------------------------------------------------------------
	// GetInputStatus
	// Query if the MFT is accepting more input.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::GetInputStatus(
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
		std::lock_guard lock(_mtxMftAccess);

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
	STDMETHODIMP MediaAnalyzer::GetOutputStatus(DWORD *pdwFlags)
	{
		if (pdwFlags == nullptr)
		{
			return E_POINTER;
		}
		std::lock_guard lock(_mtxMftAccess);

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
	STDMETHODIMP MediaAnalyzer::SetOutputBounds(LONGLONG, LONGLONG)
	{
		return E_NOTIMPL;	// This method is not implemented for this MFT
	}

	//-------------------------------------------------------------------
	// ProcessEvent
	// Sends an event to an input stream.
	//-------------------------------------------------------------------
	STDMETHODIMP MediaAnalyzer::ProcessEvent(
		DWORD              ,
		IMFMediaEvent      *
	)
	{
		return S_OK;
	}


	//-------------------------------------------------------------------
	// ProcessMessage
	//-------------------------------------------------------------------
	HRESULT MediaAnalyzer::ProcessMessage(
		MFT_MESSAGE_TYPE    eMessage,
		ULONG_PTR
	)
	{
		std::lock_guard lock(_mtxMftAccess);

		HRESULT hr = S_OK;

		//Diagnostics::Trace::Log_MftProcessMessage(eMessage, ulParam);

		switch (eMessage)
		{
		case MFT_MESSAGE_COMMAND_FLUSH:
			// Flush the MFT. Flush might happen at the end of stream - keep the existing samples and
			// Flush the MFT at STREAM_STARTING instead
			Analyzer_Flush();
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
			hr = Analyzer_ClearOutputQueue();	// New streaming starts, clean output
			break;
		}

		return hr;
	}

	HRESULT MediaAnalyzer::ProcessInput(DWORD dwInputStreamID, IMFSample * pSample, DWORD dwFlags)
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

		std::lock_guard lock(_mtxMftAccess);

		// Cache the sample. We do the actual work in ProcessOutput.
		m_spSample.copy_from(pSample);

		return hr;
	}
	HRESULT MediaAnalyzer::ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER * pOutputSamples, DWORD * pdwStatus)
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

		std::lock_guard lock(_mtxMftAccess);

		if (m_spSample == nullptr)
		{
			return MF_E_TRANSFORM_NEED_MORE_INPUT;
		}

		hr = Analyzer_ProcessSample(m_spSample.get());

		pOutputSamples->pSample = m_spSample.get();
		m_spSample = nullptr;

		// Set status flags.
		pOutputSamples[0].dwStatus = 0;
		*pdwStatus = 0;

		return hr;
	}

	HRESULT MediaAnalyzer::SetPresentationClock(IMFPresentationClock * pPresentationClock)
	{
		if (m_spPresentationClock != nullptr)
		{
			m_spPresentationClock->RemoveClockStateSink(this);
		}
		m_spPresentationClock.copy_from(pPresentationClock);
		//Diagnostics::Trace::Log_SetPresentationClock(pPresentationClock);
		if (m_spPresentationClock != nullptr)
			m_spPresentationClock->AddClockStateSink(this);

		return S_OK;
	}

	HRESULT MediaAnalyzer::Analyzer_TestInputType(IMFMediaType * pMediaType)
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

	HRESULT MediaAnalyzer::Analyzer_SetMediaType(IMFMediaType * pType)
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

	HRESULT MediaAnalyzer::Analyzer_Initialize()
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
		Diagnostics::Trace::Log_Initialize(m_FftLength, m_StepTotalFrames, m_StepFrameOverlap, _analyzer->GetDownsampleRate());
#endif
		return S_OK;
	}

	HRESULT MediaAnalyzer::Analyzer_ProcessSample(IMFSample * pSample)
	{
		if (m_FramesPerSecond == 0 || m_nChannels == 0)
			return E_NOT_VALID_STATE;
		if (m_FftLength == 0)
			return E_NOT_VALID_STATE;

#ifdef _TRACE
		Diagnostics::Trace::Log_ProcessSample(pSample);
#endif

		// Allow processing after a flush event.
		_bFlushPending = false;

		std::lock_guard lock(_mtxMftAccess);
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

		_analyzer->AddInput(pBufferData, cbCurrentLength / sizeof(float), position);

		spBuffer->Unlock();

		Analyzer_ScheduleProcessing();

		return S_OK;
	}

	//-----------------------------------------------
	// ScheduleInputProcessing
	// Validate if background processing is running and if not initiate
	//-----------------------------------------------
	HRESULT MediaAnalyzer::Analyzer_ScheduleProcessing()
	{
		if (m_bIsSuspended)	// Do not schedule another work item when suspended
			return S_OK;

		// See if the access semaphore is signaled
		DWORD dwWaitResult = WaitForSingleObject(_threadPoolSemaphore, 0);
		if (dwWaitResult == WAIT_OBJECT_0)
		{
			// Execute data processing on threadpool
			Windows::System::Threading::ThreadPool::RunAsync(
				Windows::System::Threading::WorkItemHandler(
					[this] (Windows::Foundation::IAsyncAction action) {
				Analyzer_ProcessData();
				ReleaseSemaphore(_threadPoolSemaphore, 1, nullptr);
				}),
				Windows::System::Threading::WorkItemPriority::High
			);
			return S_OK;
		}
		else if (dwWaitResult == WAIT_TIMEOUT)
		{
			return S_FALSE;	// Analysis is already running
		}
		else
			return E_FAIL;
	}

	void MediaAnalyzer::Analyzer_ProcessData()
	{
		// Process data until not suspended, not in a reset and output is available
		while (!m_bIsSuspended && !_bFlushPending && _analyzer->IsOutputAvailable())
		{
			long position = -1;
			bool bStepSuccess = false;
			com_ptr<implementation::ScalarData> rms = nullptr;
			com_ptr<implementation::ScalarData> peak = nullptr;
			com_ptr<implementation::SpectrumData> spectrum = nullptr;

			if (1) {	// This dummy if statement is needed for the scoped lock

				std::lock_guard lock(_analyzerAccessMutex);
				if ((int)_analyzerTypes & (int)AnalyzerType::RMS)
					rms = make_self<ScalarData>(m_nChannels);

				if ((int)_analyzerTypes & (int)AnalyzerType::Peak)
					peak = make_self<ScalarData>(m_nChannels);

				if ((int)_analyzerTypes & (int)AnalyzerType::Spectrum)
				{
					float maxFreq = (float)(m_FramesPerSecond >> 1) / (float)_analyzer->GetDownsampleRate();

					spectrum = make_self<SpectrumData>(
						m_nChannels,
						m_FftLength >> 1,
						ScaleType::Linear,
						ScaleType::Linear,
						0.0f,
						maxFreq,
						false);
				}

				bStepSuccess = _analyzer->Step(&position, rms->_pData, peak->_pData, spectrum->_pData);
				// End of scoped lock for analyzer access
			}

			
			auto dataFrame = make_self<VisualizationDataFrame>(
				Windows::Foundation::TimeSpan(frames_to_time(position)),
				Windows::Foundation::TimeSpan((REFERENCE_TIME)(1e7 / m_fOutputFps)),
				rms.as<AudioVisualizer::ScalarData>(),
				peak.as<AudioVisualizer::ScalarData>(),
				spectrum.as<AudioVisualizer::SpectrumData>()
				);

			// Only push the result if reset is not pending
			if (!_bFlushPending && bStepSuccess)
			{
				std::lock_guard lock(_outputQueueAccessMutex);
				Analyzer_CompactOutputQueue();
				m_AnalyzerOutput.push(dataFrame);
			}
		}
	}

	HRESULT MediaAnalyzer::Analyzer_Flush()
	{
		// Dissallow processing and discard any output until new samples 
		_bFlushPending = true;
		// Release input sample and reset the analyzer and queues
		// Clean up any state from buffer copying
		std::lock_guard lock(_analyzerAccessMutex);
		_analyzer->Flush();
		m_spSample = nullptr;
		return S_OK;
	}

	HRESULT MediaAnalyzer::Analyzer_CompactOutputQueue()
	{
		Analyzer_FFwdQueueTo(GetPresentationTime());
		// Now manage queue size - remove items until the size is below limit
		while (m_AnalyzerOutput.size() > cMaxOutputQueueSize)
		{
			m_AnalyzerOutput.pop();	// TODO: Make sure release is called here
		}
		return S_OK;
	}

	HRESULT MediaAnalyzer::Analyzer_ClearOutputQueue()
	{
		std::lock_guard lock(_outputQueueAccessMutex);
		while (!m_AnalyzerOutput.empty())
		{
			m_AnalyzerOutput.pop();
		}
		return S_OK;
	}

	void MediaAnalyzer::Analyzer_Resume()
	{
		m_bIsSuspended = false;
		Analyzer_ScheduleProcessing();
	}

	void MediaAnalyzer::Analyzer_Suspend()
	{
		m_bIsSuspended = true;
	}

	AudioVisualizer::VisualizationDataFrame MediaAnalyzer::Analyzer_FFwdQueueTo(REFERENCE_TIME position)
	{
		if (position < 0)
			return nullptr;

		while (!m_AnalyzerOutput.empty())
		{
			//TimeSpan time = { 0 }, duration = { 0 };
			//ComPtr<IReference<TimeSpan>> frameTime, frameDuration;

			if (m_AnalyzerOutput.front()->IsBefore(position))
			{
				return nullptr; // Current position is before the frames in visualization queue - wait until we catch up
			}

			// Add 5uS (about half sample time @96k) to avoid int time math rounding errors
			if (!m_AnalyzerOutput.front()->IsAfter(position))	// If frame is not after current position there is a match
			{
				auto frame = m_AnalyzerOutput.front();
				return frame.as<AudioVisualizer::VisualizationDataFrame>();
			}
			else
			{
				// Cleanup queue
				if (position == 0)	// Do not use position 0 to manage queue elements as position can be set to 0 for a long time after stream starts
					return nullptr;
			}
			// Current position is after the item in the queue - remove and continue searching
			m_AnalyzerOutput.pop();
		}
		return nullptr;
	}



	REFERENCE_TIME MediaAnalyzer::GetPresentationTime()
	{
		MFTIME presentationTime = -1;
		HRESULT hr = S_OK;
		if (m_spPresentationClock != nullptr)
		{
			hr = m_spPresentationClock->GetTime(&presentationTime);
		}
		return presentationTime;
	}
}
