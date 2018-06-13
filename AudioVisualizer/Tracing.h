#pragma once
#include<winrt/windows.foundation.diagnostics.h>
#include "VisualizationDataFrame.h"

using namespace winrt::Windows::Foundation::Diagnostics;

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
	static void MediaAnalyzer_AnalyzerOutput(winrt::AudioVisualizer::VisualizationDataFrame frame);
	static void MediaAnalyzer_GetFrame(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> time, winrt::AudioVisualizer::VisualizationDataFrame const &frame);

	static void VisualizeControl_RecreateDevice(winrt::hresult_error const& err);
	static void VisualizeControl_DrawLoopException(winrt::hresult_error const &err);
	static LoggingActivity VisualizeControl_StartDraw(winrt::AudioVisualizer::VisualizationDataFrame const &frame,winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const &time);
};

