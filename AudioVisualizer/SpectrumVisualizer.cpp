#include "pch.h"
#include "SpectrumVisualizer.h"

namespace winrt::AudioVisualizer::implementation
{

	SpectrumVisualizer::SpectrumVisualizer()
	{
		_unlitElement = Windows::UI::Colors::DarkGray();
	}

	com_array<AudioVisualizer::MeterBarLevel> SpectrumVisualizer::Levels()
	{
		throw hresult_not_implemented();
	}

	void SpectrumVisualizer::Levels(array_view<AudioVisualizer::MeterBarLevel const> value)
	{
		throw hresult_not_implemented();
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
}
