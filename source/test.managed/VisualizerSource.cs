
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Threading.Tasks;
using Windows.Media.Playback;
using Windows.Storage;
using Windows.Media.Core;
using Windows.UI.Xaml.Controls;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;

namespace test.managed
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public async Task VisualizerSource_CreateFromMediaPlayer()
        {
            var testFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///TestContent/test_signal.mp3"));
            var player = new MediaPlayer();
            var createTask = AudioVisualizer.VisualizationSource.CreateFromMediaPlayerAsync(player);
            player.Source = MediaSource.CreateFromStorageFile(testFile);
            player.Play();
            if (createTask.AsTask().Wait(5000))
            {
                var source = createTask.GetResults();
                Assert.IsNotNull(source);
                Assert.IsInstanceOfType(source, typeof(AudioVisualizer.MftAnalyzer));
            }
            else
            {
                Assert.Fail("Timeout when waiting for the source creation");
            }
        }

        [UITestMethod]
        public void VisualizerSource_CreateFromMediaElement()
        {
            var mediaElement = new MediaElement();
            var createTask = AudioVisualizer.VisualizationSource.CreateFromMediaElementAsync(mediaElement);
            mediaElement.Source = new Uri("ms-appx:///TestContent/test_signal.mp3");
            
            if (createTask.AsTask().Wait(5000))
            {
                var source = createTask.GetResults();
                Assert.IsNotNull(source);
                Assert.IsInstanceOfType(source, typeof(AudioVisualizer.MftAnalyzer));
            }
            else
            {
                Assert.Fail("Timeout when waiting for the source creation");
            }
        }
    }
}
