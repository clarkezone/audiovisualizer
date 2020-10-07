#pragma once

#include "DiscreteVUBar.g.h"
#include "BarVisualizerBase.h"

namespace winrt::AudioVisualizer::implementation
{
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar>,BarVisualizerBase<DiscreteVUBar,vu_bar_visualizer>
    {
		bool _displayPeak = true;
	public:
		DiscreteVUBar();
		bool DisplayPeak() { return _displayPeak; }
		void DisplayPeak(const bool value) { _displayPeak = value; }
		void OnUpdateMeter(VisualizationDataFrame const &frame);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar, implementation::DiscreteVUBar>
    {
    };
}
