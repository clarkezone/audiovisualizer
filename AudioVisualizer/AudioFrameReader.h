#pragma once
#include "AudioFrameReader.g.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <MemoryBuffer.h>

namespace winrt::AudioVisualizer::implementation
{
    struct AudioFrameReader : AudioFrameReaderT<AudioFrameReader>
    {
		template <typename _Ty> struct enum_base {

			bool HasCurrent() { return currentIndex < length; }
			bool MoveNext() {
				if (HasCurrent()) {
					static_cast<_Ty*>(this)->advance();
					return HasCurrent();
				}
				else
					return false;
			}
			float Current() {
				if (!HasCurrent())
					throw hresult_out_of_bounds();
				return static_cast<_Ty*>(this)->current();
			}
			uint32_t GetMany(array_view<float> items) const {
				throw hresult_not_implemented();
			}

			void Close() {
				if (bufferReference) {
					bufferReference.Close();
					bufferReference = nullptr;
				}
				if (buffer) {
					buffer.Close();
					buffer = nullptr;
				}
				pData = nullptr;
				length = 0;
			}

			Windows::Media::AudioBuffer buffer{ nullptr };
			Windows::Foundation::IMemoryBufferReference bufferReference{ nullptr };

			float* pData = nullptr;
			uint32_t length = 0;
			uint32_t currentIndex = 0;

			enum_base(Windows::Media::AudioFrame const& frame) {
				buffer = frame.LockBuffer(Windows::Media::AudioBufferAccessMode::Read);
				length = buffer.Length() / sizeof(float);
				bufferReference = buffer.CreateReference();
				UINT32 capacity = 0;
				check_hresult(bufferReference.as<::Windows::Foundation::IMemoryBufferByteAccess>()->GetBuffer((BYTE **)& pData, &capacity));
			}
			~enum_base() {
				Close();
			}
		};
		
		struct SimpleEnumerator : implements<SimpleEnumerator, Windows::Foundation::Collections::IIterator<float>>,enum_base<SimpleEnumerator>
		{
			SimpleEnumerator(Windows::Media::AudioFrame const& frame) : enum_base(frame) {};
			float current() { return pData[currentIndex]; }
			void advance() { currentIndex++; }
		};
		struct StrideEnumerator : implements<StrideEnumerator, Windows::Foundation::Collections::IIterator<float>>, enum_base<StrideEnumerator>
		{
			uint32_t stride;
			uint32_t offset;
			StrideEnumerator(Windows::Media::AudioFrame const& frame,uint32_t elementStride,uint32_t elementOffset) : enum_base(frame), stride(elementStride),offset(elementOffset) {};
			float current() { return pData[currentIndex+offset]; }
			void advance() { currentIndex+=stride; }
		};
		struct CombineEnumerator : implements<CombineEnumerator, Windows::Foundation::Collections::IIterator<float>>, enum_base<CombineEnumerator>
		{
			uint32_t channelCount;
			float value;
			bool hasValue;
			CombineEnumerator(Windows::Media::AudioFrame const& frame, uint32_t inputChannelCount) : enum_base(frame), channelCount(inputChannelCount), hasValue(false) {};
			float current() 
			{ 
				if (!hasValue) {
					value = 0.0f;
					for (uint32_t index = currentIndex; index < currentIndex+channelCount; index++)
					{
						value += pData[index];
					}
					value /= (float)channelCount;
				}
				return value; 
			}
			void advance() { currentIndex += channelCount; hasValue = false; }
		};

		struct SimpleReader : implements<SimpleReader, Windows::Foundation::Collections::IIterable<float>>
		{
			Windows::Media::AudioFrame _frame;
			SimpleReader(Windows::Media::AudioFrame const &frame) : _frame(frame) {}
			Windows::Foundation::Collections::IIterator<float> First() {
				return make<SimpleEnumerator>(_frame);
			}
		};

		struct SingleChannelReader : implements<SingleChannelReader, Windows::Foundation::Collections::IIterable<float>>
		{
			Windows::Media::AudioFrame _frame;
			uint32_t stride;
			uint32_t offset;
			SingleChannelReader(Windows::Media::AudioFrame const& frame,uint32_t inputChannelCount,uint32_t selectedChannel) : _frame(frame), stride(inputChannelCount), offset(selectedChannel) {}
			Windows::Foundation::Collections::IIterator<float> First() {
				return make<StrideEnumerator>(_frame,stride,offset);
			}
		};


		Windows::Media::AudioFrame frame;
		uint32_t inputChannelCount = 0;
		int32_t selectedChannel = 0;

		uint32_t SampleCount() const {
			uint32_t count = 0;
			auto buffer = frame.LockBuffer(Windows::Media::AudioBufferAccessMode::Read);
			if (inputChannelCount == 0) {
				count = buffer.Length() / sizeof(float);
			}
			else {
				count = buffer.Length() / (sizeof(float) * inputChannelCount);
			}
			buffer.Close();
			return count;
		}

		AudioFrameReader(Windows::Media::AudioFrame const & inputFrame) : frame(inputFrame) {
		}

		AudioFrameReader(Windows::Media::AudioFrame const& inputFrame,uint32_t channelCount, int32_t selectedChannel) : frame(inputFrame),inputChannelCount(channelCount), selectedChannel(selectedChannel)
		{
		}

		static AudioVisualizer::AudioFrameReader EnumerateAllSamples(Windows::Media::AudioFrame const& frame);
		static AudioVisualizer::AudioFrameReader EnumerateSingleChannel(Windows::Media::AudioFrame const& frame, uint32_t inputChannelCount, uint32_t selectedChannel);
		static AudioVisualizer::AudioFrameReader EnumerateAsMono(Windows::Media::AudioFrame const& frame, uint32_t inputChannelCount);

        Windows::Foundation::Collections::IIterator<float> First();

    };
}
namespace winrt::AudioVisualizer::factory_implementation
{
    struct AudioFrameReader : AudioFrameReaderT<AudioFrameReader, implementation::AudioFrameReader>
    {
    };
}
