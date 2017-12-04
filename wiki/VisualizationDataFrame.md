# VisualizationDataFrame
Encapsulates visualization data returned by the visualizer
## Interfaces
IVisualizationDataFrame
## Properties
### Duration (TimeSpan?)
If set contains the length of data that was used to build the visualization. For example for 60fps analyzes this will equal to 16.7ms
### Peak (ScalarData)
Contains the peak value of the input signal for the time period. Peak is measured as the maximum absolute value of any sample in the set
### RMS (ScalarData)
Contains the calculated RMS value of the input signal.
### Spectrum (SpectrumData)
Contains the spectrum data of the input signal. This is calculated by windowing overlapping input 
frames and applying FFT to it. The number of elements in the spectrum will be half of the FFT elements.
### Time (TimeSpan?)
If set will indicate the presentation time for the data

#### Remarks

#### See also
[Link](Link)
