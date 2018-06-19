using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;
using System.ComponentModel;
using Windows.Foundation;
using System.Reflection;
using System.Runtime.InteropServices;

namespace AudioVisualizer.test
{



    [TestClass]
    public class SourceConverterPropertyTests
    {
        SourceConverter sut;
        FakeVisualizationSource testSource;
        List<string> propertiesChanged;
        TypeInfo typeInfo = (typeof(SourceConverter) as IReflectableType).GetTypeInfo();

        [TestInitialize]
        public void TestInitialize()
        {
            sut = new SourceConverter();
            testSource = new FakeVisualizationSource();
            sut.Source = testSource;
            propertiesChanged = new List<string>();

            sut.ConfigurationChanged += new TypedEventHandler<IVisualizationSource, string>(
                (sender, property) =>
                {
                    propertiesChanged.Add(property);
                }
                );
        }

        void TestProperty<T>(T testValue, string propertyName) where T : struct
        {
            var propertyInfo = typeInfo.GetDeclaredProperty(propertyName);
            Assert.IsNotNull(propertyInfo, $"Property \'{propertyName}\' not found");
            Assert.IsNull(propertyInfo.GetValue(sut));
            Assert.IsFalse(propertiesChanged.Any());    // No notifications should be there
            propertyInfo.SetValue(sut, testValue);
            var value = (T?)propertyInfo.GetValue(sut);
            Assert.IsTrue(value.HasValue);
            Assert.AreEqual(testValue, value.Value, $"Set/Get {propertyName}={testValue} failed");
            CollectionAssert.AreEqual(new string[] { propertyName }, propertiesChanged, $"Missing ConfigChanged notification for {propertyName}");
            propertyInfo.SetValue(sut, testValue);
            CollectionAssert.AreEqual(new string[] { propertyName }, propertiesChanged, $"Invalid ConfigChanged notification for setting same value on {propertyName}");
            propertyInfo.SetValue(sut, null);
            CollectionAssert.AreEqual(new string[] { propertyName, propertyName }, propertiesChanged, $"Missing ConfigChanged notification for {propertyName} = null");
            propertyInfo.SetValue(sut, null);
            CollectionAssert.AreEqual(new string[] { propertyName, propertyName }, propertiesChanged, $"Invalid ConfigChanged notification for setting same value on {propertyName} = null");
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_ChannelCount()
        {
            TestProperty(2u, "ChannelCount");
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_FrequencyCount()
        {
            TestProperty(20u, "FrequencyCount");
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_MinFrequency()
        {
            TestProperty(20.0f, "MinFrequency");
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_MaxFrequency()
        {
            TestProperty(20000.0f, "MaxFrequency");
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_FrequencyScale()
        {
            TestProperty(ScaleType.Linear, "FrequencyScale");
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_AnalyzerTypes()
        {
            sut.AnalyzerTypes = AnalyzerType.Spectrum;
            Assert.AreEqual(AnalyzerType.Spectrum, sut.AnalyzerTypes);
            CollectionAssert.AreEqual(new string[] { "AnalyzerTypes" }, propertiesChanged, "Change notification missing assigning AnalyzerTypes property");
            sut.AnalyzerTypes = AnalyzerType.Spectrum;
            CollectionAssert.AreEqual(new string[] { "AnalyzerTypes" }, propertiesChanged, "Invalid change notification when assigning same value to AnalyzerTypes");
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_Source()
        {
            FakeVisualizationSource fakeSource = new FakeVisualizationSource();
            sut.Source = fakeSource;
            Assert.AreEqual(fakeSource, sut.Source);
            CollectionAssert.AreEqual(new string[] { "Source" }, propertiesChanged, "Change notification missing assigning Source property");
            sut.Source = fakeSource;
            CollectionAssert.AreEqual(new string[] { "Source" }, propertiesChanged, "Invalid change notification when assigning same value to Source");
            sut.Source = null;
            CollectionAssert.AreEqual(new string[] { "Source", "Source" }, propertiesChanged, "Change notification missing assigning null to Source property");
            sut.Source = null;
            CollectionAssert.AreEqual(new string[] { "Source", "Source" }, propertiesChanged, "Invalid change notification when assigning same value (null) to Source");
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_Property_ChannelMap()
        {
            sut.ChannelCount = 2;
            propertiesChanged.Clear();  // Remove the channelcount notification
            float[] map1 = new float[] { 0.25f, 0.75f };
            sut.ChannelMapping = map1;
            CollectionAssert.AreEqual(map1, sut.ChannelMapping);
            CollectionAssert.AreEqual(new string[] { "ChannelMap" }, propertiesChanged, "Change notification missing assigning ChannelMap property");
            sut.ChannelMapping = map1;
            CollectionAssert.AreEqual(new string[] { "ChannelMap" }, propertiesChanged, "Invalid change notification when assigning same value to ChannelMap");
            sut.ChannelMapping = null;
            CollectionAssert.AreEqual(new string[] { "ChannelMap", "ChannelMap" }, propertiesChanged, "Change notification missing assigning null to ChannelMap property");
            sut.ChannelMapping = null;
            CollectionAssert.AreEqual(new string[] { "ChannelMap", "ChannelMap" }, propertiesChanged, "Invalid change notification when assigning same value (null) to ChannelMap");
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsFpsPassedThrough()
        {
            Assert.AreEqual(testSource.Fps, sut.Fps);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsPlaybackStatePassedThrough()
        {
            Assert.AreEqual(testSource.PlaybackState, sut.PlaybackState);
            testSource.State = SourcePlaybackState.Playing;
            Assert.AreEqual(testSource.State, sut.PlaybackState);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsSuspendedPassedThrough()
        {
            Assert.AreEqual(testSource.IsSuspended, sut.IsSuspended);
            testSource.IsSuspended = !testSource.IsSuspended;
            Assert.AreEqual(testSource.IsSuspended, sut.IsSuspended);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsPresentationTimePassedThrough()
        {
            Assert.AreEqual(testSource.PresentationTime, sut.PresentationTime);
            testSource.Time = TimeSpan.FromSeconds(10);
            Assert.AreEqual(testSource.PresentationTime, sut.PresentationTime);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualChannelCountPassedThrough()
        {
            Assert.AreEqual(testSource.ActualChannelCount, sut.ActualChannelCount);

        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualFrequencyCountPassedThrough()
        {
            Assert.AreEqual(testSource.ActualFrequencyCount, sut.ActualFrequencyCount);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualMinFrequencyPassedThrough()
        {
            Assert.AreEqual(testSource.ActualMinFrequency, sut.ActualMinFrequency);
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualMaxFrequencyPassedThrough()
        {
            Assert.AreEqual(testSource.ActualMaxFrequency, sut.ActualMaxFrequency);
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualFrequencyScalePassedThrough()
        {
            Assert.AreEqual(testSource.ActualFrequencyScale, sut.ActualFrequencyScale);
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingFrequencyCountToZeroThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => { sut.FrequencyCount = 0; });
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingChannelCountToZeroThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => { sut.ChannelCount = 0; });
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingChannelMapWithoutCountThrows()
        {
            Assert.ThrowsException<COMException>(() => { sut.ChannelMapping = new float[] { 0 }; });
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingChannelMapWithLessSizeThanCountThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                sut.ChannelCount = 2;
                sut.ChannelMapping = new float[] { 0 };
            });
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingMinFrequencyToZeroWithLogScaleThrows()
        {
            sut.FrequencyScale = ScaleType.Logarithmic;
            Assert.ThrowsException<ArgumentException>(() => { sut.MinFrequency = 0.0f; });
        }
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingMinFrequencyLTZeroThrows()
        {
            sut.FrequencyScale = ScaleType.Logarithmic;
            Assert.ThrowsException<ArgumentException>(() => { sut.MinFrequency = -1.0f; });
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingMaxFrequencyLTMinThrows()
        {
            sut.MinFrequency = 20.0f;
            Assert.ThrowsException<ArgumentException>(() => { sut.MaxFrequency = 10.0f; });
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_SettingMinFrequencyGTEMaxThrows()
        {
            sut.MaxFrequency = 20000.0f;
            Assert.ThrowsException<ArgumentException>(() => { sut.MinFrequency = 20000.0f; });
        }

 

    }

    [TestClass]
    public class SourceConverterInit
    {
        SourceConverter sut;
        [TestInitialize]
        public void TestInit()
        {
            sut = new SourceConverter();
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_SetFpsThrowsNotImplemented()
        {
            Assert.ThrowsException<NotImplementedException>(() =>
            {
                sut.Fps = 61.0f;
            });
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_GetFpsThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                var v = sut.Fps;
            });
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_SetIsSuspendedThrowsNotImplemented()
        {
            Assert.ThrowsException<NotImplementedException>(() =>
            {
                sut.IsSuspended = false;
            });
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_GetIsSuspendedThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                var v = sut.IsSuspended;
            });
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_PlaybackStateIsStopped()
        {
            Assert.AreEqual(SourcePlaybackState.Stopped,sut.PlaybackState);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_PresentationTimeIsNull()
        {
                Assert.IsNull(sut.PresentationTime);
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_AnalyzerTypesAreAll()
        {
            Assert.AreEqual(AnalyzerType.All, sut.AnalyzerTypes);
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_SourceIsNull()
        {
            Assert.IsNull(sut.Source);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_FrequencyCountIsNull()
        {
            Assert.IsNull(sut.FrequencyCount);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_ChannelCountIsNull()
        {
            Assert.IsNull(sut.ChannelCount);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_RmsRiseTimeIsNull()
        {
            Assert.IsNull(sut.RmsRiseTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_RmsFallTimeIsNull()
        {
            Assert.IsNull(sut.RmsFallTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_PeakRiseTimeIsNull()
        {
            Assert.IsNull(sut.PeakRiseTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_PeakFallTimeIsNull()
        {
            Assert.IsNull(sut.PeakFallTime);
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_SpectrumRiseTimeIsNull()
        {
            Assert.IsNull(sut.SpectrumRiseTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_SpectrumFallTimeIsNull()
        {
            Assert.IsNull(sut.SpectrumFallTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_MinFrequencyIsNull()
        {
            Assert.IsNull(sut.MinFrequency);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_MaxFrequencyIsNull()
        {
            Assert.IsNull(sut.MaxFrequency);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AfterInit_FrequencyScaleIsNull()
        {
            Assert.IsNull(sut.FrequencyScale);
        }
    }

    [TestClass]
    public class SourceConverterOperation
    {
        SourceConverter sut;
        FakeVisualizationSource testSource;
        VisualizationDataFrame testFrame;

        UInt32 expectedChannelCount = 5;
        UInt32 expectedFrequencyCount = 100;
        float expectedMinFrequency = 200.0f;
        float expectedMaxFrequency = 20000.0f;
        TimeSpan expectedTime = TimeSpan.FromSeconds(1);
        TimeSpan expectedDuration = TimeSpan.FromMilliseconds(16.7);

        [TestInitialize]
        public void TestInit()
        {
            sut = new SourceConverter();
            testSource = new FakeVisualizationSource();
            sut.Source = testSource;

            testFrame = new VisualizationDataFrame(
                expectedTime,
                expectedDuration,
                ScalarData.CreateEmpty(expectedChannelCount),
                ScalarData.CreateEmpty(expectedChannelCount),
                SpectrumData.CreateEmpty(expectedChannelCount, expectedFrequencyCount, ScaleType.Linear, ScaleType.Linear, expectedMinFrequency, expectedMaxFrequency)
                );

            testSource.Frame = testFrame;
        }

        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_GetData_Passthrough()
        {
            var frame = sut.GetData();
            Assert.AreEqual(testFrame, frame);  // Needs to be the same frame if no conversion parameters are set
        }
        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_GetData_AnalyzerTypeEqRms()
        {
            sut.AnalyzerTypes = AnalyzerType.RMS;
            var frame = sut.GetData();
            Assert.AreEqual(testFrame.RMS, frame.RMS);
            Assert.IsNull(frame.Peak);
            Assert.IsNull(frame.Spectrum);
        }
        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_GetData_AnalyzerTypeEqPeak()
        {
            sut.AnalyzerTypes = AnalyzerType.Peak;
            var frame = sut.GetData();
            Assert.IsNull(frame.RMS);
            Assert.AreEqual(testFrame.Peak, frame.Peak);
            Assert.IsNull(frame.Spectrum);
        }
        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_GetData_AnalyzerTypeEqSpectrum()
        {
            sut.AnalyzerTypes = AnalyzerType.Spectrum;
            var frame = sut.GetData();
            Assert.IsNull(frame.RMS);
            Assert.IsNull(frame.Peak);
            Assert.AreEqual(testFrame.Spectrum, frame.Spectrum);
        }

        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_RiseAndFall()
        {
            var spectralData = Enumerable.Repeat<float>(1.0f, (int)expectedFrequencyCount);
            var nextFrame = new VisualizationDataFrame(
                testFrame.Time.Add(testFrame.Duration),
                testFrame.Duration,
                ScalarData.Create(Enumerable.Repeat<float>(1.0f, (int)expectedChannelCount).ToArray()),
                ScalarData.Create(Enumerable.Repeat<float>(2.0f, (int)expectedChannelCount).ToArray()),
                SpectrumData.Create( 
                    Enumerable.Repeat(spectralData.ToArray(),(int)expectedChannelCount).ToArray(), ScaleType.Linear, ScaleType.Linear, expectedMinFrequency, expectedMaxFrequency)
                    );

            sut.SpectrumRiseTime = TimeSpan.FromMilliseconds(100);
            sut.SpectrumFallTime = TimeSpan.FromMilliseconds(50);
            sut.RmsRiseTime = TimeSpan.FromMilliseconds(80);
            sut.RmsFallTime = TimeSpan.FromMilliseconds(40);
            sut.PeakRiseTime = TimeSpan.FromMilliseconds(20);
            sut.PeakFallTime = TimeSpan.FromMilliseconds(200);

            var data = sut.GetData();
            testSource.Frame = nextFrame;
            data = sut.GetData();
            Assert.IsNotNull(data.RMS);
            Assert.IsNotNull(data.Peak);
            Assert.IsNotNull(data.Spectrum);
        }
    }

    [TestClass]
    public class SourceConverter_CombineChannels
    {
        SourceConverter sut;
        FakeVisualizationSource testSource;
        VisualizationDataFrame testFrame;

        UInt32 expectedChannelCount = 5;

        [TestInitialize]
        public void TestInit()
        {
            sut = new SourceConverter();
            testSource = new FakeVisualizationSource();
            testSource.ExpectedChannels = expectedChannelCount;
            sut.Source = testSource;

            testFrame = new VisualizationDataFrame(
                TimeSpan.Zero,
                TimeSpan.FromMilliseconds(16.7),
                ScalarData.CreateEmpty(expectedChannelCount),
                ScalarData.CreateEmpty(expectedChannelCount),
                SpectrumData.CreateEmpty(expectedChannelCount, 10, ScaleType.Linear, ScaleType.Linear, 0, 10000));
                

            testSource.Frame = testFrame;
        }
        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_CombineChannels_IsRMSChannelCountCorrect()
        {
            sut.ChannelCount = 2;
            var frame = sut.GetData();
            Assert.AreEqual(2, frame.RMS.Count());
        }
        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_CombineChannels_IsPeakChannelCountCorrect()
        {
            sut.ChannelCount = 2;
            var frame = sut.GetData();
            Assert.AreEqual(2, frame.Peak.Count());
        }
        [TestCategory("SourceConverter")]
        [TestMethod]
        public void SourceConverter_CombineChannels_IsSpectrumChannelCountCorrect()
        {
            sut.ChannelCount = 2;
            var frame = sut.GetData();
            Assert.AreEqual(2, frame.Spectrum.Count());
        }

        [TestCategory("SourceConverter")]
        [DataTestMethod]
        [DataRow(2u, 1u, new float[] { 3.0f })]
        [DataRow(5u, 1u, new float[] { 31.0f })]
        [DataRow(1u, 2u, new float[] { 1.0f,1.0f })]
        [DataRow(2u, 2u, new float[] { 1.0f, 2.0f })]
        [DataRow(3u, 2u, new float[] { 1.0f, 2.0f })]
        [DataRow(4u, 2u, new float[] { 5.0f, 10.0f })]
        [DataRow(5u, 2u, new float[] { 9.0f, 18.0f })]
        [DataRow(6u, 2u, new float[] { 17.0f, 34.0f })]
        [DataRow(3u, 5u, new float[] { 1.0f, 2.0f, 4.0f, 0.0f, 0.0f })]
        [DataRow(5u, 3u, new float[] { 1.0f, 2.0f, 4.0f })]
        public void SourceConverter_CombineChannels_WoMap(uint inputChannels,uint outChannels,float [] expectedRms)
        {
            float[] input = new float[inputChannels];
            float value = 1.0f;
            for (int index = 0; index < inputChannels; index++,value *= 2.0f)
            {
                input[index] = value;
            }

            var inputFrame = new VisualizationDataFrame(TimeSpan.Zero, TimeSpan.FromMilliseconds(16), ScalarData.Create(input), null, null);
            testSource.ExpectedChannels = inputChannels;
            testSource.Frame = inputFrame;
            sut.ChannelCount = outChannels;

            var outFrame = sut.GetData();
                     
            CollectionAssert.AreEqual(expectedRms, outFrame.RMS.ToArray());
        }
        [TestCategory("SourceConverter")]
        [DataTestMethod]
        [DataRow(3u, 2u, new float[] { 0.1f,0.2f },new float[] { 0.5f,0.0f })]
        public void SourceConverter_CombineChannels_WithMap(uint inputChannels, uint outChannels, float [] map, float[] expectedRms)
        {
            float[] input = new float[inputChannels];
            float value = 1.0f;
            for (int index = 0; index < inputChannels; index++, value *= 2.0f)
            {
                input[index] = value;
            }

            var inputFrame = new VisualizationDataFrame(TimeSpan.Zero, TimeSpan.FromMilliseconds(16), ScalarData.Create(input), null, null);
            testSource.ExpectedChannels = inputChannels;
            testSource.Frame = inputFrame;
            sut.ChannelCount = outChannels;
            sut.ChannelMapping = map;

            var outFrame = sut.GetData();

            CollectionAssert.AreEqual(expectedRms, outFrame.RMS.ToArray());
        }
    }
}
