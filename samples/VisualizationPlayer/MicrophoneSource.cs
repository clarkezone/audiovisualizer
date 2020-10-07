using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;
using Windows.Media.Audio;

namespace VisualizationPlayer
{
    public class MicrophoneSource : IAudioSource, IDisposable
    {
        IVisualizationSource _source;

        public IVisualizationSource Source
        {
            get => _source;
            private set
            {
                _source = value;
                OnPropertyChanged();
            }
        }
        protected void OnPropertyChanged([CallerMemberName] string name = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }



        public event PropertyChangedEventHandler PropertyChanged;

        AudioGraph _graph;
        AudioDeviceInputNode _inputNode;

        public static async Task<MicrophoneSource> CreateAsync()
        {
            var graphResult = await AudioGraph.CreateAsync(new AudioGraphSettings(Windows.Media.Render.AudioRenderCategory.Media));
            if (graphResult.Status != AudioGraphCreationStatus.Success)
                throw new InvalidOperationException($"Graph creation failed {graphResult.Status}");
            var graph = graphResult.Graph;
            var inputNodeResult = await graph.CreateDeviceInputNodeAsync(Windows.Media.Capture.MediaCategory.Media);

            if (inputNodeResult.Status == AudioDeviceNodeCreationStatus.Success)
            {

                return new MicrophoneSource(graph, inputNodeResult.DeviceInputNode);
            }
            else
                throw new Exception($"Failed to create input node: {inputNodeResult.Status}");
        }

        public void Dispose()
        {
            _graph?.Stop();
            _inputNode?.Dispose();
            _graph?.Dispose();
        }

        private MicrophoneSource(AudioGraph graph,AudioDeviceInputNode inputNode)
        {
            _graph = graph;
            _inputNode = inputNode;
            Source = AudioVisualizer.PlaybackSource.CreateFromAudioNode(_inputNode).Source;
            _graph.Start();
        }
    }
}
