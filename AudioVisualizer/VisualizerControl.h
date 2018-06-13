#pragma once

#include "VisualizerControl.g.h"
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Windows.Graphics.Display.h>

namespace winrt::AudioVisualizer::implementation
{
    struct VisualizerControl : VisualizerControlT<VisualizerControl>
    {
	protected:
		virtual void OnDraw(Microsoft::Graphics::Canvas::CanvasDrawingSession /*drawingSession*/, VisualizationDataFrame /*dataFrame*/, Windows::Foundation::IReference<Windows::Foundation::TimeSpan> /*presentationTime*/) {};
		virtual void OnCreateResources(Microsoft::Graphics::Canvas::ICanvasResourceCreator /*resourceCreator*/, CreateResourcesReason /* reason */) {};
		Windows::UI::Composition::ContainerVisual _rootVisual{ nullptr };
		Windows::UI::Composition::SpriteVisual _swapChainVisual{ nullptr };
		Windows::UI::Composition::Compositor _compositor{ nullptr };
		Windows::UI::Composition::CompositionGraphicsDevice _compositionGraphicsDevice{ nullptr };
		Windows::UI::Composition::ICompositionSurface _compositionSurface{ nullptr };
		Windows::UI::Composition::CompositionBrush _surfaceBrush{ nullptr };
		Microsoft::Graphics::Canvas::CanvasDevice _device{ nullptr };
		Microsoft::Graphics::Canvas::CanvasSwapChain _swapChain{ nullptr };
		Windows::Foundation::Size _swapChainSize;
		std::mutex _swapChainLock;
		HANDLE _cancelDrawLoop;
		bool _bDrawEventActive;
		Windows::UI::Color _drawingSessionClearColor;
		AudioVisualizer::IVisualizationSource _visualizationSource;

		void CreateDevice();
		void RecreateDevice();
		void InitializeSwapChain();
		void CreateSwapChainWithSize(Windows::Foundation::Size size);
		void CreateSwapChainWithSizeAndDpi(Windows::Foundation::Size size,float dpi);
		void RegisterEventHandlers();
		void StartDrawLoop();
		void DrawLoop(Windows::Foundation::IAsyncAction const& operation);
		void OnSizeChanged(IInspectable sender,Windows::UI::Xaml::SizeChangedEventArgs args);
		void OnLoaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args);
		void OnUnloaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args);
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation info, IInspectable);
		void OnVisibilityChanged(Windows::UI::Xaml::DependencyObject obj, Windows::UI::Xaml::DependencyProperty property);

	public:
		VisualizerControl();
		AudioVisualizer::IVisualizationSource Source();
		void Source(AudioVisualizer::IVisualizationSource const& value);
		Windows::UI::Color BackgroundColor();
		void BackgroundColor(Windows::UI::Color const& value);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct VisualizerControl : VisualizerControlT<VisualizerControl, implementation::VisualizerControl>
    {
    };
}
