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
    public class ScalarDataTests
    {
        [TestMethod]
        [TestCategory("ScalarData")]
        public void ScalarData_CreateEmpty()
        {
            var data = ScalarData.CreateEmpty(2);
            Assert.AreEqual(2, data.Count());
            Assert.AreEqual(2, data.Count);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            Assert.AreEqual(0.0f, data[0]);
            Assert.AreEqual(0.0f, data[1]);
            CollectionAssert.AreEqual(new float[] { 0.0f, 0.0f }, data.AsEnumerable().ToArray());
        }

        [TestMethod]
        [TestCategory("ScalarData")]
        public void ScalarData_CreateWithValues()
        {
            var initData = new float[] { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f };
            var data = ScalarData.Create(initData);
            Assert.AreEqual(5, data.Count());
            Assert.AreEqual(5, data.Count);
            Assert.AreEqual(ScaleType.Linear, data.AmplitudeScale);
            for (int i = 0; i < 5; i++)
            {
                Assert.AreEqual(initData[i], data[i]);
            }
        }

        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_ConvertToDecibels()
        {
            float[] testValues = new float[] { 0.0f, 0.1f, 1.0f, 1e-6f, 1e6f, -1 };
            var data = ScalarData.Create(testValues);
            var logData = data.ConvertToDecibels(-100, 0);
            CollectionAssert.AreEqual(new float[] { -100.0f, -20.0f, 0.0f, -100.0f, 0.0f, -100.0f }, logData.ToArray());
            Assert.ThrowsException<COMException>(() => { var d2 = logData.ConvertToDecibels(-100, 0); });
            Assert.ThrowsException<ArgumentException>(() => { var d3 = data.ConvertToDecibels(0, 0); });
        }

        // Auxiliary function to calculate correct ris and fall time
        // Result[n] = Result[n-1] + (Result[n] - Result[n-1]) * (1 - exp(-t))
        // where t = Result[n] - Result[n-1] > 0 ? Time / riseTime : Time / fallTime
        // If time == 0 then copy
        float RiseFallTime(float value,float previous,float riseTime,float fallTime)
        {
            return previous + (value - previous) * (1 - (float) Math.Exp(-1.0f/(value > previous ? riseTime : fallTime)));
        }

        void AssertFloatCollectionsAreEqual(ICollection<float> expected,ICollection<float> actual,float delta,string message="")
        {
            Assert.IsNotNull(expected);
            Assert.IsNotNull(actual);
            Assert.AreEqual(expected.Count, actual.Count);
            for (int index = 0; index < expected.Count; index++)
            {
                Assert.AreEqual(expected.ElementAt(index), actual.ElementAt(index), delta, $"{message} 'Elements at {index} not equal ({expected.ElementAt(index)} != {actual.ElementAt(index)})'");
            }
        }

        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFall()
        {
            var previous = ScalarData.Create(new float[] { 2.0f, 1.0f, 1.5f });
            var data = ScalarData.Create(new float[] { 1.0f, 2.0f, 1.5f }); // First falling, second rising, 3rd same
            var result1 = data.ApplyRiseAndFall(previous,
                TimeSpan.FromMilliseconds(100),
                TimeSpan.FromMilliseconds(200),
                TimeSpan.FromMilliseconds(400)
                );
            var expectedResult1 = new float[] {
                RiseFallTime(data[0], previous[0], 0.25f, 0.5f),
                RiseFallTime(data[1], previous[1], 0.25f, 0.5f),
                RiseFallTime(data[2], previous[2], 0.25f, 0.5f) };
            AssertFloatCollectionsAreEqual(
                expectedResult1,
                result1.ToArray(),0.001f,"Calculation results incorrect"
                );
        }
        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFall_ZeroRiseAndFall()
        {
            var previous = ScalarData.Create(new float[] { 2.0f, 1.0f, 1.5f });
            var data = ScalarData.Create(new float[] { 1.0f, 2.0f, 1.5f }); // First falling, second rising, 3rd same
            var result1 = data.ApplyRiseAndFall(previous,
                TimeSpan.Zero,
                TimeSpan.Zero,
                TimeSpan.FromMilliseconds(400)
                );

            AssertFloatCollectionsAreEqual(
                data.ToArray(),
                result1.ToArray(), 0.001f, "Calculation results incorrect"
                );
        }
        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFall_ZeroTime()
        {
            var previous = ScalarData.Create(new float[] { 2.0f, 1.0f, 1.5f });
            var data = ScalarData.Create(new float[] { 1.0f, 2.0f, 1.5f }); // First falling, second rising, 3rd same
            var result1 = data.ApplyRiseAndFall(previous,
                TimeSpan.FromMilliseconds(100),
                TimeSpan.FromMilliseconds(200),
                TimeSpan.Zero
                );
            var expectedResult1 = new float[] {
                RiseFallTime(data[0], previous[0], 0.25f, 0.5f),
                RiseFallTime(data[1], previous[1], 0.25f, 0.5f),
                RiseFallTime(data[2], previous[2], 0.25f, 0.5f) };
            AssertFloatCollectionsAreEqual(
                previous.ToArray(),
                result1.ToArray(), 0.001f, "Calculation results incorrect"
                );
        }

        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFall_LogScaleArgThrows()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                ScalarData.CreateEmpty(2).ConvertToDecibels(-100, 20).ApplyRiseAndFall(null, TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1));
            }
            );
        }
        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFall_DifferentSizeThrows()
        {
            Assert.ThrowsException<ArgumentException>(
            () =>
            {
                ScalarData.CreateEmpty(2).ApplyRiseAndFall(ScalarData.CreateEmpty(1), TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(1));
            }
            );
        }

        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFall_PreviousEmpty()
        {
            var data = ScalarData.Create(new float[] { 1.0f, -1.0f, 0.0f }); // First falling, second rising, 3rd same

            var result = data.ApplyRiseAndFall(null,
                TimeSpan.FromMilliseconds(100),
                TimeSpan.FromMilliseconds(200),
                TimeSpan.FromMilliseconds(400));
            var expectedResult = new float[] {
                RiseFallTime(data[0], 0, 0.25f, 0.5f),
                RiseFallTime(data[1], 0, 0.25f, 0.5f),
                RiseFallTime(data[2], 0, 0.25f, 0.5f) };

            AssertFloatCollectionsAreEqual(
                expectedResult,
                result.ToArray(), 0.001f, "Calculation results incorrect"
                );
        }


        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFallToEmpty()
        {
            var data = ScalarData.Create(new float[] { 1.0f, 0.0f, -1.0f });
            var result = ScalarData.ApplyRiseAndFallToEmpty(data, TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(4));
            var expectedResult = new float[] {
                RiseFallTime(0,data[0], 0.25f, 0.5f),
                RiseFallTime(0,data[1], 0.25f, 0.5f),
                RiseFallTime(0,data[2], 0.25f, 0.5f)
            };
            AssertFloatCollectionsAreEqual(
                expectedResult,
                result.ToArray(), 0.001f, "Calculation results incorrect"
            );
        }
        [TestMethod()]
        [TestCategory("ScalarData")]
        public void ScalarData_RiseAndFallToEmpty_NullPreviousThrows()
        {
            Assert.ThrowsException<ArgumentException>(
                () =>
                {
                    var result = ScalarData.ApplyRiseAndFallToEmpty(null, TimeSpan.FromSeconds(1), TimeSpan.FromSeconds(2), TimeSpan.FromSeconds(4));
                }
                );
        }
    }
}
