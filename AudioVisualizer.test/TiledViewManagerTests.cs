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
            Assert.AreEqual(new ContentUnit() { X = 0, Y = 0 } , sut.ContentOffset);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ContentSizeZero()
        {
            Assert.AreEqual(new ContentUnit() { X = 0, Y = 0 }, sut.ContentSize);
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
            Assert.AreEqual(new Vector2(0,0), sut.ViewportOffset);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_IsDefault_ViewportSizeZero()
        {
            Assert.AreEqual(new Vector2(0, 0), sut.ViewportSize);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ContentOffset()
        {
            sut.ContentOffset = new ContentUnit() { X = 1, Y = 2 };
            Assert.AreEqual(new ContentUnit() { X = 1, Y = 2 }, sut.ContentOffset);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ContentSize()
        {
            sut.ContentSize = new ContentUnit() { X = 1, Y = 2 };
            Assert.AreEqual(new ContentUnit() { X = 1, Y = 2 }, sut.ContentSize);
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
            sut.ViewportOffset = new Vector2(1, 2);
            Assert.AreEqual(new Vector2(1, 2), sut.ViewportOffset);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ViewportSizeZero()
        {
            sut.ViewportSize = new Vector2(1, 2);
            Assert.AreEqual(new Vector2(1, 2), sut.ViewportSize);
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
        public void TiledViewManager_Set_ContentSizeNegativeX_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.ContentSize = new ContentUnit() { X = -1, Y = 0 };
            });
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ContentSizeNegativeY_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.ContentSize = new ContentUnit() { X = 0, Y = -1 };
            });
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ViewportSizeNegativeX_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.ViewportSize = new Vector2(-1, 0);
            });
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Set_ViewportSizeNegativeY_Throws()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.ViewportSize = new Vector2(0, -1);
            });
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Resolution_ViewToContentUnits()
        {
            sut.Resolution = 2;
            var converted = sut.ViewToContent(new Vector2(20, -40));
            Assert.AreEqual(new ContentUnit() { X = 10, Y = -20 },converted);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Resolution_ContentToViewUnits()
        {
            sut.Resolution = 2;
            var converted = sut.ContentToView(new ContentUnit() { X = 10, Y = -20 });
            Assert.AreEqual(new Vector2(20, -40), converted);
        }
        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Zoom_ViewToContentUnits()
        {
            sut.Zoom = 2;
            var converted = sut.ViewToContent(new Vector2(20, -40));
            Assert.AreEqual(new ContentUnit() { X = 10, Y = -20 }, converted);
        }

        [TestMethod()]
        [TestCategory("TiledViewManager")]
        public void TiledViewManager_Zoom_ContentToViewUnits()
        {
            sut.Zoom = 2.0;
            var converted = sut.ContentToView(new ContentUnit() { X = 10, Y = -20 });
            Assert.AreEqual(new Vector2(20, -40), converted);
        }
    }
}
