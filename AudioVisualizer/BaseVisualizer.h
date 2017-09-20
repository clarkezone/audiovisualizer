#pragma once
#include "AudioVisualizer.abi.h"
#include <wrl.h>
#include <windows.ui.composition.h>
#include <mutex>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Microsoft::Graphics::Canvas;
using namespace ABI::Windows::UI::Composition;
using namespace ABI::Windows::UI;

namespace AudioVisualizer
{
	class BaseVisualizer : public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRt>,
		Microsoft::WRL::Implements<ABI::AudioVisualizer::IVisualizer>,
		Microsoft::WRL::ComposableBase<>>	
	{
		InspectableClass(RuntimeClass_AudioVisualizer_BaseVisualizer, BaseTrust)

		typedef ABI::Windows::Foundation::ITypedEventHandler<
					ABI::AudioVisualizer::IVisualizer*, 
					ABI::AudioVisualizer::VisualizerDrawEventArgs *> Visualizer_DrawEventHandler;
		
		Microsoft::WRL::EventSource<Visualizer_DrawEventHandler> _drawEventList;

		Microsoft::WRL::ComPtr<ABI::Microsoft::Graphics::Canvas::ICanvasDeviceStatics> _deviceStatics;
		Microsoft::WRL::ComPtr<ABI::Microsoft::Graphics::Canvas::UI::Composition::ICanvasCompositionStatics> _canvasCompositionStatics;
		Microsoft::WRL::ComPtr<ABI::Microsoft::Graphics::Canvas::ICanvasDevice> _device;
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Composition::ICompositionGraphicsDevice> _compositionGraphicsDevice;
		ComPtr<ICanvasSwapChainFactory> _swapChainFactory;
		ComPtr<ICanvasSwapChain> _swapChain;
		ComPtr<ISpriteVisual> _swapChainVisual;

		EventRegistrationToken _deviceLostToken;
		HANDLE _cancelDrawLoop;
		Color _backgroundColor;

		std::mutex _mtxSwapChain;
		CriticalSection _csLock;

	private:
		HRESULT CreateDevice();
		HRESULT OnDraw();

	protected:
		
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Composition::ICompositor> _compositor;
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Composition::IContainerVisual> _rootVisual;

		Microsoft::WRL::ComPtr<ABI::AudioVisualizer::IVisualizationSource> _source;

		Microsoft::WRL::ComPtr<IInspectable> _control;
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Xaml::IWindow> _window;
		EventRegistrationToken _sizeChangedToken;
		EventRegistrationToken _loadedToken;

		HRESULT OnSizeChanged(const ABI::Windows::Foundation::Size &size);
		HRESULT OnLoaded();
		HRESULT StartDrawLoop();

	public:
		BaseVisualizer();
		~BaseVisualizer();

		STDMETHODIMP add_Draw(
			Visualizer_DrawEventHandler* value,
			EventRegistrationToken *token);
		STDMETHODIMP remove_Draw(
			EventRegistrationToken token);
		
		STDMETHODIMP get_Source(ABI::AudioVisualizer::IVisualizationSource **ppSource)
		{
			if (ppSource == nullptr)
				return E_POINTER;
			*ppSource = _source.Get();
			return S_OK;
		}

		STDMETHODIMP put_Source(ABI::AudioVisualizer::IVisualizationSource *pSource)
		{
			auto lock = _csLock.Lock();
			_source = pSource;
			return S_OK;
		}

		STDMETHODIMP get_BackgroundColor(Color *pColor)
		{
			if (pColor == nullptr)
				return E_POINTER;
			*pColor = _backgroundColor;
			return S_OK;
		}
		STDMETHODIMP put_BackgroundColor(Color color)
		{
			auto lock = _csLock.Lock();
			_backgroundColor = color;
			return S_OK;
		}

		Microsoft::WRL::ComPtr<IInspectable> GetComposableBase();
	};
}

