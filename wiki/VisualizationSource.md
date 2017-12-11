# IVisualizationSource
Interface that will be implemented by objects that can provide analyzed data frames on audio streams
## Methods
### GetData()
Calling this method will return [VisualizationDataFrame](VisualizationDataFrame.md) for the currently rendering audio
##### Returns
**[VisualizationDataFrame](VisualizationDataFrame.md)**
#### Notes
This method can return null if the stream is in stopped state or the analyzer is catching up with stream change (for example seek)
#### Example
    var data = source.GetData();
	if (data != null)
	{
		// Render visualization
	}
## Properties
### AnalyzerTypes (AnalyzerType)
This is a combination of flags that will configure the type of analyzes that will be run
### Example
    source.AnalyzerType = AnalyzerType.RMS | AnalyzerType.Peak;
### Fps (float)
Indicates the frame rate of analyzer, this is currently fixed to 60 frames per second
### IsSuspended (bool)
Gets or sets the supended state of the analyzer. If analyzer is suspended then the calculations will stop to run,
however the already calculated and buffered results will stay in buffer and can be retrieved. You can use this property
to suspend the analyzer if current UI navigates away from the page that contains visualizations.
### PlaybackState (SourcePlaybackState)
You can use this property to determine if the stream is playing or is paused. If stream is paused then GetData 
will continue to return data frame for the current position, you might want to use this property to suspend drawing visualization 
if playback is paused
### PresentationTime (TimeSpan?)
This property if set will indicate current playback position. You can use this to display play time or other visualization calculations

[Home](AudioVisualizer.md)