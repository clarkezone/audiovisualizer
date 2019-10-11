using AudioVisualizer;
using Microsoft.Graphics.Canvas;
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

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace VisualizationPlayer
{
    public sealed partial class PlayerControlBar : UserControl
    {
        PlayerData _data;

        public PlayerData Data
        {
            get => _data;
            set
            {
                if (_data != null)
                {
                    Data.PropertyChanged -= Data_PropertyChanged;
                }
                _data = value;
                positionDisplay.Source = Data?.Source;
                Data.PropertyChanged += Data_PropertyChanged;
            }
        }

        public PlayerControlBar()
        {
            this.InitializeComponent();
        }

        private void Data_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Source")
            {
                positionDisplay.Source = App.Data.Source;
            }
        }

        private void CustomVisualizer_Draw(object sender, AudioVisualizer.VisualizerDrawEventArgs args)
        {
            TimeSpan position = TimeSpan.Zero;
            if (args.PresentationTime != null)
            {
                position = args.PresentationTime.Value;
            }
            string timeString = position.ToString("mm\\:ss\\.ff");
            var ds = (CanvasDrawingSession)args.DrawingSession;
            ds.DrawText(timeString, 0, 0, Colors.Gray);
        }
    }
}
