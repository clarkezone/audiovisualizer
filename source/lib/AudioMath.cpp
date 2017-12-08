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
		
		namespace Internal
		{
			float _AreaOfElementFromStart(const float *pInput,size_t inputSize,int elementIndex,float x)
			{
				float value = elementIndex < (int)inputSize && elementIndex >= 0 ? pInput[elementIndex] : 0;
				float nextValue = elementIndex + 1 < (int) inputSize && elementIndex + 1 >=0 ? pInput[elementIndex + 1] : 0;
				return (x*(2 * value + x * (nextValue - value))) / 2;
			}

			float _AreaOfElementToEnd(const float *pInput,size_t inputSize,int elementIndex,float x)
			{
				float value = elementIndex < (int) inputSize && elementIndex >= 0 ? pInput[elementIndex] : 0;
				float nextValue = elementIndex + 1 < (int) inputSize && elementIndex + 1 >= 0 ? pInput[elementIndex + 1] : 0;
				return ((1 - x)*(value + nextValue + x * (nextValue - value))) / 2;
			}
			float _AreaOfElement(const float *pInput,size_t inputSize,int elementIndex,float x1,float x2)
			{
				float value = elementIndex < (int) inputSize && elementIndex >= 0 ? pInput[elementIndex] : 0;
				float nextValue = elementIndex + 1 < (int) inputSize && elementIndex + 1 >= 0 ? pInput[elementIndex + 1] : 0;
				return ((x1 - x2)*(x1*(value - nextValue) - 2 * value + x2 * (value - nextValue))) / 2;
			}
		}

		void SpectrumTransform(const float *pInput, size_t inputSize, float fromIndex, float toIndex, float *pOutput, size_t outputSize,bool bLinear)
		{
			float inStep = bLinear == true ? (toIndex - fromIndex) / (float)outputSize : powf(toIndex/fromIndex,1/(float)(outputSize -1));
			float inIndex = fromIndex;
			float nextInIndex = bLinear == true ? inIndex + inStep : inIndex * inStep;

			for (size_t outIndex = 0; outIndex < outputSize; outIndex++)
			{
				int inValueIntIndex = (int)floor(inIndex);
				int inValueIntNextIndex = (int)floor(nextInIndex);

				float outValue = 0;

				if (inValueIntNextIndex > inValueIntIndex)
				{
					// Add up the full elements
					for (int index = inValueIntIndex + 1; index < inValueIntNextIndex && index < (int)inputSize; index++)
					{
						outValue += index < (int)inputSize && index >= 0 ? pInput[index] : 0;
					}
					// Add fractional parts
					outValue += Internal::_AreaOfElementToEnd(pInput, inputSize, inValueIntIndex, inIndex - inValueIntIndex);
					outValue += Internal::_AreaOfElementFromStart(pInput, inputSize, inValueIntNextIndex, nextInIndex - inValueIntNextIndex);
				}
				else
				{
					// Result contains only fractional parts
					outValue = Internal::_AreaOfElement(pInput, inputSize, inValueIntIndex, inIndex, nextInIndex);
				}
				pOutput[outIndex] = outValue;

				inIndex = nextInIndex;
				nextInIndex = bLinear == true ? inIndex + inStep : inIndex * inStep;
			}

		}
		void SpectrumLogTransform(const float * pInput, size_t inputSize, float fromIndex, float toIndex, float * pOutput, size_t outputSize)
		{
			float inStep = powf(toIndex / fromIndex, 1.0f / outputSize);
			float inIndex = fromIndex;
			float nextInIndex = inIndex * inStep;

			for (size_t outIndex = 0; outIndex < outputSize; outIndex++)
			{
				int inValueIntIndex = (int)floor(inIndex);
				int inValueIntNextIndex = (int)floor(nextInIndex);

				float outValue = 0;

				if (inValueIntNextIndex > inValueIntIndex)
				{
					// Add up the full parts
					for (int index = inValueIntIndex + 1; index < inValueIntNextIndex && index < (int)inputSize; index++)
					{
						if (index < (int)inputSize && index >= 0)
							outValue += pInput[index];
					}
					// Add first fractional value
					if (inValueIntIndex < (int)inputSize && inValueIntIndex >= 0)
						outValue += pInput[inValueIntIndex] * (1 - inIndex + +(float)inValueIntIndex);
					// Add last fractional value
					if (inValueIntNextIndex < (int)inputSize && inValueIntNextIndex >= 0)
						outValue += pInput[inValueIntNextIndex] * (nextInIndex - (float)inValueIntNextIndex);
				}
				else
				{
					// There is only a fractional part
					if (inValueIntIndex < (int)inputSize && inValueIntIndex >= 0)
					{
						float nextValue = inValueIntIndex + 1 < (int) inputSize ? pInput[inValueIntIndex + 1] : 0;
						float dy = nextValue - pInput[inValueIntIndex];
						float x1 = inIndex - inValueIntIndex;
						float x2 = nextInIndex - inValueIntIndex;
						outValue = (x1 * dy + pInput[inValueIntIndex]) * (x2 - x1) + dy * (x2-x1) / 2;
					}
					float current = inValueIntIndex < (int)inputSize && inValueIntIndex >= 0 ? pInput[inValueIntIndex] : 0;
					outValue = current * inStep;
				}
				pOutput[outIndex] = outValue;

				inIndex = nextInIndex;
				nextInIndex = inIndex * inStep;
			}
		}
		void CombineChannels(DirectX::XMVECTOR **pSource, size_t sourceCount, size_t sourcevLength, float * pMap, DirectX::XMVECTOR * pDest)
		{
			using namespace DirectX;
			for (size_t index = 0; index < sourcevLength; index++)
			{
				XMVECTOR sum = XMVectorZero();
				for (size_t sourceIndex = 0; sourceIndex < sourceCount; sourceIndex++)
				{
					sum += XMVectorScale(pSource[sourceIndex][index], pMap[sourceIndex]);
				}
				pDest[index] = sum;
			}
		}
	}
}