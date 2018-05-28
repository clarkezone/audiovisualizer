using AudioVisualizer;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AudioVisualizer.test
{
    [TestClass]
    public class SpectrumDataInitLinear
    {
        protected SpectrumData sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000);
        }

        [TestCategory("SpectrumData")]
        [TestMethod]
        public void SpectrumData_Init_ChannelCount()
        {
            Assert.AreEqual(2, sut.Count());
            Assert.AreEqual(2, sut.Count);
        }
        [TestCategory("SpectrumData")]
        [TestMethod]
        public void SpectrumData_Init_AmpScaleType()
        {
            Assert.AreEqual(ScaleType.Linear, sut.AmplitudeScale);
        }
        [TestCategory("SpectrumData")]
        [TestMethod]
        public void SpectrumData_Init_FScaleType()
        {
            Assert.AreEqual(ScaleType.Linear, sut.FrequencyScale);
        }
        [TestCategory("SpectrumData")]
        [TestMethod]
        public void SpectrumData_Init_ItemCount()
        {
            foreach (var item in sut)
            {
                Assert.AreEqual(10, item.Count);
            }
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_FrequencyCount()
        {
            Assert.AreEqual(10u, sut.FrequencyCount, "Frequency bin count");
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_MinFrequency()
        {
            Assert.AreEqual(0.0f, sut.MinFrequency, "Min Frequency property init");
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_MaxFrequency()
        {
            Assert.AreEqual(20000.0f, sut.MaxFrequency, "Max Frequency property init");
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_FStep()
        {
            Assert.AreEqual(2000.0f, sut.FrequencyStep);
        }

        float[][] g_InitialValues = {
                new float[] { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f },
                new float[] { 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }
                };

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CreateWithValues()
        {

            var data = SpectrumData.Create(g_InitialValues, ScaleType.Linear, ScaleType.Linear, 0, 20000);
            CollectionAssert.AreEqual(g_InitialValues[0], data[0].ToArray());
            CollectionAssert.AreEqual(g_InitialValues[1], data[1].ToArray());
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CreateArgs_ValuesAreNull()
        {
            Assert.ThrowsException<NullReferenceException>(
                () =>
                {
                    var s = SpectrumData.Create(null, ScaleType.Linear, ScaleType.Linear, 0.0f, 20000f);
                }, "Null initializer");
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CreateArgs_ChannelValuesAreDifferentSize()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                var s = SpectrumData.Create(new float[][] { new float[] { 1, 2, 3 }, new float[] { 1, 2, 3, 4 } },
                    ScaleType.Linear, ScaleType.Linear, 0.0f, 20000f);
            }, "Unequal channel initializers");
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CreateArgs_MinF_GT_MaxF()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                var s = SpectrumData.Create(g_InitialValues, ScaleType.Linear, ScaleType.Linear, 30000.0f, 20000f);
            }, "MinFrequency > MaxFrequency");
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CreateArgs_MinF_EQ_MaxF()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.Create(g_InitialValues, ScaleType.Linear, ScaleType.Linear, 20000f, 20000f);
                }, "MinFrequency == MaxFrequency");
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CreateArgs_MinF_Negative()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                var s = SpectrumData.Create(g_InitialValues, ScaleType.Linear, ScaleType.Linear, -1.0f, 20000f);
            }, "MinFrequency < 0");
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CreateArgs_MinFZeroWhenFScaleLog()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                var s = SpectrumData.Create(g_InitialValues, ScaleType.Linear, ScaleType.Logarithmic, 0.0f, 20000f);
            }, "MinFrequency == 0 while FrequencyScale == Logarithmic");
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Linear_GetCenterFrequency()
        {
            Assert.AreEqual(1000f, sut.GetCenterFrequency(0));
            Assert.AreEqual(3000f, sut.GetCenterFrequency(1));
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Linear_GetFrequency()
        {
            Assert.AreEqual(0f, sut.GetFrequency(0));
            Assert.AreEqual(2000f, sut.GetFrequency(1));
        }
    }

    [TestClass]
    public class SpectrumDataInitLog
    {
        protected SpectrumData sut;

        [TestInitialize]
        public void TestInit()
        {
            sut = SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Logarithmic, 10, 10000);
        }

        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_LogF_AmpScaleType()
        {
            Assert.AreEqual(ScaleType.Linear, sut.AmplitudeScale);
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_LogF_FScaleType()
        {
            Assert.AreEqual(ScaleType.Logarithmic, sut.FrequencyScale);
        }

        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_LogF_MinFrequency()
        {
            Assert.AreEqual(10.0f, sut.MinFrequency, "Min Frequency property init");
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_LogF_MaxFrequency()
        {
            Assert.AreEqual(10000.0f, sut.MaxFrequency, "Max Frequency property init");
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Init_LogF_FStep()
        {
            Assert.AreEqual(1.995262f, sut.FrequencyStep, 1e-6f);
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Log_GetCenterFrequency()
        {
            Assert.AreEqual(14.12538f, sut.GetCenterFrequency(0),0.00001);
            Assert.AreEqual(28.18383f, sut.GetCenterFrequency(1),0.00001);
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_Log_GetFrequency()
        {
            Assert.AreEqual(10.0f, sut.GetFrequency(0),0.0001);
            Assert.AreEqual(19.95262f, sut.GetFrequency(1),0.0001);
        }
    }

    [TestClass]
    public class SpectrumDataOperations
    {
        SpectrumData ltd;
        [TestInitialize]
        public void TestInit()
        {
            ltd = SpectrumData.Create(
                new float[][]
                {
                    new float [] { 1,2,3,4,5 },
                    new float [] { 10,20,30,40,50 }
                },
                ScaleType.Linear,
                ScaleType.Linear,
                0,
                20000
                );
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LinearTransform_1()
        {
            // Case 1 exact division - bins are at 4000 -> mapping to 8000
            var s1 = ltd.LinearTransform(3, 0, 24000);
            Assert.AreEqual(2, s1.Count);
            Assert.AreEqual(3u, s1.FrequencyCount);
            Assert.AreEqual(ScaleType.Linear, s1.AmplitudeScale);
            Assert.AreEqual(ScaleType.Linear, s1.FrequencyScale);
            Assert.AreEqual(8000.0f, s1.FrequencyStep);
            Assert.AreEqual(0.0f, s1.MinFrequency);
            Assert.AreEqual(24000.0f, s1.MaxFrequency);
            CollectionAssert.AreEqual(new float[] { 4, 8, 2.5f }, s1[0].ToArray());
            CollectionAssert.AreEqual(new float[] { 40, 80, 25 }, s1[1].ToArray());
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LinearTransform_2()
        {
            // Case 2 result bins are > 1 but fractional and shifted up by 1k
            var s2 = ltd.LinearTransform(4, 1000, 25000);
            Assert.AreEqual(4u, s2.FrequencyCount);
            Assert.AreEqual(6000.0f, s2.FrequencyStep);
            Assert.AreEqual(1000.0f, s2.MinFrequency);
            Assert.AreEqual(25000.0f, s2.MaxFrequency);
            CollectionAssert.AreEqual(new float[] { 3.0f, 5.25f, 5.8125f, 0.15625f }, s2[0].ToArray());
            CollectionAssert.AreEqual(new float[] { 30f, 52.5f, 58.125f, 1.5625f }, s2[1].ToArray());
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LinearTransform_LogAmpScaleThrows()
        {
            Assert.ThrowsException<COMException>(() => 
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 10000).LinearTransform(5, 0, 4000);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LinearTransform_LogFreqScaleThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Logarithmic, 10, 10000).LinearTransform(5, 20, 4000);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LinearTransform_ZeroResultSizeThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 10, 10000).LinearTransform(0, 20, 4000);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LinearTransform_FromFrequencyGTEToFrequencyThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 10, 10000).LinearTransform(5, 4000, 4000);
            });
        }

        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LogarithmicTransform()
        {
            SpectrumData data = SpectrumData.Create(
                new float[][]
                {
                    new float[] { 0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f,0.8f,0.9f,1.0f }
                },
                ScaleType.Linear,
                ScaleType.Linear,
                0f, 1000f);
            var log = data.LogarithmicTransform(3, 1, 1000);
            Assert.AreEqual(1, log.Count);
            Assert.AreEqual(3u, log.FrequencyCount);
            Assert.AreEqual(1, log.MinFrequency);
            Assert.AreEqual(1000, log.MaxFrequency);

            var expected = new float[] { 0.009495f, 0.1395f, 5.3f };
            for (int index = 0; index < log.Count; index++)
            {
                Assert.AreEqual(expected[0], log[0][0], 1e-9);
                Assert.AreEqual(expected[1], log[0][1], 1e-7);
                Assert.AreEqual(expected[2], log[0][2], 1e-5);
            }
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LogTransform_LogAmpScaleThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 10000).LogarithmicTransform(5, 0, 4000);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LogTransform_LogFreqScaleThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Logarithmic, 10, 10000).LogarithmicTransform(5, 20, 4000);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LogTransform_ZeroResultSizeThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 10, 10000).LogarithmicTransform(0, 20, 4000);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LogTransform_FromFrequencyGTEToFrequencyThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 10, 10000).LogarithmicTransform(5, 4000, 4000);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_LogTransform_ZeroFromFrequencyThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 10000).LogarithmicTransform(5, 0, 4000);
            });
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ConvertToDecibels()
        {
            var testValues = new float[][] { new float[] { 0.0f, 0.1f, 1.0f, 1e-6f, 1e6f, -1 } };
            var data = SpectrumData.Create(testValues, ScaleType.Linear, ScaleType.Linear, 20, 20000);
            var logData = data.ConvertToDecibels(-100, 0);
            CollectionAssert.AreEqual(new float[] { -100.0f, -20.0f, 0.0f, -100.0f, 0.0f, -100.0f }, logData[0].ToArray());
        }

        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ConvertToDecibels_WithLogScaleThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                var d = SpectrumData.CreateEmpty(2, 100, ScaleType.Logarithmic, ScaleType.Linear, 0, 20000).ConvertToDecibels(-100, 0);
            });
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ConvertToDecibels_MinGTEMaxThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var d = SpectrumData.CreateEmpty(2, 100, ScaleType.Linear, ScaleType.Linear, 0, 20000).ConvertToDecibels(0, 0);
            });
        }


        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CombineChannels()
        {
            SpectrumData data = SpectrumData.Create(
                    new float[][]
                    {
                        new float[] { 1,0,0,0,0 },
                        new float[] { 0,1,0,0,0 },
                        new float[] { 0,0,1,0,0 },
                        new float[] { 0,0,0,1,0 },
                        new float[] { 0,0,0,0,1 },
                    },
                    ScaleType.Linear,
                    ScaleType.Linear,
                    0,
                    20000
                );

            var spectrum = data.CombineChannels(new float[] { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f });
            Assert.AreEqual(2, spectrum.Count);
            Assert.AreEqual(5u, spectrum.FrequencyCount);
            Assert.AreEqual(ScaleType.Linear, spectrum.AmplitudeScale);
            Assert.AreEqual(ScaleType.Linear, spectrum.FrequencyScale);
            Assert.AreEqual(0.0f, spectrum.MinFrequency);
            Assert.AreEqual(20000f, spectrum.MaxFrequency);
            Assert.AreEqual(4000f, spectrum.FrequencyStep);
            CollectionAssert.AreEqual(new float[] { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f }, spectrum[0].ToArray());
            CollectionAssert.AreEqual(new float[] { 0.6f, 0.7f, 0.8f, 0.9f, 1.0f }, spectrum[1].ToArray());
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CombineChannels_NullMapThrows()
        {
            Assert.ThrowsException<NullReferenceException>(() =>
            {
                var d = SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000).CombineChannels(null);
            });
        }
        [TestMethod()]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CombineChannels_MapWithLessSizeThanChannelsThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var d = SpectrumData.CreateEmpty(2, 100, ScaleType.Linear, ScaleType.Linear, 0, 20000).CombineChannels(new float[] { 1 });
            });
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_CombineChannels_WithLogScaleThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                var d = SpectrumData.CreateEmpty(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 20000).CombineChannels(new float[] { 1, 1 });
            });
        }

        float[][] g_ExpectedRFResult = new float[][]
        { new float[] { 0.3934682f,0.7869364f,1.18040466f,1.5738728f,1.967341f },
                new float[] { 3.934682f,7.869364f,11.8040457f,15.7387276f,19.67341f } };
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFall()
        {
            var previous = SpectrumData.CreateEmpty(2, 5, ScaleType.Linear, ScaleType.Linear, 0, 20000);
            var result = ltd.ApplyRiseAndFall(previous, TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(4), TimeSpan.FromSeconds(1));
            CollectionAssert.AreEqual(g_ExpectedRFResult[0], result[0].ToArray());
            CollectionAssert.AreEqual(g_ExpectedRFResult[1], result[1].ToArray());
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFall_WithNullPrevious()
        {
            var result = ltd.ApplyRiseAndFall(null, TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(4), TimeSpan.FromSeconds(1));
            CollectionAssert.AreEqual(g_ExpectedRFResult[0], result[0].ToArray());
            CollectionAssert.AreEqual(g_ExpectedRFResult[1], result[1].ToArray());
        }

        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFall_LogAmpScaleThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                SpectrumData.CreateEmpty(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 20000).ApplyRiseAndFall(null, TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1));
            });
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFall_WithPreviousLogAmpScaleThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var p = SpectrumData.CreateEmpty(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 20000);
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000).ApplyRiseAndFall(p, TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1));
            });
        }

        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFall_WithNEChannelCountThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var p = SpectrumData.CreateEmpty(1, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000);
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000).ApplyRiseAndFall(p, TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1));
            });
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFall_WithNEElementCountThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var p = SpectrumData.CreateEmpty(2, 20, ScaleType.Linear, ScaleType.Linear, 0, 20000);
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000).ApplyRiseAndFall(p, TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1));
            });
        }

        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFallToEmpty()
        {
            SpectrumData.ApplyRiseAndFallToEmpty(ltd, TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(1));
        }

        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFallToEmpty_WithNullPreviousThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                SpectrumData.ApplyRiseAndFallToEmpty(null, TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(1));
            });
        }
        [TestMethod]
        [TestCategory("SpectrumData")]
        public void SpectrumData_ApplyRiseAndFallToEmpty_WithLogAmpScalePreviousThrows()
        {
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var p = SpectrumData.CreateEmpty(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 20000);
                SpectrumData.ApplyRiseAndFallToEmpty(p, TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(1));
            });
        }
    }

}
