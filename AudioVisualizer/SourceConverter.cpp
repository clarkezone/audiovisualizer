#include "pch.h"
#include "SourceConverter.h"
#include "ScalarData.h"
#include "SpectrumData.h"
#include "VisualizationDataFrame.h"

namespace winrt::AudioVisualizer::implementation
{
	template<typename T>
	bool IsValueReferenceEqual(Windows::Foundation::IReference<T> const &value1, Windows::Foundation::IReference<T> const &value2)
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

	template<typename T>
	bool IsObjectEqual(T const &value1, T const &value2)
	{
		if (!value1 && !value2) // Both values are nullptr, no change
		{
			return true;
		}
		else if (value1 && value2 && winrt::get_abi(value1) == winrt::get_abi(value2))	// No change in value, return
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
		std::lock_guard lock(_lock);
		return _source;
	}

	void SourceConverter::Source(AudioVisualizer::IVisualizationSource const& value)
	{
		std::lock_guard lock(_lock);
		if (value == _source) {
			return;	// Both values nullptr, no change
		}

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
		std::lock_guard lock(_lock);
		return _frequencyCount;
	}

	void SourceConverter::FrequencyCount(Windows::Foundation::IReference<uint32_t> const& value)
	{
		std::lock_guard lock(_lock);
		if (value && value.GetUInt32() == 0) {
			throw hresult_invalid_argument();
		}
		if (value == _frequencyCount)
			return;

		_frequencyCount = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"FrequencyCount"));
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ChannelCount()
	{
		std::lock_guard lock(_lock);
		return _channelCount;
	}

	void SourceConverter::ChannelCount(Windows::Foundation::IReference<uint32_t> const& value)
	{
		std::lock_guard lock(_lock);

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
		std::lock_guard lock(_lock);
		return _minFrequency;
	}

	void SourceConverter::MinFrequency(Windows::Foundation::IReference<float> const& value)
	{
		std::lock_guard lock(_lock);

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
		std::lock_guard lock(_lock);

		return _maxFrequency;
	}

	void SourceConverter::MaxFrequency(Windows::Foundation::IReference<float> const& value)
	{
		std::lock_guard lock(_lock);

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
		std::lock_guard lock(_lock);
		return _frequencyScale;
	}

	void SourceConverter::FrequencyScale(Windows::Foundation::IReference<AudioVisualizer::ScaleType> const& value)
	{
		std::lock_guard lock(_lock);
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
		std::lock_guard lock(_lock);
		return _rmsRiseTime;
	}

	void SourceConverter::RmsRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::lock_guard lock(_lock);
		if (IsValueReferenceEqual(value, _rmsRiseTime))
			return;

		_rmsRiseTime = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"RmsRiseTime"));

	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::RmsFallTime()
	{
		std::lock_guard lock(_lock);
		return _rmsFallTime;
	}

	void SourceConverter::RmsFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::lock_guard lock(_lock);
		if (IsValueReferenceEqual(value, _rmsFallTime))
			return;

		_rmsFallTime = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"RmsFallTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PeakRiseTime()
	{
		std::lock_guard lock(_lock);
		return _peakRiseTime;
	}

	void SourceConverter::PeakRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::lock_guard lock(_lock);
		if (IsValueReferenceEqual(value, _peakRiseTime))
			return;

		_peakRiseTime = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"PeakRiseTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PeakFallTime()
	{
		std::lock_guard lock(_lock);;
		return _peakFallTime;
	}

	void SourceConverter::PeakFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::lock_guard lock(_lock);
		if (IsValueReferenceEqual(value, _peakFallTime))
			return;

		_peakFallTime = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"PeakFallTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::SpectrumRiseTime()
	{
		std::lock_guard lock(_lock);
		return _spectrumRiseTime;
	}

	void SourceConverter::SpectrumRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::lock_guard lock(_lock);
		if (IsValueReferenceEqual(value, _spectrumRiseTime))
			return;

		_spectrumRiseTime = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"SpectrumRiseTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::SpectrumFallTime()
	{
		std::lock_guard lock(_lock);
		return _spectrumFallTime;
	}

	void SourceConverter::SpectrumFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::lock_guard lock(_lock);
		if (IsValueReferenceEqual(value, _spectrumFallTime))
			return;

		_spectrumFallTime = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, hstring(L"SpectrumFallTime"));
	}

	bool SourceConverter::IsSuspended()
	{
		std::lock_guard lock(_lock);
		if (!_source)
		{
			throw hresult_error(E_NOT_VALID_STATE, L"Source not set");
		}
		return _source.IsSuspended();
	}

	void SourceConverter::IsSuspended(bool value)
	{
		throw hresult_not_implemented();
	}

	float SourceConverter::Fps()
	{
		std::lock_guard lock(_lock);
		if (!_source)
		{
			throw hresult_error(E_NOT_VALID_STATE, L"Source not set");
		}
		return _source.Fps();
	}

	void SourceConverter::Fps(float value)
	{
		throw hresult_not_implemented();
	}

	AudioVisualizer::AnalyzerType SourceConverter::AnalyzerTypes()
	{
		std::lock_guard lock(_lock);
		return _analyzerTypes;
	}

	void SourceConverter::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
	{
		std::lock_guard lock(_lock);
		if (_analyzerTypes == value) {
			return;	// No change
		}
		_analyzerTypes = value;
		_cachedOutputFrame = nullptr;
		_configurationChangedEvent(*this, L"AnalyzerTypes");
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PresentationTime()
	{
		std::lock_guard lock(_lock);
		if (!_source)
		{
			throw hresult_error(E_NOT_VALID_STATE, L"Source not set");
		}
		return _source.PresentationTime();
	}

	AudioVisualizer::SourcePlaybackState SourceConverter::PlaybackState()
	{
		std::lock_guard lock(_lock);
		if (!_source)
		{
			throw hresult_error(E_NOT_VALID_STATE, L"Source not set");
		}
		return _source.PlaybackState();
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ActualFrequencyCount()
	{
		std::lock_guard lock(_lock);
		return _actualFrequencyCount();
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ActualChannelCount()
	{
		std::lock_guard lock(_lock);
		return _actualChannelCount();
	}

	Windows::Foundation::IReference<float> SourceConverter::ActualMinFrequency()
	{
		std::lock_guard lock(_lock);
		return _actualMinFrequency();
	}

	Windows::Foundation::IReference<float> SourceConverter::ActualMaxFrequency()
	{
		std::lock_guard lock(_lock);
		return _actualMaxFrequency();
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> SourceConverter::ActualFrequencyScale()
	{
		std::lock_guard lock(_lock);
		return _actualFrequencyScale();
	}

	event_token SourceConverter::ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler)
	{
		return _configurationChangedEvent.add(handler);
	}

	void SourceConverter::ConfigurationChanged(event_token const& token)
	{
		_configurationChangedEvent.remove(token);
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::_actualFrequencyCount()
	{
		if (_frequencyCount) {
			return _frequencyCount;
		}
		else if (_source) {
			return _source.ActualFrequencyCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::_actualChannelCount()
	{
		if (_channelCount) {
			return _channelCount;
		}
		else if (_source) {
			return _source.ActualChannelCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<float> SourceConverter::_actualMinFrequency()
	{
		if (_minFrequency) {
			return _minFrequency;
		}
		else if (_source) {
			return _source.ActualMinFrequency();
		}
		return nullptr;

	}

	Windows::Foundation::IReference<float> SourceConverter::_actualMaxFrequency()
	{
		if (_maxFrequency) {
			return _maxFrequency;
		}
		else if (_source) {
			return _source.ActualMaxFrequency();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> SourceConverter::_actualFrequencyScale()
	{
		if (_frequencyScale) {
			return _frequencyScale;
		}
		else if (_source) {
			return _source.ActualFrequencyScale();
		}
		return nullptr;

	}
	/*
	This method proxies a call to data source and the logic works as follows
	1. If source is nullptr return nullptr
	2. Obtain frame from source.
	3.a If frame is not null
		3.a.1 if the frame matches cached frame, caching is allowed then return cached output frame
		3.a.2 Process the frame
	3.b If frame is null
		3.b.1 If previous frame exists then do processing as if input was empty
		3.b.2 If previous frame exists try to construct empty frame and return it

	*/
	AudioVisualizer::VisualizationDataFrame SourceConverter::GetData()
	{
		std::lock_guard lock(_lock);
		if (!_source)
		{
			return nullptr;
		}

		auto sourceFrame = _source.GetData();

		if (_analyzerTypes == AnalyzerType::All &&	// No analyzer changes
			!_minFrequency && !_maxFrequency && !_frequencyCount && !_frequencyScale &&	// No frequency changes
			!_channelCount &&
			!_rmsRiseTime && !_rmsFallTime && !_peakRiseTime && !_peakFallTime && !_spectrumRiseTime && !_spectrumFallTime)	// No rise and fall changes
		{
			return sourceFrame;	// Just return source frame - passthrough
		}

		// Test if the object we got is the same we keep as cached source
		if (sourceFrame == _cachedSourceFrame && _bCacheData && _cachedOutputFrame)
		{
			// As it is the same input bypass calculations and return last calculated output frame
			return _cachedOutputFrame;
		}

		if (!sourceFrame)
		{
			if (_cachedSourceFrame)
				sourceFrame = CloneFrame(_cachedSourceFrame);	// If cached source exists try to clone an empty frame and use it
			else
				return nullptr;	// Did not work
		}
		auto result = ProcessFrame(sourceFrame);
		_cachedOutputFrame = result;
		return result;
	}

	AudioVisualizer::VisualizationDataFrame SourceConverter::ProcessFrame(AudioVisualizer::VisualizationDataFrame const & source)
	{
		AudioVisualizer::ScalarData rms{ nullptr };
		AudioVisualizer::ScalarData peak{ nullptr };
		AudioVisualizer::SpectrumData spectrum{ nullptr };

		if (((uint32_t)_analyzerTypes & (uint32_t)AnalyzerType::Spectrum) && source.Spectrum())
		{
			spectrum = ApplyFrequencyTransforms(source.Spectrum());
			spectrum = ApplyRiseAndFall(spectrum, _previousSpectrum, _spectrumRiseTime, _spectrumFallTime);
			_previousSpectrum = spectrum;
		}
		if (((uint32_t) _analyzerTypes & (uint32_t)AnalyzerType::RMS) && source.RMS())
		{
			rms = ApplyRiseAndFall(source.RMS(), _previousRMS, _rmsRiseTime, _rmsFallTime);
			_previousRMS = rms;
		}
		if (((uint32_t) _analyzerTypes & (uint32_t)AnalyzerType::Peak) && source.Peak())
		{
			peak = ApplyRiseAndFall(source.Peak(), _previousPeak, _peakRiseTime, _peakFallTime);
			_previousPeak = peak;
		}		
		return make<VisualizationDataFrame>(source.Time(), source.Duration(), rms, peak, spectrum);
	}

	AudioVisualizer::VisualizationDataFrame SourceConverter::CloneFrame(AudioVisualizer::VisualizationDataFrame const &frameToClone)
	{
		// Clone empty frame from output frame
		AudioVisualizer::ScalarData rms{ nullptr };
		if (frameToClone.RMS())
		{
			auto cachedRms = frameToClone.RMS();
			rms = make<ScalarData>(cachedRms.Size(), cachedRms.AmplitudeScale());
		}
		AudioVisualizer::ScalarData peak{ nullptr };
		if (frameToClone.Peak())
		{
			auto cachedPeak = frameToClone.Peak();
			peak = make<ScalarData>(cachedPeak.Size(), cachedPeak.AmplitudeScale());
		}
		AudioVisualizer::SpectrumData spectrum{ nullptr };
		if (frameToClone.Spectrum())
		{
			auto cachedSpectrum = frameToClone.Spectrum();
			spectrum = make<SpectrumData>(cachedSpectrum.Size(), cachedSpectrum.FrequencyCount(), cachedSpectrum.AmplitudeScale(), cachedSpectrum.FrequencyScale(), cachedSpectrum.MinFrequency(), cachedSpectrum.MaxFrequency());
		}
		return  make<VisualizationDataFrame>(frameToClone.Time(), frameToClone.Duration(), rms, peak, spectrum);
	}

	AudioVisualizer::SpectrumData SourceConverter::ApplyFrequencyTransforms(AudioVisualizer::SpectrumData data)
	{
		// No conversion
		if (!_frequencyScale && !_minFrequency && !_maxFrequency && !_frequencyCount)
		{
			return data;
		}

		UINT32 frequencyCount = _actualFrequencyCount() ? _actualFrequencyCount().Value() : data.Size();
		float minFrequency = _actualMinFrequency() ? _actualMinFrequency().Value() : data.MinFrequency();
		float maxFrequency = _actualMaxFrequency() ? _actualMaxFrequency().Value() : data.MaxFrequency();

		ScaleType fScale = _actualFrequencyScale() ? _actualFrequencyScale().Value() : data.FrequencyScale();

		if (fScale == ScaleType::Linear && data.FrequencyScale() == ScaleType::Linear)
		{
			return data.LinearTransform(frequencyCount, minFrequency, maxFrequency);
		}
		else if (fScale == ScaleType::Logarithmic && data.FrequencyScale() == ScaleType::Linear)
		{
			return data.LogarithmicTransform(frequencyCount, minFrequency, maxFrequency);
		}
		else
		{
			throw hresult_not_implemented(L"Logarithmic to log scale transforms not implemented");
		}
	}

	AudioVisualizer::SpectrumData SourceConverter::ApplyRiseAndFall(AudioVisualizer::SpectrumData data, AudioVisualizer::SpectrumData previous, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> riseTime, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> fallTime)
	{
		if (!riseTime && !fallTime)
		{
			return data;
		}

		Windows::Foundation::TimeSpan actualRiseTime = (riseTime) ? riseTime.Value() : Windows::Foundation::TimeSpan::zero();
		Windows::Foundation::TimeSpan actualFallTime = (fallTime) ? fallTime.Value() : Windows::Foundation::TimeSpan::zero();

		if (data)
		{
			return data.ApplyRiseAndFall(previous, actualRiseTime, actualFallTime, _timeFromPrevious);
		}
		else if (previous)
		{
			SpectrumData::ApplyRiseAndFallToEmpty(previous, actualRiseTime, actualFallTime, _timeFromPrevious);
		}
		return nullptr;	// Shoudl never get here
	}

	AudioVisualizer::ScalarData SourceConverter::ApplyRiseAndFall(AudioVisualizer::ScalarData data, AudioVisualizer::ScalarData previous, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> riseTime, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> fallTime)
	{
		if (!riseTime && !fallTime)
		{
			return data;
		}

		Windows::Foundation::TimeSpan actualRiseTime = (riseTime) ? riseTime.Value() : Windows::Foundation::TimeSpan::zero();
		Windows::Foundation::TimeSpan actualFallTime = (fallTime) ? fallTime.Value() : Windows::Foundation::TimeSpan::zero();

		if (data)
		{
			return data.ApplyRiseAndFall(previous, actualRiseTime, actualFallTime, _timeFromPrevious);
		}
		else if (previous)
		{
			ScalarData::ApplyRiseAndFallToEmpty(previous, actualRiseTime, actualFallTime, _timeFromPrevious);
		}
		return nullptr;	// Shoudl never get here
	}

}
