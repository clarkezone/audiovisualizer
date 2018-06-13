#include "pch.h"
#include "DiscreteVUBar.h"
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

using namespace winrt::Windows::UI::Xaml::Controls;

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
		std::lock_guard<std::mutex> lock(_lock);

		ScalarData data{ nullptr };
		if (dataFrame && dataFrame.RMS() && dataFrame.RMS().AmplitudeScale() == ScaleType::Linear)
		{
				data = dataFrame.RMS().ConvertToDecibels(_minAmp, _maxAmp);
		}
		else if (dataFrame) {
			data = dataFrame.RMS();
		}

		float barAbsWidth = (_orientation == Orientation::Vertical ? _swapChainSize.Width : _swapChainSize.Height);
		float barAbsHeight = (_orientation == Orientation::Vertical ? _swapChainSize.Height : _swapChainSize.Width);

		float level = -100.0f;
		if (data && _channelIndex < data.Size())
		{
			level = data.GetAt(_channelIndex);
		}
		Windows::Foundation::Rect elementRect;
		float elementHeight = (barAbsHeight / (float)_levels.size());
		elementRect.X = barAbsWidth * (float)_elementMargin.Left;
		elementRect.Height = elementHeight * (1.0f - (float)_elementMargin.Top - (float)_elementMargin.Bottom);
		elementRect.Width = (barAbsWidth) * (1.0f - (float)_elementMargin.Left - (float)_elementMargin.Right);

		for (size_t levelIndex = 0; levelIndex < _levels.size(); levelIndex++)
		{
			MeterBarLevel elementLevel = _levels[levelIndex];
			MeterBarLevel nextElementLevel = levelIndex + 1 < _levels.size() ? _levels[levelIndex + 1] : elementLevel;
			Windows::UI::Color elementColor;
			if (level >= nextElementLevel.Level)	// Element is fully lit
				elementColor = elementLevel.Color;
			else if (level > elementLevel.Level)
			{
				// Element partly lit
				elementColor = elementLevel.Color;
			}
			else
			{
				elementColor = _unlitElement;
			}
			elementRect.Y = (_levels.size() - levelIndex) * elementHeight + (float)_elementMargin.Top;
			drawingSession.FillRoundedRectangle(elementRect, 0, 0, elementColor);
		}
	}
}
