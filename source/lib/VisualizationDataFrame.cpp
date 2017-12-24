#include "pch.h"
#include "VisualizationDataFrame.h"

namespace AudioVisualizer
{
	class VisualizationDataFrameFactory : public AgileActivationFactory<IVisualizationDataFrameFactory>
	{
	public:
		STDMETHODIMP Create(TimeSpan time, TimeSpan duration, IScalarData *pRms, IScalarData *pPeak, ISpectrumData *pSpectrum,IVisualizationDataFrame **ppResult)
		{
			if (ppResult == nullptr)
				return E_POINTER;
			ComPtr<VisualizationDataFrame> frame = Make<VisualizationDataFrame>(time.Duration, duration.Duration, pRms, pPeak, pSpectrum);
			return frame.CopyTo(ppResult);
		}
	};

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

	ActivatableClassWithFactory(VisualizationDataFrame, VisualizationDataFrameFactory);
}
