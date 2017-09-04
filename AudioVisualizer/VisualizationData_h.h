

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 05:14:07 2038
 */
/* Compiler settings for C:\Users\tonuv\AppData\Local\Temp\VisualizationData.idl-d0c997d5:
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

#ifndef __VisualizationData_h_h__
#define __VisualizationData_h_h__

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

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationData_FWD_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationData_FWD_DEFINED__
typedef interface __x_ABI_CAudioVisualizer_CIVisualizationData __x_ABI_CAudioVisualizer_CIVisualizationData;

#ifdef __cplusplus
namespace ABI {
    namespace AudioVisualizer {
        interface IVisualizationData;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationData_FWD_DEFINED__ */


/* header files for imported files */
#include "Windows.Foundation.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_VisualizationData_0000_0000 */
/* [local] */ 

#if !defined(____x_ABI_CAudioVisualizer_CIVisualizationData_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioVisualizer_IVisualizationData[] = L"AudioVisualizer.IVisualizationData";
#endif /* !defined(____x_ABI_CAudioVisualizer_CIVisualizationData_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_VisualizationData_0000_0000 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_VisualizationData_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_VisualizationData_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioVisualizer_CIVisualizationData_INTERFACE_DEFINED__
#define ____x_ABI_CAudioVisualizer_CIVisualizationData_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioVisualizer_CIVisualizationData */
/* [uuid][object] */ 



/* interface ABI::AudioVisualizer::IVisualizationData */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioVisualizer_CIVisualizationData;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioVisualizer {
            
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

    typedef struct __x_ABI_CAudioVisualizer_CIVisualizationDataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioVisualizer_CIVisualizationData * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioVisualizer_CIVisualizationData * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioVisualizer_CIVisualizationData * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioVisualizer_CIVisualizationData * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioVisualizer_CIVisualizationData * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioVisualizer_CIVisualizationData * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AudioChannels )( 
            __x_ABI_CAudioVisualizer_CIVisualizationData * This,
            /* [out][retval] */ unsigned int *result);
        
        END_INTERFACE
    } __x_ABI_CAudioVisualizer_CIVisualizationDataVtbl;

    interface __x_ABI_CAudioVisualizer_CIVisualizationData
    {
        CONST_VTBL struct __x_ABI_CAudioVisualizer_CIVisualizationDataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioVisualizer_CIVisualizationData_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationData_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationData_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationData_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationData_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioVisualizer_CIVisualizationData_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioVisualizer_CIVisualizationData_get_AudioChannels(This,result)	\
    ( (This)->lpVtbl -> get_AudioChannels(This,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioVisualizer_CIVisualizationData_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_VisualizationData_0000_0001 */
/* [local] */ 

#ifdef __cplusplus
namespace ABI {
namespace AudioVisualizer {
class VisualizationData;
} /*AudioVisualizer*/
}
#endif

#ifndef RUNTIMECLASS_AudioVisualizer_VisualizationData_DEFINED
#define RUNTIMECLASS_AudioVisualizer_VisualizationData_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_AudioVisualizer_VisualizationData[] = L"AudioVisualizer.VisualizationData";
#endif


/* interface __MIDL_itf_VisualizationData_0000_0001 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_VisualizationData_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_VisualizationData_0000_0001_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


