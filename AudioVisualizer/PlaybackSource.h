#pragma once

#include "PlaybackSource.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct PlaybackSource : PlaybackSourceT<PlaybackSource>
    {
		winrt::event<Windows::Foundation::EventHandler<IVisualizationSource>> _sourceChangedEvent;
		IVisualizationSource _source;
		Windows::Foundation::Collections::PropertySet _propSet;

        PlaybackSource() = delete;
		PlaybackSource(Windows::Media::Playback::MediaPlayer const& mediaPlayer);

        AudioVisualizer::IVisualizationSource Source();
		event_token SourceChanged(Windows::Foundation::EventHandler<AudioVisualizer::IVisualizationSource> const& handler);
		void SourceChanged(event_token const& token);

        static AudioVisualizer::PlaybackSource Create(Windows::Media::Playback::MediaPlayer const& mediaPlayer);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct PlaybackSource : PlaybackSourceT<PlaybackSource, implementation::PlaybackSource>
    {
    };
}
