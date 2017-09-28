

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:35 2017
 */
/* Compiler settings for cKeybStreamCtrl.idl:
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

#ifndef __cKeybStreamCtrl_h__
#define __cKeybStreamCtrl_h__

// {BF8C6571-A973-49A7-821B-AC2C01675AC1}
DEFINE_GUID(IID_IKeybStreamCtrl, 
0xbf8c6571, 0xa973, 0x49a7, 0x82, 0x1b, 0xac, 0x2c, 0x1, 0x67, 0x5a, 0xc1);

#ifdef __cplusplus
extern "C"{
#endif 

    enum NLEKeyFrameType
    {
        NORMAL = 0,
        MARKER,
        HOT_KEY
    };

typedef struct NLEKeyBoardFrame
    {
        int8_t          keyString[100];
        NLEKeyFrameType frameType;
        unsigned char   version;
    } 	NLEKeyBoardFrame;

// 键盘流的控制接口

#ifndef __IKeybStreamCtrl_INTERFACE_DEFINED__
#define __IKeybStreamCtrl_INTERFACE_DEFINED__


class IKeybStreamCtrl : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE PushFrame( 
		/* [in] */ NLEKeyBoardFrame *pv) = 0;
	
};


#endif 	/* __IKeybStreamCtrl_INTERFACE_DEFINED__ */


#ifdef __cplusplus
}
#endif

#endif


