#include "pch.h"
#include "AudioAnalyzer.h"
#include <DirectXMath.h>
#include "XDSP.h"
#include <memorybuffer.h>
#include <winrt/windows.system.threading.h>
#include "ScalarData.h"
#include "SpectrumData.h"
#include "VisualizationDataFrame.h"
#include "util.h"
#include "tracing.h"

using namespace DirectX;
using namespace XDSP;


// This function is moved out of class due to a compiler bug (vs 15.7.3) where
// compiler is unable to resolve XMVECTOR operator overloads when moved inside the namespace definition block below
void CreateBlackmannNuttalWindow(XMVECTOR *pWindow,size_t nElements)
{
	// Initialize window, use Blackman-Nuttall window for low sidelobes
	float a0 = 0.3635819f,a1 = 0.4891775f,a2 = 0.1365995f,a3 = 0.0106411f;
	XMVECTOR va0 = XMVectorReplicate(a0);
	XMVECTOR vElementIndex = XMVectorSet(0, 1, 2, 3), vElementStep = XMVectorReplicate(4.0f);
	float fIndexScaler = 1.0f / (float)(nElements-1);
	size_t vWindowElements = (nElements + 3) >> 2;
	for (size_t vIndex = 0; vIndex < vWindowElements; vIndex++, vElementIndex += vElementStep)
	{
		XMVECTOR vCosArg = XMVectorScale(DirectX::g_XMTwoPi * vElementIndex, fIndexScaler);
		pWindow[vIndex] = va0 - a1 * XMVectorCos(vCosArg) + a2 * XMVectorCos(vCosArg * 2.0f) - a3 * XMVectorCos(vCosArg * 3.0f);
	}
}

namespace winrt::AudioVisualizer::implementation
{
	AudioAnalyzer::AudioAnalyzer(uint32_t bufferSize, uint32_t inputChannels, uint32_t sampleRate, uint32_t inputStep, uint32_t inputOverlap, uint32_t fftLength,bool asyncProcessing) :
		_inputChannels(0),
		_sampleRate(0),
		_fftLength(0),
		_stepFrames(0),
		_fOutSampleRate(0.0f),
		_overlapFrames(0),
		_pWindow(nullptr),
		_pFftReal(nullptr),
		_pFftUnityTable(nullptr),
		_pFftBuffers(nullptr),
		_analyzerTypes(AnalyzerType::All),
		_bIsSuspended(false),
		_bIsFlushPending(false)
	{
		if (inputChannels == 0 || sampleRate == 0 || inputStep == 0 || fftLength == 0)
		{
			throw hresult_invalid_argument();
		}

		if (bufferSize < (inputStep + inputOverlap))
		{
			throw hresult_invalid_argument(L"Invalid buffer size");
		}
		if ((fftLength & (fftLength - 1)) != 0)
		{
			throw hresult_invalid_argument(L"FFT length has to be a power of 2");
		}

		_bIsClosed = false;
		_asyncProcessing = asyncProcessing;
		_threadPoolSemaphore = CreateSemaphore(nullptr, 1, 1, nullptr);
		if (_threadPoolSemaphore == INVALID_HANDLE_VALUE)
			throw hresult_error(E_FAIL);

		_inputChannels = inputChannels;
		_sampleRate = sampleRate;
		_fOutSampleRate = (float)sampleRate / (float)inputStep;


		_fftLength = fftLength;
		_fFftScale = 2.0f / _fftLength;

		_fftLengthLog2 = 1;
		while (1U << _fftLengthLog2 != _fftLength)
			_fftLengthLog2++;

		// Calculate downsample factor, if FFT length is too short for step+overlap then downsample input
		UINT32 downsampleFactor = 1;
		while ((inputStep + inputOverlap) / downsampleFactor > fftLength)
			downsampleFactor++;

		_inputBuffer = ::AudioVisualizer::ring_buffer(bufferSize, inputChannels, inputStep, inputOverlap, downsampleFactor);

		_stepFrames = inputStep / downsampleFactor;
		_overlapFrames = inputOverlap / downsampleFactor;
		
		InitWindow();
		_pFftReal = static_cast<XMVECTOR *>(_aligned_malloc_dbg(_fftLength * sizeof(XMVECTOR) * _inputChannels, 16, __FILE__, __LINE__));	// For real data allocate space for all channels
		_pFftUnityTable = static_cast<XMVECTOR *> (_aligned_malloc_dbg(2 * _fftLength * sizeof(XMVECTOR), 16, __FILE__, __LINE__));	// Complex values 
		_pFftBuffers = static_cast<XMVECTOR *>(_aligned_malloc_dbg(2 * _fftLength * sizeof(XMVECTOR), 16, __FILE__, __LINE__));	// Preallocate buffers for FFT calculation 2*length of Fft
		if (_pFftReal == nullptr || _pFftUnityTable == nullptr || _pFftBuffers == nullptr)
			throw std::bad_alloc();
		XDSP::FFTInitializeUnityTable(_pFftUnityTable, _fftLength);
	}

	void AudioAnalyzer::InitWindow()
	{
		if (_pWindow != nullptr) {
			_aligned_free_dbg(_pWindow);
		}
		_pWindow = static_cast<XMVECTOR *>(_aligned_malloc_dbg((((_stepFrames + _overlapFrames) + 3) >> 2) * sizeof(XMVECTOR), 16, __FILE__, __LINE__));
		if (_pWindow == nullptr) {
			throw std::bad_alloc();
		}		
		CreateBlackmannNuttalWindow(_pWindow, _stepFrames + _overlapFrames);
	}

	void AudioAnalyzer::FreeBuffers()
	{
		if (_pFftReal != nullptr)
			_aligned_free(_pFftReal);
		_pFftReal = nullptr;
		if (_pFftUnityTable != nullptr)
			_aligned_free(_pFftUnityTable);
		_pFftUnityTable = nullptr;
		if (_pFftBuffers != nullptr)
			_aligned_free(_pFftBuffers);
		_pFftBuffers = nullptr;
		if (_pWindow != nullptr)
			_aligned_free(_pWindow);
		_pWindow = nullptr;
	}

	void AudioAnalyzer::ProcessInput(Windows::Media::AudioFrame const& frame)
	{
		if (_inputChannels == 0) {
			throw hresult_error(E_NOT_VALID_STATE, L"Need to call configure first");	// Not initalized
		}
		if (!frame) {
			throw hresult_invalid_argument();
		}
		Trace::AudioAnalyzer_ProcessInput(frame);
		AddInput(frame);
		StartProcessing();
	}

	void AudioAnalyzer::AddInput(const winrt::Windows::Media::AudioFrame & frame)
	{
		std::lock_guard<std::mutex> _lock(_inputBufferAccess);

		if (_bSeedPosition) {
			if (_seedPosition) {
				_inputBuffer.readPositionFrameIndex = _seedPosition.Value();
				_bSeedPosition = false;
			} 
			else  {
				_inputBuffer.readPositionFrameIndex = frame.RelativeTime() ? time_to_frames(frame.RelativeTime().Value()) : 0;
				_bSeedPosition = false;
			} 
		}
		_bIsFlushPending = false;
		auto buffer = frame.LockBuffer(Windows::Media::AudioBufferAccessMode::Read);
		auto bufferReference = buffer.CreateReference();
		auto memoryBuffer = bufferReference.as<::Windows::Foundation::IMemoryBufferByteAccess>();

		float *pData = nullptr;
		UINT32 capacity = 0;
		memoryBuffer->GetBuffer((BYTE **)&pData, &capacity);

		_inputBuffer.add_samples(pData, buffer.Length() / sizeof(float));

		bufferReference.Close();
		buffer.Close();
	}

	void AudioAnalyzer::StartProcessing()
	{
		if (_bIsSuspended)
			return;

		if (_asyncProcessing) {
			ScheduleProcessing();
		}
		else {
			AnalyzeData();
		}
	}

	void AudioAnalyzer::ScheduleProcessing()
	{
		// See if the access semaphore is signaled
		DWORD dwWaitResult = WaitForSingleObject(_threadPoolSemaphore, 0);
		if (dwWaitResult == WAIT_OBJECT_0)
		{
			Trace::AudioAnalyzer_RunAsync();
			// Execute data processing on threadpool
			Windows::System::Threading::ThreadPool::RunAsync(
				Windows::System::Threading::WorkItemHandler(
					[=](Windows::Foundation::IAsyncAction /*action*/) {
				AnalyzeData();
				ReleaseSemaphore(_threadPoolSemaphore, 1, nullptr);
			}
				), Windows::System::Threading::WorkItemPriority::High
			);
		}
		else if (dwWaitResult == WAIT_TIMEOUT)
		{
			return;	// Analysis is already running
		}
		else
			throw hresult_error(HRESULT_FROM_WIN32(GetLastError()));
	}

	void AudioAnalyzer::AnalyzeData()
	{
		// Process data until not suspended, not in a reset and output is available
		while (!_bIsSuspended && !_bIsFlushPending)
		{
			auto activity = Trace::AudioAnalyzer_Calculate();
			com_ptr<implementation::ScalarData> rms = nullptr;
			com_ptr<implementation::ScalarData> peak = nullptr;
			com_ptr<implementation::SpectrumData> spectrum = nullptr;
			Windows::Foundation::IReference<int64_t> dataFrameIndex;

			if (true) {	// Dummy if statement just for lock scope for coping from input buffer
				std::scoped_lock<std::mutex> _lock(_inputBufferAccess);

				if (_inputBuffer.empty())	// Nothing more to process, return
					return;

				// Copy next data frame to the real part of fft
				dataFrameIndex = _inputBuffer.readPositionFrameIndex;
				_inputBuffer.get_deinterleaved((float*)_pFftReal, _fftLength);
			}

			if (util::enum_has_flag(_analyzerTypes, AnalyzerType::RMS)) {
				rms = make_self<ScalarData>(_inputChannels);
			}

			if (util::enum_has_flag(_analyzerTypes, AnalyzerType::Peak)) {
				peak = make_self<ScalarData>(_inputChannels);
			}

			if (util::enum_has_flag(_analyzerTypes, AnalyzerType::Spectrum))
			{
				float maxFreq = (float)(_sampleRate >> 1) / (float)_inputBuffer.downsampleRate();

				spectrum = make_self<SpectrumData>(
						_inputChannels,
						_fftLength >> 1,
						ScaleType::Linear,
						ScaleType::Linear,
						0.0f,
						maxFreq,
						false);
			}	

			Calculate(rms ? rms->_pData : nullptr, peak ? peak->_pData : nullptr, spectrum ? spectrum->_pData : nullptr);

			Windows::Foundation::TimeSpan time;
			if (dataFrameIndex) {
				time = frames_to_time(dataFrameIndex.Value());
			}
			auto dataFrame = make<VisualizationDataFrame>(
				time,
				Windows::Foundation::TimeSpan((REFERENCE_TIME)(1e7 / _fOutSampleRate)),
				rms ? rms.as<AudioVisualizer::ScalarData>() : nullptr,
				peak ? peak.as<AudioVisualizer::ScalarData>() : nullptr,
				spectrum ? spectrum.as<AudioVisualizer::SpectrumData>() : nullptr
				);

			activity.StopActivity(activity.Name);
			// Only push the result if reset is not pending
			if (!_bIsFlushPending)
			{
				_output(*this, dataFrame);
			}
		}
	}

	void AudioAnalyzer::Calculate(XMVECTOR *pRms, DirectX::XMVECTOR *pPeak, DirectX::XMVECTOR *pSpectrum)
	{
		size_t vStep = _fftLength >> 2;

		if (pRms != nullptr || pPeak != nullptr)
		{
			size_t vFromFrame = _overlapFrames >> 2, vToFrame = (_stepFrames + _overlapFrames) >> 2;
			float rmsScaler = 1.0f / _stepFrames;

			for (size_t channelIndex = 0, vOffset = 0; channelIndex < _inputChannels; channelIndex++, vOffset += vStep)
			{
				XMVECTOR vRMSSum = XMVectorZero();
				XMVECTOR vPeak = XMVectorZero();
				for (size_t vIndex = vFromFrame + vOffset; vIndex < vToFrame + vOffset; vIndex++)
				{
					XMVECTOR vValue = _pFftReal[vIndex];
					vRMSSum += vValue * vValue;
					vPeak = XMVectorMax(vPeak, XMVectorAbs(vValue));
				}
				if (pRms != nullptr)
				{
					XMVECTOR vRMS = XMVectorSqrt(XMVectorScale(XMVectorSum(vRMSSum), rmsScaler));
					((float *)pRms)[channelIndex] = XMVectorGetX(vRMS);
				}
				if (pPeak != nullptr)
				{
					float peakValue = std::max(std::max(std::max(XMVectorGetByIndex(vPeak, 0), XMVectorGetByIndex(vPeak, 1)), XMVectorGetByIndex(vPeak, 2)), XMVectorGetByIndex(vPeak, 3));
					((float*)pPeak)[channelIndex] = peakValue;
				}
			}
		}
		if (pSpectrum != nullptr)
		{
			size_t spectrumFrames = _stepFrames + _overlapFrames;
			size_t vSpectrumFrames = (spectrumFrames + 3) >> 2;
			
			// First window the data
			for (size_t channelIndex = 0, vBaseIndex = 0; channelIndex < _inputChannels; channelIndex++, vBaseIndex += vStep)
			{
				for (size_t vIndex = 0, vElementIndex=vBaseIndex; vIndex < vSpectrumFrames; vIndex++,vElementIndex++)
				{
					_pFftReal[vElementIndex] *= _pWindow[vIndex];
				}
			}
			for (size_t channelIndex = 0; channelIndex < _inputChannels; channelIndex++)
			{
				XMVECTOR *pData = _pFftReal + channelIndex * vStep;
				XMVECTOR *pOutput = pSpectrum + channelIndex * (vStep >> 1);

				XMVECTOR *pImag = _pFftBuffers;
				XMVECTOR *pRealUnswizzled = _pFftBuffers + vStep;

				memset(pImag, 0, sizeof(float)*_fftLength);	// Imaginary values are 0 for input

				XDSP::FFT(pData, pImag, _pFftUnityTable, _fftLength);
				XDSP::FFTUnswizzle(pRealUnswizzled, pData, _fftLengthLog2);
				XDSP::FFTUnswizzle(pData, pImag, _fftLengthLog2); // Use input data for temporary buffer for reordered imaginary data

																		// Calculate abs value first half of FFT output and copy to output
				for (size_t vIndex = 0; vIndex < vStep >> 1; vIndex++)	// vector length is 4 times shorter, copy only positive frequency values
				{
					XMVECTOR vRR = XMVectorMultiply(pRealUnswizzled[vIndex], pRealUnswizzled[vIndex]);
					XMVECTOR vII = XMVectorMultiply(pData[vIndex], pData[vIndex]);	// pData is used as buffer for reordered imaginary values
					XMVECTOR vRRplusvII = XMVectorAdd(vRR, vII);
					XMVECTOR vAbs = XMVectorSqrtEst(vRRplusvII);
					XMVECTOR vScaledAbs = XMVectorScale(vAbs, _fFftScale);
					pOutput[vIndex] = vScaledAbs;
				}
			}
		}
	}

	AudioVisualizer::AnalyzerType AudioAnalyzer::AnalyzerTypes()
	{
		return _analyzerTypes;
	}

	void AudioAnalyzer::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
	{
		_analyzerTypes = value;
	}

	float AudioAnalyzer::SpectrumStep()
	{
		return (float)_sampleRate / (float)_fftLength / (float)_inputBuffer.downsampleRate();
	}
	uint32_t AudioAnalyzer::SpectrumElementCount()
	{
		return _fftLength >> 1;
	}

	
	void AudioAnalyzer::Flush()
	{
		std::scoped_lock<std::mutex> _lock(_inputBufferAccess);
		_seedPosition = nullptr;	// Get from stream
		_bIsFlushPending = true;
		_inputBuffer.flush();
	}
	
	void AudioAnalyzer::Flush(int64_t seedPosition)
	{
		std::scoped_lock<std::mutex> _lock(_inputBufferAccess);
		_seedPosition = seedPosition;
		_bIsFlushPending = true;
		_inputBuffer.flush();
	}

	bool AudioAnalyzer::IsSuspended()
	{
		return _bIsSuspended;
	}

	void AudioAnalyzer::IsSuspended(bool value)
	{
		if (value != _bIsSuspended) {
			_bIsSuspended = value;
			if (!value) {
				StartProcessing();
			}
		}
	}

	event_token AudioAnalyzer::Output(Windows::Foundation::TypedEventHandler<AudioVisualizer::AudioAnalyzer, AudioVisualizer::VisualizationDataFrame> const& handler)
	{
		return _output.add(handler);
	}

	void AudioAnalyzer::Output(event_token const& token)
	{
		_output.remove(token);
	}

	void AudioAnalyzer::Close()
	{
		if (!_bIsClosed) {
			FreeBuffers();
			_bIsClosed = true;
		}
	}
}
