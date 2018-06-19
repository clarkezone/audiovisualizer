#include "pch.h"
#include "DiscreteVUBar.h"
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

using namespace winrt::Windows::UI::Xaml::Controls;

namespace winrt::AudioVisualizer::implementation
{

	DiscreteVUBar::DiscreteVUBar()
	{
	}


	void DiscreteVUBar::OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime)
	{
		using namespace Windows::Foundation;
		using namespace Windows::UI::Xaml;

		std::lock_guard<std::mutex> lock(_lock);

		ScalarData rms{ nullptr };
		if (dataFrame && dataFrame.RMS() && dataFrame.RMS().AmplitudeScale() == ScaleType::Linear)
		{
			rms = dataFrame.RMS().ConvertToDecibels(_levels.front().Level,_levels.back().Level);
		}
		else if (dataFrame) {
			rms = dataFrame.RMS();
		}
		ScalarData peak{ nullptr };
		if (_displayPeak && dataFrame && dataFrame.Peak() && dataFrame.Peak().AmplitudeScale() == ScaleType::Linear)
		{
			peak = dataFrame.Peak().ConvertToDecibels(_levels.front().Level, _levels.back().Level);
		}
		else if (dataFrame) {
			peak = dataFrame.Peak();
		}

		float rmsLevel = _levels.front().Level - 1;
		float peakLevel = _levels.front().Level - 1;

		if (rms && _channelIndex < rms.Size())
		{
			rmsLevel = rms.GetAt(_channelIndex);
		}
		if (peak && _channelIndex < peak.Size())
		{
			peakLevel = peak.GetAt(_channelIndex);
		}

		Rect rect(Point(0, 0),_swapChainSize);
		DrawBar(drawingSession, rmsLevel,peakLevel, rect);
	}
}
