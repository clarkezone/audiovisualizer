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

		auto returnValue = make_self<ScalarData>(_size, ScaleType::Logarithmic);
		size_t vSize = (_size + 3) >> 2;	
		AudioMath::ConvertToLogarithmic(_pData, returnValue->_pData, vSize, minValue, maxValue);
		
		return returnValue.as<winrt::AudioVisualizer::ScalarData>();
	}

	AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFall(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
	{
		if (_amplitudeScale != ScaleType::Linear) {
			throw hresult_invalid_argument(hstring(L"Amplitude scale needs to be linear"));
		}
		if (timeFromPrevious.count() == 0) {
			throw hresult_invalid_argument(hstring(L"Time delta parameter zero"));
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
			pLastData = previous.as<AudioVisualizer::implementation::ScalarData>()->_pData;
		}
		float normalizedRiseTime = riseTime.count() != 0 ? (float)timeFromPrevious.count() / riseTime.count() : std::numeric_limits<float>::infinity();
		float normalizedFallTime = fallTime.count() != 0 ? (float)timeFromPrevious.count() / fallTime.count() : std::numeric_limits<float>::infinity();

		AudioMath::ApplyRiseAndFall(pLastData, _pData, result->_pData, vSize, normalizedRiseTime, normalizedFallTime);

		return result.as<AudioVisualizer::ScalarData>();
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

    AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFallToEmpty(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
		if (timeFromPrevious.count() == 0) {
			throw hresult_invalid_argument(hstring(L"Time delta parameter zero"));
		}
		if (previous == nullptr) {
			throw hresult_invalid_argument(hstring(L"Previous data cannot be null"));
		}

		auto result = make_self<ScalarData>(previous.Size(), ScaleType::Linear);

		float normalizedRiseTime = riseTime.count() != 0 ? (float)timeFromPrevious.count() / riseTime.count() : std::numeric_limits<float>::infinity();
		float normalizedFallTime = fallTime.count() != 0 ? (float)timeFromPrevious.count() / fallTime.count() : std::numeric_limits<float>::infinity();
		size_t vectorSize = (previous.Size() + 3) >> 2;

		AudioMath::ApplyRiseAndFall(previous.as<AudioVisualizer::implementation::ScalarData>()->_pData, nullptr, result->_pData, vectorSize, normalizedRiseTime, normalizedFallTime);
		return result.as<AudioVisualizer::ScalarData> ();
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
