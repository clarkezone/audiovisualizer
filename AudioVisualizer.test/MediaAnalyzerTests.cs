using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AudioVisualizer.test
{
    [TestClass]
    public class MediaAnalyzerTests
    {
        IMFTransform sut;
        [TestInitialize]
        public void Test_Init()
        {
            var analyzer = new AudioVisualizer.MediaAnalyzer();
            Assert.IsNotNull(analyzer);
            sut = analyzer as object as IMFTransform;
            Assert.IsNotNull(sut, "Cannot get IMFTransform interface");
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetStreamLimits()
        {
            UInt32 imin = 0, imax = 0, omin = 0, omax = 0;
            sut.GetStreamLimits(out imin, out imax, out omin, out omax);
            Assert.AreEqual(1u, imin);
            Assert.AreEqual(1u, imax);
            Assert.AreEqual(1u, omin);
            Assert.AreEqual(1u, omax);
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetStreamCount()
        {
            UInt32 cInput = 0, cOutput = 0;
            sut.GetStreamCount(out cInput, out cOutput);
            Assert.AreEqual(1u, cInput);
            Assert.AreEqual(1u, cOutput);
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetStreamIDs()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    UInt32 cInput = 0, cOutput = 0;
                    UInt32 outStreams = 0, inStreams = 0;
                    sut.GetStreamIDs(cInput, out inStreams, cOutput, out outStreams);
                });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetInputStreamInfo()
        {
            MFT_INPUT_STREAM_INFO streamInfo;
            sut.GetInputStreamInfo(0, out streamInfo);
            Assert.AreEqual(0u, streamInfo.cbAlignment);
            Assert.AreEqual(0u, streamInfo.cbMaxLookahead);
            Assert.AreEqual(0u, streamInfo.cbSize);
            Assert.AreEqual(1u, streamInfo.dwFlags);
            Assert.AreEqual(0u, streamInfo.hnsMaxLatency);
            Assert.ThrowsException<COMException>( ()=> 
            {
                sut.GetInputStreamInfo(1, out streamInfo);
            });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetOutputStreamInfo()
        {
            MFT_OUTPUT_STREAM_INFO streamInfo;
            sut.GetOutputStreamInfo(0, out streamInfo);
            Assert.AreEqual(0u, streamInfo.cbAlignment);
            Assert.AreEqual(0u, streamInfo.cbSize);
            Assert.AreEqual(517u, streamInfo.dwFlags);
            Assert.ThrowsException<COMException>(() =>
            {
                sut.GetOutputStreamInfo(1, out streamInfo);
            });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetInputStreamAttributes()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    IMFAttributes a = null;
                    sut.GetInputStreamAttributes(0,out a);
                });
        }
        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetOutputStreamAttributes()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    IMFAttributes a = null;
                    sut.GetOutputStreamAttributes(0, out a);
                });
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_GetAttributes()
        {
            IMFAttributes a = null;
            sut.GetAttributes(out a);
            Assert.IsNotNull(a);
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_DeleteInputStream()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    sut.DeleteInputStream(0);
                });
        }

        [TestMethod]
        [TestCategory("IMFTransform")]
        public void MediaAnalyzer_Mft_AddInputStreams()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    uint strmId = 0;
                    sut.AddInputStreams(1, out strmId);
                });
        }
    }
}