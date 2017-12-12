#include "pch.h"
#include "VisualizationDataFrame.h"

namespace AudioVisualizer
{
	VisualizationDataFrame::VisualizationDataFrame(REFERENCE_TIME time, REFERENCE_TIME duration, IScalarData *pRms, IScalarData *pPeak, ISpectrumData *pSpectrum)
	{
		_time.Duration = time;
		_duration.Duration = duration;
		_rms = pRms;
		_peak = pPeak;
		_spectrum = pSpectrum;
	}

	VisualizationDataFrame::~VisualizationDataFrame()
	{
		_rms = nullptr;
		_peak = nullptr;
		_spectrum = nullptr;
	}

}
