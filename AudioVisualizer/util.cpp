#include "pch.h"
#include "util.h"
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.Foundation.Collections.h>

util::performance_frequency util::stopwatch::_frequency;


winrt::Windows::UI::Composition::CompositionBrush util::make_composition_brush(winrt::Windows::UI::Xaml::Media::Brush const & xamlBrush,winrt::Windows::UI::Composition::Compositor const & compositor)
{
	using namespace winrt::Windows::UI::Composition;
	using namespace winrt::Windows::UI::Xaml::Media;
	if (!xamlBrush)
		return nullptr;

	auto brushType = winrt::get_class_name(xamlBrush);
	CompositionBrush result{ nullptr };

	if (brushType == L"Windows.UI.Xaml.Media.SolidColorBrush")
	{
		auto xamlColorBrush = xamlBrush.as<SolidColorBrush>();
		auto color = xamlColorBrush.Color();
		result = compositor.CreateColorBrush(color);
	}
	if (brushType == L"Windows.UI.Xaml.Media.LinearGradientBrush")
	{
		auto xamlLinearGradientBrush = xamlBrush.as<LinearGradientBrush>();
		auto gradientBrush = compositor.CreateLinearGradientBrush();
		gradientBrush.StartPoint(gradientBrush.StartPoint());
		gradientBrush.EndPoint(gradientBrush.EndPoint());
		for (auto gradientStop : xamlLinearGradientBrush.GradientStops()) {
			auto compGradientStop = compositor.CreateColorGradientStop((float) gradientStop.Offset(), gradientStop.Color());
			gradientBrush.ColorStops().Append(compGradientStop);
		}
		result = gradientBrush;
	}
	return result;
}

