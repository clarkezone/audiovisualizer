## SpectrumData class
This class contains data from the spectral analyzes and also methods for different kind of conversions.
SpetralData is essentially a 2 dimensional array of data,first dimension being audio channels and the second being frequency dimension.
and the values containing the amplitude for a specific frequency component for specific audio channel.
Spectrum data implements read-only collection interfaces to a readonly collection of floats. Thus you can use the traditional collection access and enumeration methods.

```C#

    float value = spectrumData[1][10];  // 11th frequency element of the second channel
```
Each channel element is an array of float values containing the FFT values for absolute values of different frequency components. AmplitudeScale property will indicate if these values are linear (as they are a absolute values they would be always positive) or logarithmic - in that case expressed in db.
You can use  FrequencyScale, MinFrequency, MaxFrequency, FrequencyStep or GetFrequency and GetCenterFrequency to calculate the frequency value of the bin index. 

You can read more on how the spectrum data is created [here](Spectrum.md).

## Processing data
The data arrives from the analyzer in linear amplitude and frequency scale. There are several convinience methods to convert the data.
### Converting amplitude values
The spectrum frequency amplitude values are useful to be presented on a logarithmic scale. One can use ConvertToLogAmplitude can be used to convert spectral amplitude values to logarithmic. One can specify lower and upper scale values to clamp the value range.
    var logData = data.ConvertToLogAmplitude(-60.0f,0.0f);  // Convert to range -60 to 0 db.
There is no method to convert amplitude back to linear and many of the other operations require the data to be represented on a linear amplitude scale, so it is a good idea to apply this conversion as a last one.
### Converting frequency scale
There are methods to convert data to a scale with a set number of elements, frequency range and scale type.
```C#
    var spectrum1 = data.TransformLinearFrequency(50,0.0f,20000.0f);    // Convert input to 50 element spectrum ranging from 0-20kHz
    var spectrum2 = data.TransformLogFrequency(90,27.5f,4700.0f); // Convert spectrum into a musical spectrum range
```
Linear spectrum can be converted to logarithmic but not vice versa. The amplitude values need to be linear scale for this conversion.
### Applying rise and fall times
Signal meters and visualizations commonly have fall and rise times - if you apply a step signal for input it takes some time for the visualization to reach it's peak and vice versa, when input signal moves to zero it will take some time for visualization to reach zero level.
This kind of effect applied to data is like a filter that removes harsh signal movements and makes it more pleasing to the eye.
It is characterized by a "time constant" rise time and fall time, which indicates how much time is needed for the output to change 67% (1-exp(-1)) of the difference of a step input. So 100ms step time means that with a input changing from 0 to 1 at 0sec then at 0.1sec the output will be (1-0.37) = 0.63, at 0.2 sec (1-0.37x0.37) 0.86 and at 0.3 sec (1-0.37x0.37x0.37) = 0.95
You apply rise and fall times by using method ApplyRiseAndFall
```C#
    TimeSpan stepTime = TimeSpan.FromSeconds(dataFrame.Fps);
    TimeSpan time = TimeSpan.FromSeconds(0.1f); // Use 100ms rise and fall time
    var data = data.ApplyRiseAndFall(previousData,time,time,stepTime);
    // Draw visualization here
    ...
    previousData = data; 
```
If the input frames are missing (for example stream is stopped), then you can use the static version of the ApplyRiseAndFall method to assume the current frame being.
```C#
    void Draw(VisualizationDataFrame frame)
    {
        var data = frame != null ? frame.Spectrum.ApplyRiseAndFall(previousData,time,time,stepTime) : SpectrumData.ApplyRiseAndFall(previousData,time,time,stepTime);
        ...    
    }
```
### Interfaces
``` C#
IReadOnlyList<IReadOnlyList<float>>
IEnumerable<IReadOnlyList<float>>
```
### Constructors
#### static SpectrumData CreateEmpty(UInt32 cChannels, UInt32 cElements, ScaleType amplitudeScale, ScaleType frequencyScale, Single minFrequency, Single maxFrequency)
Creates a new spectrum data object with empty (0.0) values with given properties.
#### static SpectrumData Create(Single [] values, UInt32 cChannels, ScaleType amplitudeScale, ScaleType frequencyScale, Single minFrequency, Single maxFrequency)
Creates new spectrum data object with given initial values. The initial values are passed in as one dimensional array but must contain frequencyCount * cChannels elements. The data should be organized by frequency elements and then by channels.

### Properties
#### AmplitudeScale (ScaleType)
- Linear: Frequency amplitudes are represented by linear scale 0.0f - 1.0f
- Logarithmic: Frequency amplitudes are represented in db, where value 1.0f = 0dB
**Please Note** - most conversion functions will throw and exception and fail if the data has logarithic amplitude scale. Thus convert data to logarithmic as a last step

#### FrequencyCount (uint)
The count of frequency bins in spectrum.

#### FrequencyScale (ScaleType)
- Linear: Frequency scale is linear. f[n+1] = f[n] + FrequencyStep
- Logarithmic: Frequency scale is logarithmic. f[n+1] = f[n] * FrequencyStep

#### FrequencyStep (float)
Frequency value difference (linear) or ratio (logarithmic) between spectrum elements

#### MinFrequency (float)
Spectrum element 0 starting frequency

#### MaxFrequency (float)
Spectrum element FrequencyCount starting frequency

### Methods
#### ApplyRiseAndFallToEmpty(SpectrumData previous, Windows.Foundation.TimeSpan riseTime, Windows.Foundation.TimeSpan fallTime, Windows.Foundation.TimeSpan timeFromPrevious)
#### static ApplyRiseAndFallToEmpty(SpectrumData previous, Windows.Foundation.TimeSpan riseTime, Windows.Foundation.TimeSpan fallTime, Windows.Foundation.TimeSpan timeFromPrevious)
This method allows to apply rise and fall times to the spectrum data over time and thus smooth the spectrum movements over time. For calculation you need to keep the value of last displayed spectrum values and pass them as the first parameter of the method. 
The time value of rise (and fall) time indicate the time by what the difference in values of spectrum data has decreased by 67%. So if the input for spectrum bin 1kHz changes from 0 to 1.0 at 1sec and the riseTime = 300ms then by 1.3sec the output value of this method at 1khZ will be 0.67
This method will fail if the AmplitudeScale is Logarithmic or the Frequency Counts and other attributes do not match

The static version can be used if current instance of data is null. 

#### Parameters
##### previousData (SpectrumData) 
Spectrum values of the previous instance. If null calculations run as if all values of previous data were 0
##### riseTime (TimeSpan)
Time constant for rising values
###### fallTime (TimeSpan)
Time constant for falling values
##### timeFromPrevious (TimeSpan)
Time passed from previousData
#### Returns (SpectrumData)
Method returns the calculated spectrum values with fall and rise times applied

### SpectrumData LinearTransform(UInt32 elementCount,float fromFrequency,float toFrequency)
Maps spectrum frequency data to a different frequency range and element count
###	SpectrumData LogarithmicTransform(UInt32 elementCount,Single fromFrequency,Single toFrequency)
Maps spectrum frequency data to a different logarithmic frequency range and element count
###	SpectrumData ConvertToDecibels(Single minValue,Single maxValue)
Converts amplitude data to logarithmic scale and clamps the values to specified range
### SpectrumData CombineChannels(Single [] map)
Converts data to different channel mapping given specific transformation matrix
### float GetFrequency(UInt32 elementIndex)
Returns the frequency bin starting frequency
### float GetCenterFrequency(UInt32 elementIndex)
Returns the frequency bin center frequency

[Home](AudioVisualizer.md)