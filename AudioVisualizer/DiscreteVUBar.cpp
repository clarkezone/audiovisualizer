#include "pch.h"
#include "DiscreteVUBar.h"
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

namespace winrt::AudioVisualizer::implementation
{
	com_array<AudioVisualizer::MeterBarLevel> DiscreteVUBar::Levels()
	{
		throw hresult_not_implemented();
	}

	void DiscreteVUBar::Levels(array_view<AudioVisualizer::MeterBarLevel const> value)
	{
		throw hresult_not_implemented();
	}

	Windows::UI::Xaml::Controls::Orientation DiscreteVUBar::Orientation()
	{
		throw hresult_not_implemented();
	}

	void DiscreteVUBar::Orientation(Windows::UI::Xaml::Controls::Orientation const& value)
	{
		throw hresult_not_implemented();
	}

	uint32_t DiscreteVUBar::ChannelIndex()
	{
		throw hresult_not_implemented();
	}

	void DiscreteVUBar::ChannelIndex(uint32_t value)
	{
		throw hresult_not_implemented();
	}

	Windows::UI::Xaml::Thickness DiscreteVUBar::RelativeElementMargin()
	{
		throw hresult_not_implemented();
	}

	void DiscreteVUBar::RelativeElementMargin(Windows::UI::Xaml::Thickness const& value)
	{
		throw hresult_not_implemented();
	}

	Windows::UI::Color DiscreteVUBar::UnlitElement()
	{
		throw hresult_not_implemented();
	}

	void DiscreteVUBar::UnlitElement(Windows::UI::Color const& value)
	{
		throw hresult_not_implemented();
	}

	void DiscreteVUBar::OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime)
	{
		drawingSession.DrawText(hstring(L"DiscreteVUBar"),0.0f,0.0f,Windows::UI::Colors::Red(),Microsoft::Graphics::Canvas::Text::CanvasTextFormat());
	}
}
