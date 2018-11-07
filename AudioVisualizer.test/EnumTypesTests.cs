
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AudioVisualizer;

namespace AudioVisualizer.test
{
    [TestClass]
    public class EnumTypesTests
    {
        [TestMethod]
        [TestCategory("ScaleType")]
        public void ScaleType_Values()
        {
            Assert.AreEqual((Int32) ScaleType.Linear, 0);
            Assert.AreEqual((Int32) ScaleType.Logarithmic, 1);
        }

        [TestMethod]
        [TestCategory("CreateResourcesReason")]
        public void CreateResourcesReason_Values()
        {
            Assert.AreEqual((Int32) CreateResourcesReason.New,0);
            Assert.AreEqual((Int32) CreateResourcesReason.DeviceLost,1);
            Assert.AreEqual((Int32) CreateResourcesReason.DpiChanged, 2);
        }
    }
}
