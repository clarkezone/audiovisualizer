using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;

namespace test.managed
{
    [TestClass()]
    public class CustomVisualizerTests
    {
        [UITestMethod]
        public void CustomVisualizer_if()
        {
            var v = new AudioVisualizer.CustomVisualizer();
            Assert.IsNotNull(v);
            Assert.IsInstanceOfType(v, typeof(UIElement));
            Assert.IsInstanceOfType(v, typeof(FrameworkElement));
            Assert.IsInstanceOfType(v, typeof(Control));


        }
    }
}
