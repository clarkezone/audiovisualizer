# PlaybackSource
Playback source is a class to create and keep track of visualization data source changes.

## Interfaces
IPlaybackSource
## Methods
### static PlaybackSource CreateFromMediaPlayer(Windows.Media.Playback.MediaPlayer)
Creates a playback source object from media player. Media player source will not be created until the player starts playing and opening another source will cause a new source created. Thus you need to track the SourceChanged event to set the visualization controls source property
### static PlaybackSource CreateFromAudioNode(Windows.Media.Audio.IAudioNode)
Creates a playback source object from audio graph node. The source property in this case will be available right away

#### Source (IVisualizationSource)
Current visualization source object. Use this to obtain visualization data
### See also
[IVisualizationSource](VisualizationSource.md)
## Events
### SourceChanged
This event will be fired if the Source property changes. This will happen if Media Player starts playing or opens a new source.
#### Parameters
##### sender (object)
Sender of the event
##### source (IVisualizationSource)
Visualization source of the playback object


[Home](AudioVisualizer.md)