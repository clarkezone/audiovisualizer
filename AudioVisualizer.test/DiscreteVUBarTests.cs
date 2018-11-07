using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using Windows.ApplicationModel.Core;
using Windows.Foundation;
using Windows.UI;
using Windows.UI.Composition;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Hosting;
using Windows.UI.Xaml.Media;

namespace AudioVisualizer.test
{
    [TestClass]
    public class DiscreteVUBarTests
    {
        DiscreteVUBar sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = new AudioVisualizer.DiscreteVUBar();
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_DefaultSourceIsNull()
        {
            Assert.IsNull(sut.Source);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetSource()
        {
            FakeVisualizationSource source = new FakeVisualizationSource();
            sut.Source = source;
            Assert.AreEqual(source, sut.Source);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_IsDefaultChannelSetToZero()
        {
            Assert.AreEqual(0, sut.ChannelIndex);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetChannelIndex()
        {
            sut.ChannelIndex = 1;
            Assert.AreEqual(1, sut.ChannelIndex);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_SettingChannelIndexToNegativeThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => { sut.ChannelIndex = -1; });
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_IsDefaultLevelsSet()
        {
            CollectionAssert.AreEqual(g_DefaultLevels, sut.Levels);
        }

        MeterBarLevel[] g_LevelsToSet = new MeterBarLevel[]
        {
            new MeterBarLevel() { Level = -10, Color = Colors.Red },
            new MeterBarLevel() { Level = -5, Color = Colors.Red },
            new MeterBarLevel() { Level = 0, Color = Colors.Lime}
        };
        MeterBarLevel[] g_DefaultLevels = new MeterBarLevel[]
        {
            new MeterBarLevel() { Level = -60, Color = Colors.Lime },
            new MeterBarLevel() { Level = -57, Color = Colors.Lime },
            new MeterBarLevel() { Level = -54, Color = Colors.Lime },
            new MeterBarLevel() { Level = -51, Color = Colors.Lime },
            new MeterBarLevel() { Level = -48, Color = Colors.Lime },
            new MeterBarLevel() { Level = -45, Color = Colors.Lime },
            new MeterBarLevel() { Level = -42, Color = Colors.Lime },
            new MeterBarLevel() { Level = -39, Color = Colors.Lime },
            new MeterBarLevel() { Level = -36, Color = Colors.Lime },
            new MeterBarLevel() { Level = -33, Color = Colors.Lime },
            new MeterBarLevel() { Level = -30, Color = Colors.Lime },
            new MeterBarLevel() { Level = -27, Color = Colors.Lime },
            new MeterBarLevel() { Level = -24, Color = Colors.Lime },
            new MeterBarLevel() { Level = -21, Color = Colors.Lime },
            new MeterBarLevel() { Level = -18, Color = Colors.Lime },
            new MeterBarLevel() { Level = -15, Color = Colors.Lime },
            new MeterBarLevel() { Level = -12, Color = Colors.Lime },
            new MeterBarLevel() { Level = -9, Color = Colors.Lime },
            new MeterBarLevel() { Level = -6, Color = Colors.Yellow },
            new MeterBarLevel() { Level = -3, Color = Colors.Yellow },
            new MeterBarLevel() { Level = -0, Color = Colors.Yellow },
            new MeterBarLevel() { Level = 3, Color = Colors.Red },
            new MeterBarLevel() { Level = 6, Color = Colors.Red },
        };

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetLevels()
        {
            sut.Levels = g_LevelsToSet;
            CollectionAssert.AreEqual(g_LevelsToSet, sut.Levels);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_SettingLevelsToNullThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                sut.Levels = null;
            });
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_SettingLevelsToEmptyThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                sut.Levels = new MeterBarLevel[] { };
            });
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_SettingLevelsToNonAscendingOrderThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                sut.Levels = new MeterBarLevel[] {
                    new MeterBarLevel() { Level = -100 },
                    new MeterBarLevel() { Level = -100 },
                    new MeterBarLevel() { Level = -80 },
                };
            });
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_IsDefaultOrientationVertical()
        {
            Assert.AreEqual(Orientation.Vertical, sut.Orientation);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetOrientation()
        {
            sut.Orientation = Orientation.Horizontal;
            Assert.AreEqual(Orientation.Horizontal, sut.Orientation);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_IsDefaultRelativeElementMarginZero()
        {
            Assert.AreEqual(new Thickness(0), sut.RelativeElementMargin);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetRelativeElementMargin()
        {
            var margin = new Thickness(0.1);
            sut.RelativeElementMargin = margin;
            Assert.AreEqual(margin, sut.RelativeElementMargin);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_SettingTooWideHorizontalMargin()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.RelativeElementMargin = new Thickness(0.5, 0, 0.5, 0);
            });
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_SettingTooWideVerticalMargin()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                sut.RelativeElementMargin = new Thickness(0, 0.5, 0, 0.5);
            });
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_IsDefaultUnlitElementTransparent()
        {
            Assert.AreEqual(Colors.Transparent, sut.UnlitElement);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetUnlitElement()
        {
            sut.UnlitElement = Colors.Gray;
            Assert.AreEqual(Colors.Gray, sut.UnlitElement);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ShadowBlurRadiusIsZero()
        {
            Assert.AreEqual(0, sut.ElementShadowBlurRadius);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetShadowBlurRadius()
        {
            sut.ElementShadowBlurRadius = 1.0f;
            Assert.AreEqual(1.0f, sut.ElementShadowBlurRadius);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ShadowColorIsTransparent()
        {
            Assert.AreEqual(Colors.Transparent, sut.ElementShadowColor);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetShadowColor()
        {
            sut.ElementShadowColor = Colors.Black;
            Assert.AreEqual(Colors.Black, sut.ElementShadowColor);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ShadowOffsetIsZeroSet()
        {
            Assert.AreEqual(new System.Numerics.Vector3(0,0,0), sut.ElementShadowOffset);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetShadowOffset()
        {
            sut.ElementShadowOffset = new Vector3(1, 1, 1);
            Assert.AreEqual(new Vector3(1, 1, 1), sut.ElementShadowOffset);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ShadowOpacityIsOne()
        {
            Assert.AreEqual(1.0f, sut.ElementShadowOpacity);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CanSetShadowOpacity()
        {
            sut.ElementShadowOpacity = 0.0f;
            Assert.AreEqual(0.0f, sut.ElementShadowOpacity);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ElementVisualIsContainerVisual()
        {
            Assert.IsInstanceOfType(ElementCompositionPreview.GetElementChildVisual(sut), typeof(ContainerVisual));
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ElementVisualChildrenAreOfRightType()
        {
            Type[] expectedTypes = new Type[] { typeof(SpriteVisual), typeof(ContainerVisual) };
            var elementVisual = (ContainerVisual) ElementCompositionPreview.GetElementChildVisual(sut);
            CollectionAssert.AreEqual(expectedTypes, elementVisual.Children.Select(visual => visual.GetType()).ToArray());
        }


        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_BackgroundVisualHasBackgroundBrush()
        {
            var elementVisual = (ContainerVisual)ElementCompositionPreview.GetElementChildVisual(sut);
            var backgroundVisual = (SpriteVisual) elementVisual.Children.First();
            sut.Background = new SolidColorBrush(Colors.OrangeRed);

            var background = (CompositionColorBrush) backgroundVisual.Brush;
            Assert.AreEqual(Colors.OrangeRed, background.Color);
        }

        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_CorrectAmountOfBarElementVisuals()
        {
            var elementVisual = (ContainerVisual)ElementCompositionPreview.GetElementChildVisual(sut);
            var elementContainer = (ContainerVisual)elementVisual.Children.Last();
            Assert.AreEqual(sut.Levels.Count(), elementContainer.Children.Count);
        }

        class CustomElementFactory : AudioVisualizer.IBarElementFactory
        {
            public List<(object,Windows.UI.Color,Size,Compositor,CompositionGraphicsDevice)> Calls = new List<(object,Color,Size,Compositor,CompositionGraphicsDevice)>();
            public CompositionBrush CreateElementBrush(object sender, Color elementColor, Size size, Compositor compositor, CompositionGraphicsDevice device)
            {
                Calls.Add((sender, elementColor, size, compositor, device));
                return compositor.CreateColorBrush(elementColor);
            }

            public CompositionBrush CreateShadowMask(object sender, Size size, Compositor compositor, CompositionGraphicsDevice device)
            {
                return null;
            }
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ElementFactory_CreateBrushCalledForUnlitElement()
        {
            var factory = new CustomElementFactory();
            sut.ElementFactory = factory;
            factory.Calls.Clear();
            sut.UnlitElement = Colors.Blue;
            Assert.AreEqual(1, factory.Calls.Count);
            var call = factory.Calls.First();
            Assert.AreSame(sut, call.Item1);
            Assert.AreEqual(Colors.Blue, call.Item2);
            Assert.AreEqual(new Size(0,0), call.Item3);
            Assert.IsNotNull(call.Item4);
            Assert.IsNotNull(call.Item5);
        }
        [UITestMethod]
        [TestCategory("DiscreteVUBar")]
        public void DiscreteVUBar_ElementFactory_CreateBrushCalledForLitElements()
        {
            var factory = new CustomElementFactory();
            sut.ElementFactory = factory;
            factory.Calls.Clear();
            sut.Levels = new MeterBarLevel[]
            {
                new MeterBarLevel() { Color = Colors.Red, Level = -20 },
                new MeterBarLevel() { Color = Colors.Red, Level = -10 },
                new MeterBarLevel() { Color = Colors.Green, Level = 0 }
            };
            Assert.AreEqual(2, factory.Calls.Count);
            Assert.IsTrue(factory.Calls.Exists( (obj) => obj.Item2 == Colors.Red),"Should contain Red element call");
            Assert.IsTrue(factory.Calls.Exists((obj) => obj.Item2 == Colors.Green),"Should contain Green element call");
            var call = factory.Calls.First();
            Assert.AreSame(sut, call.Item1);
            Assert.AreEqual(new Size(0, 0), call.Item3);
            Assert.IsNotNull(call.Item4);
            Assert.IsNotNull(call.Item5);
        }
    }
}
