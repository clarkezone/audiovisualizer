#pragma once

#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer_h.h"
#include "ScalarData.h"
#include "VectorData.h"
#include "Nullable.h"
#include <DirectXMath.h>
#include "LifeSpanTracker.h"
#include "trace.h"

using namespace ABI::AudioVisualizer;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Diagnostics;

namespace AudioVisualizer
{
	class VisualizationDataFrame : public RuntimeClass<IVisualizationDataFrame>, public LifespanTracker<VisualizationDataFrame>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizationDataFrame, BaseTrust)
		ComPtr<IReference<TimeSpan>> _time;
		ComPtr<IReference<TimeSpan>> _duration;
		ComPtr<ScalarData> _rms;
		ComPtr<ScalarData> _peak;
		ComPtr<VectorData> _spectrum;
	public:
		VisualizationDataFrame(IReference<TimeSpan> *pTime, IReference<TimeSpan> *pDuration, ScalarData *pRms, ScalarData *pPeak, VectorData *pSpectrum);
		~VisualizationDataFrame();

		STDMETHODIMP get_Time(ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			_time.CopyTo(ppTimeStamp);
			return S_OK;
		}
		STDMETHODIMP get_Duration(ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			_duration.CopyTo(ppTimeStamp);
			return S_OK;
		}
		STDMETHODIMP GetReference(ABI::AudioVisualizer::IVisualizationDataReference **ppResult);

	};
}

