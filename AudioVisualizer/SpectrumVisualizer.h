#pragma once

#include "SpectrumVisualizer.g.h"
#include "VisualizerControl.h"
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.h>

namespace winrt::AudioVisualizer::implementation
{
    struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer, AudioVisualizer::implementation::VisualizerControl>
    {
		Windows::UI::Color _unlitElement;
		std::vector<AudioVisualizer::MeterBarLevel> _levels;
		Windows::UI::Xaml::Controls::Orientation _orientation;
		uint32_t _channelIndex;
		Windows::UI::Xaml::Thickness _relativeElementMargin;
	public:
        SpectrumVisualizer();
		com_array<AudioVisualizer::MeterBarLevel> Levels();
		void Levels(array_view<AudioVisualizer::MeterBarLevel const> value);
		Windows::UI::Xaml::Controls::Orientation Orientation();
		void Orientation(Windows::UI::Xaml::Controls::Orientation const& value);
		uint32_t ChannelIndex();
		void ChannelIndex(uint32_t value);
		Windows::UI::Xaml::Thickness RelativeElementMargin();
		void RelativeElementMargin(Windows::UI::Xaml::Thickness const& value);
		Windows::UI::Color UnlitElement();
		void UnlitElement(Windows::UI::Color const& value);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct SpectrumVisualizer : SpectrumVisualizerT<SpectrumVisualizer, implementation::SpectrumVisualizer>
    {
    };
}
