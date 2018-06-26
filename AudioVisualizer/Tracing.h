#pragma once
#include<winrt/windows.foundation.diagnostics.h>
#include "VisualizationDataFrame.h"

using namespace winrt::Windows::Foundation::Diagnostics;
using namespace winrt::AudioVisualizer;

#ifdef _TRACE_

class Trace
{
private:
	static void AddMediaTypeFields(IMFMediaType * pType, winrt::Windows::Foundation::Diagnostics::LoggingFields &fields);
	static void AddSampleFields(IMFSample * pSample, winrt::Windows::Foundation::Diagnostics::LoggingFields &fields);
public:
	struct Activity {
	private:
		LoggingActivity _activity;
	public:
		Activity(LoggingActivity const &activity) : _activity(activity) {}
		~Activity() {
			_activity.StopActivity(_activity.Name());
		}
	};
	static void Initialize();
	static void ShutDown();
	static Activity  Constructor(LPCTSTR szObjectName,void *pObject);
	static void MediaAnalyzer_SetInputType(IMFMediaType *pType, bool bTest);
	static void MediaAnalyzer_SetOutputType(IMFMediaType *pType, bool bTest);
	static void MediaAnalyzer_ProcessMessage(MFT_MESSAGE_TYPE message);
	static void MediaAnalyzer_ProcessInput(IMFSample *pSample);
	static void MediaAnalyzer_ProcessOutput();
	static void MediaAnalyzer_AnalyzerOutput(VisualizationDataFrame frame);
	static void MediaAnalyzer_GetFrame(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> time, winrt::AudioVisualizer::VisualizationDataFrame const &frame);
	static void MediaAnalyzer_OutputQueueAdd(VisualizationDataFrame const &frame, VisualizationDataFrame const &front, VisualizationDataFrame const &back, size_t queueSize);
	static void MediaAnalyzer_OutputQueueRemove(VisualizationDataFrame const &frame, size_t queueSize);
	static void MediaAnalyzer_OutputQueueGet(winrt::Windows::Foundation::TimeSpan time, VisualizationDataFrame const &front, VisualizationDataFrame const &back, size_t queueSize);
	static void MediaAnalyzer_OutputQueueBehind(winrt::Windows::Foundation::TimeSpan time);
	static void MediaAnalyzer_OutputQueueItemFound(winrt::Windows::Foundation::TimeSpan time);
	static void MediaAnalyzer_OutputQueueTest(winrt::Windows::Foundation::TimeSpan queueItem, winrt::Windows::Foundation::TimeSpan time, bool isQueueItemBefore, bool isQueueItemAfter);
	static void MediaAnalyzer_OutputQueueClear();

	static void AudioAnalyzer_ProcessInput(winrt::Windows::Media::AudioFrame const &frame);
	static void AudioAnalyzer_RunAsync();
	static LoggingActivity AudioAnalyzer_Calculate();
	static void AudioAnalyzer_SeedFromPosition(int64_t position);
	static void AudioAnalyzer_SeedFromStream(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const &time,int64_t position);

	static void VisualizeControl_RecreateDevice(winrt::hresult_error const& err);
	static void VisualizeControl_DrawLoopException(winrt::hresult_error const &err);
	static LoggingActivity VisualizeControl_StartDraw(winrt::AudioVisualizer::VisualizationDataFrame const &frame,winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const &time);

	static void VisualizerAudioEffect_ProcessFrame(winrt::Windows::Media::AudioFrame const &frame);
	static void VisualizerAudioEffect_OnAnalyzerOutput(VisualizationDataFrame  const&frame);
	static void VisualizerAudioEffect_GetData(VisualizationDataFrame  const&frame);
	static void VisualizerAudioEffect_DiscardQueuedFrames();
};

#endif