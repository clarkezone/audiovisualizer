#pragma once
#include "AudioVisualizer.abi.h"
#include "BaseVisualizer.h"
#include "VisualizerDrawEventArgs.h"

using namespace ABI::AudioVisualizer;
using namespace ABI::Microsoft::Graphics::Canvas;


namespace AudioVisualizer
{
	class CustomVisualizer :  public RuntimeClass<IVisualizer,IVisualizerCustomDraw,ComposableBase<>>, public MixIn<CustomVisualizer,IVisualizer>, public BaseVisualizer<CustomVisualizer>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_CustomVisualizer, BaseTrust)

			typedef ABI::Windows::Foundation::ITypedEventHandler<
			ABI::AudioVisualizer::IVisualizer*,
			ABI::AudioVisualizer::VisualizerDrawEventArgs *> Visualizer_DrawEventHandler;

		Microsoft::WRL::EventSource<Visualizer_DrawEventHandler> _drawEventList;
	protected:
		virtual HRESULT OnDraw(ICanvasDrawingSession *pSession);

	public:
		CustomVisualizer();
		~CustomVisualizer();
		STDMETHODIMP get_Source(ABI::AudioVisualizer::IVisualizationSource **ppSource)
		{
			if (ppSource == nullptr)
				return E_POINTER;
			*ppSource = _visualizationSource.Get();
			return S_OK;
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
			EventRegistrationToken *token);
		STDMETHODIMP remove_Draw(
			EventRegistrationToken token);
	};
}

