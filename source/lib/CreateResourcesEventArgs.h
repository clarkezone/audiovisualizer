#pragma once
#include "AudioVisualizer.abi.h"


using namespace Microsoft::WRL;

namespace AudioVisualizer
{
	class CreateResourcesEventArgsImpl : public RuntimeClass <ABI::AudioVisualizer::ICreateResourcesEventArgs>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_CreateResourcesEventArgs, BaseTrust);

		ABI::AudioVisualizer::CreateResourcesReason _reason;
		ComPtr<IInspectable> _creatorObject;
	public:

		CreateResourcesEventArgsImpl(ABI::AudioVisualizer::CreateResourcesReason reason, IInspectable *pCreatorObject)
		{
			_reason = reason;
			_creatorObject = pCreatorObject;
		}

		STDMETHODIMP get_Reason(ABI::AudioVisualizer::CreateResourcesReason *pReason)
		{
			if (pReason == nullptr)
				return E_POINTER;
			*pReason = _reason;
			return S_OK;
		}
		STDMETHODIMP get_ResourceCreator(IInspectable **ppObject)
		{
			if (ppObject == nullptr)
				return E_POINTER;
			return _creatorObject.CopyTo(ppObject);
		}
	};
}

