#pragma once

#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer.abi.h"
#include "ScalarData.h"
#include "SpectrumData.h"
#include <DirectXMath.h>
#include <AudioAnalyzer.h>
#include "LifeSpanTracker.h"
#include "trace.h"

using namespace ABI::AudioVisualizer;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Diagnostics;
using namespace Microsoft::WRL::Wrappers;
using namespace wrl_util;

namespace AudioVisualizer
{
	class VisualizationDataFrame : public RuntimeClass<IVisualizationDataFrame,FtmBase>, public LifespanTracker<VisualizationDataFrame>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizationDataFrame, BaseTrust)
		TimeSpan _time;
		TimeSpan _duration;
		ComPtr<IScalarData> _rms;
		ComPtr<IScalarData> _peak;
		ComPtr<ISpectrumData> _spectrum;

	public:
		VisualizationDataFrame(REFERENCE_TIME time,REFERENCE_TIME duration,IScalarData *pRms,IScalarData *pPeak,ISpectrumData *pSpectrum);
		~VisualizationDataFrame();

		STDMETHODIMP get_Time(TimeSpan *pTimeStamp)
		{
			if (pTimeStamp == nullptr)
				return E_INVALIDARG;
			*pTimeStamp = _time;
			return S_OK;
		}
		STDMETHODIMP get_Duration(TimeSpan *pTimeStamp)
		{
			if (pTimeStamp == nullptr)
				return E_INVALIDARG;
			*pTimeStamp = _duration;
			return S_OK;
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
		STDMETHODIMP get_Spectrum(ISpectrumData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			return _spectrum.CopyTo(ppData);
		}

		bool IsBefore(REFERENCE_TIME time) { return time < _time.Duration; }
		bool IsAfter(REFERENCE_TIME time) { return time >= (_time.Duration + _duration.Duration) + 50; }
	};
}

