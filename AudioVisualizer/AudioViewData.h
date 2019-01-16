#pragma once

#include "AudioViewData.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct AudioViewData : AudioViewDataT<AudioViewData>, winrt::vector_view_base<AudioViewData,float>
    {
		uint32_t _scale;
		std::vector<float> _data;

        AudioViewData(uint32_t scale,uint32_t reserveCapacity);

		auto& get_container() const noexcept
		{
			return _data;
		}

        uint32_t Scale();
    };
}
