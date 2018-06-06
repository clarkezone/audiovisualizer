using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AudioVisualizer.test
{
    [TestClass]
    public class VisualizerControlTest
    {
        VisualizerControl sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = new AudioVisualizer.VisualizerControl();
        }

        [UITestMethod]
        [TestCategory("VisualizerControl")]
        public void VisualizerControl_DefaultSourceIsNull()
        {
            Assert.IsNull(sut.Source);
        }

        [UITestMethod]
        [TestCategory("VisualizerControl")]
        public void VisualizerControl_CanSetSource()
        {
            FakeVisualizationSource source = new FakeVisualizationSource();
            sut.Source = source;
            Assert.AreEqual(source, sut.Source);
        }

        [UITestMethod]
        [TestCategory("VisualizerControl")]
        public void VisualizerControl_DefaultBackgroundIsTransparent()
        {
            Assert.AreEqual(Windows.UI.Colors.Transparent, sut.BackgroundColor);
        }
        [UITestMethod]
        [TestCategory("VisualizerControl")]
        public void VisualizerControl_CanSetBackground()
        {
            sut.BackgroundColor = Windows.UI.Colors.White;
            Assert.AreEqual(Windows.UI.Colors.White, sut.BackgroundColor);
        }



    }
}
