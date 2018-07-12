# Spectrum
Spectrum data is calculated from the overlapped input data. To calculate the spectrum a sample of input data (with possible overlap to previous samples) is taken, windowed with [Blackman-Nuttal window](https://en.wikipedia.org/wiki/Window_function
) and passed through a Fast Fourier Transform. The length of the FFT will define the number of spectrum bins and resolution. Increasing the length will provide better resolution but also need more processing power. [Fast Fourier Transform](https://en.wikipedia.org/wiki/Fast_Fourier_transform) used in the analyzer has the complexity of O(n log n) and by observation calculating FFT on a i7-4xxx is taking very roughly 2ms for stereo signal (this is for 1 core) and FFT size of 2048. So for a modern 4-core desktop CPU the CPU load for stereo signal 60fps is approximately 2/16.7/4 = 3%.

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
### Downsampling
If you happen to set the FFT length to less than input samples, then the analyzer will downsample the input signal -
this will cause the maximum frequency of the spectrum also to be halved (or divided by 3 or 4)
See the below table for frequency ranges and spectrum of some common sampling rates and FFT lengths.
[SpectrumData](SpectrumData.md) and [SourceConverter](sourceConverter.md) contains methods that makes it possible to convert and interpolate whatever 
specrtum range.

Resolution and highest frequency (in kHz) and frequency step (Hz) for different values (overlap set to 50%)

Sample Rate / FFT Size | 1024 | 2048 | 4096 | 8192
------------|------|------|------|-----
32000 | 16kHz / 16Hz | 16kHz / 8Hz | 16kHz / 4Hz | 16kHz / 2Hz
44100 | 11kHz / 11Hz | 22kHz / 11Hz | 22kHz / 5Hz | 22kHz / 3Hz
48000 | 12kHz / 12Hz | 24kHz / 12Hz | 24kHz / 6Hz | 24kHz / 3Hz
96000 | 12kHz / 12Hz | 24kHz / 12Hz | 48kHz / 12Hz | 48kHz / 6Hz
19200 | 12kHz / 12Hz | 24kHz / 12Hz | 48kHz / 12Hz | 96kHz / 12Hz

## Overlap
Overlap parameter is a percentage of samples included from the previous frame in the analyzes. So at 48kHz sample rate 
and 60 fps output rate the analyzer will consume 800 samples for each output frame without an overlap. With 50% overlap 400 
samples from previous set are included.

[Home](AudioVisualizer.md)
