using AudioVisualizer;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
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

        [TestMethod]
        public void SpectrumData_Init_ChannelCount()
        {
            Assert.AreEqual(2, sut.Count());
            Assert.AreEqual(2, sut.Count);
        }
        [TestMethod]
        public void SpectrumData_Init_AmpScaleType()
        {
            Assert.AreEqual(ScaleType.Linear, sut.AmplitudeScale);
        }
        public void SpectrumData_Init_FScaleType()
        {
            Assert.AreEqual(ScaleType.Linear, sut.FrequencyScale);
        }

        [TestMethod]
        public void SpectrumData_Init_ItemCount()
        {
            foreach (var item in sut)
            {
                Assert.AreEqual(10, item.Count);
            }
        }
        [TestMethod]
        public void SpectrumData_Init_FrequencyCount()
        {
            Assert.AreEqual(10u, sut.FrequencyCount, "Frequency bin count");
        }
        [TestMethod]
        public void SpectrumData_Init_MinFrequency()
        {
            Assert.AreEqual(0.0f, sut.MinFrequency, "Min Frequency property init");
        }
        [TestMethod]
        public void SpectrumData_Init_MaxFrequency()
        {
            Assert.AreEqual(20000.0f, sut.MaxFrequency, "Max Frequency property init");
        }
        [TestMethod]
        public void SpectrumData_Init_FStep()
        {
            Assert.AreEqual(2000.0f, sut.FrequencyStep);
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
        public void SpectrumData_Init_LogF_AmpScaleType()
        {
            Assert.AreEqual(ScaleType.Linear, sut.AmplitudeScale);
        }
        public void SpectrumData_Init_LogF_FScaleType()
        {
            Assert.AreEqual(ScaleType.Logarithmic, sut.FrequencyScale);
        }

        [TestMethod]
        public void SpectrumData_Init_LogF_MinFrequency()
        {
            Assert.AreEqual(10.0f, sut.MinFrequency, "Min Frequency property init");
        }
        [TestMethod]
        public void SpectrumData_Init_LogF_MaxFrequency()
        {
            Assert.AreEqual(10000.0f, sut.MaxFrequency, "Max Frequency property init");
        }
        [TestMethod]
        public void SpectrumData_Init_LogF_FStep()
        {
            Assert.AreEqual(1.995262f, sut.FrequencyStep, 1e-6f);
        }
    }

    [TestClass]
    public class SpectrumDataTests
    {
        [TestInitialize]
        [TestMethod()]
        public void SpectrumData_CreateEmpty()
        {
            var data = SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000);
            Assert.AreEqual(2, data.Count());
            Assert.AreEqual(2, data.Count, "Channel count property init");
            foreach (var item in data)
            {
                Assert.AreEqual(10, item.Count);
            }
            Assert.AreEqual(10u, data.FrequencyCount, "Frequency bin count");
            Assert.AreEqual(20000.0f, data.MaxFrequency, "Max Frequency property init");
            Assert.AreEqual(0.0f, data.MinFrequency, "Min Frequency property init");
            Assert.AreEqual(2000.0f, data.FrequencyStep);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);

            data = SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Logarithmic, 20, 20000);
            Assert.AreEqual(20, data.MinFrequency);
            Assert.AreEqual(20000, data.MaxFrequency);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            Assert.AreEqual(ScaleType.Logarithmic, data.FrequencyScale);
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(10u, data.FrequencyCount);
            Assert.AreEqual(1.995262f, data.FrequencyStep, 1e-6f);
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.CreateEmpty(0, 10, ScaleType.Linear, ScaleType.Linear, 0.0f, 20000f);
                }, "Zero channels value");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.CreateEmpty(2, 0, ScaleType.Linear, ScaleType.Linear, 0.0f, 20000f);
                }, "Zero elements value");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.CreateEmpty(0, 10, ScaleType.Linear, ScaleType.Linear, 30000.0f, 20000f);
                }, "MinFrequency > MaxFrequency");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.CreateEmpty(0, 10, ScaleType.Linear, ScaleType.Linear, 20000f, 20000f);
                }, "MinFrequency == MaxFrequency");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.CreateEmpty(0, 10, ScaleType.Linear, ScaleType.Linear, -1.0f, 20000f);
                }, "MinFrequency < 0");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.CreateEmpty(0, 10, ScaleType.Linear, ScaleType.Logarithmic, 0.0f, 20000f);
                }, "MinFrequency == 0 while FrequencyScale == Logarithmic");
        }

        static float[][] g_LogValues =
        {
                new float[] { -100.0f,-80.0f,-60.0f,-40.0f,-20.0f },
                new float[] { -90.0f,-70.0f,-50.0f,-30.0f,-10.0f }
        };

        float[][] g_InitValues = {
                new float[] { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f },
                new float[] { 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }
                };

        [TestMethod()]
        public void SpectrumData_CreateWithValues()
        {
            float[][] initialValues = {
                new float[] { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f },
                new float[] { 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }
                };
            var data = SpectrumData.Create(initialValues, ScaleType.Linear, ScaleType.Linear, 0, 20000);
            CollectionAssert.AreEqual(initialValues[0], data[0].ToArray());
            CollectionAssert.AreEqual(initialValues[1], data[1].ToArray());
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(2, data.Count());
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            Assert.AreEqual(5u, data.FrequencyCount);
            Assert.AreEqual(ScaleType.Linear, data.FrequencyScale);
            Assert.AreEqual(20000 / 5, data.FrequencyStep);
            Assert.AreEqual(0, data.MinFrequency);
            Assert.AreEqual(20000, data.MaxFrequency);
            float[][] logValues =
            {
                new float[] { -100.0f,-80.0f,-60.0f,-40.0f,-20.0f },
                new float[] { -90.0f,-70.0f,-50.0f,-30.0f,-10.0f }
            };
            var logData = SpectrumData.Create(logValues, ScaleType.Logarithmic, ScaleType.Logarithmic, 20, 20000);
            CollectionAssert.AreEqual(logValues[0], logData[0].ToArray());
            CollectionAssert.AreEqual(logValues[1], logData[1].ToArray());
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(2, data.Count());
            Assert.AreEqual(ScaleType.Logarithmic, logData.AmplitudeScale);
            Assert.AreEqual(5u, data.FrequencyCount);
            Assert.AreEqual(ScaleType.Logarithmic, logData.FrequencyScale);
            Assert.AreEqual((float)Math.Pow(1000, 1.0 / 5.0), logData.FrequencyStep);
            Assert.AreEqual(20, logData.MinFrequency);
            Assert.AreEqual(20000, logData.MaxFrequency);
        }

        [TestMethod()]
        public void SpectrumData_CreateArgs_ValuesAreNull()
        {
            Assert.ThrowsException<NullReferenceException>(
                () =>
                {
                    var s = SpectrumData.Create(null, ScaleType.Linear, ScaleType.Linear, 0.0f, 20000f);
                }, "Null initializer");
        }

        [TestMethod()]
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
        public void SpectrumData_CreateArgs_MinF_GT_MaxF()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                var s = SpectrumData.Create(g_InitValues, ScaleType.Linear, ScaleType.Linear, 30000.0f, 20000f);
            }, "MinFrequency > MaxFrequency");
        }

        [TestMethod()]
        public void SpectrumData_CreateArgs_MinF_EQ_MaxF()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var s = SpectrumData.Create(g_InitValues, ScaleType.Linear, ScaleType.Linear, 20000f, 20000f);
                }, "MinFrequency == MaxFrequency");
        }

        [TestMethod()]
        public void SpectrumData_CreateArgs_MinF_Negative()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                var s = SpectrumData.Create(g_InitValues, ScaleType.Linear, ScaleType.Linear, -1.0f, 20000f);
            }, "MinFrequency < 0");
        }

        [TestMethod()]
        public void SpectrumData_CreateArgs_MinFZeroWhenFScaleLog()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                var s = SpectrumData.Create(g_InitValues, ScaleType.Linear, ScaleType.Logarithmic, 0.0f, 20000f);
            }, "MinFrequency == 0 while FrequencyScale == Logarithmic");
        }

        [TestMethod()]
        public void SpectrumData_LinearTransform()
        {
            SpectrumData data = SpectrumData.Create(
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

            // Case 1 exact division - bins are at 4000 -> mapping to 8000
            var s1 = data.LinearTransform(3, 0, 24000);
            Assert.AreEqual(2, s1.Count);
            Assert.AreEqual(3u, s1.FrequencyCount);
            Assert.AreEqual(ScaleType.Linear, s1.AmplitudeScale);
            Assert.AreEqual(ScaleType.Linear, s1.FrequencyScale);
            Assert.AreEqual(8000.0f, s1.FrequencyStep);
            Assert.AreEqual(0.0f, s1.MinFrequency);
            Assert.AreEqual(24000.0f, s1.MaxFrequency);
            CollectionAssert.AreEqual(new float[] { 4, 8, 2.5f }, s1[0].ToArray());
            CollectionAssert.AreEqual(new float[] { 40, 80, 25 }, s1[1].ToArray());
            // Case 2 bins are > 1 but fractional and shifted up by 1k
            var s2 = data.LinearTransform(4, 1000, 25000);
            Assert.AreEqual(4u, s2.FrequencyCount);
            Assert.AreEqual(6000.0f, s2.FrequencyStep);
            Assert.AreEqual(1000.0f, s2.MinFrequency);
            Assert.AreEqual(25000.0f, s2.MaxFrequency);
            CollectionAssert.AreEqual(new float[] { 3.0f, 5.25f, 5.8125f, 0.15625f }, s2[0].ToArray());
            CollectionAssert.AreEqual(new float[] { 30f, 52.5f, 58.125f, 1.5625f }, s2[1].ToArray());
        }

        [TestMethod()]
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

            Assert.ThrowsException<NullReferenceException>(() => { data.CombineChannels(null); }, "Null parameter");
            Assert.ThrowsException<ArgumentException>(() => { data.CombineChannels(new float[] { }); }, "Empty map");
            Assert.ThrowsException<ArgumentException>(() => { data.CombineChannels(new float[] { 0, 0, 0, 0 }); }, "Too few elements");
            Assert.ThrowsException<ArgumentException>(() =>
            {
                var logData = SpectrumData.CreateEmpty(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 20000);
                data.CombineChannels(new float[] { 0.5f, 0.5f });
            }, "Combine log amp data");

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

        public float BinFrequency(SpectrumData data, uint bin)
        {
            if (data.FrequencyScale == ScaleType.Linear)
            {
                return bin * data.FrequencyStep + data.MinFrequency;
            }
            else
            {
                return data.MinFrequency * (float)Math.Pow(data.FrequencyStep, bin);
            }
        }

    }
}
