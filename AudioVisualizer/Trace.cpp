#include "pch.h"
#include "trace.h"
#include <windows.foundation.diagnostics.h>
#include <windows.media.mediaproperties.h>
#include "VisualizationDataFrame.h"

#include <wrl.h>

using namespace ABI::Windows::Foundation::Diagnostics;
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

#define LOG_CHANNEL_NAME L"AudioVisualizer-MftAnalyzer"

#define EVT_SET_MEDIA_TYPE L"SetMediaType"
#define EVT_INITIALIZE L"Initialize"
#define EVT_SET_INPUT_POSITION L"SetInputPosition"
#define EVT_PROCESS_SAMPLE L"ProcessSample"
#define EVT_START_CALCULATE L"Calculate"
#define EVT_FLUSH L"Flush"
#define EVT_GET_DATA L"GetData"
#define EVT_OUTPUT_PUSH L"OutputPush"
#define EVT_OUTPUT_POP L"OutputPop"
#define EVT_GET_FROM_BUFFER L"GetFromBuffer"
#define EVT_START_CALCULATE_FFT L"Fft"
#define EVT_START_DRAW L"InvokeDraw"
#define EVT_LIFESPAN L"Lifespan"
#define EVT_CLOSE_OBJECT L"Close"
#define EVT_LOCK L"Lock"
#define EVT_LOCK_AQUIRED L"Aquired"
#define EVT_REF_COUNT L"RefCount"

namespace AudioVisualizer
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
			hr = object.CopyTo(ppFields);
			return hr;
		}

		HRESULT AddVisualizationFrameProperties(IVisualizationDataFrame *pFrame, ILoggingFields *pFields,const wchar_t *szTime,const wchar_t *szDuration)
		{
			auto timeTitle = HStringReference(szTime);
			auto durationTitle = HStringReference(szDuration);
			if (pFrame != nullptr)
			{
				ComPtr<IReference<TimeSpan>> time = 0;
				pFrame->get_Time(&time);
				if (time != nullptr)
				{
					TimeSpan value;
					time->get_Value(&value);
					pFields->AddTimeSpan(timeTitle.Get(), value);
				}
				else
					pFields->AddEmpty(timeTitle.Get());

				pFrame->get_Duration(&time);
				if (time != nullptr)
				{
					TimeSpan value;
					time->get_Value(&value);
					pFields->AddTimeSpan(durationTitle.Get(), value);
				}
				else
					pFields->AddEmpty(durationTitle.Get());
			}
			else
			{
				pFields->AddEmpty(timeTitle.Get());
				pFields->AddEmpty(durationTitle.Get());
			}
			return S_OK;
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
		HRESULT Trace::Log_Initialize(size_t fftLength, size_t stepLen, size_t overlap)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			spFields->AddUInt32(HStringReference(L"FftLength").Get(), (UINT32) fftLength);
			spFields->AddUInt32(HStringReference(L"Step").Get(), (UINT32)stepLen);
			spFields->AddUInt32(HStringReference(L"Overlap").Get(), (UINT32)overlap);
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_INITIALIZE).Get(), spFields.Get());
			return hr;
		}
		HRESULT Trace::Log_SetInputPosition(long frameIndex)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			spFields->AddInt64(HStringReference(L"FrameIndex").Get(), frameIndex);
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_SET_INPUT_POSITION).Get(), spFields.Get());
			return hr;
		}
		HRESULT Trace::Log_ProcessSample(IMFSample * pSample)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			if (pSample != nullptr)
			{
				REFERENCE_TIME time = 0, duration = 0;
				pSample->GetSampleTime(&time);
				pSample->GetSampleDuration(&duration);
				spFields->AddTimeSpan(HStringReference(L"Time").Get(), ABI::Windows::Foundation::TimeSpan() = {time});
				spFields->AddTimeSpan(HStringReference(L"Duration").Get(), ABI::Windows::Foundation::TimeSpan() = { duration });
			}
			else
			{
				spFields->AddEmpty(HStringReference(L"Time").Get());
				spFields->AddEmpty(HStringReference(L"Duration").Get());
			}
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_PROCESS_SAMPLE).Get(), spFields.Get());
			return hr;
		}
		HRESULT Trace::Log_StartCalculate(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity, long position, size_t bufferLength)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			spFields->AddInt64(HStringReference(L"Position").Get(), position);
			spFields->AddUInt32(HStringReference(L"BufferLength").Get(), (UINT32) bufferLength);
			return g_pLoggingChannel->StartActivityWithFields(HStringReference(EVT_START_CALCULATE).Get(), spFields.Get(), ppActivity);
		}

		HRESULT Trace::Log_GetData(REFERENCE_TIME currentPosition, IVisualizationDataFrame *pFrame, IVisualizationDataFrame	*pQueueFront, size_t queueSize, HRESULT result)
		{
			using namespace Windows::Foundation;
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			spFields->AddTimeSpan(HStringReference(L"Position").Get(), ABI::Windows::Foundation::TimeSpan() = { currentPosition });

			AddVisualizationFrameProperties(pFrame, spFields.Get(),L"Time",L"Duration");
			AddVisualizationFrameProperties(pFrame, spFields.Get(), L"QFTime", L"QFDuration");

			spFields->AddUInt32(HStringReference(L"QueueSize").Get(), (UINT32)queueSize);
			spFields->AddInt32WithFormat(HStringReference(L"Result").Get(), result, LoggingFieldFormat::LoggingFieldFormat_HResult);
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_GET_DATA).Get(), spFields.Get());
			return hr;
		}
		HRESULT Trace::Log_OutputQueuePush(IVisualizationDataFrame *pFrame,size_t queueSize)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;

			AddVisualizationFrameProperties(pFrame, spFields.Get(), L"Time", L"Duration");
			spFields->AddUInt32(HStringReference(L"QueueSize").Get(), (UINT32) queueSize);
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_OUTPUT_PUSH).Get(), spFields.Get());
			return hr;
		}
		HRESULT Trace::Log_OutputQueuePop(IVisualizationDataFrame * pFrame, size_t queueSize, int reason)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;

			AddVisualizationFrameProperties(pFrame, spFields.Get(), L"Time", L"Duration");
			spFields->AddUInt32(HStringReference(L"QueueSize").Get(), (UINT32)queueSize);
			wchar_t *szReason = L"Unknown";
			switch (reason)
			{
			case 0:
				szReason = L"GetFrame";
				break;
			case 1:
				szReason = L"Compacting";
				break;
			case 2:
				szReason = L"Reset";
				break;
			}
			spFields->AddString(HStringReference(L"Reason").Get(), HStringReference(szReason).Get());
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_OUTPUT_POP).Get(), spFields.Get());
			return hr;
		}
		HRESULT Trace::Log_GetFromBuffer(REFERENCE_TIME position)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			spFields->AddTimeSpan(HStringReference(L"Time").Get(), ABI::Windows::Foundation::TimeSpan() = { position });
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_GET_FROM_BUFFER).Get(), spFields.Get());
			return hr;
		}

		HRESULT Trace::Log_StartCalculateFft(ABI::Windows::Foundation::Diagnostics::ILoggingActivity ** ppActivity)
		{
			return g_pLoggingChannel->StartActivity(HStringReference(EVT_START_CALCULATE_FFT).Get(),ppActivity);
		}

		HRESULT Trace::Log_LineNumber(UINT32 lineNumber)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			spFields->AddUInt32(HStringReference(L"Number").Get(), lineNumber);
			hr = g_pLoggingChannel->LogEventWithFields(HStringReference(L"Line").Get(), spFields.Get());
			return hr;
		}

		HRESULT Trace::Log_StartDraw(IVisualizationDataFrame * pFrame, ILoggingActivity ** ppActivity)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			if (pFrame != nullptr)
			{
				ComPtr<IReference<TimeSpan>> time = 0;
				pFrame->get_Time(&time);
				if (time != nullptr)
				{
					TimeSpan value;
					time->get_Value(&value);
					spFields->AddTimeSpan(HStringReference(L"Time").Get(), value);
				}
				else
					spFields->AddEmpty(HStringReference(L"Time").Get());
			}
			else
				spFields->AddEmpty(HStringReference(L"Time").Get());
			return g_pLoggingChannel->StartActivityWithFields(HStringReference(EVT_START_DRAW).Get(), spFields.Get(), ppActivity);
		}

		HRESULT Trace::Log_ctor(const wchar_t * pTypeName, const void *ptr, size_t objectCount,ILoggingActivity ** ppActivity)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			spFields->AddString(HStringReference(L"Type").Get(), HStringReference(pTypeName).Get());
			spFields->AddUInt32(HStringReference(L"Count").Get(), objectCount);
			return g_pLoggingChannel->StartActivityWithFields(HStringReference(EVT_LIFESPAN).Get(), spFields.Get(), ppActivity);
		}

		HRESULT Trace::Log_dtor(const wchar_t * pTypeName, const void *ptr, size_t objectCount, ILoggingActivity *pActivity)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			spFields->AddString(HStringReference(L"Type").Get(), HStringReference(pTypeName).Get());
			spFields->AddUInt32(HStringReference(L"Count").Get(), objectCount);

			ComPtr<ILoggingActivity2> activity2;
			ComPtr<ILoggingActivity>(pActivity).As(&activity2);
			return activity2->StopActivityWithFields(HStringReference(EVT_LIFESPAN).Get(),spFields.Get());
		}

		HRESULT Trace::Log_CloseObject(const wchar_t * pTypeName, const void *ptr,size_t objCount, ILoggingActivity * pActivity)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			spFields->AddString(HStringReference(L"Type").Get(), HStringReference(pTypeName).Get());
			spFields->AddUInt32(HStringReference(L"Count").Get(), objCount);
			ComPtr<ILoggingTarget> logTarget;
			ComPtr<ILoggingActivity>(pActivity).As(&logTarget);
			return logTarget->LogEventWithFields(HStringReference(EVT_CLOSE_OBJECT).Get(), spFields.Get());
		}

		HRESULT Trace::Log_RefCount_Impl(const wchar_t * pContext, IUnknown *pObject, ILoggingActivity * pActivity)
		{
		
			ComPtr<ILoggingFields> spFields;
			HRESULT hr = CreateLoggingFields(&spFields);
			spFields->AddString(HStringReference(L"Context").Get(), HStringReference(pContext).Get());

			pObject->AddRef();
			size_t refCount = pObject->Release();

			spFields->AddUInt32(HStringReference(L"RefCount").Get(), refCount);

			if (pActivity != nullptr)
			{
				ComPtr<ILoggingTarget> logTarget;
				ComPtr<ILoggingActivity>(pActivity).As(&logTarget);
				return logTarget->LogEventWithFields(HStringReference(EVT_REF_COUNT).Get(), spFields.Get());
			}
			else
				return g_pLoggingChannel->LogEventWithFields(HStringReference(EVT_REF_COUNT).Get(), spFields.Get());
		}

		HRESULT Trace::_StartLockOperation(wchar_t * szObjName, ILoggingActivity ** ppActivity)
		{
			HRESULT hr = S_OK;
			ComPtr<ILoggingFields> spFields;
			hr = CreateLoggingFields(&spFields);
			if (FAILED(hr))
				return hr;
			spFields->AddString(HStringReference(L"Name").Get(), HStringReference(szObjName).Get());

			return g_pLoggingChannel->StartActivityWithFields(HStringReference(EVT_LOCK).Get(), spFields.Get(),ppActivity);
		}

		HRESULT Trace::_LockAquired(wchar_t * szObjName, ILoggingActivity *pActivity)
		{
			ComPtr<ILoggingFields> spFields;
			HRESULT hr = CreateLoggingFields(&spFields);
			spFields->AddString(HStringReference(L"Name").Get(), HStringReference(szObjName).Get());
			ComPtr<ILoggingTarget> logTarget;
			ComPtr<ILoggingActivity>(pActivity).As(&logTarget);
			return logTarget->LogEventWithFields(HStringReference(EVT_LOCK_AQUIRED).Get(), spFields.Get());
		}

		HRESULT Trace::_StartOperation(wchar_t * szName, ILoggingActivity **ppActivity)
		{
			return g_pLoggingChannel->StartActivity(HStringReference(szName).Get(),ppActivity);
		}

		HRESULT Trace::Log_Flush()
		{
			return g_pLoggingChannel->LogEvent(HStringReference(EVT_FLUSH).Get());
		}
	}
}