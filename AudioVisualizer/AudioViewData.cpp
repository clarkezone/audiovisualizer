#include "pch.h"
#include "AudioViewData.h"
#include "AudioSourceReader.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/coroutine.h>
#include <robuffer.h>

namespace winrt::AudioVisualizer::implementation
{
	AudioViewData::AudioViewData(uint32_t reserveCapacity, uint32_t detailLevels, uint32_t downsample) {
		if (downsample < 1) {
			throw winrt::hresult_invalid_argument();
		}
		if (detailLevels < 1 || detailLevels >= 16 || (reserveCapacity != 0 && reserveCapacity >> (detailLevels * 2) == 0))
		{
			throw winrt::hresult_invalid_argument();
		}
		downsampleInput = DirectX::g_XMZero;
		downsampleSumOfSquares = DirectX::g_XMZero;
		downsampleCounter = 0;
		downsampleFactor = downsample;

		uint32_t levelRequiredCapacity = reserveCapacity / downsample;

		levelSumsOfSquares.resize(detailLevels);
		levelSumCounters.resize(detailLevels);

		for (size_t levelIndex = 0; levelIndex < detailLevels; levelIndex++,levelRequiredCapacity << 2)
		{
			levels.push_back(make<LevelData>(levelRequiredCapacity));
		}

	}

	void AudioViewData::AddSquare(float value, uint32_t level)
	{
		auto levelData = winrt::get_self<LevelData>(levels[level]);
		levelData->values.push_back(sqrtf(value));

		auto nextLevel = level + 1;
		if (nextLevel + 1 < levels.size()) {

			levelSumsOfSquares[nextLevel] += value;
			levelSumCounters[nextLevel]++;
			if (levelSumCounters[nextLevel] >= 4) {
				AddSquare(levelSumsOfSquares[nextLevel] / 4, nextLevel);
				levelSumsOfSquares[nextLevel] = 0;
				levelSumCounters[nextLevel] = 0;
			}
		}
	}
	void AudioViewData::Add(Windows::Foundation::Collections::IIterable<float> const& samples)
	{
		auto iterator = samples.First();
		while (iterator.HasCurrent()) {
			Add(iterator.Current());
			iterator.MoveNext();
		}
	}
	void AudioViewData::Add(float value)
	{
		using namespace DirectX;
		uint32_t vectorIndex = downsampleCounter & 3;
		downsampleInput = DirectX::XMVectorSetByIndex(downsampleInput, value, vectorIndex);

		// Calculate sum of squares every 4th element or in case this is the last element in the batch
		if (vectorIndex == 3 || downsampleCounter + 1 == downsampleFactor) {
			downsampleSumOfSquares += downsampleInput * downsampleInput;
			downsampleInput = DirectX::g_XMZero;
		}
		downsampleCounter++;

		if (downsampleCounter == downsampleFactor) {
			AddSquare(XMVectorGetX(XMVectorSum(downsampleSumOfSquares)) / float(downsampleFactor), 0);
			downsampleCounter = 0;
			downsampleSumOfSquares = DirectX::g_XMZero;
		}
	}
	void AudioViewData::Add(Windows::Media::AudioFrame const& frame, array_view<const float> map)
	{
		using namespace DirectX;
		if (map.size() < 1 || map.size() > 8)
			throw hresult_invalid_argument();

		auto buffer = frame.LockBuffer(Windows::Media::AudioBufferAccessMode::Read);
		auto bufferReference = buffer.CreateReference();
		auto byteAccess = bufferReference.as<::Windows::Storage::Streams::IBufferByteAccess>();
		float* pData = nullptr;
		byteAccess->Buffer((byte **) &pData);

		bufferReference.Close();
		buffer.Close();
	}

	winrt::Windows::Foundation::IAsyncActionWithProgress<Windows::Foundation::TimeSpan> AudioViewData::LoadFromStreamAsync(winrt::Windows::Storage::Streams::IRandomAccessStream stream)
	{

		auto progress{ co_await winrt::get_progress_token() };

		co_await winrt::resume_background();

		auto reader = make<AudioSourceReader>(stream);

		Windows::Media::AudioFrame frame{ nullptr };

		uint32_t channelCount = reader.Format().as<Windows::Media::MediaProperties::AudioEncodingProperties>().ChannelCount();

		do {
			frame = reader.Read();
			if (!frame)
				break;

			auto samples = AudioFrameReader::EnumerateAsMono(frame, channelCount);
			Add(samples);

			progress(frame.RelativeTime().Value() + frame.Duration().Value());

		} while (frame != nullptr);
		
		co_return;
	}


}
