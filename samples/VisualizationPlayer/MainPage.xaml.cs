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
        public PlayerData Data
        {
            get => App.Data;
        }

        public MainPage()
        {
            this.InitializeComponent();
        }


        private async void SourceFromMic_Click(object sender, RoutedEventArgs e)
        {
            await Data.CreateSourceFromMicrophoneAsync();
        }

        private async void SourceFromLoopback_Click(object sender, RoutedEventArgs e)
        {
            await Data.CreateSourceFromLoopbackAsync();
        }

        private async void SourceFromFile_Click(object sender, RoutedEventArgs e)
        {
            await Data.CreateSourceFromFileAsync();
        }

        private void NavigationView_SelectionChanged(Microsoft.UI.Xaml.Controls.NavigationView sender, Microsoft.UI.Xaml.Controls.NavigationViewSelectionChangedEventArgs args)
        {
            if (!args.IsSettingsSelected)
            {
                var item = args.SelectedItem as FrameworkElement;
                var pageType = Type.GetType((string) item.Tag);
                contentFrame.Navigate(pageType);
            }
        }
    }
}
