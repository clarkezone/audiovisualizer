#pragma once

#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer.abi.h"
#include "ScalarData.h"
#include "ArrayData.h"
#include "Nullable.h"
#include <DirectXMath.h>
#include <AudioAnalyzer.h>
#include "LifeSpanTracker.h"
#include "trace.h"

using namespace ABI::AudioVisualizer;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Diagnostics;
using namespace Microsoft::WRL::Wrappers;

namespace AudioVisualizer
{
	/*
	struct IAnalyzerFrame : public IUnknown
	{
		virtual bool IsBefore(REFERENCE_TIME time)=0;	// true if time < frame->time
		virtual bool IsAfter(REFERENCE_TIME time) = 0;	// true is time >= frame->time + 50us
	};*/

	class VisualizationDataFrame : public RuntimeClass<IVisualizationDataFrame,FtmBase>, public LifespanTracker<VisualizationDataFrame>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizationDataFrame, BaseTrust)
		TimeSpan _time;
		TimeSpan _duration;
		ComPtr<IScalarData> _rms;
		ComPtr<IScalarData> _peak;
		ComPtr<IArrayData> _spectrum;

	public:
		VisualizationDataFrame(REFERENCE_TIME time,REFERENCE_TIME duration,IScalarData *pRms,IScalarData *pPeak,IArrayData *pSpectrum);
		~VisualizationDataFrame();

		STDMETHODIMP get_Time(IReference<TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			ComPtr<Nullable<TimeSpan>> spTime = Make<Nullable<TimeSpan>>(_time);		
			return spTime.CopyTo(ppTimeStamp);
		}
		STDMETHODIMP get_Duration(IReference<TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			ComPtr<Nullable<TimeSpan>> spTime = Make<Nullable<TimeSpan>>(_duration);
			return spTime.CopyTo(ppTimeStamp);
		}
		STDMETHODIMP get_RMS(IScalarData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			return _rms.CopyTo(ppData);
		}
		STDMETHODIMP get_Peak(IScalarData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			return _peak.CopyTo(ppData);
		}
		STDMETHODIMP get_Spectrum(IArrayData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			return _spectrum.CopyTo(ppData);
		}

		bool IsBefore(REFERENCE_TIME time) { return time < _time.Duration; }
		bool IsAfter(REFERENCE_TIME time) { return time >= (_time.Duration + _duration.Duration) + 50; }
	};
}

