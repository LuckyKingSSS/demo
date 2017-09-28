#pragma once
#include <EncodeParam.h>
#include "COMMacro.h"


/*!
* \class ICaptureEngine
* \brief 获取媒体文件信息接口 
*/
// {85B03E98-8309-4eda-B1DE-E602356164E5}
DEFINE_GUID(IID_ICaptureEngine, 
			0x85b03e98, 0x8309, 0x4eda, 0xb1, 0xde, 0xe6, 0x2, 0x35, 0x61, 0x64, 0xe5);
EXTERN_C const IID IID_ICaptureEngine;
MACRO_MIDL_INTERFACE(ICaptureEngine, "85B03E98-8309-4eda-B1DE-E602356164E5")
: public IUnknown
{
public:
	virtual STDMETHODIMP Init() = 0;
	virtual STDMETHODIMP Start() = 0;
	virtual STDMETHODIMP Stop() = 0;
	virtual STDMETHODIMP SetScreenCapturer(IUnknown* pVideoCaptureSource, BOOL bCaptureCursor) = 0;
	virtual STDMETHODIMP SetAudioCapturer(IUnknown* pAudioCaptureSource) = 0;
	virtual STDMETHODIMP SetEncodeParam(EncodeParam* pEncParam) = 0;
	virtual STDMETHODIMP SetOutputFile(wchar_t *pszFilePath) = 0;
	virtual STDMETHODIMP SetCropRect(RECT rect) = 0;
	virtual STDMETHODIMP SetCaptureCallback(CaptureCallback captureCallback, void* pUserData) = 0;
	virtual STDMETHODIMP AddClickAnimationItem(HDIBIMAGE hAnimationItem, int duration) = 0;
	virtual STDMETHODIMP AddClickVoiceFile(wchar_t* pszNoiseFilePath) = 0;
	virtual STDMETHODIMP CursorClick(int xPos, int yPos) = 0;
};

// {F7AFB65B-D0EF-4778-9F0B-E6236DE49591}
//定义组件对象DecMgr
MIDL_DEFINE_GUID(CLSID, CLSID_CCaptureEngine, 0xf7afb65b, 0xd0ef, 0x4778, 0x9f, 0xb, 0xe6, 0x23, 0x6d, 0xe4, 0x95, 0x91);

EXTERN_C const CLSID CLSID_CCaptureEngine;

#ifdef __cplusplus
extern "C" {
#endif
void CaptureInit(int isAppStore);
#ifdef __cplusplus
};
#endif