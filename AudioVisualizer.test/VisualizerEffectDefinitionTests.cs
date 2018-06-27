using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Media.Effects;

namespace AudioVisualizer.test
{
    [TestClass]
    public class VisualizerEffectDefinitionTests
    {
        FakeAudioNode _audioNode;

        [TestInitialize]
        public void TestInit()
        {
            _audioNode = new FakeAudioNode();
        }

        [TestMethod]
        [TestCategory("VisualizerEffectDefinition")]
        public void PlaybackSource_IsAudioEffectDefinitionClassIdCorrect()
        {
            string effectName = "";
            _audioNode.EffectAdded += new TypedEventHandler<FakeAudioNode, IAudioEffectDefinition>(
                (node, effectDefinition) =>
                {
                    effectName = effectDefinition.ActivatableClassId;
                }
                );
            PlaybackSource.CreateFromAudioNode(_audioNode);
            Assert.AreEqual(typeof(VisualizerAudioEffect).FullName, effectName);
        }

        [TestMethod]
        [TestCategory("VisualizerEffectDefinition")]
        public void PlaybackSource_IsAudioEffectDefinitionPropertiesNotNull()
        {
            Windows.Foundation.Collections.IPropertySet props = null;
            _audioNode.EffectAdded += new TypedEventHandler<FakeAudioNode, IAudioEffectDefinition>(
                (node, effectDefinition) =>
                {
                    props = effectDefinition.Properties;
                }
                );
            PlaybackSource.CreateFromAudioNode(_audioNode);
            Assert.IsNotNull(props);
        }
    }
}

