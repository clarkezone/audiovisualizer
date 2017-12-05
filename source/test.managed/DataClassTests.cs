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
        public void ScalarData()
        {
            var data = new ScalarData(2);
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
        public void SpectrumData_ctor_zero()
        { 
            var data = new SpectrumData(2, 10,ScaleType.Linear,ScaleType.Linear,0,20000);
            Assert.AreEqual(2, data.Count());
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(20000.0f, data.MaxFrequency);
            Assert.AreEqual(0.0f, data.MinFrequency);
            Assert.AreEqual(2000.0f, data.FrequencyStep);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            foreach (var channelData in data)
            {
                foreach (var item in channelData)
                {
                    Assert.AreEqual(0.0f, item);
                }
            }
        }

        [TestMethod()]
        public void SpectrumData_ctor_values()
        {
            float[][] initialValues = {
                new float[] { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f },
                new float[] { 0.5f, 0.6f, 0.7f, 0.8f, 0.9f }
                };
            var data = new SpectrumData(initialValues, ScaleType.Linear, ScaleType.Logarithmic, 20, 20000);
            Assert.AreEqual(20, data.MinFrequency);
            Assert.AreEqual(20000, data.MaxFrequency);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            Assert.AreEqual(ScaleType.Logarithmic, data.FrequencyScale);
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(5U, data.FrequencyCount);
            Assert.AreEqual(3.9810717f, data.FrequencyStep);
            CollectionAssert.AreEqual(initialValues[0], data[0].ToArray());
            CollectionAssert.AreEqual(initialValues[1], data[1].ToArray());

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
