#pragma once
#define _CRTDBG_MAP_ALLOC 

#include "AudioVisualizer.abi.h"
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
	class ScalarData : public RuntimeClass<IVectorView<float>,IScalarData>,
						LifespanTracker<ScalarData>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_ScalarData, BaseTrust);

		ScaleType _amplitudeScale;
		DirectX::XMVECTOR *_pData;
		size_t _size;
	public:
		ScalarData(size_t cElements, ScaleType scaleType = ScaleType::Linear);
		~ScalarData();

		DirectX::XMVECTOR *GetBuffer() { return _pData; }

		STDMETHODIMP GetAt(unsigned int index, float *pValue)
		{
			if (pValue == nullptr)
				return E_POINTER;
			if (index >= _size)
				return E_INVALIDARG;
			*pValue = ((float *)_pData)[index];
			return S_OK;
		}
		STDMETHODIMP get_Size(UINT32 *pSize)
		{
			if (pSize == nullptr)
				return E_INVALIDARG;
			*pSize = _size;
			return S_OK;
		}
		STDMETHODIMP IndexOf(float value, unsigned int *, boolean *)
		{
			return E_NOTIMPL;
		}

		STDMETHODIMP get_AmplitudeScale(ScaleType *pScale)
		{
			if (pScale == nullptr)
				return E_INVALIDARG;
			*pScale = _amplitudeScale;
			return S_OK;
		}
		STDMETHODIMP ConvertToLogAmplitude(float minValue, float maxValue, IScalarData **ppResult);
		STDMETHODIMP ApplyRiseAndFall(IScalarData *pPrevious, TimeSpan fallTime, TimeSpan riseTime, TimeSpan timeDelta, IScalarData **ppResult);
	};
}
