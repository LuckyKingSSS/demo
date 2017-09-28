
/*
*/
#pragma once
#include "CommonInterface.h"
#include <scombase.h>
#include "COMMacro.h"

#ifdef WIN32
typedef HWND HVIEWWND;
#else
typedef void* HVIEWWND;
#endif

// {7BD2B06A-4587-4a34-8759-848E9ED7566A}
DEFINE_GUID(IID_IStreamConvertPreview, 
			0x7bd2b06a, 0x4587, 0x4a34, 0x87, 0x59, 0x84, 0x8e, 0x9e, 0xd7, 0x56, 0x6a );
EXTERN_C const IID IID_IStreamConvertPreview;
MACRO_MIDL_INTERFACE(IStreamConvertPreview, "7BD2B06A-4587-4A34-8759-848E9ED7566A")
 : public IUnknown
{
public:
	virtual STDMETHODIMP SetView(HVIEWWND hHIView, RECT* pRect) = 0;
	virtual STDMETHODIMP EnablePreview(int nEnabled) = 0;
};

// {A4363541-9C7F-42ed-8EFA-53D2CE684328}
MIDL_DEFINE_GUID(CLSID, CLSID_CStreamConvertPreview, 0xa4363541, 0x9c7f, 0x42ed, 0x8e, 0xfa, 0x53, 0xd2, 0xce, 0x68, 0x43, 0x28 );

EXTERN_C const CLSID CLSID_CStreamConvertPreview;