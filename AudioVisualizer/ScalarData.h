#pragma once
#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer_h.h"
#include <DirectXMath.h>
#include <vector>
#include "LifeSpanTracker.h"
#include "trace.h"

using namespace Microsoft::WRL;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Diagnostics;

namespace AudioVisualizer
{
	class ScalarData : public RuntimeClass<IScalarData>, LifespanTracker<ScalarData>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_ScalarData, BaseTrust);

		ScaleType _amplitudeScale;
		DirectX::XMVECTOR *_pData;
		size_t _size;
	public:
		ScalarData(size_t cElements, ScaleType scaleType = ScaleType::Linear);
		~ScalarData();

		DirectX::XMVECTOR *GetBuffer() { return _pData; }

		STDMETHODIMP get_AmplitudeScale(ScaleType *pScale)
		{
			if (pScale == nullptr)
				return E_INVALIDARG;
			*pScale = _amplitudeScale;
			return S_OK;
		}
		STDMETHODIMP get_Values(UINT32 *pcElements, float **ppValues)
		{
			if (ppValues == nullptr)
				return E_INVALIDARG;
			*ppValues = (float *)_pData;
			*pcElements = _size;
			return S_OK;
		}
		STDMETHODIMP get_Size(UINT32 *pSize)
		{
			if (pSize == nullptr)
				return E_INVALIDARG;
			*pSize = _size;
			return S_OK;
		}
		STDMETHODIMP ConvertToLogAmplitude(float minValue, float maxValue, IScalarData **ppResult);
		STDMETHODIMP ApplyRiseAndFall(IScalarData *pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IScalarData **ppResult);
	};
}
