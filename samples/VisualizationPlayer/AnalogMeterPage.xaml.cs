using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using AudioVisualizer;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
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
    public sealed partial class AnalogMeterPage : Page
    {
        public SourceConverter MeterSource;

        public PlayerData Data
        {
            get => App.Data;
        }

        public AnalogMeterPage()
        {
            MeterSource = new SourceConverter();
            MeterSource.AnalyzerTypes = AnalyzerType.RMS;
            MeterSource.RmsRiseTime = TimeSpan.FromMilliseconds(500);
            MeterSource.RmsFallTime = TimeSpan.FromMilliseconds(500);
            MeterSource.Source = Data.Source;
            Data.PropertyChanged += Data_PropertyChanged;
            this.InitializeComponent();
        }

        private void Data_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Source")
            {
                MeterSource.Source = Data.Source;
            }
        }


    }
}
