using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.VisualStudio.TestTools.UnitTesting.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Media;
using Windows.Media.Audio;
using Windows.Media.Effects;
using Windows.Media.MediaProperties;

namespace AudioVisualizer.test
{
    [TestClass]
    public class VisualizerAudioEffectTests
    {
        FakeAudioNode _audioNode;
        IBasicAudioEffect sut;

        [TestInitialize]
        public void TestInit()
        {
            _audioNode = new FakeAudioNode();
            _audioNode.EffectAdded += new TypedEventHandler<FakeAudioNode, IAudioEffectDefinition>(
                (node, effectDefinition) =>
                {
                    sut = (VisualizerAudioEffect) node.CreateAudioEffect(effectDefinition);
                }
                );
            PlaybackSource.CreateFromAudioNode(_audioNode);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_CreateSucceeded()
        {
            Assert.IsNotNull(sut);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_IsOfRightType()
        {
            Assert.IsInstanceOfType(sut, typeof(AudioVisualizer.VisualizerAudioEffect));
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_UseInputFrameForOutputTrue()
        {
            Assert.IsTrue(sut.UseInputFrameForOutput);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_IsSourceSetInProperties()
        {
            PropertySet properties = new PropertySet();
            sut.SetProperties(properties);
            Assert.AreEqual(sut, properties["Source"]);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_IsTypeSetInProperties()
        {
            PropertySet properties = new PropertySet();
            sut.SetProperties(properties);
            Assert.AreEqual(typeof(VisualizerAudioEffect).FullName, (string) properties["Type"]);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SetEncodingProperties()
        {
            var encoding = AudioEncodingProperties.CreatePcm(48000, 2, 32);
            encoding.Subtype = "Float";
            sut.SetEncodingProperties(encoding);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SetNonFloatEncodingPropertiesThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                sut.SetEncodingProperties(AudioEncodingProperties.CreatePcm(44100, 2, 16));
            });
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SetNullEncodingPropertiesThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                sut.SetEncodingProperties(null);
            });
        }

        [DataTestMethod()]
        [DataRow(8000u, 1u)]
        [DataRow(11025u, 1u)]
        [DataRow(16000u, 1u)]
        [DataRow(22050u, 1u)]
        [DataRow(44100u, 1u)]
        [DataRow(48000u, 1u)]
        [DataRow(96000u, 1u)]
        [DataRow(192000u, 1u)]
        [DataRow(8000u,2u)]
        [DataRow(11025u,2u)]
        [DataRow(16000u,2u)]
        [DataRow(22050u,2u)]
        [DataRow(44100u,2u)]
        [DataRow(48000u,2u)]
        [DataRow(96000u, 2u)]
        [DataRow(192000u, 2u)]
        [DataRow(44100u, 5u)]
        [DataRow(48000u, 5u)]
        [DataRow(96000u, 5u)]
        [DataRow(192000u, 5u)]
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_GetSupportedEncodingProperties(UInt32 sampleRate,UInt32 channelCount)
        {
            var encodings = sut.SupportedEncodingProperties;
            var match = encodings.Where((prop) => prop.Type == "Audio" && prop.Subtype == "Float" && prop.SampleRate == sampleRate && prop.ChannelCount == channelCount && prop.BitsPerSample == 32);
            Assert.AreEqual(1, match.Count(), $"{sampleRate}Hz {channelCount}ch, {match.Count()} found.");
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SetNotPo2FftLengthThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)sut;
                analyzer.ConfigureSpectrum(2047, 0.0f);
            });
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SetFftLengthLT256Throws()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)sut;
                analyzer.ConfigureSpectrum(128, 0.0f);
            });
        }


        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SetOverlapToNegativeThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)sut;
                analyzer.ConfigureSpectrum(2048, -1.0f);
            });
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SetOverlapOverOneThrows()
        {
            Assert.ThrowsException<ArgumentException>(() => {
                ISpectralAnalyzer analyzer = (ISpectralAnalyzer)sut;
                analyzer.ConfigureSpectrum(2048, 1.1f);
            });
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualChannelCountIsNullWihtoutEncodingSet()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsNull(source.ActualChannelCount);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualChannelCountEQEncodingSet()
        {
            var encoding = AudioEncodingProperties.CreatePcm(48000, 2, 32);
            encoding.Subtype = "Float";
            sut.SetEncodingProperties(encoding);
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(encoding.ChannelCount,source.ActualChannelCount);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualFrequencyCountCorrectWihtoutEncodingSet()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsNull(source.ActualFrequencyCount);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualFrequencyStepCorrectWihtoutEncodingSet()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsNull(source.ActualFrequencyStep);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualFrequencyCountCorrectWithEncodingSet()
        {
            var encoding = AudioEncodingProperties.CreatePcm(48000, 2, 32);
            encoding.Subtype = "Float";
            sut.SetEncodingProperties(encoding);
            (sut as ISpectralAnalyzer).ConfigureSpectrum(1024, 0.0f);
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(512u, source.ActualFrequencyCount);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualFrequencyStepCorrectWithEncodingSet()
        {
            var encoding = AudioEncodingProperties.CreatePcm(48000, 2, 32);
            encoding.Subtype = "Float";
            sut.SetEncodingProperties(encoding);
            (sut as ISpectralAnalyzer).ConfigureSpectrum(1024, 0.0f);
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(24000f/512f, source.ActualFrequencyStep);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_DefaultFrequencyScaleIsLinear()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(ScaleType.Linear, source.ActualFrequencyScale);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualMaxFrequencyIsNullWihtoutEncodingSet()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsNull(source.ActualMaxFrequency);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualMaxFrequencyCorrectWithEncodingSet()
        {
            var encoding = AudioEncodingProperties.CreatePcm(48000, 2, 32);
            encoding.Subtype = "Float";
            sut.SetEncodingProperties(encoding);
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(24000, source.ActualMaxFrequency);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_ActualMinFrequencyIsZero()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(0.0f,source.ActualMinFrequency);
        }


        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_DefaultAnalyzerTypesAll()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(AnalyzerType.All, source.AnalyzerTypes);
        }
        [DataTestMethod]
        [DataRow(AnalyzerType.None)]
        [DataRow(AnalyzerType.Peak)]
        [DataRow(AnalyzerType.RMS)]
        [DataRow(AnalyzerType.Spectrum)]
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_CanSetAnalyzerType(AnalyzerType type)
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            source.AnalyzerTypes = type;
            Assert.AreEqual(type, source.AnalyzerTypes);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_IsFpsCorrect()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.AreEqual(60.0f, source.Fps);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SettingFpsThrows()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.ThrowsException<NotImplementedException>(() => {
                source.Fps = 0.1f;
            });
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_SettingSuspendedThrows()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.ThrowsException<NotImplementedException>(() => {
                source.IsSuspended = true;
            });
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_NotConfiguredIsSuspended()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsTrue(source.IsSuspended);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_DefaultNotIsSuspended()
        {
            var encoding = AudioEncodingProperties.CreatePcm(48000, 2, 32);
            encoding.Subtype = "Float";
            sut.SetEncodingProperties(encoding);
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsFalse(source.IsSuspended);
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_GettingPlaybackStates()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.ThrowsException<NotImplementedException>(() => {
                var state = source.PlaybackState;
            });
        }

        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_PresentationTimeReturnsNull()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsNull(source.PresentationTime);
        }
        [TestMethod]
        [TestCategory("VisualizerAudioEffect")]
        public void VisualizerAudioEffect_GetDataReturnsNull()
        {
            IVisualizationSource source = (IVisualizationSource)sut;
            Assert.IsNull(source.GetData());
        }
    }
}
