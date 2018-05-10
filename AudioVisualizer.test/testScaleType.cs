
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AudioVisualizer;

namespace AudioVisualizer.test
{
    [TestClass]
    public class testScaleType
    {
        [TestMethod]
        public void ScaleType_Values()
        {
            Assert.AreEqual(ScaleType.Linear, 0);
            Assert.AreEqual(ScaleType.Logarithmic, 1);
        }
    }
}
