#pragma once

#include "DiscreteVUBar.g.h"
#include "VisualizerControl.h"

namespace winrt::AudioVisualizer::implementation
{
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar, AudioVisualizer::implementation::VisualizerControl>
    {
		virtual void OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime);
		Windows::UI::Xaml::Controls::Orientation _orientation;
		std::vector<MeterBarLevel> _levels;
		UINT32 _channelIndex;
		Windows::UI::Xaml::Thickness _elementMargin;
		Windows::UI::Color _unlitElement;
		float _minAmp;
		float _maxAmp;
		bool _displayPeak;
		std::mutex _lock;

	public:
		DiscreteVUBar();

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
    struct DiscreteVUBar : DiscreteVUBarT<DiscreteVUBar, implementation::DiscreteVUBar>
    {
    };
}
