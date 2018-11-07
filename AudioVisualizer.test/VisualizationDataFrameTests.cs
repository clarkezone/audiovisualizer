using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;


namespace AudioVisualizer.test
{
    [TestClass]
    public class VisualizationDataFrameTests
    {
        AudioVisualizer.VisualizationDataFrame sut;
        TimeSpan expectedTime = TimeSpan.FromSeconds(1);
        TimeSpan expectedDuration = TimeSpan.FromMilliseconds(100);
        ScalarData expectedPeak = ScalarData.CreateEmpty(2);
        ScalarData expectedRms = ScalarData.CreateEmpty(2);
        SpectrumData expectedSpectrum = SpectrumData.CreateEmpty(2, 20, ScaleType.Linear, ScaleType.Logarithmic, 20, 20000);

        [TestInitialize]
        public void TestInit()
        {
            sut = new VisualizationDataFrame(expectedTime, expectedDuration, expectedRms, expectedPeak, expectedSpectrum);
        }

        [TestMethod]
        [TestCategory("VisualizationDataFrame")]
        public void VisualizationDataFrame_Time()
        {
            Assert.AreEqual(expectedTime, sut.Time);
        }

        [TestMethod]
        [TestCategory("VisualizationDataFrame")]
        public void VisualizationDataFrame_Duration()
        {
            Assert.AreEqual(expectedDuration, sut.Duration);
        }
        [TestMethod]
        [TestCategory("VisualizationDataFrame")]
        public void VisualizationDataFrame_Rms()
        {
            Assert.AreEqual(expectedRms, sut.RMS);
        }
        [TestMethod]
        [TestCategory("VisualizationDataFrame")]
        public void VisualizationDataFrame_Peak()
        {
            Assert.AreEqual(expectedPeak, sut.Peak);
        }
        [TestMethod]
        [TestCategory("VisualizationDataFrame")]
        public void VisualizationDataFrame_Spectrum()
        {
            Assert.AreEqual(expectedSpectrum, sut.Spectrum);
        }
        [TestMethod]
        [TestCategory("VisualizationDataFrame")]
        public void VisualizationDataFrame_PropertiesNotNull()
        {
            Assert.IsNotNull(sut.Properties);
        }
        [TestMethod]
        [TestCategory("VisualizationDataFrame")]
        public void VisualizationDataFrame_PropertiesEmpty()
        {
            Assert.IsFalse(sut.Properties.Any());
        }
    }
}
