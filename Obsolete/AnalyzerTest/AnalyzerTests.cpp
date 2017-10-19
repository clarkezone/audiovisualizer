#include "pch.h"
#include "CppUnitTest.h"
#include <AudioAnalyzer.h>	// Added include path to project setting to reference header file
#include <math.h>
#include <limits>
#include <DirectXMath.h>
#include <AudioMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace AudioMath;

namespace AnalyzerTest
{
	TEST_CLASS(AnalyzerTests)
	{
	public:
		TEST_METHOD(Analyzer)
		{
			using namespace DirectX;
			AudioMath::CAudioAnalyzer analyzer(1024);
			analyzer.ConfigureAnalyzer(1024, 256, 0);
			float signal[512];
			float pi = 3.14159f;
			for (size_t i = 0; i < 512; i+=2)
			{
				size_t t = i >> 1;
				signal[i] = sinf(4 * pi*t / 256);
				signal[i+1] = (0.1f / 7.0f) * (t & 0x7);	// Sawtooth wave
			}
			analyzer.AddInput(signal, 256, 0);
			Assert::IsFalse(analyzer.IsOutputAvailable(), L"IsOutputAVailable", LINE_INFO());
			analyzer.AddInput(signal + 256, 256);
			Assert::IsTrue(analyzer.IsOutputAvailable(),L"IsOutputAVailable",LINE_INFO());

			long position = -1;
			XMVECTOR rms;
			XMVECTOR peak;
			XMVECTOR spectrum[256];
			analyzer.Step(&position, &rms, &peak, spectrum);

			Assert::AreEqual(0L, position, L"position");
			Assert::AreEqual(0.707107067f, rms.m128_f32[0], L"RMS[0]", LINE_INFO());
			Assert::AreEqual(0.0597614311f, rms.m128_f32[1], L"RMS[1]", LINE_INFO());
			Assert::AreEqual(1.0f, peak.m128_f32[0], L"Peak[0]", LINE_INFO());
			Assert::AreEqual(0.1f, peak.m128_f32[1], L"Peak[1]", LINE_INFO());

			for (size_t channel = 0; channel < 2; channel++)
			{
				int peakIndex = -1;
				float maxSpectrum = 0.0f, sum = 0.0f;
				float *s = (float *)(spectrum + 128 * channel);
				for (size_t i = 0; i < 512; i++)	// Spectrum length is half fft length
				{
					Assert::IsTrue(s[i] >= 0 && s[i] < 1, L"FFT output");
					if (s[i] > maxSpectrum)
					{
						maxSpectrum = s[i];
						peakIndex = i;
					}
					sum += s[i];
				}
				if (channel == 0)
				{
					Assert::AreEqual(0.94f, sum,0.01f);
					Assert::AreEqual(8, peakIndex);
				}
				else
				{
					Assert::AreEqual(0.13f, sum, 0.01f);
				}
			}

			for (size_t i = 0; i < 512; i+=2)
			{
				size_t t = i >> 1;
				signal[i] = sinf(4 * pi*t / 256);
				signal[i+1] = 0.5f * sinf(8 * pi*t / 256);
			}
			analyzer.AddInput(signal, 512);

			analyzer.Step(&position, nullptr,nullptr,nullptr);

			Assert::AreEqual(256L, position, L"position");
		}
	};
}