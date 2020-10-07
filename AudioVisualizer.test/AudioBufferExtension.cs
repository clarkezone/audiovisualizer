using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media;

namespace AudioVisualizer.test
{
    public static class AudioBufferExtension
    {
        public delegate float GeneratorFunction(Int64 frameOffset, uint channelIndex);
        public static unsafe void Generate(this AudioFrame frame, uint channels, Int64 frameOffset, GeneratorFunction generator)
        {
            using (var buffer = frame.LockBuffer(AudioBufferAccessMode.ReadWrite))
            {
                using (var bufferReference = buffer.CreateReference())
                {
                    byte* pByteData;
                    uint capacity;
                    ((IMemoryBufferByteAccess)bufferReference).GetBuffer(out pByteData, out capacity);
                    uint bufferLength = capacity / sizeof(float) / channels;
                    float* pSamples = (float*)pByteData;
                    int sampleIndex = 0;
                    for (Int64 frameIndex = frameOffset; frameIndex < frameOffset + bufferLength; frameIndex++)
                    {
                        for (uint channelIndex = 0; channelIndex < channels; channelIndex++, sampleIndex++)
                        {
                            pSamples[sampleIndex] = generator(frameIndex, channelIndex);
                        }

                    }
                }
            }
        }
    }
}
