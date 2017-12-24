# AudioVisualizer Library
Audiovisualizer library is meant to make it easy to create visual representation of audio. 
It works by inserting an effect into the rendering path and performing necessary calculations in real time. 
Currently the following data is available:
* [RMS] (https://en.wikipedia.org/wiki/Root_mean_square)
* Peak values
* Spectrum

##Initializing the source
First you need to create the analyzer object, that implements [IVisualizationSource](VisualizationSource.md) interface which is basis to retrieve audio data.

To use visualizations with MediaPlayer or MediaElement you need to use PlaybackSource class. Both MediaPlayer and MediaElement use Media Foundation pipeline to render media. Pipeline will re-create the Media Foundation Tranform object that does the analysis every time you open a new source. That means that the instance of IVisualizationSource will change for every new media that you open in MediaPlayer.

PlaybackSource monitors media pipeline and get's the new visualization when it is created and raises an event. You should use this event to get the new visualization source and set the source for all the visualization controls you have active. This is also a good place to configure the analyzer if you wish - for more on this please see Configuring the Analyzer.
If the player is a global object it is also good idea to set the current visualization source in OnLoad or OnNavigatedTo type of an event/overload.

    partial class VisualizationPage : Page
    {
        MediaPlayer _player;
        AudioVisualizer.PlaybackSource _source;

        public VisualizationPage()
        {
            _player = new MediaPlayer();
            _source = new PlaybackSource(_player);
            _source.SourceChanged += Playback_SourceChanged;
		}

		private void Playback_SourceChanged(object sender, IVisualizationSource source)
        {
            ((ISpectralAnalyzerSettings)source).ConfigureSpectrum(4096, 0.5f);
			vuMeter.Source = source;
			spectrumDisplay.Source = source;
			playPositionDisplay.Source = source;
        }

		private void Page_Loaded(object sender, RoutedEventArgs e)
        {
			vuMeter.Source = _source.VisualizationSource;
			spectrumDisplay.Source = _source.VisualizationSource;
			playPositionDisplay.Source = _source.VisualizationSource;
        }
   }
#Configuring the source
You can configure the analyzer by switching on and off certain data points. Switching them off when not used 
will save some CPU resources.
You can also configure the spectrum analyzer by specifying number of FFT elements and also overlap across input data 
samples used.

## Getting the data
Once you have a properly initialized [IVisualizationSource](VisualizationSource.md) it is very easy to get the 
visualization data by calling GetData(). This method will return [VisualizationDataFrame](VisualizationDataFrame.md) 
for the current audio being played or null if the current state of stream is stopped or if the analyzer is catching up processing 
for example due to seek operation.

