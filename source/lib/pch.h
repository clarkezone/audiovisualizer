#pragma once

#define _TRACE // Enable ETW tracing

#ifdef _DEBUG
	//#define _TRACE_LOCKS	// Trace locking
	#define ENABLE_LIFESPAN_TRACKER
	#define _CRTDBG_MAP_ALLOC 
#endif


#ifndef NOMINMAX
	#define NOMINMAX
#endif 

#include <stdlib.h>  
#include <crtdbg.h>

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <wrl.h>

#include <DirectXMath.h>
#include <mfapi.h>
#include <mftransform.h>
#include <mfidl.h>
#include <mferror.h>
#include <windows.foundation.h>
#include <windows.ui.xaml.h>
#include <windows.ui.xaml.controls.h>
#include <windows.ui.composition.h>
#include <windows.ui.xaml.hosting.h>
#include <windows.media.audio.h>
#include <windows.media.mediaproperties.h>
#include <Microsoft.Graphics.Canvas.h>
#include "AudioVisualizer.abi.h"
