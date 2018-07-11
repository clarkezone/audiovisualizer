using AudioVisualizer;
using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.Text;
using Microsoft.Graphics.Canvas.UI.Composition;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.DirectX;
using Windows.Media.Core;
using Windows.Media.Playback;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.UI;
using Windows.UI.Composition;
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
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MediaPlayerPage : Page, IBarElementFactory
    {
        MediaPlayer _player;
        PlaybackSource _playbackSource;
        SourceConverter _source;
        SourceConverter _analogSource;
        SourceConverter _spectrumSource;

        public MediaPlayerPage()
        {
            this.InitializeComponent();
            _player = new MediaPlayer();
            _player.MediaOpened += Player_MediaOpened;
            _player.PlaybackSession.PositionChanged += Player_PositionChanged;
            _playbackSource = PlaybackSource.CreateFromMediaPlayer(_player);
            _playbackSource.SourceChanged += PlaybackSource_Changed;
            _source = (SourceConverter) Resources["source"];
            _analogSource = (SourceConverter)Resources["analogSource"];
            _spectrumSource = (SourceConverter)Resources["spectrumSource"];
            _source.RmsRiseTime = TimeSpan.FromMilliseconds(50);
            _source.RmsFallTime = TimeSpan.FromMilliseconds(50);
            _source.PeakRiseTime = TimeSpan.FromMilliseconds(50);
            _source.PeakFallTime = TimeSpan.FromMilliseconds(1000);
            _analogSource.RmsRiseTime = TimeSpan.FromMilliseconds(500);
            _analogSource.RmsFallTime = TimeSpan.FromMilliseconds(500);
            _analogSource.AnalyzerTypes = AnalyzerType.RMS;
            _spectrumSource.SpectrumRiseTime = TimeSpan.FromMilliseconds(100);
            _spectrumSource.SpectrumFallTime = TimeSpan.FromMilliseconds(200);
            _spectrumSource.FrequencyCount = 50;
            _spectrumSource.MinFrequency = 20.0f;
            _spectrumSource.MaxFrequency = 20000.0f;
            _spectrumSource.FrequencyScale = ScaleType.Logarithmic;
            _spectrumSource.AnalyzerTypes = AnalyzerType.Spectrum;

            // Create bar steps with 1db steps from -86db to +6
            const int fromDb = -86;
            const int toDb = 6;
            MeterBarLevel[] levels = new MeterBarLevel[toDb-fromDb];
            Color fromColor = Colors.Yellow;
            Color toColor = Colors.Red;
            float redStep = (float)toColor.R - (float)fromColor.R;
            float greenStep = (float)toColor.G - (float)fromColor.G;
            float blueStep = (float)toColor.B - (float)fromColor.B;

            for (int i = 0; i < levels.Count(); i++)
            {
                float ratio = (float)i / (float) levels.Count();
                levels[i].Color = Color.FromArgb(255,(byte)((redStep * ratio) + fromColor.R), (byte)((greenStep * ratio) + fromColor.G), (byte)((blueStep * ratio) + fromColor.B));
                levels[i].Level = i + fromDb;
            }
            bar0.Levels = levels;
            bar1.Levels = levels;
            barspectrum.ElementFactory = this;
            barspectrum.ElementShadowBlurRadius = 3;
            barspectrum.ElementShadowOffset = new Vector3(5, 5, 10);
            barspectrum.ElementShadowColor = Colors.Black;
        }

        private void PlaybackSource_Changed(object sender, IVisualizationSource args)
        {
            _source.Source = _playbackSource.Source;
            _analogSource.Source = _playbackSource.Source;
            _spectrumSource.Source = _playbackSource.Source;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            _source.Source = _playbackSource.Source;
            _analogSource.Source = _playbackSource.Source;
            _spectrumSource.Source = _playbackSource.Source;
        }

        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            base.OnNavigatingFrom(e);
            _player.Pause(); 
        }

        private async void OpenFile_Click(object sender, RoutedEventArgs e)
        {
            var picker = new FileOpenPicker();
            picker.FileTypeFilter.Add(".wav");
            picker.FileTypeFilter.Add(".mp3");
            picker.FileTypeFilter.Add(".wmv");
            picker.FileTypeFilter.Add(".mp4");
            picker.FileTypeFilter.Add(".flac");
            var file = await picker.PickSingleFileAsync();
            if (file != null)
            {
                await OpenMediaFile(file);
                _player.Play();
            }
        }

        private async Task OpenMediaFile(StorageFile file)
        {
            _player.Source = MediaSource.CreateFromStorageFile(file);
            MusicProperties musicProps = await file.Properties.GetMusicPropertiesAsync();
            nowPlaying.Text = $"Now playing {musicProps.Title} by {musicProps.Artist}";
        }

        bool _insidePositionUpdate = false;

        private async void Player_PositionChanged(MediaPlaybackSession session, object args)
        {
          
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    _insidePositionUpdate = true;
                    seekSlider.Value = session.Position.TotalSeconds;
                    _insidePositionUpdate = false;
                });
        }

        private async void Player_MediaOpened(object sender, object e)
        {
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    seekSlider.Maximum = _player.PlaybackSession.NaturalDuration.TotalSeconds;
                });
        }


        private void PositionDisplay_Draw(VisualizerControl sender, VisualizerDrawEventArgs args)
        {
            if (args.PresentationTime != null)
            {
                string timeString = args.PresentationTime.Value.ToString("mm\\:ss\\.ff");
                var ds = (CanvasDrawingSession)args.DrawingSession;
                ds.DrawText(timeString, 0, 0, Colors.Gray);
            }
        }

        private void PlayButton_Click(object sender, RoutedEventArgs e)
        {
            _player.Play();
        }

        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            _player.Pause();
        }

        private void seekSlider_ValueChanged(object sender, RangeBaseValueChangedEventArgs e)
        {
            if (!_insidePositionUpdate)
            {
                _player.PlaybackSession.Position =  TimeSpan.FromSeconds(e.NewValue);
            }
        }

        private void spectrum_CreateResources(object sender, AudioVisualizer.CreateResourcesEventArgs args)
        {
            _spectrumTextFormat = new CanvasTextFormat();
            _spectrumTextFormat.VerticalAlignment = CanvasVerticalAlignment.Center;
            _spectrumTextFormat.HorizontalAlignment = CanvasHorizontalAlignment.Center;
            _spectrumTextFormat.FontSize = 9;
        }

        const uint spectrumBarCount = 50;
        SpectrumData _emptySpectrum = SpectrumData.CreateEmpty(2, spectrumBarCount,ScaleType.Linear,ScaleType.Linear,0, 20000);
        SpectrumData _previousSpectrum;
        SpectrumData _previousPeakSpectrum;

        TimeSpan _rmsRiseTime = TimeSpan.FromMilliseconds(50);
        TimeSpan _rmsFallTime = TimeSpan.FromMilliseconds(50);
        TimeSpan _peakRiseTime = TimeSpan.FromMilliseconds(10);
        TimeSpan _peakFallTime = TimeSpan.FromMilliseconds(3000);
        TimeSpan _frameDuration = TimeSpan.FromMilliseconds(16.7);
        CanvasTextFormat _spectrumTextFormat;

        private void spectrum_Draw(object sender, AudioVisualizer.VisualizerDrawEventArgs args)
        {
            var drawingSession = (CanvasDrawingSession)args.DrawingSession;

            Thickness margin = new Thickness(40,10,20,10);
            // Calculate spectum metrics
            Vector2 barSize = new Vector2((float)(args.ViewExtent.Width - margin.Left - margin.Right) / spectrumBarCount,
                                    (float)(args.ViewExtent.Height / 2 - margin.Top * 2 - margin.Bottom * 2)); // Top and bottom margins apply twice (as there are two spectrum displays)

            Color gridlineColor = Colors.Black;
            Color textColor = Colors.Blue;

            // Draw bounding rectangles for spectrum
            Rect boundingRectTop = new Rect(margin.Left, margin.Top, args.ViewExtent.Width - margin.Right - margin.Left, barSize.Y);
            Rect boundingRectBottom = new Rect(margin.Left, margin.Top * 2 + margin.Bottom + barSize.Y, args.ViewExtent.Width - margin.Right - margin.Left, barSize.Y);

            // Get the data if data exists and source is in play state, else use empty
            var spectrumData = args.Data != null && 
                                spectrum.Source?.PlaybackState == SourcePlaybackState.Playing ? 
                                args.Data.Spectrum.LogarithmicTransform(spectrumBarCount,20f,20000f) : 
                                _emptySpectrum;

            _previousSpectrum = spectrumData.ApplyRiseAndFall(_previousSpectrum, _rmsRiseTime, _rmsFallTime, _frameDuration);
            _previousPeakSpectrum = spectrumData.ApplyRiseAndFall(_previousPeakSpectrum, _peakRiseTime, _peakFallTime, _frameDuration);

            var logSpectrum = _previousSpectrum.ConvertToDecibels(-50, 0);
            var logPeakSpectrum = _previousPeakSpectrum.ConvertToDecibels(-50, 0);

            // Draw spectrum bars
            for (int index = 0; index < spectrumBarCount; index++)
            {
                float barX = (float)margin.Left + index * barSize.X;

                float spectrumBarHeight1 = barSize.Y * (1.0f - logSpectrum[0][index] / -50.0f);
                float spectrumBarHeight2 = barSize.Y * (1.0f - logSpectrum[1][index] / -50.0f);
                drawingSession.FillRectangle(barX, (float)boundingRectTop.Bottom - spectrumBarHeight1, barSize.X, spectrumBarHeight1, Colors.DarkCyan);
                drawingSession.FillRectangle(barX, (float)boundingRectBottom.Bottom - spectrumBarHeight2, barSize.X, spectrumBarHeight2, Colors.DarkCyan);
            }

                // Spectrum points to draw a slow decay line
                Vector2 prevPointLeft = new Vector2(), prevPointRight = new Vector2();
                for (int index = 0; index < spectrumBarCount; index++)
                {
                    float X = (float)margin.Left + index * barSize.X + barSize.X / 2;

                    Vector2 leftPoint = new Vector2(X, (float)boundingRectTop.Bottom - barSize.Y * (1.0f - logPeakSpectrum[0][index] / -50.0f));
                    Vector2 rightPoint = new Vector2(X, (float)boundingRectBottom.Bottom - barSize.Y * (1.0f - logPeakSpectrum[1][index] / -50.0f));
                    if (index != 0)
                    {
                        drawingSession.DrawLine(prevPointLeft, leftPoint, Colors.Red, 3);
                        drawingSession.DrawLine(prevPointRight, rightPoint, Colors.Red, 3);
                    }
                    prevPointLeft = leftPoint;
                    prevPointRight = rightPoint;
                }

            // Draw grid for 1k step from 0 - 20kHz
            float fStepWidth = (float) boundingRectTop.Width / 20;
            float freq = 20.0f;
            float fStep = (float) Math.Pow(1000.0, 1.0 / 20.0);
            for (int f=0;f<20;f++,freq*=fStep)
            {
                float X = f * fStepWidth + (float) margin.Left;
                if (f!=0)
                {
                    drawingSession.DrawLine(X, (float)boundingRectTop.Top, X, (float)boundingRectTop.Bottom, gridlineColor);
                    drawingSession.DrawLine(X, (float)boundingRectBottom.Top, X, (float)boundingRectBottom.Bottom, gridlineColor);
                }
                string freqText = freq < 100.0f ? $"{freq:F0}" :
                                    freq < 1000.0f ? $"{10 * Math.Round(freq / 10):F0}" :
                                    freq < 10000.0f ? $"{freq / 1e3:F1}k" : $"{freq / 1e3:F0}k";
                drawingSession.DrawText(freqText, X + fStepWidth/2, (float)boundingRectTop.Bottom + 10, textColor, _spectrumTextFormat);
                drawingSession.DrawText(freqText, X + fStepWidth / 2, (float)boundingRectBottom.Bottom + 10, textColor, _spectrumTextFormat);
            }

            drawingSession.DrawRectangle(boundingRectTop, gridlineColor);
            drawingSession.DrawRectangle(boundingRectBottom, gridlineColor);

            // Draw db based volume grid for spectrum -40 to 0db, every 10db
            for (int i = -50; i <= 0; i += 10)
            {
                float topY = (float)boundingRectTop.Top - (float)i * (float)boundingRectTop.Height / 50.0f;
                float bottomY = (float)boundingRectBottom.Bottom + (float)i * (float)boundingRectBottom.Height / 50.0f;
                drawingSession.DrawLine((float)boundingRectTop.Left, topY, (float)boundingRectTop.Right, topY, gridlineColor);
                drawingSession.DrawLine((float)boundingRectTop.Left, bottomY, (float)boundingRectTop.Right, bottomY, gridlineColor);
                drawingSession.DrawText($"{i}dB", (float)boundingRectTop.Left - (float)margin.Left / 2, topY, textColor, _spectrumTextFormat);
                drawingSession.DrawText($"{i}dB", (float)boundingRectTop.Left - (float)margin.Left / 2, bottomY, textColor, _spectrumTextFormat);

            }
        }

        TimeSpan position = TimeSpan.Zero;

        private void PositionDisplay_Draw(object sender, VisualizerDrawEventArgs args)
        {
            if (args.Data != null)
            {
                var ds = args.DrawingSession as CanvasDrawingSession;
                if (args.Data.Time != null)
                {
                    position = args.Data.Time.Value;
                }
                string timeString = String.Format("mm:ss.ff", position);
                ds.DrawText(timeString, 0, 0, Colors.DarkGray);

            }
        }

        public CompositionBrush CreateElementBrush(object sender, Color elementColor, Size size, Compositor compositor, CompositionGraphicsDevice device)
        {
            if (size.Width == 0 && size.Height == 0)
                return null;

            var surface = device.CreateDrawingSurface(size, DirectXPixelFormat.B8G8R8A8UIntNormalized, DirectXAlphaMode.Premultiplied);
            using (var drawingSession = CanvasComposition.CreateDrawingSession(surface))
            {
                drawingSession.Clear(Colors.Transparent);
                var center = size.ToVector2() / 2.0f;
                var radius = center.X > center.Y ? center.Y : center.X;
                drawingSession.FillCircle(center,radius, elementColor);
            }
            return compositor.CreateSurfaceBrush(surface);
        }

        public CompositionBrush CreateShadowMask(object sender, Size size, Compositor compositor, CompositionGraphicsDevice device)
        {
            if (size.Width == 0 && size.Height == 0)
                return null;

            var surface = device.CreateDrawingSurface(size, DirectXPixelFormat.B8G8R8A8UIntNormalized, DirectXAlphaMode.Premultiplied);
            using (var drawingSession = CanvasComposition.CreateDrawingSession(surface))
            {
                drawingSession.Clear(Colors.Transparent);
                var center = size.ToVector2() / 2.0f;
                var radius = center.X > center.Y ? center.Y : center.X;
                drawingSession.FillCircle(center, radius, Color.FromArgb(255,255,255,255));
            }
            return compositor.CreateSurfaceBrush(surface);
        }
    }
}
