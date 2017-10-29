#pragma once
#include "BaseVisualizer.h"
#include "VisualizerDrawEventArgs.h"

using namespace ABI::AudioVisualizer;
using namespace ABI::Microsoft::Graphics::Canvas;


namespace AudioVisualizer
{
	class CustomVisualizer :  
		public RuntimeClass<IVisualizer,IVisualizerCustomDraw,ComposableBase<>>, 
		public MixIn<CustomVisualizer,IVisualizer>, 
		public BaseVisualizer<CustomVisualizer>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_CustomVisualizer, BaseTrust)

		typedef ABI::Windows::Foundation::ITypedEventHandler<
			ABI::AudioVisualizer::IVisualizer*,
			ABI::AudioVisualizer::VisualizerDrawEventArgs *> Visualizer_DrawEventHandler;

		typedef ABI::Windows::Foundation::ITypedEventHandler<
			IInspectable *,
			IInspectable *> Visualizer_CreateResources_EventHandler;

		Microsoft::WRL::EventSource<Visualizer_DrawEventHandler> _drawEventList;
		Microsoft::WRL::EventSource<Visualizer_CreateResources_EventHandler> _createResourcesEventList;
	protected:
		virtual HRESULT OnDraw(ICanvasDrawingSession *pSession,IVisualizationDataFrame *pDataFrame);
		virtual HRESULT OnCreateResources()
		{
			return _createResourcesEventList.InvokeAll(As<IInspectable>(GetControl()).Get(), _swapChain.Get());
		}
	public:
		STDMETHODIMP get_Source(ABI::AudioVisualizer::IVisualizationSource **ppSource)
		{
			if (ppSource == nullptr)
				return E_POINTER;
			return _visualizationSource.CopyTo(ppSource);
		}

		STDMETHODIMP put_Source(ABI::AudioVisualizer::IVisualizationSource *pSource)
		{
			_visualizationSource = pSource;
			return S_OK;
		}

		STDMETHODIMP get_BackgroundColor(Color *pColor)
		{
			if (pColor == nullptr)
				return E_POINTER;
			*pColor = _drawingSessionBackgroundColor;
			return S_OK;
		}
		STDMETHODIMP put_BackgroundColor(Color color)
		{
			_drawingSessionBackgroundColor = color;
			return S_OK;
		}


		STDMETHODIMP add_Draw(
			Visualizer_DrawEventHandler* value,
			EventRegistrationToken *token)
		{
			return _drawEventList.Add(value, token);
		}
		STDMETHODIMP remove_Draw(
			EventRegistrationToken token)
		{
			return _drawEventList.Remove(token);
		}
		
		STDMETHODIMP add_CreateResources(
			Visualizer_CreateResources_EventHandler* value,
			EventRegistrationToken *token)
		{
			return _createResourcesEventList.Add(value, token);
		}
		STDMETHODIMP remove_CreateResources(
			EventRegistrationToken token)
		{
			return _createResourcesEventList.Remove(token);
		}
	};
}

