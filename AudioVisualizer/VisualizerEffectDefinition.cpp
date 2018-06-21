#include "pch.h"
#include "VisualizerEffectDefinition.h"
#include "VisualizerAudioEffect.h"

namespace winrt::AudioVisualizer::implementation
{
	VisualizerEffectDefinition::VisualizerEffectDefinition(Windows::Foundation::Collections::IPropertySet properties)
	{
		this->properties = properties;
	}

    hstring VisualizerEffectDefinition::ActivatableClassId()
    {
       return name_of<AudioVisualizer::VisualizerAudioEffect>().data();
    }

    Windows::Foundation::Collections::IPropertySet VisualizerEffectDefinition::Properties()
    {
		return properties;
    }
}
