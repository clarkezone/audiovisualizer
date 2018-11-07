#include "pch.h"
#include <limits>
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

	void DiscreteVUBar::OnUpdateMeter(VisualizationDataFrame const &frame)
	{
		float rmsValue = -100.0f;
		float peakValue = -100.0f;
		if (frame && frame.RMS() && frame.RMS().Size() > _channelIndex)
		{
			rmsValue = frame.RMS().AmplitudeScale() == ScaleType::Linear ? 20.0f * log10f(frame.RMS().GetAt(_channelIndex)) : frame.RMS().GetAt(_channelIndex);
		}
		if (_displayPeak && frame && frame.Peak() && frame.Peak().Size() > _channelIndex)
		{
			peakValue = frame.Peak().AmplitudeScale() == ScaleType::Linear ? 20.0f * log10f(frame.Peak().GetAt(_channelIndex)) : frame.Peak().GetAt(_channelIndex);
		}
		UpdateBarValue(0, rmsValue, peakValue);
	}
}
