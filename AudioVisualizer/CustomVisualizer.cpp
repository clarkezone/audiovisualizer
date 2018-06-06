#include "pch.h"
#include "CustomVisualizer.h"

namespace winrt::AudioVisualizer::implementation
{
    event_token CustomVisualizer::Draw(Windows::Foundation::TypedEventHandler<AudioVisualizer::VisualizerControl, AudioVisualizer::VisualizerDrawEventArgs> const& handler)
    {
		return _drawEvent.add(handler);
    }

    void CustomVisualizer::Draw(event_token const& token)
    {
		_drawEvent.remove(token);
    }

    event_token CustomVisualizer::CreateResources(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::CreateResourcesEventArgs> const& handler)
    {
		return _createResourcesEvent.add(handler);
    }

    void CustomVisualizer::CreateResources(event_token const& token)
    {
		_createResourcesEvent.remove(token);
    }
}
