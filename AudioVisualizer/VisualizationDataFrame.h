#pragma once
#include "AudioVisualizer_h.h"
#include "Nullable.h"
#include <DirectXMath.h>

using namespace ABI::AudioVisualizer;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;

namespace AudioVisualizer
{
	class VisualizationDataFrame : public RuntimeClass<IVisualizationDataFrame>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_VisualizationDataFrame, BaseTrust)
		unsigned _channels;
		ComPtr<IReference<TimeSpan>> _time;
		ComPtr<IReference<TimeSpan>> _duration;
		ComPtr<IVisualizationData> _rms;
		ComPtr<IVisualizationData> _peak;
		ComPtr<IVisualizationData> _specter;

	public:
		VisualizationDataFrame(IReference<TimeSpan> *pTime,IReference<TimeSpan> *pDuration,size_t channels,size_t spectralLength);

		STDMETHODIMP get_Time(ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			_time.CopyTo(ppTimeStamp);
			return S_OK;
		}
		STDMETHODIMP get_Duration(ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::TimeSpan> **ppTimeStamp)
		{
			if (ppTimeStamp == nullptr)
				return E_INVALIDARG;
			_duration.CopyTo(ppTimeStamp);
			return S_OK;
		}
		STDMETHODIMP get_RMS(IVisualizationData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			_rms.CopyTo(ppData);
			return S_OK;
		}
		STDMETHODIMP get_Peak(IVisualizationData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			_peak.CopyTo(ppData);
			return S_OK;
		}
	};
}

