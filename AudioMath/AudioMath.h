#pragma once
#include <DirectXMath.h>
#include <Windows.h>

namespace AudioMath
{
	extern float g_fDbScaler;
	extern float g_fDbInverseScaler;

	void ConvertToLogarithmic(const DirectX::XMVECTOR *pSource, DirectX::XMVECTOR *pResult, size_t count, float clampLow = -100.0f, float clampHigh = 0.0f,float scaler = g_fDbScaler /* This is 20 / log10(e) for db conversion */);
	void ConvertToLinear(const DirectX::XMVECTOR *pSource, DirectX::XMVECTOR *pResult, size_t count, float scaler = g_fDbInverseScaler /* This is log10(e)/ 20.0f for db conversion */);
	void ApplyRiseAndFall(const DirectX::XMVECTOR *pPrevious, const DirectX::XMVECTOR *pCurrent,DirectX::XMVECTOR *pResult, size_t count,float riseByT,float fallByT);

}