#include "pch.h"
#include "VisualizerDrawEventArgs.h"

namespace winrt::AudioVisualizer::implementation
{
    VisualizerDrawEventArgs::VisualizerDrawEventArgs(AudioVisualizer::VisualizationDataFrame const& data, Windows::Foundation::IInspectable const& drawingSession, Windows::Foundation::Size const& viewExtent, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& presentationTime) : _dataFrame(data)
    {
		_drawingSession = drawingSession;
		_viewExtent = viewExtent;
		_presentationTime = presentationTime;
    }

    AudioVisualizer::VisualizationDataFrame VisualizerDrawEventArgs::Data()
    {
		return _dataFrame;
    }

    Windows::Foundation::IInspectable VisualizerDrawEventArgs::DrawingSession()
    {
		return _drawingSession;
    }

    Windows::Foundation::Size VisualizerDrawEventArgs::ViewExtent()
    {
		return _viewExtent;
    }

    Windows::Foundation::IReference<Windows::Foundation::TimeSpan> VisualizerDrawEventArgs::PresentationTime()
    {
		return _presentationTime;
    }
}
