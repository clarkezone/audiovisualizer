#pragma once
#include "AudioVisualizer.abi.h"

using namespace Microsoft::WRL;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Media::MediaProperties;

namespace AudioVisualizer
{
	class AudioAnalyzer : RuntimeClass<IAudioAnalyzer>
	{
		ComPtr<IAudioEncodingProperties> _mediaType;
	public:
		AudioAnalyzer(size_t bufferSize);
		~AudioAnalyzer();

		STDMETHODIMP get_MediaType(IAudioEncodingProperties **ppType)
		{
			if (ppType == nullptr)
				return E_POINTER;
			return _mediaType.CopyTo(ppType);
		}

		STDMETHODIMP put_MediaType(IAudioEncodingProperties *pType);
	};
}

