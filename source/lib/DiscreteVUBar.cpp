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
		_channelCount = 2;
		_orientation = Orientation::Orientation_Vertical;
		_elementSize = Size() = { 20,8 };
		_elementMargin = Thickness() = { 5, 1, 5, 1 };
		_unlitElement = Color() = { 0, 96, 96, 96 };
		_riseTime.Duration = 1000000;	// 100ms
		_fallTime.Duration = 1000000;	// 100ms

		ResizeControl();
	}


	DiscreteVUBar::~DiscreteVUBar()
	{
	}

	Size DiscreteVUBar::CalculateSize()
	{
		Size calculatedSize;
		calculatedSize.Height = _levels.size() * (_elementSize.Height + (float)_elementMargin.Top + (float)_elementMargin.Bottom);
		calculatedSize.Width = _channelCount * (_elementSize.Width + (float)_elementMargin.Left + (float)_elementMargin.Right);
		if (_orientation == Orientation::Orientation_Horizontal)
			std::swap(calculatedSize.Height, calculatedSize.Width);

		return calculatedSize;
	}

	void DiscreteVUBar::ResizeControl()
	{
		Size size = CalculateSize();
		auto element = As<IFrameworkElement>(GetControl());
		element->put_Width(size.Width);
		element->put_Height(size.Height);
	}

	HRESULT DiscreteVUBar::OnDraw(ICanvasDrawingSession *pDrawingSession, IVisualizationDataFrame *pDataFrame, IReference<TimeSpan> *)
	{
		ComPtr<IScalarData> logRms;
		ComPtr<ABI::Windows::Foundation::Collections::IVectorView<float>> rmsValues;
		UINT32 valueCount = 0;

		SourcePlaybackState state = SourcePlaybackState::Stopped;
		if (_visualizationSource != nullptr)
			_visualizationSource->get_PlaybackState(&state);

		ComPtr<IScalarData> rms;
		if (pDataFrame != nullptr && state == SourcePlaybackState::Playing)
			pDataFrame->get_RMS(&rms);
		else
			rms = Make<ScalarData>(_channelCount, ScaleType::Linear, true);

		ComPtr<IReference<TimeSpan>> ref_duration;
		TimeSpan duration = { 166667 };

		if (pDataFrame != nullptr)
		{
			pDataFrame->get_Duration(&duration);
		}
		ComPtr<IScalarData> prevValue;
		rms->ApplyRiseAndFall(_previousValues.Get(), _riseTime, _fallTime, duration, &prevValue);
		_previousValues = prevValue;
		_previousValues->ConvertToLogAmplitude(-100, 0, &logRms);
		logRms.As(&rmsValues);
		rmsValues->get_Size(&valueCount);

		for (size_t levelIndex = 0; levelIndex < _levels.size(); levelIndex++)
		{
			for (size_t channelIndex = 0; channelIndex < _channelCount; channelIndex++)
			{
				Color elementColor = _unlitElement;
				if (rmsValues != nullptr && valueCount > channelIndex)
				{
					float value = -100.0f;
					rmsValues->GetAt(channelIndex, &value);
					if (value > _levels[levelIndex].Level)
						elementColor = _levels[levelIndex].Color;
				}
				float elementX = channelIndex * (_elementSize.Width + (float)_elementMargin.Left + (float)_elementMargin.Right) + (float)_elementMargin.Left;
				float elementY = ((int)_levels.size() - (int)levelIndex) * (_elementSize.Height + (float)_elementMargin.Top + (float)_elementMargin.Bottom) + (float)_elementMargin.Top;
				if (_orientation == Orientation::Orientation_Vertical)
				{
					pDrawingSession->FillRectangleAtCoordsWithColor(
						elementX, elementY, _elementSize.Width, _elementSize.Height, elementColor
					);
				}
				else
				{
					pDrawingSession->FillRectangleAtCoordsWithColor(
						elementY, elementX, _elementSize.Height, _elementSize.Width, elementColor
					);
				}
			}
		}
		return S_OK;
	}

	ActivatableClass(DiscreteVUBar);
}
