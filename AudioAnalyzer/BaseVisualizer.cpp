#include "pch.h"
#include "BaseVisualizer.h"
#include <windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.h>
#include "utilities.h"
#include "ErrorHandling.h"
#include <stdio.h>

using namespace ABI::Windows::UI::Xaml::Hosting;
using namespace ABI::Windows::UI::Composition;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::Core;

namespace AudioAnalyzer
{
	ActivatableClass(CBaseVisualizer);

	HRESULT CBaseVisualizer::OnSizeChanged(const ABI::Windows::Foundation::Size & size)
	{
		return S_OK;
	}

	CBaseVisualizer::CBaseVisualizer()
	{
	}


	CBaseVisualizer::~CBaseVisualizer()
	{
		_window->remove_SizeChanged(_sizeChangedToken);
	}

	void DumpInterfaces(IInspectable *pObject)
	{
		ULONG ulCount = 0;
		IID *pIids = nullptr;

		pObject->GetIids(&ulCount, &pIids);
		for (size_t index = 0; index < ulCount; index++)
		{
			wchar_t szMessage[1024];
			unsigned short *pData = (unsigned short *) pIids[index].Data4;
			swprintf_s(szMessage, L"{%08X-%04X-%04X-%04X-%04X%04X%04X}\n", 
				pIids[index].Data1,
				pIids[index].Data2, 
				pIids[index].Data3,
				pData[0], pData[1], pData[2], pData[3]
			);
			OutputDebugString(szMessage);
		}
		CoTaskMemFree(pIids);
	}

	HRESULT CBaseVisualizer::RuntimeClassInitialize()
	{
		ComPtr<IElementCompositionPreviewStatics> spComp = GetStatics<IElementCompositionPreviewStatics>(HStringReference(RuntimeClass_Windows_UI_Xaml_Hosting_ElementCompositionPreview).Get());

		// Create control
		ComPtr<IControlFactory> spControlFactory;
		HRESULT hr = GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Control).Get(), &spControlFactory);

		ComPtr<IControl> spInstance;
		hr = spControlFactory->CreateInstance(this, &_control, &spInstance);

		SetComposableBasePointers(_control.Get());

		OutputDebugString(L"IIDs of this\n");
		DumpInterfaces(this);
		OutputDebugString(L"IIDs of control\n");
		DumpInterfaces(_control.Get());


		ComPtr<IVisual> spElementVisual;
		ThrowIfFailed(spComp->GetElementVisual(As<IUIElement>(_control.Get()).Get(), &spElementVisual));

		ComPtr<ICompositionObject> spCompObj;
		ThrowIfFailed(spElementVisual.As(&spCompObj));
		
		ThrowIfFailed(spCompObj->get_Compositor(&_compositor));

		ThrowIfFailed(_compositor->CreateContainerVisual(&_rootVisual));

		auto spWindow = GetStatics<IWindowStatics>(HStringReference(RuntimeClass_Windows_UI_Xaml_Window).Get());
		hr = spWindow->get_Current(&_window);

		hr = _window->add_SizeChanged(Callback<IWindowSizeChangedEventHandler>(
			[=] (IInspectable *pSender, IWindowSizeChangedEventArgs *pArgs) -> HRESULT
			{
				Size size;
				HRESULT hr = pArgs->get_Size(&size);
				if (SUCCEEDED(hr))
					return OnSizeChanged(size);
				return hr;
			}
			).Get(), 
			&_sizeChangedToken);

		return S_OK;
	}
	STDMETHODIMP CBaseVisualizer::add_Draw(Visualizer_DrawEventHandler * value, EventRegistrationToken * token)
	{
		return _drawEventList.Add(value, token);
	}
	STDMETHODIMP CBaseVisualizer::remove_Draw(EventRegistrationToken token)
	{
		return _drawEventList.Remove(token);
	}
}
