using AudioVisualizer;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace VisualizationPlayer
{
    public class FakeDataSource : IVisualizationSource
    {
        public VisualizationDataFrame Frame;
        public VisualizationDataFrame GetData()
        {
            return Frame;
        }

        public float? ActualFrequencyStep => 24000.0f / 1024f;
        public uint? ActualChannelCount => 1;

        public uint? ActualFrequencyCount => 1024;

        public ScaleType? ActualFrequencyScale => ScaleType.Linear;

        public float? ActualMaxFrequency => 24000.0f;

        public float? ActualMinFrequency => 0.0f;

        public AnalyzerType AnalyzerTypes { get => AnalyzerType.All; set => throw new NotImplementedException(); }
        public float Fps { get => 60.0f; set => throw new NotImplementedException(); }
        public bool IsSuspended { get => false; set => throw new NotImplementedException(); }

        public SourcePlaybackState PlaybackState => SourcePlaybackState.Playing;

        public TimeSpan? PresentationTime => throw new NotImplementedException();

        public event TypedEventHandler<IVisualizationSource, string> ConfigurationChanged;
    }

    public class ExperimentDataContext : INotifyPropertyChanged
    {
        private SourceConverter _source;
        public FakeDataSource MainSource;

        public ExperimentDataContext() 
        {
            MainSource = new FakeDataSource();
            _source = new SourceConverter();
            _source.Source = MainSource;
            GenerateDataFrame();
        }

        private float rms = -100.0f;
        public float Rms
        { get => rms;
            set
            {
                rms = value;
                GenerateDataFrame();
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Rms"));
            }
        }
        private float peak = -100.0f;


        public float Peak
        {
            get => peak;
            set
            {
                peak = value;
                GenerateDataFrame();
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Peak"));
            }
        }

        public IVisualizationSource Source { get => _source; }

        private void GenerateDataFrame()
        {
            var rmsValue = ScalarData.Create(new float[] { (float) (Math.Pow(10,rms/20)) });
            var peakValue = ScalarData.Create(new float[] { (float)(Math.Pow(10,peak/20)) });
            MainSource.Frame = new VisualizationDataFrame(TimeSpan.Zero, TimeSpan.FromMilliseconds(16.7), rmsValue, peakValue, null);
        }
        public event PropertyChangedEventHandler PropertyChanged;
    }

    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ExperimentWithControlsPage : Page
    {
        ExperimentDataContext Context;
        public ExperimentWithControlsPage()
        {
            this.InitializeComponent();
            Context = (ExperimentDataContext) Resources["dataContext"];
            Loaded += ExperimentWithControlsPage_Loaded;
        }

        private void ExperimentWithControlsPage_Loaded(object sender, RoutedEventArgs e)
        {
            barControl.Source = Context.Source;
            UpdateBarLabels();
        }

        void UpdateBarLabels()
        {
            barLabels.Children.Clear();
            barLabels.RowDefinitions.Clear();
            foreach (var level in barControl.Levels.OrderByDescending((barLevel) => barLevel.Level))
            {
                var text = new TextBlock() { HorizontalAlignment = HorizontalAlignment.Right, VerticalAlignment = VerticalAlignment.Center };
                text.Text = $"{barLabels.RowDefinitions.Count()}. {level.Level}dB";
                Grid.SetRow(text, barLabels.RowDefinitions.Count());
                barLabels.RowDefinitions.Add(new RowDefinition());
                barLabels.Children.Add(text);
            }

        }
    }
}
