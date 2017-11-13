#pragma once
#include <wrl.h>
#include <windows.ui.composition.h>
#include <mutex>
#include "Utilities.h"
#include "wrl_util.h"
#include <windows.system.threading.h>
#include <Microsoft.Graphics.Canvas.h>
#include <windows.graphics.display.h>
#include <Windows.ApplicationModel.h>



using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Microsoft::Graphics::Canvas;
using namespace ABI::Windows::UI::Composition;
using namespace ABI::Windows::UI;
using namespace ABI::Windows::UI::Xaml; 
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Windows::UI::Xaml::Hosting;
using namespace ABI::AudioVisualizer;
using namespace ABI::Microsoft::Graphics::Canvas::UI::Composition;
using namespace ABI::Windows::System::Threading;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::ApplicationModel;
using namespace wrl_util;

#define DEFAULT_DPI 96.0f

namespace AudioVisualizer
{
	template<class ControlType>
	class BaseVisualizer
	{
		ComPtr<ABI::Windows::UI::Xaml::IWindow> _window;
		EventRegistrationToken _sizeChangedToken;
		EventRegistrationToken _loadedToken;
		EventRegistrationToken _deviceLostToken;
		EventRegistrationToken _deviceReplacedToken;
		EventRegistrationToken _dpiChangedToken;
		EventRegistrationToken _visibilityChangedToken;
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Composition::ICompositor> _compositor;
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Composition::IContainerVisual> _rootVisual;
		ComPtr<ABI::Windows::UI::Composition::ISpriteVisual> _swapChainVisual;
		ComPtr<ABI::Windows::UI::Composition::ICompositionGraphicsDevice> _compositionGraphicsDevice;

		ComPtr<ICanvasCompositionStatics> _canvasCompositionStatics;
		ComPtr<ICanvasSwapChainFactory> _swapChainFactory;

		CriticalSection _swapChainLock;
		HANDLE _cancelDrawLoop;

	protected:
		ComPtr<IVisualizationSource> _visualizationSource;
		ABI::Windows::UI::Color _drawingSessionBackgroundColor;
		ComPtr<IInspectable> _composableBase;
		bool _bDrawEventActive;
		bool _bIsDesignMode;

		ComPtr<ICanvasSwapChain> _swapChain;
		ComPtr<ICanvasDevice> _device;
		Size _swapChainSize;

		ControlType *GetControl() { return static_cast<ControlType *>(this); };

		float GetLogicalDpi()
		{
			if (_bIsDesignMode)
				return DEFAULT_DPI;
			ComPtr<ABI::Windows::Graphics::Display::IDisplayInformationStatics> displayInfoStatics;
			ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Windows_Graphics_Display_DisplayInformation).Get(), &displayInfoStatics));

			ComPtr<ABI::Windows::Graphics::Display::IDisplayInformation> dispInfo;
			displayInfoStatics->GetForCurrentView(&dispInfo);

			float logicalDpi = 96.0f;

			dispInfo->get_LogicalDpi(&logicalDpi);
			return logicalDpi;
		}

		HRESULT CreateDevice()
		{
			ComPtr<ICanvasDeviceStatics> _deviceStatics;
			ThrowIfFailed(ABI::Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Microsoft_Graphics_Canvas_CanvasDevice).Get(), &_deviceStatics));

			ThrowIfFailed(_deviceStatics->GetSharedDevice(&_device));

			if (_compositionGraphicsDevice == nullptr)
				ThrowIfFailed(_canvasCompositionStatics->CreateCompositionGraphicsDevice(_compositor.Get(), _device.Get(), &_compositionGraphicsDevice));
			else
				ThrowIfFailed(_canvasCompositionStatics->SetCanvasDevice(_compositionGraphicsDevice.Get(), _device.Get()));

			/*_compositionGraphicsDevice->add_RenderingDeviceReplaced(
				Callback<__FITypedEventHandler_2_Windows__CUI__CComposition__CCompositionGraphicsDevice_Windows__CUI__CComposition__CRenderingDeviceReplacedEventArgs>(
					[](ABI::Windows::UI::Composition::ICompositionGraphicsDevice*pDevice, ABI::Windows::UI::Composition::IRenderingDeviceReplacedEventArgs*pArgs) -> HRESULT
			{
				return S_OK;
			}
					).Get(),&_deviceReplacedToken
			
			);*/


			/*ThrowIfFailed(_device->add_DeviceLost(
				Callback<__FITypedEventHandler_2_Microsoft__CGraphics__CCanvas__CCanvasDevice_IInspectable>(
					[](ABI::Microsoft::Graphics::Canvas::ICanvasDevice*, IInspectable *) -> HRESULT
			{
				// TODO: Unregister callback and create a new device on UI thread
				return S_OK;
			}
					).Get(),
				&_deviceLostToken
				));*/

			return S_OK;

		}

		HRESULT InitializeSwapChain()
		{
			ComPtr<IElementCompositionPreviewStatics> spComp;
			ThrowIfFailed(ABI::Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Hosting_ElementCompositionPreview).Get(), &spComp));
			ThrowIfFailed(ABI::Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Microsoft_Graphics_Canvas_CanvasSwapChain).Get(), &_swapChainFactory));
			ThrowIfFailed(ABI::Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Microsoft_Graphics_Canvas_UI_Composition_CanvasComposition).Get(), &_canvasCompositionStatics));

			ComPtr<IVisual> spElementVisual;
			ThrowIfFailed(spComp->GetElementVisual(As<IUIElement>(GetControl()).Get(), &spElementVisual));

			ComPtr<ICompositionObject> spCompObj;
			ThrowIfFailed(spElementVisual.As(&spCompObj));

			ThrowIfFailed(spCompObj->get_Compositor(&_compositor));

			ThrowIfFailed(_compositor->CreateContainerVisual(&_rootVisual));
			ThrowIfFailed(spComp->SetElementChildVisual(As<IUIElement>(GetControl()).Get(), As<IVisual>(_rootVisual).Get()));

			ThrowIfFailed(CreateDevice());

			ThrowIfFailed(_compositor->CreateSpriteVisual(&_swapChainVisual));
			ComPtr<IVisualCollection> children;
			ThrowIfFailed(_rootVisual->get_Children(&children));
			ThrowIfFailed(children->InsertAtTop(As<IVisual>(_swapChainVisual).Get()));
			return S_OK;
		}
		HRESULT CreateSwapChain()
		{
			auto element = As<IFrameworkElement>(GetControl());
			double width = 0, height = 0;
			element->get_ActualWidth(&width);
			element->get_ActualHeight(&height);
			return CreateSwapChainWithSize(Size() = { (float)width, (float)height });
		}
		HRESULT CreateSwapChainWithSize(ABI::Windows::Foundation::Size size)
		{
			float logicalDpi = GetLogicalDpi();
			return CreateSwapChainWithSizeAndDpi(size, logicalDpi);
		}

		HRESULT CreateSwapChainWithSizeAndDpi(ABI::Windows::Foundation::Size size,float dpi)
		{
			auto _lock = _swapChainLock.Lock();

			HRESULT hr = _swapChainFactory->CreateWithWidthAndHeightAndDpi(
				As<ICanvasResourceCreator>(_device).Get(), size.Width, size.Height, dpi, &_swapChain);

			if (FAILED(hr))
				return hr;

			_swapChainSize = size;

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

			return hr;
		}
		HRESULT StartDrawLoop()
		{
			ComPtr<ABI::Windows::System::Threading::IThreadPoolStatics> spThreadPool;
			HRESULT hr = ABI::Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).Get(), &spThreadPool);

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

					if (dwWaitResult != WAIT_TIMEOUT)
						break;

					auto lock = _swapChainLock.Lock();
					if (_swapChain != nullptr && _bDrawEventActive)
					{
						ComPtr<ICanvasDrawingSession> drawingSession;
						
						HRESULT hr = _swapChain->CreateDrawingSession(_drawingSessionBackgroundColor, &drawingSession);
						
						if (FAILED(hr))
						{
							if (DeviceLostException::IsDeviceLostHResult(hr))
							{
								Diagnostics::Trace::Log_DeviceLost(hr);
								RecreateDevice();
								continue;
							}
							else
								continue;
						}

						ComPtr<IVisualizationDataFrame> dataFrame;
						ComPtr<IReference<TimeSpan>> presentationTime;
						if (_visualizationSource != nullptr)
						{
							_visualizationSource->get_PresentationTime(&presentationTime);
							_visualizationSource->GetData(&dataFrame);
						}
						hr = OnDraw(drawingSession.Get(),dataFrame.Get(),presentationTime.Get());
						if (FAILED(hr))
							continue;

						hr = As<IClosable>(drawingSession)->Close();
						if (FAILED(hr))
							continue;

						_swapChain->Present();
						if (FAILED(hr))
							continue;

						auto copy = _swapChain;
						lock.Unlock();
						hr = copy->WaitForVerticalBlank();
					}
					else
					{
						lock.Unlock();
						WaitForSingleObject(cancelEvent, 17);	// If there is no swap chain or draw look is suspended wait for 17ms and retry
					}
				}
				return hr;
			});

			ComPtr<IAsyncAction> asyncAction;
			hr = spThreadPool->RunAsync(drawLoop.Get(), &asyncAction);
			return hr;
		}
		HRESULT RecreateDevice()
		{
			_swapChain = nullptr;
			_compositionGraphicsDevice = nullptr;
			_device = nullptr;

			ComPtr<ABI::Windows::UI::Core::ICoreDispatcher> dispatcher;
			As<IDependencyObject>(GetControl())->get_Dispatcher(&dispatcher);

			ComPtr<ABI::Windows::Foundation::IAsyncAction> action;
			auto callback = Callback<AddFtmBase<ABI::Windows::UI::Core::IDispatchedHandler>::Type>(
				[this]() -> HRESULT
			{
				CreateDevice();
				auto element = As<IFrameworkElement>(GetControl());
				double width = 0, height = 0;
				element->get_ActualWidth(&width);
				element->get_ActualHeight(&height);
				CreateSwapChainWithSize(Size() = { (float)width, (float)height });
				OnCreateResources(ABI::AudioVisualizer::CreateResourcesReason::DeviceLost);
				return S_OK;
			}
				);

			dispatcher->RunAsync(
				ABI::Windows::UI::Core::CoreDispatcherPriority::CoreDispatcherPriority_Normal,
				callback.Get(),
				&action
				);

			return S_OK;
		}

		virtual HRESULT OnDraw(ABI::Microsoft::Graphics::Canvas::ICanvasDrawingSession *pSession,ABI::AudioVisualizer::IVisualizationDataFrame *pDataFrame,IReference<TimeSpan> *pPresentationTime) = 0;
		virtual HRESULT OnCreateResources(ABI::AudioVisualizer::CreateResourcesReason reason) { return S_OK; };

		ABI::Windows::Foundation::Size GetSize()
		{
			auto element = As<IFrameworkElement>(GetControl());
			double width = 0, height = 0;
			element->get_ActualWidth(&width);
			element->get_ActualHeight(&height);
			return ABI::Windows::Foundation::Size() = { (float)width, (float)height };
		}
	public:
		BaseVisualizer()
		{
#ifdef _TRACE
			AudioVisualizer::Diagnostics::Trace::Initialize();
#endif
			_drawingSessionBackgroundColor = Color() = { 0,0,0,0 };
			_swapChainSize.Width = 0.0f;
			_swapChainSize.Height = 0.0f;
			_bDrawEventActive = true;
			
			ComPtr<IDesignModeStatics> designModeStatics;
			ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Windows_ApplicationModel_DesignMode).Get(), &designModeStatics));
			boolean designMode;
			designModeStatics->get_DesignModeEnabled(&designMode);
			_bIsDesignMode = !!designMode;

			CreateBaseControl();
			RegisterEventHandlers();
			InitializeSwapChain();
		}
		~BaseVisualizer()
		{
			auto frameworkElement = As<IFrameworkElement>(GetControl());
			frameworkElement->remove_Loaded(_loadedToken);
			frameworkElement->remove_SizeChanged(_sizeChangedToken);
		}
		

		Microsoft::WRL::ComPtr<IInspectable> GetComposableBase();

	private:
		HRESULT OnLoaded(IInspectable *pSender, IRoutedEventArgs *pArgs)
		{
			OnCreateResources(CreateResourcesReason::New);
			if (!_bIsDesignMode)	// Create the loop and swap panel if not in design mode
			{
				StartDrawLoop();
				CreateSwapChainWithSize(GetSize());
			}
			return S_OK;
		}


 		HRESULT OnSizeChanged(IInspectable *pSender, ISizeChangedEventArgs *pArgs)
		{
			ABI::Windows::Foundation::Size size;		
			pArgs->get_NewSize(&size);
			if (size.Width > 0 && size.Height > 0)
			{
				CreateSwapChainWithSize(size);
			}
			return S_OK;
		}

		void CreateBaseControl()
		{
			ComPtr<IControlFactory> spControlFactory;
			ThrowIfFailed(ABI::Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Control).Get(), &spControlFactory));

			ComPtr<IInspectable> spControlInspectable;
			ComPtr<IControl> spControl;
			ThrowIfFailed(spControlFactory->CreateInstance(As<IInspectable>(GetControl()).Get(), &spControlInspectable, &spControl));
			ThrowIfFailed(GetControl()->SetComposableBasePointers(spControlInspectable.Get()));
		}
		void RegisterEventHandlers()
		{
			ComPtr<IFrameworkElement> frameworkElement = As<IFrameworkElement>(GetControl());
			ThrowIfFailed(frameworkElement->add_Loaded(
				Callback<IRoutedEventHandler>(
					[=](IInspectable *pSender, IRoutedEventArgs *pArgs)->HRESULT
			{
				return OnLoaded(pSender,pArgs);
			}
			).Get(),&_loadedToken));


			ThrowIfFailed(frameworkElement->add_SizeChanged(Callback<ISizeChangedEventHandler>(
				[=](IInspectable *pSender, ISizeChangedEventArgs *pArgs) -> HRESULT
			{
				return OnSizeChanged(pSender, pArgs);
			}).Get(),&_sizeChangedToken));

			if (!_bIsDesignMode)
			{
				ComPtr<ABI::Windows::Graphics::Display::IDisplayInformationStatics> displayInfoStatics;
				ThrowIfFailed(GetActivationFactory(HStringReference(RuntimeClass_Windows_Graphics_Display_DisplayInformation).Get(), &displayInfoStatics));

				ComPtr<ABI::Windows::Graphics::Display::IDisplayInformation> dispInfo;
				displayInfoStatics->GetForCurrentView(&dispInfo);

				dispInfo->add_DpiChanged(
					Callback<__FITypedEventHandler_2_Windows__CGraphics__CDisplay__CDisplayInformation_IInspectable>(
						[this](ABI::Windows::Graphics::Display::IDisplayInformation *pInfo, IInspectable *pObj) ->HRESULT
				{
					float newDpi;
					pInfo->get_LogicalDpi(&newDpi);
					CreateSwapChainWithSizeAndDpi(GetSize(), newDpi);
					OnCreateResources(ABI::AudioVisualizer::CreateResourcesReason::DpiChanged);

					return S_OK;
				}
						).Get(),
					&_dpiChangedToken
					);
			}
			ComPtr<IUIElementStatics> uies;
			GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_UIElement).Get(), &uies);
			ComPtr<IDependencyProperty> visibilityProperty;
			uies->get_VisibilityProperty(&visibilityProperty);

			ComPtr<IDependencyObject2> dpObj;
			dpObj = As<IDependencyObject2>(GetControl());

			auto visibilityChangedCallback = Make<DependencyPropertyChangedCallbackImpl>(
				Callback<ITypedEventHandler<IInspectable*, IInspectable*>>(
					[this](IInspectable *pObject, IInspectable *pProp) -> HRESULT
			{
				Visibility visibility;
				HRESULT hr = As<IUIElement>(GetControl())->get_Visibility(&visibility);
				if (SUCCEEDED(hr))
				{
					_bDrawEventActive = visibility == Visibility::Visibility_Visible;
					return S_OK;
				}
				return hr;
			}
					).Get()	);

			INT64 regPropReturnValue = 0;
			dpObj->RegisterPropertyChangedCallback(visibilityProperty.Get(),
				visibilityChangedCallback.Get(),
				&regPropReturnValue
			);
		}
	};
}

