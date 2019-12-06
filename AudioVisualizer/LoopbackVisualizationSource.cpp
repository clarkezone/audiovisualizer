#include "pch.h"
#include "LoopbackVisualizationSource.h"
#include "AudioInterfaceActivator.h"
#include <pplawait.h>
#include <winrt/Windows.Media.Devices.h>
#include <chrono>

namespace winrt::AudioVisualizer::implementation
{
	void LoopbackVisualizationSource::InitWorkQueue()
	{
		// Prepare work queue 
		// Use regular priority queue
		check_hresult(MFLockSharedWorkQueue(L"", 0, nullptr, &_workQueueId));
		// Create async result that can be shared across wait operations as there is only one active at a time
		check_hresult(MFCreateAsyncResult(nullptr, (IMFAsyncCallback*)(this), nullptr, _asyncResult.put()));
		
		_callbackEvent = winrt::handle(::CreateEventEx(0, 0, 0, EVENT_ALL_ACCESS));
		_audioClient->SetEventHandle(_callbackEvent.get());
	}
	void LoopbackVisualizationSource::InitAnalyzer(uint32_t fftLength,float overlap)
	{
		// Remove existing event handler
		if (_analyzer) {
			_analyzer.Output(_analyzerOutputEvent);
		}
		// Initialize the analyzer
		uint32_t inputStep = (uint32_t)((float)_inputSampleRate / Fps());
		uint32_t inputOverlap = (uint32_t)(overlap * inputStep);
		uint32_t bufferSize = 2 * (inputStep + inputOverlap);
		_analyzer = make<AudioAnalyzer>(bufferSize,_inputChannels,_inputSampleRate,inputStep,inputOverlap,fftLength,true);
		_analyzerOutputEvent = _analyzer.Output({ this,&LoopbackVisualizationSource::OnAnalyzerOutput });
	}
	void LoopbackVisualizationSource::ScheduleWorkItem()
	{
		check_hresult(MFPutWaitingWorkItem(_callbackEvent.get(), 0, _asyncResult.get(), &_workItemKey));
	}
	void LoopbackVisualizationSource::CancelWorkItem()
	{
		check_hresult(MFCancelWorkItem(_workItemKey));
	}
	void LoopbackVisualizationSource::OnAnalyzerOutput(AudioVisualizer::AudioAnalyzer const&, AudioVisualizer::VisualizationDataFrame const& frame)
	{
		std::lock_guard<std::mutex> lock(_outputFrameLock);
		_outputFrame = frame;
	}

	LoopbackVisualizationSource::LoopbackVisualizationSource(winrt::com_ptr<::IAudioClient3> const& audioClient)
	{
		_audioClient = audioClient;
		// Initialize with default period
		REFERENCE_TIME defaultPeriod{ 0 };
		check_hresult(_audioClient->GetDevicePeriod(&defaultPeriod, nullptr));
		WAVEFORMATEX* pFormat {nullptr};
		check_hresult(_audioClient->GetMixFormat(&pFormat));

		// This should never fail as internal mix format is always float, but just in case
		check_bool(pFormat->wBitsPerSample == 32);

		check_hresult(_audioClient->Initialize(
			AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED,
			AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_LOOPBACK, 0, defaultPeriod, pFormat, nullptr));

		_inputSampleRate = pFormat->nSamplesPerSec;
		_inputChannels = pFormat->nChannels;

		CoTaskMemFree(pFormat);

		check_hresult(_audioClient->GetService(__uuidof(::IAudioCaptureClient), _captureClient.put_void()));

		InitWorkQueue();
		InitAnalyzer();
	}


	Windows::Foundation::IAsyncOperation<AudioVisualizer::LoopbackVisualizationSource> LoopbackVisualizationSource::CreateAsync()
    {
		using namespace winrt::Windows::Media::Devices;
		auto audioClient = co_await AudioInterfaceActivator::ActivateAudioInterfaceAsync(MediaDevice::GetDefaultAudioRenderId(AudioDeviceRole::Default).c_str());
		return *make_self<LoopbackVisualizationSource>(audioClient);
    }
    Windows::Foundation::IAsyncOperation<AudioVisualizer::LoopbackVisualizationSource> LoopbackVisualizationSource::CreateAsync(Windows::Devices::Enumeration::DeviceInformation renderDevice)
    {
		if (!renderDevice) {
			throw hresult_invalid_argument();
		}
		auto audioClient = co_await AudioInterfaceActivator::ActivateAudioInterfaceAsync(renderDevice.Id().c_str());
		return *make_self<LoopbackVisualizationSource>(audioClient);
	}

    AudioVisualizer::VisualizationDataFrame LoopbackVisualizationSource::GetData()
    {
		std::lock_guard<std::mutex> lock(_outputFrameLock);
		return _outputFrame;
    }
    bool LoopbackVisualizationSource::IsSuspended()
    {
		return _suspended;
    }
    void LoopbackVisualizationSource::IsSuspended(bool value)
    {
		if (value != _suspended) {
			if (value)
				Stop();
			else
				Start();
		}
    }

    float LoopbackVisualizationSource::Fps()
    {
		return 60.0f;
    }
    void LoopbackVisualizationSource::Fps(float /* value */)
    {
        throw hresult_not_implemented();
    }
    AudioVisualizer::AnalyzerType LoopbackVisualizationSource::AnalyzerTypes()
    {
		return _analyzer.AnalyzerTypes();
    }
    void LoopbackVisualizationSource::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
    {
		_analyzer.AnalyzerTypes(value);
    }
	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> LoopbackVisualizationSource::PresentationTime()
	{
		return _streamPosition;
	};

    
    AudioVisualizer::SourcePlaybackState LoopbackVisualizationSource::PlaybackState()
    {
		return _suspended ? SourcePlaybackState::Stopped : SourcePlaybackState::Playing;
    }
    Windows::Foundation::IReference<uint32_t> LoopbackVisualizationSource::ActualFrequencyCount()
    {
		return _analyzer.SpectrumElementCount();
    }
    Windows::Foundation::IReference<uint32_t> LoopbackVisualizationSource::ActualChannelCount()
    {
		return _inputChannels;
    }
    Windows::Foundation::IReference<float> LoopbackVisualizationSource::ActualMinFrequency()
    {
		return 0.0f;
    }
    Windows::Foundation::IReference<float> LoopbackVisualizationSource::ActualMaxFrequency()
    {
		return (float)_analyzer.SpectrumElementCount() * _analyzer.SpectrumStep();
    }
	Windows::Foundation::IReference<float> LoopbackVisualizationSource::ActualFrequencyStep()
	{
		if (_analyzer && ((unsigned)_analyzer.AnalyzerTypes() & (unsigned)AnalyzerType::Spectrum) != 0) {
			return _analyzer.SpectrumStep();
		}
		return nullptr;
	}
    Windows::Foundation::IReference<AudioVisualizer::ScaleType> LoopbackVisualizationSource::ActualFrequencyScale()
    {
		return ScaleType::Linear;
    }
    winrt::event_token LoopbackVisualizationSource::ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler)
    {
		return _configurationChangedEvent.add(handler);
    }
    void LoopbackVisualizationSource::ConfigurationChanged(winrt::event_token const& token) noexcept
    {
		_configurationChangedEvent.remove(token);
    }
	void LoopbackVisualizationSource::ConfigureSpectrum(uint32_t fftLength, float overlap)
	{
		InitAnalyzer(fftLength, overlap);
	}

	void LoopbackVisualizationSource::Start()
	{
		_suspended = false;
		check_hresult(_audioClient->Start());
		ScheduleWorkItem();
	}

	void LoopbackVisualizationSource::Stop()
	{
		_suspended = true;
		check_hresult(_audioClient->Stop());
		CancelWorkItem();
	}

	HRESULT __stdcall LoopbackVisualizationSource::GetParameters(DWORD* pdwFlags, DWORD* pdwQueue)
	{
		if (pdwFlags == nullptr || pdwQueue == nullptr) {
			return E_POINTER;
		}
		*pdwQueue = _workQueueId;
		return S_OK;
	}
	HRESULT __stdcall LoopbackVisualizationSource::Invoke(IMFAsyncResult* /*pAsyncResult*/)
	{
		float* pSamples = nullptr;
		UINT32 numFramesToRead = 0;
		DWORD dwFlags = 0;
		UINT64 devicePosition = 0;
		HRESULT hr = _captureClient->GetBuffer((BYTE**)&pSamples, &numFramesToRead, &dwFlags, &devicePosition, nullptr);
		winrt::get_self<AudioAnalyzer>(_analyzer)->ProcessInputRaw(pSamples, numFramesToRead,devicePosition);
		hr = _captureClient->ReleaseBuffer(numFramesToRead);
		_streamPosition = Windows::Foundation::TimeSpan() = { 10000000L * devicePosition / _inputSampleRate };
		ScheduleWorkItem();
		return S_OK;
	}
}
