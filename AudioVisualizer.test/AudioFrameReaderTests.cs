using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media;
using AudioVisualizer;

namespace AudioVisualizer.test
{
    [TestClass]
    public class AudioFrameReaderTests
    {
        AudioFrame frame;
        [TestInitialize]
        public void Initialize()
        {
            frame = new AudioFrame(40); // 10 elements
            frame.Generate(1, 0, (i, ch) => i);
        }

        [TestMethod]
        [TestCategory("AudioFrameReader")]
        public void AudioFrameReader_EnumerateAll()
        {
            var samples = AudioFrameReader.EnumerateAllSamples(frame);
            CollectionAssert.AreEqual(new float[] { 0f, 1f, 2f, 3f, 4f, 5f, 6f, 7f, 8f, 9f }, samples.ToArray());
        }

        [TestMethod]
        [TestCategory("AudioFrameReader")]
        public void AudioFrameReader_EnumerateAll_SampleCount()
        {
            var samples = AudioFrameReader.EnumerateAllSamples(frame);
            Assert.AreEqual(10u, samples.SampleCount);
        }

        [TestMethod]
        [TestCategory("AudioFrameReader")]
        public void AudioFrameReader_EnumerateMono()
        {
            var samples = AudioFrameReader.EnumerateAsMono(frame,2);
            CollectionAssert.AreEqual(new float[] { 0.5f, 2.5f, 4.5f, 6.5f, 8.5f }, samples.ToArray());
        }

        [TestMethod]
        [TestCategory("AudioFrameReader")]
        public void AudioFrameReader_EnumerateMono_SampleCount()
        {
            var samples = AudioFrameReader.EnumerateAsMono (frame,2);
            Assert.AreEqual(5u, samples.SampleCount);
        }

        [TestMethod]
        [TestCategory("AudioFrameReader")]
        public void AudioFrameReader_EnumerateSingleChannel()
        {
            var samples = AudioFrameReader.EnumerateSingleChannel(frame, 2, 1);
            CollectionAssert.AreEqual(new float[] { 1f, 3f, 5f, 7f, 9f }, samples.ToArray());
        }
        
        [TestMethod]
        [TestCategory("AudioFrameReader")]
        public void AudioFrameReader_EnumerateSingleChannel_SampleCount()
        {
            var samples = AudioFrameReader.EnumerateSingleChannel(frame, 2, 1);
            Assert.AreEqual(5u, samples.SampleCount);
        }
    }
}

