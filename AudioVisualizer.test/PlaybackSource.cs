
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
using Windows.Media.Audio;
using Windows.Media.Effects;
using Windows.Media.MediaProperties;
using System.Collections.ObjectModel;
using Windows.Media;

namespace AudioVisualizer.test
{


    [TestClass]
    public class PlayerSourceTests
    {
        [TestMethod]
        [TestCategory("PlaybackSource")]
        public async Task PlaybackSource_MediaPlayer()
        {
            var testFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///TestContent/test_signal.mp3"));
            var player = new MediaPlayer();

            var playerSource = PlaybackSource.CreateFromMediaPlayer(player);
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

        // TODO: This test does not work from VM due to missing audio adapter
        //[TestMethod]
        //[TestCategory("PlaybackSource")]
        private async Task PlaybackSource_CreateFromAudioNode() // TODO make this test work for VM
        {
            var graphResult = await AudioGraph.CreateAsync(new AudioGraphSettings(Windows.Media.Render.AudioRenderCategory.Media));
            if (graphResult.Status == AudioGraphCreationStatus.Success)
            {
                var node = graphResult.Graph.CreateFrameInputNode();
                var source = PlaybackSource.CreateFromAudioNode(node);
                graphResult.Graph.Start();
                Assert.IsNotNull(source.Source);
            } else
            {
                Assert.Inconclusive($"Not able conduct test, could not activate audio graph, result = {graphResult.Status}, {graphResult.ExtendedError.Message}");
            }
        }
   }
 
}
