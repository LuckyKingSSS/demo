

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:35 2017
 */
/* Compiler settings for cGameStreamCtrl.idl:
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

#ifndef __cGameStreamCtrl_h__
#define __cGameStreamCtrl_h__

// {298415DB-06D9-4C72-9E2B-6E4A1166000F}
DEFINE_GUID(IID_IGameStreamCtrl, 
0x298415db, 0x6d9, 0x4c72, 0x9e, 0x2b, 0x6e, 0x4a, 0x11, 0x66, 0x0, 0xf);

#ifdef __cplusplus
extern "C"{
#endif 


// 游戏流的控制接口

#ifndef __IGameStreamCtrl_INTERFACE_DEFINED__
#define __IGameStreamCtrl_INTERFACE_DEFINED__


class IGameStreamCtrl : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetTargetInfo( 
		/* [out][in] */ wchar_t *info,
		/* [out][in] */ int32_t *size) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetTargetInfo( 
		/* [in] */ wchar_t *info,
		/* [in] */ int32_t size) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetFramerate(
		/* [in] */ int32_t framerate) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetFramerate(
		/* [out] */ int32_t *framerate) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetSize(
        /* [out]*/ int32_t *w,
        /* [out]*/ int32_t *h) = 0;

	// type == 0用于预览， 1用于录制
	virtual HRESULT STDMETHODCALLTYPE SetStreamType(
		/* [in] */ int32_t type) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetStreamType(
		/* [in] */ int32_t *type) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetRecordWaitTime(
        /* [in] */ int32_t milliseconds) = 0;
        
};


#endif 	/* __IGameStreamCtrl_INTERFACE_DEFINED__ */


#ifdef __cplusplus
}
#endif

#endif


