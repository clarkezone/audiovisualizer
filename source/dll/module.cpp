#include "pch.h"
#include <wrl/module.h>

using namespace Microsoft::WRL;
extern void InitializeTrace();

BOOL APIENTRY DllMain(HMODULE /* hModule */, DWORD ul_reason_for_call, LPVOID /* lpReserved */)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// TODO: Initialize trace
		InitializeTrace();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		// TODO: Shutdown trace
		
		break;
	}
	return TRUE;
}

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
