#include "pch.h"
#include "DiscreteVUBar.h"

namespace winrt::AudioVisualizer::implementation
{
	void DiscreteVUBar::OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime)
	{
		OutputDebugString(L"OnDraw");
	}
}
