#pragma once
#include "AudioVisualizer.abi.h"
#include "LifeSpanTracker.h"

namespace AudioVisualizer
{
	using namespace Microsoft::WRL;
	using namespace ABI::AudioVisualizer;
	using namespace ABI::Microsoft::Graphics::Canvas;

	class VisualizerDrawEventArgs : public RuntimeClass<IVisualizerDrawEventArgs,FtmBase>, LifespanTracker<VisualizerDrawEventArgs>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizerDrawEventArgs,BaseTrust);

		ComPtr<IInspectable> _drawingSession;
		ComPtr<IVisualizationDataFrame> _data;
	public:
		VisualizerDrawEventArgs(IInspectable *pSession, IVisualizationDataFrame *pData)
		{
			_drawingSession = pSession;
			_data = pData;
		}
		~VisualizerDrawEventArgs()
		{
		}
		STDMETHODIMP get_DrawingSession(IInspectable **ppSession)
		{
			if (ppSession == nullptr)
				return E_INVALIDARG;

			return _drawingSession.CopyTo(ppSession);
		}

		STDMETHODIMP get_Data(IVisualizationDataFrame **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			_data.CopyTo(ppData);
			return S_OK;
		}

	};
}

