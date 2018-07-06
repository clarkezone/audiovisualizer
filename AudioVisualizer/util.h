#pragma once
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.h>

namespace util
{
	template<typename EnumType>
	inline bool enum_has_flag(const EnumType& value, const EnumType & flag)
	{
		return (value & flag) != static_cast<EnumType>(0);
	}

	winrt::Windows::UI::Composition::CompositionBrush make_composition_brush(winrt::Windows::UI::Xaml::Media::Brush const &xamlBrush);
}
