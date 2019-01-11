using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.MediaProperties;

namespace AudioVisualizer.test
{
    [TestClass]
    public class AudioViewDataTests
    {
        AudioViewData sut;
        [TestInitialize]
        public void Init()
        {
            sut = new AudioViewData();
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_Append_ZeroStrideThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.Append(new Windows.Media.AudioFrame(480), 0, 0);
            });
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_Append_OffsetGTStrideThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.Append(new Windows.Media.AudioFrame(480), 2, 2);
            });
        }

        [TestMethod]
        [TestCategory("AudioViewData")]
        public void AudioViewData_Append()
        {
           sut.Append(new Windows.Media.AudioFrame(480));
        }
    }
}
