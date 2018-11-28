# CustomVisualizer
Win2D based visualizer that generates events draw events for each display frame. Audio visualization data is pulled automatically from the source.

(placeholder to be edited)

		IVisualizationSource Source;
		event Windows.Foundation.TypedEventHandler<IInspectable,VisualizerDrawEventArgs> Draw;
		event Windows.Foundation.TypedEventHandler<IInspectable,CreateResourcesEventArgs> CreateResources;


## Properties
### Source (IVisualizationSource)
The data source where from to get the visualization data

## Events
### Draw 
#### TypedEventHandler<IInspectable,VisualizerDrawEventArgs>
This event is raised to draw every frame if the visualization. Visualization data and other properties as passed in through the [VisualizerDrawEventArgs](VisualizerDrawEventArgs.md).
### CreateResources
#### TypedEventHandler<IInspectable,CreateResourcesEventArgs>
Allows to create resources for the draw event.

[Home](AudioVisualizer.md)