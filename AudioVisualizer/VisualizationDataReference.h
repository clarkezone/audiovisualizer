#pragma once
#include <wrl.h>
#include "ScalarData.h"
#include "VectorData.h"
#include "VisualizationDataFrame.h"
#include "AudioVisualizer_h.h"
#include <windows.foundation.h>
#include "LifeSpanTracker.h"

using namespace Microsoft::WRL;
using namespace ABI::AudioVisualizer;

namespace AudioVisualizer
{
	class VisualizationDataReference : public  RuntimeClass<IVisualizationDataReference,IClosable>, LifespanTracker<VisualizationDataReference>
	{
		ComPtr<ScalarData> _rms;
		ComPtr<ScalarData> _peak;
		ComPtr<VectorData> _spectrum;
		ComPtr<VisualizationDataFrame> _frame;
		bool _bIsClosed;
	public:
		VisualizationDataReference(VisualizationDataFrame *pFrame,ScalarData *pRms, ScalarData *pPeak, VectorData *pSpectrum)
		{
			//_frame = pFrame;	// AddRef also the data frame to avoid deleting it until the reference is released
			_rms = pRms;
			_peak = pPeak;
			_spectrum = pSpectrum;
			_bIsClosed = false;
		}
		STDMETHODIMP get_RMS(IScalarData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			if (_bIsClosed)
				return RO_E_CLOSED;
			_rms.CopyTo(ppData);
			return S_OK;
		}
		STDMETHODIMP get_Peak(IScalarData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			if (_bIsClosed)
				return RO_E_CLOSED;
			_peak.CopyTo(ppData);
			return S_OK;
		}
		STDMETHODIMP get_Spectrum(IVectorData **ppData)
		{
			if (ppData == nullptr)
				return E_INVALIDARG;
			if (_bIsClosed)
				return RO_E_CLOSED;
			_spectrum.CopyTo(ppData);
			return S_OK;
		}
		STDMETHODIMP Close()
		{
			if (_bIsClosed)
				return RO_E_CLOSED;

			_bIsClosed = true;
			_rms = nullptr;
			_peak = nullptr;
			_spectrum = nullptr;
			// _frame = nullptr;
#ifdef _DEBUG
			Lifespan_CloseObject();
#endif // _DEBUG

			return S_OK;
		}
	};
}

