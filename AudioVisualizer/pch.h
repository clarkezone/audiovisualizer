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
#include "winrt/Windows.Foundation.h"
#include <mfapi.h>
#include <mftransform.h>
#include <mfidl.h>
#include <mferror.h>
#include "util.h"
