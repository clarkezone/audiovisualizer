#include "pch.h"
#include "CustomVisualizer.h"
#include "Tracing.h"
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>
#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.h>

namespace winrt::AudioVisualizer::implementation
{
    event_token CustomVisualizer::Draw(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::VisualizerDrawEventArgs> const& handler)
    {
		return _drawEvent.add(handler);
    }

    void CustomVisualizer::Draw(event_token const& token)
    {
		_drawEvent.remove(token);
    }

    event_token CustomVisualizer::CreateResources(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::CreateResourcesEventArgs> const& handler)
    {
		return _createResourcesEvent.add(handler);
    }

    void CustomVisualizer::CreateResources(event_token const& token)
    {
		_createResourcesEvent.remove(token);
    }

	AudioVisualizer::IVisualizationSource CustomVisualizer::Source()
	{
		return _visualizationSource;
	}

	void CustomVisualizer::Source(AudioVisualizer::IVisualizationSource const& value)
	{
		_visualizationSource = value;
	}

	void CustomVisualizer::CreateDevice()
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
	void CustomVisualizer::RecreateDevice()
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
			auto args = make<CreateResourcesEventArgs>(CreateResourcesReason::DeviceLost, _swapChain);
			_createResourcesEvent(*this, args);
		}
		));
	}
	void CustomVisualizer::InitializeSwapChain()
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

	void CustomVisualizer::CreateSwapChainWithSizeAndDpi(Windows::Foundation::Size size, float dpi)
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
	void CustomVisualizer::CreateSwapChainWithSize(Windows::Foundation::Size size)
	{
		float logicalDpi = Windows::ApplicationModel::DesignMode::DesignModeEnabled() ? 96.0f : Windows::Graphics::Display::DisplayInformation::GetForCurrentView().LogicalDpi();
		CreateSwapChainWithSizeAndDpi(size, logicalDpi);
	}

	void CustomVisualizer::DrawLoop(Windows::Foundation::IAsyncAction const& /*action*/)
	{
		SetThreadDescription(GetCurrentThread(), L"CustomVisualizer draw loop");
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
						auto drawingSession = _swapChain.CreateDrawingSession(Windows::UI::Colors::Transparent());
						AudioVisualizer::VisualizationDataFrame dataFrame{ nullptr };
						Windows::Foundation::IReference<Windows::Foundation::TimeSpan> presentationTime;
						if (_visualizationSource) {
							dataFrame = _visualizationSource.GetData();
							presentationTime = _visualizationSource.PresentationTime();
						}
#ifdef _TRACE_
						auto activity = Trace::VisualizeControl_StartDraw(dataFrame, presentationTime);
#endif
						auto args = make<VisualizerDrawEventArgs>(dataFrame, drawingSession, _swapChainSize, presentationTime);
						_drawEvent(*this, args);
#ifdef _TRACE_
						activity.StopActivity(activity.Name());
#endif
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
#ifdef _TRACE_
							Trace::VisualizeControl_RecreateDevice(err);
#endif
							RecreateDevice();
							continue;
						}
						else
						{
#ifdef _TRACE_
							Trace::VisualizeControl_DrawLoopException(err);
#endif
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
	void CustomVisualizer::StartDrawLoop()
	{
		Windows::System::Threading::ThreadPool::RunAsync(
			Windows::System::Threading::WorkItemHandler(this, &CustomVisualizer::DrawLoop)
		);
	}

	void CustomVisualizer::OnSizeChanged(IInspectable sender, Windows::UI::Xaml::SizeChangedEventArgs args)
	{
		if (args.NewSize().Height > 0 && args.NewSize().Width > 0)
		{
			CreateSwapChainWithSize(args.NewSize());
		}
	}

	void CustomVisualizer::OnLoaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args)
	{
		Windows::Foundation::Size size;
		size.Width = (float)ActualWidth();
		size.Height = (float)ActualHeight();
		// Do not create draw loop in design mode
		if (!Windows::ApplicationModel::DesignMode::DesignModeEnabled())
		{
			CreateSwapChainWithSize(size);
			auto createEventArgs = make<CreateResourcesEventArgs>(CreateResourcesReason::New, _swapChain);
			_createResourcesEvent(*this, createEventArgs);
			_bDrawEventActive = true;
			StartDrawLoop();
		}
	}
	void CustomVisualizer::OnUnloaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args)
	{
		_bDrawEventActive = false;
		SetEvent(_cancelDrawLoop);
	}
	void CustomVisualizer::OnDpiChanged(Windows::Graphics::Display::DisplayInformation info, IInspectable)
	{
		Windows::Foundation::Size size;
		size.Width = (float)ActualWidth();
		size.Height = (float)ActualHeight();
		CreateSwapChainWithSizeAndDpi(size, info.LogicalDpi());
		auto args = make<CreateResourcesEventArgs>(CreateResourcesReason::DpiChanged, _swapChain);
		_createResourcesEvent(*this, args);
	}
	void CustomVisualizer::OnVisibilityChanged(Windows::UI::Xaml::DependencyObject obj, Windows::UI::Xaml::DependencyProperty prop)
	{
		_bDrawEventActive = Visibility() == Windows::UI::Xaml::Visibility::Visible;
	}

	void CustomVisualizer::RegisterEventHandlers()
	{
		Loaded(Windows::UI::Xaml::RoutedEventHandler(this, &CustomVisualizer::OnLoaded));
		Unloaded(Windows::UI::Xaml::RoutedEventHandler(this, &CustomVisualizer::OnUnloaded));
		SizeChanged(Windows::UI::Xaml::SizeChangedEventHandler(this, &CustomVisualizer::OnSizeChanged));
		if (!Windows::ApplicationModel::DesignMode::DesignModeEnabled()) {
			Windows::Graphics::Display::DisplayInformation::GetForCurrentView().DpiChanged(
				Windows::Foundation::TypedEventHandler<Windows::Graphics::Display::DisplayInformation, IInspectable>(this, &CustomVisualizer::OnDpiChanged)
			);
		}
		RegisterPropertyChangedCallback(Windows::UI::Xaml::UIElement::VisibilityProperty(), Windows::UI::Xaml::DependencyPropertyChangedCallback(this, &CustomVisualizer::OnVisibilityChanged));
	}

	CustomVisualizer::CustomVisualizer()
	{
		_cancelDrawLoop = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
		_drawingSessionClearColor = Windows::UI::Colors::Transparent();
		InitializeSwapChain();
		RegisterEventHandlers();
	}

}
