#pragma once

#include "ScalarData.g.h"
#include <windows.foundation.h>


namespace winrt::AudioVisualizer::implementation
{
	struct VectorDataIterator : implements<VectorDataIterator, Windows::Foundation::Collections::IIterator<float>>
	{
		const float *_pData;
		size_t _size;
		size_t _currentIndex;

		VectorDataIterator(const float *pData, size_t size) {
			_pData = pData;
			_size = size;
			_currentIndex = 0;
		}

		uint32_t GetMany(array_view<float> )
		{
			throw hresult_not_implemented();
		}

		bool MoveNext()
		{
			if (_currentIndex < _size)
			{
				_currentIndex++;
			}
			return  _currentIndex < _size;
		}

		bool HasCurrent()
		{
			return _currentIndex < _size;
		}

		float Current()
		{
			if (_currentIndex >= _size) {
				throw hresult_out_of_bounds();
			}
			return _pData[_currentIndex];
		}

	};

    struct ScalarData : ScalarDataT<ScalarData>
    {
        ScalarData() = delete;
		ScalarData(std::nullptr_t = nullptr) noexcept {}

        AudioVisualizer::ScaleType AmplitudeScale();
        AudioVisualizer::ScalarData ConvertToDecibels(float minValue, float maxValue);
        AudioVisualizer::ScalarData ApplyRiseAndFall(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);
        Windows::Foundation::Collections::IIterator<float> First();
        float GetAt(uint32_t index);
		uint32_t Size() { return _size; }
        bool IndexOf(float const& value, uint32_t& index);
        uint32_t GetMany(uint32_t startIndex, array_view<float> items);

        static AudioVisualizer::ScalarData CreateEmpty(uint32_t channels);
        static AudioVisualizer::ScalarData Create(array_view<float const> values);
        static AudioVisualizer::ScalarData ApplyRiseAndFallToEmpty(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious);
    
		ScaleType _amplitudeScale;
		DirectX::XMVECTOR *_pData;
		size_t _size;

		ScalarData(size_t cElements, ScaleType scaleType = ScaleType::Linear,bool bInitToZero = true);
		ScalarData(array_view<float const> values, ScaleType scaleType = ScaleType::Linear);
		~ScalarData();
	};
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct ScalarData : ScalarDataT<ScalarData, implementation::ScalarData>
    {
    };
}
