

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:32 2017
 */
/* Compiler settings for cCaptureMgr.idl:
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

#ifndef __cCaptureMgr_h__
#define __cCaptureMgr_h__


/* header files for imported files */
#include "cCaptureStream.h"

// {E7C2FB23-A71C-4AE0-8186-6B469731E52E}
DEFINE_GUID(CLSID_NLECaptureMgr, 0xe7c2fb23, 0xa71c, 0x4ae0, 0x81, 0x86, 0x6b, 0x46, 0x97, 0x31, 0xe5, 0x2e);
// {89D138BB-0AF5-44AE-A29E-8A6DE4A73B8C}
DEFINE_GUID(IID_ICaptureMgr, 0x89d138bb, 0xaf5, 0x44ae, 0xa2, 0x9e, 0x8a, 0x6d, 0xe4, 0xa7, 0x3b, 0x8c);

#ifdef __cplusplus
extern "C"{
#endif 


typedef 
enum Capture_Type
    {
        CaptureType_Mic	= 0,
        CaptureType_Camera	= ( CaptureType_Mic + 1 ) ,
        CaptureType_Desktop	= ( CaptureType_Camera + 1 ) ,
        CaptureType_Game	= ( CaptureType_Desktop + 1 ) ,
        CaptureType_Keyb	= ( CaptureType_Game + 1 ) ,
        CaptureType_Mark	= ( CaptureType_Keyb + 1 ) ,
        CaptureType_Mouse	= ( CaptureType_Mark + 1 ) 
    } 	Capture_Type;

typedef struct Capture_Info
    {
    Capture_Type type;
    int32_t idx;
    wchar_t displayName[ 256 ];
    wchar_t description[ 256 ];
    } 	Capture_Info;

typedef 
enum Capture_MSG
    {
        CaptureMSG_DeviceLost	= 0,
        CaptureMSG_DeviceFound	= ( CaptureMSG_DeviceLost + 1 ),

		CaptureMsg_GameLost		= 0x38,
		CaptureMsg_GameFound	= (CaptureMsg_GameLost + 1),
    } 	Capture_MSG;

typedef HRESULT (__stdcall *CB_CaptureMgr )( 
    void *userData,
    int32_t msg,
    int32_t wparam,
    int32_t lparam);

// 捕获流的管理接口

#ifndef __ICaptureMgr_INTERFACE_DEFINED__
#define __ICaptureMgr_INTERFACE_DEFINED__

class ICaptureMgr : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE InitInstance( void) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetCaptureCount( 
		/* [out][in] */ int32_t *pv) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetCaptureInfo( 
		/* [in] */ int32_t streamIdx,
		/* [out][in] */ Capture_Info *pci) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE CreateCaptureStream( 
		/* [in] */ int32_t streamIdx,
		/* [out][in] */ IUnknown **ppv) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetCallback( 
		/* [in] */ int32_t *userData,
		/* [in] */ int32_t *cb) = 0;
	
};


#endif 	/* __ICaptureMgr_INTERFACE_DEFINED__ */


#ifdef __cplusplus
}
#endif

#endif


