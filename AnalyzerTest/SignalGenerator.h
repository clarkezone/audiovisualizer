#pragma once

#include <mfapi.h>
#include <functional>
#include <vector>
#include <wrl.h>

namespace AnalyzerTest
{
	
	class CGenerator
	{
		unsigned long m_SampleOffset;
		unsigned m_SampleRate;
		unsigned m_Channels;
	public:
		CGenerator(unsigned sampleRate,unsigned channels)
		{
			m_SampleRate = sampleRate;
			m_SampleOffset = 0;
			m_Channels = channels;
		}

		HRESULT Seek(unsigned long index)
		{
			m_SampleOffset = index;
			return S_OK;
		}

		template<class Func> inline
		HRESULT GetSample(IMFSample **ppSample, size_t samplesRequested,Func f)
		{
			using namespace Microsoft::WRL;
			ComPtr<IMFSample> sample;
			HRESULT hr = MFCreateSample(&sample);
			sample->SetSampleTime((10000000L * (long long)m_SampleOffset) / m_SampleRate);	// Need to cast to long long to avoid overflow
			sample->SetSampleDuration((10000000L * (long long)samplesRequested) / m_SampleRate);

			ComPtr<IMFMediaBuffer> buffer;
			hr = MFCreateMemoryBuffer((DWORD) samplesRequested * sizeof(float) * m_Channels, &buffer);
			if (FAILED(hr))
				return hr;
			float *pBuffer;
			hr = buffer->Lock((BYTE **)&pBuffer, nullptr, nullptr);

			for (size_t sampleIndex = 0; sampleIndex < samplesRequested; sampleIndex++)
			{
				for (size_t channelIndex = 0; channelIndex < m_Channels; channelIndex++)
				{
					pBuffer[sampleIndex * m_Channels + channelIndex] = f(sampleIndex + m_SampleOffset,channelIndex);
				}
			}

			hr = buffer->Unlock();
			hr = buffer->SetCurrentLength(samplesRequested * sizeof(float) * m_Channels);
			hr = sample->AddBuffer(buffer.Get());
			
			sample.CopyTo(ppSample);
			m_SampleOffset += samplesRequested;
			
			return hr;

		}
	};
}
