#include "pch.h"
#include "AudioViewData.h"
#include "SampleBuffer.h"
#include <DirectXMath.h>

using namespace winrt::Windows::Media;

namespace winrt::AudioVisualizer::implementation
{
    void AudioViewData::Append(Windows::Media::AudioFrame const& frame)
    {
		Append(frame, 1, 0);
    }

    void AudioViewData::Append(Windows::Media::AudioFrame const& frame, uint32_t stride, uint32_t offset)
    {
		using namespace DirectX;
		if (stride < 1 || offset >= stride)
			throw hresult_invalid_argument();
		
		DirectX::XMVECTOR sum = DirectX::g_XMZero;

		SampleBuffer buffer(frame);

		for (uint32_t index = offset, sum_index=0; index < buffer.Length(); index+=stride, sum_index++)
		{
			XMVectorSetByIndex(sum, buffer[index], sum_index & 3);
			if (sum_index & 3 == 3) {
				_prescaleSum = XMVectorAdd(_prescaleSum,XMVector4LengthSq(sum));
				sum = DirectX::g_XMZero;
				_prescaleBy4Counter++;
				if (_prescaleBy4 == _prescaleBy4Counter) {
					_prescaleBy4Counter = 0;

					AppendValue(XMVectorGetX(XMVectorSqrtEst(_prescaleSum) * _prescaleSumScaler));
				}
			}
		}
    }

	void AudioViewData::AppendValue(float value) {
		values.emplace_back(value);
	}
}
