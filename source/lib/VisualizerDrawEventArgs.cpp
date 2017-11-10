#include "pch.h"
#include "ErrorHandling.h"
#include "VisualizerDrawEventArgs.h"

namespace AudioVisualizer
{
	class VisualizerDrawEventArgsFactory : public AgileActivationFactory<>
	{
	public:
		IFACEMETHODIMP Create(ICanvasDrawingSession *pDrawingSession, IVisualizationDataFrame *pData, IVisualizerDrawEventArgs **ppArgs,Size size,IReference<TimeSpan> *pTime)
		{
			if (ppArgs == nullptr)
				return E_INVALIDARG;
			
			*ppArgs = nullptr;

			auto args = Make<VisualizerDrawEventArgs>(pDrawingSession, pData,size,pTime);
			if (args == nullptr)
				return E_FAIL;

			ThrowIfFailed(args.CopyTo(ppArgs));
		}
	};

	ActivatableClassWithFactory(VisualizerDrawEventArgs, VisualizerDrawEventArgsFactory);
}
