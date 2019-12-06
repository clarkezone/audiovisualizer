using AudioVisualizer;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.Core;
using Windows.Media.Playback;

namespace VisualizationPlayer
{
    public class FileSource : IAudioSource, IDisposable
    {
        MediaPlayer _player;
        PlaybackSource _playbackSource;
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

        public FileSource(Windows.Storage.StorageFile file)
        {
            _player = new MediaPlayer();
            _player.AutoPlay = true;
            _playbackSource = PlaybackSource.CreateFromMediaPlayer(_player);
            _playbackSource.SourceChanged += PlaybackSource_SourceChanged;
            _player.Source = MediaSource.CreateFromStorageFile(file);
        }

        private void PlaybackSource_SourceChanged(object sender, IVisualizationSource source)
        {
            Source = source;
        }

        protected void OnPropertyChanged([CallerMemberName] string name = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        public void Dispose()
        {
            _player?.Pause();
            _player?.Dispose();
            _player = null;
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
