#pragma once

#include "AudioViewData.g.h"
#include <vector>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::AudioVisualizer::implementation
{
    struct AudioViewData : AudioViewDataT<AudioViewData>
    {
		DirectX::XMVECTOR downsampleInput;
		DirectX::XMVECTOR downsampleSumOfSquares;
		uint32_t downsampleInputCounter;
		uint32_t downsampleCounter;
		uint32_t downsampleVectorCount;
		uint32_t downsampleFactor;

		std::vector<std::vector<float>> data;
		std::vector<float> levelSumsOfSquares;
		std::vector<uint32_t> levelSumCounters;

		void AddSquare(float value, uint32_t level);

		AudioViewData(uint32_t reserveCapacity, uint32_t detailLevels, uint32_t downsample);
		void Add(Windows::Foundation::Collections::IIterable<float> const& data);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
	struct AudioViewData : AudioViewDataT<AudioViewData, implementation::AudioViewData>
	{
	};
}
