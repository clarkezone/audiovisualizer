#include "pch.h"
#include "CustomVisualizer.h"
#include "trace.h"
#include "ErrorHandling.h"
#include <Microsoft.Graphics.Canvas.h>

namespace AudioVisualizer
{
	CustomVisualizer::CustomVisualizer()
	{
	}

	CustomVisualizer::~CustomVisualizer()
	{
	}
	STDMETHODIMP CustomVisualizer::add_Draw(Visualizer_DrawEventHandler * value, EventRegistrationToken * token)
	{
		return _drawEventList.Add(value, token);
	}
	STDMETHODIMP CustomVisualizer::remove_Draw(EventRegistrationToken token)
	{
		return _drawEventList.Remove(token);
	}
	HRESULT CustomVisualizer::OnDraw(ICanvasDrawingSession *pSession, IVisualizationDataFrame *pDataFrame)
	{
		auto args = Make<VisualizerDrawEventArgs>(pSession, pDataFrame);
#ifdef _TRACE
			ComPtr<ILoggingActivity> activity;
			Diagnostics::Trace::Log_StartDraw(pDataFrame, &activity);
			AudioVisualizer::Diagnostics::CLogActivityHelper drawActivity(activity.Get());
#endif
		return _drawEventList.InvokeAll(this, args.Get());
	}

	ActivatableClass(CustomVisualizer);

}
