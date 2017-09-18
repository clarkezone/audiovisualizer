#pragma once
#include <mfapi.h>
#include <windows.foundation.diagnostics.h>
#include "AudioVisualizer_h.h"
#include <Unknwn.h>


using namespace ABI::Windows::Foundation::Diagnostics;
using namespace ABI::AudioVisualizer;

namespace AudioVisualizer
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
		private:
			static HRESULT _StartLockOperation(wchar_t *szObjName, ILoggingActivity **ppActivity);
			static HRESULT _LockAquired(wchar_t *szObjName, ILoggingActivity *ppActivity);
			static HRESULT _StartOperation(wchar_t *szObjName, ILoggingActivity **ppActivity);

		public:
			static HRESULT Initialize();
			static HRESULT Shutdown();
			static HRESULT Log_SetMediaType(IMFMediaType *pType,HRESULT result);
			static HRESULT Log_Initialize(size_t fftLength, size_t stepLen, size_t overlap);
			static HRESULT Log_SetInputPosition(long frameIndex);
			static HRESULT Log_ProcessSample(IMFSample *pSample);
			static HRESULT Log_StartCalculate(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity, long position, size_t bufferLength);
			static HRESULT Log_GetData(REFERENCE_TIME currentPosition, IVisualizationDataFrame *pFrame, IVisualizationDataFrame	*pQueueFront, size_t queueSize, HRESULT result);
			static HRESULT Log_OutputQueuePush(IVisualizationDataFrame *pFrame,size_t queueSize);
			static HRESULT Log_OutputQueuePop(IVisualizationDataFrame *pFrame, size_t queueSize, int reason);

			static HRESULT Log_GetFromBuffer(REFERENCE_TIME position);
			static HRESULT Log_StartCalculateFft(ILoggingActivity **ppActivity);
			static HRESULT Log_LineNumber(UINT32 lineNumber);
			static HRESULT Log_StartDraw(IVisualizationDataFrame *pFrame, ILoggingActivity **ppActivity);
			static HRESULT Log_ctor(const wchar_t *pTypeName,const void *ptr,size_t objCount,ILoggingActivity **ppActivity);
			static HRESULT Log_dtor(const wchar_t *pTypeName,const void *ptr,size_t objCount,ILoggingActivity *pActivity);
			static HRESULT Log_CloseObject(const wchar_t *pTypeName,const void *ptr,size_t objCount, ILoggingActivity *pActivity);
			static HRESULT Log_RefCount_Impl(const wchar_t *pContext, IUnknown *pObject,ILoggingActivity *pActivity);
			static HRESULT Log_Flush();

			template<class T> static void Trace_Log_RefCount1(const wchar_t *szContext, T *pObject, ILoggingActivity *pActivity)
			{
				IUnknown *pUnk = reinterpret_cast<IUnknown *>(pObject);
				Diagnostics::Trace::Log_RefCount_Impl(szContext, pUnk, pActivity);
			}
			template<class _Func> static void Trace_Lock(Microsoft::WRL::Wrappers::CriticalSection *cs, wchar_t *szName,_Func f)
			{
				ComPtr<ILoggingActivity> _activity;
				_StartLockOperation(szName, &_activity);
				CLogActivityHelper helper(_activity.Get());
				auto lock = cs->Lock();
				_LockAquired(szName, _activity.Get());
				f();
			}
			template<class _Func> static void Trace_Operation(wchar_t *szEventName, _Func f)
			{
				ComPtr<ILoggingActivity> _activity;
				CLogActivityHelper helper(_activity.Get());
				_StartOperation(szName, &_activity);
				f();
			}
		};
	}
#ifdef _TRACE
#define TRACK_CS_LOCK(name,lock,body) { AudioVisualization::Diagnostics::Trace::Trace_Lock(lock,name,[=] { body } ); }
#else
#define TRACK_CS_LOCK(name,lock,body) { auto __lock__ = lock.Lock(); body }
#endif

}