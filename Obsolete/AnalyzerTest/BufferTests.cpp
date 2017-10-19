#include "pch.h"
#include "CppUnitTest.h"
#include <AudioBuffer.h>	// Added include path to project setting to reference header file
#include <math.h>
#include <limits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AudioMath;

namespace AnalyzerTest
{
	TEST_CLASS(Buffer)
	{
	private:
		float _fNaN = std::numeric_limits<float>::quiet_NaN();

		void ValidateOutputBuffer(const float *pExpected, const float *pActual)
		{
			for (size_t index = 0; index < 4; index++)
			{
				Assert::AreEqual(pExpected[index], pActual[index]);
			}
			Assert::AreEqual(_fNaN,pActual[4]);	// Detect buffer overruns
			Assert::AreEqual(_fNaN, pActual[5]);	// Detect buffer overruns
		}
	public:
		TEST_METHOD(AudioBuffer_ctor)
		{
			CAudioBuffer buffer(8);
			// Test for initial conditions
			Assert::AreEqual((size_t) 0, buffer.GetLength());
			Assert::AreEqual((size_t) 8, buffer.GetSize());
			Assert::AreEqual((size_t) 0, buffer.GetStepLength());
			Assert::AreEqual((size_t) 0, buffer.GetStepOverlap());
			Assert::AreEqual((size_t) 2, buffer.GetFrameSize());
			Assert::AreEqual(-1L, buffer.GetPosition());
		}

		TEST_METHOD(AudioBuffer_AddStep)
		{
			CAudioBuffer buffer(8); // Initialize to 8 samples
			buffer.SetFrameSize(2);
			Assert::AreEqual((size_t) 8, buffer.GetSize());
			HRESULT hr = buffer.Configure(2, 1);	// We want to get 4 samples (2 frames stereo), 2 fresh +  2 overlapping
			Assert::IsTrue(SUCCEEDED(hr));

			// Simulate stereo data, L channel = frame_no, R channel = frame_no / 10
			float testData[] = { 1.0f,0.1f, 2.0f,0.2f, 3.0f, 0.3f, 4.0f, 0.4f, 
								 5.0f, 0.5f, 6.0f, 0.6f, 7.0f, 0.7f, 8.0f, 0.8f };

			Assert::AreEqual((size_t)0, buffer.GetLength());	// Before any samples added this should be 0
			buffer.SetPosition(0);
			Assert::AreEqual(0L, buffer.GetPosition());

			float output1[6];
			for (size_t index = 0; index < sizeof(output1) / sizeof(float); index++)
			{
				output1[index] = _fNaN;
			}

			Assert::IsFalse(buffer.IsDataAvailable(),L"IsDataAvailable",LINE_INFO());
			buffer.Add(testData, 2);
			Assert::AreEqual((size_t)1, buffer.GetLength(),L"GetLength",LINE_INFO());
			Assert::AreEqual(0L, buffer.GetPosition(),L"GetPosition",LINE_INFO());	// Position should change on
			Assert::IsTrue(buffer.IsDataAvailable(), L"IsDataAvailable", LINE_INFO());

			buffer.Step(output1);
			Assert::AreEqual(1L, buffer.GetPosition(), L"GetPosition", LINE_INFO());	// Position should now read 2

			float _ref1[] = { 0,1.0,0.0f,0.1f };
			for (size_t index = 0; index < 4; index++)
			{
				Assert::AreEqual(_ref1[index], output1[index]);
			}
			Assert::AreEqual(_fNaN, output1[4]);	// Detect buffer overruns
			Assert::AreEqual(_fNaN, output1[5]);	// Detect buffer overruns

			hr = buffer.Add(testData + 2, 4);
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(1L, buffer.GetPosition(), L"GetPosition", LINE_INFO());	

			hr = buffer.Add(testData + 6, 6);	// Overflow the buffer
			Assert::IsTrue(SUCCEEDED(hr));
			Assert::AreEqual(3L, buffer.GetPosition(), L"GetPosition", LINE_INFO());	// Position should change on overflow
			Assert::AreEqual((size_t)3, buffer.GetLength(), L"GetLength", LINE_INFO());
			Assert::IsTrue(buffer.IsDataAvailable(), L"IsDataAvailable", LINE_INFO());

			float window[] = { 10.0f, 100.0f };
			float output2[8];
			memset(output2, -1, 32);
			for (size_t index = 0; index < sizeof(output2) / sizeof(float); index++)
			{
				output2[index] = nanf(nullptr);
			}
			hr = buffer.Step(output2, 3, window);
			Assert::IsTrue(SUCCEEDED(hr));
			float _ref2[] = { 30.0,400.0,0.0f,3.0f,40.0f,0.0f };
			for (size_t index = 0; index < 6; index++)
			{
				Assert::AreEqual(_ref2[index], output2[index]);
			}
			Assert::AreEqual(_fNaN, output2[6]);	// Detect buffer overruns
			Assert::AreEqual(_fNaN, output2[7]);	// Detect buffer overruns
		}
	};
}