

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sun May 10 23:23:32 2015
 */
/* Compiler settings for XiYuan.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
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
#endif // __RPCNDR_H_VERSION__


#ifndef __XiYuan_h_h__
#define __XiYuan_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IXiYuan_FWD_DEFINED__
#define __IXiYuan_FWD_DEFINED__
typedef interface IXiYuan IXiYuan;

#endif 	/* __IXiYuan_FWD_DEFINED__ */


#ifndef __CXiYuanDoc_FWD_DEFINED__
#define __CXiYuanDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CXiYuanDoc CXiYuanDoc;
#else
typedef struct CXiYuanDoc CXiYuanDoc;
#endif /* __cplusplus */

#endif 	/* __CXiYuanDoc_FWD_DEFINED__ */


#ifndef __IVodCtrl_FWD_DEFINED__
#define __IVodCtrl_FWD_DEFINED__
typedef interface IVodCtrl IVodCtrl;

#endif 	/* __IVodCtrl_FWD_DEFINED__ */


#ifndef __VodCtrl_FWD_DEFINED__
#define __VodCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class VodCtrl VodCtrl;
#else
typedef struct VodCtrl VodCtrl;
#endif /* __cplusplus */

#endif 	/* __VodCtrl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __XiYuan_LIBRARY_DEFINED__
#define __XiYuan_LIBRARY_DEFINED__

/* library XiYuan */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_XiYuan;

#ifndef __IXiYuan_DISPINTERFACE_DEFINED__
#define __IXiYuan_DISPINTERFACE_DEFINED__

/* dispinterface IXiYuan */
/* [uuid] */ 


EXTERN_C const IID DIID_IXiYuan;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("10A5A587-D169-48EB-AE84-A7762B0C3CDA")
    IXiYuan : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IXiYuanVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IXiYuan * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IXiYuan * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IXiYuan * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IXiYuan * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IXiYuan * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IXiYuan * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IXiYuan * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IXiYuanVtbl;

    interface IXiYuan
    {
        CONST_VTBL struct IXiYuanVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXiYuan_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IXiYuan_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IXiYuan_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IXiYuan_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IXiYuan_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IXiYuan_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IXiYuan_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IXiYuan_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CXiYuanDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("0AAF2B95-7B08-4734-A2A1-6499D3D1AF54")
CXiYuanDoc;
#endif

#ifndef __IVodCtrl_DISPINTERFACE_DEFINED__
#define __IVodCtrl_DISPINTERFACE_DEFINED__

/* dispinterface IVodCtrl */
/* [uuid] */ 


EXTERN_C const IID DIID_IVodCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5277186D-01B5-4226-880A-169C24433906")
    IVodCtrl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IVodCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVodCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVodCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVodCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVodCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVodCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVodCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVodCtrl * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IVodCtrlVtbl;

    interface IVodCtrl
    {
        CONST_VTBL struct IVodCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVodCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVodCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVodCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVodCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVodCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVodCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVodCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IVodCtrl_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_VodCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("8092C88C-973F-4E6C-AF31-6253168B8AA5")
VodCtrl;
#endif
#endif /* __XiYuan_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


