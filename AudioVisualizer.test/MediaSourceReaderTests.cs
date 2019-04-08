using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Media;
using Windows.Media.MediaProperties;
using Windows.Storage;

namespace AudioVisualizer.test
{
    [TestClass]
    public class MediaSourceReaderTests
    {
        AudioSourceReader sut;
        [TestInitialize]
        public async Task Init()
        {
            var testFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///TestContent/test_signal.mp3"));
            var stream = await testFile.OpenAsync(FileAccessMode.Read);
            sut = new AudioSourceReader(stream);
        }
        [TestCleanup]
        public void Cleanup()
        {
            sut.Dispose();
            sut = null;
        }

        [TestMethod]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_FirstAudioStreamIndex()
        {
            Assert.AreEqual(-3, AudioSourceReader.FirstAudioStreamIndex);
        }


        [TestMethod]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_Duration()
        {
            Assert.AreEqual(TimeSpan.FromSeconds(5.064), sut.Duration);
        }

        [TestMethod]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_NativeTypeStream0()
        {
            var type = sut.GetNativeFormat(0);
            Assert.IsInstanceOfType(type, typeof(Windows.Media.MediaProperties.AudioEncodingProperties));
            var audioProps = (AudioEncodingProperties)type;
            Assert.AreEqual("Audio", audioProps.Type);
            Assert.AreEqual("MP3", audioProps.Subtype);
            Assert.AreEqual(48000u, audioProps.SampleRate);
            Assert.AreEqual(1u, audioProps.ChannelCount);
        }

        [TestMethod]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_NativeTypeStream1()
        {
            var type = sut.GetNativeFormat(1);
            Assert.IsNull(type);
        }

        [TestMethod]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_SetFormat()
        {
            sut.Format = AudioEncodingProperties.CreatePcm(48000, 1, 16);
        }

        [TestMethod]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_GetFormat()
        {
            sut.Format = AudioEncodingProperties.CreatePcm(48000, 1, 16);
            var format = (AudioEncodingProperties)sut.Format;
            Assert.AreEqual("Audio", format.Type);
            Assert.AreEqual("PCM", format.Subtype);
            Assert.AreEqual(48000u, format.SampleRate);
            Assert.AreEqual(1u, format.ChannelCount);
            Assert.AreEqual(16u, format.BitsPerSample);
        }


        /* Functional test of reader. All of the file is read to the end in different formats and reader and frame states and properties validated */
        [TestCategory("MediaSourceReader")]
        [DataTestMethod]
        [DataRow(44100u, 2u, 16u)]
        [DataRow(44100u, 1u, 16u)]
        [DataRow(44100u, 2u, 32u)]
        [DataRow(44100u, 1u, 32u)]
        [DataRow(48000u, 2u, 16u)]
        [DataRow(48000u, 1u, 16u)]
        [DataRow(48000u, 2u, 32u)]
        /*[DataRow(48000u, 1u, 32u)]*/
        public void MediaSourceReader_Read(uint sampleRate, uint channels, uint bitsPerSample)
        {
            var bytesPerSample = bitsPerSample >> 3;
            var format = AudioEncodingProperties.CreatePcm(sampleRate, channels, bitsPerSample);
            if (bitsPerSample == 32)
            {
                format.Subtype = "Float";
            }
            sut.Format = format;
            TimeSpan time = TimeSpan.Zero;

            AudioFrame frame = null;
            do
            {
                frame = sut.Read();
                if (frame == null)
                    break;

                Assert.AreEqual(sut.Flags, AudioSourceReaderFlags.None);
                Assert.IsTrue(Math.Abs(time.Subtract(frame.RelativeTime.Value).Ticks) < 2); // Max 1 tick difference

                using (var buffer = frame.LockBuffer(AudioBufferAccessMode.Read))
                {
                    var frameCount = buffer.Length / bytesPerSample / channels;
                    var expectedDuration = TimeSpan.FromTicks((long)frameCount * 10000000L / (long)sampleRate);
                    Assert.AreEqual(expectedDuration, frame.Duration.Value);
                }

                time = frame.RelativeTime.Value + frame.Duration.Value; // Next frame has to start where previous ended
            } while (frame != null);

            Assert.AreEqual(sut.Flags, AudioSourceReaderFlags.EndOfStream);
        }

        /* Functional test of reader. All of the file is read to the end in different formats and reader and frame states and properties validated */
        [DataTestMethod]
        [DataRow(new double[] { 0.0 })]
        [DataRow(new double[] { 5.0 })]
        [DataRow(new double[] { 5.0,1.0 })]
        [DataRow(new double[] { 2.0, 2.1 })]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_Seek(double [] seekSequence)
        {
            sut.Format = AudioEncodingProperties.CreatePcm(48000, 2, 16);

            foreach (var position in seekSequence)
            {
                var seekTime = TimeSpan.FromSeconds(position);

                sut.Seek(seekTime);
                var frame = sut.Read();
                Assert.IsTrue(frame.RelativeTime.Value <= seekTime);
                Assert.IsTrue(frame.RelativeTime.Value + frame.Duration.Value >= seekTime);
            }

        }

        [TestMethod]
        [TestCategory("MediaSourceReader")]
        public void MediaSourceReader_SeekBeyondEnd()
        {
            sut.Format = AudioEncodingProperties.CreatePcm(48000, 2, 16);
#if DEBUG
            Assert.ThrowsException<COMException>(
#else
            Assert.ThrowsException<Exception>(
#endif
            ()=>
                {
                    sut.Seek(TimeSpan.FromHours(1));
                }
                );
        }
    }
}
