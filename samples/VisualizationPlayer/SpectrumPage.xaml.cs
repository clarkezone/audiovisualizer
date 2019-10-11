using AudioVisualizer;
using Microsoft.Graphics.Canvas.UI.Composition;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.DirectX;
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
    public enum SpectrumConfigurationOptions
    {
        LinearLessFrequencies,
        LinearMoreFrequencies,
        Logarithmic,
        MusicalRange
    }
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class SpectrumPage : Page, IBarElementFactory
    {

        public PlayerData Data
        {
            get => App.Data;
        }

        public IEnumerable<SpectrumConfigurationOptions> SpectrumConfigurations {
            get => Enum.GetValues(typeof(SpectrumConfigurationOptions)).Cast<SpectrumConfigurationOptions>();
        }

        SpectrumConfigurationOptions _configuation;
        public SpectrumConfigurationOptions CurrentConfiguration
        {
            get => _configuation;
            set
            {
                if (_configuation != value)
                {
                    _configuation = value;
                    ReconfigureSpectrumView();
                }
            }
        }

        private void Data_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Source")
            {
                SpectrumSource.Source = Data.Source;
            }
        }

        void ReconfigureSpectrumView()
        {
            switch (CurrentConfiguration)
            {
                case SpectrumConfigurationOptions.LinearLessFrequencies:
                    SpectrumSource.FrequencyCount = 10;
                    SpectrumSource.FrequencyScale = ScaleType.Linear;
                    SpectrumSource.MinFrequency = 0.0f;
                    SpectrumSource.MaxFrequency = 20000.0f;
                    break;
                case SpectrumConfigurationOptions.LinearMoreFrequencies:
                    SpectrumSource.FrequencyCount = 50;
                    SpectrumSource.FrequencyScale = ScaleType.Linear;
                    SpectrumSource.MinFrequency = 0.0f;
                    SpectrumSource.MaxFrequency = 20000.0f;
                    break;
                case SpectrumConfigurationOptions.Logarithmic:
                    SpectrumSource.FrequencyCount = 20;
                    SpectrumSource.MinFrequency = 20.0f;
                    SpectrumSource.MaxFrequency = 20000.0f;
                    SpectrumSource.FrequencyScale = ScaleType.Logarithmic;
                    break;
                case SpectrumConfigurationOptions.MusicalRange:
                    // Set frequency range to piano notes
                    SpectrumSource.FrequencyCount = 88;
                    SpectrumSource.MinFrequency = 27.5f;    // A0
                    SpectrumSource.MaxFrequency = 4186.0f;  // C8
                    SpectrumSource.FrequencyScale = ScaleType.Logarithmic;
                    break;
                default:
                    throw new InvalidOperationException();
            }
        }
        SourceConverter SpectrumSource;
        public SpectrumPage()
        {
            this.InitializeComponent();
            SpectrumSource = new SourceConverter();
            SpectrumSource.SpectrumRiseTime = TimeSpan.FromMilliseconds(100);
            SpectrumSource.SpectrumFallTime = TimeSpan.FromMilliseconds(200);
            SpectrumSource.AnalyzerTypes = AnalyzerType.All;
            SpectrumSource.Source = Data.Source;
            SpectrumSource.ConfigurationChanged += SpectrumSource_ConfigurationChanged;
            // barSpectrum.ElementFactory = this;
            barSpectrum.ElementShadowBlurRadius = 5;
            barSpectrum.ElementShadowOffset = new Vector3(2, 2, -10);
            barSpectrum.ElementShadowColor = Colors.LightGreen;
            Data.PropertyChanged += Data_PropertyChanged;
            ReconfigureSpectrumView();

        }

        private void SpectrumSource_ConfigurationChanged(IVisualizationSource sender, string args)
        {
            lowFrequency.Text = $"Minimum frequency: {sender.ActualMinFrequency}Hz";
            highFrequency.Text = $"Maximum frequency: { sender.ActualMaxFrequency}Hz";
            frequencyStep.Text = $"Frequency step: {sender.ActualFrequencyStep}";
            frequencyCount.Text= $"Frequency count: {sender.ActualFrequencyCount}";
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
                drawingSession.FillCircle(center, radius, elementColor);
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
                drawingSession.FillCircle(center, radius, Color.FromArgb(255, 255, 255, 255));
            }
            return compositor.CreateSurfaceBrush(surface);
        }

    }
}
