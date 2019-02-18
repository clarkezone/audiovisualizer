using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;

namespace AudioVisualizer.test
{
    [TestClass()]
    public class BinaryScaleManagerTests
    {
        BinaryScaleManager sut;
        [TestInitialize]
        public void Init()
        {
            sut = new BinaryScaleManager();
        }
        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManager_GetRange_0_5()
        {
            var range = sut.GetRange(0.5);
            Assert.AreEqual(new ScaleRange() { From = .5, To = 1.0 }, range);
        }
        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManager_GetRange_2()
        {
            var range = sut.GetRange(2.0);
            Assert.AreEqual(new ScaleRange() { From = 2.0, To = 4.0 },range);
        }
        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManager_GetRange_3()
        {
            var range = sut.GetRange(3.0);
            Assert.AreEqual(new ScaleRange() { From = 2.0, To = 4.0 }, range);
        }
        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManager_GetRange_4()
        {
            var range = sut.GetRange(4.0);
            Assert.AreEqual(new ScaleRange() { From = 4.0, To = 8.0 }, range);
        }
    }

    [TestClass]
    public class BinaryScaleManagerWithStepTests
    {
        BinaryScaleManager sut;
        [TestInitialize]
        public void Init()
        {
            sut = new BinaryScaleManager(1,3);
        }

        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManagerWS_GetRange_1()
        {
            var range = sut.GetRange(1.0);
            Assert.AreEqual(new ScaleRange() { From = 0.25, To = 2.0 }, range);
        }

        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManagerWS_GetRange_2()
        {
            var range = sut.GetRange(2.0);
            Assert.AreEqual(new ScaleRange() { From = 2.0, To = 16.0 }, range);
        }

        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManagerWS_GetRange_8()
        {
            var range = sut.GetRange(8.0);
            Assert.AreEqual(new ScaleRange() { From = 2.0, To = 16.0 }, range);
        }
        [TestMethod()]
        [TestCategory("ScaleManager")]
        public void BinaryScaleManagerWS_GetRange_16()
        {
            var range = sut.GetRange(16.0);
            Assert.AreEqual(new ScaleRange() { From = 16.0, To = 128.0 }, range);
        }
    }

}
