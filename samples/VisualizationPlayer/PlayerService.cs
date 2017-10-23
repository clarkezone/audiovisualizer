using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualizer;
using Windows.Media.Playback;
using Windows.Storage;
using Windows.Media.Core;

namespace VisualizationPlayer
{
    public class PlayerService
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
            VisualizationSourceChanged?.Invoke(sender, source);
        }

        private void PlaybackSession_PositionChanged(MediaPlaybackSession sender, object args)
        {
            PositionChanged?.Invoke(sender, _player.PlaybackSession.Position);
        }

        public void OpenFile(StorageFile file)
        {
            _player.Source = MediaSource.CreateFromStorageFile(file);
        }

        public void Play()
        {
            _player.Play();
        }

        private void Player_MediaOpened(MediaPlayer sender, object args)
        {
            System.Diagnostics.Debug.WriteLine("Media opened");
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
    }
}
