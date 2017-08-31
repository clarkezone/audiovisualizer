#include "pch.h"
#include "VisualizationData.h"

using namespace Microsoft::WRL;

namespace AudioAnalyzer
{
	CVisualizationData::CVisualizationData()
	{
		_time = 10000000L;
		_channels = 2;
	}
	HRESULT CVisualizationData::RuntimeClassInitialize()
	{
		return S_OK;
	}
}
