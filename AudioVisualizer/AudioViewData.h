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
		AudioViewData(nullptr_t) {};
		AudioViewData(uint32_t reserveCapacity, uint32_t detailLevels, uint32_t downsample);
		void Add(Windows::Foundation::Collections::IIterable<float> const& data);

		Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IVectorView<float>> First();
		Windows::Foundation::Collections::IVectorView<float> GetAt(uint32_t index);
		uint32_t Size();
		bool IndexOf(Windows::Foundation::Collections::IVectorView<float> const& value, uint32_t& index);
		uint32_t GetMany(uint32_t startIndex, array_view<Windows::Foundation::Collections::IVectorView<float>> items);
	};

	struct AudioViewDataLevel : implements<AudioViewDataLevel, Windows::Foundation::Collections::IVectorView<float>, Windows::Foundation::Collections::IIterable<float>> {
		winrt::com_ptr<AudioViewData> data;
		uint32_t level;

		Windows::Foundation::Collections::IIterator<float> First();
		float GetAt(uint32_t index);
		uint32_t Size();
		bool IndexOf(float const& value, uint32_t& index);
		uint32_t GetMany(uint32_t startIndex, array_view<float> items);

		AudioViewDataLevel(AudioViewData* pViewData, uint32_t dataLevel)
		{
			data.copy_from(pViewData);
			level = dataLevel;
		}
	};

	struct AudioViewDataEnumerator : implements<AudioViewDataEnumerator, Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IVectorView<float>>>
	{
		winrt::com_ptr<AudioViewData> viewData;
		uint32_t itemIndex = 0;
		winrt::com_ptr<AudioViewDataLevel> current;

		AudioViewDataEnumerator(AudioViewData* pViewData)
		{
			viewData.copy_from(pViewData);
			if (pViewData->data.size() > 0) {
				current = make_self<AudioViewDataLevel>(pViewData, 0);
			}
		}

		bool HasCurrent() {
			return itemIndex < viewData->data.size();
		}

		Windows::Foundation::Collections::IVectorView<float> Current()
		{
			return current.as< Windows::Foundation::Collections::IVectorView<float>>();
		}

		uint32_t GetMany(array_view<Windows::Foundation::Collections::IVectorView<float>> items)
		{
			throw hresult_not_implemented();
		}

		bool MoveNext()
		{
			if (itemIndex < viewData->data.size()) {
				current = make_self<AudioViewDataLevel>(viewData.get(), itemIndex++);
				return itemIndex < viewData->data.size();
			}
			else {
				return false;
			}
		}
	};

}

namespace winrt::AudioVisualizer::factory_implementation
{
	struct AudioViewData : AudioViewDataT<AudioViewData, implementation::AudioViewData>
	{
	};
}
