#include "pch.h"
#include "VisualizerDrawEventArgs.h"
#include "wrtutil.h"

namespace AudioVisualizer
{
	class VisualizerDrawEventArgsFactory : public AgileActivationFactory<>
	{
	public:
		IFACEMETHODIMP Create(ICanvasDrawingSession *pDrawingSession, IVisualizationDataFrame *pData, IVisualizerDrawEventArgs **ppArgs)
		{
			if (ppArgs == nullptr)
				return E_INVALIDARG;
			
			*ppArgs = nullptr;

			auto args = Make<VisualizerDrawEventArgs>(pDrawingSession, pData);
			if (args == nullptr)
				return E_FAIL;

			wrt_util::ThrowIfFailed(args.CopyTo(ppArgs));
		}
	};

	ActivatableClassWithFactory(VisualizerDrawEventArgs, VisualizerDrawEventArgsFactory);
}
