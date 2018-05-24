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
    class FakeVisualizationSource : IVisualizationSource
    {
        public VisualizationDataFrame Frame;  
        public VisualizationDataFrame GetData()
        {
            return Frame;
        }
        AnalyzerType _types = AnalyzerType.All;
        float _fps = 60.0f;
        bool _bIsSuspended = false;

        public SourcePlaybackState State = SourcePlaybackState.Stopped;
        public TimeSpan? Time = null;

        public AnalyzerType AnalyzerTypes { get => _types; set => _types = value; }
        public float Fps { get => _fps; set => _fps = value; }
        public bool IsSuspended { get => _bIsSuspended; set => _bIsSuspended = value; }

        public SourcePlaybackState PlaybackState => State;

        public TimeSpan? PresentationTime => Time;

        public event TypedEventHandler<IVisualizationSource, string> ConfigurationChanged;

        public uint? ActualChannelCount => 2;

        public uint? ActualFrequencyCount => 50;

        public float? ActualMaxFrequency => 20000.0f;

        public float? ActualMinFrequency => 0.0f;

        public ScaleType? ActualFrequencyScale => ScaleType.Linear;
    }

    [TestClass]
    public class SourceConverterPropertyTests
    {
        SourceConverter sut;
        FakeVisualizationSource source;
        List<string> propertiesChanged;
        TypeInfo typeInfo = (typeof(SourceConverter) as IReflectableType).GetTypeInfo();


        [TestInitialize]
        public void TestInitialize()
        {
            sut = new SourceConverter();
            source = new FakeVisualizationSource();
            sut.Source = source;

            propertiesChanged = new List<string>();

            sut.ConfigurationChanged += new TypedEventHandler<IVisualizationSource, string>(
                (sender, property) =>
                {
                    propertiesChanged.Add(property);
                }
                );
        }

        void TestProperty<T>(T testValue,string propertyName) where T : struct
        {
           var propertyInfo = typeInfo.GetDeclaredProperty(propertyName);
            Assert.IsNotNull(propertyInfo, $"Property \'{propertyName}\' not found");
            Assert.IsNull(propertyInfo.GetValue(sut));
            Assert.IsFalse(propertiesChanged.Any());    // No notifications should be there
            propertyInfo.SetValue(sut, testValue);
            var value = (T?)propertyInfo.GetValue(sut);
            Assert.IsTrue(value.HasValue);
            Assert.AreEqual(testValue, value.Value,$"Set/Get {propertyName}={testValue} failed");
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
            CollectionAssert.AreEqual(new string[] { "Source" }, propertiesChanged,"Change notification missing assigning Source property");
            sut.Source = fakeSource;
            CollectionAssert.AreEqual(new string[] { "Source" }, propertiesChanged,"Invalid change notification when assigning same value to Source");
            sut.Source = null;
            CollectionAssert.AreEqual(new string[] { "Source","Source" }, propertiesChanged, "Change notification missing assigning null to Source property");
            sut.Source = null;
            CollectionAssert.AreEqual(new string[] { "Source", "Source" }, propertiesChanged, "Invalid change notification when assigning same value (null) to Source");
        }


    }

    [TestClass]
    public class SourceConverterPropertyValidationWithSourceSet
    {
        SourceConverter sut;
        FakeVisualizationSource _source;
        [TestInitialize]
        public void TestInit()
        {
            sut = new SourceConverter();
            _source = new FakeVisualizationSource();
            sut.Source = _source;
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsFpsPassedThrough()
        {
            Assert.AreEqual(_source.Fps, sut.Fps);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsPlaybackStatePassedThrough()
        {
            Assert.AreEqual(_source.PlaybackState, sut.PlaybackState);
            _source.State = SourcePlaybackState.Playing;
            Assert.AreEqual(_source.State, sut.PlaybackState);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsSuspendedPassedThrough()
        {
            Assert.AreEqual(_source.IsSuspended, sut.IsSuspended);
            _source.IsSuspended = !_source.IsSuspended;
            Assert.AreEqual(_source.IsSuspended, sut.IsSuspended);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsPresentationTimePassedThrough()
        {
            Assert.AreEqual(_source.PresentationTime, sut.PresentationTime);
            _source.Time = TimeSpan.FromSeconds(10);
            Assert.AreEqual(_source.PresentationTime, sut.PresentationTime);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualChannelCountPassedThrough()
        {
            Assert.AreEqual(_source.ActualChannelCount, sut.ActualChannelCount);
            
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualFrequencyCountPassedThrough()
        {
            Assert.AreEqual(_source.ActualFrequencyCount, sut.ActualFrequencyCount);
        }
        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualMinFrequencyPassedThrough()
        {
            Assert.AreEqual(_source.ActualMinFrequency, sut.ActualMinFrequency);
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualMaxFrequencyPassedThrough()
        {
            Assert.AreEqual(_source.ActualMaxFrequency, sut.ActualMaxFrequency);
        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        public void SourceConverter_IsActualFrequencyScalePassedThrough()
        {
            Assert.AreEqual(_source.ActualFrequencyScale, sut.ActualFrequencyScale);
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

        [TestMethod]
        [TestCategory("SourceConverter")]
        void SourceConverter_GetData()
        {

            var frame = new VisualizationDataFrame(
                TimeSpan.FromSeconds(1),
                TimeSpan.FromTicks(166667),
                ScalarData.CreateEmpty(2),
                ScalarData.CreateEmpty(2),
                SpectrumData.CreateEmpty(2, 1024, ScaleType.Linear, ScaleType.Linear, 0.0f, 22100.0f)
                );
            _source.Frame = frame;

            // First step passthrough
            var f = sut.GetData();
            Assert.IsNotNull(f);
            Assert.AreEqual(2, f.Spectrum.Count);
            Assert.AreEqual(1024u, f.Spectrum.FrequencyCount);

            _source.Frame = null;
            f = sut.GetData();
            Assert.IsNotNull(f);

        }

        [TestMethod]
        [TestCategory("SourceConverter")]
        void SourceConverter_RiseAndFall()
        {
            sut.Source = _source;

            var frame1 = new VisualizationDataFrame(
                TimeSpan.FromSeconds(1),
                TimeSpan.FromTicks(166667),
                ScalarData.CreateEmpty(2),
                ScalarData.CreateEmpty(2),
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0.0f, 22100.0f)
                );

            var frame2 = new VisualizationDataFrame(
                TimeSpan.FromSeconds(2),
                TimeSpan.FromTicks(166667),
                ScalarData.Create(new float[] { 0.5f, 1.0f }),
                ScalarData.Create(new float[] { 1.0f, 0.5f }),
                SpectrumData.Create(new float[][]
                {
                    new float[] { 1.0f,0,0,0,0,0,0,0,0,0 },
                    new float[] { 0.5f,0,0,0,0,0,0,0,0,0 }
                }, ScaleType.Linear, ScaleType.Linear, 0.0f, 22100.0f)
                );

            _source.Frame = frame1;
            sut.SpectrumRiseTime = TimeSpan.FromMilliseconds(100);
            sut.SpectrumFallTime = TimeSpan.FromMilliseconds(50);
            sut.RmsRiseTime = TimeSpan.FromMilliseconds(80);
            sut.RmsFallTime = TimeSpan.FromMilliseconds(40);
            sut.PeakRiseTime = TimeSpan.FromMilliseconds(20);
            sut.PeakFallTime = TimeSpan.FromMilliseconds(200);
            var data1 = sut.GetData();
            _source.Frame = frame2;
            var data2 = sut.GetData();
            Assert.IsNotNull(data2.RMS);
            Assert.IsNotNull(data2.Peak);
            Assert.IsNotNull(data2.Spectrum);

            CollectionAssert.AreEqual(new float[] { 0.09403199f, 0.188063979f }, data2.RMS.ToArray());
            CollectionAssert.AreEqual(new float[] { 0.5654025f, 0.282701254f }, data2.Peak.ToArray());
            CollectionAssert.AreEqual(new float[] { 0.1535185f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, data2.Spectrum[0].ToArray());
            CollectionAssert.AreEqual(new float[] { 0.07675925f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, data2.Spectrum[1].ToArray());
        }

    }

    [TestClass]
    public class SourceConverterPropertyValidationAfterInit
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
        public void SourceConverter_GetPlaybackStateThrowsNotImplemented()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                var v = sut.PlaybackState;
            });
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_GetPlaybackStateThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                var v = sut.PlaybackState;
            });
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_GetPresentationTimeThrows()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                var v = sut.PresentationTime;
            });
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_AnalyzerTypesAreAll()
        {
            Assert.AreEqual(AnalyzerType.All, sut.AnalyzerTypes);
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_SourceIsNull()
        {
            Assert.IsNull(sut.Source);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_FrequencyCountIsNull()
        {
            Assert.IsNull(sut.FrequencyCount);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_ChannelCountIsNull()
        {
            Assert.IsNull(sut.ChannelCount);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_RmsRiseTimeIsNull()
        {
            Assert.IsNull(sut.RmsRiseTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_RmsFallTimeIsNull()
        {
            Assert.IsNull(sut.RmsFallTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_PeakRiseTimeIsNull()
        {
            Assert.IsNull(sut.PeakRiseTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_PeakFallTimeIsNull()
        {
            Assert.IsNull(sut.PeakFallTime);
        }

        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_SpectrumRiseTimeIsNull()
        {
            Assert.IsNull(sut.SpectrumRiseTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_SpectrumFallTimeIsNull()
        {
            Assert.IsNull(sut.SpectrumFallTime);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_MinFrequencyIsNull()
        {
            Assert.IsNull(sut.MinFrequency);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_MaxFrequencyIsNull()
        {
            Assert.IsNull(sut.MaxFrequency);
        }
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_FrequencyScaleIsNull()
        {
            Assert.IsNull(sut.FrequencyScale);
        }


        public void SourceConverter_props()
        {
            var converter = new SourceConverter();

            List<string> propertiesChanged = new List<string>();

            converter.ConfigurationChanged += new TypedEventHandler<IVisualizationSource, string>(
                (sender,property) =>
                {
                    propertiesChanged.Add(property);
                }
                );

            var source = new FakeVisualizationSource();
            converter.Source = source;
            Assert.AreEqual(source, converter.Source);
            Assert.AreEqual(source.Fps, converter.Fps);
            Assert.AreEqual(source.IsSuspended, converter.IsSuspended);
            Assert.AreEqual(source.PlaybackState, converter.PlaybackState);
            Assert.AreEqual(source.Time, converter.PresentationTime);

            Assert.AreEqual(2u, converter.ActualChannelCount);
            Assert.AreEqual(50u, converter.ActualFrequencyCount);
            Assert.AreEqual(0.0f, converter.ActualMinFrequency);
            Assert.AreEqual(20000.0f, converter.ActualMaxFrequency);
            Assert.AreEqual(ScaleType.Linear, converter.ActualFrequencyScale);

            source.AnalyzerTypes = AnalyzerType.RMS;
            source.IsSuspended = !source.IsSuspended;
            source.State = SourcePlaybackState.Playing;
            source.Time = TimeSpan.FromSeconds(10);

            Assert.AreEqual(source.AnalyzerTypes, AnalyzerType.RMS);
            Assert.AreEqual(source.IsSuspended, converter.IsSuspended);
            Assert.AreEqual(source.PlaybackState, converter.PlaybackState);
            Assert.AreEqual(source.Time, converter.PresentationTime);

            Assert.ThrowsException<ArgumentException>(() => { converter.FrequencyCount = 0; });
            converter.FrequencyCount = 10;
            Assert.IsNotNull(converter.FrequencyCount);
            Assert.AreEqual(10u, converter.FrequencyCount.Value);
            Assert.ThrowsException<ArgumentException>(() => { converter.ChannelCount = 0; });
            converter.ChannelCount = 2;
            Assert.IsNotNull(converter.FrequencyCount);
            Assert.AreEqual(2u, converter.ChannelCount.Value);
            Assert.ThrowsException<ArgumentException>(() => { converter.RmsRiseTime = TimeSpan.Zero; });
            converter.RmsRiseTime = TimeSpan.FromTicks(1);
            Assert.IsNotNull(converter.RmsRiseTime);
            Assert.AreEqual(TimeSpan.FromTicks(1), converter.RmsRiseTime);
            Assert.ThrowsException<ArgumentException>(() => { converter.RmsFallTime = TimeSpan.Zero; });
            converter.RmsFallTime = TimeSpan.FromTicks(1);
            Assert.IsNotNull(converter.RmsFallTime);
            Assert.AreEqual(TimeSpan.FromTicks(1), converter.RmsFallTime);

            Assert.ThrowsException<ArgumentException>(() => { converter.PeakRiseTime = TimeSpan.Zero; });
            converter.PeakRiseTime = TimeSpan.FromTicks(1);
            Assert.IsNotNull(converter.PeakRiseTime);
            Assert.AreEqual(TimeSpan.FromTicks(1), converter.PeakRiseTime);
            Assert.ThrowsException<ArgumentException>(() => { converter.PeakFallTime = TimeSpan.Zero; });
            converter.PeakFallTime = TimeSpan.FromTicks(1);
            Assert.IsNotNull(converter.PeakFallTime);
            Assert.AreEqual(TimeSpan.FromTicks(1), converter.PeakFallTime);

            Assert.ThrowsException<ArgumentException>(() => { converter.SpectrumRiseTime = TimeSpan.Zero; });
            converter.SpectrumRiseTime = TimeSpan.FromTicks(1);
            Assert.IsNotNull(converter.SpectrumRiseTime);
            Assert.AreEqual(TimeSpan.FromTicks(1), converter.SpectrumRiseTime);
            Assert.ThrowsException<ArgumentException>(() => { converter.SpectrumFallTime = TimeSpan.Zero; });
            converter.SpectrumFallTime = TimeSpan.FromTicks(1);
            Assert.IsNotNull(converter.SpectrumFallTime);
            Assert.AreEqual(TimeSpan.FromTicks(1), converter.SpectrumFallTime);



            converter.MinFrequency = 20.0f;
            converter.MaxFrequency = 20000.0f;
            converter.FrequencyScale = ScaleType.Logarithmic;

            Assert.AreEqual(converter.FrequencyScale, ScaleType.Logarithmic);
            Assert.AreEqual(converter.MinFrequency, 20.0f);
            Assert.ThrowsException<ArgumentException>(() => { converter.MinFrequency = 0.0f; });
            Assert.ThrowsException<ArgumentException>(() => { converter.MinFrequency = 20000.0f; });
            Assert.ThrowsException<ArgumentException>(() => { converter.MinFrequency = -1.0f; });
            Assert.AreEqual(converter.MaxFrequency, 20000.0f);
            Assert.ThrowsException<ArgumentException>(() => { converter.MaxFrequency = 10.0f; });
            converter.FrequencyScale = ScaleType.Linear;
            converter.MinFrequency = 0.0f;
            Assert.AreEqual(0.0f, converter.MinFrequency);
            CollectionAssert.AreEqual(
                new string[] { "Source","FrequencyCount","ChannelCount",
                    "RmsRiseTime","RmsFallTime","PeakRiseTime","PeakFallTime",
                    "SpectrumRiseTime","SpectrumFallTime",
                    "MinFrequency","MaxFrequency","FrequencyScale","FrequencyScale","MinFrequency" },
                propertiesChanged
                );
            converter = null;
            GC.Collect();
        }


        public void SourceConverter_GetData()
        {
            SourceConverter converter = new SourceConverter();
            FakeVisualizationSource source = new FakeVisualizationSource();

            converter.Source = source;

            var frame = new VisualizationDataFrame(
                TimeSpan.FromSeconds(1),
                TimeSpan.FromTicks(166667),
                ScalarData.CreateEmpty(2),
                ScalarData.CreateEmpty(2),
                SpectrumData.CreateEmpty(2,1024,ScaleType.Linear,ScaleType.Linear,0.0f,22100.0f)
                );
            source.Frame = frame;

            // First step passthrough
            var f = converter.GetData();
            Assert.IsNotNull(f);
            Assert.AreEqual(2, f.Spectrum.Count);
            Assert.AreEqual(1024u, f.Spectrum.FrequencyCount);

            source.Frame = null;
            f = converter.GetData();
            Assert.IsNotNull(f);

        }

        public void SourceConverter_RiseAndFall()
        {
            SourceConverter converter = new SourceConverter();
            FakeVisualizationSource source = new FakeVisualizationSource();

            converter.Source = source;

            var frame1 = new VisualizationDataFrame(
                TimeSpan.FromSeconds(1),
                TimeSpan.FromTicks(166667),
                ScalarData.CreateEmpty(2),
                ScalarData.CreateEmpty(2),
                SpectrumData.CreateEmpty(2, 10, ScaleType.Linear, ScaleType.Linear, 0.0f, 22100.0f)
                );

            var frame2 = new VisualizationDataFrame(
                TimeSpan.FromSeconds(2),
                TimeSpan.FromTicks(166667),
                ScalarData.Create(new float[] { 0.5f, 1.0f }),
                ScalarData.Create(new float[] { 1.0f, 0.5f }),
                SpectrumData.Create(new float[][] 
                {
                    new float[] { 1.0f,0,0,0,0,0,0,0,0,0 },
                    new float[] { 0.5f,0,0,0,0,0,0,0,0,0 }
                },ScaleType.Linear,ScaleType.Linear,0.0f,22100.0f)
                );

            source.Frame = frame1;
            converter.SpectrumRiseTime = TimeSpan.FromMilliseconds(100);
            converter.SpectrumFallTime = TimeSpan.FromMilliseconds(50);
            converter.RmsRiseTime = TimeSpan.FromMilliseconds(80);
            converter.RmsFallTime = TimeSpan.FromMilliseconds(40);
            converter.PeakRiseTime = TimeSpan.FromMilliseconds(20);
            converter.PeakFallTime = TimeSpan.FromMilliseconds(200);
            var data1 = converter.GetData();
            source.Frame = frame2;
            var data2 = converter.GetData();
            Assert.IsNotNull(data2.RMS);
            Assert.IsNotNull(data2.Peak);
            Assert.IsNotNull(data2.Spectrum);

            CollectionAssert.AreEqual(new float[] {0.09403199f , 0.188063979f }, data2.RMS.ToArray());
            CollectionAssert.AreEqual(new float[] { 0.5654025f,0.282701254f }, data2.Peak.ToArray());
            CollectionAssert.AreEqual(new float[] { 0.1535185f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, data2.Spectrum[0].ToArray());
            CollectionAssert.AreEqual(new float[] { 0.07675925f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, data2.Spectrum[1].ToArray());
        }
    }
}
