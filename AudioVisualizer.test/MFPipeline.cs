using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.MediaProperties;

namespace AudioVisualizer.test
{
    namespace MediaFoundation
    {
        internal class MediaPipelineFake
        {
            private MftWrapper _mft;

            // Configuration properties
            public readonly uint SampleRate;
            public readonly uint Channels;



            public MediaPipelineFake(MftWrapper mftObject,uint sampleRate,uint nChannels)
            {
                _mft = mftObject;
                SampleRate = sampleRate;
                Channels = nChannels;
                AudioEncodingProperties mediaType = AudioEncodingProperties.CreatePcm(SampleRate, Channels, 32);
                mediaType.Subtype = "Float";
                _mft.SetInputMediaType(0, mediaType, false);
                _mft.SetOutputMediaType(0, mediaType, false);
            }


        }
    }
}
