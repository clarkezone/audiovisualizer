#include "pch.h"
#include "CustomVisualizer.h"
#include "trace.h"
#include "ErrorHandling.h"

namespace AudioVisualizer
{

	class CustomVisualizerFactory : public AgileActivationFactory<>
	{
	public:
		IFACEMETHODIMP ActivateInstance(IInspectable** obj) override
		{
			return ExceptionBoundary(
				[&]
			{
				CheckAndClearOutPointer(obj);
				auto control = Make<CustomVisualizer>();
				CheckMakeResult(control);
				ThrowIfFailed(control.CopyTo(obj));
			});
		}
	};

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
	HRESULT CustomVisualizer::OnDraw(ICanvasDrawingSession *pSession)
	{
		ComPtr<IVisualizationDataFrame> dataFrame;
		if (_visualizationSource != nullptr)
			_visualizationSource->GetData(&dataFrame);

		auto args = Make<VisualizerDrawEventArgs>(pSession, dataFrame.Get());
#ifdef _TRACE
			ComPtr<ILoggingActivity> activity;
			Diagnostics::Trace::Log_StartDraw(dataFrame.Get(), &activity);
			AudioVisualizer::Diagnostics::CLogActivityHelper drawActivity(activity.Get());
#endif
		return _drawEventList.InvokeAll(this, args.Get());
	}

	ActivatableClassWithFactory(CustomVisualizer, CustomVisualizerFactory);

}
