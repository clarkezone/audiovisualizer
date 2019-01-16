#include "pch.h"
#include "AudioViewData.h"

namespace winrt::AudioVisualizer::implementation
{
	AudioViewData::AudioViewData(uint32_t scale, uint32_t capacity) {
		_scale = scale;
		_data.reserve(capacity);
	}

    uint32_t AudioViewData::Scale()
    {
		return _scale;
    }
}
