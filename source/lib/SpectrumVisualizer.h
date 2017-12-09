#pragma once
#include "ErrorHandling.h"
#include "trace.h"
#include "BaseVisualizer.h"
#include "SpectrumData.h"
#include <vector>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Microsoft::Graphics::Canvas;
using namespace ABI::Windows::Foundation;

namespace AudioVisualizer
{
	class SpectrumVisualizer : public RuntimeClass<
		IVisualizer,
		IBarVisualizer,
		ISpectrumVisualizer,
		ComposableBase<>>,
		BaseVisualizer<SpectrumVisualizer>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_SpectrumVisualizer,BaseTrust);

		Orientation _orientation;
		std::vector<MeterBarLevel> _levels;
		UINT32 _channelCount;
		Size _elementSize;
		Thickness _elementMargin;
		Color _unlitElement;
		TimeSpan _riseTime;
		TimeSpan _fallTime;
		UINT32 _barCount;
		float _minFrequency;
		float _maxFrequency;
		ScaleType _frequencyScale;
		std::vector<float> _channelMap;

		Size _calculatedSize;

		SRWLock _lock;

		ComPtr<SpectrumData> _emptySpectrum;
		ComPtr<SpectrumData> _previousSpectrum;

	public:
		SpectrumVisualizer();
		~SpectrumVisualizer();
		// IVisualizer implementation
		STDMETHODIMP get_Source(ABI::AudioVisualizer::IVisualizationSource **ppSource)
		{
			if (ppSource == nullptr)
				return E_POINTER;
			*ppSource = _visualizationSource.Get();
			return S_OK;
		}

		STDMETHODIMP put_Source(ABI::AudioVisualizer::IVisualizationSource *pSource)
		{
			_visualizationSource = pSource;
			return S_OK;
		}

		STDMETHODIMP get_BackgroundColor(Color *pColor)
		{
			if (pColor == nullptr)
				return E_POINTER;
			*pColor = _drawingSessionBackgroundColor;
			return S_OK;
		}
		STDMETHODIMP put_BackgroundColor(Color color)
		{
			_drawingSessionBackgroundColor = color;
			return S_OK;
		}
		// IBarVisualizer implementation
		STDMETHODIMP get_Orientation(Orientation *value)
		{
			if (value == nullptr)
				return E_POINTER;
			*value = _orientation;
			return S_OK;
		}
		STDMETHODIMP put_Orientation(Orientation value)
		{
			auto lock = _lock.LockExclusive();
			_orientation = value;
			ResizeControl();
			return S_OK;
		}

		STDMETHODIMP get_Levels(UINT32 *pcElements, MeterBarLevel **ppLevels)
		{
			if (ppLevels == nullptr || pcElements == nullptr)
				return E_POINTER;
			auto lock = _lock.LockShared();
			*pcElements = _levels.size();
			*ppLevels = (MeterBarLevel *)CoTaskMemAlloc(sizeof(MeterBarLevel) * _levels.size());
			if (*ppLevels == nullptr)
				return E_OUTOFMEMORY;
			for (size_t i = 0; i < _levels.size(); i++)
			{
				*ppLevels[i] = _levels[i];
			}
			return S_OK;
		}

		STDMETHODIMP put_Levels(UINT32 cElements, MeterBarLevel *pLevels)
		{
			if (pLevels == nullptr)
				return E_POINTER;
			auto lock = _lock.LockExclusive();
			_levels.resize(cElements);
			for (size_t i = 0; i < cElements; i++)
			{
				_levels[i] = pLevels[i];
			}
			ResizeControl();
			return S_OK;
		}

		STDMETHODIMP get_ChannelCount(UINT32 *pChannels)
		{
			if (pChannels == nullptr)
				return E_POINTER;
			*pChannels = _channelCount;
			return S_OK;
		}
		STDMETHODIMP put_ChannelCount(UINT32 channels)
		{
			auto lock = _lock.LockExclusive();
			_channelCount = channels;
			ResizeControl();
			return S_OK;
		}

		STDMETHODIMP get_ChannelMapping(UINT32 *pcCount, float **ppValue)
		{
			if (pcCount == nullptr || ppValue == nullptr)
				return E_POINTER;
			*pcCount = 0;
			*ppValue = nullptr;
			return S_OK;
		}
		STDMETHODIMP put_ChannelMapping(UINT32 cCount, float *pValue)
		{
			std::vector<float> mapping(cCount);
			return S_OK;
		}

		STDMETHODIMP get_ElementSize(Size *pElementSize)
		{
			if (pElementSize == nullptr)
				return E_POINTER;
			*pElementSize = _elementSize;
			return S_OK;
		}
		STDMETHODIMP put_ElementSize(Size elementSize)
		{
			auto lock = _lock.LockExclusive();
			_elementSize = elementSize;
			ResizeControl();
			return S_OK;
		}

		STDMETHODIMP get_ElementMargin(Thickness *pElementMargin)
		{
			if (pElementMargin == nullptr)
				return E_POINTER;
			*pElementMargin = _elementMargin;
			return S_OK;
		}
		STDMETHODIMP put_ElementMargin(Thickness elementMargin)
		{
			auto lock = _lock.LockExclusive();
			_elementMargin = elementMargin;
			ResizeControl();
			return S_OK;
		}

		STDMETHODIMP get_UnlitElement(Color *pColor)
		{
			if (pColor == nullptr)
				return E_POINTER;
			*pColor = _unlitElement;
			return S_OK;
		}
		STDMETHODIMP put_UnlitElement(Color color)
		{
			auto lock = _lock.LockExclusive();
			_unlitElement = color;
			return S_OK;
		}

		STDMETHODIMP get_RiseTime(TimeSpan *pTime)
		{
			if (pTime == nullptr)
				return E_POINTER;
			*pTime = _riseTime;
			return S_OK;
		}
		STDMETHODIMP put_RiseTime(TimeSpan time)
		{
			_riseTime = time;
			return S_OK;
		}
		STDMETHODIMP get_FallTime(TimeSpan *pTime)
		{
			if (pTime == nullptr)
				return E_POINTER;
			*pTime = _fallTime;
			return S_OK;
		}
		STDMETHODIMP put_FallTime(TimeSpan time)
		{
			_riseTime = time;
			return S_OK;
		}

		// ISpectrumVisualizer implementation
		STDMETHODIMP get_BarCount(UINT32 *pBarCount)
		{
			if (pBarCount == nullptr)
				return E_POINTER;
			*pBarCount = _barCount;
			return S_OK;
		}
		STDMETHODIMP put_BarCount(UINT32 barCount)
		{
			auto lock = _lock.LockExclusive();
			_barCount = barCount;
			ResizeControl();
			return S_OK;
		}
		STDMETHODIMP get_MinFrequency(float *pValue)
		{
			if (pValue == nullptr)
				return E_POINTER;
			*pValue = _minFrequency;
			return S_OK;
		}
		STDMETHODIMP put_MinFrequency(float value)
		{
			auto lock = _lock.LockExclusive();
			_minFrequency = value;
			Rescale();
			return S_OK;
		}
		STDMETHODIMP get_MaxFrequency(float *pValue)
		{
			if (pValue == nullptr)
				return E_POINTER;
			*pValue = _maxFrequency;
			return S_OK;
		}
		STDMETHODIMP put_MaxFrequency(float value)
		{
			auto lock = _lock.LockExclusive();
			_maxFrequency = value;
			Rescale();
			return S_OK;
		}
		STDMETHODIMP get_Scale(ScaleType *pValue)
		{
			if (pValue == nullptr)
				return E_POINTER;
			*pValue = _frequencyScale;
			return S_OK;
		}
		STDMETHODIMP put_Scale(ScaleType value)
		{
			auto lock = _lock.LockExclusive();
			_frequencyScale = value;
			Rescale();
			return S_OK;
		}
private:
		void ResizeControl();
		HRESULT Rescale();
		virtual HRESULT OnDraw(ICanvasDrawingSession *pSession, IVisualizationDataFrame *pDataFrame, IReference<TimeSpan> *pPresentationTime);


	};
}

