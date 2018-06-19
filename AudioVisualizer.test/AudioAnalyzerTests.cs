using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Media;

namespace AudioVisualizer.test
{
    internal class HResult
    {
        public static Int32 NotValidState = -2147019873;
    }

    [ComImport]
    [Guid("5B0D3235-4DBA-4D44-865E-8F1D0E4FD04D")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal unsafe interface IMemoryBufferByteAccess
    {
        void GetBuffer(out byte* buffer, out uint capacity);
    }

    public static class AudioBufferExtension
    {
        public delegate float GeneratorFunction(Int64 frameOffset, uint channelIndex);
        public static unsafe void Generate(this AudioFrame frame,uint channels,Int64 frameOffset,GeneratorFunction generator)
        {
            using (var buffer = frame.LockBuffer(AudioBufferAccessMode.ReadWrite))
            {
                using (var bufferReference = buffer.CreateReference())
                {
                    byte* pByteData;
                    uint capacity;
                    ((IMemoryBufferByteAccess)bufferReference).GetBuffer(out pByteData, out capacity);
                    uint bufferLength = capacity / sizeof(float) / channels;
                    float* pSamples = (float*)pByteData;
                    int sampleIndex = 0;
                    for (Int64 frameIndex = frameOffset; frameIndex < frameOffset + bufferLength; frameIndex++)
                    {
                        for (uint channelIndex = 0; channelIndex < channels; channelIndex++,sampleIndex++)
                        {
                            pSamples[sampleIndex] = generator(frameIndex, channelIndex);
                        }

                    }
                }
            }
        }
    }

    [TestClass]
    public class AudioAnalyzerTests
    {
        AudioFrame inputFrame;
        List<VisualizationDataFrame> outputFrames = new List<VisualizationDataFrame>();
        uint inputFrameSize = 800;

        [TestInitialize]
        public void TestInit()
        {
            inputFrame = new AudioFrame(4 * inputFrameSize * 2); // 800 floats, 2 channels
            // Generate sine wave of amp 1 and period of 200 (f=0.25) samples into channel 0 and triangle with period 800 and amp 0.1 inopt channel 1
            inputFrame.Generate(2, 0,
                (Int64 frameIndex, uint channelIndex) => 
                {
                    return channelIndex == 0 ? (float)Math.Sin(2.0 * Math.PI * frameIndex / 200.0) : (float)(frameIndex % inputFrameSize) / 7990.0f;
                }
                );
            inputFrame.RelativeTime = TimeSpan.FromSeconds(1);
        }
        void RegisterOutputHandler(AudioAnalyzer analyzer)
        {
            analyzer.Output += new Windows.Foundation.TypedEventHandler<AudioAnalyzer, VisualizationDataFrame>(
            (a, frame) =>
            {
                Assert.AreEqual(a, analyzer);
                outputFrames.Add(frame);
            }
            );

        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Sync_Performance()
        {
            var sut = new AudioAnalyzer(48000, 2, 48000, 800, 400, 2048, false);
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();

            List<TimeSpan> outTimes = new List<TimeSpan>();
            sut.Output += new Windows.Foundation.TypedEventHandler<AudioAnalyzer, VisualizationDataFrame>(
            (a, data) =>
            {
                outTimes.Add(sw.Elapsed);
            }
            );
            AudioFrame frame = new AudioFrame(24000 * 4 * 2);   // 0.5 sec worth of audio data
            sw.Start();
            sut.ProcessInput(frame);

            List<TimeSpan> durations = new List<TimeSpan>();
            for (int i = 0; i < outTimes.Count(); i++)
            {
                durations.Add(outTimes[i].Subtract(i != 0 ? outTimes[i - 1] : TimeSpan.Zero));
            }

            double avg = durations.Average((time) => { return time.TotalMilliseconds; });
            Logger.LogMessage($"Analyzer performance {avg}ms per run");
            Assert.IsTrue(avg < 5);

        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_IsCloseable()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            using (sut)
            {
            }
            sut.Dispose();  // Second close should also succeed
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_IsCloseableAsync()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, true);
            using (sut)
            {
                Task.Delay(50).Wait();
            }
            sut.Dispose();  // Second close should also succeed
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_AllAnalyzerTypesByDefault()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            Assert.AreEqual(AnalyzerType.All, sut.AnalyzerTypes);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_SetOutput()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            sut.Output += new Windows.Foundation.TypedEventHandler<AudioAnalyzer, VisualizationDataFrame>(
                (analyzer,frame)=> { }
                );
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_BufferSizeLTOL_plus_StepThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var sut = new AudioAnalyzer(1200, 2, 48000, 800, 401, 2048, false);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_BufferSizeEQOL_plus_StepOk()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
        }


        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_ZeroChannelsThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => 
            {
                var sut = new AudioAnalyzer(1200, 0, 48000, 800, 400, 2048, false);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_ZeroSampleRateThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var sut = new AudioAnalyzer(1200, 2, 0, 800, 0, 2048, false);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_ZeroStepThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var sut = new AudioAnalyzer(1200, 2, 48000, 0, 0, 2048, false);
            });
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_NotPo2FFT_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2047, false);
            });
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_SpectrumStepSet()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            Assert.AreEqual(48000.0f / 2048.0f, sut.SpectrumStep);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_SpectrumElementCountSet()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            Assert.AreEqual(1024u, sut.SpectrumElementCount);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_SpectrumStepSetWithDownsample()
        {
            // As 2*(1600+800) > 3200 input will be downsampled by 2
            var sut = new AudioAnalyzer(2400, 2, 96000, 1600, 800, 2048, false);
            Assert.AreEqual(96000.0f / 2048.0f / 2.0f, sut.SpectrumStep);
        }


        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_GeneratesOneOutputFrame()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(1,outputFrames.Count);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_SetIsSuspendedStopsSyncProcessing()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.IsSuspended = true;
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(0, outputFrames.Count);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_SetIsSuspendedStopsAsyncProcessing()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, true);
            RegisterOutputHandler(sut);
            sut.IsSuspended = true;
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(0, outputFrames.Count);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_ResetIsSuspendedContinuesSyncProcessing()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.IsSuspended = true;
            sut.ProcessInput(inputFrame);
            sut.IsSuspended = false;
            Assert.AreEqual(1, outputFrames.Count);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public async Task AudioAnalyzer_ProcessInput_ResetIsSuspendedContinuesAsyncProcessing()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, true);
            RegisterOutputHandler(sut);
            sut.IsSuspended = true;
            sut.ProcessInput(inputFrame);
            sut.IsSuspended = false;
            await Task.Delay(50);
            Assert.AreEqual(1, outputFrames.Count);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public async Task AudioAnalyzer_ProcessInputAsync_GeneratesOneOutputFrame()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, true);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            await Task.Delay(50);
            Assert.AreEqual(1, outputFrames.Count);
        }


        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsFirstOutputFrameTimeCorrect()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(TimeSpan.FromSeconds(1),outputFrames.First().Time);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsSecondOutputFrameTimeCorrect()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(TimeSpan.FromTicks(10166666),outputFrames[1].Time);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsTimeCorrectAfterFlush()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            inputFrame.RelativeTime = TimeSpan.FromSeconds(2);
            sut.Flush();
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(outputFrames.First().Time, TimeSpan.FromSeconds(2));
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsTimeCorrectAfterFlushWithFrameTimeNotSet()
        {
            var sut = new AudioAnalyzer(1200, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            inputFrame.RelativeTime = null;
            sut.Flush();
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(outputFrames.First().Time, TimeSpan.Zero);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsTimeCorrectAfterFlushWithSeed()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.Flush(480000);
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(outputFrames.First().Time, TimeSpan.FromSeconds(10));
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsOutputFrameDurationCorrect()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(outputFrames.First().Duration, TimeSpan.FromTicks(166666));
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsRmsNotNull()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            Assert.IsNotNull(outputFrames.First().RMS);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsRmsCorrectForSine()
        {
            var sut = new AudioAnalyzer(2400, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.RMS;
            sut.ProcessInput(inputFrame);
            Assert.AreEqual((float)1.0f/Math.Sqrt(2), outputFrames.First().RMS[0],1e-7);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsRmsCorrectForSawTooth()
        {
            var sut = new AudioAnalyzer(2400, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.RMS;
            sut.ProcessInput(inputFrame);
            Assert.AreEqual((float)0.1f/Math.Sqrt(3), outputFrames.First().RMS[1],2e-5);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_AnalyzerTypeCleared_IsRmsNull()
        {
            var sut = new AudioAnalyzer(2400, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.None;
            sut.ProcessInput(inputFrame);
            Assert.IsNull(outputFrames.First().RMS);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsPeakNotNull()
        {
            var sut = new AudioAnalyzer(2400, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            Assert.IsNotNull(outputFrames.First().Peak);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_AnalyzerTypeCleared_IsPeakNull()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.None;
            sut.ProcessInput(inputFrame);
            Assert.IsNull(outputFrames.First().Peak);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsPeakCorrectForSine()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.Peak;
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(1.0f, outputFrames.First().Peak[0]);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsPeakCorrectForSawTooth()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.Peak;
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(0.1f, outputFrames.First().Peak[1]);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsSpectrumNotNull()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            Assert.IsNotNull(outputFrames.First().Spectrum);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_AnalyzerTypeCleared_IsSpectrumNull()
        {
            var sut = new AudioAnalyzer(1600, 2, 48000, 800, 400, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.None;
            sut.ProcessInput(inputFrame);
            Assert.IsNull(outputFrames.First().Spectrum);
        }

        void SetupSpectrum(int T)
        {
            var sut = new AudioAnalyzer(4096, 2, 48000, 1024, 0, 2048, false);
            RegisterOutputHandler(sut);
            sut.AnalyzerTypes = AnalyzerType.Spectrum;
            var sineFrame = new AudioFrame(2048 * sizeof(float));
            // Generate signal with 0 channel sine wave of period T
            sineFrame.Generate(2, 0,
                (frameIndex, channelIndex) =>
                {
                    return channelIndex == 0 ? (float)Math.Sin(Math.PI * frameIndex / (double)T) : 0.0f;
                });
            sut.ProcessInput(sineFrame);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_HasSpectrumRightAmountOfElements()
        {
            SetupSpectrum(32);
            Assert.AreEqual(1024, outputFrames.First().Spectrum[0].Count);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsSpectrumPeakAtRightPlace()
        {
            int T = 32;
            SetupSpectrum(T);

            int maxIndex = -1;
            float maxValue = 0.0f;
            var values = outputFrames.First().Spectrum[0];
            for (int index = 0; index < values.Count; index++)
            {
                if (values[index] > maxValue)
                {
                    maxValue = values[index];
                    maxIndex = index;
                }
            }
            Assert.AreEqual(T, maxIndex);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsSineSpectrumTotalEnergyOne()
        {
            int T = 32;
            SetupSpectrum(T);
            var values = outputFrames.First().Spectrum[0];
            var sum = values.Sum();
            Assert.AreEqual(1.0f, sum, 2e-4);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsSilenceSpectrumTotalEnergyZero()
        {
            int T = 32;
            SetupSpectrum(T);
            var values = outputFrames.First().Spectrum[1];
            var sum = values.Sum();
            Assert.AreEqual(0.0f, sum, 1e-4);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput_IsMostSpectrumEnergyAroundPeak()
        {
            int T = 32;
            int sideWidth = 4; // Include this many elements on the side
            SetupSpectrum(T);

            var values = outputFrames.First().Spectrum[0];
            // Calculate the sum of energy in peak
            float peakSum = values.Skip(T - (sideWidth) - 1).Take(sideWidth * 2 + 1).Sum();
            Assert.IsTrue(peakSum > 0.9f);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Buffer_WrapOverEnd()
        {
            var sut = new AudioAnalyzer(2400, 2, 48000, 700, 0, 2048, false);
            RegisterOutputHandler(sut);
            sut.ProcessInput(inputFrame);
            sut.ProcessInput(inputFrame);
            Assert.AreEqual(2, outputFrames.Count);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Buffer_OverlapAreaIsClear()
        {
            var sut = new AudioAnalyzer(3200, 2, 48000, 800, 800, 2048, false);
            RegisterOutputHandler(sut);
            AudioFrame silence = new AudioFrame(4 * 2 * 800);
            silence.Generate(2, 0, (frameIndex, channelIndex) => { return 0.0f; } );
            sut.ProcessInput(silence);

            Assert.AreEqual(0.0f,outputFrames.First().Spectrum[0].Sum());
            Assert.AreEqual(0.0f, outputFrames.First().Spectrum[1].Sum());
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Buffer_SequenceReadIsCorrect()
        {
            // Generate audio signal with steps after 600 frames, packaged in frame of length 800 frames.
            // Output frames should contain exactly steps
            var sut = new AudioAnalyzer(2400, 2, 48000, 600, 300, 2048, false);
            RegisterOutputHandler(sut);
            AudioFrame[] frames = new AudioFrame[3]
            {
                new AudioFrame(4 * 2 * 800),new AudioFrame(4 * 2 * 800),new AudioFrame(4 * 2 * 800)
            };
            frames[0].Generate(2, 0, (frameIndex, channelIndex) => { return frameIndex >= 600 ? 1.0f : 0.0f; });
            frames[1].Generate(2, 800, (frameIndex, channelIndex) => { return frameIndex >= 1200 ? 2.0f : 1.0f; });
            frames[2].Generate(2, 1600, (frameIndex, channelIndex) => { return frameIndex >= 1800 ? 3.0f : 2.0f; });

            foreach (var frame in frames)
            {
                sut.ProcessInput(frame);
            }

            for (int outputFrameIndex = 0; outputFrameIndex < 4; outputFrameIndex++)
            {
                float expectedValue = (float)outputFrameIndex;
                Assert.AreEqual(expectedValue, outputFrames[outputFrameIndex].Peak[0],"Channel 0");
                Assert.AreEqual(expectedValue, outputFrames[outputFrameIndex].Peak[1],"Channel 1");
            }


        }
    }

}
