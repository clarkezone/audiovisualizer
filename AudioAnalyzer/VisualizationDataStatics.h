#pragma once
#include <wrl.h>
#include <wrl/Async.h>
#include <windows.foundation.h>
#include "AudioAnalyzer_h.h"
#include "AudioAnalyzer.h"

namespace AudioAnalyzer
{
	
	class CVisualizationDataStatics : public Microsoft::WRL::ActivationFactory<ABI::AudioAnalyzer::IVisualizationDataStatics>
	{
		InspectableClassStatic(RuntimeClass_AudioAnalyzer_VisualizationData, BaseTrust);
	public:
		CVisualizationDataStatics();
		~CVisualizationDataStatics();
		STDMETHODIMP ActivateInstance(_Outptr_result_nullonfailure_ IInspectable** ppvObject);
		STDMETHODIMP CreateVisualizationSourceAsync(IInspectable *pObject,ABI::Windows::Foundation::IAsyncOperation<ABI::AudioAnalyzer::IVisualizationSource *> **pSource);
	};


	// Helper for marking our callback delegates as agile, by mixing in FtmBase.
	// Without this WinRT would marshal everything back to the UI thread.
	template<typename T>
	struct AddFtmBase
	{
		typedef Microsoft::WRL::Implements<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, T, Microsoft::WRL::FtmBase> Type;
	};

	class CCreateSourceAsyncOperation : public
		Microsoft::WRL::RuntimeClass<
			Microsoft::WRL::AsyncBase<ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::AudioAnalyzer::IVisualizationSource *>>,
			ABI::Windows::Foundation::IAsyncOperation<ABI::AudioAnalyzer::IVisualizationSource *>>
	{
		InspectableClass(ABI::Windows::Foundation::IAsyncOperation<ABI::AudioAnalyzer::IVisualizationSource *>::z_get_rc_name_impl(), BaseTrust);

			Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::IPropertySet> _spPropSet;
			Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Collections::MapChangedEventHandler<HSTRING, IInspectable*>> _propChangedCallback;
			EventRegistrationToken _evToken;
			Microsoft::WRL::ComPtr<ABI::AudioAnalyzer::IVisualizationSource> _spSource;
			HANDLE _hWaitEvent;


	public:
		CCreateSourceAsyncOperation();
		~CCreateSourceAsyncOperation();
		HRESULT RuntimeClassInitialize(IInspectable *pHostObject);
	protected:
		virtual STDMETHODIMP OnStart();
		virtual void OnCancel()
		{
		}
		virtual void OnClose()
		{
			_spSource = nullptr;
		}
		virtual STDMETHODIMP GetResults(ABI::AudioAnalyzer::IVisualizationSource **ppSource)
		{
			HRESULT hr = CheckValidStateForResultsCall();
			if (FAILED(hr))
			{
				return hr;
			}
			return _spSource.CopyTo(ppSource);
		}
		// Sets the completion callback. If the async operation has already completed, the handler will be called straight away.
		virtual HRESULT STDMETHODCALLTYPE put_Completed(ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::AudioAnalyzer::IVisualizationSource *> *handler)
		{
			return PutOnComplete(handler);
		}

		// Gets the completion callback.
		virtual HRESULT STDMETHODCALLTYPE get_Completed(ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::AudioAnalyzer::IVisualizationSource *>** handler)
		{
			return GetOnComplete(handler);
		}
		
	};
}

