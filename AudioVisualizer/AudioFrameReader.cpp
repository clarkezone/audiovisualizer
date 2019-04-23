#include "pch.h"
#include "AudioFrameReader.h"
#include <MemoryBuffer.h>

namespace winrt::AudioVisualizer::implementation
{

	AudioVisualizer::AudioFrameReader AudioFrameReader::EnumerateAllSamples(Windows::Media::AudioFrame const& frame)
	{
		return make<AudioFrameReader>(frame);
	}
	AudioVisualizer::AudioFrameReader AudioFrameReader::EnumerateSingleChannel(Windows::Media::AudioFrame const& frame,uint32_t inputChannelCount,uint32_t selectedChannel)
	{
		if (inputChannelCount == 0 || selectedChannel >= inputChannelCount)
			throw hresult_invalid_argument();

		return make<AudioFrameReader>(frame,inputChannelCount,selectedChannel);
	}

	AudioVisualizer::AudioFrameReader AudioFrameReader::EnumerateAsMono(Windows::Media::AudioFrame const& frame, uint32_t inputChannelCount)
	{
		return make<AudioFrameReader>(frame, inputChannelCount, -1);
	}

    Windows::Foundation::Collections::IIterator<float> AudioFrameReader::First()
    {
		if (inputChannelCount == 0) {
			return make<SimpleEnumerator>(frame);
		}
		else if (selectedChannel >= 0) {
			return make<StrideEnumerator>(frame,inputChannelCount,selectedChannel);
		}
		else {
			return make<CombineEnumerator>(frame, inputChannelCount);
		}
		throw hresult_not_implemented();
    }
}
