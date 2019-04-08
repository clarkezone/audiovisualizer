using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;
using System.Numerics;

namespace AudioVisualizer.test
{
    [TestClass]
    public class TiledViewManagerTests
    {
        TiledViewManager sut;

        [TestInitialize()]
        public void Init()
        {
            sut = new TiledViewManager();
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ContentOffsetZero()
        {
            Assert.AreEqual(0.0, sut.ContentOffset);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ContentSizeZero()
        {
            Assert.AreEqual(0.0, sut.ContentSize);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ResolutionOne()
        {
            Assert.AreEqual(1.0, sut.Resolution);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ZoomOne()
        {
            Assert.AreEqual(1.0, sut.Zoom);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ViewportOffsetZero()
        {
            Assert.AreEqual(0.0f, sut.ViewportOffset);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ViewportSizeZero()
        {
            Assert.AreEqual(0.0f, sut.ViewportSize);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ContentOffset()
        {
            sut.ContentOffset = 1;
            Assert.AreEqual(1, sut.ContentOffset);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ContentSize()
        {
            sut.ContentSize = 2;
            Assert.AreEqual(2, sut.ContentSize);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_Resolution()
        {
            sut.Resolution = 10.0;
            Assert.AreEqual(10.0, sut.Resolution);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_Zoom()
        {
            sut.Zoom = 10.0;
            Assert.AreEqual(10.0, sut.Zoom);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ViewportOffset()
        {
            sut.ViewportOffset = 2;
            Assert.AreEqual(2, sut.ViewportOffset);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ViewportSizeZero()
        {
            sut.ViewportSize = 1;
            Assert.AreEqual(1, sut.ViewportSize);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ZoomZero_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() => 
            {
                sut.Zoom = 0;
            });
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ResolutionZero_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.Resolution = 0;
            });
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ContentSizeNegativeValue_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.ContentSize = -1;
            });
        }


        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ViewportSizeNegativeValue_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.ViewportSize = -1;
            });
        }


        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Resolution_ViewToContentUnits()
        {
            sut.Resolution = 2;
            var converted = sut.ViewToContent(-40);
            Assert.AreEqual(-20,converted);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Resolution_ContentToViewUnits()
        {
            sut.Resolution = 2;
            var converted = sut.ContentToView(10);
            Assert.AreEqual(20, converted);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Zoom_ViewToContentUnits()
        {
            sut.Zoom = 2;
            var converted = sut.ViewToContent(-40);
            Assert.AreEqual(-20, converted);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Zoom_ContentToViewUnits()
        {
            sut.Zoom = 2.0;
            var converted = sut.ContentToView(10);
            Assert.AreEqual(20, converted);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ViewportSize()
        {
            sut.ViewportSize = 1920;
        }

    }
}
