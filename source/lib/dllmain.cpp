#include "pch.h"

using namespace Microsoft::WRL;

extern "C" {
	HRESULT WINAPI DllGetActivationFactory(_In_ HSTRING activatibleClassId, _Outptr_ IActivationFactory** factory)
	{
		auto &module = Module< InProc >::GetModule();
		return module.GetActivationFactory(activatibleClassId, factory);
	}

	STDAPI DllCanUnloadNow()
	{
		auto &module = Module<InProc>::GetModule();
		return module.Terminate() ? S_OK : S_FALSE;
	}

	STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
	{
		auto &module = Module<InProc>::GetModule();
		return module.GetClassObject(rclsid, riid, ppv);
	}
}

