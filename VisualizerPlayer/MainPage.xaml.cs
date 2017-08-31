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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace VisualizerPlayer
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        AudioAnalyzer.IVisualizationSource m_VisualizationSource;


        public async Task<AudioAnalyzer.IVisualizationSource> CreateAnalyzerAsync(MediaElement element)
        {
            var propSet = new PropertySet();
            ManualResetEventSlim opComplete = new ManualResetEventSlim();
            AudioAnalyzer.IVisualizationSource source = null;
            propSet.MapChanged += new MapChangedEventHandler<string,object>(
                (IObservableMap<string, object> sender, IMapChangedEventArgs<string> @event) =>
                {
                    source = (AudioAnalyzer.IVisualizationSource)sender["Source"];
                    opComplete.Set();
                }
                );
            element.AddAudioEffect("AudioAnalyzer.AnalyzerEffect", false, propSet);

            await Task.Run(() => { opComplete.Wait(); }); 
            return source; 
        }


        public MainPage()
        {
            this.InitializeComponent();
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
                var stream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read);
                mePlayer.SetSource(stream, file.ContentType);

                mePlayer.Play();
            }
        }

        private void CanvasAnimatedControl_CreateResources(Microsoft.Graphics.Canvas.UI.Xaml.CanvasAnimatedControl sender, Microsoft.Graphics.Canvas.UI.CanvasCreateResourcesEventArgs args)
        {
        }

        private void CanvasAnimatedControl_Draw(Microsoft.Graphics.Canvas.UI.Xaml.ICanvasAnimatedControl sender, Microsoft.Graphics.Canvas.UI.Xaml.CanvasAnimatedDrawEventArgs args)
        {
            if (m_VisualizationSource != null)
            {
                /*
                var frame = m_VisualizationSource.GetFrame();
                if (frame != null)
                {
                    try
                    {
                        var data = frame.AsVisualizationData();
                        float barWidth = (float)(m_VisualizationSize.Width / 202.0);
                        float position = 0.0f;
                        for (uint i = 0; i < 202; i++, position += barWidth)
                        {
                            float height = 100 + data[i];
                            args.DrawingSession.FillRectangle(position, 0, barWidth, 100 + data[i], Colors.Red);
                        }
                    }
                    catch (Exception e)
                    {
                        System.Diagnostics.Debug.WriteLine($"{e.Message}");
                    }
                }
                else
                {
#pragma warning disable CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed
                    Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, ()=>
                    {
                        System.Diagnostics.Debug.WriteLine($"Analyzer frame null Player at:{mePlayer.Position} D2D frame {args.Timing.UpdateCount} ");
                    }
                    );
#pragma warning restore CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed
                }*/
            }
        }

        Size m_VisualizationSize;
        private void CanvasAnimatedControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            m_VisualizationSize = e.NewSize;
        }

        private async void Page_Loaded(object sender, RoutedEventArgs e)
        {
            m_VisualizationSource = await AudioAnalyzer.VisualizationData.CreateVisualizationSourceAsync(mePlayer);
            m_VisualizationSource.Configure(4096, 60, 0.5f);
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

        private void visualizer_Draw(AudioAnalyzer.IVisualizer sender, AudioAnalyzer.VisualizerDrawEventArgs args)
        {

        }
    }
}
