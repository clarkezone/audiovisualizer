//
// pch.h
// Header for platform projection include files
//

#pragma once

#define NOMINMAX
#ifdef _DEBUG
	#define _TRACE_
#endif

#include <Unknwn.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>

#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.MediaProperties.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>
#include <winrt/Microsoft.Graphics.Canvas.Geometry.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Composition.h>

#include <memorybuffer.h>

#include <mutex>
#include <shared_mutex>

#include <mfapi.h>
#include <mftransform.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>

#include <DirectXMath.h>
#include "XDSP.h"

#include "util.h"
