#pragma once

#include "SpectrumVisualizer.g.h"
#include "VisualizerControl.h"
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.h>
#include "BarBaseVisualizer.h"

namespace winrt::AudioVisualizer::implementation
{
    struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer, AudioVisualizer::implementation::VisualizerControl>, BarBaseVisualizer
    {
		virtual void OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime);
	public:
        SpectrumVisualizer();
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer, implementation::SpectrumVisualizer>
    {
    };
}
