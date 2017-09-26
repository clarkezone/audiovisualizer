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
using Microsoft.Graphics.Canvas.Text;
using System.Numerics;

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
            propSet.MapChanged += new MapChangedEventHandler<string, object>(
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


        private IVisualizationSource m_VisualizationSource;

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            CreateVisualizer();
        }

        private async void CreateVisualizer()
        {
            m_VisualizationSource = await AudioVisualizer.VisualizationSource.CreateFromMediaElementAsync(mePlayer);
            m_VisualizationSource.Configure(AnalyzerType.All, 60, 4096, 0.5f);
            //visualizer.Source = m_VisualizationSource;
            vuBar.Source = m_VisualizationSource;
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

        ScalarData _previousRMS;
        ScalarData _previousPeak;
        ArrayData _previousSpectrum;
        ArrayData _previousPeakSpectrum;

        private void visualizer_Draw(AudioVisualizer.IVisualizer sender, AudioVisualizer.VisualizerDrawEventArgs args)
        {
            var traceActivity = Trace.BaseVisualizer_StartDraw();

            if (args.Data != null)
            {
                _previousRMS = args.Data.RMS.ApplyRiseAndFall(_previousRMS, TimeSpan.FromMilliseconds(50), TimeSpan.FromMilliseconds(50), args.Data.Duration.Value);
                _previousPeak = args.Data.Peak.ApplyRiseAndFall(_previousPeak, TimeSpan.FromMilliseconds(1), TimeSpan.FromMilliseconds(3000), args.Data.Duration.Value);
                var logRMS = _previousRMS.ConvertToLogAmplitude(-40.0f, 0.0f);
                var logPeak = _previousPeak.ConvertToLogAmplitude(-40.0f, 0.0f);

                args.DrawingSession.FillRectangle(10, 10, 20 + 20*(40.0f + (logRMS[0])), 20, Colors.Green);
                args.DrawingSession.FillRectangle(10, 40, 20 + 20*(40.0f + (logRMS[1])), 20, Colors.Green);

                CanvasTextFormat textFormat = new CanvasTextFormat();
                textFormat.VerticalAlignment = CanvasVerticalAlignment.Center;
                textFormat.HorizontalAlignment = CanvasHorizontalAlignment.Center;
                textFormat.FontSize = 9;

                for (int i = -40; i <= 0; i+=10)
                {
                    args.DrawingSession.DrawLine(810 + 20 * i, 10, 810 + 20 * i, 60,Colors.White);
                    args.DrawingSession.DrawText($"{i}dB", 810 + 20 * i, 70, Colors.White, textFormat);
                }

                args.DrawingSession.DrawLine(820.0f + 20 * logPeak[0], 10, 820.0f + 20*logPeak[0], 30, Colors.Red, 3);
                args.DrawingSession.DrawLine(820.0f + 20 * logPeak[1], 40, 820.0f + 20*logPeak[1], 60, Colors.Red, 3);

                var spectrum = args.Data.Spectrum.TransformLinearFrequency(20);

                _previousSpectrum = spectrum.ApplyRiseAndFall(_previousSpectrum,TimeSpan.FromMilliseconds(100),TimeSpan.FromMilliseconds(100),args.Data.Duration.Value);
                _previousPeakSpectrum = spectrum.ApplyRiseAndFall(_previousPeakSpectrum, TimeSpan.FromMilliseconds(10), TimeSpan.FromMilliseconds(2000), args.Data.Duration.Value);
                args.DrawingSession.DrawLine(0, 320, 1000, 320, Colors.WhiteSmoke);
                args.DrawingSession.DrawLine(0, 340, 1000, 340, Colors.WhiteSmoke);

                var logSpectrum = _previousSpectrum.ConvertToLogAmplitude(-50, 0);
                var logPeakSpectrum = _previousPeakSpectrum.ConvertToLogAmplitude(-50, 0);

                Vector2 prevPointLeft = new Vector2(),prevPointRight = new Vector2();

                for (int i = 0; i < logSpectrum.FrequencyCount; i++)
                {
                    float barHeight0 = 160+3.2f * logSpectrum[0][i];
                    args.DrawingSession.FillRectangle(i * 50, 320 - barHeight0, 50, barHeight0, Colors.WhiteSmoke);
                    float barHeight1 = 160+3.2f * logSpectrum[1][i];
                    args.DrawingSession.FillRectangle(i * 50, 340, 50, barHeight1, Colors.WhiteSmoke);

                    Vector2 leftPoint = new Vector2(i * 50 + 25, 160 - 3.2f * logPeakSpectrum[0][i]);
                    Vector2 rightPoint = new Vector2(i * 50 + 25, 500 + 3.2f * logPeakSpectrum[1][i]);
                    if (i != 0)
                    {
                        args.DrawingSession.DrawLine(prevPointLeft, leftPoint, Colors.Red, 3);
                        args.DrawingSession.DrawLine(prevPointRight, rightPoint, Colors.Red, 3);
                    }
                    prevPointLeft = leftPoint;
                    prevPointRight = rightPoint;

                    string freqText = $"{Math.Round(logSpectrum.FrequencyStep * i * 1e-3),2:F0}k";
                    args.DrawingSession.DrawText(freqText, i * 50 + 25, 330, Colors.White, textFormat);
                }
            }
            else
            {
                _previousRMS = null;
                _previousPeak = null;
                _previousSpectrum = null;
            }
            traceActivity.StopActivity(traceActivity.Name);
        }
    }
}
