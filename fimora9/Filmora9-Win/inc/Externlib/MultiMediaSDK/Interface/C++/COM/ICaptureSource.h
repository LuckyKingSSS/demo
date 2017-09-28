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
    virtual STDMETHODIMP                    SetRGBA() = 0;
};

/*!
* \class ICaptureStream
* \brief 获取媒体文件信息接口 
*/
// {8160232C-1334-476e-8573-8F39A797DB55}
DEFINE_GUID(IID_ICaptureStream, 
			0x8160232c, 0x1334, 0x476e, 0x85, 0x73, 0x8f, 0x39, 0xa7, 0x97, 0xdb, 0x55);
EXTERN_C const IID IID_ICaptureStream;
MACRO_MIDL_INTERFACE(ICaptureStream, "8160232C-1334-476e-8573-8F39A797DB55")
: public IUnknown
{
public:
	virtual STDMETHODIMP					Init() = 0;
	virtual STDMETHODIMP					Start() = 0;
	virtual STDMETHODIMP_(MEDIA_FRAME*)		LockFrame() = 0;
	virtual STDMETHODIMP					UnlockFrame(MEDIA_FRAME* pFrame) = 0;
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

#define CAPTURE_MESSAGE_AUDIO_VOLUME 0x00000001 //lParam: wave 0~100
typedef int (__stdcall *CaptureCallback)(void* pUserData, WPARAM wParam, LPARAM lParam);

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
};

// {58DA667D-94C6-415a-82CA-5B8CAC3C821B}
//定义组件对象DecMgr
MIDL_DEFINE_GUID(CLSID, CLSID_CDesktopCaptureMgr, 0x58da667d, 0x94c6, 0x415a, 0x82, 0xca, 0x5b, 0x8c, 0xac, 0x3c, 0x82, 0x1b);

EXTERN_C const CLSID CLSID_CDesktopCaptureMgr;