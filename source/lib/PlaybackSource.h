#pragma once
#include <windows.media.playback.h>

using namespace Microsoft::WRL;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Media::Playback;
using namespace ABI::Windows::Foundation::Collections;

namespace AudioVisualizer
{
	class PlaybackSource : public RuntimeClass<IPlaybackSource>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_PlaybackSource, BaseTrust);

		ComPtr<IPropertySet> _spPropSet;
		ComPtr<IVisualizationSource> _spSource;
		EventRegistrationToken _propSetChangedToken;

		typedef ABI::Windows::Foundation::ITypedEventHandler<
			IInspectable*,
			IVisualizationSource *> PlaybackSource_SourceChangedHandler;

		EventSource<PlaybackSource_SourceChangedHandler> _sourceChangedHandlers;
		HRESULT SetupPropSetWatcher();

	public:

		HRESULT RuntimeClassInitialize(IMediaPlayer *pPlayer);

		STDMETHODIMP get_Source(ABI::AudioVisualizer::IVisualizationSource **ppSource)
		{
			if (ppSource == nullptr)
				return E_POINTER;
			return _spSource.CopyTo(ppSource);
		}

		STDMETHODIMP add_SourceChanged(
			PlaybackSource_SourceChangedHandler* value,
			EventRegistrationToken *token)
		{
			return _sourceChangedHandlers.Add(value, token);
		}
		STDMETHODIMP remove_SourceChanged(
			EventRegistrationToken token)
		{
			return _sourceChangedHandlers.Remove(token);
		}
	};
}

