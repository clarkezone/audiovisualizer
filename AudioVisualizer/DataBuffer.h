#pragma once
#include <DirectXMath.h>
#include <Unknwn.h>

namespace AudioVisualizer
{
	class DataBuffer
	{
	public:
		virtual DirectX::XMVECTOR *GetBuffer()=0;
	};
}
