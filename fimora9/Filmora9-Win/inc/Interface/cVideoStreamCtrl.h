

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:38 2017
 */
/* Compiler settings for cVideoStreamCtrl.idl:
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


#ifndef __cVideoStreamCtrl_h__
#define __cVideoStreamCtrl_h__

// {062C62D1-1564-4E3C-B603-41D2B937E9AF}
DEFINE_GUID(IID_IVideoStreamCtrl, 
0x62c62d1, 0x1564, 0x4e3c, 0xb6, 0x3, 0x41, 0xd2, 0xb9, 0x37, 0xe9, 0xaf);

#ifdef __cplusplus
extern "C"{
#endif 


typedef 
enum Video_Format
    {
        VideoFormat_YV12	= 0,
        VideoFormat_YUY2	= ( VideoFormat_YV12 + 1 ) ,
        VideoFormat_UYVY	= ( VideoFormat_YUY2 + 1 ) ,
        VideoFormat_ARGB	= ( VideoFormat_UYVY + 1 ) ,
        VideoFormat_RGBA	= ( VideoFormat_ARGB + 1 ) ,
        VideoFormat_RGB24	= ( VideoFormat_RGBA + 1 ) ,
        VideoFormat_H264	= ( VideoFormat_RGB24 + 1 ) ,
        VideoFormat_MJPEG	= ( VideoFormat_H264 + 1 ) 
    } 	Video_Format;

// 视频流的公共控制接口

#ifndef __IVideoStreamCtrl_INTERFACE_DEFINED__
#define __IVideoStreamCtrl_INTERFACE_DEFINED__

class IVideoStreamCtrl : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetFormatList( 
		/* [out][in] */ int32_t *formatList,
		/* [out][in] */ int32_t *cnt) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetFormat( 
		/* [out][in] */ int32_t *format) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetFormat( 
		/* [in] */ int32_t format) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetSizeList( 
		/* [out][in] */ int32_t *wList,
		/* [out][in] */ int32_t *hList,
		/* [out][in] */ int32_t *cnt) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetSize( 
		/* [out][in] */ int32_t *w,
		/* [out][in] */ int32_t *h) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetSize( 
		/* [in] */ int32_t w,
		/* [in] */ int32_t h) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetFramerateList( 
		/* [out][in] */ float *framerateList,
		/* [out][in] */ int32_t *cnt) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetFramerate( 
		/* [out][in] */ float *framerate) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetFramerate( 
		/* [in] */ float framerate) = 0;
	
};


#endif 	/* __IVideoStreamCtrl_INTERFACE_DEFINED__ */


#ifdef __cplusplus
}
#endif

#endif


