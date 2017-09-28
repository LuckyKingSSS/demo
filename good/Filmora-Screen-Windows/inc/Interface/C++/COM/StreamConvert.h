#pragma once

#include <scombase.h>
#include "COMMacro.h"
#include "CommonInterface.h"
#include "StreamDevice.h"
#include "StreamDecInterface.h"
#include "DecodeParam.h"
#include "MediaConverter.h"

//œ¬√Êµƒœ˚œ¢”√ªß…Ë±∏–≈œ¢ªÿµ˜
#ifdef WIN32
typedef HWND HVIEWWND;
#else
typedef void* HVIEWWND;
#endif

DEFINE_GUID(IID_IStreamConverter, 
			0xef83ae58, 0xfb64, 0x4fc8, 0x88, 0x82, 0x79, 0x94, 0x4f, 0x6f, 0x2b, 0x11);

EXTERN_C const IID IID_IStreamConverter;
MACRO_MIDL_INTERFACE(IStreamConverter, "EF83AE58-FB64-4fc8-8882-79944F6F2B11")
: public IUnknown
{
public:
	virtual STDMETHODIMP SetStreamDec(IStreamDec *pStreamDec) = 0;

	virtual STDMETHODIMP SetHIViewRef(HVIEWWND hHIView, RECT* pRect) = 0;

	virtual STDMETHODIMP GetEncodeParam(IMediaEncParam** pMediaEncParam) = 0;
	
	virtual STDMETHODIMP SetFrameRate(double dFrameRate) = 0;
	
	virtual STDMETHODIMP SetVideoEnable(int bVideoEnable) = 0;
	virtual STDMETHODIMP SetAudioEnable(int bVideoEnable) = 0;

	virtual STDMETHODIMP SetCallBack(MULTI_CALLBACK pCallBack, void *pUserObj) = 0;

	virtual STDMETHODIMP SetDesFilePath(const BSTR filePath) = 0;

	virtual STDMETHODIMP StartConvert() = 0; 
	
	virtual STDMETHODIMP StartPreview() = 0;

	virtual STDMETHODIMP StopPreview() = 0;

	virtual STDMETHODIMP EndConvert() = 0;
};

// {1F61FDDA-3071-41a1-8552-0B4411E0727E}
//∂®“Â◊Èº˛∂‘œÛStreamConverter

MIDL_DEFINE_GUID(CLSID, CLSID_CSTREAM_CONVERTER, 0x1f61fdda, 0x3071, 0x41a1, 0x85, 0x52, 0xb, 0x44, 0x11, 0xe0, 0x72, 0x7e);

EXTERN_C const CLSID CLSID_CSTREAM_CONVERTER;