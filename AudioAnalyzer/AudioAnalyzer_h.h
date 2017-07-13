

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\tonuv\AppData\Local\Temp\AudioAnalyzer.idl-bb4fb473:
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

#ifndef ____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_FWD_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_FWD_DEFINED__
typedef interface __x_ABI_CAudioAnalyzer_CIAudioAnalyzer __x_ABI_CAudioAnalyzer_CIAudioAnalyzer;

#ifdef __cplusplus
namespace ABI {
    namespace AudioAnalyzer {
        interface IAudioAnalyzer;
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_FWD_DEFINED__ */


/* header files for imported files */
#include "Windows.Media.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_AudioAnalyzer_0000_0000 */
/* [local] */ 

#pragma warning(push)
#pragma warning(disable:4668) 
#pragma warning(disable:4001) 
#pragma once
#pragma warning(pop)
#if !defined(____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_AudioAnalyzer_IAudioAnalyzer[] = L"AudioAnalyzer.IAudioAnalyzer";
#endif /* !defined(____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_AudioAnalyzer_0000_0000 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_INTERFACE_DEFINED__
#define ____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_INTERFACE_DEFINED__

/* interface __x_ABI_CAudioAnalyzer_CIAudioAnalyzer */
/* [uuid][object] */ 



/* interface ABI::AudioAnalyzer::IAudioAnalyzer */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CAudioAnalyzer_CIAudioAnalyzer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace AudioAnalyzer {
            
            MIDL_INTERFACE("B74989FE-1436-4179-92B6-E06B540E0C7E")
            IAudioAnalyzer : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE Configure( 
                    /* [in] */ unsigned long fftLength,
                    /* [in] */ float outSampleRate,
                    /* [in] */ float overlapPercent) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE GetFrame( 
                    /* [out][retval] */ ABI::Windows::Media::IAudioFrame **result) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE SetLinearFScale( 
                    /* [in] */ unsigned long outputElements) = 0;
                
                virtual HRESULT STDMETHODCALLTYPE SetLogFScale( 
                    /* [in] */ float lowFrequency,
                    /* [in] */ float highFrequency,
                    /* [in] */ unsigned long numberOfBins) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FrequencyStep( 
                    /* [out][retval] */ float *result) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsLogFrequencyScale( 
                    /* [out][retval] */ boolean *result) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LowFrequency( 
                    /* [out][retval] */ float *result) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HighFrequency( 
                    /* [out][retval] */ float *result) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_OutputElementsCount( 
                    /* [out][retval] */ unsigned long *result) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsLogAmplitudeScale( 
                    /* [out][retval] */ boolean *result) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsLogAmplitudeScale( 
                    boolean value) = 0;
                
                virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsSuspended( 
                    /* [out][retval] */ boolean *bIsSuspended) = 0;
                
                virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsSuspended( 
                    boolean bIsSuspended) = 0;
                
            };

            extern const __declspec(selectany) IID & IID_IAudioAnalyzer = __uuidof(IAudioAnalyzer);

            
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_CAudioAnalyzer_CIAudioAnalyzerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *Configure )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [in] */ unsigned long fftLength,
            /* [in] */ float outSampleRate,
            /* [in] */ float overlapPercent);
        
        HRESULT ( STDMETHODCALLTYPE *GetFrame )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ __x_ABI_CWindows_CMedia_CIAudioFrame **result);
        
        HRESULT ( STDMETHODCALLTYPE *SetLinearFScale )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [in] */ unsigned long outputElements);
        
        HRESULT ( STDMETHODCALLTYPE *SetLogFScale )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [in] */ float lowFrequency,
            /* [in] */ float highFrequency,
            /* [in] */ unsigned long numberOfBins);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrequencyStep )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ float *result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsLogFrequencyScale )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ boolean *result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LowFrequency )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ float *result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HighFrequency )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ float *result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_OutputElementsCount )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ unsigned long *result);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsLogAmplitudeScale )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ boolean *result);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsLogAmplitudeScale )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            boolean value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSuspended )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            /* [out][retval] */ boolean *bIsSuspended);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsSuspended )( 
            __x_ABI_CAudioAnalyzer_CIAudioAnalyzer * This,
            boolean bIsSuspended);
        
        END_INTERFACE
    } __x_ABI_CAudioAnalyzer_CIAudioAnalyzerVtbl;

    interface __x_ABI_CAudioAnalyzer_CIAudioAnalyzer
    {
        CONST_VTBL struct __x_ABI_CAudioAnalyzer_CIAudioAnalyzerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_Configure(This,fftLength,outSampleRate,overlapPercent)	\
    ( (This)->lpVtbl -> Configure(This,fftLength,outSampleRate,overlapPercent) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_GetFrame(This,result)	\
    ( (This)->lpVtbl -> GetFrame(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_SetLinearFScale(This,outputElements)	\
    ( (This)->lpVtbl -> SetLinearFScale(This,outputElements) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_SetLogFScale(This,lowFrequency,highFrequency,numberOfBins)	\
    ( (This)->lpVtbl -> SetLogFScale(This,lowFrequency,highFrequency,numberOfBins) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_get_FrequencyStep(This,result)	\
    ( (This)->lpVtbl -> get_FrequencyStep(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_get_IsLogFrequencyScale(This,result)	\
    ( (This)->lpVtbl -> get_IsLogFrequencyScale(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_get_LowFrequency(This,result)	\
    ( (This)->lpVtbl -> get_LowFrequency(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_get_HighFrequency(This,result)	\
    ( (This)->lpVtbl -> get_HighFrequency(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_get_OutputElementsCount(This,result)	\
    ( (This)->lpVtbl -> get_OutputElementsCount(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_get_IsLogAmplitudeScale(This,result)	\
    ( (This)->lpVtbl -> get_IsLogAmplitudeScale(This,result) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_put_IsLogAmplitudeScale(This,value)	\
    ( (This)->lpVtbl -> put_IsLogAmplitudeScale(This,value) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_get_IsSuspended(This,bIsSuspended)	\
    ( (This)->lpVtbl -> get_IsSuspended(This,bIsSuspended) ) 

#define __x_ABI_CAudioAnalyzer_CIAudioAnalyzer_put_IsSuspended(This,bIsSuspended)	\
    ( (This)->lpVtbl -> put_IsSuspended(This,bIsSuspended) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CAudioAnalyzer_CIAudioAnalyzer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_AudioAnalyzer_0000_0001 */
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


/* interface __MIDL_itf_AudioAnalyzer_0000_0001 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AudioAnalyzer_0000_0001_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


