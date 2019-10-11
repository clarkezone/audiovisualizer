using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AudioVisualizer.test
{
    [TestClass]
    public class LoopbackVisualizationSourceTests
    {
        private LoopbackVisualizationSource sut;

        [TestInitialize]
        public async Task ActivateSourceAsync()
        {
            sut = await LoopbackVisualizationSource.CreateAsync();
        }

        [TestMethod]
        public void FpsIs60()
        {
            Assert.AreEqual(60.0f, sut.Fps);
        }
        [TestMethod]
        public void SetFpsThrows()
        {
            Assert.ThrowsException<NotImplementedException>(
                ()=>
                {
                    sut.Fps = 10.0f;
                });
        }
        [TestMethod]
        public void ActualChannelCountIsNotNull()
        {
            Assert.IsNotNull(sut.ActualChannelCount);
        }
        [TestMethod]
        public void ActualFrequencyCountIsNotNull()
        {
            Assert.IsNotNull(sut.ActualFrequencyCount);
        }
        [TestMethod]
        public void ActualFrequencyStepIsNotNull()
        {
            Assert.IsNotNull(sut.ActualFrequencyStep);
        }
        [TestMethod]
        public void ActualFrquencyScaleIsLinear()
        {
            Assert.AreEqual(ScaleType.Linear,sut.ActualFrequencyScale);
        }
        [TestMethod]
        public void ActualMaxFrequencyIsNotNull()
        {
            Assert.IsNotNull(sut.ActualMaxFrequency);
        }

        [TestMethod]
        public void ActualMinFrequencyIsZero()
        {
            Assert.AreEqual(0.0f, sut.ActualMinFrequency);
        }

        [TestMethod]
        public void AnalyzerTypesAreAll()
        {
            Assert.AreEqual(AnalyzerType.All, sut.AnalyzerTypes);
        }

        [TestMethod]
        public void DefaultSuspendedIsTrue()
        {
            Assert.IsTrue( sut.IsSuspended);
        }
        [TestMethod]
        public void DefaultPlaybackStateIsStopped()
        {
            Assert.AreEqual(SourcePlaybackState.Stopped, sut.PlaybackState);
        }

        [TestMethod]
        public void CanAddConfiguationChangedHandler()
        {
            sut.ConfigurationChanged += Source_ConfiguationChanged;
        }

        private void Source_ConfiguationChanged(IVisualizationSource sender, string args)
        {

        }

        [TestMethod]
        public void CanRemoveConfiguationChangedHandler()
        {
            sut.ConfigurationChanged += Source_ConfiguationChanged;
            sut.ConfigurationChanged -= Source_ConfiguationChanged;
        }


        [TestMethod]
        public void DefaultPresentationTimeIsZero()
        {
            Assert.AreEqual(TimeSpan.Zero, sut.PresentationTime);
        }
        [TestMethod]
        public void SuspendedIsFalseAfterSet()
        {
            sut.IsSuspended = false;
            Assert.IsFalse(sut.IsSuspended);
        }

        [TestMethod]
        public void PlaybackStateIsPlayingAfterSettingSuspendedToFalse()
        {
            sut.IsSuspended = false;
            Assert.AreEqual(SourcePlaybackState.Playing, sut.PlaybackState);
        }


    }
}
