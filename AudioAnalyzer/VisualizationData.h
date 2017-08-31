#pragma once
#include <wrl.h>
#include "Nullable.h"
#include "AudioAnalyzer_h.h"

namespace AudioAnalyzer
{
	class CVisualizationData : public Microsoft::WRL::RuntimeClass<
		Microsoft::WRL::RuntimeClassFlags< Microsoft::WRL::RuntimeClassType::WinRtClassicComMix >,
		ABI::AudioAnalyzer::IVisualizationData>
	{
		InspectableClass(RuntimeClass_AudioAnalyzer_VisualizationData, BaseTrust)
		unsigned _channels;
		REFERENCE_TIME _time;
	public:
		CVisualizationData();
		HRESULT RuntimeClassInitialize();


		STDMETHODIMP get_AudioChannels(unsigned *pChannels)
		{
			return _channels;
		}
		STDMETHODIMP get_RMS(unsigned *pSize, float **ppData)
		{
			float *pData = (float *)CoTaskMemAlloc(sizeof(float) * 2);
			pData[0] = 1.0f;
			pData[1] = 2.0f;
			*pSize = 2;
			*ppData = pData;
			return S_OK;
		}

		STDMETHODIMP get_Time(ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::TimeSpan> **ppTimeStamp)
		{
			using namespace Microsoft::WRL;
			using namespace ABI::Windows::Foundation;
			if (_time != -1)
			{
				ComPtr<IReference<TimeSpan>> spTime;
				TimeSpan ts = { _time };
				spTime = Make<Nullable<TimeSpan>>(ts);
				spTime.CopyTo(ppTimeStamp);
			}
			else
			{
				*ppTimeStamp = nullptr;
			}
			return S_OK;
		}
		
	};
}

