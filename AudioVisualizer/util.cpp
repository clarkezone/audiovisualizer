#include "pch.h"
#include "util.h"

winrt::Windows::UI::Composition::CompositionBrush util::make_composition_brush(winrt::Windows::UI::Xaml::Media::Brush const & xamlBrush)
{
	if (!xamlBrush)
		return nullptr;

	auto brushType = winrt::get_class_name(xamlBrush);
	winrt::Windows::UI::Composition::CompositionBrush result{ nullptr };

	if (brushType == L"Windows.UI.Xaml.Media.SolidColorBrush")
	{
	}
	return result;
}

