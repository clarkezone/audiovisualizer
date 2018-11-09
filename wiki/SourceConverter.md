# SourceConverter
Source converter is a proxy data source that will allow easily to reshape the data from the analyzer.
## Interfaces
IVisualizationSource
## Methods
### SourceConverter()
Constructor 

## Properties
### AnalyzerTypes (AnalyzerType)
This is a combination of flags that will configure the type of data that will be processed available in the output
### Example
    source.AnalyzerType = AnalyzerType.RMS | AnalyzerType.Peak;
### CacheData (Bool)
If set to false will recalculate output every time GetData is called. If true will only do recalculations if input data of configuration has changed.
### ChannelCount (UInt32?)
Specifies the channel count of data in the output. If not set the count is unchanged.
#### Remarks
Setting channel count will guarantee a set number of channels for output data irrespective of input channel count

If ChannelMapping is not set then channels are mapped as following
* if ChannelCount is 1 then all input channels are added up
* if ChannelCount is 2 then channels are mapped as following

ChannelCount | InputChannels |  Output
------------ | ------------- | ----------- 
2 | 1 | out0 = in0, out1 = in0
2 | 3	| out0 = in0, out1 = in1
2	| 4	| out0 = in0+in2, out1 = in1+in3
2	| 5	| out0 = in0+in3, out1 = in1+in4
2	| 6	| out0 = in0+in4, out1 = in1+in5

In other cases respective inputs copied to output, missing outputs are clipped and missing inputs are of value 0

### ChannelMapping (float [])
Specifies how the input channel data is combined into the output data. To set this value you need to set the ChannelCount value first.

### FrequencyCount (UInt32?)
Specifies the frequency count of spectrum data in the output. If not set the count is unchanged
### FrequencyScale (ScaleType?)
Specifies the frequency scale of output spectrum data.
### Fps (float)
Indicates the frame rate of analyzer, this is will be equal to the Fps property of the source. If source is not set exception will be raised
### IsSuspended (bool)
This property is passed on to the source. If source is not set exception will be raised
#### MinFrequency (float?)
If set specifies what frequency the output spectrum data lower bounds.
#### MaxFrequency (float?)
If set specifies what frequency the output spectrum data upper bounds.
#### Remarks
Only conversion from Linear to Logarithmic is supported
### PlaybackState (SourcePlaybackState)
Returns the playbackstate of the source. If source is not set returns SourcePlaybackState.Stopped
### RmsRiseTime (TimeSpan?)
Specifies the rise time of RMS data in the output
### RmsFallTime (TimeSpan?)
Specifies the rise time of RMS data in the output
### PeakRiseTime (TimeSpan?)
Specifies the rise time of RMS data in the output
### PeakFallTime (TimeSpan?)
Specifies the rise time of RMS data in the output
### PresentationTime (TimeSpan?)
Returns the PresentationTime of the Source
### SpectrumRiseTime (TimeSpan?)
Specifies the rise time of RMS data in the output
### SpectrumFallTime (TimeSpan?)
Specifies the rise time of RMS data in the output





