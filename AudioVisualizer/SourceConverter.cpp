#include "pch.h"
#include "SourceConverter.h"

namespace winrt::AudioVisualizer::implementation
{
	template<typename T>
	bool IsValueReferenceEqual(Windows::Foundation::IReference<T> value1,Windows::Foundation::IReference<T> value2)
	{
		if (!value1 && !value2) // Both values are nullptr, no change
		{
			return true;
		}
		else if (value1 && value2 && value1.Value() == value2.Value())	// No change in value, return
		{
			return true;
		}
		return false;
	}

	SourceConverter::SourceConverter()
	{
		_analyzerTypes = AnalyzerType::All;
		_bCacheData = true;
		_timeFromPrevious = Windows::Foundation::TimeSpan(166667);	// Default to 1/60sec
	}

    AudioVisualizer::IVisualizationSource SourceConverter::Source()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		return _source;
    }

    void SourceConverter::Source(AudioVisualizer::IVisualizationSource const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		if (_source)
		{
			_source.ConfigurationChanged(_sourceChanged);
		}
		_source = value;

		if (_source)
		{
			_source.ConfigurationChanged(
				Windows::Foundation::TypedEventHandler<IVisualizationSource, hstring>(
					[this](IVisualizationSource sender, hstring propertyName) {
						_configurationChangedEvent(sender, propertyName);
					}
					)
			);
		}
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"Source"));
	}

    Windows::Foundation::IReference<uint32_t> SourceConverter::FrequencyCount()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		return _frequencyCount;
    }

    void SourceConverter::FrequencyCount(Windows::Foundation::IReference<uint32_t> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		if (value && value.GetUInt32() == 0) {
			throw hresult_invalid_argument();
		}
		if (IsValueReferenceEqual(value, _frequencyCount))
			return;

		_frequencyCount = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"FrequencyCount"));
	}

    Windows::Foundation::IReference<uint32_t> SourceConverter::ChannelCount()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		return _channelCount;
	}

    void SourceConverter::ChannelCount(Windows::Foundation::IReference<uint32_t> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		if (value && value.GetUInt32() == 0) {
			throw hresult_invalid_argument();
		}
		if (IsValueReferenceEqual(value, _channelCount))
			return;

		_channelCount = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"ChannelCount"));
    }

    com_array<float> SourceConverter::ChannelMapping()
    {
        throw hresult_not_implemented();
    }

    void SourceConverter::ChannelMapping(array_view<float const> value)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<float> SourceConverter::MinFrequency()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		return _minFrequency;
	}

    void SourceConverter::MinFrequency(Windows::Foundation::IReference<float> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		if (value) {
			float newValue = value.GetSingle();
			ScaleType fScale = _frequencyScale ? _frequencyScale.Value() : ScaleType::Linear;
			float maxFrequency = _maxFrequency ? _maxFrequency.GetSingle() : std::numeric_limits<float>::max();
			
			if ((fScale == ScaleType::Logarithmic && newValue == 0) || newValue < 0 || newValue >= maxFrequency)
			{
				throw hresult_invalid_argument();
			}
		}
		if (IsValueReferenceEqual(value, _minFrequency))
			return;
		_minFrequency = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"MinFrequency"));
	}

    Windows::Foundation::IReference<float> SourceConverter::MaxFrequency()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		return _maxFrequency;
    }

    void SourceConverter::MaxFrequency(Windows::Foundation::IReference<float> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		if (value) {
			float newValue = value.GetSingle();
			ScaleType fScale = _frequencyScale ? (ScaleType)_frequencyScale.GetInt32() : ScaleType::Linear;
			float minFrequency = _minFrequency ? _minFrequency.GetSingle() : 0.0f;

			if (newValue <= minFrequency)
			{
				throw hresult_invalid_argument();
			}
		}
		if (IsValueReferenceEqual(value, _maxFrequency))
			return;

		_maxFrequency = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"MaxFrequency"));
	}

    Windows::Foundation::IReference<AudioVisualizer::ScaleType> SourceConverter::FrequencyScale()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		return _frequencyScale;
	}

    void SourceConverter::FrequencyScale(Windows::Foundation::IReference<AudioVisualizer::ScaleType> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);

		if (value)
		{
			if (value.Value() == ScaleType::Logarithmic && _minFrequency)
			{
				if (_minFrequency.Value() == 0)
					throw hresult_invalid_argument();
			}
		}
		if (IsValueReferenceEqual(value, _frequencyScale))
			return;

		_frequencyScale = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"FrequencyScale"));

    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::RmsRiseTime()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    void SourceConverter::RmsRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::RmsFallTime()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    void SourceConverter::RmsFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PeakRiseTime()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    void SourceConverter::PeakRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PeakFallTime()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    void SourceConverter::PeakFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::SpectrumRiseTime()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    void SourceConverter::SpectrumRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::SpectrumFallTime()
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    void SourceConverter::SpectrumFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
    {
		std::lock_guard<std::mutex> lock(_lockMutex);
		throw hresult_not_implemented();
    }

    AudioVisualizer::VisualizationDataFrame SourceConverter::GetData()
    {
        throw hresult_not_implemented();
    }

    bool SourceConverter::IsSuspended()
    {
        throw hresult_not_implemented();
    }

    void SourceConverter::IsSuspended(bool value)
    {
        throw hresult_not_implemented();
    }

    float SourceConverter::Fps()
    {
        throw hresult_not_implemented();
    }

    void SourceConverter::Fps(float value)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::AnalyzerType SourceConverter::AnalyzerTypes()
    {
        throw hresult_not_implemented();
    }

    void SourceConverter::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PresentationTime()
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::SourcePlaybackState SourceConverter::PlaybackState()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<uint32_t> SourceConverter::ActualFrequencyCount()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<uint32_t> SourceConverter::ActualChannelCount()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<float> SourceConverter::ActualMinFrequency()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<float> SourceConverter::ActualMaxFrequency()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IReference<AudioVisualizer::ScaleType> SourceConverter::ActualFrequencyScale()
    {
        throw hresult_not_implemented();
    }

    event_token SourceConverter::ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler)
    {
		return _configurationChangedEvent.add(handler);
    }

    void SourceConverter::ConfigurationChanged(event_token const& token)
    { 
		_configurationChangedEvent.remove(token);
    }
}
