#include "pch.h"
#include "VisualizerControl.h"
#include "Tracing.h"
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>
#include <winrt/Windows.Graphics.Display.h>
#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.h>

namespace winrt::AudioVisualizer::implementation
{

	AudioVisualizer::IVisualizationSource VisualizerControl::Source()
	{
		return _visualizationSource;
	}

	void VisualizerControl::Source(AudioVisualizer::IVisualizationSource const& value)
	{
		_visualizationSource = value;
	}

	Windows::UI::Color VisualizerControl::BackgroundColor()
	{
		return _drawingSessionClearColor;
	}

	void VisualizerControl::BackgroundColor(Windows::UI::Color const& value)
	{
		_drawingSessionClearColor = value;
	}

	void VisualizerControl::CreateDevice()
	{
		using namespace Microsoft::Graphics::Canvas;
		using namespace Microsoft::Graphics::Canvas::UI::Composition;

		_device = CanvasDevice::GetSharedDevice();
		if (!_compositionGraphicsDevice) {
			CanvasComposition::CreateCompositionGraphicsDevice(_compositor, _device);
		}
		else {
			CanvasComposition::SetCanvasDevice(_compositionGraphicsDevice, _device);
		}	
	}
	void VisualizerControl::RecreateDevice()
	{
		_swapChain = nullptr;
		_compositionGraphicsDevice = nullptr;
		_device = nullptr;

		Dispatcher().RunAsync(
			Windows::UI::Core::CoreDispatcherPriority::Normal,
			Windows::UI::Core::DispatchedHandler(
				[this] {
				CreateDevice();
				Windows::Foundation::Size size;
				size.Width = (float)ActualWidth();
				size.Height = (float)ActualHeight();
				CreateSwapChainWithSize(size);
				OnCreateResources(_swapChain, CreateResourcesReason::DeviceLost);
			}
			));
	}
	void VisualizerControl::InitializeSwapChain()
	{
		using namespace Windows::UI::Xaml::Hosting;
		auto elementVisual = ElementCompositionPreview::GetElementVisual(*this);
		_compositor = elementVisual.Compositor();
		_rootVisual = _compositor.CreateContainerVisual();
		ElementCompositionPreview::SetElementChildVisual(*this, _rootVisual);

		CreateDevice();

		_swapChainVisual = _compositor.CreateSpriteVisual();
		_rootVisual.Children().InsertAtTop(_swapChainVisual);
	}

	void VisualizerControl::CreateSwapChainWithSizeAndDpi(Windows::Foundation::Size size, float dpi)
	{
		std::lock_guard<std::mutex> _lock(_swapChainLock);

		_swapChain = Microsoft::Graphics::Canvas::CanvasSwapChain::CanvasSwapChain(_device, size.Width, size.Height, dpi);
		_swapChainSize = size;

		_compositionSurface = Microsoft::Graphics::Canvas::UI::Composition::CanvasComposition::CreateCompositionSurfaceForSwapChain(_compositor, _swapChain);
		_surfaceBrush = _compositor.CreateSurfaceBrush(_compositionSurface);

		_swapChainVisual.Brush(_surfaceBrush);
		Windows::Foundation::Numerics::float2 swapChainSize;
		swapChainSize.x = size.Width;
		swapChainSize.y = size.Height;
		_swapChainVisual.Size(swapChainSize);
	}
	void VisualizerControl::CreateSwapChainWithSize(Windows::Foundation::Size size)
	{
		float logicalDpi = Windows::Graphics::Display::DisplayInformation::GetForCurrentView().LogicalDpi();
		CreateSwapChainWithSizeAndDpi(size, logicalDpi);
	}

	void VisualizerControl::DrawLoop(Windows::Foundation::IAsyncAction const& /*action*/)
	{
		SetThreadDescription(GetCurrentThread(), L"VisualizerControl draw loop");
		while (TRUE)
		{
			DWORD dwWaitResult = WaitForSingleObject(_cancelDrawLoop, 0);

			if (dwWaitResult != WAIT_TIMEOUT)
				break;

			Microsoft::Graphics::Canvas::CanvasSwapChain waitableSwapChain{ nullptr };
			if (true)	// Scope for the lock below
			{
				std::lock_guard<std::mutex> _lock(_swapChainLock);
 				if (_swapChain != nullptr && _bDrawEventActive)
				{
					try {
						auto drawingSession = _swapChain.CreateDrawingSession(_drawingSessionClearColor);
						AudioVisualizer::VisualizationDataFrame dataFrame{ nullptr };
						Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime;
						if (_visualizationSource) {
							dataFrame = _visualizationSource.GetData();
							presentationTime = _visualizationSource.PresentationTime();
						}
						auto activity = Trace::VisualizeControl_StartDraw(dataFrame, presentationTime);
						OnDraw(drawingSession, dataFrame, presentationTime);
						drawingSession.Close();
						_swapChain.Present();
						waitableSwapChain = _swapChain;

					}
					catch (hresult_error err)
					{
						if (err.code() == DXGI_ERROR_DEVICE_HUNG ||
							err.code() == DXGI_ERROR_DEVICE_REMOVED ||
							err.code() == DXGI_ERROR_DEVICE_RESET ||
							err.code() == DXGI_ERROR_DRIVER_INTERNAL_ERROR ||
							err.code() == DXGI_ERROR_INVALID_CALL ||
							err.code() == D2DERR_RECREATE_TARGET)
						{
							Trace::VisualizeControl_RecreateDevice(err);
							RecreateDevice();
							continue;
						}
						else
						{
							Trace::VisualizeControl_DrawLoopException(err);
							throw;
						}
					}
				}
			}
			if (waitableSwapChain) {
				waitableSwapChain.WaitForVerticalBlank();
			}
			else {
				WaitForSingleObject(_cancelDrawLoop, 17);				// If there is no swap chain or draw lock is suspended wait for 17ms and retry
			}
		}

	}
	void VisualizerControl::StartDrawLoop()
	{
		Windows::System::Threading::ThreadPool::RunAsync(
			Windows::System::Threading::WorkItemHandler(this,&VisualizerControl::DrawLoop)
		);
	}

	void VisualizerControl::OnSizeChanged(IInspectable sender, Windows::UI::Xaml::SizeChangedEventArgs args)
	{
		if (args.NewSize().Height > 0 && args.NewSize().Width > 0)
		{
			CreateSwapChainWithSize(args.NewSize());
		}
	}

	void VisualizerControl::OnLoaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args)
	{
		Windows::Foundation::Size size;
		size.Width = (float)ActualWidth();
		size.Height = (float)ActualHeight();
		CreateSwapChainWithSize(size);
		OnCreateResources(_swapChain, CreateResourcesReason::New);
		_bDrawEventActive = true;
		StartDrawLoop();
	}
	void VisualizerControl::OnUnloaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args)
	{
		_bDrawEventActive = false;
		SetEvent(_cancelDrawLoop);
	}
	void VisualizerControl::OnDpiChanged(Windows::Graphics::Display::DisplayInformation info, IInspectable)
	{
		Windows::Foundation::Size size;
		size.Width = (float) ActualWidth();
		size.Height = (float)ActualHeight();
		CreateSwapChainWithSizeAndDpi(size, info.LogicalDpi());
		OnCreateResources(_swapChain,CreateResourcesReason::DpiChanged);
	}
	void VisualizerControl::OnVisibilityChanged(Windows::UI::Xaml::DependencyObject obj, Windows::UI::Xaml::DependencyProperty prop)
	{
		_bDrawEventActive = Visibility() == Windows::UI::Xaml::Visibility::Visible;
	}

	void VisualizerControl::RegisterEventHandlers()
	{
		Loaded(Windows::UI::Xaml::RoutedEventHandler(this, &VisualizerControl::OnLoaded));
		Unloaded(Windows::UI::Xaml::RoutedEventHandler(this, &VisualizerControl::OnUnloaded));
		SizeChanged(Windows::UI::Xaml::SizeChangedEventHandler(this, &VisualizerControl::OnSizeChanged));
		if (!Windows::ApplicationModel::DesignMode::DesignModeEnabled()) {
			Windows::Graphics::Display::DisplayInformation::GetForCurrentView().DpiChanged(
				Windows::Foundation::TypedEventHandler<Windows::Graphics::Display::DisplayInformation,IInspectable>(this,&VisualizerControl::OnDpiChanged)
			);
		}
		RegisterPropertyChangedCallback(Windows::UI::Xaml::UIElement::VisibilityProperty(), Windows::UI::Xaml::DependencyPropertyChangedCallback(this,&VisualizerControl::OnVisibilityChanged));
	}

	VisualizerControl::VisualizerControl()
	{
		_cancelDrawLoop = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
		_drawingSessionClearColor = Windows::UI::Colors::Transparent();
		InitializeSwapChain();
		RegisterEventHandlers();
	}
}
