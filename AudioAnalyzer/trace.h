#pragma once
#include <mfapi.h>

namespace AudioAnalyzer
{
	namespace Diagnostics
	{
		class Trace
		{
		public:
			static HRESULT Initialize();
			static HRESULT Shutdown();
			static HRESULT Log_SetMediaType(IMFMediaType *pType,HRESULT result);
		};
	}
}