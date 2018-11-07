#pragma once

#include "PlaybackSource.g.h"
#include "VisualizerEffectDefinition.h"

namespace winrt::AudioVisualizer::implementation
{
    struct PlaybackSource : PlaybackSourceT<PlaybackSource>
    {
		winrt::event<Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable,IVisualizationSource>> _sourceChangedEvent;
		IVisualizationSource _source;
		Windows::Foundation::Collections::PropertySet _propSet;

        PlaybackSource();
		PlaybackSource(Windows::Media::Playback::MediaPlayer const& mediaPlayer);
		PlaybackSource(Windows::Media::Audio::IAudioNode const&node);

        AudioVisualizer::IVisualizationSource Source();
		event_token SourceChanged(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::IVisualizationSource> const& handler);
		void SourceChanged(event_token const& token);

        static AudioVisualizer::PlaybackSource CreateFromMediaPlayer(Windows::Media::Playback::MediaPlayer const& mediaPlayer);
		static AudioVisualizer::PlaybackSource CreateFromAudioNode(Windows::Media::Audio::IAudioNode const&audioNode);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct PlaybackSource : PlaybackSourceT<PlaybackSource, implementation::PlaybackSource>
    {
    };
}
