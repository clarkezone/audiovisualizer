#pragma once
#include "AudioVisualizer.abi.h"
#include <limits>
#include <windows.ui.xaml.data.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::AudioVisualizer;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml::Data;

namespace AudioVisualizer
{
	class SourceConverter : public RuntimeClass<
		IVisualizationSource,
		ISourceConverter>
	{
		ComPtr<IVisualizationSource> _source;
		AnalyzerType _analyzerTypes;
		ComPtr<IReference<UINT32>> _elementCount;
		ComPtr<IReference<UINT32>> _channelCount;
		ComPtr<IReference<TimeSpan>> _riseTime;
		ComPtr<IReference<TimeSpan>> _fallTime;
		ComPtr<IReference<float>> _minFrequency;
		ComPtr<IReference<float>> _maxFrequency;
		ComPtr<IReference<ScaleType>> _frequencyScale;
		ComPtr<IVisualizationDataFrame> _cachedSourceFrame;
		ComPtr<IVisualizationDataFrame> _cachedOutputFrame;
		bool _bCacheData;

		ComPtr<ISpectrumData> _previousSpectrum;

		CriticalSection _csLock;

		EventSource<ITypedEventHandler<IVisualizationSource*, HSTRING>> _configurationChangedList;
		HRESULT RaiseConfigurationChanged(wchar_t *wszPropertyName)
		{
			return _configurationChangedList.InvokeAll(this, HStringReference(wszPropertyName).Get());
		}

		InspectableClass(RuntimeClass_AudioVisualizer_SourceConverter, BaseTrust);

		HRESULT ProcessFrame(IVisualizationDataFrame *pSource, IVisualizationDataFrame **ppResult);
		HRESULT TryConstructingSourceFrame(IVisualizationDataFrame **ppResult);
		HRESULT CloneSpectrum(ISpectrumData *pSource, ISpectrumData **ppResult);
		HRESULT CloneScalarData(IScalarData *pSource, IScalarData **ppResult);
		HRESULT CloneFromFrame(IVisualizationDataFrame *pSource, IVisualizationDataFrame **ppTarget);
		HRESULT ApplyFrequencyTransforms(ComPtr<ISpectrumData> &data);
		HRESULT ApplyRiseAndFall(ComPtr<ISpectrumData> &data);

	public:
		SourceConverter();
		~SourceConverter();

		STDMETHODIMP GetData(IVisualizationDataFrame **ppResult);
		STDMETHODIMP get_IsSuspended(boolean *pbIsSuspended)
		{
			if (_source == nullptr)
				return E_NOT_VALID_STATE;
			return _source->get_IsSuspended(pbIsSuspended);
		}
		STDMETHODIMP put_IsSuspended(boolean bIsSuspended)
		{
			if (_source == nullptr)
				return E_NOT_VALID_STATE;
			return _source->put_IsSuspended(bIsSuspended);
		}
		STDMETHODIMP get_Fps(float *pFramesPerSecond)
		{
			if (pFramesPerSecond == nullptr)
				return E_POINTER;
			if (_source == nullptr)
				return E_NOT_VALID_STATE;
			return _source->get_Fps(pFramesPerSecond);

		}
		STDMETHODIMP put_Fps(float framesPerSecond)
		{
			if (_source == nullptr)
				return E_NOT_VALID_STATE;
			return _source->put_Fps(framesPerSecond);

		}
		STDMETHODIMP get_AnalyzerTypes(AnalyzerType *pResult)
		{
			if (pResult == nullptr)
				return E_POINTER;
			*pResult = _analyzerTypes;
			return S_OK;
		}
		STDMETHODIMP put_AnalyzerTypes(AnalyzerType types)
		{
			auto lock = _csLock.Lock();
			if (_source == nullptr)
				return E_NOT_VALID_STATE;
			_analyzerTypes = types;
			return S_OK;
		}
		STDMETHODIMP get_PresentationTime(IReference<TimeSpan> **ppTime)
		{
			if (ppTime == nullptr)
				return E_POINTER;
			if (_source == nullptr)
				return E_NOT_VALID_STATE;
			return _source->get_PresentationTime(ppTime);
		}
		STDMETHODIMP get_PlaybackState(SourcePlaybackState *pState)
		{
			if (pState == nullptr)
				return E_POINTER;
			if (_source == nullptr)
				return E_NOT_VALID_STATE;
			return _source->get_PlaybackState(pState);
		}

		STDMETHODIMP get_Source(IVisualizationSource **ppSource)
		{
			if (ppSource == nullptr)
				return E_POINTER;
			*ppSource = _source.Get();
			return S_OK;
		}

		STDMETHODIMP put_Source(IVisualizationSource *pSource)
		{
			auto lock = _csLock.Lock();
			if (pSource == nullptr)
				return E_POINTER;
			_source = pSource;
			_cachedOutputFrame = nullptr;
			_cachedOutputFrame = nullptr;
			RaiseConfigurationChanged(L"Source");
			return S_OK;
		}
		STDMETHODIMP get_FrequencyCount(IReference<UINT32> **ppcElements)
		{
			if (ppcElements == nullptr)
				return E_POINTER;
			return _elementCount.CopyTo(ppcElements);
		}
		STDMETHODIMP put_FrequencyCount(IReference<UINT32> *pcElements)
		{
			auto lock = _csLock.Lock();
			if (pcElements == nullptr)
				return E_POINTER;
			if (pcElements != nullptr)
			{
				UINT32 value = 0;
				pcElements->get_Value(&value);
				if (value == 0)
					return E_INVALIDARG;
			}
			_elementCount = pcElements;
			RaiseConfigurationChanged(L"FrequencyCount");
			return S_OK;
		}
		STDMETHODIMP get_ChannelCount(IReference<UINT32> **ppcElements)
		{
			if (ppcElements == nullptr)
				return E_POINTER;
			return _channelCount.CopyTo(ppcElements);
		}
		STDMETHODIMP put_ChannelCount(IReference<UINT32> *pcElements)
		{
			auto lock = _csLock.Lock();
			if (pcElements == nullptr)
				return E_POINTER;
			if (pcElements != nullptr)
			{
				UINT32 value = 0;
				pcElements->get_Value(&value);
				if (value == 0)
					return E_INVALIDARG;
			}
			_channelCount = pcElements;
			RaiseConfigurationChanged(L"ChannelCount");
			return S_OK;
		}

		STDMETHODIMP get_MinFrequency(IReference<float> **ppValue)
		{
			if (ppValue == nullptr)
				return E_POINTER;
			return _minFrequency.CopyTo(ppValue);
		}
		STDMETHODIMP put_MinFrequency(IReference<float> *pValue)
		{
			auto lock = _csLock.Lock();
			if (pValue == nullptr)
				return E_POINTER;
			if (pValue != nullptr)
			{
				float value = 0;
				pValue->get_Value(&value);
				ScaleType scale = ScaleType::Linear;
				if (_frequencyScale != nullptr)
					_frequencyScale->get_Value(&scale);
				float maxFreq = std::numeric_limits<float>::max();
				if (_maxFrequency != nullptr)
					_maxFrequency->get_Value(&maxFreq);

				if ((scale == ScaleType::Logarithmic && value == 0) || value < 0 || value >= maxFreq)
					return E_INVALIDARG;
			}
			_minFrequency = pValue;
			RaiseConfigurationChanged(L"MinFrequency");
			return S_OK;
		}
		STDMETHODIMP get_MaxFrequency(IReference<float> **ppValue)
		{
			if (ppValue == nullptr)
				return E_POINTER;
			return _maxFrequency.CopyTo(ppValue);
		}
		STDMETHODIMP put_MaxFrequency(IReference<float> *pValue)
		{
			auto lock = _csLock.Lock();
			if (pValue == nullptr)
				return E_POINTER;
			if (pValue != nullptr)
			{
				float value = 0;
				pValue->get_Value(&value);
				if (value < 0)
					return E_INVALIDARG;
				if (_minFrequency != nullptr)
				{
					float minFrequency = 0.0f;
					_minFrequency->get_Value(&minFrequency);
					if (minFrequency >= value)
						return E_INVALIDARG;
				}
			}
			_maxFrequency = pValue;
			RaiseConfigurationChanged(L"MaxFrequency");
			return S_OK;
		}

		STDMETHODIMP get_FrequencyScale(IReference<ScaleType> **ppValue)
		{
			if (ppValue == nullptr)
				return E_POINTER;
			return _frequencyScale.CopyTo(ppValue);
		}
		STDMETHODIMP put_FrequencyScale(IReference<ScaleType> *pValue)
		{
			auto lock = _csLock.Lock();
			if (pValue == nullptr)
				return E_POINTER;
			if (pValue != nullptr)
			{
				ScaleType value = ScaleType::Linear;
				pValue->get_Value(&value);
				if (value == ScaleType::Logarithmic && _minFrequency != nullptr)
				{
					float minFrequency = 0;
					_minFrequency->get_Value(&minFrequency);
					if (_minFrequency == 0)
						return E_INVALIDARG;
				}
			}
			_frequencyScale = pValue;
			RaiseConfigurationChanged(L"FrequencyScale");
			return S_OK;
		}

		STDMETHODIMP get_RiseTime(IReference<TimeSpan> **ppTime)
		{
			if (ppTime == nullptr)
				return E_POINTER;
			return _riseTime.CopyTo(ppTime);
		}
		STDMETHODIMP put_RiseTime(IReference<TimeSpan> *pTime)
		{
			auto lock = _csLock.Lock();
			if (pTime == nullptr)
				return E_POINTER;
			if (pTime != nullptr)
			{
				TimeSpan value = { 0 };
				pTime->get_Value(&value);
				if (value.Duration == 0)
					return E_INVALIDARG;
			}
			_riseTime = pTime;
			RaiseConfigurationChanged(L"RiseTime");
			return S_OK;
		}

		STDMETHODIMP get_FallTime(IReference<TimeSpan> **ppTime)
		{
			if (ppTime == nullptr)
				return E_POINTER;
			return _fallTime.CopyTo(ppTime);
		}
		STDMETHODIMP put_FallTime(IReference<TimeSpan> *pTime)
		{
			auto lock = _csLock.Lock();
			if (pTime == nullptr)
				return E_POINTER;
			if (pTime != nullptr)
			{
				TimeSpan value = { 0 };
				pTime->get_Value(&value);
				if (value.Duration == 0)
					return E_INVALIDARG;
			}
			_fallTime = pTime;
			RaiseConfigurationChanged(L"FallTime");
			return S_OK;
		}

		STDMETHODIMP add_ConfigurationChanged(
			ITypedEventHandler<IVisualizationSource *,HSTRING> *value,
			EventRegistrationToken *token)
		{
			return _configurationChangedList.Add(value, token);
		}
		STDMETHODIMP remove_ConfigurationChanged(
			EventRegistrationToken token)
		{
			return _configurationChangedList.Remove(token);
		}
	};
}
