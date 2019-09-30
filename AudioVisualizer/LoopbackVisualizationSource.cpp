#include "pch.h"
#include "LoopbackVisualizationSource.h"
#include "AudioInterfaceActivator.h"
#include <pplawait.h>
#include <winrt/Windows.Media.Devices.h>

namespace winrt::AudioVisualizer::implementation
{
	LoopbackVisualizationSource::LoopbackVisualizationSource(winrt::com_ptr<IAudioClient3> const& audioClient)
	{
		_audioClient = audioClient;
		// Initialize with default period
		REFERENCE_TIME defaultPeriod{ 0 };
		check_hresult(_audioClient->GetDevicePeriod(&defaultPeriod, nullptr));
		WAVEFORMATEX* pFormat {nullptr};
		check_hresult(_audioClient->GetMixFormat(&pFormat));

		check_hresult(_audioClient->Initialize(
			AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED,
			AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_LOOPBACK, 0, defaultPeriod, pFormat, nullptr));

		CoTaskMemFree(pFormat);
		
		_audioClient->SetEventHandle(_callbackEvent.get());

		// Prepare work queue 

		check_hresult(MFLockSharedWorkQueue(L"", 0, &_workQueueTaskId, &_workQueueId));
		// Create async result that can be shared across wait operations as there is only one active at a time
		check_hresult(MFCreateAsyncResult(nullptr, (IMFAsyncCallback*)(this), nullptr, _asyncResult.put()));
	}


	Windows::Foundation::IAsyncOperation<AudioVisualizer::LoopbackVisualizationSource> LoopbackVisualizationSource::CreateAsync()
    {
		using namespace winrt::Windows::Media::Devices;
		auto audioClient = co_await AudioInterfaceActivator::ActivateAudioInterfaceAsync(MediaDevice::GetDefaultAudioRenderId(AudioDeviceRole::Default).c_str());
		return make_self<LoopbackVisualizationSource>(audioClient).as<AudioVisualizer::LoopbackVisualizationSource>();
    }
    Windows::Foundation::IAsyncOperation<AudioVisualizer::LoopbackVisualizationSource> LoopbackVisualizationSource::CreateAsync(Windows::Devices::Enumeration::DeviceInformation renderDevice)
    {
		if (!renderDevice) {
			throw hresult_invalid_argument();
		}
		auto audioClient = co_await AudioInterfaceActivator::ActivateAudioInterfaceAsync(renderDevice.Id().c_str());
		return make_self<LoopbackVisualizationSource>(audioClient).as<AudioVisualizer::LoopbackVisualizationSource>();
	}
    AudioVisualizer::VisualizationDataFrame LoopbackVisualizationSource::GetData()
    {
        throw hresult_not_implemented();
    }
    bool LoopbackVisualizationSource::IsSuspended()
    {
        throw hresult_not_implemented();
    }
    void LoopbackVisualizationSource::IsSuspended(bool value)
    {
        throw hresult_not_implemented();
    }
    float LoopbackVisualizationSource::Fps()
    {
        throw hresult_not_implemented();
    }
    void LoopbackVisualizationSource::Fps(float value)
    {
        throw hresult_not_implemented();
    }
    AudioVisualizer::AnalyzerType LoopbackVisualizationSource::AnalyzerTypes()
    {
        throw hresult_not_implemented();
    }
    void LoopbackVisualizationSource::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> LoopbackVisualizationSource::PresentationTime()
    {
        throw hresult_not_implemented();
    }
    AudioVisualizer::SourcePlaybackState LoopbackVisualizationSource::PlaybackState()
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::IReference<uint32_t> LoopbackVisualizationSource::ActualFrequencyCount()
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::IReference<uint32_t> LoopbackVisualizationSource::ActualChannelCount()
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::IReference<float> LoopbackVisualizationSource::ActualMinFrequency()
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::IReference<float> LoopbackVisualizationSource::ActualMaxFrequency()
    {
        throw hresult_not_implemented();
    }
    Windows::Foundation::IReference<AudioVisualizer::ScaleType> LoopbackVisualizationSource::ActualFrequencyScale()
    {
        throw hresult_not_implemented();
    }
    winrt::event_token LoopbackVisualizationSource::ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler)
    {
        throw hresult_not_implemented();
    }
    void LoopbackVisualizationSource::ConfigurationChanged(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
	void LoopbackVisualizationSource::ConfigureSpectrum(uint32_t fftLength, float overlap)
	{
	}
	void LoopbackVisualizationSource::Start()
	{
	}
	void LoopbackVisualizationSource::Stop()
	{
	}
	HRESULT __stdcall LoopbackVisualizationSource::GetParameters(DWORD* pdwFlags, DWORD* pdwQueue)
	{
		if (pdwFlags == nullptr || pdwQueue == nullptr) {
			return E_POINTER;
		}
		*pdwQueue = _workQueueId;
		return S_OK;
	}
	HRESULT __stdcall LoopbackVisualizationSource::Invoke(IMFAsyncResult* pAsyncResult)
	{
		return E_NOTIMPL;
	}
}
