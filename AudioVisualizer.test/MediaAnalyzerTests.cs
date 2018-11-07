using AudioVisualizer.test.MediaFoundation.Interop;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation.Collections;
using Windows.Media.MediaProperties;

namespace AudioVisualizer.test
{
    [TestClass]
    public class MediaAnalyzerTests
    {
        private MediaFoundation.MftWrapper _mft;
        private MediaAnalyzer _analyzer;

        private static AudioEncodingProperties CreateFloatEncodingProperty(uint sampleRate, uint channels)
        {
            return new AudioEncodingProperties()
            {
                SampleRate = sampleRate,
                ChannelCount = channels,
                BitsPerSample = 32,
                Bitrate = 32 * channels * sampleRate,
                Subtype = "Float"
            };
        }

        [TestInitialize]
        public void Test_Init()
        {
            _analyzer = new AudioVisualizer.MediaAnalyzer();
            Assert.IsNotNull(_analyzer);
            _mft = new MediaFoundation.MftWrapper(_analyzer);
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]

        public void MediaAnalyzer_Mft_GetStreamLimits()
        {
            var limits = _mft.GetStreamLimits();
            Assert.AreEqual(1u, limits.minInput);
            Assert.AreEqual(1u, limits.maxInput);
            Assert.AreEqual(1u, limits.minOutput);
            Assert.AreEqual(1u, limits.maxOutput);
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetStreamCount()
        {
            var counts = _mft.GetStreamCount();
            Assert.AreEqual(1u, counts.inputStreamCount);
            Assert.AreEqual(1u, counts.outStreamCount);
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetStreamIDs()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.GetStreamIds();
                });
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetInputStreamInfo_Stream_0()
        {
            var streamInfo = _mft.GetInputStreamInfo(0);
            Assert.AreEqual(0u, streamInfo.cbAlignment);
            Assert.AreEqual(0u, streamInfo.cbMaxLookahead);
            Assert.AreEqual(0u, streamInfo.cbSize);
            Assert.AreEqual(1u, streamInfo.dwFlags);
            Assert.AreEqual(0u, streamInfo.hnsMaxLatency);
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetInputStreamInfo_Stream_1_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.GetInputStreamInfo(1);
            });
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetOutputStreamInfo_Stream_0()
        {
            var streamInfo = _mft.GetOutputStreamInfo(0);
            Assert.AreEqual(0u, streamInfo.cbAlignment);
            Assert.AreEqual(0u, streamInfo.cbSize);
            Assert.AreEqual(517u, streamInfo.dwFlags);
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetOutputStreamInfo_Stream_1_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.GetOutputStreamInfo(1);
            });
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetInputStreamAttributes()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.GetInputStreamAttributes();
                });
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetOutputStreamAttributes()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.GetOutputStreamAttributes();
                });
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetAttributes()
        {
            var attributes = _mft.GetAttributes();
            Assert.IsNotNull(attributes);
            Assert.IsFalse(attributes.Any());
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_DeleteInputStream()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.DeleteInputStream(0);
                });
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_AddInputStreams()
        {
            Assert.ThrowsException<NotImplementedException>(
                () =>
                {
                    _mft.AddInputStreams(0);
                });
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetAvailableInputTypes_Type_0()
        {
            var type0 = _mft.GetAvailableInputTypes(0, 0);
            Assert.AreEqual(2, type0.Properties.Count());
            Assert.AreEqual("Audio", type0.Type);
            Assert.AreEqual("Float", type0.Subtype);
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.GetAvailableInputTypes(0, 1);
            }
            );
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_GetAvailableInputTypes_Type_1_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                try
                {
                    _mft.GetAvailableInputTypes(0, 1);
                }
                catch (Exception e)
                {
                    Assert.AreEqual(-1072875847, e.HResult);
                    throw;
                }
            }
            );
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_SetInputType_MP3_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.SetInputMediaType(0, AudioEncodingProperties.CreateMp3(44100, 2, 128000), true);
            });
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_Mft_SetInputType_PCM_Throws()
        {
            Assert.ThrowsException<COMException>(() =>
            {
                _mft.SetInputMediaType(0, AudioEncodingProperties.CreatePcm(44100, 2, 16), true);
            });
        }
        [TestCategory("MediaAnalyzer")]
        [DataTestMethod()]
        [DataRow(22050, 1)]
        [DataRow(32000,1)]
        [DataRow(44100, 1)]
        [DataRow(48000, 1)]
        [DataRow(96000, 1)]
        [DataRow(192000, 1)]
        [DataRow(22050, 2)]
        [DataRow(32000, 2)]
        [DataRow(44100, 2)]
        [DataRow(48000, 2)]
        [DataRow(96000, 2)]
        [DataRow(192000, 2)]
        [DataRow(48000, 5)]
        public void MediaAnalyzer_Mft_SetInputType(int sampleRate,int channels)
        {
            _mft.SetInputMediaType(0, CreateFloatEncodingProperty((uint) sampleRate, (uint) channels), true);
        }
        [TestCategory("MediaAnalyzer")]
        [DataTestMethod()]
        [DataRow(22050, 1)]
        [DataRow(32000, 1)]
        [DataRow(44100, 1)]
        [DataRow(48000, 1)]
        [DataRow(96000, 1)]
        [DataRow(192000, 1)]
        [DataRow(22050, 2)]
        [DataRow(32000, 2)]
        [DataRow(44100, 2)]
        [DataRow(48000, 2)]
        [DataRow(96000, 2)]
        [DataRow(192000, 2)]
        [DataRow(48000, 5)]
        public void MediaAnalyzer_Mft_SetOutputType(int sampleRate, int channels)
        {
            _mft.SetInputMediaType(0, CreateFloatEncodingProperty((uint)sampleRate, (uint)channels), true);
        }

        [TestCategory("MediaAnalyzer")]
        [DataTestMethod()]
        [DataRow(22050, 1)]
        [DataRow(32000, 1)]
        [DataRow(44100, 1)]
        [DataRow(48000, 1)]
        [DataRow(96000, 1)]
        [DataRow(192000, 1)]
        [DataRow(22050, 2)]
        [DataRow(32000, 2)]
        [DataRow(44100, 2)]
        [DataRow(48000, 2)]
        [DataRow(96000, 2)]
        [DataRow(192000, 2)]
        [DataRow(48000, 5)]
        public void MediaAnalyzer_Mft_Process(int sampleRate,int channels)
        {
           MediaFoundation.MediaPipelineFake pipeline = new MediaFoundation.MediaPipelineFake(_mft,(uint)sampleRate,(uint)channels);
        }

        public void MediaAnalyzer_Mft_IsInputSamplePassedToOutput()
        {
            var encoding = CreateFloatEncodingProperty((uint)48000, (uint)2);
            _mft.SetInputMediaType(0, encoding, false);
            _mft.SetOutputMediaType(0, encoding, false);
            IMFSample sample;
            MediaFoundation.Interop.MFPlat.MFCreateSample(out sample);
            _mft.ProcessInput(sample);
            var outSample = _mft.ProcessOutput();
            Assert.AreSame(sample, outSample);
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_VisualizationSource_ActualChannelCount()
        {
            var encoding = CreateFloatEncodingProperty((uint)48000, (uint)2);
            _mft.SetInputMediaType(0, encoding, false);
            _mft.SetOutputMediaType(0, encoding, false);
            _analyzer.ConfigureSpectrum(2048, 0.5f);
            Assert.AreEqual(2u, _analyzer.ActualChannelCount);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_ActualFrequencyCount()
        {
            var encoding = CreateFloatEncodingProperty((uint)48000, (uint)2);
            _mft.SetInputMediaType(0, encoding, false);
            _mft.SetOutputMediaType(0, encoding, false);
            _analyzer.ConfigureSpectrum(2048, 0.5f);
            Assert.AreEqual(1024u, _analyzer.ActualFrequencyCount);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_ActualFrequencyScale()
        {
            var encoding = CreateFloatEncodingProperty((uint)48000, (uint)2);
            _mft.SetInputMediaType(0, encoding, false);
            _mft.SetOutputMediaType(0, encoding, false);
            _analyzer.ConfigureSpectrum(2048, 0.5f);
            Assert.AreEqual(ScaleType.Linear, _analyzer.ActualFrequencyScale);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_ActualMinFrequency()
        {
            var encoding = CreateFloatEncodingProperty((uint)48000, (uint)2);
            _mft.SetInputMediaType(0, encoding, false);
            _mft.SetOutputMediaType(0, encoding, false);
            _analyzer.ConfigureSpectrum(2048, 0.5f);
            Assert.AreEqual(0.0f, _analyzer.ActualMinFrequency);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_ActualMaxFrequency()
        {
            var encoding = CreateFloatEncodingProperty((uint)48000, (uint)2);
            _mft.SetInputMediaType(0, encoding, false);
            _mft.SetOutputMediaType(0, encoding, false);
            _analyzer.ConfigureSpectrum(2048, 0.5f);
            Assert.AreEqual(24000.0f, _analyzer.ActualMaxFrequency);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_IsSuspended()
        {
            Assert.IsFalse(_analyzer.IsSuspended);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_SetIsSuspended()
        {
            var encoding = CreateFloatEncodingProperty((uint)48000, (uint)2);
            _mft.SetInputMediaType(0, encoding, false);
            _mft.SetOutputMediaType(0, encoding, false);
            _analyzer.ConfigureSpectrum(2048, 0.5f);
            _analyzer.IsSuspended = true;
            Assert.IsTrue(_analyzer.IsSuspended);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_FpsIs60()
        {
            Assert.AreEqual(60.0f, _analyzer.Fps);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_DefaultAnalyzerTypesAreAll()
        {
            Assert.AreEqual(AnalyzerType.All, _analyzer.AnalyzerTypes);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_SetAnalyzerTypes()
        {
            _analyzer.AnalyzerTypes = AnalyzerType.RMS;
            Assert.AreEqual(AnalyzerType.RMS, _analyzer.AnalyzerTypes);
        }

        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_DefaultPresentationTimeIsNull()
        {
            Assert.IsNull(_analyzer.PresentationTime);
        }

        /* Need more work there are com interop issues executing this test
        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_IsPresentationClockTimePassed()
        {
            MediaFoundation.FakePresentationClock clock = new MediaFoundation.FakePresentationClock() { Time = TimeSpan.FromSeconds(10) };
            var clockConsumer = ((IMFClockConsumer)(object)_analyzer);
            //clock.AddClockStateSink(null);

            clockConsumer.SetPresentationClock(clock);
            Assert.AreEqual(TimeSpan.FromSeconds(10),_analyzer.PresentationTime);
        }*/
        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_VisualizationSource_DefaultPlaybackStateIsStopped()
        {
            Assert.AreEqual(SourcePlaybackState.Stopped,_analyzer.PlaybackState);
        }
        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_ClockState_PlaybackStatePlayingAfterStart()
        {
            ((IMFClockStateSink)(object)_analyzer).OnClockStart(0, 0);
            Assert.AreEqual(SourcePlaybackState.Playing, _analyzer.PlaybackState);
        }
        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_ClockState_PlaybackStateStoppedAfterStop()
        {
            ((IMFClockStateSink)(object)_analyzer).OnClockStop(0);
            Assert.AreEqual(SourcePlaybackState.Stopped, _analyzer.PlaybackState);
        }
        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_ClockState_PlaybackStatePausedAfterPause()
        {
            ((IMFClockStateSink)(object)_analyzer).OnClockPause(0);
            Assert.AreEqual(SourcePlaybackState.Paused, _analyzer.PlaybackState);
        }
        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_ClockState_PlaybackStatePlayingAfterRestart()
        {
            ((IMFClockStateSink)(object)_analyzer).OnClockRestart(0);
            Assert.AreEqual(SourcePlaybackState.Playing, _analyzer.PlaybackState);
        }
        [TestCategory("MediaAnalyzer")]
        [TestMethod]
        public void MediaAnalyzer_IMediaExtension_SetProperties()
        {
            PropertySet properties = new PropertySet();
            _analyzer.SetProperties(properties);
            Assert.AreEqual(_analyzer,properties["Source"]);
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_SetNotPo2FftLengthThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)_analyzer;
                analyzer.ConfigureSpectrum(2047, 0.0f);
            });
        }

        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_SetFftLengthLT256Throws()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)_analyzer;
                analyzer.ConfigureSpectrum(128, 0.0f);
            });
        }


        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_SetOverlapToNegativeThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)_analyzer;
                analyzer.ConfigureSpectrum(2048, -1.0f);
            });
        }
        [TestMethod]
        [TestCategory("MediaAnalyzer")]
        public void MediaAnalyzer_SetOverlapOverOneThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)_analyzer;
                analyzer.ConfigureSpectrum(2048, 1.1f);
            });
        }
    }
}