#pragma once
#include "AudioVisualizer.abi.h"
#include "LifeSpanTracker.h"

namespace AudioVisualizer
{
	using namespace Microsoft::WRL;
	using namespace ABI::AudioVisualizer;
	using namespace ABI::Microsoft::Graphics::Canvas;
	using namespace ABI::Windows::Foundation;

	class VisualizerDrawEventArgs : public RuntimeClass<IVisualizerDrawEventArgs,FtmBase>, LifespanTracker<VisualizerDrawEventArgs>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizerDrawEventArgs,BaseTrust);

		ComPtr<IInspectable> _drawingSession;
		ComPtr<IVisualizationDataFrame> _data;
		Size _viewExtent;
		ComPtr<IReference<TimeSpan>> _presentationTime;
	public:
		VisualizerDrawEventArgs(IInspectable *pSession, IVisualizationDataFrame *pData,Size viewExtent,IReference<TimeSpan> *pTime)
		{
			_drawingSession = pSession;
			_data = pData;
			_viewExtent = viewExtent;
			_presentationTime = pTime;
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
		STDMETHODIMP get_ViewExtent(Size *pSize)
		{
			if (pSize == nullptr)
				return E_INVALIDARG;
			*pSize = _viewExtent;
			return S_OK;
		}
		STDMETHODIMP get_PresentationTime(IReference<TimeSpan> **ppTime)
		{
			if (ppTime == nullptr)
				return E_POINTER;
			*ppTime = nullptr;
			return _presentationTime.CopyTo(ppTime);
		}
	};
}

