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

	struct performance_frequency
	{
		LARGE_INTEGER frequency;
		performance_frequency()
		{
			frequency.QuadPart = 0;
			QueryPerformanceFrequency(&frequency);
		}
	};

	struct stopwatch {
	private:
		static performance_frequency _frequency;
		LARGE_INTEGER _start;
	public:
		stopwatch() {
			start();
		}
		winrt::Windows::Foundation::TimeSpan time() {
			LARGE_INTEGER pfc;
			pfc.QuadPart = 0;
			QueryPerformanceCounter(&pfc);
			return winrt::Windows::Foundation::TimeSpan(10000000L * (pfc.QuadPart - _start.QuadPart) / _frequency.frequency.QuadPart);
		}
		void start()
		{
			_start.QuadPart = 0;
			QueryPerformanceCounter(&_start);
		}
	};

	winrt::Windows::UI::Composition::CompositionBrush make_composition_brush(winrt::Windows::UI::Xaml::Media::Brush const &xamlBrush,winrt::Windows::UI::Composition::Compositor const & compositor);
}
