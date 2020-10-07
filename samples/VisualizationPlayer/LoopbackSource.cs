using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;

namespace VisualizationPlayer
{
    public class LoopbackSource : IAudioSource, IDisposable
    {
        private IVisualizationSource _source;
        public IVisualizationSource Source
        {
            get => _source;
            private set
            {
                _source = value;
                OnPropertyChanged();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected void OnPropertyChanged([CallerMemberName] string name = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        public void Dispose()
        {
        }

        private LoopbackSource(LoopbackVisualizationSource source)
        {
            Source = source;
            source.IsSuspended = false;
        }
        public static async Task<LoopbackSource> CreateAsync()
        {
            var source = await LoopbackVisualizationSource.CreateAsync();
            return new LoopbackSource(source);
        }
    }
}
