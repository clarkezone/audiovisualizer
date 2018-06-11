#include "pch.h"
#include "DiscreteVUBar.h"
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

namespace winrt::AudioVisualizer::implementation
{
	DiscreteVUBar::DiscreteVUBar()
	{
		_levels.resize(24);
		int level = -60;
		for (size_t i = 0; i < 24; i++, level += 3)
		{
			_levels[i].Level = (float)level;
			if (level < -6)
				_levels[i].Color = Windows::UI::ColorHelper::FromArgb(255, 0, 255, 0);
			else if (level <= 0)
				_levels[i].Color = Windows::UI::ColorHelper::FromArgb(255, 255, 0, 0);
			else
				_levels[i].Color = Windows::UI::ColorHelper::FromArgb(255, 0, 0, 0);
		}
		_minAmp = -60.0f;
		_maxAmp = -12.0f;
		_channelIndex = 0;
		_orientation = Windows::UI::Xaml::Controls::Orientation::Vertical;
		_elementMargin = Windows::UI::Xaml::ThicknessHelper::FromUniformLength(0);
		_unlitElement = Windows::UI::ColorHelper::FromArgb( 0, 96, 96, 96 );

		SizeChanged(Windows::UI::Xaml::SizeChangedEventHandler(
			[=](Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::SizeChangedEventArgs const& args)
			{
				_controlSize = args.NewSize();
			}
		));
	}

	com_array<AudioVisualizer::MeterBarLevel> DiscreteVUBar::Levels()
	{
		return com_array<MeterBarLevel>(_levels);
	}

	void DiscreteVUBar::Levels(array_view<AudioVisualizer::MeterBarLevel const> value)
	{
		_levels.resize(value.size());
		std::copy(value.begin(), value.end(), _levels.begin());
	}

	Windows::UI::Xaml::Controls::Orientation DiscreteVUBar::Orientation()
	{
		return _orientation;
	}

	void DiscreteVUBar::Orientation(Windows::UI::Xaml::Controls::Orientation const& value)
	{
		_orientation = value;
	}

	uint32_t DiscreteVUBar::ChannelIndex()
	{
		return _channelIndex;
	}

	void DiscreteVUBar::ChannelIndex(uint32_t value)
	{
		_channelIndex = value;
	}

	Windows::UI::Xaml::Thickness DiscreteVUBar::RelativeElementMargin()
	{
		return _elementMargin;
	}

	void DiscreteVUBar::RelativeElementMargin(Windows::UI::Xaml::Thickness const& value)
	{
		_elementMargin = value;
	}

	Windows::UI::Color DiscreteVUBar::UnlitElement()
	{
		return _unlitElement;
	}

	void DiscreteVUBar::UnlitElement(Windows::UI::Color const& value)
	{
		_unlitElement = value;
	}

	void DiscreteVUBar::OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime)
	{
		drawingSession.DrawText(hstring(L"DiscreteVUBar"),0.0f,0.0f,Windows::UI::Colors::Red(),Microsoft::Graphics::Canvas::Text::CanvasTextFormat());
	}
}
