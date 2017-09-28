

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:36 2017
 */
/* Compiler settings for cMarkStreamCtrl.idl:
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

#ifndef __cMarkStreamCtrl_h__
#define __cMarkStreamCtrl_h__

// {7ECD57FD-82F7-4E88-A847-3C069A19775D}
DEFINE_GUID(IID_IMarkStreamCtrl, 
0x7ecd57fd, 0x82f7, 0x4e88, 0xa8, 0x47, 0x3c, 0x6, 0x9a, 0x19, 0x77, 0x5d);

#ifdef __cplusplus
extern "C"{
#endif 


typedef struct Mark_Frame
    {
    int16_t keyCode;
    int16_t charCode;
    int32_t auxCode;
    int32_t delay;
    } 	Mark_Frame;

// 标记流的控制接口


#ifndef __IMarkStreamCtrl_INTERFACE_DEFINED__
#define __IMarkStreamCtrl_INTERFACE_DEFINED__

class IMarkStreamCtrl : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE PushFrame( 
		/* [in] */ Mark_Frame *pv) = 0;
	
};
    

#endif 	/* __IMarkStreamCtrl_INTERFACE_DEFINED__ */


#ifdef __cplusplus
}
#endif

#endif


