
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AudioVisualizer;

namespace AudioVisualizer.test
{
    [TestClass]
    public class ScaleTypeTests
    {
        [TestMethod]
        public void ScaleType_Values()
        {
            Assert.AreEqual((Int32) ScaleType.Linear, 0);
            Assert.AreEqual((Int32) ScaleType.Logarithmic, 1);
        }
    }
}
