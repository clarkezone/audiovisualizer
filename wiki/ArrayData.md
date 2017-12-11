## ArrayData class
 [ArrayData](wiki/ArrayData.md)
ArrayData class is used to process spectrum data. The instance of this class is created by the library and you rarely want to create your own.

### Interfaces
- IReadOnlyList<IReadOnlyList<float>>
- IEnumerable<IReadOnlyList<float>>

### Properties
#### AmplitudeScale (ScaleType)
- Linear: Frequency amplitudes are represented by linear scale 0.0f - 1.0f
- Logarithmic: Frequency amplitudes are represented in db, where value 1.0f = 0dB

#### FrequencyCount (uint)
The count of frequency bins in spectrum.

#### FrequencyScale (ScaleType)
- Linear: Frequency scale is linear
- Logarithmic: Frequency scale is logarithmic

#### FrequencyStep (float)
Frequency value difference (linear) or ratio (logarithmic) between spectrum elements

#### MinFrequency (float)
Spectrum element 0 starting frequency

#### MaxFrequency (float)
Spectrum element FrequencyCount starting frequency

### Methods
#### ApplyRiseAndFall
This method allows to apply rise and fall times to the spectrum data over time and thus smooth the spectrum movements over time. For calculation you need to keep the value of last displayed spectrum values and pass them as the first parameter of the method. 
The time value of rise (and fall) time indicate the time by what the difference in values of spectrum data has decreased by 67%. So if the input for spectrum bin 1kHz changes from 0 to 1.0 at 1sec and the riseTime = 300ms then by 1.3sec the output value of this method at 1khZ will be 0.67
This method will fail if the AmplitudeScale is Logarithmic or the Frequency Counts and other attributes do not match

##### Parameters
###### previousData (ArrayData) 
Spectrum values of the previous instance. If null calculations run as if all values of previous data were 0
###### riseTime (TimeSpan)
Time constant for rising values
###### fallTime (TimeSpan)
Time constant for falling values
###### timeFromPrevious (TimeSpan)
Time passed from previousData
##### Returns (ArrayData)
Method returns the calculated spectrum values with fall and rise times applied


### Notes
You can use this code to calculate the bin frequency like this
```
public float BinFrequency(ArrayData data, uint bin)
{
    if (data.FrequencyScale == ScaleType.Linear)
    {
        return bin * data.FrequencyStep + data.MinFrequency;
    }
    else
    {
        return data.MinFrequency * (float) Math.Pow(data.FrequencyStep, bin);
    }
}

You access data as you would in an array
    ArrayData a1;
    float third_value_channel_0 = a1[0][3];
