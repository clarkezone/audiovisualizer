#pragma once

#include "AudioViewData.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct AudioViewData : AudioViewDataT<AudioViewData>
    {
		uint32_t _prescaleBy4 = 12;
		uint32_t _prescaleBy4Counter = 0;
		DirectX::XMVECTOR _prescaleSum = DirectX::g_XMZero;
		DirectX::XMVECTOR _prescaleSumScaler = DirectX::XMVectorReplicate(1.0f/48.0f);
		std::vector<float> values;

		void AppendValue(float value);

        AudioViewData() = default;

        void Append(Windows::Media::AudioFrame const& frame);
        void Append(Windows::Media::AudioFrame const& frame, uint32_t stride, uint32_t offset);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct AudioViewData : AudioViewDataT<AudioViewData, implementation::AudioViewData>
    {
    };
}
