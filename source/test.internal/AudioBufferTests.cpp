#include "pch.h"
#include "CppUnitTest.h"
#include <AudioBuffer.h>	// Added include path to project setting to reference header file
#include <math.h>
#include <limits>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AudioVisualizer::Math;
using namespace std;

namespace AnalyzerTest
{

	TEST_CLASS(AudioBuffer)
	{

	private:
		void TestBuffer(size_t channels,size_t downsample)
		{
			const size_t cTestBufferSamples = 4;
			size_t bufferSize = cTestBufferSamples * channels;
			CAudioBuffer buffer(bufferSize);
			Assert::AreEqual((size_t)0, buffer.GetLength(), L"GetLength()", LINE_INFO());
			Assert::AreEqual(bufferSize, buffer.GetSize(), L"GetSize()", LINE_INFO());
			Assert::AreEqual((size_t)0, buffer.GetStepLength(), L"GetStepLength()", LINE_INFO());
			Assert::AreEqual((size_t)0, buffer.GetStepOverlap(), L"GetStepOverlap()", LINE_INFO());
			Assert::AreEqual((size_t)2, buffer.GetFrameSize(), L"GetFrameSize()", LINE_INFO());
			Assert::AreEqual((size_t)1, buffer.GetDownsampleFactor(), L"GetDownsampleFactor()", LINE_INFO());
			Assert::AreEqual(-1L, buffer.GetPosition(), L"GetPosition()", LINE_INFO());

			buffer.SetFrameSize(channels);
			Assert::AreEqual(channels, buffer.GetFrameSize(), L"GetFrameSize()", LINE_INFO());
			buffer.Configure(2, 1, downsample);
			Assert::AreEqual((size_t)2, buffer.GetStepLength(), L"GetStepLength()", LINE_INFO());
			Assert::AreEqual((size_t)1, buffer.GetStepOverlap(), L"GetStepOverlap()", LINE_INFO());
			Assert::AreEqual(downsample, buffer.GetDownsampleFactor(), L"GetDownsampleFactor()", LINE_INFO());
			buffer.SetPosition(0L);
			Assert::AreEqual(0L, buffer.GetPosition(),L"GetPosition()", LINE_INFO());
			// Set up input data
			vector<float> inputData(2 * bufferSize * downsample);
			float value = 1.0f;
			for (size_t index = 0; index < inputData.size(); index++,value++)
			{
				inputData[index] = value;
			}

			size_t outputBufferStep = buffer.GetStepLength() * 2;
			size_t outputBufferCapacity = outputBufferStep * (channels + 1);
			vector<float> output(outputBufferCapacity);	// 1 for overflow checking

			for (size_t testIndex = 0; testIndex < cTestBufferSamples + 1; testIndex++)	// Run tests so that buffer fills + 1 for overflow
			{
				Assert::IsFalse(buffer.IsDataAvailable(), L"IsDataAvailable", LINE_INFO());
				Assert::AreEqual((size_t)0, buffer.GetLength(), L"GetLength", LINE_INFO());
				buffer.Add(&inputData[testIndex * downsample * channels], channels * downsample);
				Assert::AreEqual((size_t)1, buffer.GetLength(), L"GetLength", LINE_INFO());
				Assert::AreEqual((long) (testIndex * downsample), buffer.GetPosition(), L"GetPosition", LINE_INFO());
				Assert::IsTrue(buffer.IsDataAvailable(), L"IsDataAvailable", LINE_INFO());
				fill(output.begin(), output.end(), numeric_limits<float>::quiet_NaN());
				buffer.Step(&*output.begin(), outputBufferStep);

				Assert::IsFalse(buffer.IsDataAvailable(), L"IsDataAvailable", LINE_INFO());
				Assert::AreEqual((size_t)0, buffer.GetLength(), L"GetLength", LINE_INFO());
				Assert::AreEqual(long(testIndex + 1) * (long)downsample, buffer.GetPosition(), L"GetPosition", LINE_INFO());

				for (size_t channelIndex = 0; channelIndex < channels + 1; channelIndex++) // channel+1 is intialized to nan
				{
					size_t indexOffset = channelIndex * outputBufferStep;

					for (size_t index = 0; index < outputBufferStep; index++)
					{
						wstringstream s_context;
						s_context << L"TestPass=" << testIndex << " Channel=" << channelIndex << " Item=" << index;
						if (channelIndex < channels)
						{
 							if (index < 2)
							{
								float expectedValue = 0;
								if (index == 1)
									expectedValue = (float)(downsample * channels * testIndex + channelIndex + 1);
								else if (testIndex != 0)
									expectedValue = (float)(downsample * channels * (testIndex - 1) + channelIndex + 1);

								Assert::AreEqual(expectedValue, output[index + indexOffset], s_context.str().c_str(), LINE_INFO());
							}
							else
							{
								Assert::AreEqual(0.0f, output[index + indexOffset], s_context.str().c_str(), LINE_INFO());
							}
						}
						else
						{
							Assert::IsTrue(isnan(output[index + indexOffset]), s_context.str().c_str(), LINE_INFO());
						}
					}
				}

			}
		}

	public:
		TEST_METHOD(AudioBuffer_1ch_1ds)
		{
			TestBuffer(1,1);
		}
		TEST_METHOD(AudioBuffer_2ch_1ds)
		{
			TestBuffer(2, 1);
		}
		TEST_METHOD(AudioBuffer_2ch_2ds)
		{
			TestBuffer(2, 2);
		}
		TEST_METHOD(AudioBuffer_2ch_4ds)
		{
			TestBuffer(2, 4);
		}
		TEST_METHOD(AudioBuffer_5ch_4ds)
		{
			TestBuffer(5, 4);
		}
	};
}