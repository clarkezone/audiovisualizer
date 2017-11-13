#include "pch.h"
#include "CustomVisualizer.h"
#include "trace.h"
#include "ErrorHandling.h"
#include <Microsoft.Graphics.Canvas.h>
#include "CreateResourcesEventArgs.h"

namespace AudioVisualizer
{
	HRESULT CustomVisualizer::OnDraw(ICanvasDrawingSession *pSession, IVisualizationDataFrame *pDataFrame, IReference<TimeSpan> *pPresentationTime)
	{

		auto args = Make<VisualizerDrawEventArgs>(pSession, pDataFrame,_swapChainSize,pPresentationTime);
#ifdef _TRACE
			ComPtr<ILoggingActivity> activity;
			Diagnostics::Trace::Log_StartDraw(pDataFrame, &activity);
			AudioVisualizer::Diagnostics::CLogActivityHelper drawActivity(activity.Get());
#endif
		return _drawEventList.InvokeAll(this, args.Get());
	}

	HRESULT CustomVisualizer::OnCreateResources(ABI::AudioVisualizer::CreateResourcesReason reason)
	{
		ComPtr<CreateResourcesEventArgsImpl> args = Make<CreateResourcesEventArgsImpl>(reason, _swapChain.Get());
		return _createResourcesEventList.InvokeAll(As<IInspectable>(GetControl()).Get(), args.Get());
	}

	ActivatableClass(CustomVisualizer);

}
