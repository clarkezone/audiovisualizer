#include "pch.h"
#include "BarBaseVisualizer.h"

namespace winrt::AudioVisualizer::implementation
{
	BarBaseVisualizer::BarBaseVisualizer()
	{
		_levels.resize(23);
		int level = -60;
		for (size_t i = 0; i < 23; i++, level += 3)
		{
			_levels[i].Level = (float)level;
			if (level < -6)
				_levels[i].Color = Windows::UI::Colors::Lime();
			else if (level <= 0)
				_levels[i].Color = Windows::UI::Colors::Yellow();
			else
				_levels[i].Color = Windows::UI::Colors::Red();
		}
		_channelIndex = 0;
		_orientation = Windows::UI::Xaml::Controls::Orientation::Vertical;
		_elementMargin = Windows::UI::Xaml::ThicknessHelper::FromUniformLength(0);
		_unlitElement = Windows::UI::Colors::DarkGray();
	}
	void BarBaseVisualizer::DrawBar(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, float value, float auxValue, winrt::Windows::Foundation::Rect barRect)
	{
		using namespace winrt::Windows::Foundation;
		using namespace winrt::Windows::UI::Xaml::Controls;
		using namespace winrt::Windows::UI::Xaml;

		int levelIndex = -1;
		if (value > _levels.front().Level) {
			auto level = std::find_if(std::begin(_levels), std::end(_levels), [=](const MeterBarLevel &level) { return level.Level >= value; });
			if (level != std::end(_levels)) {
				levelIndex = level - std::begin(_levels);
			}
			else {
				levelIndex = _levels.size();
			}
		}
		int auxLevelIndex = -1;
		if (auxValue > _levels.front().Level) {
			auto level = std::find_if(std::begin(_levels), std::end(_levels), [=](const MeterBarLevel &level) { return level.Level >= value; });
			if (level != std::end(_levels)) {
				auxLevelIndex = level - std::begin(_levels);
			}
			else {
				auxLevelIndex = _levels.size();
			}
		}

		Size cellSize = _orientation == Orientation::Vertical ? Size(barRect.Width, barRect.Height / _levels.size()) : Size(barRect.Width / _levels.size(), barRect.Height);
		Thickness cellMargins = ThicknessHelper::FromLengths(_elementMargin.Left * cellSize.Width, _elementMargin.Top * cellSize.Height, _elementMargin.Right * cellSize.Width, _elementMargin.Bottom * cellSize.Height);
		Rect cellRect = _orientation == Orientation::Vertical ? Rect(barRect.X, barRect.Height - cellSize.Height, cellSize.Width, cellSize.Height) : Rect(barRect.Y, barRect.X, cellSize.Width, cellSize.Height);
		Rect elementRect = Rect(cellRect.X + (float)cellMargins.Left,
			cellRect.Y + (float)cellMargins.Top,
			cellRect.Width - (float)cellMargins.Left - (float)cellMargins.Right,
			cellRect.Height - (float)cellMargins.Top - (float)cellMargins.Bottom);

		int elementIndex = 0;
		for (auto element : _levels)
		{
			auto elementColor = elementIndex <= levelIndex || elementIndex == auxLevelIndex ? element.Color : _unlitElement;
			drawingSession.FillRectangle(elementRect, elementColor);
			if (_orientation == Orientation::Vertical) {
				elementRect.Y -= cellSize.Height;
			}
			else {
				elementRect.X += cellSize.Width;
			}
			elementIndex++;
		}
	}
	com_array<AudioVisualizer::MeterBarLevel> BarBaseVisualizer::Levels()
	{
		return com_array<MeterBarLevel>(_levels);
	}
	void BarBaseVisualizer::Levels(array_view<AudioVisualizer::MeterBarLevel const> value)
	{
		if (value.data() == nullptr || value.size() < 1)
			throw winrt::hresult_invalid_argument(L"Value cannot be empty");

		float lastValue = value.at(0).Level;
		for (size_t index = 1; index < value.size(); index++)
		{
			if (value.at(index).Level <= lastValue)
				throw winrt::hresult_invalid_argument(L"Array elements need to be in ascending order");
			lastValue = value.at(index).Level;
		}

		_levels.resize(value.size());
		std::copy(value.begin(), value.end(), _levels.begin());
	}
	Windows::UI::Xaml::Controls::Orientation BarBaseVisualizer::Orientation()
	{
		return _orientation;
	}
	void BarBaseVisualizer::Orientation(Windows::UI::Xaml::Controls::Orientation const & value)
	{
		_orientation = value;
	}
	uint32_t BarBaseVisualizer::ChannelIndex()
	{
		return _channelIndex;
	}
	void BarBaseVisualizer::ChannelIndex(uint32_t value)
	{
		_channelIndex = value;
	}
	Windows::UI::Xaml::Thickness BarBaseVisualizer::RelativeElementMargin()
	{
		return _elementMargin;
	}
	void BarBaseVisualizer::RelativeElementMargin(Windows::UI::Xaml::Thickness const & value)
	{
		_elementMargin = value;
	}
	Windows::UI::Color BarBaseVisualizer::UnlitElement()
	{
		return _unlitElement;
	}
	void BarBaseVisualizer::UnlitElement(Windows::UI::Color const & value)
	{
		_unlitElement = value;
	}

}