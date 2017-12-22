#include "pch.h"
#include <algorithm>
#include "SpectrumVisualizer.h"

namespace AudioVisualizer
{
	SpectrumVisualizer::SpectrumVisualizer()
	{
		_orientation = Orientation::Orientation_Vertical; // Vertical bars
		_channelIndex = 0; // Map to first channel by default
		_elementMargin = Thickness() = { 0.1,0.1,0.1,0.1 };
		_unlitElement = Color() = { 0xff, 0x40, 0x40, 0x40 };
		_style = BarVisualizationStyle::Blocks;
		_controlSize.Width = 0.0f;
		_controlSize.Height = 0.0f;
		_levels.resize(24);
		_minAmp = -60.0;
		_maxAmp = 12.0f;
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

		ComPtr<IFrameworkElement> frameworkElement = As<IFrameworkElement>(GetControl());

		ThrowIfFailed(frameworkElement->add_SizeChanged(Callback<ISizeChangedEventHandler>(
			[=](IInspectable *pSender, ISizeChangedEventArgs *pArgs) -> HRESULT
		{
			pArgs->get_NewSize(&_controlSize);
			return S_OK;
		}).Get(), &_sizeChangedToken));

	}

	SpectrumVisualizer::~SpectrumVisualizer()
	{
		ComPtr<IFrameworkElement> frameworkElement = As<IFrameworkElement>(GetControl());
		frameworkElement->remove_SizeChanged(_sizeChangedToken);
	}

	STDMETHODIMP SpectrumVisualizer::put_Source(ABI::AudioVisualizer::IVisualizationSource * pSource)
	{
		auto lock = _lock.LockExclusive();

		if (_visualizationSource != nullptr)
		{
			// Remove change handler
			_visualizationSource->remove_ConfigurationChanged(_sourceConfigurationChangedToken);
		}
		_visualizationSource = pSource;
		if (pSource != nullptr)
		{
			_visualizationSource->add_ConfigurationChanged(
				Callback<ITypedEventHandler<IVisualizationSource *, HSTRING>>
				(
					[=](IVisualizationSource *pSender, HSTRING propertyName)->HRESULT
					{
						auto lock = _lock.LockExclusive();
						ReconfigureSpectrum(propertyName);
						return S_OK;
					}
				).Get(),
				&_sourceConfigurationChangedToken
			);
		}
		ReconfigureSpectrum(HStringReference(L"Source").Get());
		return S_OK;
	}
	
	void SpectrumVisualizer::ReconfigureSpectrum(HSTRING propertyName)
	{
		_expectedFrequencyCount = 0;
		if (_visualizationSource == nullptr)
			return;
		ComPtr<IReference<UINT32>> fCount;
		_visualizationSource->get_ActualFrequencyCount(&fCount);
		if (fCount != nullptr)
			fCount->get_Value(&_expectedFrequencyCount);
	}

	HRESULT SpectrumVisualizer::OnDraw(ICanvasDrawingSession * pSession, IVisualizationDataFrame * pDataFrame, IReference<TimeSpan>* pPresentationTime)
	{
		auto lock = _lock.LockExclusive();
		HRESULT hr = S_OK;

		ComPtr<ISpectrumData> data;
		if (pDataFrame != nullptr)
		{
			ComPtr<ISpectrumData> spectrumData;
			pDataFrame->get_Spectrum(&spectrumData);

			if (spectrumData != nullptr)
			{
				ScaleType ampScale = ScaleType::Linear;
				spectrumData->get_AmplitudeScale(&ampScale);
				if (ampScale == ScaleType::Linear)
				{
					// Amplitude is linear, convert to logarithmic
					ComPtr<ISpectrumData> logSpectrum;
					spectrumData->ConvertToDecibels(_minAmp, _maxAmp, &logSpectrum);
					data = logSpectrum;
				}
				else
					data = spectrumData;
			}
		}

		UINT32 barCount = _expectedFrequencyCount;
		if (data != nullptr)
		{
			data->get_FrequencyCount(&barCount);
		}

		if (barCount == 0)	// Cannot determine bar count stop drawing
			return S_FALSE;

		float barAbsWidth = (_orientation == Orientation::Orientation_Vertical ? _controlSize.Width : _controlSize.Height) / (float) barCount;
		float barAbsHeight = (_orientation == Orientation::Orientation_Vertical ? _controlSize.Height : _controlSize.Width);

		ComPtr<IVectorView<float>> spectrum;
		if (data != nullptr)
		{
			ComPtr<IVectorView<IVectorView<float>*>> channels;
			data.As(&channels);
			channels->GetAt(_channelIndex, &spectrum);
		}

		for (size_t barIndex = 0; barIndex < barCount; barIndex++)
		{
			float level = -100.0f;
			if (spectrum != nullptr)
			{
				spectrum->GetAt(barIndex, &level);
			}
			if (_style == BarVisualizationStyle::Blocks || _style == BarVisualizationStyle::TopBlock)
			{
				Rect elementRect;
				float elementHeight = (barAbsHeight / (float)_levels.size());
				elementRect.X = barIndex * barAbsWidth + barAbsWidth * (float) _elementMargin.Left;
				elementRect.Height =  elementHeight * (1.0f - (float) _elementMargin.Top - (float) _elementMargin.Bottom);
				elementRect.Width = (barAbsWidth) * (1.0f - (float) _elementMargin.Left - (float) _elementMargin.Right);

				for (size_t levelIndex = 0; levelIndex < _levels.size(); levelIndex++)
				{
					MeterBarLevel elementLevel = _levels[levelIndex];
					MeterBarLevel nextElementLevel = levelIndex + 1 < _levels.size() ? _levels[levelIndex + 1] : elementLevel;
					Color elementColor;
					if (level >= nextElementLevel.Level)	// Element is fully lit unless only top block style
						elementColor = _style == BarVisualizationStyle::Blocks ? elementLevel.Color : _unlitElement;
					else if (level > elementLevel.Level)
					{
						// Element partly lit
						elementColor = elementLevel.Color;
					}
					else
					{
						elementColor = _unlitElement;
					}
					elementRect.Y = (_levels.size() - levelIndex) * elementHeight + (float) _elementMargin.Top;
					pSession->FillRoundedRectangleWithColor(elementRect, 0, 0, elementColor);
				}
			}
		}
		
		return hr;
	}

	ActivatableClass(SpectrumVisualizer);
}
