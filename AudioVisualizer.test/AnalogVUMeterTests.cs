using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace AudioVisualizer.test
{
    [TestClass]
    public class AnalogVUMeterTests
    {
        AnalogVUMeter sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = new AudioVisualizer.AnalogVUMeter();
        }

        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_DefaultSourceIsNull()
        {
            Assert.IsNull(sut.Source);
        }

        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetSource()
        {
            FakeVisualizationSource source = new FakeVisualizationSource();
            sut.Source = source;
            Assert.AreEqual(source, sut.Source);
        }

        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_IsDefaultChannelSetToZero()
        {
            Assert.AreEqual(0, sut.ChannelIndex);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetChannelIndex()
        {
            sut.ChannelIndex = 1;
            Assert.AreEqual(1, sut.ChannelIndex);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_SettingChannelIndexToNegativeThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => 
            {
                sut.ChannelIndex = -1;
            });
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_AreDefaultScaleSectionsSet()
        {
            Assert.AreEqual(2, sut.ScaleSections.Count());
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetScaleSections()
        {
            var sections = new MeterScaleSection[]
            {
                new MeterScaleSection() { Start = 0.0f, Length=0.2f, LineColor=Colors.Blue, LineThickness=5.0f }
            };
            sut.ScaleSections = sections;
            CollectionAssert.AreEqual(sections, sut.ScaleSections);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetScaleSectionsToNull()
        {
            sut.ScaleSections = null;
            Assert.IsNull(sut.ScaleSections);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetScaleLables()
        {
            var labels = new MeterScaleLabel[]
            {
                new MeterScaleLabel() { Label = "X", Position = 0.1f, TickColor = Colors.Wheat, TickLength = 5, TickThickness = 5}
            };
            sut.ScaleLabels = labels;
            CollectionAssert.AreEqual(labels, sut.ScaleLabels);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetScaleLabelsToNull()
        {
            sut.ScaleLabels = null;
            Assert.IsNull(sut.ScaleLabels);
        }

        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_AreDefaultScaleLabelsSet()
        {
            Assert.AreEqual(9, sut.ScaleLabels.Count());
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_IsDefaultDialFixPointSet()
        {
            Assert.AreEqual(new System.Numerics.Vector2(0.5f,1.5f), sut.DialRelativeFixPoint);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetDialFixPoint()
        {
            var newPoint = new System.Numerics.Vector2(0.4f, 1.6f);
            sut.DialRelativeFixPoint = newPoint;
            Assert.AreEqual(newPoint, sut.DialRelativeFixPoint);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_IsDefaulScaleStartPointSet()
        {
            Assert.AreEqual(new System.Numerics.Vector2(0.1f, 0.6f), sut.ScaleRelativeStartingPoint);
        }
        [UITestMethod]
        [TestCategory("AnalogVUMeter")]
        public void AnalogVUMeter_CanSetScaleStartPoint()
        {
            var newPoint = new System.Numerics.Vector2(0.4f, 1.6f);
            sut.ScaleRelativeStartingPoint = newPoint;
            Assert.AreEqual(newPoint, sut.ScaleRelativeStartingPoint);
        }

    }
}
