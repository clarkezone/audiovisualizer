using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;
using Windows.Media.Playback;
using Windows.Storage;
using Windows.Media.Core;
using Windows.Storage.FileProperties;
using System.ComponentModel;

namespace VisualizationPlayer
{
    public class PlayerService : INotifyPropertyChanged
    {
        public IVisualizationSource VisualizationSource { get
            {
                return _source?.Source;
            }
        }

        MediaPlayer _player;
        AudioVisualizer.PlaybackSource _source;
        public PlayerService()
        {
            _player = new MediaPlayer();
            _source = new PlaybackSource(_player);
            _source.SourceChanged += _source_SourceChanged;
            _player.MediaOpened += Player_MediaOpened;
            _player.PlaybackSession.PositionChanged += PlaybackSession_PositionChanged;
        }

        private void _source_SourceChanged(object sender, IVisualizationSource source)
        {
            // ((ISpectralAnalyzerSettings)source).ConfigureSpectrum(4096, 0.5f);
            VisualizationSourceChanged?.Invoke(sender, source);
        }

        private void PlaybackSession_PositionChanged(MediaPlaybackSession sender, object args)
        {
            PositionChanged?.Invoke(sender, _player.PlaybackSession.Position);
        }

        public void OpenFile(StorageFile file)
        {
            _player.Source = MediaSource.CreateFromStorageFile(file);
            GetMusicInfoAsync(file);
        }

        async void GetMusicInfoAsync(StorageFile file)
        {
            MusicProperties musicProps = await file.Properties.GetMusicPropertiesAsync();
            Title = musicProps.Title;
            Artist = musicProps.Artist;
        }

        private string _artist;

        public string Artist
        {
            get { return _artist; }
            set {
                _artist = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Artist"));
            }
        }


        private string m_Title;

        public string Title
        {
            get { return m_Title; }
            set {
                m_Title = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Title"));
            }
        }


        public void Play()
        {
            _player.Play();
        }

        private void Player_MediaOpened(MediaPlayer sender, object args)
        {
            MediaOpened?.Invoke(sender, args);
        }

        public TimeSpan MediaDuration
        {
            get
            {
                if (_player?.PlaybackSession == null)
                    return TimeSpan.Zero;

                 return _player.PlaybackSession.NaturalDuration;
            }
        }


        public event EventHandler<object> MediaOpened;
        public event EventHandler<TimeSpan> PositionChanged;
        public event EventHandler<IVisualizationSource> VisualizationSourceChanged;
        public event PropertyChangedEventHandler PropertyChanged;

        internal void Pause()
        {
            _player.Pause();
        }

        internal void Seek(TimeSpan timeSpan)
        {
            _player.PlaybackSession.Position = timeSpan;
        }
    }
}
