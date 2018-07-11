#include "pch.h"
#include "util.h"
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Xaml.Media.h>

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
	return result;
}

