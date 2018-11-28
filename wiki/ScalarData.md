# ScalarData
Scalar data holds scalar visualization data like RMS or Peak. This class implements readonly collection and enumeration interfaces, thus data for different channels can be accessed and enumerated with traditional vector access methods.
```C#
    var channelCount = data.Count();
    var leftChannelData = data[0];
    var rightChannelData = data[1];
    for (var channelData in data)
    {
        // Do something useful
    }
```

## Constructors
### static ScalarData CreateEmpty(UInt32 channels)
Creates empty scalar data with values set to 0
### static ScalarData Create(Single[] values)
Creates scalar data with set values

## Methods
### static ScalarData ApplyRiseAndFallToEmpty(ScalarData previous, Windows.Foundation.TimeSpan riseTime, Windows.Foundation.TimeSpan fallTime, Windows.Foundation.TimeSpan timeFromPrevious)
### ScalarData ApplyRiseAndFall(ScalarData previous, Windows.Foundation.TimeSpan riseTime, Windows.Foundation.TimeSpan fallTime, Windows.Foundation.TimeSpan timeFromPrevious);
Applies rise and fall times to the values. Please see [SpectrumData](SpectrumData.md) for more detailed explanation.
### ScalarData ConvertToDecibels(float minValue, float maxValue)
Converts data to logarithmic scale and clamps it to preset minimum and maximum values
### ScalarData CombineChannels(Single[] map)
Remaps data between channels

## Properties
### ScaleType AmplitudeScale
Indicates if data is represented on linear or logarithmic scale

[Home](AudioVisualizer.md)