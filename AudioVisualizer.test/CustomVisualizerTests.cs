using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;

namespace AudioVisualizer.test
{
    [TestClass]
    public class CustomVisualizerTests
    {
        CustomVisualizer sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = new AudioVisualizer.CustomVisualizer();
        }

    }
}
