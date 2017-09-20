#include "pch.h"
#include "AudioMath.h"
#include <limits>

namespace AudioMath
{
	float g_fDbScaler = 8.6858896f;
	float g_fDbInverseScaler = 0.1151293f;

	void ConvertToLogarithmic(const DirectX::XMVECTOR * pSource, DirectX::XMVECTOR * pResult, size_t count, float clampLow, float clampHigh, float scaler)
	{
		using namespace DirectX;
		XMVECTOR vLogScaler = XMVectorReplicate(scaler);
		XMVECTOR vClampLow = XMVectorReplicate(clampLow);
		XMVECTOR vClampHigh = XMVectorReplicate(clampHigh);
		XMVECTOR vFloatMax = XMVectorReplicate(std::numeric_limits<float>::max());
		for (size_t vIndex = 0; vIndex < count; vIndex++)
		{
			// Clamp first input data between 0 - FLT_MAX to avoid log(negative_number)
			XMVECTOR vClampedSource = XMVectorClamp(pSource[vIndex], DirectX::g_XMZero, vFloatMax);
			XMVECTOR vLog = XMVectorLogE(vClampedSource) * vLogScaler;
			pResult[vIndex] = XMVectorClamp(vLog, vClampLow, vClampHigh);	// Clamp output value
		}
	}

	void ConvertToLinear(const DirectX::XMVECTOR * pSource, DirectX::XMVECTOR * pResult, size_t count, float scaler)
	{
		using namespace DirectX;
		XMVECTOR vExpScaler = XMVectorReplicate(scaler);
		for (size_t vIndex = 0; vIndex < count; vIndex++)
		{
			pResult[vIndex] = XMVectorExpE(pSource[vIndex] * vExpScaler);
		}
	}

	void ApplyRiseAndFall(const DirectX::XMVECTOR *pPrevious, 
							const DirectX::XMVECTOR *pCurrent, 
							DirectX::XMVECTOR *pResult, 
							size_t count, float riseByT, float fallByT)
	{
		using namespace DirectX;
		XMVECTOR vRiseExp = g_XMOne - XMVectorExpE(XMVectorReplicate(-riseByT));
		XMVECTOR vFallExp = g_XMOne - XMVectorExpE(XMVectorReplicate(-fallByT));

		for (size_t vIndex = 0; vIndex < count; vIndex++)
		{
			XMVECTOR vDelta = pCurrent[vIndex] - pPrevious[vIndex];
			XMVECTOR vSelector = XMVectorLess(DirectX::g_XMZero,vDelta);
			XMVECTOR vFactors = XMVectorSelect(vRiseExp, vFallExp, vSelector);
			pResult[vIndex] = pPrevious[vIndex] + vFactors * vDelta;
		}
	}

}
