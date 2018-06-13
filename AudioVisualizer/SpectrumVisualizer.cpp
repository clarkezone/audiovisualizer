#include "pch.h"
#include "SpectrumVisualizer.h"
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::AudioVisualizer::implementation
{

	SpectrumVisualizer::SpectrumVisualizer()
	{
		_unlitElement = Windows::UI::Colors::DarkGray();

		_orientation = Orientation::Vertical; // Vertical bars by default
		_channelIndex = 0; // Map to first channel by default
		_relativeElementMargin = Thickness() = { 0.1,0.1,0.1,0.1 };
		_unlitElement = Color() = { 0xff, 0x40, 0x40, 0x40 };
		_levels.resize(24);
		_minAmp = -60.0;
		_maxAmp = 12.0f;
		_style = SpectrumVisualizationStyle::Blocks;
		int level = -60;
		for (size_t i = 0; i < 24; i++, level += 3)
		{
			_levels[i].Level = (float)level;
			if (level < -6)
				_levels[i].Color = Color() = { 255 , 0, 255, 255 };
			else if (level <= 0)
				_levels[i].Color = Color() = { 255, 255, 255, 0 };
			else
				_levels[i].Color = Color() = { 255, 255, 0, 0 };
		}
	}

	AudioVisualizer::SpectrumVisualizationStyle SpectrumVisualizer::SpectrumStyle()
	{
		return _style;
	}

	void SpectrumVisualizer::SpectrumStyle(AudioVisualizer::SpectrumVisualizationStyle const& value)
	{
		_style = value;
	}

	com_array<AudioVisualizer::MeterBarLevel> SpectrumVisualizer::Levels()
	{
		return com_array<MeterBarLevel>(_levels);
	}

	void SpectrumVisualizer::Levels(array_view<AudioVisualizer::MeterBarLevel const> value)
	{
		_levels.resize(value.size());
		std::copy(value.begin(), value.end(), _levels.begin());
	}

	Windows::UI::Xaml::Controls::Orientation SpectrumVisualizer::Orientation()
	{
		return _orientation;
	}

	void SpectrumVisualizer::Orientation(Windows::UI::Xaml::Controls::Orientation const& value)
	{
		_orientation = value;
	}

	uint32_t SpectrumVisualizer::ChannelIndex()
	{
		return _channelIndex;
	}

	void SpectrumVisualizer::ChannelIndex(uint32_t value)
	{
		_channelIndex = value;
	}

	Windows::UI::Xaml::Thickness SpectrumVisualizer::RelativeElementMargin()
	{
		return _relativeElementMargin;
	}

	void SpectrumVisualizer::RelativeElementMargin(Windows::UI::Xaml::Thickness const& value)
	{
		_relativeElementMargin = value;
	}

	Windows::UI::Color SpectrumVisualizer::UnlitElement()
	{
		return _unlitElement;
	}

	void SpectrumVisualizer::UnlitElement(Windows::UI::Color const& value)
	{
		_unlitElement = value;
	}

	void SpectrumVisualizer::OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime)
	{
		std::lock_guard<std::mutex> lock(_lock);
		SpectrumData data{ nullptr }; 
		if (dataFrame && dataFrame.Spectrum())
		{
			if (dataFrame.Spectrum().AmplitudeScale() == ScaleType::Linear) {
				data = dataFrame.Spectrum().ConvertToDecibels(_minAmp, _maxAmp);
			}
			else {
				data = dataFrame.Spectrum();
			}
		}

		if (!data)
			return;

		float barAbsWidth = (_orientation == Orientation::Vertical ? _swapChainSize.Width : _swapChainSize.Height) / (float)data.FrequencyCount();
		float barAbsHeight = (_orientation == Orientation::Vertical ? _swapChainSize.Height : _swapChainSize.Width);
	
		Windows::Foundation::Collections::IVectorView<float> spectrum{ nullptr };
		if (_channelIndex < data.Size())
		{
			spectrum = data.GetAt(_channelIndex);
		}


		for (size_t barIndex = 0; barIndex < data.FrequencyCount(); barIndex++)
		{
			float level = -100.0f;
			if (spectrum)
			{
				level = spectrum.GetAt(barIndex);
			}
			if (_style == SpectrumVisualizationStyle::Blocks || _style == SpectrumVisualizationStyle::TopBlock)
			{
				Rect elementRect;
				float elementHeight = (barAbsHeight / (float)_levels.size());
				elementRect.X = barIndex * barAbsWidth + barAbsWidth * (float)_relativeElementMargin.Left;
				elementRect.Height = elementHeight * (1.0f - (float)_relativeElementMargin.Top - (float)_relativeElementMargin.Bottom);
				elementRect.Width = (barAbsWidth) * (1.0f - (float)_relativeElementMargin.Left - (float)_relativeElementMargin.Right);

				for (size_t levelIndex = 0; levelIndex < _levels.size(); levelIndex++)
				{
					MeterBarLevel elementLevel = _levels[levelIndex];
					MeterBarLevel nextElementLevel = levelIndex + 1 < _levels.size() ? _levels[levelIndex + 1] : elementLevel;
					Color elementColor;
					if (level >= nextElementLevel.Level)	// Element is fully lit unless only top block style
						elementColor = _style == SpectrumVisualizationStyle::Blocks ? elementLevel.Color : _unlitElement;
					else if (level > elementLevel.Level)
					{
						// Element partly lit
						elementColor = elementLevel.Color;
					}
					else
					{
						elementColor = _unlitElement;
					}
					elementRect.Y = (_levels.size() - levelIndex) * elementHeight + (float)_relativeElementMargin.Top;
					drawingSession.FillRoundedRectangle(elementRect, 0, 0, elementColor);
				}
			}
		}
	}
}
