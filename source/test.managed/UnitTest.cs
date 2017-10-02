
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Threading.Tasks;
using Windows.Media.Playback;
using Windows.Storage;
using Windows.Media.Core;

namespace test.managed
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public async Task VisualizerSource_CreateFromMediaPlayer()
        {
            var player = new MediaPlayer();
            var createTask = AudioVisualizer.VisualizationSource.CreateFromMediaPlayerAsync(player);
            var testFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx://TestContent/test_signal.mp3"));
            player.Source = MediaSource.CreateFromStorageFile(testFile);
            player.Play();
            var source = await createTask;
            Assert.IsNotNull(source);

        }
    }
}
