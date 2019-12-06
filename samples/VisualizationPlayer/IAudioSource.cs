using AudioVisualizer;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VisualizationPlayer
{
    public interface IAudioSource : INotifyPropertyChanged
    {
        IVisualizationSource Source { get; }
    }
}
