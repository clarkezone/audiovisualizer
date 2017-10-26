# Build status
[![Build status](https://ci.appveyor.com/api/projects/status/p4a7jmpa9391bxu8?svg=true)](https://ci.appveyor.com/project/clarkezone/audiovisualizer) 

# Introduction
TODO: Give a short introduction of your project. Let this section explain the objectives or the motivation behind this project. 

AudioAnalyzer project contains a Media Foundation Transform object that can be loaded as MediaPlayer effect
This effect uses overlapped FFT to produce spectrum of the audio data


# Getting Started
TODO: Guide users through getting your code up and running on their own system. In this section you can talk about:
1.	Installation process
2.	Software dependencies
3.	Latest releases
4.	API references

You add the effect to the MediaPlayer pipeline by calling AddEffect:

	var propertySet = new PropertySet();
    propertySet.MapChanged += EffectProperties_Changed;
    mediaPlayer.AddAudioEffect("AudioAnalyzer.AnalyzerEffect", false, m_AnalyzerPropertySet);

MediaPlayer will load the effect and on load the effect will add reference to itself under key "Analyzer". 
One way to capture that and configure the analyzer is to register for MapChanged event for the
PropertySet. Then you can configure the analyzer in the event handler and keep the reference to the 
IAudioAnalyzer, so you can get the spectrum data later.

	AudioAnalyzer.IAudioAnalyzer = _analyzer;

    private void EffectProperties_Changed(IObservableMap<string, object> sender, IMapChangedEventArgs<string> @event)
    {
		_analyzer = (AudioAnalyzer.IAudioAnalyzer) sender["Analyzer"];
        _analyzer.Configure(4096, 60, 0.5f);
        _analyzer.SetLinearFScale(100);
    }

IAudioAnalyzer interface

Methods

void Configure(uint fftLength,float outFrameRate,float overlap)
	Configures the analyzer for operation, needs to be called before operation as exception will be generated otherwise

	fftLength -		this is the length of elements in FFT that is used to analyze the spectrum.
					The bigger the number the higher the precision of output. By default and also the maximum
					number of elements for output is fftLength / 2
					fftLength has to be power of 2 and needs to be more audio frames than output frame + overlap time will cover
					So at 48000 sample rate and 60Hz out frame rate and 50% (0.5) overlap the minimum
					number of frames is (1+0.5) * (48000 * 1/60) = 900 -> closest power of 2 is 1024.
					Usually 2048 or 4096 will be good values to use here
	outFrameRate -	Input audio data will be analyzed at this frequency. 60.0 is a good value to be in sync with 
					display updates
	overlap -		Percentage value of how much data that is analyzed will overlap with previous frame. So if using
					60fps with 48000HZ audio and 0.5 overlap the first spectr will be generated from audio frames -300 ... 600,
					second from 300...1200

void SetLinearFScale(uint outputElementsCount)
	Configures the output frames to contain a specific number of elements

	outputElementsCount -	Number of elements in the output frames. Need to be 1...fftLength/2. Default is
							fftLength/2

void SetLogFScale(float lowFrequency,float hiFrequency,uint outputElementsCount)
	Not implemented in this version

AudioFrame GetFrame()
	Based on the presentation time of current stream extracts spectral data and returns this as AudioFrame
	If there is not frame with current presentation time in the queue then returns null
	You can expect this function to return null at the start of the stream or when seeking for couple of frames

	Data is packed in the AudioFrame buffer as following

	<aligned to 16 bytes> | Channel 0 outputElements (float) |
	<aligned to 16 bytes> | Channel 1 outputElements (float) |
	<aligned to 16 bytes> | float array of RMS data for all channels |

	You can obtain the step from one channel data to another and rms data offset
	by getting extended properties as following. Data is given in float elements (4 bytes).

	dataStep = (uint)frame.ExtendedProperties["{F4D65F78-CF5A-4949-88C1-76DAD605C313}"];
    rmsOffset = (uint)frame.ExtendedProperties["{0B82B25D-E6E1-4B6B-92A1-7EEE99D02CFE}"];



# Build and Test
TODO: Describe and show how to build your code and run the tests. 

# Contribute
TODO: Explain how other users and developers can contribute to make your code better. 

If you want to learn more about creating good readme files then refer the following [guidelines](https://www.visualstudio.com/en-us/docs/git/create-a-readme). You can also seek inspiration from the below readme files:
- [ASP.NET Core](https://github.com/aspnet/Home)
- [Visual Studio Code](https://github.com/Microsoft/vscode)
- [Chakra Core](https://github.com/Microsoft/ChakraCore)
