#pragma once
#include "AudioAnalyzer_h.h"
#include <wrl.h>
#include <windows.ui.composition.h>

namespace AudioAnalyzer
{
	class CVisualizerDrawEventArgs : public Microsoft::WRL::RuntimeClass<	
		ABI::AudioAnalyzer::IVisualizerDrawEventArgs, 
		Microsoft::WRL::ComposableBase<>>
	{

	};

	class CBaseVisualizer : public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRt>,
		ABI::AudioAnalyzer::IVisualizer>	
	{
		InspectableClass(RuntimeClass_AudioAnalyzer_BaseVisualizer, BaseTrust)

		typedef ABI::Windows::Foundation::ITypedEventHandler<
					ABI::AudioAnalyzer::IVisualizer*, 
					ABI::AudioAnalyzer::VisualizerDrawEventArgs *> Visualizer_DrawEventHandler;
		
		Microsoft::WRL::EventSource<Visualizer_DrawEventHandler> _drawEventList;
	protected:
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Composition::ICompositor> _compositor;
		Microsoft::WRL::ComPtr<ABI::Windows::UI::Composition::IContainerVisual> _rootVisual;

		Microsoft::WRL::ComPtr<ABI::AudioAnalyzer::IVisualizationSource> _source;

	public:
		CBaseVisualizer();
		~CBaseVisualizer();

		HRESULT RuntimeClassInitialize();

		STDMETHODIMP add_Draw(
			Visualizer_DrawEventHandler* value,
			EventRegistrationToken *token);
		STDMETHODIMP remove_Draw(
			EventRegistrationToken token);
		
		STDMETHODIMP get_Source(ABI::AudioAnalyzer::IVisualizationSource **ppSource)
		{
			if (ppSource == nullptr)
				return E_INVALIDARG;
			*ppSource = _source.Get();
			return S_OK;
		}

		STDMETHODIMP put_Source(ABI::AudioAnalyzer::IVisualizationSource *pSource)
		{
			_source = pSource;
			return S_OK;
		}
	};
}

