#include "pch.h"
#include "SpectrumVisualizer.h"

namespace AudioVisualizer
{
	SpectrumVisualizer::SpectrumVisualizer()
	{
		_orientation = Orientation::Orientation_Vertical; // Vertical bars
		_channelCount = 1; // Display mono spectrum
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
	}


	SpectrumVisualizer::~SpectrumVisualizer()
	{
	}
	void SpectrumVisualizer::ResizeControl()
	{
	}
	void SpectrumVisualizer::Rescale()
	{
	}
	HRESULT SpectrumVisualizer::OnDraw(ICanvasDrawingSession * pSession, IVisualizationDataFrame * pDataFrame, IReference<TimeSpan>* pPresentationTime)
	{
		return S_OK;
	}

	ActivatableClass(SpectrumVisualizer);
}
