#pragma once
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <ppltasks.h>

namespace winrt::AudioVisualizer
{
	// Class wrapping async audio interface activateion and returning a waitable task
	struct AudioInterfaceActivator : winrt::implements<AudioInterfaceActivator, ::IActivateAudioInterfaceCompletionHandler>
	{
		concurrency::task_completion_event<winrt::com_ptr<::IAudioClient3>> activationCompleted;
		HRESULT STDMETHODCALLTYPE ActivateCompleted(::IActivateAudioInterfaceAsyncOperation* activateOperation);
		static concurrency::task<winrt::com_ptr<::IAudioClient3>> ActivateAudioInterfaceAsync(LPCWCHAR szDeviceId);
	};
}
