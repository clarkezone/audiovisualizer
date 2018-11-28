# AnalogVUMeter
Mimics the appearance and function of an analog meter. The analog meter consist of a rectangular area with an arc shaped scale with labels and a dial that indicates input RMS value. The meter itself does not apply any physics to the dial movement (like inertia). You can achieve that by feeding the visualization data through [SourceConverter](SourceConverter.md). This control accepts both linear and logarithmic scale data.
The metrics of the meter are defined by values relative to the meter visible area. So (0.5,0.5) would be the midpoint of the meter and (0.5,1.1) would be horizontal midpoint and 1/10 of the height off the bottom edge.

![Analog Meter](analogvumeter.png)

## Properties
### ChannelIndex (Int32)
Set this to audio channel index you want this meter to display.
### Source (IVisualizationSource)
Points to the data visualization source
### DialColor (Windows.UI.Color)
The color of the dial. Default is red.
### DialRelativeLength (Float)
This is the relative length of dial from the DialRelativeFixPoint. Default value is 1.08
### DialRelativeFixPoint (Windows.Foundation.Numerics.Vector2)
Indicates in relative coordinates the fixture point of the dial. This is the point around which the dial rotates to indicate a value. Coordinates are relative to the control size. The default value is (0.5,1.5)
### DialShadowOpacity (Float)
Specifies the opacity of the dial shadow. Default value is 0.5
### DialShadowBlurRadius (Float)
Blur radius of the dial shadow. Default value is 5.0
### DialShadowOffset (Windows.Foundation.Numerics.Vector3)
Dial shadow offset. The default value is (-3,-3,-10)
### DialShadowColor (Windows.UI.Color)
The color of the dial shadow. Default color is black
### DialWidth (float)
The width of the line representing dial.
### ScaleRelativeStartingPoint (Windows.Foundation.Numerics.Vector2)
Indicates the starting point of the scale in relative coordinates. The default value is (0.1,0.6)
### ScaleSections ([MeterScaleSection](MeterScaleSection.md) [])
Array of scale section definitions. Different sections can have a different line color and width. Default meter has two sections - one covering 2/3 of the scale, black color line, width 3.0, second one the last 1/3, red color and width 6.0.
### ScaleLabels ([MeterScaleLabel](MeterScaleLabel.md) [])
Array of scale labels.
Default value contains black text labels -20,-10,-7,-5,-3,-1 and 0 and red text labels +1,+3

[Home](AudioVisualizer.md)
