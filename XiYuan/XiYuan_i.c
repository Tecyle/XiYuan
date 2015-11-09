

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu May 21 22:03:07 2015
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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_XiYuan,0xC3AA3981,0x94C9,0x43A3,0x93,0x63,0x5F,0xFC,0xF4,0xEC,0x1F,0xAC);


MIDL_DEFINE_GUID(IID, DIID_IXiYuan,0x10A5A587,0xD169,0x48EB,0xAE,0x84,0xA7,0x76,0x2B,0x0C,0x3C,0xDA);


MIDL_DEFINE_GUID(CLSID, CLSID_CXiYuanDoc,0x0AAF2B95,0x7B08,0x4734,0xA2,0xA1,0x64,0x99,0xD3,0xD1,0xAF,0x54);


MIDL_DEFINE_GUID(IID, DIID_IVodCtrl,0x5277186D,0x01B5,0x4226,0x88,0x0A,0x16,0x9C,0x24,0x43,0x39,0x06);


MIDL_DEFINE_GUID(CLSID, CLSID_VodCtrl,0x8092C88C,0x973F,0x4E6C,0xAF,0x31,0x62,0x53,0x16,0x8B,0x8A,0xA5);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



