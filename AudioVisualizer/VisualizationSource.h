#pragma once
#include <wrl.h>
#include <wrl\async.h>
#include "AudioVisualizer_h.h"


using namespace ABI::AudioVisualizer;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::Media::Audio;

namespace AudioVisualizer
{
	class VisualizationSourceStatics : public ActivationFactory<IVisualizationSourceStatics>
	{
		InspectableClassStatic(RuntimeClass_AudioVisualizer_VisualizationSource, BaseTrust);
	public:
		STDMETHODIMP ActivateInstance(_Outptr_result_nullonfailure_ IInspectable** ppvObject);
		STDMETHODIMP CreateSourceFromMediaElementAsync(IMediaElement *pElement, IAsyncOperation<IVisualizationSource *> **pSource);
		STDMETHODIMP CreateSourceFromAudioNode(IAudioNode *pNode, IVisualizationSource **pSource)
		{
			return E_NOTIMPL;
		}
	};
}

