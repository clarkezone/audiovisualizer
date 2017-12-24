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
        public PlayerService Player { get { return App.Player; } }

        public MainPage()
        {
            this.InitializeComponent();
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
                App.Player.OpenFile(file);
                App.Player.Play();
            }
        }


        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            App.Player.MediaOpened += Player_MediaOpened;
            App.Player.PositionChanged += Player_PositionChanged;
            App.Player.VisualizationSourceChanged += Player_VisualizationSourceChanged;
            PositionDisplay.Source = App.Player.VisualizationSource;
            navView.SelectedItem = navView.MenuItems[0];
        }

        private void Player_VisualizationSourceChanged(object sender, IVisualizationSource visualizationSource)
        {
            PositionDisplay.Source = visualizationSource;
        }

        private void Player_VisualizationSourceCreated(object sender, IVisualizationSource source)
        {
            PositionDisplay.Source = source;
        }

        bool _insidePositionUpdate = false;

        private async void Player_PositionChanged(object sender, TimeSpan position)
        {
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    _insidePositionUpdate = true;
                    seekSlider.Value = position.TotalSeconds;
                    _insidePositionUpdate = false;
                });
        }

        private async void Player_MediaOpened(object sender, object e)
        {
            await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    seekSlider.Maximum = App.Player.MediaDuration.TotalSeconds;
                });
        }


        private void PositionDisplay_Draw(IVisualizer sender, VisualizerDrawEventArgs args)
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
            App.Player.Play();
        }

        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            App.Player.Pause();
        }

        private void seekSlider_ValueChanged(object sender, RangeBaseValueChangedEventArgs e)
        {
            if (!_insidePositionUpdate)
            {
                App.Player.Seek(TimeSpan.FromSeconds(e.NewValue));
            }
        }

        private void navView_SelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args)
        {
            if (args.IsSettingsSelected)
            {
                // not implemented
            }
            else
            {
                string typeName = (string)((NavigationViewItem)args.SelectedItem).Tag;
                Type pageType = Type.GetType(typeName);
                ContentFrame.Navigate(pageType);
            }
        }
    }
}
