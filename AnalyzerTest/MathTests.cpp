#include "pch.h"
#include "CppUnitTest.h"
#include <AudioBuffer.h>	// Added include path to project setting to reference header file
#include <math.h>
#include <limits>
#include <DirectXMath.h>
#include <AudioMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AudioMath;

namespace AnalyzerTest
{
	TEST_CLASS(MathTests)
	{
	public:
		TEST_METHOD(Math_RiseAndFall)
		{
			using namespace DirectX;
			XMVECTOR vPrevious = XMVectorSet(1, 1, 2, 2);
			XMVECTOR vCurrent = XMVectorSet(1, 2, 1, 2);
			XMVECTOR vResult = XMVectorReplicate(std::numeric_limits<float>::max());
			AudioMath::ApplyRiseAndFall(&vPrevious, &vCurrent, &vResult, 1, 1, 3);

			Assert::AreEqual(1, vResult.m128_f32[0], 0.001f, L"1->1");
			Assert::AreEqual(1.63212055f, vResult.m128_f32[1], 0.001f, L"1->2");
			Assert::AreEqual(1.04978707f, vResult.m128_f32[2], 0.001f, L"2->1");
			Assert::AreEqual(2, vResult.m128_f32[3], 0.001f, L"2->2");
		}
	};
}