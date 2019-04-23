#pragma once

#include "AudioViewData.g.h"
#include <vector>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::AudioVisualizer::implementation
{

	struct AudioViewData : AudioViewDataT<AudioViewData>, 
		winrt::vector_view_base<AudioViewData,Windows::Foundation::Collections::IVectorView<float>>
	{
		struct LevelData : implements<LevelData, Windows::Foundation::Collections::IVectorView<float>, 
			Windows::Foundation::Collections::IIterable<float>>, 
			winrt::vector_view_base<LevelData, float>
		{
			std::vector<float> values;

			auto& get_container() const noexcept
			{
				return values;
			}

			LevelData(int capacity) {
				values.reserve(capacity);
			}
		};

		std::vector<Windows::Foundation::Collections::IVectorView<float>> levels;

		auto& get_container() const noexcept
		{
			return levels;
		}

		DirectX::XMVECTOR downsampleInput;
		DirectX::XMVECTOR downsampleSumOfSquares;
		uint32_t downsampleCounter;
		uint32_t downsampleFactor;

		std::vector<float> levelSumsOfSquares;
		std::vector<uint32_t> levelSumCounters;

		void AddSquare(float value, uint32_t level);

		AudioViewData(uint32_t reserveCapacity, uint32_t detailLevels, uint32_t downsample);
		void Add(float value);
		void Add(Windows::Foundation::Collections::IIterable<float> const& data);
		void Add(Windows::Media::AudioFrame const& frame,array_view<const float> map);

		uint32_t Downsample() {
			return downsampleFactor;
		}

		winrt::Windows::Foundation::IAsyncActionWithProgress<Windows::Foundation::TimeSpan> LoadFromStreamAsync(winrt::Windows::Storage::Streams::IRandomAccessStream stream);
	};



}

namespace winrt::AudioVisualizer::factory_implementation
{
	struct AudioViewData : AudioViewDataT<AudioViewData, implementation::AudioViewData>
	{
	};
}
