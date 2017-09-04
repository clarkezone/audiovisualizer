#pragma once
#include <winrt/base.h>

namespace AudioVisualizer
{
	namespace winrt::AudioVisualizer::VisualizationData::implementation
	{	
		template <typename D, typename ... I>
		struct VisualizationData_base : ::winrt::impl::module_lock, ::winrt::implements<D, AudioVisualizer::IVisualizationData, I ...>
		{
			using class_type = AudioVisualizer::VisualizationData;

			operator class_type() const noexcept
			{
				class_type result{ nullptr };
				::winrt::attach_abi(result, ::winrt::detach_abi(static_cast<typename D::first_interface>(*this)));
				return result;
			}

			hstring GetRuntimeClassName() const
			{
				::winrt::imp
				return L"AudioVisualizer.VisualizationData";
			}
		};

		template <typename D, typename ... I>
		using VisualizationDataT = VisualizationData_base<D, I ...>;

		struct VisualizationData : VisualizationDataT<VisualizationData>
		{
			VisualizationData() = default;
		private:

		};
	}
}

