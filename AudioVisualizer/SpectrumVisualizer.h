#pragma once

#include "SpectrumVisualizer.g.h"
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.h>
#include "BarVisualizerBase.h"

namespace winrt::AudioVisualizer::implementation
{
	struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer>, BarVisualizerBase<SpectrumVisualizer,spectrum_bar_visualizer>
	{
	public:
		SpectrumVisualizer();
		void OnUpdateMeter(AudioVisualizer::VisualizationDataFrame const& frame);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer, implementation::SpectrumVisualizer>
    {
    };
}
