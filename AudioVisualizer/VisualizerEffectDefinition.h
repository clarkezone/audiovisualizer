#pragma once

#include "VisualizerEffectDefinition.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct VisualizerEffectDefinition : VisualizerEffectDefinitionT<VisualizerEffectDefinition>
    {
		Windows::Foundation::Collections::IPropertySet properties;
        VisualizerEffectDefinition(Windows::Foundation::Collections::IPropertySet properties);
        hstring ActivatableClassId();
        Windows::Foundation::Collections::IPropertySet Properties();
    };
}
