#include "pch.h"
#include "SpectrumVisualizer.h"
#include <winrt/Microsoft.Graphics.Canvas.Text.h>
#include <pplawait.h>

using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::AudioVisualizer::implementation
{

	SpectrumVisualizer::SpectrumVisualizer()
	{
	}

	void SpectrumVisualizer::OnUpdateMeter(AudioVisualizer::VisualizationDataFrame const & frame)
	{
		Windows::Foundation::Collections::IVectorView<float> spectrumValues{ nullptr };
		if (frame && frame.Spectrum() && frame.Spectrum().Size() > _channelIndex) {
			spectrumValues = frame.Spectrum().GetAt(_channelIndex);
		}

		for (uint32_t barIndex = 0; barIndex < _barCount; barIndex++)
		{
			float value = -100.0f;
			if (spectrumValues) {
				value = frame.Spectrum().AmplitudeScale() == ScaleType::Linear ? 20.0f * log10f(spectrumValues.GetAt(barIndex)) : spectrumValues.GetAt(barIndex);
			}
			UpdateBarValue(barIndex, value, -100.0f);
		}
	}
}
