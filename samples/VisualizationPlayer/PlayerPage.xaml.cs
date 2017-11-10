using Microsoft.Graphics.Canvas;
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
    public sealed partial class PlayerPage : Page
    {
        public PlayerService Player { get { return App.Player; } }

        public PlayerPage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            if (App.Player.VisualizationSource != null)
            {
                App.Player.VisualizationSource.IsSuspended = false;
                ledbar.Source = App.Player.VisualizationSource;
                App.Player.VisualizationSourceChanged += Player_VisualizationSourceChanged;
            }

        }

        private void Player_VisualizationSourceChanged(object sender, AudioVisualizer.IVisualizationSource source)
        {
            ledbar.Source = source;
        }

        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            base.OnNavigatingFrom(e);
            if (App.Player.VisualizationSource != null)
            {
                App.Player.VisualizationSource.IsSuspended = true;
            }
        }

        private void spectrum_CreateResources(object sender, AudioVisualizer.CreateResourcesEventArgs args)
        {
            System.Diagnostics.Debug.WriteLine($"Create resources - reason: {args.Reason}");
        }

        private void spectrum_Draw(AudioVisualizer.IVisualizer sender, AudioVisualizer.VisualizerDrawEventArgs args)
        {
        }
    }
}
