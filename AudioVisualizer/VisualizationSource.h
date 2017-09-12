#pragma once
#include <wrl.h>
#include <wrl\async.h>
#include "AudioVisualizer_h.h"


using namespace ABI::AudioVisualizer;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::Media::Audio;
using namespace ABI::Windows::Media::Playback;

namespace AudioVisualizer
{
	class VisualizationSourceStatics : public ActivationFactory<IVisualizationSourceStatics>
	{
		InspectableClassStatic(RuntimeClass_AudioVisualizer_VisualizationSource, BaseTrust);
	public:
		STDMETHODIMP ActivateInstance(_Outptr_result_nullonfailure_ IInspectable** ppvObject);
		STDMETHODIMP CreateFromMediaElementAsync(IMediaElement *pElement, IAsyncOperation<IVisualizationSource *> **pSource);
		STDMETHODIMP CreateFromMediaPlayerAsync(IMediaPlayer *pElement, IAsyncOperation<IVisualizationSource *> **pSource);
		STDMETHODIMP CreateFromAudioNode(IAudioNode *pNode, IVisualizationSource **pSource);
	};
}

