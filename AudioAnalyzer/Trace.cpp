#include "pch.h"
#include "trace.h"
#include <windows.foundation.diagnostics.h>
#include <windows.media.mediaproperties.h>

#include <wrl.h>

using namespace ABI::Windows::Foundation::Diagnostics;
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

#define LOG_CHANNEL_NAME L"AudioVisualization-AudioAnalyzer"

#define EVT_SET_MEDIA_TYPE L"SetMediaType"

namespace AudioAnalyzer
{
	namespace Diagnostics
	{
	
		static ILoggingTarget *g_pLoggingChannel = nullptr;

		HRESULT CreateLoggingFields(ILoggingFields **ppFields)
		{
			HRESULT hr = S_OK;
			HStringReference runtimeClassLoggingFields(RuntimeClass_Windows_Foundation_Diagnostics_LoggingFields);
			ComPtr<IActivationFactory> spFactory;
			hr = ABI::Windows::Foundation::GetActivationFactory(runtimeClassLoggingFields.Get(), &spFactory);
			if (FAILED(hr))
				return hr;
			ComPtr<IInspectable> object;
			spFactory->ActivateInstance(&object);

			hr = object->QueryInterface(ppFields);
			return hr;
		}

		HRESULT Trace::Initialize()
		{
			HRESULT hr = S_OK;
			if (g_pLoggingChannel == nullptr)
			{
				ComPtr<ILoggingChannelFactory2> spLogChannelFactory;
				hr = ABI::Windows::Foundation::GetActivationFactory(
					HStringReference(RuntimeClass_Windows_Foundation_Diagnostics_LoggingChannel).Get(),
					&spLogChannelFactory
				);
				if (FAILED(hr))
					return hr;
				ComPtr<ILoggingChannel> spLogChannel;
				hr = spLogChannelFactory->CreateWithOptions(
					HStringReference(LOG_CHANNEL_NAME).Get(),
					nullptr,
					&spLogChannel
				);
				if (FAILED(hr))
					return hr;

				hr = spLogChannel->QueryInterface(&g_pLoggingChannel);
			}
			else
			{
				g_pLoggingChannel->AddRef();
			}
			return hr;
		}

		HRESULT Trace::Shutdown()
		{
			if (g_pLoggingChannel != nullptr)
			{
				if (g_pLoggingChannel->Release() == 0)
					g_pLoggingChannel = nullptr;
			}
			return S_OK;
		}

		HRESULT Trace::Log_SetMediaType(IMFMediaType * pMediaType,HRESULT result)
		{
			using namespace ABI::Windows::Media::MediaProperties;

			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			
			if (pMediaType != nullptr)
			{
				Microsoft::WRL::ComPtr<IMediaEncodingProperties> spMediaProps;

				HRESULT hr = MFCreatePropertiesFromMediaType(pMediaType, IID_PPV_ARGS(&spMediaProps));
				if (FAILED(hr))
					return hr;

				HSTRING hsMediaType = 0;
				spMediaProps->get_Type(&hsMediaType);
				HSTRING hsMediaSubType = 0;
				spMediaProps->get_Subtype(&hsMediaSubType);
				spFields->AddString(HStringReference(L"Type").Get(), hsMediaType);
				spFields->AddString(HStringReference(L"SubType").Get(), hsMediaSubType);
				UINT32 samplesPerSecond = 0;
				hr = pMediaType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &samplesPerSecond);
				if (SUCCEEDED(hr))
					spFields->AddUInt32(HStringReference(L"SampleRate").Get(), samplesPerSecond);
				UINT32 numChannels;
				hr = pMediaType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &numChannels);
				if (SUCCEEDED(hr))
					spFields->AddUInt32(HStringReference(L"Channels").Get(), numChannels);
			}
			else
			{
				spFields->AddEmpty(HStringReference(L"Type").Get());
			}
			spFields->AddInt32WithFormat(HStringReference(L"Result").Get(), result, LoggingFieldFormat::LoggingFieldFormat_HResult);

			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_SET_MEDIA_TYPE).Get(),spFields.Get());

			return hr;
		}
	}
}