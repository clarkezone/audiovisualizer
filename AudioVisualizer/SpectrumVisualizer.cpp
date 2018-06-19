#include "pch.h"
#include "SpectrumVisualizer.h"
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

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

	void SpectrumVisualizer::OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime)
	{
		std::lock_guard<std::mutex> lock(_lock);
		SpectrumData data{ nullptr }; 
		if (dataFrame && dataFrame.Spectrum())
		{
			if (dataFrame.Spectrum().AmplitudeScale() == ScaleType::Linear) {
				data = dataFrame.Spectrum().ConvertToDecibels(_levels.front().Level, _levels.back().Level);
			}
			else {
				data = dataFrame.Spectrum();
			}
		}

		if (!data || _channelIndex >= data.Size())
			return;
	
		Windows::Foundation::Collections::IVectorView<float> spectrum = data.GetAt(_channelIndex);

		Size barSize(_orientation == Orientation::Vertical ? _swapChainSize.Width / spectrum.Size() : _swapChainSize.Width,
							_orientation == Orientation::Vertical ? _swapChainSize.Height : _swapChainSize.Height / spectrum.Size());
		
		Rect barRect(_orientation == Orientation::Vertical ? Point(0,_swapChainSize.Height - barSize.Height) : Point(0,0), barSize);

		for (size_t barIndex = 0; barIndex < spectrum.Size(); barIndex++)
		{
			DrawBar(drawingSession, spectrum.GetAt(barIndex), _levels.front().Level - 1, barRect);
			if (_orientation == Orientation::Vertical) {
				barRect.X += barSize.Width;
			}
			else {
				barRect.Y += barSize.Height;
			}
		}
	}
}
