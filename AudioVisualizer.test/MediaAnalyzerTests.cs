using AudioVisualizer.test.MediaFoundation.Interop;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.MediaProperties;

namespace AudioVisualizer.test
{
    [TestClass]
    public class MediaAnalyzerTests
    {
        private MediaFoundation.MftWrapper _mft;

        private static AudioEncodingProperties CreateFloatEncodingProperty(uint sampleRate, uint channels)
        {
            return new AudioEncodingProperties()
            {
                SampleRate = sampleRate,
                ChannelCount = channels,
                BitsPerSample = 32,
                Bitrate = 32 * channels * sampleRate,
                Subtype = "Float"
            };
        }

        [TestInitialize]
        public void Test_Init()
        {
            var analyzer = new AudioVisualizer.MediaAnalyzer();
            Assert.IsNotNull(analyzer);
            _mft = new MediaFoundation.MftWrapper(analyzer);
        }

        [TestMethod]
        [TestCategory("IMFTransform")]

        public void MediaAnalyzer_Mft_GetStreamLimits()
        {
            var limits = _mft.GetStreamLimits();
            Assert.AreEqual(1u, limits.minInput);
            Assert.AreEqual(1u, limits.maxInput);
            Assert.AreEqual(1u, limits.minOutput);
            Assert.AreEqual(1u, limits.maxOutput);
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetStreamCount()
        {
            var counts = _mft.GetStreamCount();
            Assert.AreEqual(1u, counts.inputStreamCount);
            Assert.AreEqual(1u, counts.outStreamCount);
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetStreamIDs()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.GetStreamIds();
                });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetInputStreamInfo_Stream_0()
        {
            var streamInfo = _mft.GetInputStreamInfo(0);
            Assert.AreEqual(0u, streamInfo.cbAlignment);
            Assert.AreEqual(0u, streamInfo.cbMaxLookahead);
            Assert.AreEqual(0u, streamInfo.cbSize);
            Assert.AreEqual(1u, streamInfo.dwFlags);
            Assert.AreEqual(0u, streamInfo.hnsMaxLatency);
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetInputStreamInfo_Stream_1_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.GetInputStreamInfo(1);
            });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetOutputStreamInfo_Stream_0()
        {
            var streamInfo = _mft.GetOutputStreamInfo(0);
            Assert.AreEqual(0u, streamInfo.cbAlignment);
            Assert.AreEqual(0u, streamInfo.cbSize);
            Assert.AreEqual(517u, streamInfo.dwFlags);
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetOutputStreamInfo_Stream_1_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.GetOutputStreamInfo(1);
            });
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetInputStreamAttributes()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.GetInputStreamAttributes();
                });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetOutputStreamAttributes()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.GetOutputStreamAttributes();
                });
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetAttributes()
        {
            var attributes = _mft.GetAttributes();
            Assert.IsNotNull(attributes);
            Assert.IsFalse(attributes.Any());
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_DeleteInputStream()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.DeleteInputStream(0);
                });
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_AddInputStreams()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.AddInputStreams(0);
                });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetAvailableInputTypes_Type_0()
        {
            var type0 = _mft.GetAvailableInputTypes(0, 0);
            Assert.AreEqual(2, type0.Properties.Count());
            Assert.AreEqual("Audio", type0.Type);
            Assert.AreEqual("Float", type0.Subtype);
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.GetAvailableInputTypes(0, 1);
            }
            );
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetAvailableInputTypes_Type_1_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                try
                {
                    _mft.GetAvailableInputTypes(0, 1);
                }
                catch (Exception e)
                {
                    Assert.AreEqual(-1072875847, e.HResult);
                    throw;
                }
            }
            );
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_SetInputType_MP3_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.SetInputMediaType(0, AudioEncodingProperties.CreateMp3(44100, 2, 128000), true);
            });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_SetInputType_PCM_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.SetInputMediaType(0, AudioEncodingProperties.CreatePcm(44100, 2, 16), true);
            });
        }
        [TestCategory("IMFTransform")]
        [DataTestMethod()]
        [DataRow(22050, 1)]
        [DataRow(32000,1)]
        [DataRow(44100, 1)]
        [DataRow(48000, 1)]
        [DataRow(96000, 1)]
        [DataRow(192000, 1)]
        [DataRow(22050, 2)]
        [DataRow(32000, 2)]
        [DataRow(44100, 2)]
        [DataRow(48000, 2)]
        [DataRow(96000, 2)]
        [DataRow(192000, 2)]
        [DataRow(48000, 5)]
        public void MediaAnalyzer_Mft_SetInputType(int sampleRate,int channels)
        {
            _mft.SetInputMediaType(0, CreateFloatEncodingProperty((uint) sampleRate, (uint) channels), true);
        }
        [TestCategory("IMFTransform")]
        [DataTestMethod()]
        [DataRow(22050, 1)]
        [DataRow(32000, 1)]
        [DataRow(44100, 1)]
        [DataRow(48000, 1)]
        [DataRow(96000, 1)]
        [DataRow(192000, 1)]
        [DataRow(22050, 2)]
        [DataRow(32000, 2)]
        [DataRow(44100, 2)]
        [DataRow(48000, 2)]
        [DataRow(96000, 2)]
        [DataRow(192000, 2)]
        [DataRow(48000, 5)]
        public void MediaAnalyzer_Mft_SetOutputType(int sampleRate, int channels)
        {
            _mft.SetInputMediaType(0, CreateFloatEncodingProperty((uint)sampleRate, (uint)channels), true);
        }

        [TestCategory("IMFTransform")]
        [DataTestMethod()]
        [DataRow(22050, 1)]
        [DataRow(32000, 1)]
        [DataRow(44100, 1)]
        [DataRow(48000, 1)]
        [DataRow(96000, 1)]
        [DataRow(192000, 1)]
        [DataRow(22050, 2)]
        [DataRow(32000, 2)]
        [DataRow(44100, 2)]
        [DataRow(48000, 2)]
        [DataRow(96000, 2)]
        [DataRow(192000, 2)]
        [DataRow(48000, 5)]
        public void MediaAnalyzer_Mft_Process(int sampleRate,int channels)
        {
            MediaFoundation.MediaPipelineFake pipeline = new MediaFoundation.MediaPipelineFake(_mft,(uint)sampleRate,(uint)channels);
            
        }
    }
}