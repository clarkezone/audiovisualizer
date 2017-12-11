# SpectralData
This class contains data from the spectral analyzes and also methods for different kind of conversions.
SpetralData is essentially a 2 dimensional array of data, with one axis being audio channels and the other being frequency
and the values containing the amplitude for a specific frequency component for specific audio channel.

## Interfaces
ISpectralData, IReadOnlyList<IReadOnlyList<float>>
## Methods
### SpectralData()
Method 
#### Parameters
##### ParameterName (type)
Description
##### Returns
**type**
Description
#### Examples
#### Remarks
Remarks here
#### See also
[Link](Link)
## Properties
### Property 1
Property description
#### PropertyName (type)
Additional Description
#### Remarks
Remarks here
#### See also
[Link](Link)
### Property 2
Property description
#### PropertyName (type)
Additional Description
#### Remarks
Remarks here
#### See also
[Link](Link)
## Events
### Event 1
Description

## Accessing Data
The object implements IReadOnlyList and this is how different channel data can be accessed - through a subscript operator.
All the other usual tricks work as well.

    var channelCount = data.Count();
    var leftChannelData = data[0];
    var rightChannelData = data[1];
    for (var channelData in data)
    {
        // Do something useful
        // channelData is IReadOnlyList<float>
        if (channelData[0] > 0.1f) {
            // If the lowest frequency component is greater than 0.1 do something
        }
    }

Each channel element is an array of float values containing the FFT values for absolute values of different frequency components. AmplitudeScale property will indicate if these values are linear (as they are a absolute values they would be always positive) or logarithmic - in that case expressed in db.
You can use  FrequencyScale, MinFrequency, MaxFrequency and FrequencyStep to calculate the frequency index of the value. 
If the FrequencyScale is Linear then an array element of index N will range from fN to fNp1 midpoint being fN + data.FrequencyStep / 2
    var fN = data.MinFrequency + N*data.FrequencyStep;
    var fNp1 = fN + data.FrequencyStep;

If the FrequencyScale is logarithmic then an element of index N will range from fN to fNp1, midpoint being fN * Math.Sqrt(data.FrequencyStep)
    var fN = data.MinFrequency * Math.Pow(data.MaxFrequency/data.MinFrequency,N/data.FrequencyCount);
    var fNp1 = fN * data.FrequencyStep;

## Processing data
The data arrives from the analyzer in linear amplitude and frequency scale. There are several convinience methods to convert the data.
### Converting amplitude values
The spectrum frequency amplitude values are useful to be presented on a logarithmic scale. One can use ConvertToLogAmplitude can be used to convert spectral amplitude values to logarithmic. One can specify lower and upper scale values to clamp the value range.
    var logData = data.ConvertToLogAmplitude(-60.0f,0.0f);  // Convert to range -60 to 0 db.
There is no method to convert amplitude back to linear and many of the other operations require the data to be represented on a linear amplitude scale, so it is a good idea to apply this conversion as a last one.
### Converting frequency scale
There are methods to convert data to a scale with a set number of elements, frequency range and scale type. 
    var spectrum1 = data.TransformLinearFrequency(50,0.0f,20000.0f);    // Convert input to 50 element spectrum ranging from 0-20kHz
    var spectrum2 = data.TransformLogFrequency(90,27.5f,4700.0f); // Convert spectrum into a musical spectrum range

Linear spectrum can be converted to logarithmic but not vice versa. The amplitude values need to be linear scale for this conversion.
### Applying rise and fall times
Signal meters and visualizations commonly have fall and rise times - if you apply a step signal for input it takes some time for the visualization to reach it's peak and vice versa, when input signal moves to zero it will take some time for visualization to reach zero level.
This kind of effect applied to data is like a filter that removes harsh signal movements and makes it more pleasing to the eye.
It is characterized by a "time constant" rise time and fall time, which indicates how much time is needed for the output to change 67% (1-exp(-1)) of the difference of a step input. So 100ms step time means that with a input changing from 0 to 1 at 0sec then at 0.1sec the output will be (1-0.37) = 0.63, at 0.2 sec (1-0.37x0.37) 0.86 and at 0.3 sec (1-0.37x0.37x0.37) = 0.95
You apply rise and fall times by using method ApplyRiseAndFall
    TimeSpan stepTime = TimeSpan.FromSeconds(dataFrame.Fps);
    TimeSpan time = TimeSpan.FromSeconds(0.1f); // Use 100ms rise and fall time
    var data = data.ApplyRiseAndFall(previousData,time,time,stepTime);
    // Draw visualization here
    ...
    previousData = data; 
If the input frames are missing (for example stream is stopped), then you can create an empty SpectrumData and call this method for the output gently go back to zero.
    var emptySpectrum = new SpectrumData(2,50,ScaleType.Linear,ScaleType.Linear,0.0f,20000.0f);

    void Draw(VisualizationDataFrame frame)
    {
        var data = frame != null ? frame.Spectrum : emtpySpectrum;
        var data = data.ApplyRiseAndFall(previousData,time,time,stepTime);
        ...    
    }

#### Remarks
Remark text here
