

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Mar 02 11:38:33 2017
 */
/* Compiler settings for cCaptureStream.idl:
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

#ifndef __cCaptureStream_h__
#define __cCaptureStream_h__


/* header files for imported files */
#include "cAudioStreamCtrl.h"
#include "cVideoStreamCtrl.h"
#include "cGameStreamCtrl.h"
#include "cDesktopStreamCtrl.h"
#include "cMouseStreamCtrl.h"
#include "cKeybStreamCtrl.h"
#include "cMarkStreamCtrl.h"

// {1DBECCF4-F06A-49EB-A71B-D0F8EA2A93F4}
DEFINE_GUID(IID_ICaptureStream, 
0xf321bc22, 0x5f44, 0x4481, 0x98, 0x88, 0xf4, 0x23, 0xeb, 0xce, 0x0a, 0xcb);

// {123AB767-4D3A-4e0c-A86D-FB023B999204} 
DEFINE_GUID(IID_ICaptureStreamProperty,
0x123ab767, 0x4d3a, 0x4e0c, 0xa8, 0x6d, 0xfb, 0x2, 0x3b, 0x99, 0x92, 0x04);

// {5CA5D651-E880-471e-A553-2C36A5982933} 
DEFINE_GUID(IID_ICaptureStreamControl,
0x5ca5d651, 0xe880, 0x471e, 0xa5, 0x53, 0x2c, 0x36, 0xa5, 0x98, 0x29, 0x33);

#ifdef __cplusplus
extern "C"{
#endif 


// 捕获流的公共接口

#ifndef __ICaptureStream_INTERFACE_DEFINED__
#define __ICaptureStream_INTERFACE_DEFINED__

class ICaptureStream : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Init( void) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE Start( void) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE LockFrame( 
		/* [out][in] */ int32_t **ppFrame) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE UnlockFrame( 
		/* [in] */ int32_t *pFrame) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE GetStreamInfo( 
		/* [out][in] */ int32_t *psi) = 0;
	
};


#endif 	/* __ICaptureStream_INTERFACE_DEFINED__ */

#ifndef NLE_RENDER_TYPE_DEFINE_
#define NLE_RENDER_TYPE_DEFINE_
/**
@brief 屏幕录制的文件中的数据流的类型
1. 桌面录制+系统声音组合为一个Timline;
2.游戏视频+系统声音组合为一个Timline;
3.麦克声音为一个Timline;
4.摄像头视频为一个Timline
*/
typedef enum NLEScreenRecordingStreamType
{
	NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN = 0,
	NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO = 1, //桌面视频
	NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO = 2, //游戏视频
	NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO = 3, //系统声音
	NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO = 4,	//麦克风
	NLE_SCREEN_RECORDING_STREAM_TYPE_MOUSE = 5, //鼠标流
	NLE_SCREEN_RECORDING_STREAM_TYPE_KEYBOARD = 6, //键盘流
	NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO = 7, //摄像头视频
	NLE_SCREEN_RECORDING_STREAM_TYPE_MARK = 8, //标记信息流
	NLE_SCREEN_RECORDING_STREAM_TYPE_UNREGISTER = 9, //显示未注册的视频流 
}NLEScreenRecordingStreamType;
#endif

typedef struct CaptureStreamProperty
{
	union
	{
		NLEScreenRecordingStreamType streamType;
		int bFlip;
		int reserved[1024];
	};
} CaptureStreamProperty;

typedef enum CaptureStreamPropertyEnum
{
	CSPEType = 0,
	CSPEFLIP = 1,

} CaptureStreamPropertyEnum;

#ifndef __ICaptureStreamProperty_INTERFACE_DEFINED__
#define __ICaptureStreamProperty_INTERFACE_DEFINED__

class ICaptureStreamProperty : public IUnknown
{
public:
	virtual STDMETHODIMP GetProperty(
		/* [in] */ CaptureStreamPropertyEnum propertyEnum,
		/* [out][in] */ CaptureStreamProperty *pProperty) = 0;
};

#endif 	/* __ICaptureStreamProperty_INTERFACE_DEFINED__ */

#define CAPTURESTREAM_CONTROL_STOP 0x00000001 //停止源 

#ifndef __ICaptureStreamControl_INTERFACE_DEFINED__
#define __ICaptureStreamControl_INTERFACE_DEFINED__

class ICaptureStreamControl : public IUnknown
{
public:
	virtual STDMETHODIMP Control(/* [in] */ int cmd, /* [in] */ void *arg) = 0;
};

#endif 	/* __ICaptureStreamControl_INTERFACE_DEFINED__ */

#ifdef __cplusplus
}
#endif

#endif


