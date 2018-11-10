# AudioVisualizer
AudioVisualizer is a library that makes it easy to visualize audio signals. The analyzer works with MediaPlayer and Audio Graph API nodes and can be also used individually. As a visual output there are Win2D based CustomVisualizer control and analog and digital volume and spectrum displays.

## Analyzers
Audio signal analyzer is encapsulated in [AudioAnalyzer](AudioAnalyzer.md) class, which can be used on it's own. You supply audio data in [AudioFrame](https://docs.microsoft.com/en-us/uwp/api/windows.media.audioframe) objects by calling ProcessInput. The analyzer will calculate the RMS and peak values for the audio signal and also perform [spectral analysis](Spectrum.md). The analyzer can both oeprate in async and sync mode, in both cases when output data is available an event is raised.

## Visualization Data Sources
You can insert the audio analyzer in the processing path of MediaPlayer or Audio Graph through the [PlaybackSource](PlaybackSource.md) class. Use the static methods of CreateFromMediaPlayer or CreateFromAudioNode to create the object. [PlaybackSource](PlaybackSource.md) has one Source property that can be used as visualization data source for all the built in controls. [PlaybackSource](PlaybackSource.md) also has one event SourceChanged which is called when the source has changed. The source is of type IVisualizationSource and calling GetData on that interface will return the most current data frame for processing and visualization.
The sources generated from MediaPlayer and AudioGraph behave a bit differently. First the source generated from MediaPlayer will be created only after you open media, so immediately after returning a call from CreateFromMediaPlayer the Source property will likely be null and you will recieve SourceChanged event after opening the media. Likewise after opening new media file new SourceChanged event will be generated with new source. The frames returned from the source will be based on and synchronized with the current play position.
The source generated from AudioGraph will be available immediately and will be valid throughout the lifetime of the audio node. As the graph api is realtime api the frames returned are not buffered in any way but returned based on the data passed through.
## Visualization Data
The data is packed into frames and presented through VisualizationDataFrame[VisualizationDataFrame.md] objects. It has properties of Time (which can be used together with CustomVisualizer to display play position), RMS (root mean square), Peak and Spectrum. RMS and Peak are both of ScalarData type and the data can be accessed through normal array subscript for different audio channels. SpectrumData data can be also accessed through array subscript but the array is two dimensional. Both data classes have other utility methods for data converstions and properties.
VisualizationDataFrame also has a property called Properties, which is a PropertySet that can be used for future extensions in other type of analyzers.

## Data processing
The shape of the data generated from the source like MediaPlayer and AudioGraph is based on analyzer configuration and one might want to process and convert the data before supplying it to the controls or using it for visualizations. You can switch certain analyzes on/off to save processing power and set some parameters for spectral analyzes. In order to make it easy to configure and shape visualization data you can use the [SourceConverter](SourceConverter.md). You can
- Combine or split audio channel data
- Apply rise and fall times to signals
- Convert the spectrum data into different sizes
- Convert from linear scales to logarithmic
SourceConverter itself implements IVisualizationSource interface so it can be used as a data source for visualization controls. It will also perform data caching so you can hook up several controls to one source without the fear of performance hit.

## Visualizers
[CustomVisualizer](CustomVisualizer.md) is a Win2D based control that has a Draw event which supplies both Win2D drawing session and visualization data. [AnalogVUMeter](AnalogVUMeter.md) is a VU meter which looks like classical analog meter with a dial and scale. [DigitalVUBar](DigitalVUBar.md) and [SpectrumVisualizer](SpectrumVisualizer.md) are both stacked bar type visualizer, the first one displaying the amplitude of the signal and the second one the spectrum. All visualizers are adapting to the size of the screen and get their configuration (like the number of bars displayed for spectrum) from the source data.

## Classes
- [AnalogVUMeter](AnalogVUMeter.md)
- [ArrayData](ArrayData.md)
- [AudioAnalyzer](AudioAnalyzer.md)
- [CustomVisualizer](CustomVisualizer.md)
- [DiscreteVUBar](DiscreteVUBar.md)
- [PlaybackSource](PlaybackSource.md)
- [ScalarData](ScalarData.md)
- [SourceConverter](SourceConverter.md)
- [SpectrumVisualizer](spectrumVisualizer.md)
- [VisualizationDataFrame](VisualizationDataFrame.md)
- [VisualizationSource](VisualizationSource.md)
- [VisualizerDrawEventArgs](VisualizerDrawEventArgs.md)