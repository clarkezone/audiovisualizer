#pragma once
#include <wrl.h>
#include <mfapi.h>

namespace AnalyzerTest
{
	// Fake presentation clock implementation for tests
	class CFakeClock : public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::ClassicCom>,
		IMFPresentationClock>
	{
		MFTIME m_Time;
	public:
		STDMETHODIMP GetClockCharacteristics(
			/* [out] */ __RPC__out DWORD *pdwCharacteristics)
		{
			return  E_NOTIMPL;
		}

		STDMETHODIMP GetCorrelatedTime(
			/* [in] */ DWORD dwReserved,
			/* [out] */ __RPC__out LONGLONG *pllClockTime,
			/* [out] */ __RPC__out MFTIME *phnsSystemTime)
		{
			return  E_NOTIMPL;
		}

		STDMETHODIMP GetContinuityKey(
			/* [out] */ __RPC__out DWORD *pdwContinuityKey) {
			return  E_NOTIMPL;
		}

		STDMETHODIMP GetState(
			/* [in] */ DWORD dwReserved,
			/* [out] */ __RPC__out MFCLOCK_STATE *peClockState)
		{
			return  E_NOTIMPL;
		}

		STDMETHODIMP GetProperties(
			/* [out] */ __RPC__out MFCLOCK_PROPERTIES *pClockProperties)
		{
			return  E_NOTIMPL;
		}

		STDMETHODIMP SetTimeSource(
			/* [in] */ __RPC__in_opt IMFPresentationTimeSource *pTimeSource)
		{
			return E_NOTIMPL;
		};

		STDMETHODIMP GetTimeSource(
			/* [out] */ __RPC__deref_out_opt IMFPresentationTimeSource **ppTimeSource)
		{
			return E_NOTIMPL;
		};

		STDMETHODIMP GetTime(
			/* [out] */ __RPC__out MFTIME *phnsClockTime)
		{
			*phnsClockTime = m_Time;
			return S_OK;
		};

		STDMETHODIMP AddClockStateSink(
			/* [in] */ __RPC__in_opt IMFClockStateSink *pStateSink)
		{
			return E_NOTIMPL;
		};

		STDMETHODIMP RemoveClockStateSink(
			/* [in] */ __RPC__in_opt IMFClockStateSink *pStateSink)
		{
			return E_NOTIMPL;
		};

		STDMETHODIMP Start(
			/* [in] */ LONGLONG llClockStartOffset)
		{
			return E_NOTIMPL;
		};

		STDMETHODIMP Stop(void)
		{
			return E_NOTIMPL;
		};

		STDMETHODIMP Pause(void)
		{
			return E_NOTIMPL;
		};

		void SetTime(MFTIME time)
		{
			m_Time = time;
		}
		CFakeClock()
		{
			m_Time = 0;
		}
	};
}