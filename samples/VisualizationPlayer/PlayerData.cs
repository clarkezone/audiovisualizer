using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Controls;
using Windows.Storage.Pickers;
using System.Linq;
using AudioVisualizer;

namespace VisualizationPlayer
{

    public class PlayerData : INotifyPropertyChanged
    {
        private Symbol _sourceTypeSymbol = Symbol.More;

        public Symbol SourceTypeSymbol { 
            get => _sourceTypeSymbol; 
            set
            {
                _sourceTypeSymbol = value;
                OnPropertyChanged();
            } 
        }

        private IVisualizationSource source;

        internal IVisualizationSource Source 
        { 
            get => source; 
            set
            {
                source = value;
                OnPropertyChanged();
            } 
        }

        private string _title;

        public string Title { 
            get => _title; 
            set { _title = value; OnPropertyChanged();  } 
        }

        private IAudioSource _audioSource;

        protected IAudioSource AudioSource
        {
            get => _audioSource;
            set
            {
                if (_audioSource != value)
                {
                    if (_audioSource != null)
                    {
                        _audioSource.PropertyChanged -= AudioSource_PropertyChanged;
                        (_audioSource as IDisposable)?.Dispose();
                        _audioSource = null;
                    }
                    _audioSource = value;
                    _audioSource.PropertyChanged += AudioSource_PropertyChanged;
                    Source = AudioSource.Source;
                }
            }
        }

        private void AudioSource_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Source")
            {
                Source = AudioSource.Source;
            }
        }

        protected void OnPropertyChanged([CallerMemberName] string name = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
        public event PropertyChangedEventHandler PropertyChanged;

        internal async Task CreateSourceFromMicrophoneAsync()
        {
            AudioSource = await MicrophoneSource.CreateAsync();
            Title = "Microphone";
            SourceTypeSymbol = Symbol.Microphone;

        }

        internal async Task CreateSourceFromLoopbackAsync()
        {
            AudioSource = await LoopbackSource.CreateAsync();
            Title = "Audio output";
            SourceTypeSymbol = Symbol.Volume;
        }

        internal async Task CreateSourceFromFileAsync()
        {
            FileOpenPicker openPicker = new FileOpenPicker();
            var extensions = new string[] { ".mp3", ".wma", ".wav", ".mp4" };
            foreach (var filter in extensions)
            {
                openPicker.FileTypeFilter.Add(filter);
            }
            openPicker.SuggestedStartLocation = PickerLocationId.MusicLibrary;
            var file = await openPicker.PickSingleFileAsync();
            if (file != null)
            {
                Title = file.DisplayName;
                AudioSource = new FileSource(file);
                SourceTypeSymbol = Symbol.OpenFile;
            }
        }
    }
}
