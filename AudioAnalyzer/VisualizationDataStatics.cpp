#include "pch.h"
#include <wrl.h>
#include "AudioAnalyzer.h"
#include "AudioAnalyzer_h.h"
#include "VisualizationData.h"
#include "VisualizationDataStatics.h"
#include <windows.ui.xaml.controls.h>
#include <windows.system.threading.h>

namespace AudioAnalyzer
{
	using namespace Microsoft::WRL;
	using namespace Microsoft::WRL::Wrappers;
	using namespace ABI::Windows::Foundation;
	using namespace ABI::AudioAnalyzer;

	ActivatableClassWithFactory(CVisualizationData, CVisualizationDataStatics);

	CVisualizationDataStatics::CVisualizationDataStatics()
	{
	}

	CVisualizationDataStatics::~CVisualizationDataStatics()
	{
	}
	STDMETHODIMP CVisualizationDataStatics::ActivateInstance(IInspectable ** ppvObject)
	{
		return MakeAndInitialize<CVisualizationData>(ppvObject);
	}
	STDMETHODIMP CVisualizationDataStatics::CreateVisualizationSourceAsync(IInspectable * pObject, ABI::Windows::Foundation::IAsyncOperation<ABI::AudioAnalyzer::IVisualizationSource *> **ppAsyncOp)
	{
		ComPtr<CCreateSourceAsyncOperation> op;
		HRESULT hr = MakeAndInitialize<CCreateSourceAsyncOperation>(&op,pObject);
		if (FAILED(hr))
			return hr;

		hr = op.CopyTo(ppAsyncOp);
		return hr;
	}

	CCreateSourceAsyncOperation::CCreateSourceAsyncOperation()
	{
		_hWaitEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
		Start();
	}

	CCreateSourceAsyncOperation::~CCreateSourceAsyncOperation()
	{
		CloseHandle(_hWaitEvent);
	}

	HRESULT CCreateSourceAsyncOperation::RuntimeClassInitialize(IInspectable * pHostObject)
	{
		using namespace ABI::Windows::UI::Xaml::Controls;
		using namespace ABI::Windows::Foundation::Collections;

		if (pHostObject == nullptr)
			return E_INVALIDARG;

		IMediaElement *pMediaElement = nullptr;
		HRESULT hr = pHostObject->QueryInterface(&pMediaElement);

		if (FAILED(hr))
			return E_INVALIDARG;

		hr = ABI::Windows::Foundation::ActivateInstance(
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

		hr = pMediaElement->AddAudioEffect(
		HStringReference(RuntimeClass_AudioAnalyzer_AnalyzerEffect).Get(),
		false,
		_spPropSet.Get());

		if (FAILED(hr))
			return hr;

		return hr;
	}

	HRESULT CCreateSourceAsyncOperation::OnStart()
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
			[this] (IAsyncAction *) -> HRESULT
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
		return spThreadPool->RunAsync(threadPoolDelegate.Get(),&spAction);
	}

}
