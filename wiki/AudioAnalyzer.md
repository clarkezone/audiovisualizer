## AudioAnalyzer
AudioAnalyzer class does the main work to analyze audio frames and produce [RMS](https://en.wikipedia.org/wiki/Root_mean_square), peak and [spectrum](Spectrum.md) based on the audio data.
To calculate the Time property for the output frames the analyzer will look at the RelativeTime value of the first frame recieved after Flush or then provided seed position. For the consecutive frames the Time property is calculated by adding up the frames consumed from input. This timekeeping is necessary as the timing info recieved from the Media Foundation stack is only approximately correct especially for frames recieved from different decoders. This will also provide meaningful timing info for sources that do not have timing like microphone input from AudioNode.

## Methods
### Constructor
    AudioAnalyzer(UInt32 bufferSize, UInt32 inputChannels, UInt32 sampleRate, UInt32 inputStep, UInt32 inputOverlap, UInt32 fftLength,bool backgroundProcessing);

Parameter | Description
-------| -----------
**UInt32** bufferSize | Buffer size (in audio frames) that the analyzer will hold. Longer buffer will allow accumulating more data than the analyzer will be able to process in real time. Minimum buffer size is step + overlap
**UInt32** inputChannels | Audio channel count of input data
**UInt32** sampleRate | Sample rate of input data
**UInt32** inputStep | How many new input samples to consume from the buffer for each output data frame
**UInt32** inputOverlap | How many previous samples from buffer to consume
**UInt32** fftLength | Length of FFT. Needs to be a power of 2
**bool** backgroundProcessing | If set to true then processing occurs on a background thread

### ProcessInput
    ProcessInput(AudioFrame inputFrame);
Provides input sample for the audio analyzer.
### Flush
    Flush();
    Flush(Int64 seedPosition);
Removes all data from input buffer and stops processing any current data. Either next audio frame RelativeTime property or seedPosition value will provide output data frame Time. Time is calculated from the seedPosition as

    Time = seedPosition / sampleRate
## Properties
### AnalyzerTypes
Type: **[AnalyzerType](AnalyzerType.md)**

Flags that specify which analyzes to perform. If you do not need spectrum, you can only specify RMS and Peak in order to conserve processing power.

    analyzer.AnalyzerTypes = AnalyzerType.RMS;

### SpectrumStep
Type: **float**

The frequency step of one spectrum element in the output data.

    float tenthBinFrequency = analyzer.SpectrumStep * 10.0f;
### SpectrumElementCount
Type: **UInt32**

The number of spectrum elements in the output.   
### IsSuspended
Type: **bool**

Set this property to true to suspend the processing, however it will not stop the buffering of input data. If the input buffer overflows then old data will be overwritten. After suspended state is switched off the analyzer will generate output for all accumulated input.
## Events
### Output
    event Windows.Foundation.TypedEventHandler<AudioAnalyzer, VisualizationDataFrame> Output;
This event is raised every time there is output data frame generated from input data. The event will provide the analyzer as first parameter and data frame as second parameter


[Home](AudioVisualizer.md)