#include "pch.h"
#include "AudioViewData.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::AudioVisualizer::implementation
{
	void AudioViewData::AddSquare(float value, uint32_t level)
	{
		data[level].push_back(sqrtf(value) );

		auto nextLevel = level + 1;
		if (nextLevel + 1 < data.size()) {
			
			levelSumsOfSquares[nextLevel] += value;
			levelSumCounters[nextLevel]++;
			if (levelSumCounters[nextLevel] >= 4) {
				AddSquare(levelSumsOfSquares[nextLevel] / 4, nextLevel);
				levelSumsOfSquares[nextLevel] = 0;
				levelSumCounters[nextLevel] = 0;
			}
		}
	}

	AudioViewData::AudioViewData(uint32_t reserveCapacity, uint32_t detailLevels, uint32_t downsample) {
		if ((downsample & 3) != 0 || downsample < 4) {
			throw winrt::hresult_invalid_argument();
		}
		if (detailLevels < 1 || detailLevels >= 16 || reserveCapacity >> (detailLevels * 2) == 0)
		{
			throw winrt::hresult_invalid_argument();
		}
		downsampleInput = DirectX::g_XMZero;
		downsampleSumOfSquares = DirectX::g_XMZero;
		downsampleInputCounter = 0;
		downsampleCounter = 0;
		downsampleFactor = downsample;
		downsampleVectorCount = downsample >> 2;

		uint32_t levelRequiredCapacity = reserveCapacity / downsample;

		data.resize(detailLevels);
		levelSumsOfSquares.resize(detailLevels);
		levelSumCounters.resize(detailLevels);

		for (size_t levelIndex = 0; levelIndex < detailLevels; levelIndex++,levelRequiredCapacity << 2)
		{
			data[levelIndex].reserve(levelRequiredCapacity);
		}

	}
	void AudioViewData::Add(Windows::Foundation::Collections::IIterable<float> const& samples)
	{
		using namespace DirectX;

		auto iterator = samples.First();
		while (iterator.HasCurrent()) {
			downsampleInput = DirectX::XMVectorSetByIndex(downsampleInput, iterator.Current(), downsampleInputCounter++);
			if (downsampleInputCounter >= 4) {
				downsampleSumOfSquares += downsampleInput * downsampleInput;
				downsampleInput = DirectX::g_XMZero;
				downsampleInputCounter = 0;
				downsampleCounter++;
				if (downsampleCounter >= downsampleVectorCount) {
					AddSquare(XMVectorGetX(XMVectorSum(downsampleSumOfSquares)) / float(downsampleFactor)  ,0);
					downsampleCounter = 0;
				}
			}

			iterator.MoveNext();
		}
	}

	Windows::Foundation::Collections::IIterator<Windows::Foundation::Collections::IVectorView<float>> AudioViewData::First()
	{
		return winrt::make<AudioViewDataEnumerator>(this);
	}

	Windows::Foundation::Collections::IVectorView<float> AudioViewData::GetAt(uint32_t index)
	{
		if (index >= data.size())
			throw hresult_invalid_argument();

		return AudioViewDataLevel(this, index);
	}

	uint32_t AudioViewData::Size()
	{
		return data.size();
	}

	bool AudioViewData::IndexOf(Windows::Foundation::Collections::IVectorView<float> const& value, uint32_t& index)
	{
		throw hresult_not_implemented();
	}
	uint32_t AudioViewData::GetMany(uint32_t startIndex, array_view<Windows::Foundation::Collections::IVectorView<float>> items)
	{
		throw hresult_not_implemented();
	}

	Windows::Foundation::Collections::IIterator<float> AudioViewDataLevel::First()
	{
		return Windows::Foundation::Collections::IIterator<float>();
	}

	float AudioViewDataLevel::GetAt(uint32_t index)
	{
		return 0.0f;
	}

	uint32_t AudioViewDataLevel::Size()
	{
		return uint32_t();
	}

	bool AudioViewDataLevel::IndexOf(float const& value, uint32_t& index)
	{
		return false;
	}

	uint32_t AudioViewDataLevel::GetMany(uint32_t startIndex, array_view<float> items)
	{
		return uint32_t();
	}

}
