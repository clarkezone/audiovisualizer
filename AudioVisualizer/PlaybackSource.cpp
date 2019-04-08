#include "pch.h"
#include "PlaybackSource.h"
#include "Tracing.h"
#include <winrt/Windows.Foundation.Collections.h>


namespace winrt::AudioVisualizer::implementation
{
    AudioVisualizer::IVisualizationSource PlaybackSource::Source()
    {
		return _source;
    }

	event_token PlaybackSource::SourceChanged(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable,AudioVisualizer::IVisualizationSource> const& handler)
	{
		return _sourceChangedEvent.add(handler);
	}

	void PlaybackSource::SourceChanged(event_token const& token)
	{
		_sourceChangedEvent.remove(token);
	}

	PlaybackSource::PlaybackSource()
	{
		_propSet = Windows::Foundation::Collections::PropertySet();
		_propSet.MapChanged([this](IInspectable const &, Windows::Foundation::Collections::IMapChangedEventArgs<hstring> const &args)
		{
			if (args.Key() == L"Source") {
				auto source = _propSet.Lookup(args.Key()).as<AudioVisualizer::IVisualizationSource>();
				this->_source = source;
#ifdef _TRACE_
				Trace::PlaybackSource_SourcePropertyChanged(source);
#endif
				_sourceChangedEvent(*this, source);
			}
		});
	}

	PlaybackSource::PlaybackSource(Windows::Media::Playback::MediaPlayer const& mediaPlayer) : PlaybackSource()
	{
		if (!mediaPlayer)
			throw hresult_invalid_argument();

#ifdef _TRACE_
		Trace::PlaybackSource_CreateFromMediaPlayer();
#endif
		mediaPlayer.AddAudioEffect(L"AudioVisualizer.MediaAnalyzer", false, _propSet);
	}

	PlaybackSource::PlaybackSource(Windows::Media::Audio::IAudioNode const & node) : PlaybackSource()
	{
		if (!node)
			throw hresult_invalid_argument();
		auto effect = make<VisualizerEffectDefinition>(_propSet);
		
		if (!node.EffectDefinitions())
			throw hresult_error(E_NOT_VALID_STATE);

		node.EffectDefinitions().Append(effect);
	}

	AudioVisualizer::PlaybackSource PlaybackSource::CreateFromMediaPlayer(Windows::Media::Playback::MediaPlayer const& mediaPlayer)
	{
		return make<PlaybackSource>(mediaPlayer);
	}
	AudioVisualizer::PlaybackSource PlaybackSource::CreateFromAudioNode(Windows::Media::Audio::IAudioNode const & audioNode)
	{
		return make<PlaybackSource>(audioNode);
	}
}
