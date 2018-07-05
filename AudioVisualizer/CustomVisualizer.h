#pragma once

#include "CustomVisualizer.g.h"
#include "VisualizerControl.h"
#include "CreateResourcesEventArgs.h"
#include "VisualizerDrawEventArgs.h"

namespace winrt::AudioVisualizer::implementation
{
    struct CustomVisualizer : CustomVisualizerT<CustomVisualizer>
    {
	private:
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
		void CreateSwapChainWithSizeAndDpi(Windows::Foundation::Size size, float dpi);
		void RegisterEventHandlers();
		void StartDrawLoop();
		void DrawLoop(Windows::Foundation::IAsyncAction const& operation);
		void OnSizeChanged(IInspectable sender, Windows::UI::Xaml::SizeChangedEventArgs args);
		void OnLoaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args);
		void OnUnloaded(IInspectable sender, Windows::UI::Xaml::RoutedEventArgs args);
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation info, IInspectable);
		void OnVisibilityChanged(Windows::UI::Xaml::DependencyObject obj, Windows::UI::Xaml::DependencyProperty property);
		winrt::event<Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::VisualizerDrawEventArgs >> _drawEvent;
		winrt::event<Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::CreateResourcesEventArgs>> _createResourcesEvent;
	public:
        CustomVisualizer();
		AudioVisualizer::IVisualizationSource Source();
		void Source(AudioVisualizer::IVisualizationSource const& value);
        event_token Draw(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::VisualizerDrawEventArgs> const& handler);
        void Draw(event_token const& token);
        event_token CreateResources(Windows::Foundation::TypedEventHandler<Windows::Foundation::IInspectable, AudioVisualizer::CreateResourcesEventArgs> const& handler);
        void CreateResources(event_token const& token);
    };
}

namespace winrt::AudioVisualizer::factory_implementation
{
    struct CustomVisualizer : CustomVisualizerT<CustomVisualizer, implementation::CustomVisualizer>
    {
    };
}
