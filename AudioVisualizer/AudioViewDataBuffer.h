#pragma once
#include <winrt/Windows.Foundation.h>
#include "AudioViewDataBuffer.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct AudioViewDataBuffer : AudioViewDataBufferT<AudioViewDataBuffer>
    {
		DirectX::XMVECTOR _downSampleSum = DirectX::g_XMZero;
		DirectX::XMVECTOR _downSampleSumScaler;
		DirectX::XMVECTOR _vectorSum = DirectX::g_XMZero;

		uint32_t _vectorSumIndex = 0;
		uint32_t _downSampleBy4 = 0;
		uint32_t _downSampleBy4Counter = 0;

		Windows::Foundation::Collections::IVector<AudioVisualizer::AudioViewData> _data{ nullptr };

		void AppendValue(float value);

		AudioViewDataBuffer(uint32_t reserveCapacity,uint32_t downsampleRate,uint32_t detailLevels);

        void Append(Windows::Media::AudioFrame const& frame);
        void Append(Windows::Media::AudioFrame const& frame, uint32_t stride, uint32_t offset);

		Windows::Foundation::Collections::IVectorView<AudioVisualizer::AudioViewData> Data();

		static Windows::Foundation::IAsyncOperationWithProgress<Windows::Foundation::Collections::IVectorView<AudioVisualizer::AudioViewDataBuffer>, uint32_t> CreateFromFileAsync(Windows::Storage::StorageFile const file, uint32_t readerChannels, array_view<uint32_t const> outputChannels, AudioVisualizer::IAudioViewDataIncrementalLoad const callback);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct AudioViewDataBuffer : AudioViewDataBufferT<AudioViewDataBuffer, implementation::AudioViewDataBuffer>
    {
    };
}
