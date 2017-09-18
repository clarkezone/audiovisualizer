#include "pch.h"
#include "VisualizationSource.h"
#include <windows.system.threading.h>
#include <wrl.h>
#include "LifeSpanTracker.h"
#include "Utilities.h"
#include <windows.media.playback.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Media::Playback;

namespace AudioVisualizer
{
	class CreateSourceAsyncOperation : public
		RuntimeClass<AsyncBase<IAsyncOperationCompletedHandler<IVisualizationSource *>>, IAsyncOperation<IVisualizationSource *>>
	{
		InspectableClass(IAsyncOperation<IVisualizationSource *>::z_get_rc_name_impl(), BaseTrust);

		Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IPropertySet> _spPropSet;
		Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::MapChangedEventHandler<HSTRING, IInspectable*>> _propChangedCallback;
		EventRegistrationToken _evToken;
		Microsoft::WRL::ComPtr<ABI::AudioVisualizer::IVisualizationSource> _spSource;
		HANDLE _hWaitEvent;


	public:
		CreateSourceAsyncOperation()
		{
			_hWaitEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
			Start();
		}
		~CreateSourceAsyncOperation()
		{
			CloseHandle(_hWaitEvent);
		}
		HRESULT RuntimeClassInitialize(IInspectable *pObject)
		{
			using namespace ABI::Windows::UI::Xaml::Controls;
			using namespace ABI::Windows::Foundation::Collections;


			if (pObject == nullptr)
				return E_POINTER;
			ComPtr<IInspectable> obj = pObject;

			ComPtr<IMediaElement> mediaElement;
			ComPtr<IMediaPlayerEffects> mediaPlayer;
			if (
				FAILED(obj.As(&mediaPlayer)) &&
				FAILED(obj.As(&mediaElement))
				)
			{
				return E_INVALIDARG;
			}

			HRESULT hr = ABI::Windows::Foundation::ActivateInstance(
				HStringReference(RuntimeClass_Windows_Foundation_Collections_PropertySet).Get(),
				&_spPropSet);

			if (FAILED(hr))
				return hr;

			ComPtr<IObservableMap<HSTRING, IInspectable*>> spObservableMap;
			hr = _spPropSet.As(&spObservableMap);
			if (FAILED(hr))
				return hr;

			this->AddRef();

			_propChangedCallback = Callback<MapChangedEventHandler<HSTRING, IInspectable*>>(
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

				SetEvent(_hWaitEvent);
				return hr;
			}
			);
			hr = spObservableMap->add_MapChanged(_propChangedCallback.Get(), &_evToken);
			if (FAILED(hr))
				return hr;

			if (mediaPlayer != nullptr)
			{
				hr = mediaPlayer->AddAudioEffect(
					HStringReference(RuntimeClass_AudioVisualizer_MftAnalyzer).Get(),
					false,
					_spPropSet.Get());
			}
			else if (mediaElement != nullptr)
			{
				hr = mediaElement->AddAudioEffect(
					HStringReference(RuntimeClass_AudioVisualizer_MftAnalyzer).Get(),
					false,
					_spPropSet.Get());
			}
			else
				return E_FAIL;

			if (FAILED(hr))
				return hr;

			return hr;

		}
	protected:
		virtual HRESULT OnStart()
		{
			using namespace ABI::Windows::System::Threading;
			using namespace ABI::Windows::Foundation;
			using namespace ABI::Windows::Foundation::Collections;

			ComPtr<IThreadPoolStatics> spThreadPool;
			HRESULT hr = GetActivationFactory(HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).Get(), &spThreadPool);

			if (FAILED(hr))
				return hr;

			typedef AddFtmBase<IWorkItemHandler>::Type CallbackType;

			auto threadPoolDelegate = Callback<CallbackType>(
				[this](IAsyncAction *) -> HRESULT
			{
				HRESULT hr = S_OK;
				ComPtr<IObservableMap<HSTRING, IInspectable*>> spObservableMap;

				DWORD dwWaitResult = WaitForSingleObject(_hWaitEvent, INFINITE);

				if (dwWaitResult != WAIT_OBJECT_0)
				{
					hr = E_FAIL;
					goto exit;
				}

				hr = _spPropSet.As(&spObservableMap);
				if (FAILED(hr))
					goto exit;

				hr = spObservableMap->remove_MapChanged(_evToken);
				if (FAILED(hr))
					goto exit;
			exit:
				if (SUCCEEDED(hr))
					FireCompletion();
				else
					TryTransitionToError(hr);

				return S_OK;
			});

			ComPtr<IAsyncAction> spAction;

			return spThreadPool->RunAsync(threadPoolDelegate.Get(), &spAction);
		}
		virtual void OnCancel()
		{
		}
		virtual void OnClose()
		{
			_spSource = nullptr;
		}
		virtual STDMETHODIMP GetResults(ABI::AudioVisualizer::IVisualizationSource **ppSource)
		{
			HRESULT hr = CheckValidStateForResultsCall();
			if (FAILED(hr))
			{
				return hr;
			}
			return _spSource.CopyTo(ppSource);
		}
		// Sets the completion callback. If the async operation has already completed, the handler will be called straight away.
		virtual HRESULT STDMETHODCALLTYPE put_Completed(IAsyncOperationCompletedHandler<IVisualizationSource *> *handler)
		{
			return PutOnComplete(handler);
		}

		// Gets the completion callback.
		virtual HRESULT STDMETHODCALLTYPE get_Completed(IAsyncOperationCompletedHandler<IVisualizationSource *>** handler)
		{
			return GetOnComplete(handler);
		}

	};

	// Dummy class for statics factory
	class VisualizationSource : public RuntimeClass<IInspectable>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizationSource, BaseTrust);
	public:
	};

	ActivatableClassWithFactory(VisualizationSource, VisualizationSourceStatics);

	STDMETHODIMP VisualizationSourceStatics::ActivateInstance(IInspectable ** ppvObject)
	{
		auto source = Make<VisualizationSource>();
		source.CopyTo(ppvObject);
		return S_OK;
	}

	STDMETHODIMP VisualizationSourceStatics::CreateFromMediaPlayerAsync(IMediaPlayer * pElement, IAsyncOperation<IVisualizationSource*>** ppAsyncOp)
	{
		ComPtr<CreateSourceAsyncOperation> op;
		HRESULT hr = MakeAndInitialize<CreateSourceAsyncOperation>(&op, pElement);
		if (FAILED(hr))
			return hr;

		hr = op.CopyTo(ppAsyncOp);
		return hr;
	}

	STDMETHODIMP VisualizationSourceStatics::CreateFromMediaElementAsync(IMediaElement * pElement, IAsyncOperation<IVisualizationSource*>** ppAsyncOp)
	{
		ComPtr<CreateSourceAsyncOperation> op;
		HRESULT hr = MakeAndInitialize<CreateSourceAsyncOperation>(&op, pElement);
		if (FAILED(hr))
			return hr;

		hr = op.CopyTo(ppAsyncOp);
		return hr;
	}

	STDMETHODIMP VisualizationSourceStatics::CreateFromAudioNode(IAudioNode * pNode, IVisualizationSource ** pSource)
	{
		return E_NOTIMPL;
	}


}