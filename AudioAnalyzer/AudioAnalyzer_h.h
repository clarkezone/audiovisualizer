

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 05:14:07 2038
 */
/* Compiler settings for C:\Users\tonuv\AppData\Local\Temp\AudioAnalyzer.idl-531a6a5d:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
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

#ifndef __AudioAnalyzer_h_h__
#define __AudioAnalyzer_h_h__

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

#ifndef ____FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__
typedef interface __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource;

#endif 	/* ____FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__ */


#ifndef ____FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__
#define ____FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__
typedef interface __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource;

#endif 	/* ____FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_FWD_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_FWD_DEFINED__
typedef interface __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics;

#ifdef __cplusplus
namespace ABI {
    namespace AudioAnalyzer {
        interface IVisualizationDataStatics;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizationData_FWD_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizationData_FWD_DEFINED__
typedef interface __x_ABI_CAudioAnalyzer_CIVisualizationData __x_ABI_CAudioAnalyzer_CIVisualizationData;

#ifdef __cplusplus
namespace ABI {
    namespace AudioAnalyzer {
        interface IVisualizationData;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizationData_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizationSource_FWD_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizationSource_FWD_DEFINED__
typedef interface __x_ABI_CAudioAnalyzer_CIVisualizationSource __x_ABI_CAudioAnalyzer_CIVisualizationSource;

#ifdef __cplusplus
namespace ABI {
    namespace AudioAnalyzer {
        interface IVisualizationSource;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizationSource_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_FWD_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_FWD_DEFINED__
typedef interface __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs;

#ifdef __cplusplus
namespace ABI {
    namespace AudioAnalyzer {
        interface IVisualizerDrawEventArgs;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_FWD_DEFINED__ */


#ifndef ____FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_FWD_DEFINED__
#define ____FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_FWD_DEFINED__
typedef interface __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs;

#endif 	/* ____FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_FWD_DEFINED__ */


#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizer_FWD_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizer_FWD_DEFINED__
typedef interface __x_ABI_CAudioAnalyzer_CIVisualizer __x_ABI_CAudioAnalyzer_CIVisualizer;

#ifdef __cplusplus
namespace ABI {
    namespace AudioAnalyzer {
        interface IVisualizer;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizer_FWD_DEFINED__ */


/* header files for imported files */
#include "inspectable.h"
#include "EventToken.h"
#include "AsyncInfo.h"
#include "Windows.Foundation.h"
#include "Windows.Media.h"
#include "Windows.UI.Xaml.Controls.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_AudioAnalyzer_0000_0000 */
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
namespace AudioAnalyzer {
interface IVisualizationSource;
} /*AudioAnalyzer*/
}
#endif


/* interface __MIDL_itf_AudioAnalyzer_0000_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0000_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5374 */




/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5374 */




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5374_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5374_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer_0000_0001 */
/* [local] */ 

#ifndef DEF___FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_USE
#define DEF___FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("75edd8f3-d828-5f63-8901-976bd84d88cd"))
IAsyncOperationCompletedHandler<ABI::AudioAnalyzer::IVisualizationSource*> : IAsyncOperationCompletedHandler_impl<ABI::AudioAnalyzer::IVisualizationSource*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.AsyncOperationCompletedHandler`1<AudioAnalyzer.IVisualizationSource>"; }
};
typedef IAsyncOperationCompletedHandler<ABI::AudioAnalyzer::IVisualizationSource*> __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_t;
#define ____FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__
#define __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource ABI::Windows::Foundation::__FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_USE */


/* interface __MIDL_itf_AudioAnalyzer_0000_0001 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0001_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5375 */




/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5375 */




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5375_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5375_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer_0000_0002 */
/* [local] */ 

#ifndef DEF___FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_USE
#define DEF___FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("946be94e-bee2-54d4-8b92-6746772a07d0"))
IAsyncOperation<ABI::AudioAnalyzer::IVisualizationSource*> : IAsyncOperation_impl<ABI::AudioAnalyzer::IVisualizationSource*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.IAsyncOperation`1<AudioAnalyzer.IVisualizationSource>"; }
};
typedef IAsyncOperation<ABI::AudioAnalyzer::IVisualizationSource*> __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_t;
#define ____FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_FWD_DEFINED__
#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource ABI::Windows::Foundation::__FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_USE */

#ifdef __cplusplus
namespace ABI {
namespace AudioAnalyzer {
interface IVisualizer;
} /*AudioAnalyzer*/
}
#endif
#ifdef __cplusplus
namespace ABI {
namespace AudioAnalyzer {
class VisualizerDrawEventArgs;
} /*AudioAnalyzer*/
}
#endif

#ifdef __cplusplus
namespace ABI {
namespace AudioAnalyzer {
interface IVisualizerDrawEventArgs;
} /*AudioAnalyzer*/
}
#endif


/* interface __MIDL_itf_AudioAnalyzer_0000_0002 */
/* [local] */ 






extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0002_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5376 */




/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5376 */




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5376_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5376_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer_0000_0003 */
/* [local] */ 

#ifndef DEF___FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_USE
#define DEF___FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("c439d7cf-ec2f-501f-8237-1c7592f67ab5"))
ITypedEventHandler<ABI::AudioAnalyzer::IVisualizer*,ABI::AudioAnalyzer::VisualizerDrawEventArgs*> : ITypedEventHandler_impl<ABI::AudioAnalyzer::IVisualizer*,ABI::Windows::Foundation::Internal::AggregateType<ABI::AudioAnalyzer::VisualizerDrawEventArgs*, ABI::AudioAnalyzer::IVisualizerDrawEventArgs*>> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.TypedEventHandler`2<AudioAnalyzer.IVisualizer, AudioAnalyzer.VisualizerDrawEventArgs>"; }
};
typedef ITypedEventHandler<ABI::AudioAnalyzer::IVisualizer*,ABI::AudioAnalyzer::VisualizerDrawEventArgs*> __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_t;
#define ____FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_FWD_DEFINED__
#define __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs ABI::Windows::Foundation::__FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_USE */
#pragma warning(push)
#pragma warning(disable:4668) 
#pragma warning(disable:4001) 
#pragma once
#pragma warning(pop)

#ifdef __cplusplus
namespace ABI {
namespace AudioAnalyzer {
class VisualizationData;
} /*AudioAnalyzer*/
}
#endif


/* interface __MIDL_itf_AudioAnalyzer_0000_0003 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0003_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5377 */




/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5377 */




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5377_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5377_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer_0000_0004 */
/* [local] */ 

#ifndef DEF___FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource
#define DEF___FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)



/* interface __MIDL_itf_AudioAnalyzer_0000_0004 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0004_v0_0_s_ifspec;

#ifndef ____FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_INTERFACE_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_INTERFACE_DEFINED__

/* interface __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource */
/* [unique][uuid][object] */ 



/* interface __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("75edd8f3-d828-5f63-8901-976bd84d88cd")
    __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource *asyncInfo,
            /* [in] */ AsyncStatus status) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [in] */ __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource *asyncInfo,
            /* [in] */ AsyncStatus status);
        
        END_INTERFACE
    } __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSourceVtbl;

    interface __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource
    {
        CONST_VTBL struct __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_Invoke(This,asyncInfo,status)	\
    ( (This)->lpVtbl -> Invoke(This,asyncInfo,status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0005 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource */


/* interface __MIDL_itf_AudioAnalyzer_0000_0005 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0005_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5378 */




/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5378 */




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5378_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5378_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer_0000_0006 */
/* [local] */ 

#ifndef DEF___FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource
#define DEF___FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_AudioAnalyzer_0000_0006 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0006_v0_0_s_ifspec;

#ifndef ____FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_INTERFACE_DEFINED__
#define ____FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_INTERFACE_DEFINED__

/* interface __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource */
/* [unique][uuid][object] */ 



/* interface __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("946be94e-bee2-54d4-8b92-6746772a07d0")
    __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource : public IInspectable
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Completed( 
            /* [in] */ __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource *handler) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Completed( 
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource **handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResults( 
            /* [retval][out] */ ABI::AudioAnalyzer::IVisualizationSource **results) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Completed )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [in] */ __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource *handler);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Completed )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_AudioAnalyzer__CIVisualizationSource **handler);
        
        HRESULT ( STDMETHODCALLTYPE *GetResults )( 
            __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource * This,
            /* [retval][out] */ __x_ABI_CAudioAnalyzer_CIVisualizationSource **results);
        
        END_INTERFACE
    } __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSourceVtbl;

    interface __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource
    {
        CONST_VTBL struct __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_put_Completed(This,handler)	\
    ( (This)->lpVtbl -> put_Completed(This,handler) ) 

#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_get_Completed(This,handler)	\
    ( (This)->lpVtbl -> get_Completed(This,handler) ) 

#define __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_GetResults(This,results)	\
    ( (This)->lpVtbl -> GetResults(This,results) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0007 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource */
#if !defined(____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioAnalyzer_IVisualizationDataStatics[] = L"AudioAnalyzer.IVisualizationDataStatics";
#endif /* !defined(____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioAnalyzer_0000_0007 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0007_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0007_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_INTERFACE_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics */
/* [uuid][object] */ 



/* interface ABI::AudioAnalyzer::IVisualizationDataStatics */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioAnalyzer_CIVisualizationDataStatics;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioAnalyzer {
            
            MIDL_INTERFACE("B7589126-33C4-43E8-9D8E-BCF3F3DCE802")
            IVisualizationDataStatics : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE CreateVisualizationSourceAsync( 
                    /* [in] */ IInspectable *pElement,
                    /* [out][retval] */ __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource **value) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizationDataStatics = __uuidof(IVisualizationDataStatics);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioAnalyzer_CIVisualizationDataStaticsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *CreateVisualizationSourceAsync )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics * This,
            /* [in] */ IInspectable *pElement,
            /* [out][retval] */ __FIAsyncOperation_1_AudioAnalyzer__CIVisualizationSource **value);
        
        END_INTERFACE
    } __x_ABI_CAudioAnalyzer_CIVisualizationDataStaticsVtbl;

    interface __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics
    {
        CONST_VTBL struct __x_ABI_CAudioAnalyzer_CIVisualizationDataStaticsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_CreateVisualizationSourceAsync(This,pElement,value)	\
    ( (This)->lpVtbl -> CreateVisualizationSourceAsync(This,pElement,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizationDataStatics_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0008 */
/* [local] */ 

#if !defined(____x_ABI_CAudioAnalyzer_CIVisualizationData_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioAnalyzer_IVisualizationData[] = L"AudioAnalyzer.IVisualizationData";
#endif /* !defined(____x_ABI_CAudioAnalyzer_CIVisualizationData_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioAnalyzer_0000_0008 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0008_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0008_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizationData_INTERFACE_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizationData_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioAnalyzer_CIVisualizationData */
/* [uuid][object] */ 



/* interface ABI::AudioAnalyzer::IVisualizationData */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioAnalyzer_CIVisualizationData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioAnalyzer {
            
            MIDL_INTERFACE("E4C11BE5-112E-49AD-A565-EE713089C447")
            IVisualizationData : public IInspectable
            {
            public:
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AudioChannels( 
                    /* [out][retval] */ unsigned int *result) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizationData = __uuidof(IVisualizationData);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioAnalyzer_CIVisualizationDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationData * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationData * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationData * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioChannels )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationData * This,
            /* [out][retval] */ unsigned int *result);
        
        END_INTERFACE
    } __x_ABI_CAudioAnalyzer_CIVisualizationDataVtbl;

    interface __x_ABI_CAudioAnalyzer_CIVisualizationData
    {
        CONST_VTBL struct __x_ABI_CAudioAnalyzer_CIVisualizationDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioAnalyzer_CIVisualizationData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizationData_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationData_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationData_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizationData_get_AudioChannels(This,result)	\
    ( (This)->lpVtbl -> get_AudioChannels(This,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizationData_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0009 */
/* [local] */ 

#ifndef RUNTIMECLASS_AudioAnalyzer_VisualizationData_DEFINED
#define RUNTIMECLASS_AudioAnalyzer_VisualizationData_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioAnalyzer_VisualizationData[] = L"AudioAnalyzer.VisualizationData";
#endif
#if !defined(____x_ABI_CAudioAnalyzer_CIVisualizationSource_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioAnalyzer_IVisualizationSource[] = L"AudioAnalyzer.IVisualizationSource";
#endif /* !defined(____x_ABI_CAudioAnalyzer_CIVisualizationSource_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioAnalyzer_0000_0009 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0009_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0009_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizationSource_INTERFACE_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizationSource_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioAnalyzer_CIVisualizationSource */
/* [uuid][object] */ 



/* interface ABI::AudioAnalyzer::IVisualizationSource */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioAnalyzer_CIVisualizationSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioAnalyzer {
            
            MIDL_INTERFACE("B74989FE-1436-4179-92B6-E06B540E0C7E")
            IVisualizationSource : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE Configure( 
                    /* [in] */ unsigned long fftLength,
                    /* [in] */ float outSampleRate,
                    /* [in] */ float overlapPercent) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE GetData( 
                    /* [out][retval] */ ABI::AudioAnalyzer::IVisualizationData **result) = 0;
                
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

    typedef struct __x_ABI_CAudioAnalyzer_CIVisualizationSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *Configure )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            /* [in] */ unsigned long fftLength,
            /* [in] */ float outSampleRate,
            /* [in] */ float overlapPercent);
        
        HRESULT ( STDMETHODCALLTYPE *GetData )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            /* [out][retval] */ __x_ABI_CAudioAnalyzer_CIVisualizationData **result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSuspended )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            /* [out][retval] */ boolean *bIsSuspended);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsSuspended )( 
            __x_ABI_CAudioAnalyzer_CIVisualizationSource * This,
            boolean bIsSuspended);
        
        END_INTERFACE
    } __x_ABI_CAudioAnalyzer_CIVisualizationSourceVtbl;

    interface __x_ABI_CAudioAnalyzer_CIVisualizationSource
    {
        CONST_VTBL struct __x_ABI_CAudioAnalyzer_CIVisualizationSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_Configure(This,fftLength,outSampleRate,overlapPercent)	\
    ( (This)->lpVtbl -> Configure(This,fftLength,outSampleRate,overlapPercent) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_GetData(This,result)	\
    ( (This)->lpVtbl -> GetData(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_get_IsSuspended(This,bIsSuspended)	\
    ( (This)->lpVtbl -> get_IsSuspended(This,bIsSuspended) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizationSource_put_IsSuspended(This,bIsSuspended)	\
    ( (This)->lpVtbl -> put_IsSuspended(This,bIsSuspended) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizationSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0010 */
/* [local] */ 

#ifdef __cplusplus
namespace ABI {
namespace AudioAnalyzer {
class AnalyzerEffect;
} /*AudioAnalyzer*/
}
#endif

#ifndef RUNTIMECLASS_AudioAnalyzer_AnalyzerEffect_DEFINED
#define RUNTIMECLASS_AudioAnalyzer_AnalyzerEffect_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioAnalyzer_AnalyzerEffect[] = L"AudioAnalyzer.AnalyzerEffect";
#endif
#if !defined(____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioAnalyzer_IVisualizerDrawEventArgs[] = L"AudioAnalyzer.IVisualizerDrawEventArgs";
#endif /* !defined(____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioAnalyzer_0000_0010 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0010_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0010_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs */
/* [uuid][object] */ 



/* interface ABI::AudioAnalyzer::IVisualizerDrawEventArgs */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioAnalyzer {
            
            MIDL_INTERFACE("62C7FB4D-4712-4A49-BF0F-597AC7079F3A")
            IVisualizerDrawEventArgs : public IInspectable
            {
            public:
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
                    /* [out][retval] */ ABI::AudioAnalyzer::IVisualizationData **pData) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizerDrawEventArgs = __uuidof(IVisualizerDrawEventArgs);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs * This,
            /* [out][retval] */ __x_ABI_CAudioAnalyzer_CIVisualizationData **pData);
        
        END_INTERFACE
    } __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgsVtbl;

    interface __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs
    {
        CONST_VTBL struct __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_get_Data(This,pData)	\
    ( (This)->lpVtbl -> get_Data(This,pData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0011 */
/* [local] */ 

#ifndef RUNTIMECLASS_AudioAnalyzer_VisualizerDrawEventArgs_DEFINED
#define RUNTIMECLASS_AudioAnalyzer_VisualizerDrawEventArgs_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioAnalyzer_VisualizerDrawEventArgs[] = L"AudioAnalyzer.VisualizerDrawEventArgs";
#endif


/* interface __MIDL_itf_AudioAnalyzer_0000_0011 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0011_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0011_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5379 */




/* interface __MIDL_itf_AudioAnalyzer2Eidl_0000_5379 */




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5379_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer2Eidl_0000_5379_v0_0_s_ifspec;

/* interface __MIDL_itf_AudioAnalyzer_0000_0012 */
/* [local] */ 

#ifndef DEF___FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs
#define DEF___FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_AudioAnalyzer_0000_0012 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0012_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0012_v0_0_s_ifspec;

#ifndef ____FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_INTERFACE_DEFINED__
#define ____FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_INTERFACE_DEFINED__

/* interface __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs */
/* [unique][uuid][object] */ 



/* interface __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c439d7cf-ec2f-501f-8237-1c7592f67ab5")
    __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ABI::AudioAnalyzer::IVisualizer *sender,
            /* [in] */ ABI::AudioAnalyzer::IVisualizerDrawEventArgs *e) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs * This,
            /* [in] */ __x_ABI_CAudioAnalyzer_CIVisualizer *sender,
            /* [in] */ __x_ABI_CAudioAnalyzer_CIVisualizerDrawEventArgs *e);
        
        END_INTERFACE
    } __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgsVtbl;

    interface __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs
    {
        CONST_VTBL struct __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_Invoke(This,sender,e)	\
    ( (This)->lpVtbl -> Invoke(This,sender,e) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0013 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs */
#if !defined(____x_ABI_CAudioAnalyzer_CIVisualizer_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioAnalyzer_IVisualizer[] = L"AudioAnalyzer.IVisualizer";
#endif /* !defined(____x_ABI_CAudioAnalyzer_CIVisualizer_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioAnalyzer_0000_0013 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0013_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0013_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioAnalyzer_CIVisualizer_INTERFACE_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIVisualizer_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioAnalyzer_CIVisualizer */
/* [uuid][object] */ 



/* interface ABI::AudioAnalyzer::IVisualizer */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioAnalyzer_CIVisualizer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioAnalyzer {
            
            MIDL_INTERFACE("D3C979CE-1D6D-4507-B7CE-5A2E97F672FC")
            IVisualizer : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE add_Draw( 
                    /* [in] */ __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs *value,
                    /* [out][retval] */ EventRegistrationToken *token) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE remove_Draw( 
                    /* [in] */ EventRegistrationToken token) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Source( 
                    /* [out][retval] */ ABI::AudioAnalyzer::IVisualizationSource **pData) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Source( 
                    /* [in] */ ABI::AudioAnalyzer::IVisualizationSource *pData) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IVisualizer = __uuidof(IVisualizer);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioAnalyzer_CIVisualizerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *add_Draw )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [in] */ __FITypedEventHandler_2_AudioAnalyzer__CIVisualizer_AudioAnalyzer__CVisualizerDrawEventArgs *value,
            /* [out][retval] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_Draw )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [in] */ EventRegistrationToken token);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Source )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [out][retval] */ __x_ABI_CAudioAnalyzer_CIVisualizationSource **pData);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Source )( 
            __x_ABI_CAudioAnalyzer_CIVisualizer * This,
            /* [in] */ __x_ABI_CAudioAnalyzer_CIVisualizationSource *pData);
        
        END_INTERFACE
    } __x_ABI_CAudioAnalyzer_CIVisualizerVtbl;

    interface __x_ABI_CAudioAnalyzer_CIVisualizer
    {
        CONST_VTBL struct __x_ABI_CAudioAnalyzer_CIVisualizerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioAnalyzer_CIVisualizer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioAnalyzer_CIVisualizer_add_Draw(This,value,token)	\
    ( (This)->lpVtbl -> add_Draw(This,value,token) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizer_remove_Draw(This,token)	\
    ( (This)->lpVtbl -> remove_Draw(This,token) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizer_get_Source(This,pData)	\
    ( (This)->lpVtbl -> get_Source(This,pData) ) 

#define __x_ABI_CAudioAnalyzer_CIVisualizer_put_Source(This,pData)	\
    ( (This)->lpVtbl -> put_Source(This,pData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioAnalyzer_CIVisualizer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0014 */
/* [local] */ 

#ifdef __cplusplus
namespace ABI {
namespace AudioAnalyzer {
class BaseVisualizer;
} /*AudioAnalyzer*/
}
#endif
#ifndef RUNTIMECLASS_AudioAnalyzer_BaseVisualizer_DEFINED
#define RUNTIMECLASS_AudioAnalyzer_BaseVisualizer_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioAnalyzer_BaseVisualizer[] = L"AudioAnalyzer.BaseVisualizer";
#endif


/* interface __MIDL_itf_AudioAnalyzer_0000_0014 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0014_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0014_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


