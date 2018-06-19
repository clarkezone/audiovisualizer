#pragma once

#include "DiscreteVUBar.g.h"
#include "VisualizerControl.h"
#include "BarBaseVisualizer.h"

namespace winrt::AudioVisualizer::implementation
{
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar, AudioVisualizer::implementation::VisualizerControl>,BarBaseVisualizer
    {
		bool _displayPeak = true;
		virtual void OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime);
	public:
		DiscreteVUBar();
		bool DisplayPeak() { return _displayPeak; }
		void DisplayPeak(const bool value) { _displayPeak = value; }
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar, implementation::DiscreteVUBar>
    {
    };
}
