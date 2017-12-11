# Spectrum
Spectrum data is calculated from the overlapped input data. To configure the spectrum analyzer
you need to cast the visualization source to ISpectralAnalyzerSettings and call the method ConfigureSpectrum
    ((ISpectralAnalyzerSettings)visualizationSource).ConfigureSpectrum(4096,0.5);
There are two parameters that you can configure - FFT length and overlap.
## FFT Length
FFT length is always a power of two, the default value is 2048. The number of frequency elements generated
for spectrum will be half the FFT value, and they will range from 0 to half the sampling frequency.
So for default value of 2048 and input sample rate of 48000Hz you will recieve spectrum that will range from
0...24000Hz with a step of 24000/1024=23.44Hz.
Increasing the FFT elements will increase the spectrum resolution, however it will also increase the number
of calculations needed.
The analyzer will take a number of samples from the input that equal to the time of output sample rate. 
So if the output sample rate is 60 frames per second, input signal is at 48000Hz and overlap is set to 50% then
the samples taken for each calculation will be 1.5 * 48000/60 = 1200. If your FFT length is set to 2048 or more
then the input data is padded by zeros (this will not affect the spectrum but will enable the use of fast algorithms for FFT).
If you happen to set the FFT length to less than input samples, then the analyzer will downsample the input signal -
this will cause the maximum frequency of the spectrum also to be halved (or divided by 3 or 4)
See the below table for frequency ranges and spectrum of some common sampling rates and FFT lengths.
[SpectrumData](SpectrumData.md) contains methods that makes it possible to convert and interpolate whatever 
specrtum range to another one without worring these details.

Resolution and highest frequency (in kHz) and frequency step (Hz) for different values (overlap set to 50%)

| Sample Rate | 1024 | 2048 | 4096 | 8192 |
| 32k | 16k/16 | 16k/8 | 16k/4 | 16k/2 |
| 44.1k | 11k/11 | 22k/11 | 22k/5 | 22k/3 |
| 48k | 12k/12 | 24k/12 | 24k/6 | 24k/3 |
| 96k | 12k/12 | 24k/12 | 48k/12 | 48k/6 |
| 192k | 12k/12 | 24k/12 | 48k/12 | 96k/12 |

## Overlap
Overlap parameter is a percentage of samples included from the previous frame in the analyzes. So at 48kHz sample rate 
and 60 fps output rate the analyzer will consume 800 samples for each output frame without an overlap. With 50% overlap 400 
samples from previous set is included. The allowed range is 0-100% (expressed as 0.0 - 1.0 value).
