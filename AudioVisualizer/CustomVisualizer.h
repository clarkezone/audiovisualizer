#pragma once

#include "CustomVisualizer.g.h"
#include "VisualizerControl.h"
#include "CreateResourcesEventArgs.h"
#include "VisualizerDrawEventArgs.h"

namespace winrt::AudioVisualizer::implementation
{
    struct CustomVisualizer : CustomVisualizerT<CustomVisualizer, AudioVisualizer::implementation::VisualizerControl>
    {
	private:
		winrt::event<Windows::Foundation::TypedEventHandler<AudioVisualizer::VisualizerControl, AudioVisualizer::VisualizerDrawEventArgs>> _drawEvent;
		winrt::event<Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::CreateResourcesEventArgs>> _createResourcesEvent;

	protected:
		virtual void OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession drawingSession, VisualizationDataFrame dataFrame, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime);
		virtual void OnCreateResources(Microsoft::Graphics::Canvas::ICanvasResourceCreator resourceCreator, CreateResourcesReason reason);
	public:
        CustomVisualizer() = default;

        event_token Draw(Windows::Foundation::TypedEventHandler<AudioVisualizer::VisualizerControl, AudioVisualizer::VisualizerDrawEventArgs> const& handler);
        void Draw(event_token const& token);
        event_token CreateResources(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::CreateResourcesEventArgs> const& handler);
        void CreateResources(event_token const& token);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct CustomVisualizer : CustomVisualizerT<CustomVisualizer, implementation::CustomVisualizer>
    {
    };
}
