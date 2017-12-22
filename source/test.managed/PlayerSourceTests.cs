
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Threading.Tasks;
using Windows.Media.Playback;
using Windows.Storage;
using Windows.Media.Core;
using Windows.UI.Xaml.Controls;
using Microsoft.VisualStudio.TestTools.UnitTesting.AppContainer;
using System.Threading;
using Windows.Foundation;
using AudioVisualizer;
using System.Collections.Generic;

namespace test.managed
{
    [TestClass]
    public class PlayerSourceTests
    {
        [TestMethod]
        public async Task PlayerSource_MediaPlayer()
        {
            var testFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///TestContent/test_signal.mp3"));
            var player = new MediaPlayer();

            var playerSource = new AudioVisualizer.PlaybackSource(player);
            List<IVisualizationSource> sources = new List<IVisualizationSource>();
            ManualResetEventSlim ev = new ManualResetEventSlim();

            playerSource.SourceChanged += new TypedEventHandler<object, IVisualizationSource>(
                (sender,source)=>
                {
                    sources.Add(source);
                    ev.Set();
                }
                );
            player.Source = MediaSource.CreateFromStorageFile(testFile);
            player.Play();

            if (Task.Run(() => { ev.Wait(); }).Wait(1000))

            {
                Assert.IsTrue(sources.Count == 1);
                Assert.IsNotNull(sources[0]);
                Assert.IsInstanceOfType(sources[0], typeof(AudioVisualizer.MediaAnalyzer));
            }
            else
                Assert.Fail("Timeout when waiting for the source creation");
        }

    }
}
