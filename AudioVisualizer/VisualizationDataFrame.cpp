#include "pch.h"
#include "VisualizationDataFrame.h"
#include "VisualizationData.h"

namespace AudioVisualizer
{
	VisualizationDataFrame::VisualizationDataFrame(IReference<TimeSpan>* pTime, IReference<TimeSpan>* pDuration, size_t channels, size_t spectralLength)
	{
		_time = pTime;
		_duration = pDuration;
		
		_rms = Make<ScalarData>(channels);	
		_peak = Make<ScalarData>(channels);
		_specter = Make<VectorData>(channels, spectralLength);
	}
}
