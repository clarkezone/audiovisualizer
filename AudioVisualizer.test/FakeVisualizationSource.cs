using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using AudioVisualizer;

namespace AudioVisualizer.test
{
    internal class FakeVisualizationSource : IVisualizationSource
    {
        private uint? expectedChannels = 2;
        public uint? ExpectedChannels {
            get => expectedChannels;
            set {
                if (value != expectedChannels)
                {
                    expectedChannels = value;
                    Frame = null;
                    ConfigurationChanged?.Invoke(this, "ChannelCount");
                }
            }
        }
        private uint? expectedFrequencies = 50;
        public uint? ExpectedFrequencies
        {
            get => expectedFrequencies;
            set
            {
                if (value != expectedFrequencies)
                {
                    expectedFrequencies = value;
                    Frame = null;
                    ConfigurationChanged?.Invoke(this, "FrequencyCount");
                }
            }
        }


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

        public uint? ActualChannelCount => ExpectedChannels;

        public uint? ActualFrequencyCount => ExpectedFrequencies;

        public float? ActualMaxFrequency => 20000.0f;

        public float? ActualMinFrequency => 0.0f;

        public ScaleType? ActualFrequencyScale => ScaleType.Linear;


    }

}
