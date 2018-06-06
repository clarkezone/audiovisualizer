#pragma once

namespace AudioVisualizer
{
	namespace Diagnostics
	{
		void Initialize();
		void ShutDown();
		void Trace_ctor(LPCTSTR szObjectName);
	}
}
