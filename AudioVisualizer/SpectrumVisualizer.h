#pragma once

#include "SpectrumVisualizer.g.h"
#include "VisualizerControl.h"
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.h>
#include "BarVisualizerBase.h"

namespace winrt::AudioVisualizer::implementation
{
    struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer>,BarVisualizerBase<SpectrumVisualizer>
    {
		//virtual void OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime);
	public:
        SpectrumVisualizer();
		virtual void OnSourceChanged(hstring const &propertyName);
		virtual void OnUpdateMeter(AudioVisualizer::VisualizationDataFrame const &frame);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer, implementation::SpectrumVisualizer>
    {
    };
}
