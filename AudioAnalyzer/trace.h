#pragma once
#include <mfapi.h>
#include <windows.foundation.diagnostics.h>

namespace AudioAnalyzer
{
	namespace Diagnostics
	{
		// Helper class to log stop event automatically when object goes out of scope
		class CLogActivityHelper
		{
			Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Diagnostics::ILoggingActivity> m_spActivity;
		public:
			CLogActivityHelper(ABI::Windows::Foundation::Diagnostics::ILoggingActivity *pActivity)
			{
				m_spActivity = pActivity;
			}
			// If destructed when going out of scope log stop event with same name
			~CLogActivityHelper()
			{
				using namespace ABI::Windows::Foundation::Diagnostics;
				using namespace Microsoft::WRL;
				ComPtr<ILoggingActivity2> m_spActivity2;
				m_spActivity.As(&m_spActivity2);
				HSTRING hEventName = nullptr;
				m_spActivity->get_Name(&hEventName);
				m_spActivity2->StopActivity(hEventName);
			}
		};

		class Trace
		{
		public:
			static HRESULT Initialize();
			static HRESULT Shutdown();
			static HRESULT Log_SetMediaType(IMFMediaType *pType,HRESULT result);
			static HRESULT Log_Initialize(size_t fftLength, size_t stepLen, size_t overlap);
			static HRESULT Log_SetInputPosition(long frameIndex);
			static HRESULT Log_ProcessSample(IMFSample *pSample);
			static HRESULT Log_StartCalculate(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity, long position, size_t bufferLength);
			static HRESULT Log_GetData(REFERENCE_TIME currentPosition, IMFSample *pSample, IMFSample *pQueueFront, size_t queueSize, HRESULT result);
			static HRESULT Log_OutputQueuePush(size_t queueSize);
			static HRESULT Log_GetFromBuffer(REFERENCE_TIME position);
			static HRESULT Log_StartCalculateFft(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity);
			static HRESULT Log_LineNumber(UINT32 lineNumber);
			
			static HRESULT Log_Flush();
		};
	}
}