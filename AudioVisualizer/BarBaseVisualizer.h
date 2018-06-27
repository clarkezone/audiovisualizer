#pragma once
#include "VisualizerControl.h"
#include <winrt/windows.ui.xaml.controls.h>
#include "winrt/Microsoft.Graphics.Canvas.UI.h"

namespace winrt::AudioVisualizer::implementation
{
	struct BarBaseVisualizer
	{
	protected:
		Windows::UI::Xaml::Controls::Orientation _orientation;
		std::vector<MeterBarLevel> _levels;
		UINT32 _channelIndex;
		Windows::UI::Xaml::Thickness _elementMargin;
		Windows::UI::Color _unlitElement;
		std::mutex _lock;

		void DrawBar(Microsoft::Graphics::Canvas::CanvasDrawingSession session,float value, float auxValue,winrt::Windows::Foundation::Rect rect);

	public:
		com_array<AudioVisualizer::MeterBarLevel> Levels();
		void Levels(array_view<AudioVisualizer::MeterBarLevel const> value);
		Windows::UI::Xaml::Controls::Orientation Orientation();
		void Orientation(Windows::UI::Xaml::Controls::Orientation const& value);
		int32_t ChannelIndex();
		void ChannelIndex(int32_t value);
		Windows::UI::Xaml::Thickness RelativeElementMargin();
		void RelativeElementMargin(Windows::UI::Xaml::Thickness const& value);
		Windows::UI::Color UnlitElement();
		void UnlitElement(Windows::UI::Color const& value);
		BarBaseVisualizer();

	};

}
