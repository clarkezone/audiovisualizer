#include "pch.h"
#include "VisualizationDataFrame.h"
#include "VisualizationDataReference.h"

namespace AudioVisualizer
{
	VisualizationDataFrame::VisualizationDataFrame(IReference<TimeSpan>* pTime, IReference<TimeSpan>* pDuration, ScalarData * pRms, ScalarData * pPeak, VectorData * pSpectrum)
	{
		_time = pTime;
		_duration = pDuration;
		_rms = pRms;
		_peak = pPeak;
		_spectrum = pSpectrum;
	}
	VisualizationDataFrame::~VisualizationDataFrame()
	{

	}

	STDMETHODIMP VisualizationDataFrame::GetReference(ABI::AudioVisualizer::IVisualizationDataReference ** ppResult)
	{
			if (ppResult == nullptr)
				return E_INVALIDARG;

			ComPtr<IVisualizationDataReference> reference = Make<VisualizationDataReference>(this, _rms.Get(), _peak.Get(), _spectrum.Get());
			return reference.CopyTo(ppResult);
	}
}
