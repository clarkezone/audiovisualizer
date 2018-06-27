using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace AudioVisualizer.test
{
    [TestClass]
    public class AnalogVUMeterTests
    {
        AnalogVUMeter sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = new AudioVisualizer.AnalogVUMeter();
        }

        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_DefaultSourceIsNull()
        {
            Assert.IsNull(sut.Source);
        }

        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetSource()
        {
            FakeVisualizationSource source = new FakeVisualizationSource();
            sut.Source = source;
            Assert.AreEqual(source, sut.Source);
        }

        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_IsDefaultChannelSetToZero()
        {
            Assert.AreEqual(0, sut.ChannelIndex);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetChannelIndex()
        {
            sut.ChannelIndex = 1;
            Assert.AreEqual(1, sut.ChannelIndex);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_SettingChannelIndexToNegativeThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => 
            {
                sut.ChannelIndex = -1;
            });
        }

    }
}
