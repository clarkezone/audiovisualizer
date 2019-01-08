#pragma once

#include "AudioSourceReader.g.h"

/* 
	This wrapper works around MFSourceReader behaviour on position reporting that has some by design issues
	This means that read frame position + duration will not match next read frames position
	
	1. MP3 decoder has first 526 samples missing
	2. Reader calculates the position by adding up REFERENCE_TIME which is 100ns units of time. It is an int type 
	and rounding errors will add up
	3. Depending on the decoder there will be gaps in between samples - i.e time[n] + duration[n] < time[n+1]

	This reader patches this behavoir making sure that read sample times and durations are always consecutive 
	based on the sample counts read

*/



namespace winrt::AudioVisualizer::implementation
{
    struct AudioSourceReader : AudioSourceReaderT<AudioSourceReader>
    {
		winrt::com_ptr<IMFSourceReader> reader;
		int32_t streamIndex = 0;
		AudioSourceReaderFlags flags = (AudioSourceReaderFlags) 0u;
		int64_t currentSampleIndex = -1;
		uint32_t samplesPerSecond = 0;
		uint32_t bytesPerSample = 0;
		bool b_IsMp3 = false;
		void SetFormatProperties();

		REFERENCE_TIME samples_to_time(int64_t samples) {
			return samples * 10000000L / int64_t(samplesPerSecond);
		}
		int64_t time_to_samples(REFERENCE_TIME time) {
			return time / samplesPerSecond;
		}

		AudioSourceReader() = delete;
		AudioSourceReader(Windows::Storage::Streams::IRandomAccessStream const& stream);

		Windows::Foundation::TimeSpan Duration();
		Windows::Media::MediaProperties::IMediaEncodingProperties GetNativeFormat(int32_t streamIndex);

		Windows::Media::MediaProperties::IMediaEncodingProperties Format();
		void Format(Windows::Media::MediaProperties::IMediaEncodingProperties const &format);

		int32_t StreamIndex();
		void StreamIndex(int32_t index);

		AudioSourceReaderFlags Flags()
		{
			return flags;
		}

		void Seek(Windows::Foundation::TimeSpan position);

		Windows::Media::AudioFrame Read();

		static int32_t FirstAudioStreamIndex();

		void Close();
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct AudioSourceReader : AudioSourceReaderT<AudioSourceReader, implementation::AudioSourceReader>
    {
    };
}
