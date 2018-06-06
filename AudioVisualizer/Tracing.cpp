#include "pch.h"
#include "Tracing.h"
#include <winrt/Windows.Foundation.Diagnostics.h>

namespace AudioVisualizer::Diagnostics
{
	using namespace winrt::Windows::Foundation::Diagnostics;

	static LoggingChannel g_LogChannel { nullptr };

	#define LOG_CHANNEL_NAME L"AudioVisualizer-MftAnalyzer"

	void Initialize()
	{
		if (!g_LogChannel) {
			g_LogChannel = LoggingChannel(LOG_CHANNEL_NAME, LoggingChannelOptions());
		}
	}

	void ShutDown()
	{
		if (g_LogChannel) {
			g_LogChannel.Close();
			g_LogChannel = nullptr;
		}
	}

	void Trace_ctor(LPCTSTR szObjectName)
	{
		auto fields = LoggingFields();
		fields.AddString(L"Name", szObjectName);
		g_LogChannel.LogEvent(L"ctor", fields); 
	}
}