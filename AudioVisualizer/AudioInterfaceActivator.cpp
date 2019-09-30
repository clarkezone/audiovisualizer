#include "pch.h"
#include "AudioInterfaceActivator.h"

using namespace winrt;

HRESULT __stdcall AudioVisualizer::AudioInterfaceActivator::ActivateCompleted(IActivateAudioInterfaceAsyncOperation* pActivateOperation)
{
	HRESULT hrActivate = E_FAIL;
	com_ptr<IUnknown> activatedInterface;
	HRESULT hr = pActivateOperation->GetActivateResult(&hrActivate, activatedInterface.put());

	if (FAILED(hrActivate)) {
		activationCompleted.set_exception(winrt::hresult_error(hrActivate));
	}
	else if (FAILED(hr)) {
		activationCompleted.set_exception(winrt::hresult_error(hr));
	}
	else {
		activationCompleted.set(activatedInterface.as<IAudioClient3>());
	}
	return S_OK;;
}

concurrency::task<winrt::com_ptr<::IAudioClient3>> AudioVisualizer::AudioInterfaceActivator::ActivateAudioInterfaceAsync(LPCWCHAR szDeviceId)
{
	auto activator = winrt::make_self<AudioInterfaceActivator>();
	winrt::com_ptr<IActivateAudioInterfaceAsyncOperation> op;
	check_hresult(::ActivateAudioInterfaceAsync(
		szDeviceId,
		__uuidof(IAudioClient3),
		nullptr,
		activator.as<IActivateAudioInterfaceCompletionHandler>().get(),
		op.put()));

	return concurrency::create_task(activator->activationCompleted);
}
