#include "pch.h"
#include "MediaAnalyzer.h"
#include "ScalarData.h"
#include "SpectrumData.h"
#include "VisualizationDataFrame.h"
#include <winrt/Windows.System.Threading.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include "Tracing.h"
#include <windows.media.core.interop.h>
#include <windows.media.audio.h>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfplat.lib")

namespace winrt::AudioVisualizer::implementation
{

	const size_t RingBufferSize = 960000;	// 10 sec worth of stereo audio at 48k


	void MediaAnalyzer::dataframe_queue::add(AudioVisualizer::VisualizationDataFrame const &frame)
	{
		std::lock_guard<std::mutex> lock(_outputQueueAccessMutex);
#ifdef _TRACE_
		Trace::MediaAnalyzer_OutputQueueAdd(frame,!_data.empty() ? _data.front() : nullptr, !_data.empty() ? _data.back(): nullptr, _data.size());
#endif
		_data.push(frame);
		compact();
	}

	void MediaAnalyzer::dataframe_queue::compact()
	{
		while (_data.size() > _maxQueueSize) {
#ifdef _TRACE_
			Trace::MediaAnalyzer_OutputQueueRemove(_data.front(),_data.size());
#endif
			_data.pop();
		}
	}

	void MediaAnalyzer::dataframe_queue::clear()
	{
		std::lock_guard<std::mutex> lock(_outputQueueAccessMutex);
		while (!_data.empty()) {
			_data.pop();
		}
		Trace::MediaAnalyzer_OutputQueueClear();
	}

	AudioVisualizer::VisualizationDataFrame  MediaAnalyzer::dataframe_queue::get(Windows::Foundation::TimeSpan time)
	{
		std::lock_guard<std::mutex> lock(_outputQueueAccessMutex);
		if (time.count() < 0)
			return nullptr;
#ifdef _TRACE_
		Trace::MediaAnalyzer_OutputQueueGet(time, !_data.empty() ? _data.front() : nullptr, !_data.empty() ? _data.back() : nullptr, _data.size());
#endif
		while (!_data.empty())
		{
			auto frontItem = winrt::from_abi<VisualizationDataFrame>(_data.front());
#ifdef _TRACE_
			Trace::MediaAnalyzer_OutputQueueTest(frontItem->Time(), time, time < frontItem->Time(), time >= frontItem->Time() + frontItem->Duration());
#endif
			if (time < _data.front().Time()) // Current position is before the visualization queue head - wait until we catch up
			{
#ifdef _TRACE_
				Trace::MediaAnalyzer_OutputQueueBehind(frontItem->Time());
#endif
				return nullptr; 
			}

			// Test if front item is matching. Add 5uS to avoid int time representation rounding errors
			if (time < _data.front().Time() + _data.front().Duration() + Windows::Foundation::TimeSpan(50))
			{
#ifdef _TRACE_
				Trace::MediaAnalyzer_OutputQueueItemFound(_data.front().Time());
#endif
				return _data.front();	// Return front item
			}
			else
			{
				// Cleanup queue
				if (time.count() == 0)	// Do not use position 0 to manage queue elements as position can be set to 0 for a long time after stream starts
					return nullptr;
			}
			// Current position is after the item in the queue - remove and continue searching
#ifdef _TRACE_
			Trace::MediaAnalyzer_OutputQueueRemove(_data.front(), _data.size());
#endif
			_data.pop();
		}
		return nullptr;
	}


	MediaAnalyzer::MediaAnalyzer() :
		m_FramesPerSecond(0),
		m_nChannels(0),
		_fftLength(2048),
		_fOutputFps(60.0f),
		_fInputOverlap(0.5f),
		_playbackState(SourcePlaybackState::Stopped),
		_analyzerTypes(AnalyzerType::All),
		_analyzerOutput(cMaxOutputQueueSize)
	{
		HRESULT hr = MFCreateAttributes(m_spMftAttributes.put(), 4);
		if (FAILED(hr))
			throw_hresult(hr);

		MULTI_QI qiFactory[1] = { { &__uuidof(IAudioFrameNativeFactory),nullptr,S_OK } };
		hr = CoCreateInstanceFromApp(CLSID_AudioFrameNativeFactory, nullptr, CLSCTX_INPROC_SERVER, nullptr, 1, qiFactory);
		check_hresult(hr);
		check_hresult(qiFactory[0].hr);
		
		*_audioFrameFactory.put_void() = qiFactory[0].pItf;
	}

	MediaAnalyzer::~MediaAnalyzer()
	{
		if (m_spPresentationClock != nullptr)
		{
			m_spPresentationClock->RemoveClockStateSink(this);
		}
	}


	void MediaAnalyzer::ConfigureSpectrum(uint32_t fftLength, float overlap)
	{
		if ((fftLength & fftLength - 1) != 0)	// FFT length needs to be power of 2
			throw hresult_invalid_argument();

		if (overlap < 0.0f || overlap > 1.0f)	// Set some sensible overlap limits
			throw hresult_invalid_argument();

		_fftLength = fftLength;
		_fInputOverlap = overlap;

		// If input type is set then calculate the necessary variables and initialize
		if (m_spInputType != nullptr) {
			CreateAnalyzer();
		}

		_configurationChangedEvent(*this, hstring(L""));
	}

	AudioVisualizer::VisualizationDataFrame MediaAnalyzer::GetData()
	{
		auto currentPosition = PresentationTime();
		if (currentPosition)
		{
			auto frame = _analyzerOutput.get(currentPosition.Value());
#ifdef _TRACE_
			Trace::MediaAnalyzer_GetFrame(currentPosition, frame);
#endif
			return frame;
		}
#ifdef _TRACE_
		Trace::MediaAnalyzer_GetFrame(currentPosition, nullptr);
#endif
		return nullptr;
	}

	bool MediaAnalyzer::IsSuspended()
	{
		return _analyzer ? _analyzer.IsSuspended() : false;
	}

	void MediaAnalyzer::IsSuspended(bool value)
	{
		if (_analyzer) {
			_analyzer.IsSuspended(value);
		}
	}

	float MediaAnalyzer::Fps()
	{
		return _fOutputFps;
	}

	void MediaAnalyzer::Fps(float)
	{
		throw hresult_not_implemented();
	}

	AudioVisualizer::AnalyzerType MediaAnalyzer::AnalyzerTypes()
	{
		return _analyzerTypes;
	}

	void MediaAnalyzer::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
	{
		_analyzerTypes = value;
		if (_analyzer) {
			_analyzer.AnalyzerTypes(_analyzerTypes);
		}
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> MediaAnalyzer::PresentationTime()
	{
		if (m_spPresentationClock != nullptr)
		{
			MFTIME presentationTime = 0;
			if (SUCCEEDED(m_spPresentationClock->GetTime(&presentationTime)))
			{
				return Windows::Foundation::TimeSpan(presentationTime);
			}
		}
		return nullptr;
	}

	AudioVisualizer::SourcePlaybackState MediaAnalyzer::PlaybackState()
	{
		return _playbackState;
	}

	Windows::Foundation::IReference<uint32_t> MediaAnalyzer::ActualFrequencyCount()
	{
		if (_analyzer) {
			return _analyzer.SpectrumElementCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<uint32_t> MediaAnalyzer::ActualChannelCount()
	{
		if (m_spInputType) {
			UINT32 channels = 0;
			m_spInputType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &channels);
			return channels;
		}
		return nullptr;
	}

	Windows::Foundation::IReference<float> MediaAnalyzer::ActualMinFrequency()
	{
		return 0.0f;
	}

	Windows::Foundation::IReference<float> MediaAnalyzer::ActualMaxFrequency()
	{
		if (_analyzer) {
			return _analyzer.SpectrumStep() * _analyzer.SpectrumElementCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> MediaAnalyzer::ActualFrequencyScale()
	{
		return ScaleType::Linear;
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

			com_ptr<IMFMediaType> spPartialMediaType;

			hr = MFCreateMediaType(spPartialMediaType.put());
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

			spPartialMediaType.copy_to(ppType);
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
#ifdef _TRACE_
		Trace::MediaAnalyzer_SetInputType(pType, dwFlags != 0);
#endif
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
			GUID majorType;
			HRESULT hr = pType->GetMajorType(&majorType);
			if (FAILED(hr))
				return hr;
			if (majorType != MFMediaType_Audio)
				return MF_E_INVALIDMEDIATYPE;
			GUID minorType;
			hr = pType->GetGUID(MF_MT_SUBTYPE, &minorType);
			if (FAILED(hr))
				return hr;
			if (minorType != MFAudioFormat_Float)
				return MF_E_INVALIDMEDIATYPE;

			// Media type is ok
			hr = S_OK;
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
#ifdef _TRACE_
		Trace::MediaAnalyzer_SetOutputType(pType, dwFlags != 0);
#endif
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
			CreateAnalyzer();
			_configurationChangedEvent(*this, hstring(L""));
			return S_OK;
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
		DWORD     /*dwStreamIndex*/,
		IMFMediaEvent *     /*pEvent*/
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
#ifdef _TRACE_
		Trace::MediaAnalyzer_ProcessMessage(eMessage);
#endif

		switch (eMessage)
		{
		case MFT_MESSAGE_COMMAND_FLUSH:
			// Flush the MFT. Flush might happen at the end of stream - keep the existing samples and
			// Flush the MFT at STREAM_STARTING instead
			_analyzer.Flush();
			m_spSample = nullptr;
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
			_analyzerOutput.clear();	// New streaming starts, clean output
			break;
		}

		return hr;
	}

	HRESULT MediaAnalyzer::ProcessInput(DWORD dwInputStreamID, IMFSample * pSample, DWORD dwFlags)
	{
#ifdef _TRACE_
		Trace::MediaAnalyzer_ProcessInput(pSample);
#endif
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
#ifdef _TRACE_
		Trace::MediaAnalyzer_ProcessOutput();
#endif
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

		auto audioFrame = ConvertToAudioFrame(m_spSample.get());
		_analyzer.ProcessInput(audioFrame);

		// Set status flags.
		pOutputSamples[0].dwStatus = 0;
		pOutputSamples[0].pSample = m_spSample.detach();
		*pdwStatus = 0;

		m_spSample = nullptr;

		return hr;
	}

	Windows::Media::AudioFrame MediaAnalyzer::ConvertToAudioFrame(IMFSample *pSample)
	{
		// Convert sample to audio frame
		com_ptr<ABI::Windows::Media::IAudioFrame> frame;
		check_hresult(_audioFrameFactory->CreateFromMFSample(pSample, true, IID_PPV_ARGS(frame.put())));
		return frame.as<Windows::Media::AudioFrame>();
	}

	HRESULT MediaAnalyzer::SetPresentationClock(IMFPresentationClock * pPresentationClock)
	{
		if (m_spPresentationClock != nullptr)
		{
			m_spPresentationClock->RemoveClockStateSink(this);
		}
		m_spPresentationClock.copy_from(pPresentationClock);
		if (m_spPresentationClock != nullptr)
			m_spPresentationClock->AddClockStateSink(this);

		return S_OK;
	}

	void MediaAnalyzer::CreateAnalyzer()
	{
		UINT32 sampleRate = 0;
		m_spInputType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &sampleRate);
		size_t stepFrameCount = (size_t)(sampleRate / _fOutputFps);
		size_t overlapFrames = (size_t) (_fInputOverlap * stepFrameCount);
		UINT32 channelCount = 0;
		m_spInputType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &channelCount);
		if (_analyzer) {
			_analyzer.Output(_analyzerOutputEvent);
		}
		_analyzer = make<AudioAnalyzer>(RingBufferSize, channelCount, sampleRate, stepFrameCount, overlapFrames, _fftLength, true);
		_analyzer.AnalyzerTypes(_analyzerTypes);
		_analyzerOutputEvent = _analyzer.Output(
			Windows::Foundation::TypedEventHandler<AudioVisualizer::AudioAnalyzer, AudioVisualizer::VisualizationDataFrame>(this, &MediaAnalyzer::OnAnalyzerOutput)
		);
	}

	void MediaAnalyzer::OnAnalyzerOutput(AudioVisualizer::AudioAnalyzer analyzer, AudioVisualizer::VisualizationDataFrame dataFrame)
	{
#ifdef _TRACE_
		Trace::MediaAnalyzer_AnalyzerOutput(dataFrame);
#endif
		_analyzerOutput.add(dataFrame);
	}
}
