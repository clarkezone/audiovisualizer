#pragma once

#include "CreateResourcesEventArgs.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct CreateResourcesEventArgs : CreateResourcesEventArgsT<CreateResourcesEventArgs>
    {
		CreateResourcesReason _reason;
		Windows::Foundation::IInspectable _resourceCreator;

        CreateResourcesEventArgs() = delete;
        CreateResourcesEventArgs(AudioVisualizer::CreateResourcesReason const& reason, Windows::Foundation::IInspectable const& resourceCreator);

        AudioVisualizer::CreateResourcesReason Reason();
        Windows::Foundation::IInspectable ResourceCreator();
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct CreateResourcesEventArgs : CreateResourcesEventArgsT<CreateResourcesEventArgs, implementation::CreateResourcesEventArgs>
    {
    };
}
