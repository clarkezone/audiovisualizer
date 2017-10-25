#include "pch.h"
#include "PlaybackSource.h"
#include "ErrorHandling.h"
#include "Utilities.h"

using namespace ABI::Windows::Media::Playback;
using namespace ABI::AudioVisualizer;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;

namespace AudioVisualizer
{
	class PlaybackSourceFactory : public ActivationFactory<IPlaybackSourceFactory>
	{
	public:
		IFACEMETHODIMP Create(IMediaPlayer *pPlayer, IPlaybackSource **ppResult)
		{
			ComPtr<PlaybackSource> source;
			HRESULT hr = MakeAndInitialize<PlaybackSource>(&source, pPlayer);
			if (FAILED(hr))
				return hr;
			return source.CopyTo(ppResult);
		}

	};
	HRESULT PlaybackSource::SetupPropSetWatcher()
	{
		// Create the property set
		HRESULT hr = ABI::Windows::Foundation::ActivateInstance(
			HStringReference(RuntimeClass_Windows_Foundation_Collections_PropertySet).Get(),
			&_spPropSet);

		if (FAILED(hr))
			return hr;

		ComPtr<IObservableMap<HSTRING, IInspectable*>> spObservableMap;
		hr = _spPropSet.As(&spObservableMap);
		if (FAILED(hr))
			return hr;

		auto propertyChangedCallback = Callback<MapChangedEventHandler<HSTRING, IInspectable*>>(
			[this](IInspectable *pSender, IMapChangedEventArgs<HSTRING> *pArgs) -> HRESULT
		{
			HSTRING hsPropertyKey;
			pArgs->get_Key(&hsPropertyKey);

			ComPtr<IMap<HSTRING, IInspectable *>> spMap;
			HRESULT hr = ComPtr<IInspectable>(pSender).As(&spMap);
			if (FAILED(hr))
				return hr;

			ComPtr<IInspectable> spObject;
			hr = spMap->Lookup(hsPropertyKey, &spObject);

			hr = spObject.CopyTo(&_spSource);

			_sourceChangedHandlers.InvokeAll(this, _spSource.Get());

			return hr;
		}
		);
		hr = spObservableMap->add_MapChanged(propertyChangedCallback.Get(), &_propSetChangedToken);
		if (FAILED(hr))
			return hr;

		return S_OK;
	}

	HRESULT PlaybackSource::RuntimeClassInitialize(IMediaPlayer * pPlayer)
	{
		if (pPlayer == nullptr)
			return E_INVALIDARG;

		HRESULT hr = SetupPropSetWatcher();
		if (FAILED(hr))
			return hr;

		ComPtr<IMediaPlayerEffects> playerEffects = As<IMediaPlayerEffects>(pPlayer);

		hr = playerEffects->AddAudioEffect(
				HStringReference(RuntimeClass_AudioVisualizer_MftAnalyzer).Get(),
				false,
				_spPropSet.Get());

		return hr;
	}

	ActivatableClassWithFactory(PlaybackSource, PlaybackSourceFactory);
}
