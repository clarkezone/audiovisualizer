

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 05:14:07 2038
 */
/* Compiler settings for C:\Users\tonuv\AppData\Local\Temp\AudioVisualizer.idl-6246ad6d:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AudioVisualizer2Eabi_h__
#define __AudioVisualizer2Eabi_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__cplusplus)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
#endif


/* Forward Declarations */ 

#ifndef ____FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__
typedef interface __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource;

#endif 	/* ____FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__ */


#ifndef ____FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__
#define ____FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__
typedef interface __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource;

#endif 	/* ____FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__ */


#ifndef ____FIIterator_1___FIVectorView_1_float_FWD_DEFINED__
#define ____FIIterator_1___FIVectorView_1_float_FWD_DEFINED__
typedef interface __FIIterator_1___FIVectorView_1_float __FIIterator_1___FIVectorView_1_float;

#endif 	/* ____FIIterator_1___FIVectorView_1_float_FWD_DEFINED__ */


#ifndef ____FIIterable_1___FIVectorView_1_float_FWD_DEFINED__
#define ____FIIterable_1___FIVectorView_1_float_FWD_DEFINED__
typedef interface __FIIterable_1___FIVectorView_1_float __FIIterable_1___FIVectorView_1_float;

#endif 	/* ____FIIterable_1___FIVectorView_1_float_FWD_DEFINED__ */


#ifndef ____FIVectorView_1___FIVectorView_1_float_FWD_DEFINED__
#define ____FIVectorView_1___FIVectorView_1_float_FWD_DEFINED__
typedef interface __FIVectorView_1___FIVectorView_1_float __FIVectorView_1___FIVectorView_1_float;

#endif 	/* ____FIVectorView_1___FIVectorView_1_float_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizerDrawEventArgs;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizerDrawEventArgsFactory;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIVisualizer_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizer_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizer __x_ABI_CAudioVisualizer_CIVisualizer;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizer;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizer_FWD_DEFINED__ */


#ifndef ____FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_FWD_DEFINED__
#define ____FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_FWD_DEFINED__
typedef interface __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs;

#endif 	/* ____FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizerCustomDraw;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIDiscreteVUBar_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIDiscreteVUBar_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIDiscreteVUBar __x_ABI_CAudioVisualizer_CIDiscreteVUBar;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IDiscreteVUBar;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIDiscreteVUBar_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIArrayData_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIArrayData_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIArrayData __x_ABI_CAudioVisualizer_CIArrayData;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IArrayData;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIArrayData_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIScalarData_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIScalarData_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIScalarData __x_ABI_CAudioVisualizer_CIScalarData;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IScalarData;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIScalarData_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizationDataFrame __x_ABI_CAudioVisualizer_CIVisualizationDataFrame;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizationDataFrame;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIAudioAnalyzer_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIAudioAnalyzer_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIAudioAnalyzer __x_ABI_CAudioVisualizer_CIAudioAnalyzer;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IAudioAnalyzer;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIAudioAnalyzer_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizationSourceStatics;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationSource_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationSource_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizationSource __x_ABI_CAudioVisualizer_CIVisualizationSource;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizationSource;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationSource_FWD_DEFINED__ */


/* header files for imported files */
#include "inspectable.h"
#include "EventToken.h"
#include "AsyncInfo.h"
#include "Windows.Foundation.h"
#include "Windows.Media.h"
#include "Windows.UI.Xaml.Controls.h"
#include "Windows.Media.Audio.h"
#include "Canvas.abi.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_AudioVisualizer_0000_0000 */
/* [local] */ 

#ifdef __cplusplus
} /*extern "C"*/ 
#endif
#include <windows.foundation.collections.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
interface IVisualizationSource;
} /*AudioVisualizer*/
}
#endif


/* interface __MIDL_itf_AudioVisualizer_0000_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0000_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5620 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5620 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5620_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5620_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0001 */
/* [local] */ 

#ifndef DEF___FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_USE
#define DEF___FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("75edd8f3-d828-5f63-8901-976bd84d88cd"))
IAsyncOperationCompletedHandler<ABI::AudioVisualizer::IVisualizationSource*> : IAsyncOperationCompletedHandler_impl<ABI::AudioVisualizer::IVisualizationSource*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.AsyncOperationCompletedHandler`1<AudioVisualizer.IVisualizationSource>"; }
};
typedef IAsyncOperationCompletedHandler<ABI::AudioVisualizer::IVisualizationSource*> __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_t;
#define ____FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__
#define __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource ABI::Windows::Foundation::__FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_USE */


/* interface __MIDL_itf_AudioVisualizer_0000_0001 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0001_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5621 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5621 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5621_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5621_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0002 */
/* [local] */ 

#ifndef DEF___FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_USE
#define DEF___FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("946be94e-bee2-54d4-8b92-6746772a07d0"))
IAsyncOperation<ABI::AudioVisualizer::IVisualizationSource*> : IAsyncOperation_impl<ABI::AudioVisualizer::IVisualizationSource*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.IAsyncOperation`1<AudioVisualizer.IVisualizationSource>"; }
};
typedef IAsyncOperation<ABI::AudioVisualizer::IVisualizationSource*> __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_t;
#define ____FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_FWD_DEFINED__
#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource ABI::Windows::Foundation::__FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_USE */


/* interface __MIDL_itf_AudioVisualizer_0000_0002 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0002_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5622 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5622 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5622_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5622_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0003 */
/* [local] */ 

#ifndef DEF___FIIterator_1_float_USE
#define DEF___FIIterator_1_float_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("42614e61-b0aa-5e72-9354-2771db20b7a8"))
IIterator<float> : IIterator_impl<float> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.Collections.IIterator`1<Single>"; }
};
typedef IIterator<float> __FIIterator_1_float_t;
#define ____FIIterator_1_float_FWD_DEFINED__
#define __FIIterator_1_float ABI::Windows::Foundation::Collections::__FIIterator_1_float_t

/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIIterator_1_float_USE */


/* interface __MIDL_itf_AudioVisualizer_0000_0003 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0003_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5623 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5623 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5623_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5623_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0004 */
/* [local] */ 

#ifndef DEF___FIIterable_1_float_USE
#define DEF___FIIterable_1_float_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("b01bee51-063a-5fda-bd72-d76637bb8cb8"))
IIterable<float> : IIterable_impl<float> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.Collections.IIterable`1<Single>"; }
};
typedef IIterable<float> __FIIterable_1_float_t;
#define ____FIIterable_1_float_FWD_DEFINED__
#define __FIIterable_1_float ABI::Windows::Foundation::Collections::__FIIterable_1_float_t

/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIIterable_1_float_USE */


/* interface __MIDL_itf_AudioVisualizer_0000_0004 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0004_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5624 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5624 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5624_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5624_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0005 */
/* [local] */ 

#ifndef DEF___FIVectorView_1_float_USE
#define DEF___FIVectorView_1_float_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("7bca64fd-150c-5d50-b56b-9f4f474c5930"))
IVectorView<float> : IVectorView_impl<float> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.Collections.IVectorView`1<Single>"; }
};
typedef IVectorView<float> __FIVectorView_1_float_t;
#define ____FIVectorView_1_float_FWD_DEFINED__
#define __FIVectorView_1_float ABI::Windows::Foundation::Collections::__FIVectorView_1_float_t

/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIVectorView_1_float_USE */


/* interface __MIDL_itf_AudioVisualizer_0000_0005 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0005_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5625 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5625 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5625_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5625_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0006 */
/* [local] */ 

#ifndef DEF___FIIterator_1___FIVectorView_1_float_USE
#define DEF___FIIterator_1___FIVectorView_1_float_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("5d610cb8-261f-52fd-8422-33ec263885d8"))
IIterator<__FIVectorView_1_float*> : IIterator_impl<__FIVectorView_1_float*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.Collections.IIterator`1<Windows.Foundation.Collections.IVectorView`1<Single>>"; }
};
typedef IIterator<__FIVectorView_1_float*> __FIIterator_1___FIVectorView_1_float_t;
#define ____FIIterator_1___FIVectorView_1_float_FWD_DEFINED__
#define __FIIterator_1___FIVectorView_1_float ABI::Windows::Foundation::Collections::__FIIterator_1___FIVectorView_1_float_t

/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIIterator_1___FIVectorView_1_float_USE */


/* interface __MIDL_itf_AudioVisualizer_0000_0006 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0006_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5626 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5626 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5626_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5626_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0007 */
/* [local] */ 

#ifndef DEF___FIIterable_1___FIVectorView_1_float_USE
#define DEF___FIIterable_1___FIVectorView_1_float_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("e94b39c6-f5f7-566e-8a81-a54df7b7d7c3"))
IIterable<__FIVectorView_1_float*> : IIterable_impl<__FIVectorView_1_float*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.Collections.IIterable`1<Windows.Foundation.Collections.IVectorView`1<Single>>"; }
};
typedef IIterable<__FIVectorView_1_float*> __FIIterable_1___FIVectorView_1_float_t;
#define ____FIIterable_1___FIVectorView_1_float_FWD_DEFINED__
#define __FIIterable_1___FIVectorView_1_float ABI::Windows::Foundation::Collections::__FIIterable_1___FIVectorView_1_float_t

/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIIterable_1___FIVectorView_1_float_USE */


/* interface __MIDL_itf_AudioVisualizer_0000_0007 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0007_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0007_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5627 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5627 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5627_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5627_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0008 */
/* [local] */ 

#ifndef DEF___FIVectorView_1___FIVectorView_1_float_USE
#define DEF___FIVectorView_1___FIVectorView_1_float_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("0543e4d4-0a98-51dd-8c1d-fe6607f07378"))
IVectorView<__FIVectorView_1_float*> : IVectorView_impl<__FIVectorView_1_float*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.Collections.IVectorView`1<Windows.Foundation.Collections.IVectorView`1<Single>>"; }
};
typedef IVectorView<__FIVectorView_1_float*> __FIVectorView_1___FIVectorView_1_float_t;
#define ____FIVectorView_1___FIVectorView_1_float_FWD_DEFINED__
#define __FIVectorView_1___FIVectorView_1_float ABI::Windows::Foundation::Collections::__FIVectorView_1___FIVectorView_1_float_t

/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIVectorView_1___FIVectorView_1_float_USE */

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
interface IVisualizer;
} /*AudioVisualizer*/
}
#endif
#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class VisualizerDrawEventArgs;
} /*AudioVisualizer*/
}
#endif

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
interface IVisualizerDrawEventArgs;
} /*AudioVisualizer*/
}
#endif


/* interface __MIDL_itf_AudioVisualizer_0000_0008 */
/* [local] */ 






extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0008_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0008_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5628 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5628 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5628_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5628_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0009 */
/* [local] */ 

#ifndef DEF___FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_USE
#define DEF___FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("5bf4c984-645f-5ef1-87ab-1134ea102187"))
ITypedEventHandler<ABI::AudioVisualizer::IVisualizer*,ABI::AudioVisualizer::VisualizerDrawEventArgs*> : ITypedEventHandler_impl<ABI::AudioVisualizer::IVisualizer*,ABI::Windows::Foundation::Internal::AggregateType<ABI::AudioVisualizer::VisualizerDrawEventArgs*, ABI::AudioVisualizer::IVisualizerDrawEventArgs*>> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.TypedEventHandler`2<AudioVisualizer.IVisualizer, AudioVisualizer.VisualizerDrawEventArgs>"; }
};
typedef ITypedEventHandler<ABI::AudioVisualizer::IVisualizer*,ABI::AudioVisualizer::VisualizerDrawEventArgs*> __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_t;
#define ____FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_FWD_DEFINED__
#define __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs ABI::Windows::Foundation::__FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_USE */

#ifdef __cplusplus
namespace ABI {
namespace Windows {
namespace Foundation {
struct TimeSpan;
} /*Foundation*/
} /*Windows*/
}
#endif


/* interface __MIDL_itf_AudioVisualizer_0000_0009 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0009_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0009_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5629 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5629 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5629_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5629_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0010 */
/* [local] */ 

#ifndef DEF___FIReference_1_Windows__CFoundation__CTimeSpan_USE
#define DEF___FIReference_1_Windows__CFoundation__CTimeSpan_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("604d0c4c-91de-5c2a-935f-362f13eaf800"))
IReference<struct ABI::Windows::Foundation::TimeSpan> : IReference_impl<struct ABI::Windows::Foundation::TimeSpan> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.IReference`1<Windows.Foundation.TimeSpan>"; }
};
typedef IReference<struct ABI::Windows::Foundation::TimeSpan> __FIReference_1_Windows__CFoundation__CTimeSpan_t;
#define ____FIReference_1_Windows__CFoundation__CTimeSpan_FWD_DEFINED__
#define __FIReference_1_Windows__CFoundation__CTimeSpan ABI::Windows::Foundation::__FIReference_1_Windows__CFoundation__CTimeSpan_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIReference_1_Windows__CFoundation__CTimeSpan_USE */
#if !defined(__cplusplus)

#if !defined(__cplusplus)
/* [v1_enum] */ 
enum __x_ABI_CAudioVisualizer_CScaleType
    {
        ScaleType_Linear	= ( int  )0,
        ScaleType_Logarithmic	= ( int  )1
    } ;
#endif /* end if !defined(__cplusplus) */

#if !defined(__cplusplus)

typedef enum __x_ABI_CAudioVisualizer_CScaleType __x_ABI_CAudioVisualizer_CScaleType;


#endif /* end if !defined(__cplusplus) */


#endif
#if !defined(__cplusplus)

#if !defined(__cplusplus)
/* [v1_enum] */ 
enum __x_ABI_CAudioVisualizer_CInterpolationType
    {
        InterpolationType_Linear	= ( int  )0,
        InterpolationType_CubicSpline	= ( int  )1
    } ;
#endif /* end if !defined(__cplusplus) */

#if !defined(__cplusplus)

typedef enum __x_ABI_CAudioVisualizer_CInterpolationType __x_ABI_CAudioVisualizer_CInterpolationType;


#endif /* end if !defined(__cplusplus) */


#endif
#if !defined(__cplusplus)

#if !defined(__cplusplus)
/* [v1_enum] */ 
enum __x_ABI_CAudioVisualizer_CAnalyzerType
    {
        AnalyzerType_RMS	= ( int  )1,
        AnalyzerType_Peak	= ( int  )2,
        AnalyzerType_Spectrum	= ( int  )4,
        AnalyzerType_All	= ( int  )7
    } ;
#endif /* end if !defined(__cplusplus) */

#if !defined(__cplusplus)

typedef enum __x_ABI_CAudioVisualizer_CAnalyzerType __x_ABI_CAudioVisualizer_CAnalyzerType;


#endif /* end if !defined(__cplusplus) */


#else
namespace ABI {
namespace AudioVisualizer {
#if defined(MIDL_ENUM)
typedef MIDL_ENUM AnalyzerType AnalyzerType;
#else
typedef enum class AnalyzerType AnalyzerType;
#endif
DEFINE_ENUM_FLAG_OPERATORS(AnalyzerType)
} /*AudioVisualizer*/
}
#endif

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class VisualizationDataFrame;
} /*AudioVisualizer*/
}
#endif
#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class ArrayData;
} /*AudioVisualizer*/
}
#endif

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class ScalarData;
} /*AudioVisualizer*/
}
#endif



/* interface __MIDL_itf_AudioVisualizer_0000_0010 */
/* [local] */ 


#ifdef __cplusplus
} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        /* [v1_enum] */ 
        MIDL_ENUM ScaleType
            {
                Linear	= ( int  )0,
                Logarithmic	= ( int  )1
            } ;

        const MIDL_ENUM ScaleType ScaleType_Linear = ScaleType::Linear;
        const MIDL_ENUM ScaleType ScaleType_Logarithmic = ScaleType::Logarithmic;
        
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif

#ifdef __cplusplus

} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        typedef MIDL_ENUM ScaleType ScaleType;
        
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif

#ifdef __cplusplus
} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        /* [v1_enum] */ 
        MIDL_ENUM InterpolationType
            {
                Linear	= ( int  )0,
                CubicSpline	= ( int  )1
            } ;

        const MIDL_ENUM InterpolationType InterpolationType_Linear = InterpolationType::Linear;
        const MIDL_ENUM InterpolationType InterpolationType_CubicSpline = InterpolationType::CubicSpline;
        
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif

#ifdef __cplusplus

} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        typedef MIDL_ENUM InterpolationType InterpolationType;
        
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif

#ifdef __cplusplus
} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        /* [v1_enum] */ 
        MIDL_ENUM AnalyzerType
            {
                RMS	= ( int  )1,
                Peak	= ( int  )2,
                Spectrum	= ( int  )4,
                All	= ( int  )7
            } ;

        const MIDL_ENUM AnalyzerType AnalyzerType_RMS = AnalyzerType::RMS;
        const MIDL_ENUM AnalyzerType AnalyzerType_Peak = AnalyzerType::Peak;
        const MIDL_ENUM AnalyzerType AnalyzerType_Spectrum = AnalyzerType::Spectrum;
        const MIDL_ENUM AnalyzerType AnalyzerType_All = AnalyzerType::All;
        
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif

#ifdef __cplusplus

} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        typedef MIDL_ENUM AnalyzerType AnalyzerType;
        
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif






extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0010_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0010_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5630 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5630 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5630_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5630_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0011 */
/* [local] */ 

#ifndef DEF___FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource
#define DEF___FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)



/* interface __MIDL_itf_AudioVisualizer_0000_0011 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0011_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0011_v0_0_s_ifspec;

#ifndef ____FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_INTERFACE_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_INTERFACE_DEFINED__

/* interface __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource */
/* [unique][uuid][object] */ 



/* interface __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("75edd8f3-d828-5f63-8901-976bd84d88cd")
    __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource *asyncInfo,
            /* [in] */ AsyncStatus status) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource * This,
            /* [in] */ __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource *asyncInfo,
            /* [in] */ AsyncStatus status);
        
        END_INTERFACE
    } __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSourceVtbl;

    interface __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource
    {
        CONST_VTBL struct __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_Invoke(This,asyncInfo,status)	\
    ( (This)->lpVtbl -> Invoke(This,asyncInfo,status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0012 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource */


/* interface __MIDL_itf_AudioVisualizer_0000_0012 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0012_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0012_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5631 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5631 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5631_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5631_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0013 */
/* [local] */ 

#ifndef DEF___FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource
#define DEF___FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_AudioVisualizer_0000_0013 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0013_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0013_v0_0_s_ifspec;

#ifndef ____FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_INTERFACE_DEFINED__
#define ____FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_INTERFACE_DEFINED__

/* interface __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource */
/* [unique][uuid][object] */ 



/* interface __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("946be94e-bee2-54d4-8b92-6746772a07d0")
    __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource : public IInspectable
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Completed( 
            /* [in] */ __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource *handler) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Completed( 
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource **handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResults( 
            /* [retval][out] */ ABI::AudioVisualizer::IVisualizationSource **results) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Completed )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This,
            /* [in] */ __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource *handler);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Completed )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This,
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_AudioVisualizer__CIVisualizationSource **handler);
        
        HRESULT ( STDMETHODCALLTYPE *GetResults )( 
            __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource * This,
            /* [retval][out] */ __x_ABI_CAudioVisualizer_CIVisualizationSource **results);
        
        END_INTERFACE
    } __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSourceVtbl;

    interface __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource
    {
        CONST_VTBL struct __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_put_Completed(This,handler)	\
    ( (This)->lpVtbl -> put_Completed(This,handler) ) 

#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_get_Completed(This,handler)	\
    ( (This)->lpVtbl -> get_Completed(This,handler) ) 

#define __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_GetResults(This,results)	\
    ( (This)->lpVtbl -> GetResults(This,results) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0014 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource */


/* interface __MIDL_itf_AudioVisualizer_0000_0014 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0014_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0014_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5632 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5632 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5632_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5632_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0015 */
/* [local] */ 

#ifndef DEF___FIIterator_1___FIVectorView_1_float
#define DEF___FIIterator_1___FIVectorView_1_float
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_AudioVisualizer_0000_0015 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0015_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0015_v0_0_s_ifspec;

#ifndef ____FIIterator_1___FIVectorView_1_float_INTERFACE_DEFINED__
#define ____FIIterator_1___FIVectorView_1_float_INTERFACE_DEFINED__

/* interface __FIIterator_1___FIVectorView_1_float */
/* [unique][uuid][object] */ 



/* interface __FIIterator_1___FIVectorView_1_float */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIIterator_1___FIVectorView_1_float;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5d610cb8-261f-52fd-8422-33ec263885d8")
    __FIIterator_1___FIVectorView_1_float : public IInspectable
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Current( 
            /* [retval][out] */ __FIVectorView_1_float **current) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HasCurrent( 
            /* [retval][out] */ boolean *hasCurrent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveNext( 
            /* [retval][out] */ boolean *hasCurrent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMany( 
            /* [in] */ unsigned int capacity,
            /* [size_is][length_is][out] */ __FIVectorView_1_float **items,
            /* [retval][out] */ unsigned int *actual) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIIterator_1___FIVectorView_1_floatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIIterator_1___FIVectorView_1_float * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIIterator_1___FIVectorView_1_float * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Current )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [retval][out] */ __FIVectorView_1_float **current);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HasCurrent )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [retval][out] */ boolean *hasCurrent);
        
        HRESULT ( STDMETHODCALLTYPE *MoveNext )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [retval][out] */ boolean *hasCurrent);
        
        HRESULT ( STDMETHODCALLTYPE *GetMany )( 
            __FIIterator_1___FIVectorView_1_float * This,
            /* [in] */ unsigned int capacity,
            /* [size_is][length_is][out] */ __FIVectorView_1_float **items,
            /* [retval][out] */ unsigned int *actual);
        
        END_INTERFACE
    } __FIIterator_1___FIVectorView_1_floatVtbl;

    interface __FIIterator_1___FIVectorView_1_float
    {
        CONST_VTBL struct __FIIterator_1___FIVectorView_1_floatVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIIterator_1___FIVectorView_1_float_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIIterator_1___FIVectorView_1_float_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIIterator_1___FIVectorView_1_float_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIIterator_1___FIVectorView_1_float_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIIterator_1___FIVectorView_1_float_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIIterator_1___FIVectorView_1_float_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIIterator_1___FIVectorView_1_float_get_Current(This,current)	\
    ( (This)->lpVtbl -> get_Current(This,current) ) 

#define __FIIterator_1___FIVectorView_1_float_get_HasCurrent(This,hasCurrent)	\
    ( (This)->lpVtbl -> get_HasCurrent(This,hasCurrent) ) 

#define __FIIterator_1___FIVectorView_1_float_MoveNext(This,hasCurrent)	\
    ( (This)->lpVtbl -> MoveNext(This,hasCurrent) ) 

#define __FIIterator_1___FIVectorView_1_float_GetMany(This,capacity,items,actual)	\
    ( (This)->lpVtbl -> GetMany(This,capacity,items,actual) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIIterator_1___FIVectorView_1_float_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0016 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIIterator_1___FIVectorView_1_float */


/* interface __MIDL_itf_AudioVisualizer_0000_0016 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0016_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0016_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5633 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5633 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5633_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5633_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0017 */
/* [local] */ 

#ifndef DEF___FIIterable_1___FIVectorView_1_float
#define DEF___FIIterable_1___FIVectorView_1_float
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_AudioVisualizer_0000_0017 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0017_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0017_v0_0_s_ifspec;

#ifndef ____FIIterable_1___FIVectorView_1_float_INTERFACE_DEFINED__
#define ____FIIterable_1___FIVectorView_1_float_INTERFACE_DEFINED__

/* interface __FIIterable_1___FIVectorView_1_float */
/* [unique][uuid][object] */ 



/* interface __FIIterable_1___FIVectorView_1_float */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIIterable_1___FIVectorView_1_float;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e94b39c6-f5f7-566e-8a81-a54df7b7d7c3")
    __FIIterable_1___FIVectorView_1_float : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE First( 
            /* [retval][out] */ __FIIterator_1___FIVectorView_1_float **first) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIIterable_1___FIVectorView_1_floatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIIterable_1___FIVectorView_1_float * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIIterable_1___FIVectorView_1_float * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIIterable_1___FIVectorView_1_float * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __FIIterable_1___FIVectorView_1_float * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __FIIterable_1___FIVectorView_1_float * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __FIIterable_1___FIVectorView_1_float * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *First )( 
            __FIIterable_1___FIVectorView_1_float * This,
            /* [retval][out] */ __FIIterator_1___FIVectorView_1_float **first);
        
        END_INTERFACE
    } __FIIterable_1___FIVectorView_1_floatVtbl;

    interface __FIIterable_1___FIVectorView_1_float
    {
        CONST_VTBL struct __FIIterable_1___FIVectorView_1_floatVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIIterable_1___FIVectorView_1_float_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIIterable_1___FIVectorView_1_float_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIIterable_1___FIVectorView_1_float_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIIterable_1___FIVectorView_1_float_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIIterable_1___FIVectorView_1_float_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIIterable_1___FIVectorView_1_float_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIIterable_1___FIVectorView_1_float_First(This,first)	\
    ( (This)->lpVtbl -> First(This,first) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIIterable_1___FIVectorView_1_float_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0018 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIIterable_1___FIVectorView_1_float */


/* interface __MIDL_itf_AudioVisualizer_0000_0018 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0018_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0018_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5634 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5634 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5634_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5634_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0019 */
/* [local] */ 

#ifndef DEF___FIVectorView_1___FIVectorView_1_float
#define DEF___FIVectorView_1___FIVectorView_1_float
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_AudioVisualizer_0000_0019 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0019_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0019_v0_0_s_ifspec;

#ifndef ____FIVectorView_1___FIVectorView_1_float_INTERFACE_DEFINED__
#define ____FIVectorView_1___FIVectorView_1_float_INTERFACE_DEFINED__

/* interface __FIVectorView_1___FIVectorView_1_float */
/* [unique][uuid][object] */ 



/* interface __FIVectorView_1___FIVectorView_1_float */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIVectorView_1___FIVectorView_1_float;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0543e4d4-0a98-51dd-8c1d-fe6607f07378")
    __FIVectorView_1___FIVectorView_1_float : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetAt( 
            /* [in] */ unsigned int index,
            /* [retval][out] */ __FIVectorView_1_float **item) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Size( 
            /* [retval][out] */ unsigned int *size) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IndexOf( 
            /* [in] */ __FIVectorView_1_float *item,
            /* [out] */ unsigned int *index,
            /* [retval][out] */ boolean *found) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMany( 
            /* [in] */ unsigned int startIndex,
            /* [in] */ unsigned int capacity,
            /* [size_is][length_is][out] */ __FIVectorView_1_float **items,
            /* [retval][out] */ unsigned int *actual) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIVectorView_1___FIVectorView_1_floatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIVectorView_1___FIVectorView_1_float * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIVectorView_1___FIVectorView_1_float * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *GetAt )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [in] */ unsigned int index,
            /* [retval][out] */ __FIVectorView_1_float **item);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Size )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [retval][out] */ unsigned int *size);
        
        HRESULT ( STDMETHODCALLTYPE *IndexOf )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [in] */ __FIVectorView_1_float *item,
            /* [out] */ unsigned int *index,
            /* [retval][out] */ boolean *found);
        
        HRESULT ( STDMETHODCALLTYPE *GetMany )( 
            __FIVectorView_1___FIVectorView_1_float * This,
            /* [in] */ unsigned int startIndex,
            /* [in] */ unsigned int capacity,
            /* [size_is][length_is][out] */ __FIVectorView_1_float **items,
            /* [retval][out] */ unsigned int *actual);
        
        END_INTERFACE
    } __FIVectorView_1___FIVectorView_1_floatVtbl;

    interface __FIVectorView_1___FIVectorView_1_float
    {
        CONST_VTBL struct __FIVectorView_1___FIVectorView_1_floatVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIVectorView_1___FIVectorView_1_float_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIVectorView_1___FIVectorView_1_float_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIVectorView_1___FIVectorView_1_float_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIVectorView_1___FIVectorView_1_float_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIVectorView_1___FIVectorView_1_float_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIVectorView_1___FIVectorView_1_float_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIVectorView_1___FIVectorView_1_float_GetAt(This,index,item)	\
    ( (This)->lpVtbl -> GetAt(This,index,item) ) 

#define __FIVectorView_1___FIVectorView_1_float_get_Size(This,size)	\
    ( (This)->lpVtbl -> get_Size(This,size) ) 

#define __FIVectorView_1___FIVectorView_1_float_IndexOf(This,item,index,found)	\
    ( (This)->lpVtbl -> IndexOf(This,item,index,found) ) 

#define __FIVectorView_1___FIVectorView_1_float_GetMany(This,startIndex,capacity,items,actual)	\
    ( (This)->lpVtbl -> GetMany(This,startIndex,capacity,items,actual) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIVectorView_1___FIVectorView_1_float_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0020 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIVectorView_1___FIVectorView_1_float */
#if !defined(____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizerDrawEventArgs[] = L"AudioVisualizer.IVisualizerDrawEventArgs";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0020 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0020_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0020_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizerDrawEventArgs */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("62C7FB4D-4712-4A49-BF0F-597AC7079F3A")
            IVisualizerDrawEventArgs : public IInspectable
            {
            public:
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
                    /* [out][retval] */ ABI::AudioVisualizer::IVisualizationDataFrame **pData) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DrawingSession( 
                    /* [out][retval] */ ABI::Microsoft::Graphics::Canvas::ICanvasDrawingSession **pSession) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizerDrawEventArgs = __uuidof(IVisualizerDrawEventArgs);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIVisualizationDataFrame **pData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DrawingSession )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs * This,
            /* [out][retval] */ __x_ABI_CMicrosoft_CGraphics_CCanvas_CICanvasDrawingSession **pSession);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_get_Data(This,pData)	\
    ( (This)->lpVtbl -> get_Data(This,pData) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_get_DrawingSession(This,pSession)	\
    ( (This)->lpVtbl -> get_DrawingSession(This,pSession) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0021 */
/* [local] */ 

#if !defined(____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizerDrawEventArgsFactory[] = L"AudioVisualizer.IVisualizerDrawEventArgsFactory";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0021 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0021_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0021_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizerDrawEventArgsFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("7F644CA7-6BD9-455D-A330-BD5F4E9A6DA5")
            IVisualizerDrawEventArgsFactory : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE Create( 
                    /* [in] */ ABI::Microsoft::Graphics::Canvas::ICanvasDrawingSession *canvasDrawingSession,
                    /* [in] */ ABI::AudioVisualizer::IVisualizationDataFrame *data,
                    /* [out][retval] */ ABI::AudioVisualizer::IVisualizerDrawEventArgs **drawEventArgs) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizerDrawEventArgsFactory = __uuidof(IVisualizerDrawEventArgsFactory);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *Create )( 
            __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory * This,
            /* [in] */ __x_ABI_CMicrosoft_CGraphics_CCanvas_CICanvasDrawingSession *canvasDrawingSession,
            /* [in] */ __x_ABI_CAudioVisualizer_CIVisualizationDataFrame *data,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs **drawEventArgs);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactoryVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_Create(This,canvasDrawingSession,data,drawEventArgs)	\
    ( (This)->lpVtbl -> Create(This,canvasDrawingSession,data,drawEventArgs) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgsFactory_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0022 */
/* [local] */ 

#ifndef RUNTIMECLASS_AudioVisualizer_VisualizerDrawEventArgs_DEFINED
#define RUNTIMECLASS_AudioVisualizer_VisualizerDrawEventArgs_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_VisualizerDrawEventArgs[] = L"AudioVisualizer.VisualizerDrawEventArgs";
#endif
#if !defined(____x_ABI_CAudioVisualizer_CIVisualizer_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizer[] = L"AudioVisualizer.IVisualizer";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizer_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0022 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0022_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0022_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizer_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizer_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizer */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizer */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("D3C979CE-1D6D-4507-B7CE-5A2E97F672FC")
            IVisualizer : public IInspectable
            {
            public:
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Source( 
                    /* [out][retval] */ ABI::AudioVisualizer::IVisualizationSource **pData) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Source( 
                    /* [in] */ ABI::AudioVisualizer::IVisualizationSource *pData) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BackgroundColor( 
                    /* [out][retval] */ ABI::Windows::UI::Color *color) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BackgroundColor( 
                    /* [in] */ ABI::Windows::UI::Color color) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizer = __uuidof(IVisualizer);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Source )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIVisualizationSource **pData);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Source )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [in] */ __x_ABI_CAudioVisualizer_CIVisualizationSource *pData);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BackgroundColor )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [out][retval] */ __x_ABI_CWindows_CUI_CColor *color);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_BackgroundColor )( 
            __x_ABI_CAudioVisualizer_CIVisualizer * This,
            /* [in] */ __x_ABI_CWindows_CUI_CColor color);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizerVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizer
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizer_get_Source(This,pData)	\
    ( (This)->lpVtbl -> get_Source(This,pData) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizer_put_Source(This,pData)	\
    ( (This)->lpVtbl -> put_Source(This,pData) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizer_get_BackgroundColor(This,color)	\
    ( (This)->lpVtbl -> get_BackgroundColor(This,color) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizer_put_BackgroundColor(This,color)	\
    ( (This)->lpVtbl -> put_BackgroundColor(This,color) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5635 */




/* interface __MIDL_itf_AudioVisualizer2Eidl_0000_5635 */




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5635_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer2Eidl_0000_5635_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioVisualizer_0000_0024 */
/* [local] */ 

#ifndef DEF___FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs
#define DEF___FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_AudioVisualizer_0000_0024 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0024_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0024_v0_0_s_ifspec;

#ifndef ____FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_INTERFACE_DEFINED__
#define ____FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_INTERFACE_DEFINED__

/* interface __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs */
/* [unique][uuid][object] */ 



/* interface __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5bf4c984-645f-5ef1-87ab-1134ea102187")
    __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ABI::AudioVisualizer::IVisualizer *sender,
            /* [in] */ ABI::AudioVisualizer::IVisualizerDrawEventArgs *e) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs * This,
            /* [in] */ __x_ABI_CAudioVisualizer_CIVisualizer *sender,
            /* [in] */ __x_ABI_CAudioVisualizer_CIVisualizerDrawEventArgs *e);
        
        END_INTERFACE
    } __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgsVtbl;

    interface __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs
    {
        CONST_VTBL struct __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_Invoke(This,sender,e)	\
    ( (This)->lpVtbl -> Invoke(This,sender,e) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0025 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs */
#if !defined(____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizerCustomDraw[] = L"AudioVisualizer.IVisualizerCustomDraw";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0025 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0025_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0025_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizerCustomDraw */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizerCustomDraw;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("11A7C461-9AFB-4AFB-B2E1-6E587D581139")
            IVisualizerCustomDraw : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE add_Draw( 
                    /* [in] */ __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs *value,
                    /* [out][retval] */ EventRegistrationToken *token) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE remove_Draw( 
                    /* [in] */ EventRegistrationToken token) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizerCustomDraw = __uuidof(IVisualizerCustomDraw);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizerCustomDrawVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *add_Draw )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This,
            /* [in] */ __FITypedEventHandler_2_AudioVisualizer__CIVisualizer_AudioVisualizer__CVisualizerDrawEventArgs *value,
            /* [out][retval] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_Draw )( 
            __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw * This,
            /* [in] */ EventRegistrationToken token);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizerCustomDrawVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizerCustomDrawVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_add_Draw(This,value,token)	\
    ( (This)->lpVtbl -> add_Draw(This,value,token) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_remove_Draw(This,token)	\
    ( (This)->lpVtbl -> remove_Draw(This,token) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizerCustomDraw_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0026 */
/* [local] */ 

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class CustomVisualizer;
} /*AudioVisualizer*/
}
#endif
#ifndef RUNTIMECLASS_AudioVisualizer_CustomVisualizer_DEFINED
#define RUNTIMECLASS_AudioVisualizer_CustomVisualizer_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_CustomVisualizer[] = L"AudioVisualizer.CustomVisualizer";
#endif
#if !defined(__cplusplus)
struct __x_ABI_CAudioVisualizer_CMeterBarLevel
    {
    float Level;
    __x_ABI_CWindows_CUI_CColor Color;
    } ;
typedef struct __x_ABI_CAudioVisualizer_CMeterBarLevel __x_ABI_CAudioVisualizer_CMeterBarLevel;

#endif
#if !defined(____x_ABI_CAudioVisualizer_CIDiscreteVUBar_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IDiscreteVUBar[] = L"AudioVisualizer.IDiscreteVUBar";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIDiscreteVUBar_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0026 */
/* [local] */ 

#ifdef __cplusplus
} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        struct MeterBarLevel
            {
            float Level;
            ABI::Windows::UI::Color Color;
            } ;
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif

#ifdef __cplusplus

} /* end extern "C" */
namespace ABI {
    namespace AudioVisualizer {
        
        typedef struct MeterBarLevel MeterBarLevel;
        
    } /* end namespace */
} /* end namespace */

extern "C" { 
#endif



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0026_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0026_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIDiscreteVUBar_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIDiscreteVUBar_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIDiscreteVUBar */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IDiscreteVUBar */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIDiscreteVUBar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("665C9CD0-6AAE-4AAB-B152-82128615641C")
            IDiscreteVUBar : public IInspectable
            {
            public:
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Levels( 
                    /* [out] */ UINT32 *elementCount,
                    /* [out][retval][size_is][size_is] */ ABI::AudioVisualizer::MeterBarLevel **levels) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Levels( 
                    /* [in] */ UINT32 elementCount,
                    /* [in][size_is] */ ABI::AudioVisualizer::MeterBarLevel *levels) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Orientation( 
                    /* [out][retval] */ ABI::Windows::UI::Xaml::Controls::Orientation *orientation) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Orientation( 
                    /* [in] */ ABI::Windows::UI::Xaml::Controls::Orientation orientation) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ChannelCount( 
                    /* [out][retval] */ UINT32 *pChannels) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ChannelCount( 
                    /* [in] */ UINT32 channels) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ElementSize( 
                    /* [out][retval] */ ABI::Windows::Foundation::Size *pSize) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ElementSize( 
                    /* [in] */ ABI::Windows::Foundation::Size size) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ElementMargin( 
                    /* [out][retval] */ ABI::Windows::UI::Xaml::Thickness *pValue) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ElementMargin( 
                    /* [in] */ ABI::Windows::UI::Xaml::Thickness value) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_UnlitElement( 
                    /* [out][retval] */ ABI::Windows::UI::Color *pColor) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_UnlitElement( 
                    /* [in] */ ABI::Windows::UI::Color color) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RiseTime( 
                    /* [out][retval] */ ABI::Windows::Foundation::TimeSpan *time) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_RiseTime( 
                    /* [in] */ ABI::Windows::Foundation::TimeSpan time) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FallTime( 
                    /* [out][retval] */ ABI::Windows::Foundation::TimeSpan *time) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_FallTime( 
                    /* [in] */ ABI::Windows::Foundation::TimeSpan time) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IDiscreteVUBar = __uuidof(IDiscreteVUBar);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIDiscreteVUBarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Levels )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out] */ UINT32 *elementCount,
            /* [out][retval][size_is][size_is] */ __x_ABI_CAudioVisualizer_CMeterBarLevel **levels);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Levels )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ UINT32 elementCount,
            /* [in][size_is] */ __x_ABI_CAudioVisualizer_CMeterBarLevel *levels);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Orientation )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out][retval] */ __x_ABI_CWindows_CUI_CXaml_CControls_COrientation *orientation);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Orientation )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ __x_ABI_CWindows_CUI_CXaml_CControls_COrientation orientation);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ChannelCount )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out][retval] */ UINT32 *pChannels);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ChannelCount )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ UINT32 channels);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ElementSize )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out][retval] */ __x_ABI_CWindows_CFoundation_CSize *pSize);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ElementSize )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ __x_ABI_CWindows_CFoundation_CSize size);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ElementMargin )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out][retval] */ __x_ABI_CWindows_CUI_CXaml_CThickness *pValue);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ElementMargin )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ __x_ABI_CWindows_CUI_CXaml_CThickness value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_UnlitElement )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out][retval] */ __x_ABI_CWindows_CUI_CColor *pColor);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_UnlitElement )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ __x_ABI_CWindows_CUI_CColor color);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RiseTime )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out][retval] */ __x_ABI_CWindows_CFoundation_CTimeSpan *time);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_RiseTime )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan time);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FallTime )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [out][retval] */ __x_ABI_CWindows_CFoundation_CTimeSpan *time);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_FallTime )( 
            __x_ABI_CAudioVisualizer_CIDiscreteVUBar * This,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan time);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIDiscreteVUBarVtbl;

    interface __x_ABI_CAudioVisualizer_CIDiscreteVUBar
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIDiscreteVUBarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_Levels(This,elementCount,levels)	\
    ( (This)->lpVtbl -> get_Levels(This,elementCount,levels) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_Levels(This,elementCount,levels)	\
    ( (This)->lpVtbl -> put_Levels(This,elementCount,levels) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_Orientation(This,orientation)	\
    ( (This)->lpVtbl -> get_Orientation(This,orientation) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_Orientation(This,orientation)	\
    ( (This)->lpVtbl -> put_Orientation(This,orientation) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_ChannelCount(This,pChannels)	\
    ( (This)->lpVtbl -> get_ChannelCount(This,pChannels) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_ChannelCount(This,channels)	\
    ( (This)->lpVtbl -> put_ChannelCount(This,channels) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_ElementSize(This,pSize)	\
    ( (This)->lpVtbl -> get_ElementSize(This,pSize) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_ElementSize(This,size)	\
    ( (This)->lpVtbl -> put_ElementSize(This,size) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_ElementMargin(This,pValue)	\
    ( (This)->lpVtbl -> get_ElementMargin(This,pValue) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_ElementMargin(This,value)	\
    ( (This)->lpVtbl -> put_ElementMargin(This,value) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_UnlitElement(This,pColor)	\
    ( (This)->lpVtbl -> get_UnlitElement(This,pColor) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_UnlitElement(This,color)	\
    ( (This)->lpVtbl -> put_UnlitElement(This,color) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_RiseTime(This,time)	\
    ( (This)->lpVtbl -> get_RiseTime(This,time) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_RiseTime(This,time)	\
    ( (This)->lpVtbl -> put_RiseTime(This,time) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_get_FallTime(This,time)	\
    ( (This)->lpVtbl -> get_FallTime(This,time) ) 

#define __x_ABI_CAudioVisualizer_CIDiscreteVUBar_put_FallTime(This,time)	\
    ( (This)->lpVtbl -> put_FallTime(This,time) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIDiscreteVUBar_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0027 */
/* [local] */ 

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class DiscreteVUBar;
} /*AudioVisualizer*/
}
#endif
#ifndef RUNTIMECLASS_AudioVisualizer_DiscreteVUBar_DEFINED
#define RUNTIMECLASS_AudioVisualizer_DiscreteVUBar_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_DiscreteVUBar[] = L"AudioVisualizer.DiscreteVUBar";
#endif
#if !defined(____x_ABI_CAudioVisualizer_CIArrayData_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IArrayData[] = L"AudioVisualizer.IArrayData";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIArrayData_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0027 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0027_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0027_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIArrayData_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIArrayData_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIArrayData */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IArrayData */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIArrayData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("199F41A4-8EF0-4E0B-AAD3-9E0666511F6A")
            IArrayData : public IInspectable
            {
            public:
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AmplitudeScale( 
                    /* [out][retval] */ ABI::AudioVisualizer::ScaleType *scale) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrequencyScale( 
                    /* [out][retval] */ ABI::AudioVisualizer::ScaleType *scale) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MinFrequency( 
                    /* [out][retval] */ float *value) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MaxFrequency( 
                    /* [out][retval] */ float *value) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrequencyStep( 
                    /* [out][retval] */ float *value) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrequencyCount( 
                    /* [out][retval] */ UINT32 *value) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE TransformLinearFrequency( 
                    UINT32 elementCount,
                    /* [out][retval] */ ABI::AudioVisualizer::IArrayData **result) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE ConvertToLogFrequency( 
                    /* [in] */ UINT32 elementCount,
                    /* [in] */ float fromFrequency,
                    /* [in] */ float toFrequency,
                    /* [in] */ ABI::AudioVisualizer::InterpolationType type,
                    /* [out][retval] */ ABI::AudioVisualizer::IArrayData **result) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE ApplyRiseAndFall( 
                    /* [in] */ ABI::AudioVisualizer::IArrayData *previous,
                    /* [in] */ ABI::Windows::Foundation::TimeSpan riseTime,
                    /* [in] */ ABI::Windows::Foundation::TimeSpan fallTime,
                    /* [in] */ ABI::Windows::Foundation::TimeSpan timeFromPrevious,
                    /* [out][retval] */ ABI::AudioVisualizer::IArrayData **result) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE ConvertToLogAmplitude( 
                    /* [in] */ float minValue,
                    /* [in] */ float maxValue,
                    /* [out][retval] */ ABI::AudioVisualizer::IArrayData **result) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IArrayData = __uuidof(IArrayData);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIArrayDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AmplitudeScale )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CScaleType *scale);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrequencyScale )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CScaleType *scale);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_MinFrequency )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out][retval] */ float *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaxFrequency )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out][retval] */ float *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrequencyStep )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out][retval] */ float *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrequencyCount )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [out][retval] */ UINT32 *value);
        
        HRESULT ( STDMETHODCALLTYPE *TransformLinearFrequency )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            UINT32 elementCount,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIArrayData **result);
        
        HRESULT ( STDMETHODCALLTYPE *ConvertToLogFrequency )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [in] */ UINT32 elementCount,
            /* [in] */ float fromFrequency,
            /* [in] */ float toFrequency,
            /* [in] */ __x_ABI_CAudioVisualizer_CInterpolationType type,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIArrayData **result);
        
        HRESULT ( STDMETHODCALLTYPE *ApplyRiseAndFall )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [in] */ __x_ABI_CAudioVisualizer_CIArrayData *previous,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan riseTime,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan fallTime,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan timeFromPrevious,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIArrayData **result);
        
        HRESULT ( STDMETHODCALLTYPE *ConvertToLogAmplitude )( 
            __x_ABI_CAudioVisualizer_CIArrayData * This,
            /* [in] */ float minValue,
            /* [in] */ float maxValue,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIArrayData **result);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIArrayDataVtbl;

    interface __x_ABI_CAudioVisualizer_CIArrayData
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIArrayDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIArrayData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIArrayData_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIArrayData_get_AmplitudeScale(This,scale)	\
    ( (This)->lpVtbl -> get_AmplitudeScale(This,scale) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_get_FrequencyScale(This,scale)	\
    ( (This)->lpVtbl -> get_FrequencyScale(This,scale) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_get_MinFrequency(This,value)	\
    ( (This)->lpVtbl -> get_MinFrequency(This,value) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_get_MaxFrequency(This,value)	\
    ( (This)->lpVtbl -> get_MaxFrequency(This,value) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_get_FrequencyStep(This,value)	\
    ( (This)->lpVtbl -> get_FrequencyStep(This,value) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_get_FrequencyCount(This,value)	\
    ( (This)->lpVtbl -> get_FrequencyCount(This,value) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_TransformLinearFrequency(This,elementCount,result)	\
    ( (This)->lpVtbl -> TransformLinearFrequency(This,elementCount,result) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_ConvertToLogFrequency(This,elementCount,fromFrequency,toFrequency,type,result)	\
    ( (This)->lpVtbl -> ConvertToLogFrequency(This,elementCount,fromFrequency,toFrequency,type,result) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_ApplyRiseAndFall(This,previous,riseTime,fallTime,timeFromPrevious,result)	\
    ( (This)->lpVtbl -> ApplyRiseAndFall(This,previous,riseTime,fallTime,timeFromPrevious,result) ) 

#define __x_ABI_CAudioVisualizer_CIArrayData_ConvertToLogAmplitude(This,minValue,maxValue,result)	\
    ( (This)->lpVtbl -> ConvertToLogAmplitude(This,minValue,maxValue,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIArrayData_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0028 */
/* [local] */ 

#if !defined(____x_ABI_CAudioVisualizer_CIScalarData_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IScalarData[] = L"AudioVisualizer.IScalarData";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIScalarData_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0028 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0028_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0028_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIScalarData_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIScalarData_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIScalarData */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IScalarData */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIScalarData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("FA6EDA25-96DC-473E-AA4C-5F67C2307134")
            IScalarData : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE ConvertToLogAmplitude( 
                    /* [in] */ float minValue,
                    /* [in] */ float maxValue,
                    /* [out][retval] */ ABI::AudioVisualizer::IScalarData **result) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE ApplyRiseAndFall( 
                    /* [in] */ ABI::AudioVisualizer::IScalarData *previous,
                    /* [in] */ ABI::Windows::Foundation::TimeSpan riseTime,
                    /* [in] */ ABI::Windows::Foundation::TimeSpan fallTime,
                    /* [in] */ ABI::Windows::Foundation::TimeSpan timeFromPrevious,
                    /* [out][retval] */ ABI::AudioVisualizer::IScalarData **result) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AmplitudeScale( 
                    /* [out][retval] */ ABI::AudioVisualizer::ScaleType *scale) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IScalarData = __uuidof(IScalarData);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIScalarDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *ConvertToLogAmplitude )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This,
            /* [in] */ float minValue,
            /* [in] */ float maxValue,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIScalarData **result);
        
        HRESULT ( STDMETHODCALLTYPE *ApplyRiseAndFall )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This,
            /* [in] */ __x_ABI_CAudioVisualizer_CIScalarData *previous,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan riseTime,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan fallTime,
            /* [in] */ __x_ABI_CWindows_CFoundation_CTimeSpan timeFromPrevious,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIScalarData **result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AmplitudeScale )( 
            __x_ABI_CAudioVisualizer_CIScalarData * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CScaleType *scale);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIScalarDataVtbl;

    interface __x_ABI_CAudioVisualizer_CIScalarData
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIScalarDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIScalarData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIScalarData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIScalarData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIScalarData_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIScalarData_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIScalarData_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIScalarData_ConvertToLogAmplitude(This,minValue,maxValue,result)	\
    ( (This)->lpVtbl -> ConvertToLogAmplitude(This,minValue,maxValue,result) ) 

#define __x_ABI_CAudioVisualizer_CIScalarData_ApplyRiseAndFall(This,previous,riseTime,fallTime,timeFromPrevious,result)	\
    ( (This)->lpVtbl -> ApplyRiseAndFall(This,previous,riseTime,fallTime,timeFromPrevious,result) ) 

#define __x_ABI_CAudioVisualizer_CIScalarData_get_AmplitudeScale(This,scale)	\
    ( (This)->lpVtbl -> get_AmplitudeScale(This,scale) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIScalarData_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0029 */
/* [local] */ 

#ifndef RUNTIMECLASS_AudioVisualizer_ScalarData_DEFINED
#define RUNTIMECLASS_AudioVisualizer_ScalarData_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_ScalarData[] = L"AudioVisualizer.ScalarData";
#endif
#ifndef RUNTIMECLASS_AudioVisualizer_ArrayData_DEFINED
#define RUNTIMECLASS_AudioVisualizer_ArrayData_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_ArrayData[] = L"AudioVisualizer.ArrayData";
#endif
#if !defined(____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizationDataFrame[] = L"AudioVisualizer.IVisualizationDataFrame";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0029 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0029_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0029_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizationDataFrame */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizationDataFrame */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizationDataFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("E4C11BE5-112E-49AD-A565-EE713089C447")
            IVisualizationDataFrame : public IInspectable
            {
            public:
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Time( 
                    /* [out][retval] */ __FIReference_1_Windows__CFoundation__CTimeSpan **time) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Duration( 
                    /* [out][retval] */ __FIReference_1_Windows__CFoundation__CTimeSpan **time) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RMS( 
                    /* [out][retval] */ ABI::AudioVisualizer::IScalarData **data) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Peak( 
                    /* [out][retval] */ ABI::AudioVisualizer::IScalarData **data) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Spectrum( 
                    /* [out][retval] */ ABI::AudioVisualizer::IArrayData **data) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizationDataFrame = __uuidof(IVisualizationDataFrame);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizationDataFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Time )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out][retval] */ __FIReference_1_Windows__CFoundation__CTimeSpan **time);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Duration )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out][retval] */ __FIReference_1_Windows__CFoundation__CTimeSpan **time);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RMS )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIScalarData **data);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Peak )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIScalarData **data);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Spectrum )( 
            __x_ABI_CAudioVisualizer_CIVisualizationDataFrame * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIArrayData **data);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizationDataFrameVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizationDataFrame
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizationDataFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_get_Time(This,time)	\
    ( (This)->lpVtbl -> get_Time(This,time) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_get_Duration(This,time)	\
    ( (This)->lpVtbl -> get_Duration(This,time) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_get_RMS(This,data)	\
    ( (This)->lpVtbl -> get_RMS(This,data) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_get_Peak(This,data)	\
    ( (This)->lpVtbl -> get_Peak(This,data) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationDataFrame_get_Spectrum(This,data)	\
    ( (This)->lpVtbl -> get_Spectrum(This,data) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationDataFrame_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0030 */
/* [local] */ 

#ifndef RUNTIMECLASS_AudioVisualizer_VisualizationDataFrame_DEFINED
#define RUNTIMECLASS_AudioVisualizer_VisualizationDataFrame_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_VisualizationDataFrame[] = L"AudioVisualizer.VisualizationDataFrame";
#endif
#if !defined(____x_ABI_CAudioVisualizer_CIAudioAnalyzer_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IAudioAnalyzer[] = L"AudioVisualizer.IAudioAnalyzer";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIAudioAnalyzer_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0030 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0030_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0030_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIAudioAnalyzer_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIAudioAnalyzer_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIAudioAnalyzer */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IAudioAnalyzer */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIAudioAnalyzer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("3D7769F1-573A-411F-9576-23A0E42273C0")
            IAudioAnalyzer : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE ProcessInput( 
                    ABI::Windows::Media::IAudioFrame *pAudioFrame) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IAudioAnalyzer = __uuidof(IAudioAnalyzer);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIAudioAnalyzerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIAudioAnalyzer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIAudioAnalyzer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIAudioAnalyzer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIAudioAnalyzer * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIAudioAnalyzer * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIAudioAnalyzer * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessInput )( 
            __x_ABI_CAudioVisualizer_CIAudioAnalyzer * This,
            __x_ABI_CWindows_CMedia_CIAudioFrame *pAudioFrame);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIAudioAnalyzerVtbl;

    interface __x_ABI_CAudioVisualizer_CIAudioAnalyzer
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIAudioAnalyzerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIAudioAnalyzer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIAudioAnalyzer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIAudioAnalyzer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIAudioAnalyzer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIAudioAnalyzer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIAudioAnalyzer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIAudioAnalyzer_ProcessInput(This,pAudioFrame)	\
    ( (This)->lpVtbl -> ProcessInput(This,pAudioFrame) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIAudioAnalyzer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0031 */
/* [local] */ 

#if !defined(____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizationSourceStatics[] = L"AudioVisualizer.IVisualizationSourceStatics";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0031 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0031_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0031_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizationSourceStatics */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizationSourceStatics;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("B7589126-33C4-43E8-9D8E-BCF3F3DCE802")
            IVisualizationSourceStatics : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE CreateFromMediaElementAsync( 
                    /* [in] */ ABI::Windows::UI::Xaml::Controls::IMediaElement *pObject,
                    /* [out][retval] */ __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource **ppAsyncOp) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE CreateFromMediaPlayerAsync( 
                    /* [in] */ ABI::Windows::Media::Playback::IMediaPlayer *pObject,
                    /* [out][retval] */ __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource **ppAsyncOp) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE CreateFromAudioNode( 
                    /* [in] */ ABI::Windows::Media::Audio::IAudioNode *pObject,
                    /* [out][retval] */ ABI::AudioVisualizer::IVisualizationSource **ppSource) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizationSourceStatics = __uuidof(IVisualizationSourceStatics);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizationSourceStaticsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFromMediaElementAsync )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This,
            /* [in] */ __x_ABI_CWindows_CUI_CXaml_CControls_CIMediaElement *pObject,
            /* [out][retval] */ __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource **ppAsyncOp);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFromMediaPlayerAsync )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This,
            /* [in] */ __x_ABI_CWindows_CMedia_CPlayback_CIMediaPlayer *pObject,
            /* [out][retval] */ __FIAsyncOperation_1_AudioVisualizer__CIVisualizationSource **ppAsyncOp);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFromAudioNode )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics * This,
            /* [in] */ __x_ABI_CWindows_CMedia_CAudio_CIAudioNode *pObject,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIVisualizationSource **ppSource);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizationSourceStaticsVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizationSourceStaticsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_CreateFromMediaElementAsync(This,pObject,ppAsyncOp)	\
    ( (This)->lpVtbl -> CreateFromMediaElementAsync(This,pObject,ppAsyncOp) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_CreateFromMediaPlayerAsync(This,pObject,ppAsyncOp)	\
    ( (This)->lpVtbl -> CreateFromMediaPlayerAsync(This,pObject,ppAsyncOp) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_CreateFromAudioNode(This,pObject,ppSource)	\
    ( (This)->lpVtbl -> CreateFromAudioNode(This,pObject,ppSource) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationSourceStatics_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0032 */
/* [local] */ 

#if !defined(____x_ABI_CAudioVisualizer_CIVisualizationSource_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizationSource[] = L"AudioVisualizer.IVisualizationSource";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizationSource_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioVisualizer_0000_0032 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0032_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0032_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationSource_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationSource_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizationSource */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizationSource */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizationSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
            MIDL_INTERFACE("B74989FE-1436-4179-92B6-E06B540E0C7E")
            IVisualizationSource : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE Configure( 
                    /* [in] */ ABI::AudioVisualizer::AnalyzerType analyzersToRun,
                    /* [in] */ float outSampleRate,
                    /* [in] */ UINT32 fftLength,
                    /* [in] */ float overlapPercent) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE GetData( 
                    /* [out][retval] */ ABI::AudioVisualizer::IVisualizationDataFrame **result) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsSuspended( 
                    /* [out][retval] */ boolean *bIsSuspended) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsSuspended( 
                    boolean bIsSuspended) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizationSource = __uuidof(IVisualizationSource);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizationSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *Configure )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            /* [in] */ __x_ABI_CAudioVisualizer_CAnalyzerType analyzersToRun,
            /* [in] */ float outSampleRate,
            /* [in] */ UINT32 fftLength,
            /* [in] */ float overlapPercent);
        
        HRESULT ( STDMETHODCALLTYPE *GetData )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            /* [out][retval] */ __x_ABI_CAudioVisualizer_CIVisualizationDataFrame **result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSuspended )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            /* [out][retval] */ boolean *bIsSuspended);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsSuspended )( 
            __x_ABI_CAudioVisualizer_CIVisualizationSource * This,
            boolean bIsSuspended);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizationSourceVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizationSource
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizationSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizationSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationSource_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSource_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSource_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationSource_Configure(This,analyzersToRun,outSampleRate,fftLength,overlapPercent)	\
    ( (This)->lpVtbl -> Configure(This,analyzersToRun,outSampleRate,fftLength,overlapPercent) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSource_GetData(This,result)	\
    ( (This)->lpVtbl -> GetData(This,result) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSource_get_IsSuspended(This,bIsSuspended)	\
    ( (This)->lpVtbl -> get_IsSuspended(This,bIsSuspended) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationSource_put_IsSuspended(This,bIsSuspended)	\
    ( (This)->lpVtbl -> put_IsSuspended(This,bIsSuspended) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioVisualizer_0000_0033 */
/* [local] */ 

#ifndef RUNTIMECLASS_AudioVisualizer_VisualizationSource_DEFINED
#define RUNTIMECLASS_AudioVisualizer_VisualizationSource_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_VisualizationSource[] = L"AudioVisualizer.VisualizationSource";
#endif
#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class MftAnalyzer;
} /*AudioVisualizer*/
}
#endif

#ifndef RUNTIMECLASS_AudioVisualizer_MftAnalyzer_DEFINED
#define RUNTIMECLASS_AudioVisualizer_MftAnalyzer_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_MftAnalyzer[] = L"AudioVisualizer.MftAnalyzer";
#endif


/* interface __MIDL_itf_AudioVisualizer_0000_0033 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0033_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioVisualizer_0000_0033_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


