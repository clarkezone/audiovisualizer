#include "pch.h"
#include "SpectrumVisualizer.h"

namespace AudioVisualizer
{
	SpectrumVisualizer::SpectrumVisualizer()
	{
		_orientation = Orientation::Orientation_Vertical; // Vertical bars
		_channelIndex = 0; // Map to first channel by default
		_elementSize = Size() = { 20,4 };
		_elementMargin = Thickness() = { 5,1,5,1 };
		_unlitElement = Color() = { 0xff, 0x40, 0x40, 0x40 };
		_riseTime = TimeSpan() = { 1000000L };	// 100ms
		_fallTime = TimeSpan() = { 1000000L };	// 100ms
		_barCount = 10;
		_minFrequency = 20.0f;
		_maxFrequency = 20000.f;
		_frequencyScale = ScaleType::Logarithmic;
		_levels.resize(24);
		int level = -60;
		for (size_t i = 0; i < 24; i++, level += 3)
		{
			_levels[i].Level = (float)level;
			if (level < -6)
				_levels[i].Color = Color() = { 255 , 0, 255, 255 };
			else if (level <= 0)
				_levels[i].Color = Color() = { 255, 255, 255, 0 };
			else
				_levels[i].Color = Color() = { 255, 255, 0, 0 };
		}
		Rescale();
		ResizeControl();
	}


	SpectrumVisualizer::~SpectrumVisualizer()
	{
	}
	void SpectrumVisualizer::ResizeControl()
	{
		float spectrumWidth = _barCount * (_elementSize.Width + (float)_elementMargin.Left + (float)_elementMargin.Right);
		float spectrumHeight = _levels.size() * (_elementSize.Height + (float)_elementMargin.Top + (float)_elementMargin.Bottom);
		auto element = As<IFrameworkElement>(GetControl());
		element->put_Width(spectrumWidth);
		element->put_Height(spectrumHeight);

	}

	HRESULT SpectrumVisualizer::Rescale()
	{
		_previousSpectrum = nullptr;
		_emptySpectrum = nullptr;

		HRESULT hr = MakeAndInitialize<SpectrumData>(
			&_emptySpectrum,
			1,
			_barCount,
			ScaleType::Linear,
			_frequencyScale,
			_minFrequency,
			_maxFrequency,
			true);

		if (FAILED(hr))
			return hr;

		return S_OK;
	}

	HRESULT SpectrumVisualizer::OnDraw(ICanvasDrawingSession * pSession, IVisualizationDataFrame * pDataFrame, IReference<TimeSpan>* pPresentationTime)
	{
		auto lock = _lock.LockExclusive();
		HRESULT hr = S_OK;

		ComPtr<ISpectrumData> data;

		if (pDataFrame != nullptr)
		{
			ComPtr<ISpectrumData> frameData;
			pDataFrame->get_Spectrum(&frameData);
			if (frameData != nullptr)
			{
				ComPtr<ISpectrumData> spectrumData;	// This is a spectrum the right spectrum count and scale
				if (_frequencyScale == ScaleType::Linear)
					hr = frameData->TransformLinearFrequencyWithRange(_barCount, _minFrequency, _maxFrequency, &spectrumData);
				else
					hr = frameData->TransformLogFrequency(_barCount, _minFrequency, _maxFrequency, &spectrumData);
				if (FAILED(hr))
					return hr;

				ComPtr<ISpectrumData> spectrumCombined;
				ComPtr<IVectorView<IVectorView<float> *>> vectorView;
				hr = spectrumData.As(&vectorView);
				if (FAILED(hr))
					return hr;

				UINT32 inputDataChannelCount = 0;
				vectorView->get_Size(&inputDataChannelCount);

				// TODO: Mapping logic
				data = spectrumData;
			}
			else
				data = _emptySpectrum;
		}
		else
			data = _emptySpectrum;

		// Apply rise and fall
		_previousSpectrum = data;

		ComPtr<ISpectrumData> logData;
		data->ConvertToLogAmplitude(-100, 0, &logData);

		ComPtr<IVectorView<IVectorView<float>*>> channels;
		logData.As(&channels);

		ComPtr<IVectorView<float>> channelData;
		if (SUCCEEDED(channels->GetAt(_channelIndex, &channelData)))
		{
			for (size_t barIndex = 0; barIndex < _barCount; barIndex++)
			{
				float level = -100.0f;
				channelData->GetAt(barIndex, &level);
				Rect elementRect;
				elementRect.X = barIndex * (_elementSize.Width + (float)_elementMargin.Left + (float)_elementMargin.Right) + (float)_elementMargin.Left;
				elementRect.Width = _elementSize.Width;
				elementRect.Height = _elementSize.Height;

				for (size_t levelIndex = 0; levelIndex < _levels.size(); levelIndex++)
				{
					MeterBarLevel elementLevel = _levels[levelIndex];
					MeterBarLevel nextElementLevel = levelIndex + 1 < _levels.size() ? _levels[levelIndex + 1] : elementLevel;
					Color elementColor;
					if (level >= nextElementLevel.Level)	// Element is fully lit
						elementColor = elementLevel.Color;
					else if (level >= elementLevel.Level)
					{
						// Element partly lit
						elementColor = elementLevel.Color;
					}
					else
					{
						elementColor = _unlitElement;
					}
					elementRect.Y = (_levels.size() - levelIndex) * (_elementSize.Height + (float)_elementMargin.Top + (float)_elementMargin.Bottom) + (float)_elementMargin.Top;
					pSession->FillRoundedRectangleWithColor(elementRect, 0, 0, elementColor);
				}
			}
		}
		return hr;
	}

	ActivatableClass(SpectrumVisualizer);
}
