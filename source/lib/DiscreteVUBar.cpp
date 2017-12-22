#include "pch.h"
#include "DiscreteVUBar.h"
#include <wrl.h>
#include "ScalarData.h"
#include <Microsoft.Graphics.Canvas.h>

namespace AudioVisualizer
{
	DiscreteVUBar::DiscreteVUBar()
	{
		_levels.resize(24);
		int level = -60;
		for (size_t i = 0; i < 24; i++, level += 3)
		{
			_levels[i].Level = (float)level;
			if (level < -6)
				_levels[i].Color = Color() = { 255 , 0, 255, 0 };
			else if (level <= 0)
				_levels[i].Color = Color() = { 255, 255, 255, 0 };
			else
				_levels[i].Color = Color() = { 255, 255, 0, 0 };
		}
		_minAmp = -60.0f;
		_maxAmp = -12.0f;
		_channelIndex = 0;
		_orientation = Orientation::Orientation_Vertical;
		_elementMargin = Thickness() = { 0, 0, 0, 0 };
		_unlitElement = Color() = { 0, 96, 96, 96 };

		ComPtr<IFrameworkElement> frameworkElement = As<IFrameworkElement>(GetControl());

		ThrowIfFailed(frameworkElement->add_SizeChanged(Callback<ISizeChangedEventHandler>(
			[=](IInspectable *pSender, ISizeChangedEventArgs *pArgs) -> HRESULT
		{
			pArgs->get_NewSize(&_controlSize);
			return S_OK;
		}).Get(), &_sizeChangedToken));
	}


	DiscreteVUBar::~DiscreteVUBar()
	{
	}


	HRESULT DiscreteVUBar::OnDraw(ICanvasDrawingSession *pDrawingSession, IVisualizationDataFrame *pDataFrame, IReference<TimeSpan> *)
	{
		auto lock = _lock.LockExclusive();
		HRESULT hr = S_OK;

		ComPtr<IScalarData> data;
		if (pDataFrame != nullptr)
		{
			ComPtr<IScalarData> rmsData;
			pDataFrame->get_RMS(&rmsData);

			if (rmsData != nullptr)
			{
				ScaleType ampScale = ScaleType::Linear;
				rmsData->get_AmplitudeScale(&ampScale);
				if (ampScale == ScaleType::Linear)
				{
					// Amplitude is linear, convert to logarithmic
					ComPtr<IScalarData> logRms;
					rmsData->ConvertToDecibels(_minAmp, _maxAmp, &logRms);
					data = logRms;
				}
				else
					data = rmsData;
			}
		}

		float barAbsWidth = (_orientation == Orientation::Orientation_Vertical ? _controlSize.Width : _controlSize.Height);
		float barAbsHeight = (_orientation == Orientation::Orientation_Vertical ? _controlSize.Height : _controlSize.Width);

		float level = -100.0f;
		if (data != nullptr)
		{
			ComPtr<IVector<float>> values;
			data.As(&values);
			UINT32 size = 0;
			values->get_Size(&size);
			if (_channelIndex < size)
				values->GetAt(_channelIndex, &level);
		}

			Rect elementRect;
			float elementHeight = (barAbsHeight / (float)_levels.size());
			elementRect.X = barAbsWidth * (float)_elementMargin.Left;
			elementRect.Height = elementHeight * (1.0f - (float)_elementMargin.Top - (float)_elementMargin.Bottom);
			elementRect.Width = (barAbsWidth) * (1.0f - (float)_elementMargin.Left - (float)_elementMargin.Right);

			for (size_t levelIndex = 0; levelIndex < _levels.size(); levelIndex++)
			{
				MeterBarLevel elementLevel = _levels[levelIndex];
				MeterBarLevel nextElementLevel = levelIndex + 1 < _levels.size() ? _levels[levelIndex + 1] : elementLevel;
				Color elementColor;
				if (level >= nextElementLevel.Level)	// Element is fully lit
					elementColor = elementLevel.Color;
				else if (level > elementLevel.Level)
				{
					// Element partly lit
					elementColor = elementLevel.Color;
				}
				else
				{
					elementColor = _unlitElement;
				}
				elementRect.Y = (_levels.size() - levelIndex) * elementHeight + (float)_elementMargin.Top;
				pDrawingSession->FillRoundedRectangleWithColor(elementRect, 0, 0, elementColor);
		}


		return hr;
	}

	ActivatableClass(DiscreteVUBar);
}
