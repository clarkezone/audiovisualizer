# PlaybackSource
Playback source is a class that helps track visualization source changes
## Interfaces
IPlaybackSource
## Methods
### PlaybackSource(MediaPlayer player)
Method 
#### Parameters
##### player (MediaPlayer)
MediaPlayer object for which playback the object is contructed
#### Examples
    var player = new MediaPlayer();
    var source = new PlaybackSource(player);
    source.SourceChanged += source_SourceChanged;
#### Source (IVisualizationSource)
Current visualization source object. Use this to obtain visualization data
### See also
[IVisualizationSource](VisualizationSource.md)
## Events
### SourceChanged
Description
#### Parameters
##### sender (object)
Sender of the event
##### source (IVisualizationSource)
Current visualization source of the playback object

[Home](AudioVisualizer.md)