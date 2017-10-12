using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation.Diagnostics;

namespace VisualizerPlayer
{
    class Trace
    {
        private static LoggingChannel g_LoggingChannel;

        public static void Initialize()
        {
            g_LoggingChannel = new LoggingChannel("AudioVisualizer-Application", new LoggingChannelOptions());
        }

        public static LoggingActivity BaseVisualizer_StartDraw()
        {
            return g_LoggingChannel.StartActivity("Draw");
        }
    }
}
