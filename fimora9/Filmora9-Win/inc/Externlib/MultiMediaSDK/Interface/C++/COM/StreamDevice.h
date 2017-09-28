#pragma once
#include <scombase.h>
#include "COMMacro.h"
#ifndef DEC_MESSAGE
#define DEC_MESSAGE

#define WM_VIDEO_DEVICE_LOST  0x2001	// ”∆µ…Ë±∏∂™ ß
#define WM_AUDIO_DEVIDE_LOST  0x2002    //“Ù∆µ…Ë±∏∂™ ß

#endif


#ifdef WIN32
#ifndef WMU_CAPTURE_MESSAGE

#define WMU_CAPTURE_MESSAGE		WM_USER + 1000
#define WMU_CAPTURE_PREVIEW		WMU_CAPTURE_MESSAGE + 1
#define WMU_CAPTURE_ENGINE		WMU_CAPTURE_MESSAGE + 2

#endif
#endif

#ifndef DEVICE_ENVENT
#define DEVICE_ENVENT
typedef enum VAC_CONTROL_EVENT
{
	VAC_EVENT_DEVICE_LOST = WM_USER + 1000,
	VAC_EVENT_PREVIEW_FAIL = WM_USER + 1001,	//预览失败

}VAC_EVENT_MESSAGE;
#endif



DEFINE_GUID(IID_IVideoResolutionInfo, 
			0x7c3bd43, 0xd147, 0x4846, 0xb4, 0x79, 0xa8, 0x49, 0xc3, 0xc2, 0x36, 0x6b);
EXTERN_C const IID IID_IVideoResolutionInfo;
MACRO_MIDL_INTERFACE(IVideoResolutionInfo, "07C3BD43-D147-4846-B479-A849C3C2366B")
: public IUnknown
{
public:
	virtual STDMETHODIMP GetVideoResolution(int &nWidth, int &nHeight) = 0;	
};


//设备分辨率枚举类
DEFINE_GUID(IID_IEnumVideoResolutions, 
			0x43edf010, 0x914b, 0x4c77, 0xae, 0x39, 0x1e, 0xf6, 0x85, 0xaa, 0x5f, 0xd1);
EXTERN_C const IID IID_IEnumVideoResolutions;
MACRO_MIDL_INTERFACE(IEnumVideoResolutions, "43EDF010-914B-4c77-AE39-1EF685AA5FD1")
: public IUnknown
{
public:
	virtual STDMETHODIMP_(int) GetCount() = 0;
	virtual STDMETHODIMP GetItem(int nIndex, IVideoResolutionInfo** ppItem) = 0;	
};


DEFINE_GUID(IID_IInputDeviceName, 
			0xa2afc92f, 0xb68a, 0x42af, 0x8c, 0x53, 0x2a, 0xa5, 0x41, 0xb4, 0x73, 0x6);
EXTERN_C const IID IID_IInputDeviceName;
MACRO_MIDL_INTERFACE(IInputDeviceName, "A2AFC92F-B68A-42af-8C53-2AA541B47306")
: public IUnknown
{
public:
	virtual STDMETHODIMP GetInputDeviceName(BSTR *pDevName) = 0;	
};


DEFINE_GUID(IID_IInputDevices, 
			0x7c666a7c, 0xde71, 0x4601, 0xac, 0xe2, 0xbe, 0x7a, 0x85, 0x6b, 0xac, 0x5c);
EXTERN_C const IID IID_IInputDevices;
MACRO_MIDL_INTERFACE(IInputDevices, "7C666A7C-DE71-4601-ACE2-BE7A856BAC5C")
: public IUnknown
{
public:
	virtual STDMETHODIMP_(int) GetCount() = 0;
	virtual STDMETHODIMP GetItem(int nIndex, IInputDeviceName** ppItem) = 0;	
};



DEFINE_GUID(IID_IDeviceName, 
			0xfbfc222d, 0x2bf7, 0x4aa1, 0x91, 0xf2, 0xf8, 0xa1, 0x5a, 0xc9, 0x66, 0x7b);
EXTERN_C const IID IID_IDeviceName;
MACRO_MIDL_INTERFACE(IDeviceName, "FBFC222D-2BF7-4aa1-91F2-F8A15AC9667B")
: public IUnknown
{
public:
	virtual STDMETHODIMP GetDeviceName(BSTR *pDevName) = 0;	
	
	virtual STDMETHODIMP GetInputDevices(IInputDevices **inputDevices) = 0;		//获取输入的音频设备，仅仅WIN下可以MAC下不可以，MAC不用管
};


DEFINE_GUID(IID_IOutPutDevices, 
			0x8f8c13, 0xc664, 0x4f6e, 0xbf, 0x26, 0xa1, 0xac, 0xc9, 0x6, 0xac, 0x60);
EXTERN_C const IID IID_IOutPutDevices;
MACRO_MIDL_INTERFACE(IOutPutDevices, "008F8C13-C664-4f6e-BF26-A1ACC906AC60")
: public IUnknown
{
public:
	virtual STDMETHODIMP_(int) GetCount() = 0;
	virtual STDMETHODIMP GetItem(int nIndex, IDeviceName** ppItem) = 0;	
};
