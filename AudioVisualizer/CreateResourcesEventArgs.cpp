#include "pch.h"
#include "CreateResourcesEventArgs.h"

namespace winrt::AudioVisualizer::implementation
{
    CreateResourcesEventArgs::CreateResourcesEventArgs(AudioVisualizer::CreateResourcesReason const& reason, Windows::Foundation::IInspectable const& resourceCreator)
    {
		_reason = reason;
		_resourceCreator = resourceCreator;
    }

    AudioVisualizer::CreateResourcesReason CreateResourcesEventArgs::Reason()
    {
		return _reason;
    }

    Windows::Foundation::IInspectable CreateResourcesEventArgs::ResourceCreator()
    {
		return _resourceCreator;
    }
}
