#include "pch.h"
#include "ScalarData.h"
#include <memory>
#include <winrt/Windows.Foundation.Collections.h>
#include <AudioMath.h>


namespace winrt::AudioVisualizer::implementation
{
    AudioVisualizer::ScaleType ScalarData::AmplitudeScale()
    {
        return _amplitudeScale;
    }

    Windows::Foundation::Collections::IIterator<float> ScalarData::First()
    {
		return make<VectorDataIterator>((float *)_pData, _size);
    }

    float ScalarData::GetAt(uint32_t index)
    {
		if (index >= _size)
			throw hresult_out_of_bounds();
		return ((float*)_pData)[index];
	}

    bool ScalarData::IndexOf(float const& , uint32_t& )
    {
		throw hresult_not_implemented();
    }

    uint32_t ScalarData::GetMany(uint32_t , array_view<float> )
    {
		throw hresult_not_implemented();
	}

	AudioVisualizer::ScalarData ScalarData::ConvertToDecibels(float minValue, float maxValue)
	{
		if (_amplitudeScale == ScaleType::Logarithmic)
			throw hresult_error(E_NOT_VALID_STATE,hstring(L"Value already logarithmic"));
		if (maxValue <= minValue)
			throw hresult_invalid_argument();

		auto returnValue = make_self<ScalarData>(Size(), ScaleType::Logarithmic);
		size_t vSize = (Size() + 3) >> 2;	
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, vSize, minValue, maxValue);
		
		return returnValue.as<AudioVisualizer::ScalarData>();
	}

	AudioVisualizer::ScalarData ScalarData::CreateEmpty(uint32_t channels)
    {
		return make<ScalarData>(channels, ScaleType::Linear);
    }

    AudioVisualizer::ScalarData ScalarData::Create(array_view<float const> values)
    {
		auto value = make<ScalarData>(values,ScaleType::Linear);
		return value;
    }

	AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFall(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
	{
		if (_amplitudeScale != ScaleType::Linear) {
			throw hresult_invalid_argument(hstring(L"Amplitude scale needs to be linear"));
		}

		if (timeFromPrevious.count() == 0) {	
			return previous;	// No time has passed so no changes
		}

		if (previous != nullptr)
		{
			if (previous.AmplitudeScale() == ScaleType::Logarithmic)
				throw hresult_invalid_argument(hstring(L"Amplitude scale needs to be linear"));

			if (previous.Size() != _size)
				throw hresult_invalid_argument(hstring(L"Data needs to be of same size"));
		}
		auto result = make_self<ScalarData>(_size, _amplitudeScale);

		size_t vSize = (_size + 3) >> 2;
		DirectX::XMVECTOR *pLastData = nullptr;

		if (previous != nullptr) {
			// Get the raw buffer
			pLastData = winrt::from_abi<ScalarData>(previous)->_pData;
		}
		float normalizedRiseTime = (float)timeFromPrevious.count() / (float)riseTime.count();
		float normalizedFallTime = (float)timeFromPrevious.count() / (float)fallTime.count();

		AudioMath::ApplyRiseAndFall(pLastData, _pData, result->_pData, vSize, normalizedRiseTime, normalizedFallTime);

		return result.as<AudioVisualizer::ScalarData>();
	}

	AudioVisualizer::ScalarData ScalarData::CombineChannels(array_view<float const> map)
	{
		if (map.data() == nullptr)
		{
			throw hresult_error(E_POINTER);
		}
		if (map.size() < Size()) {
			throw hresult_invalid_argument();
		}
		if (AmplitudeScale() == ScaleType::Logarithmic) {
			throw hresult_error(E_NOT_VALID_STATE);
		}

		uint32_t newSize = map.size() / _size;

		auto result = make_self<ScalarData>(newSize, AmplitudeScale(), false);

		uint32_t mapIndex = 0;
		for (size_t resultIndex = 0; resultIndex < newSize; resultIndex++)
		{
			float sum = 0;
			for (size_t sourceIndex = 0; sourceIndex < Size(); sourceIndex++,mapIndex++)
			{
				sum += reinterpret_cast<float*>(_pData)[sourceIndex] * map.at(mapIndex);
			}
			reinterpret_cast<float *>(result->_pData)[resultIndex] = sum;
		}
		return result.as<AudioVisualizer::ScalarData>();
	}

    AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFallToEmpty(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
		if (previous == nullptr) {
			throw hresult_invalid_argument(hstring(L"Previous data cannot be null"));
		}
		if (timeFromPrevious.count() == 0) {
			return previous;
		}

		auto result = make_self<ScalarData>(previous.Size(), ScaleType::Linear);

		size_t vectorSize = (previous.Size() + 3) >> 2;

		float normalizedRiseTime = (float)timeFromPrevious.count() / (float)riseTime.count();
		float normalizedFallTime = (float)timeFromPrevious.count() / (float)fallTime.count();

		AudioMath::ApplyRiseAndFall(winrt::from_abi<ScalarData>(previous)->_pData, nullptr, result->_pData, vectorSize, normalizedRiseTime, normalizedFallTime);

		return result.as<AudioVisualizer::ScalarData>();
    }

	ScalarData::ScalarData(size_t cElements, ScaleType scaleType,bool bInitToZero)
	{
		_amplitudeScale = scaleType;
		size_t vectorLength = (cElements + 3) >> 2;
		_pData = static_cast<DirectX::XMVECTOR*>(_aligned_malloc(sizeof(DirectX::XMVECTOR)*vectorLength,16));
		_pData[vectorLength - 1] = DirectX::g_XMZero;	// Always pad with zero

		if (bInitToZero) {
			memset(_pData, 0, vectorLength * sizeof(DirectX::XMVECTOR));
		}
		_size = cElements;
	}
	ScalarData::ScalarData(array_view<float const> values, ScaleType scaleType) : ScalarData(values.size(),scaleType,false)
	{
		memcpy(_pData, values.data(), sizeof(float)*values.size());
	}

	ScalarData::~ScalarData()
	{
		if (_pData != nullptr)
			_aligned_free(_pData);
		_pData = nullptr;
	}
}
