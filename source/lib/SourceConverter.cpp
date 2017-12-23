#include "pch.h"
#include "SourceConverter.h"
#include "VisualizationDataFrame.h"
#include "ScalarData.h"
#include "SpectrumData.h"

#include <vector>

namespace AudioVisualizer
{
	/* Channel mappingss https://msdn.microsoft.com/en-us/library/windows/desktop/ee415748(v=vs.85).aspx */
	std::vector<float> mapFromStereoToMono = { 0.5f,0.5f };
	std::vector<float> mapFromMonoToStereo = { 0.5f, 0.5f };

	SourceConverter::SourceConverter()
	{
		_analyzerTypes = AnalyzerType::All;
		_bCacheData = true;
		_timeFromPrevious.Duration = 166667;
	}

	SourceConverter::~SourceConverter()
	{
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
	STDMETHODIMP SourceConverter::GetData(IVisualizationDataFrame **ppResult)
	{
		if (ppResult == nullptr)
			return E_POINTER;

		auto lock = _csLock.Lock();

		if (_source == nullptr)
		{
			*ppResult = nullptr;
			return S_OK;
		}

		ComPtr<IVisualizationDataFrame> sourceFrame;

		HRESULT hr = _source->GetData(&sourceFrame);
		if (FAILED(hr))
			return hr;
		if (sourceFrame.Get() == _cachedSourceFrame.Get() && _bCacheData && _cachedOutputFrame != nullptr)
		{
			// As it is the same input bypass calculations and return last calculated output frame
			return _cachedOutputFrame.CopyTo(ppResult);
		}

		if (sourceFrame == nullptr)
		{
			hr = TryConstructingSourceFrame(&sourceFrame);
			if (hr != S_OK)
			{
				*ppResult = nullptr;
				return S_OK;
			}
		}

		ComPtr<IVisualizationDataFrame> result;
		hr = ProcessFrame(sourceFrame.Get(), &result);
		if (FAILED(hr))
			return hr;
		_cachedOutputFrame = result;

		return result.CopyTo(ppResult);
	}

	STDMETHODIMP SourceConverter::get_ActualFrequencyCount(IReference<UINT32>** ppcElements)
	{
		auto lock = _csLock.Lock();
		ComPtr<IReference<UINT32>> result;
		this->get_FrequencyCount(&result);

		ComPtr<IVisualizationSource> source;
		get_Source(&source);

		while (source != nullptr && result == nullptr)
		{
			source->get_ActualFrequencyCount(&result);
		}
		return result.CopyTo(ppcElements);
	}

	STDMETHODIMP SourceConverter::get_ActualChannelCount(IReference<UINT32>** ppcElements)
	{
		auto lock = _csLock.Lock();
		ComPtr<IReference<UINT32>> result;
		this->get_ChannelCount(&result);

		ComPtr<IVisualizationSource> source;
		get_Source(&source);

		while (source != nullptr && result == nullptr)
		{
			source->get_ActualChannelCount(&result);
		}
		return result.CopyTo(ppcElements);
	}

	STDMETHODIMP SourceConverter::get_ActualMinFrequency(IReference<float>** ppValue)
	{
		auto lock = _csLock.Lock();
		ComPtr<IReference<float>> result;
		this->get_MinFrequency(&result);

		ComPtr<IVisualizationSource> source;
		get_Source(&source);

		while (source != nullptr && result == nullptr)
		{
			source->get_ActualMinFrequency(&result);
		}
		return result.CopyTo(ppValue);
	}

	STDMETHODIMP SourceConverter::get_ActualMaxFrequency(IReference<float>** ppValue)
	{
		auto lock = _csLock.Lock();
		ComPtr<IReference<float>> result;
		this->get_MaxFrequency(&result);

		ComPtr<IVisualizationSource> source;
		get_Source(&source);

		while (source != nullptr && result == nullptr)
		{
			source->get_ActualMaxFrequency(&result);
		}
		return result.CopyTo(ppValue);
	}

	STDMETHODIMP SourceConverter::get_ActualFrequencyScale(IReference<ScaleType>** ppValue)
	{
		auto lock = _csLock.Lock();

		ComPtr<IReference<ScaleType>> result;
		this->get_FrequencyScale(&result);

		ComPtr<IVisualizationSource> source;
		get_Source(&source);

		while (source != nullptr && result == nullptr)
		{
			source->get_ActualFrequencyScale(&result);
		}
		return result.CopyTo(ppValue);
	}

	HRESULT SourceConverter::ProcessFrame(IVisualizationDataFrame * pSource, IVisualizationDataFrame ** ppResult)
	{
		HRESULT hr = S_OK;
		TimeSpan frameTime;
		pSource->get_Time(&frameTime);
		TimeSpan frameDuration;
		pSource->get_Duration(&frameDuration);

		ComPtr<ISpectrumData> spectrum;
		pSource->get_Spectrum(&spectrum);
		ComPtr<IScalarData> rms;
		pSource->get_RMS(&rms);
		ComPtr<IScalarData> peak;
		pSource->get_Peak(&peak);

		if ((_analyzerTypes & AnalyzerType::Spectrum) == AnalyzerType::Spectrum && spectrum != nullptr)
		{
			ComPtr<ISpectrumData> fTransformed;
			hr = ApplyFrequencyTransforms(spectrum.Get(), &fTransformed);
			if (FAILED(hr))
				return hr;

			hr = ApplyRiseAndFall(fTransformed.Get(), _previousSpectrum.Get(),_spectrumRiseTime.Get(),_spectrumFallTime.Get(), &spectrum);
			if (FAILED(hr))
				return hr;
			spectrum.CopyTo(&_previousSpectrum);
		}
		if ((_analyzerTypes & AnalyzerType::RMS) == AnalyzerType::RMS && rms != nullptr)
		{
			ComPtr<IScalarData> resultRms;
			hr = ApplyRiseAndFall(rms.Get(), _previousRMS.Get(), _rmsRiseTime.Get(),_rmsFallTime.Get(), &resultRms);
			if (FAILED(hr))
				return hr;
			rms = resultRms;
			rms.CopyTo(&_previousRMS);
		}
		if ((_analyzerTypes & AnalyzerType::Peak) == AnalyzerType::Peak && peak != nullptr)
		{
			ComPtr<IScalarData> resultPeak;
			hr = ApplyRiseAndFall(peak.Get(), _previousPeak.Get(), _peakRiseTime.Get(),_peakFallTime.Get(), &resultPeak);
			if (FAILED(hr))
				return hr;
			peak = resultPeak;
			peak.CopyTo(&_previousPeak);
		}

		ComPtr<VisualizationDataFrame> resultFrame = Make<VisualizationDataFrame>(
			frameTime.Duration,
			frameDuration.Duration,
			rms.Get(),
			peak.Get(),
			spectrum.Get()
			);

		return resultFrame.CopyTo(ppResult);
	}

	// Creates an empty source frame based on input properties
	HRESULT SourceConverter::TryConstructingSourceFrame(IVisualizationDataFrame ** ppResult)
	{

		// Use cached output frame as template, otherwise look at source properties
		if (_cachedOutputFrame != nullptr)
		{
			return CloneFromFrame(_cachedOutputFrame.Get(), ppResult);
		}
		else
		{

		}
		return E_FAIL;
	}

	HRESULT SourceConverter::CloneSpectrum(ISpectrumData * pSource, ISpectrumData **ppResult)
	{
		if (pSource == nullptr)
		{
			*ppResult = nullptr;
			return S_OK;
		}
		ComPtr<IVectorView<IVectorView<float>*>> vv;
		HRESULT hr = pSource->QueryInterface<IVectorView<IVectorView<float>*>>(&vv);
		if (FAILED(hr))
			return hr;
		UINT32 channels = 0;
		vv->get_Size(&channels);
		UINT32 elements = 0;
		pSource->get_FrequencyCount(&elements);
		ScaleType ampScale, fScale;
		pSource->get_AmplitudeScale(&ampScale);
		pSource->get_FrequencyScale(&fScale);
		float minFreq = 0.0, maxFreq = 0.0;
		pSource->get_MinFrequency(&minFreq);
		pSource->get_MaxFrequency(&maxFreq);
		ComPtr<SpectrumData> result;
		hr = MakeAndInitialize<SpectrumData>(&result, channels, elements, ampScale, fScale, minFreq, maxFreq, true);
		if (FAILED(hr))
			return hr;
		return result.CopyTo(ppResult);
	}
	HRESULT SourceConverter::CloneScalarData(IScalarData *pSource, IScalarData **ppResult)
	{
		if (pSource == nullptr)
		{
			*ppResult = nullptr;
			return S_OK;
		}
		ComPtr<IVectorView<float>> vv;
		HRESULT hr = pSource->QueryInterface<IVectorView<float>>(&vv);
		if (FAILED(hr))
			return hr;
		UINT32 elements = 0;
		vv->get_Size(&elements);
		ComPtr<ScalarData> result = Make<ScalarData>(elements);
		return result.CopyTo(ppResult);
	}

	HRESULT SourceConverter::CloneFromFrame(IVisualizationDataFrame * pSource, IVisualizationDataFrame ** ppResult)
	{
		ComPtr<IScalarData> rms;
		ComPtr<IScalarData> peak;
		ComPtr<ISpectrumData> spectrum;

		TimeSpan time = { -1 }, duration = { 0 };
		_cachedOutputFrame->get_Time(&time);
		_cachedOutputFrame->get_Duration(&duration);

		ComPtr<ISpectrumData> s;
		_cachedOutputFrame->get_Spectrum(&s);
		CloneSpectrum(s.Get(), &spectrum);
		ComPtr<IScalarData> r;
		_cachedOutputFrame->get_RMS(&r);
		CloneScalarData(r.Get(), &rms);
		ComPtr<IScalarData> p;
		_cachedOutputFrame->get_Peak(&p);
		CloneScalarData(p.Get(), &peak);

		ComPtr<IVisualizationDataFrame> frame = Make<VisualizationDataFrame>(time.Duration, duration.Duration, rms.Get(), peak.Get(), spectrum.Get());
		return frame.CopyTo(ppResult);
	}

	HRESULT SourceConverter::ApplyFrequencyTransforms(ISpectrumData *pSource, ISpectrumData **ppResult)
	{
		// No conversion
		if (_frequencyScale == nullptr && _minFrequency == nullptr && _maxFrequency == nullptr && _elementCount == nullptr)
		{
			return ComPtr<ISpectrumData>(pSource).CopyTo(ppResult);
		}

		UINT32 elementCount = 0;
		if (_elementCount != nullptr)
			_elementCount->get_Value(&elementCount);
		else
		{
			pSource->get_FrequencyCount(&elementCount);	// No change
		}
		float minFrequency = 0.0f;
		if (_minFrequency != nullptr)
			_minFrequency->get_Value(&minFrequency);
		else
			pSource->get_MinFrequency(&minFrequency);

		float maxFrequency = 0.0f;
		if (_maxFrequency != nullptr)
			_maxFrequency->get_Value(&maxFrequency);
		else
			pSource->get_MaxFrequency(&maxFrequency);

		ScaleType fScale = ScaleType::Linear;
		if (_frequencyScale != nullptr)
			_frequencyScale->get_Value(&fScale);
		else
			pSource->get_FrequencyScale(&fScale);

		if (fScale == ScaleType::Linear)
			return pSource->LinearTransform(elementCount, minFrequency, maxFrequency, ppResult);
		else
			return pSource->LogarithmicTransform(elementCount, minFrequency, maxFrequency, ppResult);
	}

	HRESULT SourceConverter::ApplyRiseAndFall(ISpectrumData *pData, ISpectrumData *pPrevious, IReference<TimeSpan> *pRiseTime, IReference<TimeSpan> *pFallTime, ISpectrumData **ppResult)
	{
		if (ppResult == nullptr)
			return E_POINTER;

		if (pRiseTime == nullptr && pFallTime == nullptr)
		{
			return ComPtr<ISpectrumData>(pData).CopyTo(ppResult);
		}

		TimeSpan riseTime = { 1 };	// Init with very fast rise and fall times
		if (pRiseTime != nullptr)
			pRiseTime->get_Value(&riseTime);
		TimeSpan fallTime = { 1 };
		if (pFallTime != nullptr)
			pFallTime->get_Value(&fallTime);

		if (pData != nullptr)
		{
			pData->ApplyRiseAndFall(pPrevious, riseTime, fallTime, _timeFromPrevious, ppResult);
		}
		else if (pPrevious != nullptr)
		{
			ComPtr<ISpectrumDataStatics> spectrumStatics;
			HRESULT hr = GetActivationFactory(HStringReference(RuntimeClass_AudioVisualizer_SpectrumData).Get(), &spectrumStatics);
			spectrumStatics->ApplyRiseAndFallToEmpty(pPrevious, riseTime, fallTime, _timeFromPrevious, ppResult);
		}
		return S_OK;
	}

	HRESULT SourceConverter::ApplyRiseAndFall(IScalarData *pData, IScalarData *pPrevious, IReference<TimeSpan> *pRiseTime, IReference<TimeSpan> *pFallTime, IScalarData **ppResult)
	{
		if (ppResult == nullptr)
			return E_POINTER;

		if (pRiseTime == nullptr && pFallTime == nullptr)
		{
			return ComPtr<IScalarData>(pData).CopyTo(ppResult);
		}
		TimeSpan riseTime = { 1 }, fallTime = { 1 };
		pRiseTime->get_Value(&riseTime);
		pFallTime->get_Value(&fallTime);
		if (pData != nullptr)
		{
			pData->ApplyRiseAndFall(pPrevious, riseTime, fallTime, _timeFromPrevious, ppResult);
		}
		else if (pPrevious != nullptr)
		{
			ComPtr<IScalarDataStatics> scalar;
			HRESULT hr = GetActivationFactory(HStringReference(RuntimeClass_AudioVisualizer_ScalarData).Get(), &scalar);
			scalar->ApplyRiseAndFallToEmpty(pPrevious, riseTime, fallTime, _timeFromPrevious, ppResult);
		}
		return S_OK;
	}

	ActivatableClass(SourceConverter);
}
