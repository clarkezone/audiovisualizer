#include "pch.h"
#include "CppUnitTest.h"
#include <math.h>
#include <limits>
#include <vector>
#include <algorithm>
#include <DirectXMath.h>
#include <AudioMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AudioVisualizer::Math;
using namespace std;

namespace AnalyzerTest
{
	TEST_CLASS(MathTests)
	{
	public:
		TEST_METHOD(Math_RiseAndFall)
		{
			using namespace DirectX;
			XMVECTOR vPrevious = XMVectorSet(1, 1, 2, 1);
			XMVECTOR vCurrent = XMVectorSet(1, 2, 1, 11);
			XMVECTOR vResult = XMVectorReplicate(std::numeric_limits<float>::quiet_NaN());
			ApplyRiseAndFall(&vPrevious, &vCurrent, &vResult, 1, 1, 3);

			Assert::AreEqual(1, XMVectorGetByIndex(vResult,0), 0.001f, L"1->1");
			Assert::AreEqual(1.63212055f, XMVectorGetByIndex(vResult, 1), 0.001f, L"1->2");
			Assert::AreEqual(1.04978707f, XMVectorGetByIndex(vResult, 2), 0.001f, L"2->1");
			Assert::AreEqual(7.3212055f, XMVectorGetByIndex(vResult, 3), 0.001f, L"1->11");


			XMVECTOR vResult2 = XMVectorReplicate(std::numeric_limits<float>::quiet_NaN());
			ApplyRiseAndFall(nullptr, &vResult, &vResult2, 1, 1, 3);

			Assert::AreEqual(0.63212049f, XMVectorGetByIndex(vResult2, 0), 0.001f);
			Assert::AreEqual(1.0316968f, XMVectorGetByIndex(vResult2, 1), 0.001f);
			Assert::AreEqual(0.663591921f, XMVectorGetByIndex(vResult2, 2), 0.001f);
			Assert::AreEqual(4.62788391f, XMVectorGetByIndex(vResult2, 3), 0.001f);
		}
		TEST_METHOD(Math_ConvertToLogAmp)
		{
			using namespace DirectX;
			XMVECTOR vInput[2] = { XMVectorSet(-1.0f,0.0f,1e-6f,0.1f),XMVectorSet(1.0f,10.0f,std::numeric_limits<float>::max(),INFINITY) };
			XMVECTOR vResult[2];
			ConvertToLogarithmic(vInput, vResult, 2, -100.0f, 20.0f);
			float expected[] = { -100,-100,-100,-20,0,20,20,20 };
			for (size_t i = 0; i < 8; i++)
			{
				Assert::AreEqual(expected[i], ((float*)vResult)[i]);
			}
		}

		void Test_SpectrumTransform(vector<float> &input,vector<float> &expected,float from,float to,bool linear)
		{
			vector<float> output(expected.size() + 1);
			fill(output.begin(), output.end(), numeric_limits<float>::quiet_NaN());
			SpectrumTransform(&input[0], input.size(), from, to, &output[0], expected.size(), linear);
			for (size_t i = 0; i < expected.size(); i++)
			{
				Assert::AreEqual(expected[i], output[i]);
			}
			Assert::IsTrue(isnan(output[expected.size()]));	// Test for buffer overwrites
		}

		TEST_METHOD(Math_SpectrumTransform)
		{
			vector<float> testData = { 0,1,2,3,4,5,6,7,8,9 };

			// Full range transform into smaller
			Test_SpectrumTransform(
				testData,
				vector<float>() = { 2, 8 , 14.5 , 20.5 },
				0, 10.0f, true
			);

			// Partial range transform, from low end
			Test_SpectrumTransform(
				testData,
				vector<float>() = { 0.0f,0.0f,0.0f, 0.0f , 0.5f , 0.5f },
				-1.0f, 2.0f, true
			);

			Test_SpectrumTransform(
				testData,
				vector<float>() = { 4.5f,4.5f,0.0f, 0.0f },
				9.0f, 11.0f, true
			);
		}

		TEST_METHOD(Math_CombineChannels)
		{
			using namespace DirectX;
			XMVECTOR src[2] = { 
				XMVectorSet(1,2,3,4),
				XMVectorSet(5,6,7,8)
			};
			XMVECTOR *src_ptrs[2] = { src,src+1 };

			XMVECTOR dst;
			float map[2] = { 0.1f,0.2f };
			AudioVisualizer::Math::CombineChannels(src_ptrs, 2, 1, map, &dst);

			Assert::AreEqual(1.1f, XMVectorGetByIndex(dst, 0),1e-5f);
			Assert::AreEqual(1.4f, XMVectorGetByIndex(dst, 1),1e-5f);
			Assert::AreEqual(1.7f, XMVectorGetByIndex(dst, 2),1e-5f);
			Assert::AreEqual(2.0f, XMVectorGetByIndex(dst, 3),1e-5f);
		}

		TEST_METHOD(Math_SeriesAreas)
		{
			using namespace AudioVisualizer::Math::Internal;
			float testData[3] = { 1.0,2.0,3.0 };
			Assert::AreEqual(0.0f,_AreaOfElementFromStart(testData, 3, 0, 0.0));
			Assert::AreEqual(0.625f,_AreaOfElementFromStart(testData, 3, 0, 0.5));
			Assert::AreEqual(1.5f,_AreaOfElementFromStart(testData, 3, 0, 1.0));
			Assert::AreEqual(0.0f, _AreaOfElementFromStart(testData, 3, 2, 0.0));
			Assert::AreEqual(1.125f, _AreaOfElementFromStart(testData, 3, 2, 0.5));
			Assert::AreEqual(1.5f, _AreaOfElementFromStart(testData, 3, 2, 1.0));
			Assert::AreEqual(0.0f, _AreaOfElementFromStart(testData, 3, 4, 0.1));
			Assert::AreEqual(0.0f, _AreaOfElementFromStart(testData, 3, -1, 0.1));
		}
	};
}