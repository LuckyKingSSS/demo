#pragma once

#include <scombase.h>
#include "COMMacro.h"
#include "CommonInterface.h"
#include "StreamDecInterface.h"

DEFINE_GUID(IID_IStreamPlayer, 
			0xe405bc29, 0x8f28, 0x4605, 0xb6, 0xd, 0xe3, 0xdc, 0x64, 0xc1, 0x10, 0x18);


EXTERN_C const IID IID_IStreamPlayer;
MACRO_MIDL_INTERFACE(IStreamPlayer, "E405BC29-8F28-4605-B60D-E3DC64C11018")
: public IUnknown
{
public:
	virtual STDMETHODIMP SetStreamDec(IStreamDec *pStreamDec) = 0;

	virtual STDMETHODIMP SetHIViewRef(HWND hShowWnd, RECT* pRect) = 0;

	virtual STDMETHODIMP StartPreview() = 0;

	virtual STDMETHODIMP StopPreview(int nBlackFrame) = 0; //nBlackFrame 为1 就是将画面涂成黑色，为0就不变

	virtual STDMETHODIMP UpdateFrame() = 0;	//重绘
};

// {FB8BACED-BEBB-42c7-98CB-E889917886C0}

MIDL_DEFINE_GUID(CLSID, CLSID_CSTREAM_PLAYER, 0xfb8baced, 0xbebb, 0x42c7, 0x98, 0xcb, 0xe8, 0x89, 0x91, 0x78, 0x86, 0xc0);

EXTERN_C const CLSID CLSID_CSTREAM_PLAYER;