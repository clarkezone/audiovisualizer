#include "pch.h"
#include "Tracing.h"
#include <winrt/base.h>
#include <winrt/Windows.Foundation.Diagnostics.h>
#include <windows.media.core.h>
#include <winrt/Windows.Media.MediaProperties.h>


using namespace winrt::Windows::Foundation::Diagnostics;
using namespace winrt;

static LoggingChannel g_LogChannel{ nullptr };

LPCTSTR g_AudioVisualizerChannel = TEXT("AudioVisualizer");


void Trace::Initialize()
{
	if (!g_LogChannel) {
		g_LogChannel = LoggingChannel(g_AudioVisualizerChannel, LoggingChannelOptions());
	}
}

void Trace::ShutDown()
{
	if (g_LogChannel) {
		g_LogChannel.Close();
		g_LogChannel = nullptr;
	}
}

Trace::Activity Trace::Constructor(LPCTSTR szObjectName, void *pObject)
{
	auto fields = LoggingFields();
	fields.AddString(L"Name", szObjectName);
#ifdef _WIN64
	fields.AddInt64(L"pObject", (int64_t)pObject, LoggingFieldFormat::Hexadecimal);
#else
	fields.AddInt32(L"pObject", (int32_t)pObject, LoggingFieldFormat::Hexadecimal);
#endif
	return Activity(g_LogChannel.StartActivity(szObjectName, fields));
}

void Trace::MediaAnalyzer_SetInputType(IMFMediaType * pType, bool bTest)
{
	auto fields = LoggingFields();
	AddMediaTypeFields(pType, fields);
	fields.AddBoolean(L"TestOnly", bTest);
	g_LogChannel.LogEvent(L"MediaAnalyzer_SetInputType", fields);
}
void Trace::MediaAnalyzer_SetOutputType(IMFMediaType * pType, bool bTest)
{
	auto fields = LoggingFields();
	AddMediaTypeFields(pType, fields);
	fields.AddBoolean(L"TestOnly", bTest);
	g_LogChannel.LogEvent(L"MediaAnalyzer_SetOutputType", fields);
}

void Trace::MediaAnalyzer_ProcessMessage(MFT_MESSAGE_TYPE message)
{
	auto fields = LoggingFields();

	switch (message)
	{

	case MFT_MESSAGE_COMMAND_FLUSH:
		fields.AddString(L"Command", L"Flush");
		break;
	case MFT_MESSAGE_COMMAND_DRAIN:
		fields.AddString(L"Command", L"Drain");
		break;
	case MFT_MESSAGE_SET_D3D_MANAGER:
		fields.AddString(L"Command", L"D3DManager");
		break;
	case MFT_MESSAGE_DROP_SAMPLES:
		fields.AddString(L"Command", L"DropSamples");
		break;
	case MFT_MESSAGE_COMMAND_TICK:
		fields.AddString(L"Command", L"Tick");
		break;
	case MFT_MESSAGE_NOTIFY_BEGIN_STREAMING:
		fields.AddString(L"Notify", L"Drain");
		break;
	case MFT_MESSAGE_NOTIFY_END_STREAMING:
		fields.AddString(L"Notify", L"EndStreaming");
		break;
	case MFT_MESSAGE_NOTIFY_END_OF_STREAM:
		fields.AddString(L"Notify", L"EndOfStream");
		break;
	case MFT_MESSAGE_NOTIFY_START_OF_STREAM:
		fields.AddString(L"Notify", L"StartOfStream");
		break;
	case MFT_MESSAGE_NOTIFY_RELEASE_RESOURCES:
		fields.AddString(L"Notify", L"ReleaseResources");
		break;
	case MFT_MESSAGE_NOTIFY_REACQUIRE_RESOURCES:
		fields.AddString(L"Notify", L"ReAcquireResources");
		break;
	case MFT_MESSAGE_NOTIFY_EVENT:
		fields.AddString(L"Notify", L"Event");
		break;
	case MFT_MESSAGE_COMMAND_SET_OUTPUT_STREAM_STATE:
		fields.AddString(L"Command", L"SetOutputStreamState");
		break;
	case MFT_MESSAGE_COMMAND_FLUSH_OUTPUT_STREAM:
		fields.AddString(L"Command", L"FlushOutputStream");
		break;
	case MFT_MESSAGE_COMMAND_MARKER:
		fields.AddString(L"Command", L"Marker");
		break;
	}
	g_LogChannel.LogEvent(L"MediaAnalyzer_ProcessMessage", fields);
}

void Trace::MediaAnalyzer_ProcessInput(IMFSample * pSample)
{
	auto fields = LoggingFields();
	AddSampleFields(pSample, fields);
	g_LogChannel.LogEvent(L"MediaAnalyzer_ProcessInput", fields);
}
void Trace::MediaAnalyzer_ProcessOutput()
{
	auto fields = LoggingFields();
	g_LogChannel.LogEvent(L"MediaAnalyzer_ProcessOutput", fields);
}
void Trace::VisualizeControl_RecreateDevice(winrt::hresult_error const &err)
{
	auto fields = LoggingFields();
	fields.AddInt32(L"HResult",err.code(),LoggingFieldFormat::Hexadecimal);
	g_LogChannel.LogEvent(L"VisualizerControl_DeviceLost", fields);
}
void Trace::VisualizeControl_DrawLoopException(winrt::hresult_error const &err)
{
	auto fields = LoggingFields();
	fields.AddInt32(L"HResult", err.code(), LoggingFieldFormat::Hexadecimal);
	g_LogChannel.LogEvent(L"VisualizerControl_DeviceLost", fields);

}
LoggingActivity Trace::VisualizeControl_StartDraw(winrt::AudioVisualizer::VisualizationDataFrame const &frame, winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> const & time)
{
	auto fields = LoggingFields();
	if (time) {
		fields.AddTimeSpan(L"PresentationTime", time.Value());
	}
	else
	{
		fields.AddEmpty(L"PresentationTime");
	}
	if (frame) {
		fields.AddTimeSpan(L"Frame.Time", frame.Time());
		fields.AddTimeSpan(L"Frame.Duration", frame.Duration());
	}
	else
	{
		fields.AddEmpty(L"Frame.Time");
		fields.AddEmpty(L"Frame.Duration");
	}
	return g_LogChannel.StartActivity(L"VisualizationControl_Draw", fields);
}
void Trace::MediaAnalyzer_AnalyzerOutput(winrt::AudioVisualizer::VisualizationDataFrame frame)
{
	auto fields = LoggingFields();
	if (frame) {
		fields.AddTimeSpan(L"Time", frame.Time());
		fields.AddTimeSpan(L"Duration", frame.Duration());
	}
	g_LogChannel.LogEvent(L"MediaAnalyzer_AnalyzerOutput", fields);
}
void Trace::MediaAnalyzer_GetFrame(winrt::Windows::Foundation::IReference<winrt::Windows::Foundation::TimeSpan> time, winrt::AudioVisualizer::VisualizationDataFrame const & frame)
{
	auto fields = LoggingFields();
	if (time) {
		fields.AddTimeSpan(L"PresentationTime", time.Value());
	}
	else
	{
		fields.AddEmpty(L"PresentationTime");
	}
	if (frame) {
		fields.AddTimeSpan(L"Frame.Time", frame.Time());
		fields.AddTimeSpan(L"Frame.Duration", frame.Duration());
	}
	else
	{
		fields.AddEmpty(L"Frame.Time");
		fields.AddEmpty(L"Frame.Duration");
	}
	g_LogChannel.LogEvent(L"MediaAnalyzer_GetFrame", fields);
}

void Trace::AddSampleFields(IMFSample * pSample, winrt::Windows::Foundation::Diagnostics::LoggingFields &fields)
{
	LONGLONG sampleTime = 0;
	pSample->GetSampleTime(&sampleTime);
	fields.AddTimeSpan(L"Time", Windows::Foundation::TimeSpan(sampleTime));
	LONGLONG sampleDuration = 0;
	pSample->GetSampleDuration(&sampleDuration);
	fields.AddTimeSpan(L"Duration", Windows::Foundation::TimeSpan(sampleDuration));
	DWORD totalLength = 0;
	pSample->GetTotalLength(&totalLength);
	fields.AddUInt32(L"ByteLength", totalLength);
}

void Trace::AddMediaTypeFields(IMFMediaType * pType, winrt::Windows::Foundation::Diagnostics::LoggingFields &fields)
{
	com_ptr<ABI::Windows::Media::MediaProperties::IAudioEncodingProperties> abi_encoding;
	check_hresult(MFCreatePropertiesFromMediaType(pType, __uuidof(ABI::Windows::Media::MediaProperties::IAudioEncodingProperties), abi_encoding.put_void()));
	auto encoding = abi_encoding.as <Windows::Media::MediaProperties::AudioEncodingProperties>();
	fields.AddString(L"Type", encoding.Subtype());
	fields.AddUInt32(L"SampleRate", encoding.SampleRate());
	fields.AddUInt32(L"ChannelCount", encoding.ChannelCount());
	fields.AddUInt32(L"BitsPerSample", encoding.BitsPerSample());
}

