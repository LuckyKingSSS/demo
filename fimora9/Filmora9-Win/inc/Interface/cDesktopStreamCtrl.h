

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:34 2017
 */
/* Compiler settings for cDesktopStreamCtrl.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */

#include "stdint.h"
#include <Guiddef.h>
#include <objbase.h>

#ifndef __cDesktopStreamCtrl_h__
#define __cDesktopStreamCtrl_h__

// {BA908D3B-2BB2-4461-A568-0CA9CBF6E97B}
DEFINE_GUID(IID_IDesktopStreamCtrl, 
0xba908d3b, 0x2bb2, 0x4461, 0xa5, 0x68, 0xc, 0xa9, 0xcb, 0xf6, 0xe9, 0x7b);

#ifdef __cplusplus
extern "C"{
#endif 

// 桌面流的控制接口

#ifndef __IDesktopStreamCtrl_INTERFACE_DEFINED__
#define __IDesktopStreamCtrl_INTERFACE_DEFINED__

class IDesktopStreamCtrl : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetCrop( 
		/* [out][in] */ int32_t *x,
		/* [out][in] */ int32_t *y,
		/* [out][in] */ int32_t *w,
		/* [out][in] */ int32_t *h) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetCrop( 
		/* [in] */ int32_t x,
		/* [in] */ int32_t y,
		/* [in] */ int32_t w,
		/* [in] */ int32_t h) = 0;
	
};
    
#endif 	/* __IDesktopStreamCtrl_INTERFACE_DEFINED__ */


#ifdef __cplusplus
}
#endif

#endif


