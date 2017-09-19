#include "pch.h"
#include "BaseVisualizer.h"
#include <windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.h>
#include "utilities.h"
#include "ErrorHandling.h"
#include <stdio.h>
#include <Microsoft.Graphics.Canvas.h>
#include <wrl.h>
#include <Nullable.h>
#include <windows.system.threading.h>
#include "VisualizationDataFrame.h"
#include "VisualizerDrawEventArgs.h"
#include "trace.h"
#include <crtdbg.h>

using namespace ABI::Windows::UI::Xaml::Hosting;
using namespace ABI::Windows::UI::Composition;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Microsoft::Graphics::Canvas;
using namespace ABI::Microsoft::Graphics::Canvas::UI::Composition;
using namespace ABI::Windows::System::Threading;
using namespace std;
namespace AudioVisualizer
{
	class BaseVisualizerFactory : public AgileActivationFactory<>
	{
	public:
		IFACEMETHODIMP ActivateInstance(IInspectable** obj) override
		{
			return ExceptionBoundary(
				[&]
			{
				CheckAndClearOutPointer(obj);
				auto control = Make<BaseVisualizer>();
				CheckMakeResult(control);
				ThrowIfFailed(control.CopyTo(obj));
			});
		}
	};

	BaseVisualizer::BaseVisualizer() :
		_cancelDrawLoop(INVALID_HANDLE_VALUE),
		_backgroundColor(Color() = { 0, 0, 0, 0 })
	{
#ifdef _TRACE
		AudioVisualizer::Diagnostics::Trace::Initialize();
#endif

		ComPtr<IElementCompositionPreviewStatics> spComp;

		ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Hosting_ElementCompositionPreview).Get(), &spComp));
		ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Microsoft_Graphics_Canvas_CanvasDevice).Get(), &_deviceStatics));
		ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Microsoft_Graphics_Canvas_UI_Composition_CanvasComposition).Get(), &_canvasCompositionStatics));
		ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Microsoft_Graphics_Canvas_CanvasSwapChain).Get(), &_swapChainFactory));

		// Create base class
		ComPtr<IControlFactory> spControlFactory;
		ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Control).Get(), &spControlFactory));

		ComPtr<IInspectable> spControlInspectable;
		ComPtr<IControl> spControl;
		ThrowIfFailed(spControlFactory->CreateInstance(this, &spControlInspectable, &spControl));
		ThrowIfFailed(SetComposableBasePointers(spControlInspectable.Get()));


		ComPtr<IVisual> spElementVisual;
		ThrowIfFailed(spComp->GetElementVisual(As<IUIElement>(this).Get(), &spElementVisual));

		ComPtr<ICompositionObject> spCompObj;
		ThrowIfFailed(spElementVisual.As(&spCompObj));

		ThrowIfFailed(spCompObj->get_Compositor(&_compositor));

		ThrowIfFailed(_compositor->CreateContainerVisual(&_rootVisual));
		ThrowIfFailed(spComp->SetElementChildVisual(As<IUIElement>(this).Get(), As<IVisual>(_rootVisual).Get()));

		ComPtr<IWindowStatics> spWindow;
		ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Window).Get(), &spWindow));
		ThrowIfFailed(spWindow->get_Current(&_window));

		ComPtr<IFrameworkElement> frameworkElement = As<IFrameworkElement>(this);
		ThrowIfFailed(frameworkElement->add_Loaded(
			Callback<IRoutedEventHandler>(
				[=](IInspectable *pSender, IRoutedEventArgs *pArgs)->HRESULT
		{
			return OnLoaded();
		}
		).Get(), &_loadedToken));

		ThrowIfFailed(_window->add_SizeChanged(Callback<IWindowSizeChangedEventHandler>(
			[=](IInspectable *pSender, IWindowSizeChangedEventArgs *pArgs) -> HRESULT
		{
			Size size;
			HRESULT hr = pArgs->get_Size(&size);
			if (SUCCEEDED(hr))
				return OnSizeChanged(size);
			return hr;
		}).Get(),
			&_sizeChangedToken));

		ThrowIfFailed(CreateDevice());


		ThrowIfFailed(_compositor->CreateSpriteVisual(&_swapChainVisual));
		ComPtr<IVisualCollection> children;
		ThrowIfFailed(_rootVisual->get_Children(&children));
		ThrowIfFailed(children->InsertAtTop(As<IVisual>(_swapChainVisual).Get()));
	}


	BaseVisualizer::~BaseVisualizer()
	{
		As<IFrameworkElement>(this)->remove_Loaded(_loadedToken);
		_window->remove_SizeChanged(_sizeChangedToken);
	}

	HRESULT BaseVisualizer::CreateDevice()
	{
		ThrowIfFailed(_deviceStatics->GetSharedDevice(&_device));

		ThrowIfFailed(_device->add_DeviceLost(
			Callback<__FITypedEventHandler_2_Microsoft__CGraphics__CCanvas__CCanvasDevice_IInspectable>(
				[] (ABI::Microsoft::Graphics::Canvas::ICanvasDevice*,IInspectable *) -> HRESULT
				{
					// TODO: Unregister callback and create a new device on UI thread
					return E_NOTIMPL;
				}
				).Get(),
				&_deviceLostToken
		));

		
		if (_compositionGraphicsDevice == nullptr)
			ThrowIfFailed(_canvasCompositionStatics->CreateCompositionGraphicsDevice(_compositor.Get(), _device.Get(), &_compositionGraphicsDevice));
		else
			ThrowIfFailed(_canvasCompositionStatics->SetCanvasDevice(_compositionGraphicsDevice.Get(), _device.Get()));
			
		return S_OK;
	}

	HRESULT BaseVisualizer::OnDraw()
	{
		TimeSpan frameTime = { -1 };
		HRESULT hr = S_OK;
		if (_swapChain != nullptr)
		{
			ComPtr<ICanvasDrawingSession> drawingSession;
			HRESULT hr = _swapChain->CreateDrawingSession(_backgroundColor, &drawingSession);
			if (SUCCEEDED(hr))
			{
				ComPtr<IVisualizationDataFrame> dataFrame;
				if (_source != nullptr)
				{
					hr = _source->GetData(&dataFrame);	// This call will cause addref to dataFrame
				}

				auto args = Make<VisualizerDrawEventArgs>(drawingSession.Get(), nullptr /*dataFrame.Get()*/);					
				{
#ifdef _TRACE
					ComPtr<ILoggingActivity> activity;
					Diagnostics::Trace::Log_StartDraw(dataFrame.Get(), &activity);
					AudioVisualizer::Diagnostics::CLogActivityHelper drawActivity(activity.Get());
#endif
					ThrowIfFailed(_drawEventList.InvokeAll(this, args.Get()));
				}
				As<IClosable>(drawingSession)->Close();
				if (dataFrame != nullptr)
				{
					As<IClosable>(dataFrame)->Close();
				}
				_swapChain->Present();
			}
		}


		return hr;
	}

	HRESULT BaseVisualizer::OnSizeChanged(const ABI::Windows::Foundation::Size & size)
	{
		if (size.Width > 0 && size.Height > 0)
		{
			unique_lock<mutex> swapChainLock(_mtxSwapChain);
			HRESULT hr =_swapChainFactory->CreateWithWidthAndHeightAndDpi(
				As<ICanvasResourceCreator>(_device).Get(), size.Width, size.Height, 96.0f, &_swapChain);

			if (FAILED(hr))
				return hr;
			
			ComPtr<ICompositionSurface> spCompSurface;
			hr = _canvasCompositionStatics->CreateCompositionSurfaceForSwapChain(_compositor.Get(), _swapChain.Get(), &spCompSurface);
			if (FAILED(hr))
				return hr;
			ComPtr<ICompositionSurfaceBrush> spSurfaceBrush;
			hr = _compositor->CreateSurfaceBrushWithSurface(spCompSurface.Get(), &spSurfaceBrush);
			if (FAILED(hr))
				return hr;
			hr = _swapChainVisual->put_Brush(As<ICompositionBrush>(spSurfaceBrush).Get());
			if (FAILED(hr))
				return hr;
			hr = As<IVisual>(_swapChainVisual)->put_Size(ABI::Windows::Foundation::Numerics::Vector2() = { size.Width, size.Height });
			if (FAILED(hr))
				return hr;
		}
		return S_OK;
	}
	HRESULT BaseVisualizer::OnLoaded()
	{
		Rect bounds;
		_window->get_Bounds(&bounds);
		OnSizeChanged(Size() = { bounds.Width,bounds.Height });
		StartDrawLoop();
		return S_OK;
	}

	HRESULT BaseVisualizer::StartDrawLoop()
	{
		ComPtr<IThreadPoolStatics> spThreadPool;
		HRESULT hr = GetActivationFactory(HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).Get(), &spThreadPool);

		if (FAILED(hr))
			return hr;

		typedef AddFtmBase<IWorkItemHandler>::Type CallbackType;

		HANDLE cancelEvent = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
		_cancelDrawLoop = cancelEvent;

		auto drawLoop = Callback<CallbackType>(
			[=](IAsyncAction *) -> HRESULT
		{
			HRESULT hr = S_OK;
			SetThreadDescription(GetCurrentThread(), L"Visualizer draw loop");
			while (SUCCEEDED(hr))
			{
				DWORD dwWaitResult = WaitForSingleObject(cancelEvent, 0);

				if  (dwWaitResult != WAIT_TIMEOUT)
					break;

				_mtxSwapChain.lock();
				if (_swapChain != nullptr)
				{
					OnDraw();
					auto copy = _swapChain;
					_mtxSwapChain.unlock();
					hr = copy->WaitForVerticalBlank();
				}
				else
				{
					_mtxSwapChain.unlock();
					WaitForSingleObject(cancelEvent, 17);
				}
			}
			return hr;
		});

		ComPtr<IAsyncAction> asyncAction;
		hr = spThreadPool->RunAsync(drawLoop.Get(), &asyncAction);
		return hr;
	}





	STDMETHODIMP BaseVisualizer::add_Draw(Visualizer_DrawEventHandler * value, EventRegistrationToken * token)
	{
		return _drawEventList.Add(value, token);
	}
	STDMETHODIMP BaseVisualizer::remove_Draw(EventRegistrationToken token)
	{
		return _drawEventList.Remove(token);
	}

	ActivatableClassWithFactory(BaseVisualizer, BaseVisualizerFactory);

}
