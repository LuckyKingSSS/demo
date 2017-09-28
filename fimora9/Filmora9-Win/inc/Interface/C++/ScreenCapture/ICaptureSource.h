// ScreenCapturer.cpp : Defines the exported functions for the DLL application.
//
#pragma once
#include <codecframe.h>
#include <FileInfo.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <scombase.h>
#include <COMMacro.h>
#include <Image.h>
#include <EncodeParam.h>
enum ScreenCaptureMode
{
	SCM_DEFAULT,
	SCM_D3D,
	SCM_GDI,
	SCM_AUDIO_CLIENT
};

/*ICaptureStream以及ICaptureStreamEx的StreamInfo的uStreamID填充字段*/
typedef enum NLEScreenRecordingStreamType 
{ 
    NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN = 0, 
    NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO = 1, //桌面视频 
    NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO = 2, //游戏视频 
    NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO = 3, //系统声音 
    NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO = 4,  //麦克风 
    NLE_SCREEN_RECORDING_STREAM_TYPE_MOUSE = 5, //鼠标流 
    NLE_SCREEN_RECORDING_STREAM_TYPE_KEYBOARD = 6, //键盘流 
    NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO = 7, //摄像头视频 
    NLE_SCREEN_RECORDING_STREAM_TYPE_MARK = 8, //MARK数据 
	NLE_SCREEN_RECORDING_STREAM_TYPE_UNREGISTER = 9, //显示未注册的视频流 
}NLEScreenRecordingStreamType;

typedef struct _CaptureStreamProperty
{
	union
	{
		NLEScreenRecordingStreamType streamType;
		int bFlip;
		int reserved[1024];
	};
}CaptureStreamProperty;

enum CaptureStreamPropertyEnum
{
	CSPEType = 0,
	CSPEFLIP = 1,
};
/*!
* \class ICaptureStreamProperty
* \brief 获取流属性 
*/
// {123AB767-4D3A-4e0c-A86D-FB023B999204}
DEFINE_GUID(IID_ICaptureStreamProperty, 
			0x123ab767, 0x4d3a, 0x4e0c, 0xa8, 0x6d, 0xfb, 0x2, 0x3b, 0x99, 0x92, 0x4);
EXTERN_C const IID IID_ICaptureStreamProperty;
MACRO_MIDL_INTERFACE(ICaptureStreamProperty, "123AB767-4D3A-4e0c-A86D-FB023B999204")
: public IUnknown
{
public:
	virtual STDMETHODIMP					GetProperty(enum CaptureStreamPropertyEnum propertyEnum, CaptureStreamProperty* pProperty) = 0;
};

/*!
* \class ICaptureStreamPushFrameHandler
* \brief 设置流输出回调 
*/
// {83249EC9-5FEF-45f5-8EF6-827778C680FE}
DEFINE_GUID(IID_ICaptureStreamPushFrameHandler, 
			0x83249ec9, 0x5fef, 0x45f5, 0x8e, 0xf6, 0x82, 0x77, 0x78, 0xc6, 0x80, 0xfe);
EXTERN_C const IID IID_ICaptureStreamPushFrameHandler;
MACRO_MIDL_INTERFACE(ICaptureStreamPushFrameHandler, "83249EC9-5FEF-45f5-8EF6-827778C680FE")
: public IUnknown
{
public:
	virtual STDMETHODIMP					EnablePushMode() = 0;
	virtual STDMETHODIMP					PushFrame(MEDIA_FRAME* pFrame) = 0;
	virtual STDMETHODIMP					SetNextPushFrameHandler(ICaptureStreamPushFrameHandler* pNextPushFrameHandler) = 0;
};

/*!
* \class ICaptureStreamRegister
* \brief 设置流输出回调 
*/
// {E6B4BCA6-D1A2-405d-8F64-8B4D826CD570}
DEFINE_GUID(IID_ICaptureStreamPushFrameRegister, 
			0xe6b4bca6, 0xd1a2, 0x405d, 0x8f, 0x64, 0x8b, 0x4d, 0x82, 0x6c, 0xd5, 0x70);
EXTERN_C const IID IID_ICaptureStreamPushFrameRegister;
MACRO_MIDL_INTERFACE(ICaptureStreamPushFrameRegister, "E6B4BCA6-D1A2-405d-8F64-8B4D826CD570")
: public IUnknown
{
public:
	virtual STDMETHODIMP					RegisterPushFrameCallbackHandler(ICaptureStreamPushFrameHandler* handler) = 0;
};

/*!
* \class ICursorAction
* \brief 获取媒体文件信息接口
*/
// {FA65C69E-A74D-4BF6-B704-71002C97677D}
DEFINE_GUID(IID_ICursorAction,
	0xfa65c69e, 0xa74d, 0x4bf6, 0xb7, 0x4, 0x71, 0x0, 0x2c, 0x97, 0x67, 0x7d);
EXTERN_C const IID IID_ICursorAction;
MACRO_MIDL_INTERFACE(ICursorAction, "FA65C69E-A74D-4BF6-B704-71002C97677D")
: public IUnknown
{
public:
	virtual STDMETHODIMP					AddClickAnimationItem(HDIBIMAGE hAnimationItem, int duration) = 0;
	virtual STDMETHODIMP					AddClickVoiceFile(wchar_t* pszVoiceFile) = 0;
	virtual STDMETHODIMP					Click(int xPos, int yPos) = 0;
};

/*!
* \class ICaptureStreamSetting
* \brief 获取媒体文件信息接口 
*/
// {4437A9F7-BE16-4d9e-A1F1-1041F2DFD766}
DEFINE_GUID(IID_ICaptureStreamSetting, 
			0x4437a9f7, 0xbe16, 0x4d9e, 0xa1, 0xf1, 0x10, 0x41, 0xf2, 0xdf, 0xd7, 0x66);
EXTERN_C const IID IID_ICaptureStreamSetting;
MACRO_MIDL_INTERFACE(ICaptureStreamSetting, "4437A9F7-BE16-4d9e-A1F1-1041F2DFD766")
: public IUnknown
{
public:
	virtual STDMETHODIMP					SetEncodeParam(EncodeParam* pEncParam) = 0;
};

/*!
 * \class IColorConverterFilter
 * \brief 
 */
// {F7C6F2C9-4620-41cb-B19E-31BEB77829E7}
DEFINE_GUID(IID_IColorConverterFilter,
            0xf7c6f2c9, 0x4620, 0x41cb, 0xb1, 0x9e, 0x31, 0xbe, 0xb7, 0x78, 0x29, 0xe7);
EXTERN_C const IID IID_IColorConverterFilter;
MACRO_MIDL_INTERFACE(IColorConverterFilter, "F7C6F2C9-4620-41cb-B19E-31BEB77829E7")
: public IUnknown
{
public:
	virtual STDMETHODIMP					SetCropRect(RECT rect) = 0;
};

/*!
* \class ICaptureStream
* \brief 录制源接口 
*/
// {8160232C-1334-476e-8573-8F39A797DB55}
DEFINE_GUID(IID_ICaptureStream, 
			0x8160232c, 0x1334, 0x476e, 0x85, 0x73, 0x8f, 0x39, 0xa7, 0x97, 0xdb, 0x55);
EXTERN_C const IID IID_ICaptureStream;
MACRO_MIDL_INTERFACE(ICaptureStream, "8160232C-1334-476e-8573-8F39A797DB55")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP Init(void)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Init() = 0;

	/*! \fn virtual STDMETHODIMP Start(void)
	* \brief 开始获取数据
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Start() = 0;

	/*! \fn virtual STDMETHODIMP_(MEDIA_FRAME*) LockFrame()
	* \brief 锁定一帧数据
	* \retval 返回帧数据
	*/
	virtual STDMETHODIMP_(MEDIA_FRAME*)		LockFrame() = 0;

	/*! \fn virtual STDMETHODIMP UnlockFrame(MEDIA_FRAME* pFrame)
	* \brief 解锁一帧数据
	* \param  pFrame [in / out] 帧数据
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					UnlockFrame(MEDIA_FRAME* pFrame) = 0;

	/*! \fn virtual STDMETHODIMP_(StreamInfo*) GetStreamInfo()
	* \brief 获取流信息
	* \retval 返回流信息
	*/
	virtual STDMETHODIMP_(StreamInfo*)		GetStreamInfo() = 0;
};

/*!
* \class ICaptureFilter
* \brief 获取媒体文件信息接口
*/
// {42D44AA8-4387-4CD2-BB3C-FAC7F2760CB3}
DEFINE_GUID(IID_ICaptureFilter,
	0x42d44aa8, 0x4387, 0x4cd2, 0xbb, 0x3c, 0xfa, 0xc7, 0xf2, 0x76, 0xc, 0xb3);
EXTERN_C const IID IID_ICaptureFilter;
MACRO_MIDL_INTERFACE(ICaptureFilter, "42D44AA8-4387-4CD2-BB3C-FAC7F2760CB3")
: public IUnknown
{
public:
	virtual STDMETHODIMP					SetInputStream(ICaptureStream* pInputStream) = 0;
	virtual STDMETHODIMP					GetOutputStream(ICaptureStream** ppOutputStream) = 0;
};

enum EAudioDeviceType
{
	eATypeOutput,		//扬声器输出
	eATypeInput,		//麦克风输入
};

typedef struct _CaptureAudioDeviceInfo
{
	enum EAudioDeviceType		eAudioDeviceType;
	wchar_t						*pszDeviceName;
	wchar_t						*pszDeviceID;
}CaptureAudioDeviceInfo;

#define CAPTURE_MESSAGE_WEBCAM_LOST 0x00000000
#define CAPTURE_MESSAGE_WEBCAM_FOUND 0x00000001

#define CAPTURE_MESSAGE_AUDIO_VOLUME 0x00000002 //lParam: wave 0~100
#define CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED 0x00000003 //uMsg
#define CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED_LOST CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED + 1 //lParam
#define CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED_FOUND CAPTURE_MESSAGE_AUDIO_DEVICE_CHANGED + 2 //lParam

#define CAPTURE_MESSAGE_GAMELOST 0x00000038
#define CAPTURE_MESSAGE_GAMEFOUND CAPTURE_MESSAGE_GAMELOST + 1


#ifdef WIN32
typedef int  int32_t;
#endif
typedef HRESULT (__stdcall *CaptureCallback)(void* pUserData, int32_t nMsg, int32_t wParam, int32_t lParam);

/*!
* \class ICaptureAudioVolume
* \brief 获取媒体文件信息接口 
*/
// {6893C1D6-BB91-46be-8EA0-562BBFB56F1D}
DEFINE_GUID(IID_ICaptureAudioVolume, 
			0x6893c1d6, 0xbb91, 0x46be, 0x8e, 0xa0, 0x56, 0x2b, 0xbf, 0xb5, 0x6f, 0x1d);
EXTERN_C const IID IID_ICaptureAudioVolume;
MACRO_MIDL_INTERFACE(ICaptureAudioVolume, "6893C1D6-BB91-46be-8EA0-562BBFB56F1D")
: public IUnknown
{
public:
	virtual STDMETHODIMP					Init(int nFreq, CaptureCallback fnCallback, void* pUserData) = 0;
	virtual STDMETHODIMP					Start() = 0;
	virtual STDMETHODIMP					Stop() = 0;
};

/*!
* \class IVideoCaptureDesktopMgr
* \brief 获取媒体文件信息接口 
*/
// {C2FEC89D-A3A4-4566-BA3C-46CA2776084C}
DEFINE_GUID(IID_IDesktopCaptureMgr, 
			0xc2fec89d, 0xa3a4, 0x4566, 0xba, 0x3c, 0x46, 0xca, 0x27, 0x76, 0x8, 0x4c);
EXTERN_C const IID IID_IDesktopCaptureMgr;
MACRO_MIDL_INTERFACE(IDesktopCaptureMgr, "C2FEC89D-A3A4-4566-BA3C-46CA2776084C")
: public IUnknown
{
public:
	virtual STDMETHODIMP					CreateDesktopCaptureSource(enum ScreenCaptureMode eMode, IUnknown** ppVideoCaptureSource) = 0;
	virtual STDMETHODIMP					CreateAudioCaptureSource(enum ScreenCaptureMode eMode, CaptureAudioDeviceInfo *pDeviceInfo, IUnknown** ppAudioCaptureSource) = 0;
	virtual STDMETHODIMP					AddCursorToVideoSource(ICaptureStream* pVideoCaptureSource, ICaptureStream** ppDstVideoCapture) = 0;
	virtual STDMETHODIMP					RefreshAudioDevices() = 0;
	virtual STDMETHODIMP_(int)				GetAudioDeviceCount() = 0;
	virtual STDMETHODIMP					GetAudioDevice(int nDeviceIndex, CaptureAudioDeviceInfo* pDeviceInfo) = 0;
	virtual STDMETHODIMP					GetAudioDeviceVolume(CaptureAudioDeviceInfo* pDeviceInfo, ICaptureAudioVolume** ppCaptureAudioVolume) = 0;
	virtual STDMETHODIMP					CreateGameCaptureSource(int dwProcessID, IUnknown** ppGameCaptureSource) = 0;
	virtual STDMETHODIMP_(BOOL)				CheckGameProcess(int dwProcessID) = 0;
	virtual STDMETHODIMP					CreateWebCamCapture(wchar_t* pszDeviceName, ICaptureStream** pWebCamCapture) = 0;
};


// {58DA667D-94C6-415a-82CA-5B8CAC3C821B}
//定义组件对象DecMgr
MIDL_DEFINE_GUID(CLSID, CLSID_CDesktopCaptureMgr, 0x58da667d, 0x94c6, 0x415a, 0x82, 0xca, 0x5b, 0x8c, 0xac, 0x3c, 0x82, 0x1b);

EXTERN_C const CLSID CLSID_CDesktopCaptureMgr;

enum EDeviceType
{
	eDTDesktop,			//桌面
	eDTWebCam,			//摄像头
	eDTGame,			//游戏
	eDTCurser,			//鼠标
	eDTKeybord,			//键盘
	eDTMark,			//Mark
	eDTAudioOutput,		//扬声器输出
	eDTAudioInput,		//麦克风输入
	eDTUnRegister,		//未注册显示的视频流
};

typedef struct _CaptureDeviceInfo
{
	enum EDeviceType			eDeviceType;
	wchar_t						displayName[256];
	wchar_t						deviceID[256];
	int							nDeviceIndex;
}CaptureDeviceInfo;


/*!
* \class IAudioVolumeSetting
* \brief 音频流音量控制接口 
*/
// {A8961966-AAF1-41cd-9881-858E917B6F17}
DEFINE_GUID(IID_IAudioVolumeSetting, 
			0xa8961966, 0xaaf1, 0x41cd, 0x98, 0x81, 0x85, 0x8e, 0x91, 0x7b, 0x6f, 0x17);
EXTERN_C const IID IID_IAudioVolumeSetting;
MACRO_MIDL_INTERFACE(IAudioVolumeSetting, "A8961966-AAF1-41cd-9881-858E917B6F17")
: public IUnknown
{
public:
	virtual STDMETHODIMP SetVolume(float fVolume) = 0;
};



DEFINE_GUID(IID_ICaptureStreamEx, 
			0xf321bc22, 0x5f44, 0x4481, 0x98, 0x88, 0xf4, 0x23, 0xeb, 0xce, 0x0a, 0xcb);
EXTERN_C const IID IID_ICaptureStreamEx;
MACRO_MIDL_INTERFACE(ICaptureStreamEx, "F321BC22-5F44-4481-9888-F423EBCE0ACB")
: public IUnknown
{
public:
	/*! \fn virtual STDMETHODIMP Init(void)
	* \brief 流初始化
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Init() = 0;

	/*! \fn virtual STDMETHODIMP Start(void)
	* \brief 开始获取数据
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					Start() = 0;

	/*! \fn virtual STDMETHODIMP_(MEDIA_FRAME*) LockFrame()
	* \brief 锁定一帧数据
	* \retval 返回帧数据
	*/
	virtual STDMETHODIMP					LockFrame(MEDIA_FRAME **ppFrame) = 0;

	/*! \fn virtual STDMETHODIMP UnlockFrame(MEDIA_FRAME* pFrame)
	* \brief 解锁一帧数据
	* \param  pFrame [in / out] 帧数据
	* \retval 获取结果
	*/
	virtual STDMETHODIMP					UnlockFrame(MEDIA_FRAME* pFrame) = 0;

	/*! \fn virtual STDMETHODIMP_(StreamInfo*) GetStreamInfo()
	* \brief 获取流信息
	* \retval 返回流信息
	*/
	virtual STDMETHODIMP					GetStreamInfo(StreamInfo *pStreamInfo) = 0;
};

/*!
* \class ICaptureVideoAnimation
* \brief 视频鼠标响应接口
*/
// {50699982-A029-4c83-85BE-2E25E0DAFB6B}
DEFINE_GUID(IID_ICaptureVideoAnimation,
	0x50699982, 0xa029, 0x4c83, 0x85, 0xbe, 0x2e, 0x25, 0xe0, 0xda, 0xfb, 0x6b);
EXTERN_C const IID IID_ICaptureVideoAnimation;
MACRO_MIDL_INTERFACE(ICaptureVideoAnimation, "50699982-A029-4c83-85BE-2E25E0DAFB6B")
: public IUnknown
{
public:
	virtual STDMETHODIMP					AddClickAnimationItem(HDIBIMAGE hAnimationItem, int duration) = 0;
	virtual STDMETHODIMP					CursorClick(int xPos, int yPos) = 0;
	virtual STDMETHODIMP					SetInputStream(ICaptureStreamEx* pInputStream) = 0;
	virtual STDMETHODIMP					GetOutputStream(ICaptureStreamEx** ppOutputStream) = 0;
};

// {43754B25-C563-4B45-909D-B0131C2A3803}
//定义组件对象CLSID_CCaptureVideoAnimation
MIDL_DEFINE_GUID(CLSID, CLSID_CCaptureVideoAnimation, 0x43754b25, 0xc563, 0x4b45, 0x90, 0x9d, 0xb0, 0x13, 0x1c, 0x2a, 0x38, 0x3);

EXTERN_C const CLSID CLSID_CCaptureVideoAnimation;

/*!
* \class ICaptureFilterEx
* \brief 获取媒体文件信息接口
*/
// {7150D0EC-DB75-4296-B062-0BFB7B31C98F}
DEFINE_GUID(IID_ICaptureFilterEx,
	0x7150d0ec, 0xdb75, 0x4296, 0xb0, 0x62, 0xb, 0xfb, 0x7b, 0x31, 0xc9, 0x8f);
EXTERN_C const IID IID_ICaptureFilterEx;
MACRO_MIDL_INTERFACE(ICaptureFilterEx, "7150D0EC-DB75-4296-B062-0BFB7B31C98F")
: public IUnknown
{
public:
	virtual STDMETHODIMP					SetInputStream(ICaptureStreamEx* pInputStream) = 0;
	virtual STDMETHODIMP					GetOutputStream(ICaptureStreamEx** ppOutputStream) = 0;
};

/*!
* \class IVideoCaptureDesktopMgr
* \brief 获取媒体文件信息接口 
*/
// {E42C7DCE-949D-4054-AA1D-C6399EE6F558}
DEFINE_GUID(IID_ICaptureMgrEx, 
			0xe42c7dce, 0x949d, 0x4054, 0xaa, 0x1d, 0xc6, 0x39, 0x9e, 0xe6, 0xf5, 0x58);
EXTERN_C const IID IID_ICaptureMgrEx;
MACRO_MIDL_INTERFACE(ICaptureMgrEx, "E42C7DCE-949D-4054-AA1D-C6399EE6F558")
: public IUnknown
{
public:
	virtual STDMETHODIMP					CreateCaptureStream(CaptureDeviceInfo *pDeviceInfo, IUnknown** ppCaptureStream) = 0;
	virtual STDMETHODIMP					RefreshDevices() = 0;
	virtual STDMETHODIMP					GetDeviceCount(int *pDeviceCount) = 0;
	virtual STDMETHODIMP					GetDevice(int nDeviceIndex, CaptureDeviceInfo* pDeviceInfo) = 0;
	virtual STDMETHODIMP					GetAudioDeviceVolume(CaptureDeviceInfo* pDeviceInfo, ICaptureAudioVolume** ppCaptureAudioVolume) = 0;
	virtual STDMETHODIMP					SetCallback(CaptureCallback fnCallback, void* pUserData) = 0;
};


// {2DCB5621-B5F2-4578-889F-CDBA0028C653}
//定义组件对象CLSID_CCaptureMgrEx
MIDL_DEFINE_GUID(CLSID, CLSID_CCaptureMgrEx, 0x2dcb5621, 0xb5f2, 0x4578, 0x88, 0x9f, 0xcd, 0xba, 0x0, 0x28, 0xc6, 0x53);

EXTERN_C const CLSID CLSID_CCaptureMgrEx;


/*!
* \class ICaptureAudioMixer
* \brief 声音混音以及鼠标声音响应接口
*/
// {9AAD8A61-58F3-46d1-86A4-E71CCCFF9153}
DEFINE_GUID(IID_ICaptureAudioMixer,
	0x9aad8a61, 0x58f3, 0x46d1, 0x86, 0xa4, 0xe7, 0x1c, 0xcc, 0xff, 0x91, 0x53);
EXTERN_C const IID IID_ICaptureAudioMixer;
MACRO_MIDL_INTERFACE(ICaptureAudioMixer, "9AAD8A61-58F3-46d1-86A4-E71CCCFF9153")
: public IUnknown
{
public:
	virtual STDMETHODIMP					InitMixer() = 0;
	virtual STDMETHODIMP					AddClickVoiceFile(wchar_t* pszNoiseFilePath) = 0;
	virtual STDMETHODIMP					AddInputStream(IUnknown* pInputStream) = 0;
	virtual STDMETHODIMP					GetOutputStream(IUnknown** ppOutputStream) = 0;
	virtual STDMETHODIMP					CursorClick() = 0;
};

// {4AE118B0-9741-4fbf-AFC7-B9F51074C7E5}
//定义组件对象CLSID_CCaptureAudioMixer
MIDL_DEFINE_GUID(CLSID, CLSID_CCaptureAudioMixer, 0x4ae118b0, 0x9741, 0x4fbf, 0xaf, 0xc7, 0xb9, 0xf5, 0x10, 0x74, 0xc7, 0xe5);

EXTERN_C const CLSID CLSID_CCaptureAudioMixer;
