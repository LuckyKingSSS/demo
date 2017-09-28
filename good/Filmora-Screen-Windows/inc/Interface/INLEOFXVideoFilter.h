
#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>

#ifndef _NLE_OFXFILTER_PROPERTY_TYPE_
#define _NLE_OFXFILTER_PROPERTY_TYPE_
                                                
// {5BDDC678-21D2-48D4-A563-91604292FD2A}
DEFINE_GUID(CLSID_NLEOFXVideoFilter, 
0x5bddc678, 0x21d2, 0x48d4, 0xa5, 0x63, 0x91, 0x60, 0x42, 0x92, 0xfd, 0x2a);
// {6F9F38FF-1DD8-4C6C-8874-5B3AC706457A}
DEFINE_GUID(IID_INLEOFXVideoFilter,
0x6f9f38ff, 0x1dd8, 0x4c6c, 0x88, 0x74, 0x5b, 0x3a, 0xc7, 0x6, 0x45, 0x7a);

/**
 *@brief 视频Filter对应的Timeline
 */
class INLEOFXVideoFilter :public IUnknown
{
public:
	virtual INT __stdcall GetFilterPropertyCount() = 0;
	virtual HRESULT __stdcall GetFilterProperty(int nIndex, INLEProperties **pProperties) = 0;
};

#endif // _NLE_OFXFILTER_PROPERTY_TYPE_