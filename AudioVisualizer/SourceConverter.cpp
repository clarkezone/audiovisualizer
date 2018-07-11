#include "pch.h"
#include "SourceConverter.h"
#include "ScalarData.h"
#include "SpectrumData.h"
#include "VisualizationDataFrame.h"
#include "Tracing.h"


using namespace util;


namespace winrt::AudioVisualizer::implementation
{
	SourceConverter::SourceConverter()
	{
		_analyzerTypes = AnalyzerType::All;
		_bCacheData = true;
		_cachedSourceFrameTime = Windows::Foundation::TimeSpan(-1000000000);	// Make it large negative value
		_sourceGetTime = Windows::Foundation::TimeSpan(-100000000);
		_timeFromPrevious = Windows::Foundation::TimeSpan(166667);	// Default to 1/60sec
	}

	AudioVisualizer::IVisualizationSource SourceConverter::Source()
	{
		std::shared_lock lock(_lock);
		return _source;
	}

	void SourceConverter::Source(AudioVisualizer::IVisualizationSource const& value)
	{
		std::unique_lock lock(_lock);
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
				// Source configuration changed
				std::unique_lock lock(_lock);
#ifdef _TRACE_
				Trace::SourceConverter_SourceConfigurationChanged(sender,propertyName,*this);
#endif
				if (propertyName == L"ChannelCount") {
					ConfigureChannelMap();
				}
				CreateEmptyFrameFromSource();
			})
			);
		}
		_cachedOutputFrame = nullptr;
		CreateEmptyFrameFromSource();
		ConfigureChannelMap();
		
		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"Source"));
	}
	
	void SourceConverter::CreateEmptyFrameFromSource()
	{
		_emptySourceFrame = nullptr;
		if (!_source || !_source.ActualChannelCount() || !_source.ActualFrequencyCount())
			return;
		AudioVisualizer::ScalarData rms{ nullptr };
		if (enum_has_flag(_source.AnalyzerTypes(), AnalyzerType::RMS) && _source.ActualChannelCount()) {
			rms = ScalarData::CreateEmpty(_source.ActualChannelCount().Value());
		}
		AudioVisualizer::ScalarData peak{ nullptr };
		if (enum_has_flag(_source.AnalyzerTypes(), AnalyzerType::Peak) && _source.ActualChannelCount()) {
			peak = ScalarData::CreateEmpty(_source.ActualChannelCount().Value());
		}
		AudioVisualizer::SpectrumData spectrum{ nullptr };
		if (enum_has_flag(_source.AnalyzerTypes(), AnalyzerType::Spectrum) && _source.ActualChannelCount() && _source.ActualFrequencyCount() && _source.ActualFrequencyScale() && _source.ActualMaxFrequency() && _source.ActualMinFrequency())
		{
			spectrum = SpectrumData::CreateEmpty(
										_source.ActualChannelCount().Value(), 
										_source.ActualFrequencyCount().Value(),
										ScaleType::Linear, 
										_source.ActualFrequencyScale().Value(), 
										_source.ActualMinFrequency().Value(),
										_source.ActualMaxFrequency().Value()
					);
		}
		
		_emptySourceFrame = AudioVisualizer::VisualizationDataFrame(
			nullptr,
			Windows::Foundation::TimeSpan((REFERENCE_TIME)(1e7 / _source.Fps())),
			rms,
			peak,
			spectrum);
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::FrequencyCount()
	{
		std::shared_lock lock(_lock);
		return _frequencyCount;
	}

	void SourceConverter::FrequencyCount(Windows::Foundation::IReference<uint32_t> const& value)
	{
		std::unique_lock lock(_lock);
	
		if (value && value.GetUInt32() == 0) {
			throw hresult_invalid_argument();
		}

		if (value == _frequencyCount)
			return;

		_frequencyCount = value;
		_cachedOutputFrame = nullptr;
		
		lock.unlock();	// Unlock before calling changed event
		_configurationChangedEvent(*this, hstring(L"FrequencyCount"));
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ChannelCount()
	{
		std::shared_lock lock(_lock);
		return _channelCount;
	}

	/*
		Setting channel count will guarantee a set number of channels for output data irrespective of input channel count
		If ChannelMapping is not set then channels are mapped as following
		if channelCount is 1 then all input channels are added up
		if channelCount is 2 then channels are mapped as following
		ChannelCount	InputChannels
		2				1					out0 = in0, out1 = in0
		2				3					out0 = in0, out1 = in1
		2				4					out0 = in0+in2, out1 = in1+in3
		2				5					out0 = in0+in3, out1 = in1+in4
		2				6					out0 = in0+in4, out1 = in1+in5
		otherwise respective inputs copied to output, missing outputs are clipped and missing inputs are of value 0
	*/
	void SourceConverter::ChannelCount(Windows::Foundation::IReference<uint32_t> const& value)
	{
		std::unique_lock lock(_lock);

		if (value && value.GetUInt32() == 0) {
			throw hresult_invalid_argument();
		}
		if (value == _channelCount)
			return;

		_channelCount = value;
		_cachedOutputFrame = nullptr;
		ConfigureChannelMap();
		lock.unlock();

		_configurationChangedEvent(*this, hstring(L"ChannelCount"));
		
	}

	com_array<float> SourceConverter::ChannelMapping()
	{
		std::shared_lock lock(_lock);
		return !_channelMap.empty() ? com_array<float>(_channelMap) : com_array<float>();
	}

	void SourceConverter::ChannelMapping(array_view<float const> value)
	{
		std::unique_lock lock(_lock);

		if (value.data() != nullptr)
		{
			if (!_channelCount) {
				throw hresult_error(E_NOT_VALID_STATE, L"Need to set ChannelCount first");
			}
			if (value.size() < _channelCount.Value()) {
				throw hresult_invalid_argument(L"Map has to have at least ChannelCount elements");
			}

			if (std::equal(_channelMap.cbegin(), _channelMap.cend(), value.cbegin(), value.cend()))
				return;

			_channelMap = std::vector<float>(value.cbegin(), value.cend());
		}
		else {
			if (_channelMap.empty())
				return;
			_channelMap.clear();
		}

		ConfigureChannelMap();

		_cachedOutputFrame = nullptr;
		lock.unlock();

		_configurationChangedEvent(*this, hstring(L"ChannelMap"));
	}

	void SourceConverter::ConfigureChannelMap()
	{
		_channelCombineMap.clear();
		if (!_channelCount)
			return;

		if (_source && _source.ActualChannelCount()) {
			// Build the map if source is set and channel count is known
			auto inputChannels = _source.ActualChannelCount().Value();
			auto outputChannels = _channelCount.Value();


			if (_channelMap.empty()) {
				if (inputChannels == outputChannels)
					return;	// No conversion

				// Default mapping
				switch (outputChannels)
				{
				case 1:
					_channelCombineMap.resize(inputChannels);
					std::fill(_channelCombineMap.begin(), _channelCombineMap.end(), 1.0f);	// Copy input to all outputs
					break;
				case 2:
					switch (inputChannels)
					{
					case 1:	// Mono to stereo
						_channelCombineMap = { 1.0f,1.0f };
						break;
					case 3:	// 2.1 to stereo
						_channelCombineMap = {	1.0f,0.0f,0.0f, 
												0.0f,1.0f,0.0f };
						break;
					case 4: // Quadro to stereo
						_channelCombineMap = { 1.0f,0.0f,1.0f,0.0f,
												0.0f,1.0f,0.0f,1.0f };
						break;
					case 5: // Surround to stereo
						_channelCombineMap = {	1.0f,0.0f,0.0f,1.0f,0.0f,
												0.0f,1.0f,0.0f,0.0f,1.0f };
						break;
					case 6: // 5.1 Surround to stereo
						_channelCombineMap = {	1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,
												0.0f,1.0f,0.0f,0.0f,0.0f,1.0f };
						break;
					default:
						CreateDefautChannelMap(inputChannels, outputChannels);
						break;
					}
					break;
				default:
					CreateDefautChannelMap(inputChannels, outputChannels);
					break;
				}
			}
			else {
				// Custom mapping
				CreateDefautChannelMap(inputChannels, outputChannels);
			}
		}
	}

	void SourceConverter::CreateDefautChannelMap(const uint32_t inputChannels, const uint32_t outputChannels)
	{
		_channelCombineMap.resize(inputChannels * outputChannels);
		size_t mapIndex = 0;
		for (size_t outputIndex = 0; outputIndex < outputChannels; outputIndex++)
		{
			for (size_t inputIndex = 0; inputIndex < inputChannels; inputIndex++, mapIndex++)
			{
				_channelCombineMap[mapIndex] = _channelMap.empty() ?
					(inputIndex == outputIndex ? 1.0f : 0.0f) : // No channel map
					mapIndex < _channelMap.size() ? _channelMap[mapIndex] : 0.0f; // Copy channel map value
			}
		}
	}

	Windows::Foundation::IReference<float> SourceConverter::MinFrequency()
	{
		std::shared_lock lock(_lock);
		return _minFrequency;
	}

	void SourceConverter::MinFrequency(Windows::Foundation::IReference<float> const& value)
	{
		std::unique_lock lock(_lock);

		if (value) {
			float newValue = value.GetSingle();
			ScaleType fScale = _frequencyScale ? _frequencyScale.Value() : ScaleType::Linear;
			float maxFrequency = _maxFrequency ? _maxFrequency.GetSingle() : std::numeric_limits<float>::max();

			if ((fScale == ScaleType::Logarithmic && newValue == 0) || newValue < 0 || newValue >= maxFrequency)
			{
				throw hresult_invalid_argument();
			}
		}
		if (value == _minFrequency)
			return;
		_minFrequency = value;
		_cachedOutputFrame = nullptr;
		lock.unlock();

		_configurationChangedEvent(*this, hstring(L"MinFrequency"));
	}

	Windows::Foundation::IReference<float> SourceConverter::MaxFrequency()
	{
		std::shared_lock lock(_lock);

		return _maxFrequency;
	}

	void SourceConverter::MaxFrequency(Windows::Foundation::IReference<float> const& value)
	{
		std::unique_lock lock(_lock);

		if (value) {
			float newValue = value.GetSingle();
			float minFrequency = _minFrequency ? _minFrequency.GetSingle() : 0.0f;
			if (newValue <= minFrequency)
			{
				throw hresult_invalid_argument();
			}
		}
		if (value == _maxFrequency)
			return;

		_maxFrequency = value;
		_cachedOutputFrame = nullptr;

		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"MaxFrequency"));
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> SourceConverter::FrequencyScale()
	{
		std::shared_lock lock(_lock);
		return _frequencyScale;
	}

	void SourceConverter::FrequencyScale(Windows::Foundation::IReference<AudioVisualizer::ScaleType> const& value)
	{
		std::unique_lock lock(_lock);
		if (value)
		{
			if (value.Value() == ScaleType::Logarithmic)
			{
				if (_minFrequency) {
					if (_minFrequency.Value() == 0) {
						throw hresult_invalid_argument(L"Cannot set to logarithmic scale with minimum frequency set to zero");
					}
				}
				else {
					throw hresult_invalid_argument(L"Cannot set to logarithmic scale with minimum frequency not set");
				}
			}
		}
		if (value == _frequencyScale)
			return;

		_frequencyScale = value;
		_cachedOutputFrame = nullptr;

		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"FrequencyScale"));

	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::RmsRiseTime()
	{
		std::shared_lock lock(_lock);
		return _rmsRiseTime;
	}

	void SourceConverter::RmsRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::unique_lock lock(_lock);
		if (value == _rmsRiseTime)
			return;

		_rmsRiseTime = value;
		_cachedOutputFrame = nullptr;
		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"RmsRiseTime"));

	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::RmsFallTime()
	{
		std::shared_lock lock(_lock);
		return _rmsFallTime;
	}

	void SourceConverter::RmsFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::unique_lock lock(_lock);
		if (value == _rmsFallTime)
			return;

		_rmsFallTime = value;
		_cachedOutputFrame = nullptr;
		
		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"RmsFallTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PeakRiseTime()
	{
		std::shared_lock lock(_lock);
		return _peakRiseTime;
	}

	void SourceConverter::PeakRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::unique_lock lock(_lock);
		if (value == _peakRiseTime)
			return;

		_peakRiseTime = value;
		_cachedOutputFrame = nullptr;

		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"PeakRiseTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PeakFallTime()
	{
		std::shared_lock lock(_lock);;
		return _peakFallTime;
	}

	void SourceConverter::PeakFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::unique_lock lock(_lock);
		if (value == _peakFallTime)
			return;

		_peakFallTime = value;
		_cachedOutputFrame = nullptr;

		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"PeakFallTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::SpectrumRiseTime()
	{
		std::shared_lock lock(_lock);
		return _spectrumRiseTime;
	}

	void SourceConverter::SpectrumRiseTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::unique_lock lock(_lock);
		if (value == _spectrumRiseTime)
			return;

		_spectrumRiseTime = value;
		_cachedOutputFrame = nullptr;
		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"SpectrumRiseTime"));
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::SpectrumFallTime()
	{
		std::shared_lock lock(_lock);
		return _spectrumFallTime;
	}

	void SourceConverter::SpectrumFallTime(Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& value)
	{
		std::unique_lock lock(_lock);
		if (value == _spectrumFallTime)
			return;

		_spectrumFallTime = value;
		_cachedOutputFrame = nullptr;

		lock.unlock();
		_configurationChangedEvent(*this, hstring(L"SpectrumFallTime"));
	}

	bool SourceConverter::IsSuspended()
	{
		std::shared_lock lock(_lock);
		if (!_source)
		{
			throw hresult_error(E_NOT_VALID_STATE, L"Source not set");
		}
		return _source.IsSuspended();
	}

	void SourceConverter::IsSuspended(bool /*value*/)
	{
		throw hresult_not_implemented();
	}

	float SourceConverter::Fps()
	{
		std::shared_lock lock(_lock);
		if (!_source)
		{
			throw hresult_error(E_NOT_VALID_STATE, L"Source not set");
		}
		return _source.Fps();
	}

	void SourceConverter::Fps(float /*value*/)
	{
		throw hresult_not_implemented();
	}

	AudioVisualizer::AnalyzerType SourceConverter::AnalyzerTypes()
	{
		std::shared_lock lock(_lock);
		return _analyzerTypes;
	}

	void SourceConverter::AnalyzerTypes(AudioVisualizer::AnalyzerType const& value)
	{
		std::unique_lock lock(_lock);
		if (_analyzerTypes == value) {
			return;	// No change
		}
		_analyzerTypes = value;
		_cachedOutputFrame = nullptr;

		lock.unlock();
		_configurationChangedEvent(*this, L"AnalyzerTypes");
	}

	Windows::Foundation::IReference<Windows::Foundation::TimeSpan> SourceConverter::PresentationTime()
	{
		std::shared_lock lock(_lock);
		if (!_source)
		{
			return nullptr;
		}
		return _source.PresentationTime();
	}

	AudioVisualizer::SourcePlaybackState SourceConverter::PlaybackState()
	{
		std::shared_lock lock(_lock);
		if (!_source)
		{
			return SourcePlaybackState::Stopped;
		}
		return _source.PlaybackState();
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ActualFrequencyCount()
	{
		std::shared_lock lock(_lock);
		return ActualFrequencyCountImpl();
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ActualChannelCount()
	{
		std::shared_lock lock(_lock);
		return ActualChannelCountImpl();
	}

	Windows::Foundation::IReference<float> SourceConverter::ActualMinFrequency()
	{
		std::shared_lock lock(_lock);
		return ActualMinFrequencyImpl();
	}

	Windows::Foundation::IReference<float> SourceConverter::ActualMaxFrequency()
	{
		std::shared_lock lock(_lock);
		return ActualMaxFrequencyImpl();
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> SourceConverter::ActualFrequencyScale()
	{
		std::shared_lock lock(_lock);
		return ActualFrequencyScaleImpl();
	}

	event_token SourceConverter::ConfigurationChanged(Windows::Foundation::TypedEventHandler<AudioVisualizer::IVisualizationSource, hstring> const& handler)
	{
		return _configurationChangedEvent.add(handler);
	}

	void SourceConverter::ConfigurationChanged(event_token const& token)
	{
		_configurationChangedEvent.remove(token);
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ActualFrequencyCountImpl()
	{
		if (_frequencyCount) {
			return _frequencyCount;
		}
		else if (_source) {
			return _source.ActualFrequencyCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<uint32_t> SourceConverter::ActualChannelCountImpl()
	{
		if (_channelCount) {
			return _channelCount;
		}
		else if (_source) {
			return _source.ActualChannelCount();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<float> SourceConverter::ActualMinFrequencyImpl()
	{
		if (_minFrequency) {
			return _minFrequency;
		}
		else if (_source) {
			return _source.ActualMinFrequency();
		}
		return nullptr;

	}

	Windows::Foundation::IReference<float> SourceConverter::ActualMaxFrequencyImpl()
	{
		if (_maxFrequency) {
			return _maxFrequency;
		}
		else if (_source) {
			return _source.ActualMaxFrequency();
		}
		return nullptr;
	}

	Windows::Foundation::IReference<AudioVisualizer::ScaleType> SourceConverter::ActualFrequencyScaleImpl()
	{
		if (_frequencyScale) {
			return _frequencyScale;
		}
		else if (_source) {
			return _source.ActualFrequencyScale();
		}
		return nullptr;

	}

	bool SourceConverter::CacheData()
	{
		return _bCacheData;
	}

	void SourceConverter::CacheData(bool value)
	{
		_bCacheData = value;
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

	const Windows::Foundation::TimeSpan getNewDataThreshold = Windows::Foundation::TimeSpan(100000);

	AudioVisualizer::VisualizationDataFrame SourceConverter::GetData()
	{
		std::unique_lock lock(_lock);
		if (!_source)
		{
			return nullptr;
		}

		auto sinceLastGet = _sourceTimer.time() - _sourceGetTime;
		// If there is cached source which was aquired less than 10ms ago just return cached output
		if (_bCacheData && sinceLastGet < getNewDataThreshold) 
		{
			return _cachedOutputFrame;
		}

		auto sourceFrame = _source.GetData();
		_sourceGetTime = _sourceTimer.time();
		auto sourceFrameTimeStamp = _sourceTimer.time();

		if ( sourceFrame &&
			_analyzerTypes == AnalyzerType::All &&	// No analyzer changes
			!_minFrequency && !_maxFrequency && !_frequencyCount && !_frequencyScale &&	// No frequency changes
			!_channelCount &&
			!_rmsRiseTime && !_rmsFallTime && !_peakRiseTime && !_peakFallTime && !_spectrumRiseTime && !_spectrumFallTime)	// No rise and fall changes
		{
			_cachedOutputFrame = sourceFrame;
			return sourceFrame;	// Just return source frame - passthrough
		}

		if (!sourceFrame)
		{
			if (_emptySourceFrame)
				sourceFrame = _emptySourceFrame;
			else
				return nullptr;
		}
		//_timeFromPrevious = sinceLastGet;
		_cachedSourceFrameTime = sourceFrameTimeStamp;
		auto result = ProcessFrame(sourceFrame);
		_cachedOutputFrame = result;
		return result;
	}

	AudioVisualizer::VisualizationDataFrame SourceConverter::ProcessFrame(AudioVisualizer::VisualizationDataFrame const & source)
	{
		AudioVisualizer::ScalarData rms = enum_has_flag(_analyzerTypes, AnalyzerType::RMS) && source.RMS() ? source.RMS() : nullptr;
		AudioVisualizer::ScalarData peak = enum_has_flag(_analyzerTypes, AnalyzerType::Peak) && source.Peak() ? source.Peak() :  nullptr ;
		AudioVisualizer::SpectrumData spectrum = enum_has_flag(_analyzerTypes, AnalyzerType::Spectrum) && source.Spectrum() ? source.Spectrum() : nullptr;

		if (spectrum)
		{
			if (!_channelCombineMap.empty()) {
				spectrum = spectrum.CombineChannels(_channelCombineMap);
			}

			spectrum = ApplyFrequencyTransforms(spectrum);
			spectrum = ApplyRiseAndFall(spectrum, _previousSpectrum, _spectrumRiseTime, _spectrumFallTime);
			_previousSpectrum = spectrum;
		}
		if (rms)
		{
			if (!_channelCombineMap.empty()) {
				rms = rms.CombineChannels(_channelCombineMap);
			}
			rms = ApplyRiseAndFall(rms, _previousRMS, _rmsRiseTime, _rmsFallTime);
			_previousRMS = rms;
		}
		if (peak)
		{
			if (!_channelCombineMap.empty()) {
				peak = source.Peak().CombineChannels(_channelCombineMap);
			}
			peak = ApplyRiseAndFall(peak, _previousPeak, _peakRiseTime, _peakFallTime);
			_previousPeak = peak;
		}		
		auto result = AudioVisualizer::VisualizationDataFrame(source.Time(), source.Duration(), rms, peak, spectrum);
		return result;
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

		auto actualFrequencyCount = ActualFrequencyCountImpl();
		auto actualMinFrequency = ActualMinFrequencyImpl();
		auto actualMaxFrequency = ActualMaxFrequencyImpl();
		auto actualFrequencyScale = ActualFrequencyScaleImpl();
		UINT32 frequencyCount = actualFrequencyCount ? actualFrequencyCount.Value() : data.Size();
		float minFrequency = actualMinFrequency ? actualMinFrequency.Value() : data.MinFrequency();
		float maxFrequency = actualMaxFrequency? actualMaxFrequency.Value() : data.MaxFrequency();

		ScaleType fScale = actualFrequencyScale ? actualFrequencyScale.Value() : data.FrequencyScale();

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
