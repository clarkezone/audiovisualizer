#pragma once
#include "ErrorHandling.h"
#include "trace.h"
#include "BaseVisualizer.h"
#include <vector>
#include <wrl.h>

namespace AudioVisualizer
{
	using namespace Microsoft::WRL;
	using namespace Microsoft::WRL::Wrappers;
	using namespace ABI::AudioVisualizer;
	using namespace ABI::Windows::UI::Xaml::Controls;
	using namespace ABI::Windows::UI::Xaml;
	using namespace ABI::Microsoft::Graphics::Canvas;
	using namespace ABI::Windows::Foundation;

	class DiscreteVUBar : public 
		RuntimeClass<IVisualizer,IBarVisualizer,IDiscreteVUBar,ComposableBase<>>, 
		public BaseVisualizer<DiscreteVUBar>
	{
		InspectableClass(RuntimeClass_AudioVisualizer_DiscreteVUBar, BaseTrust);
		Orientation _orientation;
		std::vector<MeterBarLevel> _levels;
		UINT32 _channelIndex;
		Thickness _elementMargin;
		Color _unlitElement;
		float _minAmp;
		float _maxAmp;
		Size _controlSize;
		bool _displayPeak;
		EventRegistrationToken _sizeChangedToken;
		SRWLock _lock;

	public:
		DiscreteVUBar();
		~DiscreteVUBar();

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
			return S_OK;
		}

		STDMETHODIMP get_Levels(UINT32 *pcElements, MeterBarLevel **ppLevels)
		{
			if (ppLevels == nullptr || pcElements == nullptr)
				return E_POINTER;
			auto lock = _lock.LockShared();
			*pcElements = _levels.size();
			*ppLevels = (MeterBarLevel *) CoTaskMemAlloc(sizeof(MeterBarLevel) * _levels.size());
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
			return S_OK;
		}

		STDMETHODIMP get_ChannelIndex(UINT32 *pIndex)
		{
			if (pIndex == nullptr)
				return E_POINTER;
			*pIndex = _channelIndex;
			return S_OK;
		}
		STDMETHODIMP put_ChannelIndex(UINT32 index)
		{
			auto lock = _lock.LockExclusive();
			_channelIndex = index;
			return S_OK;
		}


		STDMETHODIMP get_RelativeElementMargin(Thickness *pElementMargin)
		{
			if (pElementMargin == nullptr)
				return E_POINTER;
			*pElementMargin = _elementMargin;
			return S_OK;
		}
		STDMETHODIMP put_RelativeElementMargin(Thickness elementMargin)
		{
			auto lock = _lock.LockExclusive();
			_elementMargin = elementMargin;
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

		STDMETHODIMP get_DisplayPeak(boolean *pValue)
		{
			if (pValue == nullptr)
				return E_POINTER;
			*pValue = _displayPeak;
			return S_OK;
		}

		STDMETHODIMP put_DisplayPeak(boolean value)
		{
			_displayPeak = value;
			return S_OK;
		}

		virtual HRESULT OnDraw(ICanvasDrawingSession *pSession, IVisualizationDataFrame *pDataFrame, IReference<TimeSpan> *pPresentationTime);
	};

}

