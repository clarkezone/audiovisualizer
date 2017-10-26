#include "pch.h"
#include "BaseVisualizer.h"
#include <windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.h>
#include "utilities.h"
#include "ErrorHandling.h"
#include <stdio.h>
#include <Microsoft.Graphics.Canvas.h>
#include <wrl.h>
#include <Nullable.h>
#include <windows.system.threading.h>
#include "VisualizationDataFrame.h"
#include "VisualizerDrawEventArgs.h"
#include "trace.h"
#include <crtdbg.h>

using namespace ABI::Windows::UI::Xaml::Hosting;
using namespace ABI::Windows::UI::Composition;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Microsoft::Graphics::Canvas;
using namespace ABI::Microsoft::Graphics::Canvas::UI::Composition;
using namespace ABI::Windows::System::Threading;
using namespace std;

namespace AudioVisualizer
{

}
