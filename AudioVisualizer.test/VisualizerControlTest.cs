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
        [UITestMethod]
        [TestCategory("VisualizerControl")]
        public void VisualizerControl_Init()
        {
            var ctrl = new AudioVisualizer.VisualizerControl();
        }
    }
}
