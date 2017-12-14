#include "pch.h"
#include "SourceConverter.h"
#include "VisualizationDataFrame.h"
#include <vector>

namespace AudioVisualizer
{
	/* Channel mappingss https://msdn.microsoft.com/en-us/library/windows/desktop/ee415748(v=vs.85).aspx */
	std::vector<float> mapFromStereoToMono = { 0.5f,0.5f };
	std::vector<float> mapFromMonoToStereo = { 0.5f, 0.5f };

	SourceConverter::SourceConverter()
	{
		_analyzerTypes = AnalyzerType::All;
	}

	SourceConverter::~SourceConverter()
	{
	}
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

		if (sourceFrame == nullptr)
		{
			*ppResult = nullptr;
			return S_OK;
		}

		TimeSpan frameTime;
		sourceFrame->get_Time(&frameTime);
		TimeSpan frameDuration;
		sourceFrame->get_Duration(&frameDuration);
		
		ComPtr<ISpectrumData> spectrum;
		sourceFrame->get_Spectrum(&spectrum);
		ComPtr<IScalarData> rms;
		sourceFrame->get_RMS(&rms);
		ComPtr<IScalarData> peak;
		sourceFrame->get_Peak(&peak);

		if ((_analyzerTypes & AnalyzerType::Spectrum) == AnalyzerType::Spectrum)
		{
			hr = ApplyFrequencyTransforms(spectrum);
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

	HRESULT SourceConverter::ApplyFrequencyTransforms(ComPtr<ISpectrumData>& data)
	{
		// No conversion
		if (_frequencyScale == nullptr && _minFrequency == nullptr && _maxFrequency == nullptr && _elementCount == nullptr)
			return S_OK;
		UINT32 elementCount = 0;
		if (_elementCount != nullptr)
			_elementCount->get_Value(&elementCount);
		else
		{
			data->get_FrequencyCount(&elementCount);	// No change
		}
		float minFrequency = 0.0f;
		if (_minFrequency != nullptr)
			_minFrequency->get_Value(&minFrequency);
		else
			data->get_MinFrequency(&minFrequency);

		float maxFrequency = 0.0f;
		if (_maxFrequency != nullptr)
			_maxFrequency->get_Value(&maxFrequency);
		else
			data->get_MaxFrequency(&maxFrequency);

		ScaleType fScale = ScaleType::Linear;
		if (_frequencyScale != nullptr)
			_frequencyScale->get_Value(&fScale);
		else
			data->get_FrequencyScale(&fScale);

		ComPtr<ISpectrumData> convertedData;
		if (fScale == ScaleType::Linear)
			data->LinearTransform(elementCount, minFrequency, maxFrequency, &convertedData);
		else
			data->LogarithmicTransform(elementCount, minFrequency, maxFrequency, &convertedData);

		data = convertedData;

		return S_OK;
	}
	ActivatableClass(SourceConverter);
}
