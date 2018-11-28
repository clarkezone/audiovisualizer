# DiscreteVUBar
Stacked bar type visualizer taking input from the RMS and Peak values of the input data.

## Properties
### DisplayPeak (Boolean)
Selects whether to display peak values on the bar. Peak values are displayed as a separate lit element above the RMS value.
### Source (IVisualizationSource)
The visualization data source this control pulls data from.
### Levels ([MeterBarLevel](MeterBarLevel.md) [])
Array of values indicating the number of bar elements and their threshold levels and colors.
### Orientation (Windows.UI.Xaml.Controls.Orientation)
Set the bar to be either horizontal or vertical
### ChannelIndex (Int32)
Sets the audio channel of the visualization data
### RelativeElementMargin (Windows.UI.Xaml.Thickness)
This property will allow setting the spacing for elements. During layout the space between elements (number of which is defined by Levels property) is divided equally. Setting RelativeElementMargin will allow to set margins for individual elements (in relative coordinates). For example Thickness(0.1,0.1,0.1,0.1) will leave spacing 1/10 of the cell width and height on each side.
### UnlitElement (Windows.UI.Color)
Defines the color of an unlit element
### ElementShadowOffset (Windows.Foundation.Numerics.Vector3)
Offset of a shadow from the element
### ElementShadowColor (Windows.UI.Color)
Sets the color of the shadow that the element is casting
### ElementShadowBlurRadius (float)
Set the element shadow blur radius
### ElementFactory (IBarElementFactory)
By default the bar elements are rectangles. You can define your own shape by creating a class that implements [IBarElementFactory](IBarElementFactory.md) and setting ElementFactory property to the instance of that class.


[Home](AudioVisualizer.md)