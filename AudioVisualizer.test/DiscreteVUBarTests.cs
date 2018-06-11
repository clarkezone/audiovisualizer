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
    public class DiscreteVUBarTests
    {
        DiscreteVUBar sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = new AudioVisualizer.DiscreteVUBar();
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void VisualizerControl_DefaultSourceIsNull()
        {
            Assert.IsNull(sut.Source);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void VisualizerControl_CanSetSource()
        {
            FakeVisualizationSource source = new FakeVisualizationSource();
            sut.Source = source;
            Assert.AreEqual(source, sut.Source);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void VisualizerControl_DefaultBackgroundIsTransparent()
        {
            Assert.AreEqual(Windows.UI.Colors.Transparent, sut.BackgroundColor);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void VisualizerControl_CanSetBackground()
        {
            sut.BackgroundColor = Windows.UI.Colors.White;
            Assert.AreEqual(Windows.UI.Colors.White, sut.BackgroundColor);
        }



    }
}
