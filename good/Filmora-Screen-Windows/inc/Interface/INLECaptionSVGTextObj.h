
#ifndef _NLE_SVGTEXT_PROPERTY_TYPE_
#define _NLE_SVGTEXT_PROPERTY_TYPE_

#include <Guiddef.h>                                                     
#include <objbase.h>       

// {830F58C5-0472-46C6-AE5A-01BE14CA8F70}
DEFINE_GUID(CLSID_INLECaptionSVGTextObj,
	0x830f58c5, 0x472, 0x46c6, 0xae, 0x5a, 0x1, 0xbe, 0x14, 0xca, 0x8f, 0x70);

// {4E480D24-72FD-4D8D-9B22-4C78C3E40E12}
DEFINE_GUID(IID_INLECaptionSVGTextObj,
	0x4e480d24, 0x72fd, 0x4d8d, 0x9b, 0x22, 0x4c, 0x78, 0xc3, 0xe4, 0xe, 0x12);

class INLECaptionSVGTextObj :public IUnknown
{
public:
	/**
	*@brief    
	*@param 
	*@return
	*/
	virtual bool __stdcall GetOverlayCount(int* nCount) = 0;
	/**
	*@brief    
	*@param 
	*@return
	*/
	virtual bool __stdcall GetOverlayColorByIdx(DWORD* Color, int nIdx) = 0;
	/**
	*@brief    
	*@param 
	*@return
	*/
	virtual bool __stdcall SetOverlayColorByIdx(DWORD Color, int nIdx) = 0;
};


#endif