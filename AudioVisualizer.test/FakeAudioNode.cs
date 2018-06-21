using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using Windows.Foundation;
using Windows.Media.Audio;
using Windows.Media.Effects;
using Windows.Media.MediaProperties;

namespace AudioVisualizer.test
{
    class FakeAudioNode : IAudioNode
    {

        public FakeAudioNode()
        {
            _effects = new ObservableCollection<IAudioEffectDefinition>();
            _effects.CollectionChanged += Effects_CollectionChanged;
        }

        private void Effects_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            switch (e.Action)
            {
                case System.Collections.Specialized.NotifyCollectionChangedAction.Add:
                    foreach (var item in e.NewItems)
                    {
                        IAudioEffectDefinition definition = (IAudioEffectDefinition)item;
                        CreateEffect(definition);
                    }
                    break;
                case System.Collections.Specialized.NotifyCollectionChangedAction.Move:
                    break;
                case System.Collections.Specialized.NotifyCollectionChangedAction.Remove:
                    break;
                case System.Collections.Specialized.NotifyCollectionChangedAction.Replace:
                    break;
                case System.Collections.Specialized.NotifyCollectionChangedAction.Reset:
                    break;
                default:
                    break;
            }
        }

        public void Start()
        {
            throw new NotImplementedException();
        }

        public void Stop()
        {
            throw new NotImplementedException();
        }

        public void Reset()
        {
            throw new NotImplementedException();
        }

        public void DisableEffectsByDefinition(IAudioEffectDefinition definition)
        {
            throw new NotImplementedException();
        }

        public void EnableEffectsByDefinition(IAudioEffectDefinition definition)
        {
            throw new NotImplementedException();
        }

        public bool ConsumeInput { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        private ObservableCollection<IAudioEffectDefinition> _effects;
        public IList<IAudioEffectDefinition> EffectDefinitions => _effects;

        public AudioEncodingProperties EncodingProperties => throw new NotImplementedException();

        public double OutgoingGain { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public void Dispose()
        {
            throw new NotImplementedException();
        }
        // Mock the audio stack effect creation
        void CreateEffect(IAudioEffectDefinition effectDefinition)
        {
            EffectAdded?.Invoke(this, effectDefinition);
        }

        public IBasicAudioEffect CreateAudioEffect(IAudioEffectDefinition effectDefinition)
        {
            var objectType = Type.GetType($"{effectDefinition.ActivatableClassId},AudioVisualizer, ContentType = WindowsRuntime", true);
            return (IBasicAudioEffect)Activator.CreateInstance(objectType);
        }

        public event TypedEventHandler<FakeAudioNode, IAudioEffectDefinition> EffectAdded;
    }
}
