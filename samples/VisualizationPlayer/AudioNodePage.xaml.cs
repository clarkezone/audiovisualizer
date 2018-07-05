using AudioVisualizer;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Media.Audio;
using Windows.System.Threading;
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
    public sealed partial class AudioNodePage : Page
    {
        public AudioNodePage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            CreateAudioGraphAsync();
        }
        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);
            _graph?.Stop();
            _graph?.Dispose();
            _graph = null;
        }

        AudioGraph _graph;
        AudioDeviceInputNode _inputNode;
        PlaybackSource _source;
        SourceConverter _converter;

        async void CreateAudioGraphAsync()
        {
            var graphResult = await AudioGraph.CreateAsync(new AudioGraphSettings(Windows.Media.Render.AudioRenderCategory.Media));
            if (graphResult.Status != AudioGraphCreationStatus.Success)
                throw new InvalidOperationException($"Graph creation failed {graphResult.Status}");
            _graph = graphResult.Graph;
            var inputNodeResult = await _graph.CreateDeviceInputNodeAsync(Windows.Media.Capture.MediaCategory.Media);
            if (inputNodeResult.Status != AudioDeviceNodeCreationStatus.Success)
                throw new InvalidOperationException($"Input node creation failed {inputNodeResult.Status}");

            _inputNode = inputNodeResult.DeviceInputNode;

            _source = AudioVisualizer.PlaybackSource.CreateFromAudioNode(_inputNode);
            _converter = new SourceConverter();
            _converter.Source = _source.Source;
            _converter.MinFrequency = 110.0f;    // Note A2
            _converter.MaxFrequency = 3520.0f;  // Note A7
            _converter.FrequencyCount = 12 * 5 * 5; // 5 octaves, 5 bars per note
            _converter.FrequencyScale = ScaleType.Logarithmic;
            _converter.SpectrumRiseTime = TimeSpan.FromMilliseconds(20);
            _converter.SpectrumFallTime = TimeSpan.FromMilliseconds(200);
            _converter.RmsRiseTime = TimeSpan.FromMilliseconds(20); // Use RMS to gate noise, fast rise slow fall
            _converter.RmsFallTime = TimeSpan.FromMilliseconds(500);
            _converter.ChannelCount = 1;
            notesSpectrum.Source = _converter;

            _graph.Start();
        }
    }
}
