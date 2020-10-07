using AudioVisualizer;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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
    public sealed partial class BarMeterPage : Page
    {
        public PlayerData Data
        {
            get => App.Data;
        }

        public SourceConverter MeterSource;

        public BarMeterPage()
        {
            this.InitializeComponent();

            MeterSource = new SourceConverter();
            MeterSource.RmsRiseTime = TimeSpan.FromMilliseconds(50);
            MeterSource.RmsFallTime = TimeSpan.FromMilliseconds(50);
            MeterSource.PeakRiseTime = TimeSpan.FromMilliseconds(50);
            MeterSource.PeakFallTime = TimeSpan.FromMilliseconds(1000);
            const int fromDb = -86;
            const int toDb = 6;
            MeterBarLevel[] levels = new MeterBarLevel[toDb - fromDb];
            Color fromColor = Colors.Yellow;
            Color toColor = Colors.Red;
            float redStep = (float)toColor.R - (float)fromColor.R;
            float greenStep = (float)toColor.G - (float)fromColor.G;
            float blueStep = (float)toColor.B - (float)fromColor.B;

            for (int i = 0; i < levels.Count(); i++)
            {
                float ratio = (float)i / (float)levels.Count();
                levels[i].Color = Color.FromArgb(255, (byte)((redStep * ratio) + fromColor.R), (byte)((greenStep * ratio) + fromColor.G), (byte)((blueStep * ratio) + fromColor.B));
                levels[i].Level = i + fromDb;
            }
            bar0.Levels = levels;
            bar1.Levels = levels;
            MeterSource.Source = Data.Source;
            Data.PropertyChanged += Data_PropertyChanged;
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
