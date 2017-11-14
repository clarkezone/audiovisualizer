# Build status
 

## Build status

| Target | Branch | Status | Recommended Nuget packages version |
| ------ | ------ | ------ | ------ |
| Current Release | master | [![Build status](https://ci.appveyor.com/api/projects/status/p4a7jmpa9391bxu8/branch/master?svg=true)](https://ci.appveyor.com/api/projects/status/p4a7jmpa9391bxu8/branch/master?svg=true) | [![NuGet](https://img.shields.io/nuget/v/UWPAudioVisualizer.svg)](https://www.nuget.org/packages/UWPAudioVisualizer/) |
| Pre-release | dev | [![Build status](https://ci.appveyor.com/api/projects/status/p4a7jmpa9391bxu8?svg=true)](https://ci.appveyor.com/project/clarkezone/audiovisualizer) | |

# Introduction

The AudioAnalyzer UWP extension DLL contains a component that can provide realtime audio analysis information for visualization and other purposes. Library does contain also prebuilt controls implementing VU meters and spectrum analyzer and a control that has a custom draw capability

# Getting Started
TODO: Guide users through getting your code up and running on their own system. In this section you can talk about:
1.	Installation process
2.	Software dependencies
3.	Latest releases
4.	API references


#Getting started
## Installing the library
Download and install the AudioAnalyzer nuget package, for convenient use add namespace statement to your C# code as:

using AudioVisualizer;

add namespace statement to your XAML code as:
<Page ...
    xmlns:a="using:AudioVisualizer" ...>
	...
	<a:CustomVisualizer/>
	...
</Page>

##Initializing the source
First you need to create the analyzer object, that implements IVisualizationSource interface which is basis to retrieve audio data.

To use visualizations with MediaPlayer or MediaElement you need to use PlaybackSource class. Both MediaPlayer and MediaElement use Media Foundation pipeline to render media. Pipeline will re-create the Media Foundation Tranform object that does the analysis every time you open a new source. That means that the instance of IVisualizationSource will change for every new media that you open in MediaPlayer.

PlaybackSource monitors media pipeline and get's the new visualization when it is created and raises an event. You should use this event to get the new visualization source and set the source for all the visualization controls you have active. This is also a good place to configure the analyzer if you wish - for more on this please see Configuring the Analyzer.
If the player is a global object it is also good idea to set the current visualization source in OnLoad or OnNavigatedTo type of an event/overload.

    class VisualizationPage
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

#API Reference
## ArrayData class
ArrayData class is used to process spectrum data. The instance of this class is created by the library and you rarely want to create your own.

You access data as you would in an array
    ArrayData a1;
    float third_value_channel_0 = a1[0][3];


# Build and Test
TODO: Describe and show how to build your code and run the tests. 

# Contribute
TODO: Explain how other users and developers can contribute to make your code better. 

If you want to learn more about creating good readme files then refer the following [guidelines](https://www.visualstudio.com/en-us/docs/git/create-a-readme). You can also seek inspiration from the below readme files:
- [ASP.NET Core](https://github.com/aspnet/Home)
- [Visual Studio Code](https://github.com/Microsoft/vscode)
- [Chakra Core](https://github.com/Microsoft/ChakraCore)
