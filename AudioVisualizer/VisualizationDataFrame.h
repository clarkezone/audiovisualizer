#pragma once

#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer.abi.h"
#include "ScalarData.h"
#include "VectorData.h"
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
	class VisualizationDataFrame : public RuntimeClass<IVisualizationDataFrame,ABI::Windows::Foundation::IClosable,FtmBase>, public LifespanTracker<VisualizationDataFrame>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizationDataFrame, BaseTrust)
		TimeSpan _time;
		TimeSpan _duration;
		ComPtr<AudioMath::AnalyzerFrame> _frame;
		bool _bIsClosed;

	public:
		VisualizationDataFrame(AudioMath::AnalyzerFrame *pFrame,REFERENCE_TIME time,REFERENCE_TIME duration);
		~VisualizationDataFrame();

		STDMETHODIMP get_Time(IReference<TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			ComPtr<Nullable<TimeSpan>> spTime = Make<Nullable<TimeSpan>>(_time);
			
			spTime.CopyTo(ppTimeStamp);
			return S_OK;
		}
		STDMETHODIMP get_Duration(IReference<TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			ComPtr<Nullable<TimeSpan>> spTime = Make<Nullable<TimeSpan>>(_duration);

			spTime.CopyTo(ppTimeStamp);
			return S_OK;
		}
		STDMETHODIMP GetReference(ABI::AudioVisualizer::IVisualizationDataReference **ppResult);

		STDMETHODIMP Close();
	};
}

