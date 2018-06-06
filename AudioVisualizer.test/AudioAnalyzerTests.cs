using Microsoft.VisualStudio.TestTools.UnitTesting;
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
    [TestClass]
    public class AudioAnalyzerTests
    {
        AudioAnalyzer sut;
        [TestInitialize]
        public void TestInit()
        {           
            sut = new AudioAnalyzer(48000, false);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_IsCloseable()
        {
            using (sut)
            {
            }
            sut.Dispose();  // Second close should also succeed
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_AllAnalyzerTypesByDefault()
        {
            Assert.AreEqual(AnalyzerType.All, sut.AnalyzerTypes);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_FftLengthDefault()
        {
            Assert.AreEqual(0u, sut.FftLength);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_InputChannelCountDefault()
        {
            Assert.AreEqual(0u, sut.InputChannelCount);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_InputSampleRateDefault()
        {
            Assert.AreEqual(0u, sut.InputSampleRate);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_InputStepFrameCountDefault()
        {
            Assert.AreEqual(0u, sut.InputStepFrameCount);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_InputStepOverlapDefault()
        {
            Assert.AreEqual(0u, sut.InputStepOverlap);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_FrequencyStepDefault()
        {
            Assert.AreEqual(0.0f, sut.FrequencyStep);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_PositionDefault()
        {
            Assert.IsNull(sut.Position);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_SetPosition()
        {
            sut.Position = 1;
            Assert.AreEqual(1,sut.Position);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_SetOutput()
        {
            sut.Output += new Windows.Foundation.TypedEventHandler<AudioAnalyzer, VisualizationDataFrame>(
                (analyzer,frame)=> { }
                );
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_ZeroChannelsThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => 
            {
                sut.Configure(0, 48000, 800, 0, 2048);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_ZeroSampleRateThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.Configure(2, 0, 800, 0, 2048);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_ZeroStepThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.Configure(2, 48000, 0, 0, 2048);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_Overlap_GTE_StepThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.Configure(2, 48000, 800, 800, 2048);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_With_NotPo2FFT_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.Configure(2, 48000, 800, 0, 2047);
            });
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_ChannelCountSet()
        {
            sut.Configure(2, 48000, 800, 400, 2048);
            Assert.AreEqual(2u, sut.InputChannelCount);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_SampleRateSet()
        {
            sut.Configure(2, 48000, 800, 400, 2048);
            Assert.AreEqual(48000u, sut.InputSampleRate);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_StepSet()
        {
            sut.Configure(2, 48000, 800, 400, 2048);
            Assert.AreEqual(800u, sut.InputStepFrameCount);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_OverlapSet()
        {
            sut.Configure(2, 48000, 800, 400, 2048);
            Assert.AreEqual(400u, sut.InputStepOverlap);
        }
        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_FftLengthSet()
        {
            sut.Configure(2, 48000, 800, 400, 2048);
            Assert.AreEqual(2048u, sut.FftLength);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Configure_FrequencyStepSet()
        {
            sut.Configure(2, 48000, 800, 400, 2048);
            Assert.AreEqual(11.71875f, sut.FrequencyStep);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_Process_WithoutConfigThrows()
        {
            AudioFrame frame = new AudioFrame(600);
            var e = Assert.ThrowsException<COMException>(() => 
            {
                sut.ProcessInput(frame);
            });
            Assert.AreEqual(HResult.NotValidState, e.HResult);
        }

        [TestMethod]
        [TestCategory("AudioAnalyzer")]
        public void AudioAnalyzer_ProcessInput()
        {
            sut.Configure(2, 48000, 800, 400, 2048);
            AudioFrame inputFrame = new AudioFrame(4*800*2); // 800 floats, 2 channels
            inputFrame.RelativeTime = TimeSpan.FromSeconds(1);
            using (var buffer = inputFrame.LockBuffer(AudioBufferAccessMode.ReadWrite))
            {
                buffer.Length = 4 * 800 * 2;
            }
            List<VisualizationDataFrame> frames = new List<VisualizationDataFrame>();

            sut.Output += new Windows.Foundation.TypedEventHandler<AudioAnalyzer, VisualizationDataFrame>(
                (analyzer, frame) => 
                {
                    Assert.AreEqual(sut, analyzer);
                    frames.Add(frame);
                }
            );
            sut.ProcessInput(inputFrame);
            //Assert.AreEqual(1,frames.Count);
        }
    }

}
