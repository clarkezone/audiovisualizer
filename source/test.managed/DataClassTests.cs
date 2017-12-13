using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using AudioVisualizer;

namespace test.managed
{
    [TestClass]
    public class DataClassTests
    {
        [TestMethod]
        public void ScalarData_CreateEmpty()
        {
            var data = ScalarData.CreateEmpty(2);
            Assert.AreEqual(2, data.Count());
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            Assert.AreEqual(0.0f,data[0]);
            Assert.AreEqual(0.0f, data[1]);
            CollectionAssert.AreEqual(new float[] { 0.0f, 0.0f }, data.AsEnumerable().ToArray());

            var logData = data.ConvertToLogAmplitude(-50, 0);
            Assert.AreEqual(2, logData.Count());
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(ScaleType.Logarithmic, logData.AmplitudeScale);
            Assert.AreEqual(-50.0f, logData[0]);
            Assert.AreEqual(-50.0f, logData[1]);
            CollectionAssert.AreEqual(new float[] { -50.0f, -50.0f }, logData.AsEnumerable().ToArray());
        }

        
        [TestMethod()]
        public void SpectrumData_CreateEmpty()
        { 
            var data = new SpectrumData(2, 10,ScaleType.Linear,ScaleType.Linear,0,20000);
            foreach (var channelData in data)
            {
                foreach (var item in channelData)
                {
                    Assert.AreEqual(0.0f, item,"SpectrumData element == 0");
                }
            }
        }
        [TestMethod()]
        public void SpectrumData_ctor_properties()
        {
            var data = new SpectrumData(2, 10, ScaleType.Linear, ScaleType.Linear, 0, 20000);
            Assert.AreEqual(2, data.Count());
            Assert.AreEqual(2, data.Count, "Channel count property init");
            foreach (var item in data)
            {
                Assert.AreEqual(10, item.Count);
            }
            Assert.AreEqual(10u, data.FrequencyCount,"Frequency bin count");
            Assert.AreEqual(20000.0f, data.MaxFrequency, "Max Frequency property init");
            Assert.AreEqual(0.0f, data.MinFrequency, "Min Frequency property init");
            Assert.AreEqual(2000.0f, data.FrequencyStep);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);

            data = new SpectrumData(2,10, ScaleType.Linear, ScaleType.Logarithmic, 20, 20000);
            Assert.AreEqual(20, data.MinFrequency);
            Assert.AreEqual(20000, data.MaxFrequency);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            Assert.AreEqual(ScaleType.Logarithmic, data.FrequencyScale);
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(10u, data.FrequencyCount);
            Assert.AreEqual(1.995262f, data.FrequencyStep,1e-6f);
        }
        public void SpectrumData_ctor_validation()
        {
            Assert.ThrowsException<ArgumentException>(
                ()=> 
                {
                    var data = new SpectrumData(0, 10, ScaleType.Linear, ScaleType.Linear, 0.0f, 20000f);
                },"Zero channels value");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new SpectrumData(2, 0, ScaleType.Linear, ScaleType.Linear, 0.0f, 20000f);
                },"Zero elements value");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new SpectrumData(0, 10, ScaleType.Linear, ScaleType.Linear, 30000.0f, 20000f);
                },"MinFrequency > MaxFrequency");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new SpectrumData(0, 10, ScaleType.Linear, ScaleType.Linear, 20000f, 20000f);
                }, "MinFrequency == MaxFrequency");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new SpectrumData(0, 10, ScaleType.Linear, ScaleType.Linear, -1.0f, 20000f);
                }, "MinFrequency < 0");
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var data = new SpectrumData(0, 10, ScaleType.Linear, ScaleType.Logarithmic, 0.0f, 20000f);
                }, "MinFrequency == 0 while FrequencyScale == Logarithmic");
        }

        [TestMethod()]
        public void SpectrumData_ctor_values()
        {
            float[][] initialValues = {
                new float[] { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f },
                new float[] { 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }
                };
            var data = new SpectrumData(initialValues, ScaleType.Linear, ScaleType.Logarithmic, 20, 20000);
            CollectionAssert.AreEqual(initialValues[0], data[0].ToArray());
            CollectionAssert.AreEqual(initialValues[1], data[1].ToArray());
        }

        [TestMethod()]
        public void SpectrumData_TranformToLogFrequency()
        {
            SpectrumData data = new SpectrumData(
                new float[][]
                {
                    new float[] { 0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f,0.8f,0.9f,1.0f }
                },
                ScaleType.Linear,
                ScaleType.Linear,
                0f, 1000f);
            var log = data.TransformLogFrequency(3, 1, 1000);
            Assert.AreEqual(1, log.Count);
            Assert.AreEqual(3u, log.FrequencyCount);
            Assert.AreEqual(1, log.MinFrequency);
            Assert.AreEqual(1000, log.MaxFrequency);

            var values = log[0].ToArray();
            int i = 3;
        }

        [TestMethod()]
        public void SpectrumData_CombineChannels()
        {
            SpectrumData data = new SpectrumData(
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

            Assert.ThrowsException<NullReferenceException>(() => { data.CombineChannels(null); },"Null parameter");
            Assert.ThrowsException<ArgumentException>(() => { data.CombineChannels(new float[] { }); }, "Empty map");
            Assert.ThrowsException<ArgumentException>(() => { data.CombineChannels(new float[] { 0,0,0,0 }); }, "Too few elements");
            Assert.ThrowsException<ArgumentException>(() => 
            {
                var logData = new SpectrumData(2, 10, ScaleType.Logarithmic, ScaleType.Linear, 0, 20000);
                data.CombineChannels(new float[] { 0.5f, 0.5f });
            },"Combine log amp data");

            var spectrum = data.CombineChannels(new float[] { 0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f,0.8f,0.9f,1.0f });
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
                return data.MinFrequency * (float) Math.Pow(data.FrequencyStep, bin);
            }
        }
    }
}
