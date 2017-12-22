using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;
using System.ComponentModel;
using Windows.Foundation;

namespace test.managed
{
    class FakeVisualizationSource : IVisualizationSource
    {
        public VisualizationDataFrame Frame;  
        public IVisualizationDataFrame GetData()
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

    [TestClass()]
    public class SourceConverterTests
    {
        [TestCategory("SourceConverter")]
        [TestMethod()]
        public void SourceConverter_props()
        {
            SourceConverter converter = new SourceConverter();
            Assert.ThrowsException<Exception>(() =>
            {
                var v = converter.Fps;
            });
            Assert.ThrowsException<Exception>(() =>
            {
                var v = converter.IsSuspended;
            });
            Assert.ThrowsException<Exception>(() =>
            {
                var v = converter.PlaybackState;
            });
            Assert.ThrowsException<Exception>(() =>
            {
                var v = converter.PresentationTime;
            });
            Assert.AreEqual(AnalyzerType.All, converter.AnalyzerTypes);
            Assert.IsNull(converter.Source);
            Assert.IsNull(converter.FrequencyCount);
            Assert.IsNull(converter.ChannelCount);
            Assert.IsNull(converter.SpectrumRiseTime);
            Assert.IsNull(converter.SpectrumFallTime);
            Assert.IsNull(converter.RmsRiseTime);
            Assert.IsNull(converter.RmsFallTime);
            Assert.IsNull(converter.PeakRiseTime);
            Assert.IsNull(converter.PeakFallTime);
            Assert.IsNull(converter.MinFrequency);
            Assert.IsNull(converter.MaxFrequency);
            Assert.IsNull(converter.FrequencyScale);

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
                new string[] { "Source","FrequencyCount","ChannelCount","RiseTime","FallTime","MinFrequency","MaxFrequency","FrequencyScale","FrequencyScale","MinFrequency" },
                propertiesChanged
                );
            converter = null;
            GC.Collect();
        }


        [TestCategory("SourceConverter")]
        [TestMethod()]
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
    }
}
