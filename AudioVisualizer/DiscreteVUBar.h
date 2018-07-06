#pragma once

#include "DiscreteVUBar.g.h"
#include "BarVisualizerBase.h"

namespace winrt::AudioVisualizer::implementation
{
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar>,BarVisualizerBase<DiscreteVUBar>
    {
		bool _displayPeak = true;
		//virtual void OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime);
	public:
		DiscreteVUBar();
		bool DisplayPeak() { return _displayPeak; }
		void DisplayPeak(const bool value) { _displayPeak = value; }
		virtual void OnUpdateMeter(VisualizationDataFrame const &frame);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar, implementation::DiscreteVUBar>
    {
    };
}
