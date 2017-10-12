#include "pch.h"
#include "AudioMath.h"
#include <limits>

namespace AudioVisualizer
{
	namespace Math
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
				XMVECTOR vPrevious = pPrevious != nullptr ? pPrevious[vIndex] : g_XMZero;
				XMVECTOR vDelta = pCurrent[vIndex] - vPrevious;
				XMVECTOR vSelector = XMVectorLess(DirectX::g_XMZero, vDelta);
				XMVECTOR vFactors = XMVectorSelect(vFallExp, vRiseExp, vSelector);
				pResult[vIndex] = vPrevious + vFactors * vDelta;
			}
		}

		void SpectrumLinearTransform(const float *pInput, size_t inputSize, float *pOutput, size_t outputSize)
		{
			// TODO: handle case when outputSize > inputSize
			if (inputSize == outputSize)
			{
				memcpy(pOutput, pInput, inputSize * sizeof(float));
				return;
			}

			float inStep = (float)inputSize / (float)outputSize;
			float inIndex = 0;
			float nextInIndex = inIndex + inStep;

			for (size_t outIndex = 0; outIndex < outputSize; outIndex++)
			{
				int inValueIntIndex = (int)floor(inIndex);
				int inValueIntNextIndex = (int)floor(nextInIndex);

				float sum = 0;
				for (int index = inValueIntIndex + 1; index < inValueIntNextIndex && index < (int)inputSize; index++)
				{
					sum += pInput[index];
				}
				sum += pInput[inValueIntIndex] * (1 - inIndex + (float)inValueIntIndex);
				if (inValueIntNextIndex < (int)inputSize)
					sum += pInput[inValueIntNextIndex] * (nextInIndex - (float)inValueIntNextIndex);

				pOutput[outIndex] = sum;

				inIndex = nextInIndex;
				nextInIndex = inIndex + inStep;
			}

		}
		void SpectrumLogTransform(const float * pSource, size_t inputSize, float fromIndex, float toIndex, float * pDest, size_t outputSize)
		{
			float inStep = powf(toIndex / fromIndex, 1 / ((float)outputSize - 1));

			float inIndex = 0;
			float nextInIndex = inIndex * inStep;

			/*
			for (size_t outIndex = 0; outIndex < outputSize; outIndex++)
			{
				if (nextInIndex - inIndex > 1)
				{
					int inValueIntIndex = (int)floor(inIndex);
					int inValueIntNextIndex = (int)floor(nextInIndex);

					float sum = 0;
					for (int index = inValueIntIndex + 1; index < inValueIntNextIndex && index < (int)inputSize; index++)
					{
						sum += pInput[index];
					}
					sum += pInput[inValueIntIndex] * (1 - inIndex + (float)inValueIntIndex);
					if (inValueIntNextIndex < (int)inputSize)
						sum += pInput[inValueIntNextIndex] * (nextInIndex - (float)inValueIntNextIndex);

					pOutput[outIndex] = sum;

					inIndex = nextInIndex;
					nextInIndex = inIndex + inStep;
				}
				else
				{
					// TODO:
				}
			}*/
		}
	}
}