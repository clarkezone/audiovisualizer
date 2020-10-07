﻿using AudioVisualizer;
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
using Windows.UI;
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
    public sealed partial class CustomDrawPage : Page
    {
        public PlayerData Data
        {
            get => App.Data;
        }

        public SourceConverter SpectrumSource;

        private void Data_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Source")
            {
                SpectrumSource.Source = Data.Source;
            }
        }
        public CustomDrawPage()
        {
            this.InitializeComponent();
            SpectrumSource = new SourceConverter();
            Data.PropertyChanged += Data_PropertyChanged;
            SpectrumSource.Source = Data.Source;
        }


        private void spectrum_CreateResources(object sender, AudioVisualizer.CreateResourcesEventArgs args)
        {
            _spectrumTextFormat = new CanvasTextFormat();
            _spectrumTextFormat.VerticalAlignment = CanvasVerticalAlignment.Center;
            _spectrumTextFormat.HorizontalAlignment = CanvasHorizontalAlignment.Center;
            _spectrumTextFormat.FontSize = 9;
        }

        const uint spectrumBarCount = 50;
        SpectrumData _emptySpectrum = SpectrumData.CreateEmpty(2, spectrumBarCount, ScaleType.Linear, ScaleType.Linear, 0, 20000);
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

            Thickness margin = new Thickness(40, 10, 20, 10);
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
                                args.Data.Spectrum.LogarithmicTransform(spectrumBarCount, 20f, 20000f) :
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
            float fStepWidth = (float)boundingRectTop.Width / 20;
            float freq = 20.0f;
            float fStep = (float)Math.Pow(1000.0, 1.0 / 20.0);
            for (int f = 0; f < 20; f++, freq *= fStep)
            {
                float X = f * fStepWidth + (float)margin.Left;
                if (f != 0)
                {
                    drawingSession.DrawLine(X, (float)boundingRectTop.Top, X, (float)boundingRectTop.Bottom, gridlineColor);
                    drawingSession.DrawLine(X, (float)boundingRectBottom.Top, X, (float)boundingRectBottom.Bottom, gridlineColor);
                }
                string freqText = freq < 100.0f ? $"{freq:F0}" :
                                    freq < 1000.0f ? $"{10 * Math.Round(freq / 10):F0}" :
                                    freq < 10000.0f ? $"{freq / 1e3:F1}k" : $"{freq / 1e3:F0}k";
                drawingSession.DrawText(freqText, X + fStepWidth / 2, (float)boundingRectTop.Bottom + 10, textColor, _spectrumTextFormat);
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
    }
}
