using AudioVisualizer;
using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.Text;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Media.Core;
using Windows.Media.Playback;
using Windows.Storage.Pickers;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace VisualizationPlayer
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {

        MediaPlayer _player;

        public MainPage()
        {
            this.InitializeComponent();
            _player = new MediaPlayer();
            _player.MediaOpened += Player_MediaOpened;
            _player.PlaybackSession.PositionChanged += PlaybackSession_PositionChanged;
        }

        bool _bInsideUpdate = false;
        private async void PlaybackSession_PositionChanged(MediaPlaybackSession sender, object args)
        {

            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
    () =>
    {
        _bInsideUpdate = true;
        seekBar.Value = _player.PlaybackSession.Position.TotalSeconds;
        _bInsideUpdate = false;
    }
    );
        }

        private async void Player_MediaOpened(MediaPlayer sender, object args)
        {
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    seekBar.Maximum = _player.PlaybackSession.NaturalDuration.TotalSeconds;
                }
            );
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
                _player.Source = MediaSource.CreateFromStorageFile(file);
                _player.Play();
            }
        }


        private IVisualizationSource m_VisualizationSource;

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            CreateVisualizer();
        }

        private async void CreateVisualizer()
        {

            m_VisualizationSource = await AudioVisualizer.VisualizationSource.CreateFromMediaPlayerAsync(_player);
            visualizer.Source = m_VisualizationSource;
            vuBar.Source = m_VisualizationSource;
        }

        private void ToggleSwitch_Toggled(object sender, RoutedEventArgs e)
        {
            ToggleSwitch sw = (ToggleSwitch)sender;
            if (m_VisualizationSource != null)
                m_VisualizationSource.IsSuspended = !sw.IsOn;
        }

        ScalarData _emptyVolumeData = new ScalarData(2);    // Create empty data for volume data
        ScalarData _previousRMS;
        ScalarData _previousPeak;

        ArrayData _emptySpectrum = new ArrayData(2, 20);
        ArrayData _previousSpectrum;
        ArrayData _previousPeakSpectrum;

        TimeSpan _rmsRiseTime = TimeSpan.FromMilliseconds(50);
        TimeSpan _rmsFallTime = TimeSpan.FromMilliseconds(50);
        TimeSpan _peakRiseTime = TimeSpan.FromMilliseconds(10);
        TimeSpan _peakFallTime = TimeSpan.FromMilliseconds(3000);
        TimeSpan _frameDuration = TimeSpan.FromMilliseconds(16.7);

        private void visualizer_Draw(AudioVisualizer.IVisualizer sender, AudioVisualizer.VisualizerDrawEventArgs args)
        {
            var drawingSession = (CanvasDrawingSession)args.DrawingSession;

            CanvasTextFormat textFormat = new CanvasTextFormat();
            textFormat.VerticalAlignment = CanvasVerticalAlignment.Center;
            textFormat.HorizontalAlignment = CanvasHorizontalAlignment.Center;
            textFormat.FontSize = 9;

            for (int i = -40; i <= 0; i += 10)
            {
                drawingSession.DrawLine(810 + 20 * i, 10, 810 + 20 * i, 60, Colors.White);
                drawingSession.DrawText($"{i}dB", 810 + 20 * i, 70, Colors.White, textFormat);
            }

            drawingSession.DrawLine(0, 320, 1000, 320, Colors.WhiteSmoke);
            drawingSession.DrawLine(0, 340, 1000, 340, Colors.WhiteSmoke);

            var rmsData = args.Data != null ? args.Data.RMS : _emptyVolumeData;
            var peakData = args.Data != null ? args.Data.Peak : _emptyVolumeData;

            _previousRMS = rmsData.ApplyRiseAndFall(_previousRMS, _rmsRiseTime, _rmsFallTime, _frameDuration);
            _previousPeak = peakData.ApplyRiseAndFall(_previousPeak, _peakRiseTime, _peakFallTime, _frameDuration);

            var logRMS = _previousRMS.ConvertToLogAmplitude(-40.0f, 0.0f);
            var logPeak = _previousPeak.ConvertToLogAmplitude(-40.0f, 0.0f);

            drawingSession.FillRectangle(10, 10, 20 + 20 * (40.0f + (logRMS[0])), 20, Colors.Green);
            drawingSession.FillRectangle(10, 40, 20 + 20 * (40.0f + (logRMS[1])), 20, Colors.Green);

            drawingSession.DrawLine(820.0f + 20 * logPeak[0], 10, 820.0f + 20 * logPeak[0], 30, Colors.Red, 3);
            drawingSession.DrawLine(820.0f + 20 * logPeak[1], 40, 820.0f + 20 * logPeak[1], 60, Colors.Red, 3);

            var spectrum = args.Data != null ? args.Data.Spectrum.TransformLinearFrequency(20) : _emptySpectrum;

            _previousSpectrum = spectrum.ApplyRiseAndFall(_previousSpectrum, _rmsRiseTime, _rmsFallTime, _frameDuration);
            _previousPeakSpectrum = spectrum.ApplyRiseAndFall(_previousPeakSpectrum, _peakRiseTime, _peakFallTime, _frameDuration);

            var logSpectrum = _previousSpectrum.ConvertToLogAmplitude(-50, 0);
            var logPeakSpectrum = _previousPeakSpectrum.ConvertToLogAmplitude(-50, 0);

            Vector2 prevPointLeft = new Vector2(), prevPointRight = new Vector2();

            for (int i = 0; i < logSpectrum.FrequencyCount; i++)
            {
                float barHeight0 = 160 + 3.2f * logSpectrum[0][i];
                drawingSession.FillRectangle(i * 50, 320 - barHeight0, 50, barHeight0, Colors.WhiteSmoke);
                float barHeight1 = 160 + 3.2f * logSpectrum[1][i];
                drawingSession.FillRectangle(i * 50, 340, 50, barHeight1, Colors.WhiteSmoke);

                Vector2 leftPoint = new Vector2(i * 50 + 25, 160 - 3.2f * logPeakSpectrum[0][i]);
                Vector2 rightPoint = new Vector2(i * 50 + 25, 500 + 3.2f * logPeakSpectrum[1][i]);
                if (i != 0)
                {
                    drawingSession.DrawLine(prevPointLeft, leftPoint, Colors.Red, 3);
                    drawingSession.DrawLine(prevPointRight, rightPoint, Colors.Red, 3);
                }
                prevPointLeft = leftPoint;
                prevPointRight = rightPoint;

                string freqText = $"{Math.Round(logSpectrum.FrequencyStep * i * 1e-3),2:F0}k";
                drawingSession.DrawText(freqText, i * 50 + 25, 330, Colors.White, textFormat);
            }
        }

        private void seekBar_ValueChanged(object sender, RangeBaseValueChangedEventArgs e)
        {
            if (!_bInsideUpdate)
            {
                _player.PlaybackSession.Position = TimeSpan.FromSeconds(e.NewValue);
            }
        }
    }
}
