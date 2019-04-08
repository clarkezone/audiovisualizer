#pragma once

#include "BinaryScaleManager.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct BinaryScaleManager : BinaryScaleManagerT<BinaryScaleManager>
    {
	private:
		int _exponentBias;
		int _step;
	public:
        BinaryScaleManager();
		BinaryScaleManager(uint32_t offset, uint32_t step);
		AudioVisualizer::ContentScaleRange GetRange(double value);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct BinaryScaleManager : BinaryScaleManagerT<BinaryScaleManager, implementation::BinaryScaleManager>
    {
    };
}
