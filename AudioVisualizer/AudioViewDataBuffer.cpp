#include "pch.h"
#include "AudioViewDataBuffer.h"
#include "AudioViewData.h"
#include "SampleBuffer.h"
#include <DirectXMath.h>

using namespace winrt::Windows::Media;

namespace winrt::AudioVisualizer::implementation
{
	AudioViewDataBuffer::AudioViewDataBuffer(uint32_t reserveCapacity, uint32_t downsampleRate, uint32_t detailLevels)
	{
		if ((downsampleRate & 3) != 0 || downsampleRate == 0) {
			throw hresult_invalid_argument(L"Invalid downsample rate");
		}
		if (detailLevels < 1 || detailLevels >= 16) {
			throw hresult_invalid_argument(L"Invalid detail level");
		}
		_downSampleBy4 = downsampleRate >> 2;
		_downSampleSumScaler = DirectX::XMVectorReplicate(1.0f / float(downsampleRate));

		uint32_t scale = downsampleRate;

		_data = single_threaded_vector<AudioVisualizer::AudioViewData>();
		for (size_t level = 0; level < detailLevels; level++, scale <<= 2, reserveCapacity <<= 2)
		{
			auto levelData = make<AudioViewData>(scale, reserveCapacity);
			_data.Append(levelData);
		}
	}

    void AudioViewDataBuffer::Append(Windows::Media::AudioFrame const& frame)
    {
		Append(frame, 1, 0);
    }

    void AudioViewDataBuffer::Append(Windows::Media::AudioFrame const& frame, uint32_t stride, uint32_t offset)
    {
		using namespace DirectX;
		if (stride < 1 || offset >= stride)
			throw hresult_invalid_argument();
		
		SampleBuffer buffer(frame);

		for (uint32_t index = offset; index < buffer.Length(); index+=stride)
		{
			_vectorSum = XMVectorSetByIndex(_vectorSum, buffer[index], _vectorSumIndex );
			_vectorSumIndex++;
			if (_vectorSumIndex == 4) {
				auto vectorLengthSquared = DirectX::XMVector4LengthSq(_vectorSum);
				_downSampleSum = XMVectorAdd(_downSampleSum,vectorLengthSquared);
				_vectorSum = DirectX::g_XMZero;
				_vectorSumIndex = 0;
				_downSampleBy4Counter++;
				if (_downSampleBy4 == _downSampleBy4Counter) {
					_downSampleBy4Counter = 0;
					auto sqrtSum = XMVectorSqrtEst(_downSampleSum);
					AppendValue(XMVectorGetX( sqrtSum * _downSampleSumScaler));
					_downSampleSum = DirectX::g_XMZero;
				}
			}
		}
    }

	void AudioViewDataBuffer::AppendValue(float value) {

		winrt::AudioVisualizer::implementation::AudioViewData *previous = nullptr;
		for (size_t levelIndex = 0; levelIndex < _data.Size(); levelIndex++)
		{
			auto data = winrt::get_self<AudioViewData>(_data.GetAt(levelIndex));
			if (levelIndex == 0) {
				data->_data.push_back(value);
				if ((data->Size() & 3) != 0)
					break;
			}
			else {
				// Take last 4 elements of the previous level
				uint32_t index = previous->Size() - 4;
				auto v = DirectX::XMVectorSet(previous->_data[index], previous->_data[index + 1], previous->_data[index + 2], previous->_data[index + 3]);
				data->_data.push_back(DirectX::XMVectorGetX(DirectX::XMVector4Length(v))/4.0f);
			}
			previous = data;
			if ((data->Size() & 3) != 0)
				break;
		}
	}

	Windows::Foundation::Collections::IVectorView<AudioVisualizer::AudioViewData> AudioViewDataBuffer::Data()
	{
		return _data.GetView();
	}
}
