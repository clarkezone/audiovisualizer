#include "pch.h"
#include "BaseVisualizer.h"
#include <windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.h>
#include "utilities.h"
#include "ErrorHandling.h"

using namespace ABI::Windows::UI::Xaml::Hosting;
using namespace ABI::Windows::UI::Composition;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::UI::Xaml;

namespace AudioAnalyzer
{
	ActivatableClass(CBaseVisualizer);

	CBaseVisualizer::CBaseVisualizer()
	{
	}


	CBaseVisualizer::~CBaseVisualizer()
	{
	}

	HRESULT CBaseVisualizer::RuntimeClassInitialize()
	{
		ComPtr<IElementCompositionPreviewStatics> spComp = GetStatics<IElementCompositionPreviewStatics>(HStringReference(RuntimeClass_Windows_UI_Xaml_Hosting_ElementCompositionPreview).Get());

		ComPtr<IUIElement> spUIElement = As<IUIElement>(this);

		/*
		ComPtr<IVisual> spElementVisual;
		ThrowIfFailed(spComp->GetElementVisual(spUIElement.Get(), &spElementVisual));

		ComPtr<ICompositionObject> spCompObj;
		ThrowIfFailed(spElementVisual.As(&spCompObj));
		
		ThrowIfFailed(spCompObj->get_Compositor(&_compositor));

		ThrowIfFailed(_compositor->CreateContainerVisual(&_rootVisual));
		*/
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
