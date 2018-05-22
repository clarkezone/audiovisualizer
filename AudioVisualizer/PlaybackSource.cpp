#include "pch.h"
#include "PlaybackSource.h"

namespace winrt::AudioVisualizer::implementation
{
    AudioVisualizer::IVisualizationSource PlaybackSource::Source()
    {
		return _source;
    }

	event_token PlaybackSource::SourceChanged(Windows::Foundation::EventHandler<AudioVisualizer::IVisualizationSource> const& handler)
	{
		return _sourceChangedEvent.add(handler);
	}

	void PlaybackSource::SourceChanged(event_token const& token)
	{
		_sourceChangedEvent.remove(token);
	}

	PlaybackSource::PlaybackSource(Windows::Media::Playback::MediaPlayer const& mediaPlayer)
	{
		if (mediaPlayer == nullptr)
			throw hresult_invalid_argument();

		_propSet = Windows::Foundation::Collections::PropertySet();
		_propSet.MapChanged([this](IInspectable const &sender, Windows::Foundation::Collections::IMapChangedEventArgs<hstring> const &args)
		{
			auto source = _propSet.Lookup(args.Key()).as<AudioVisualizer::IVisualizationSource>();
			this->_source = source;
			_sourceChangedEvent(*this, source);
		});
		mediaPlayer.AddAudioEffect(L"AudioVisualizer.MediaAnalyzer", false, _propSet);
	}
}
