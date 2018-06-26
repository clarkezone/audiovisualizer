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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace VisualizationPlayer
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class SpectrumPage : Page
    {
        private AudioVisualizer.SourceConverter _converterSource;

        public SpectrumPage()
        {
            this.InitializeComponent();
            _converterSource = new AudioVisualizer.SourceConverter();
            _converterSource.FrequencyCount = 50;
            _converterSource.FrequencyScale = AudioVisualizer.ScaleType.Logarithmic;
            _converterSource.MinFrequency = 20.0f;
            _converterSource.MaxFrequency = 20000.0f;
            _converterSource.SpectrumRiseTime = TimeSpan.FromMilliseconds(50);
            _converterSource.SpectrumFallTime = TimeSpan.FromMilliseconds(100);
            _converterSource.PeakRiseTime = TimeSpan.FromMilliseconds(50);
            _converterSource.PeakFallTime = TimeSpan.FromMilliseconds(1000);
            _converterSource.RmsRiseTime = TimeSpan.FromMilliseconds(100);
            _converterSource.RmsFallTime = TimeSpan.FromMilliseconds(100);
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            spectrum.Source = _converterSource;
            leftVU.Source = _converterSource;
            rightVU.Source = _converterSource;
            leftVU.ChannelIndex = 0;
            rightVU.ChannelIndex = 1;
            /*
            if (App.Player.VisualizationSource != null)
            {
                App.Player.VisualizationSource.IsSuspended = false;
                _converterSource.Source = App.Player.VisualizationSource;
            }
            App.Player.VisualizationSourceChanged += Player_VisualizationSourceChanged;
            */
        }

        private void Player_VisualizationSourceChanged(object sender, AudioVisualizer.IVisualizationSource source)
        {
            _converterSource.Source = source;
        }

        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            base.OnNavigatingFrom(e);
            /*
            if (App.Player.VisualizationSource != null)
            {
                App.Player.VisualizationSource.IsSuspended = true;
            }*/
        }

    }
}
