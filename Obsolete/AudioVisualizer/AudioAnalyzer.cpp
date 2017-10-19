#include "pch.h"
#include "AudioAnalyzer.h"

using namespace Microsoft::WRL::Wrappers;

namespace AudioVisualizer
{
	AudioAnalyzer::AudioAnalyzer(size_t bufferSize)
	{
	}


	AudioAnalyzer::~AudioAnalyzer()
	{
	}

	STDMETHODIMP AudioAnalyzer::put_MediaType(IAudioEncodingProperties * pType)
	{
		if (pType == nullptr)
			return E_POINTER;
		
		ComPtr<IMediaEncodingProperties> mediaType;
		HString hsType,hsSubType;
		mediaType->get_Type(hsType.GetAddressOf());
		mediaType->get_SubType(hsSubType.GetAddressOf());

		return E_NOTIMPL;
	}

}
