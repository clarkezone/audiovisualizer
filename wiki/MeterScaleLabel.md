# MeterScaleLabel
A struct defining a label on [analog meter](AnalogVUMeter.md) scale.
## Fields
### Label (String)
Text that is shown next to the tick on the scale. The label is aligned to the center of the line that connects label position and dial fix point.
### Position (Float)
Specifies in relative units label placement on the scale - 0.0 stands for the start of the scale arc, 1.0 stands for the end of the scale arc.
### TickColor (Windows.UI.Color)
The color of the tick and label
### TickLength (Float)
Length of the tick in screen units
### TickThickness (Float)
The width of the tick line in screen units

[Home](AudioVisualizer.md)