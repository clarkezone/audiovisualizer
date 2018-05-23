using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;

namespace AudioVisualizer.test
{
    [TestClass]
    public class VisualizationDrawArgsTest
    {
        AudioVisualizer.VisualizerDrawEventArgs sut;

        VisualizationDataFrame expectedDataFrame = new VisualizationDataFrame(TimeSpan.FromSeconds(1), TimeSpan.FromMilliseconds(100), ScalarData.CreateEmpty(2), ScalarData.CreateEmpty(2), SpectrumData.CreateEmpty(2, 20, ScaleType.Linear, ScaleType.Logarithmic, 20, 20000));
        Object expectedDrawSession = new object();
        Size expectedViewExtent = new Size(200, 100);
        TimeSpan expectedPresentationTime = TimeSpan.FromMinutes(1);

        [TestInitialize]
        public void TestInit()
        {
            sut = new AudioVisualizer.VisualizerDrawEventArgs(expectedDataFrame, expectedDrawSession,expectedViewExtent, expectedPresentationTime);
        }

        [TestMethod]
        [TestCategory("VisualizerDrawEventArgs")]
        public void VisualizerDrawEventArgs_Data()
        {
            Assert.AreEqual(expectedDataFrame, sut.Data);
        }
        [TestMethod]
        [TestCategory("VisualizerDrawEventArgs")]
        public void VisualizerDrawEventArgs_DrawSession()
        {
            Assert.AreEqual(expectedDrawSession, sut.DrawingSession);
        }
        [TestMethod]
        [TestCategory("VisualizerDrawEventArgs")]
        public void VisualizerDrawEventArgs_ViewExtent()
        {
            Assert.AreEqual(expectedViewExtent, sut.ViewExtent);
        }
        [TestMethod]
        [TestCategory("VisualizerDrawEventArgs")]
        public void VisualizerDrawEventArgs_PresentationTime()
        {
            Assert.AreEqual(expectedPresentationTime, sut.PresentationTime);
        }

    }
}
