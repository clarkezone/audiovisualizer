using AudioVisualizer;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test.managed
{
    [TestClass]
    public class VisualizationDataFrameTests
    {
        [TestMethod]
        public void VisualizationDataFrame_ctor()
        {
            var rms = ScalarData.CreateEmpty(2);
            var peak = ScalarData.CreateEmpty(2);
            var spectrum = SpectrumData.CreateEmpty(2, 100, ScaleType.Linear, ScaleType.Linear, 0, 20000.0f);
            var frame = new VisualizationDataFrame(TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1.0 / 60.0), rms, peak, spectrum);

            Assert.IsNotNull(frame);
            Assert.AreEqual(TimeSpan.FromSeconds(1), frame.Time);
            Assert.AreEqual(TimeSpan.FromSeconds(1.0 / 60.0), frame.Duration);
            Assert.AreSame(rms, frame.RMS);
            Assert.AreSame(peak, frame.Peak);
            Assert.AreSame(spectrum, frame.Spectrum);
        }
    }
}
