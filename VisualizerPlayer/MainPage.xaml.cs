using System;
using System.Collections.Generic;
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
using Windows.Storage.Pickers;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using Windows.UI;
using System.Threading;
using System.Threading.Tasks;
using AudioVisualizer;
using Windows.Media.Core;
using Windows.Media.Playback;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace VisualizerPlayer
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {       
        public async Task<AudioVisualizer.IVisualizationSource> CreateAnalyzerAsync(MediaPlayer element)
        {
            var propSet = new PropertySet();
            ManualResetEventSlim opComplete = new ManualResetEventSlim();
            AudioVisualizer.IVisualizationSource source = null;
            propSet.MapChanged += new MapChangedEventHandler<string,object>(
                (IObservableMap<string, object> sender, IMapChangedEventArgs<string> @event) =>
                {
                    source = (AudioVisualizer.IVisualizationSource)sender["Source"];
                    opComplete.Set();
                }
                );
            element.AddAudioEffect("AudioAnalyzer.MftAnalyzer", false, propSet);

            await Task.Run(() => { opComplete.Wait(); }); 
            return source; 
        }


        public MainPage()
        {
            this.InitializeComponent();
        }

        private void _player_SourceChanged(MediaPlayer sender, object args)
        {
            System.Diagnostics.Debug.WriteLine("Source changed");
        }

        private void _player_MediaOpened(MediaPlayer sender, object args)
        {
            System.Diagnostics.Debug.WriteLine("Mediaopened");
        }

        private async void OpenFile_Click(object sender, RoutedEventArgs e)
        {
            var picker = new FileOpenPicker();
            picker.FileTypeFilter.Add(".wav");
            picker.FileTypeFilter.Add(".mp3");
            picker.FileTypeFilter.Add(".wmv");
            picker.FileTypeFilter.Add(".mp4");
            var file = await picker.PickSingleFileAsync();
            if (file != null)
            {
                mePlayer.SetSource(await file.OpenAsync(Windows.Storage.FileAccessMode.Read), file.FileType);
            }
        }

        private void CanvasAnimatedControl_CreateResources(Microsoft.Graphics.Canvas.UI.Xaml.CanvasAnimatedControl sender, Microsoft.Graphics.Canvas.UI.CanvasCreateResourcesEventArgs args)
        {
        }

        private void CanvasAnimatedControl_Draw(Microsoft.Graphics.Canvas.UI.Xaml.ICanvasAnimatedControl sender, Microsoft.Graphics.Canvas.UI.Xaml.CanvasAnimatedDrawEventArgs args)
        {
            if (m_VisualizationSource != null)
            {
  
            }
        }

        Size m_VisualizationSize;
        private IVisualizationSource m_VisualizationSource;

        private void CanvasAnimatedControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            m_VisualizationSize = e.NewSize;
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            CreateVisualizer();
        }

        private async void CreateVisualizer()
        {
            m_VisualizationSource = await AudioVisualizer.VisualizationSource.CreateFromMediaElementAsync(mePlayer);
            m_VisualizationSource.Configure(AnalyzerType.All,60, 4096, 0.5f);
            visualizer.Source = m_VisualizationSource;
        }

        private void ToggleSwitch_Toggled(object sender, RoutedEventArgs e)
        {
            ToggleSwitch sw = (ToggleSwitch)sender;
            if (m_VisualizationSource != null)
                m_VisualizationSource.IsSuspended = !sw.IsOn;
        }

        private void ToggleButton_Click(object sender, RoutedEventArgs e)
        {
        }

        ScalarData previousRMS;

        private void visualizer_Draw(AudioVisualizer.IVisualizer sender, AudioVisualizer.VisualizerDrawEventArgs args)
        {
            var traceActivity = Trace.BaseVisualizer_StartDraw();

            
            if (args.Data != null)
            {
                using (var data = args.Data.GetReference())
                {
                    previousRMS = data.RMS.ApplyRiseAndFall(previousRMS, TimeSpan.FromMilliseconds(50), TimeSpan.FromMilliseconds(300), TimeSpan.FromMilliseconds(16.7));
                    var logRMS = previousRMS.ConvertToLogAmplitude(-100.0f, 0.0f).Values;

                    args.DrawingSession.FillRectangle(10, 10, 20 + (100.0f + (logRMS[0])), 20, Colors.Green);
                    args.DrawingSession.FillRectangle(10, 40, 20 + (100.0f + (logRMS[1])), 20, Colors.Green);
                }
            }
            else
            {
                previousRMS = null;
            }

            traceActivity.StopActivity(traceActivity.Name);
        }
    }
}
