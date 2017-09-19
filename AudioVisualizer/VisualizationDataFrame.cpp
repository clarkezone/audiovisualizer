#include "pch.h"
#include "VisualizationDataFrame.h"
#include "VisualizationDataReference.h"

namespace AudioVisualizer
{
	VisualizationDataFrame::VisualizationDataFrame(AudioMath::AnalyzerFrame *pFrame, REFERENCE_TIME time, REFERENCE_TIME duration)
	{
		_bIsClosed = false;
		_time.Duration = time;
		_duration.Duration = duration;
		_frame = pFrame;
	}
	VisualizationDataFrame::~VisualizationDataFrame()
	{

	}

	STDMETHODIMP VisualizationDataFrame::GetReference(ABI::AudioVisualizer::IVisualizationDataReference ** ppResult)
	{
			if (ppResult == nullptr)
				return E_INVALIDARG;
			return S_OK;
	}
	STDMETHODIMP VisualizationDataFrame::Close()
	{
		if (_bIsClosed)
			return S_OK;
		_frame = nullptr;
		_bIsClosed = true;
		return S_OK;
	}
}
