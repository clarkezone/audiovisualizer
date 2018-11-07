#pragma once

#include "VisualizerDrawEventArgs.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct VisualizerDrawEventArgs : VisualizerDrawEventArgsT<VisualizerDrawEventArgs>
    {
		VisualizationDataFrame _dataFrame;
		Windows::Foundation::IInspectable _drawingSession;
		Windows::Foundation::Size _viewExtent;
		Windows::Foundation::IReference<Windows::Foundation::TimeSpan> _presentationTime;

        VisualizerDrawEventArgs() = delete;
        VisualizerDrawEventArgs(AudioVisualizer::VisualizationDataFrame const& data, Windows::Foundation::IInspectable const& drawingSession, Windows::Foundation::Size const& viewExtent, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> const& presentationTime);

        AudioVisualizer::VisualizationDataFrame Data();
        Windows::Foundation::IInspectable DrawingSession();
        Windows::Foundation::Size ViewExtent();
        Windows::Foundation::IReference<Windows::Foundation::TimeSpan> PresentationTime();
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct VisualizerDrawEventArgs : VisualizerDrawEventArgsT<VisualizerDrawEventArgs, implementation::VisualizerDrawEventArgs>
    {
    };
}
