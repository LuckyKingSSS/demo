#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{64E4FFF3-4081-3596-3D94-788E8783B8E8}
DEFINE_GUID(CLSID_INLEMath, 0x64E4FFF3, 0x4081, 0x3596, 0x3D, 0x94, 0x78, 0x8E, 0x87, 0x83, 0xB8, 0xE8);                   
//{86B628BB-2E91-EEF1-BC80-B30DEF8B3C41}
DEFINE_GUID(IID_INLEMath, 0x86B628BB, 0x2E91, 0xEEF1, 0xBC, 0x80, 0xB3, 0x0D, 0xEF, 0x8B, 0x3C, 0x41);                         
class INLEMath :public IUnknown
{
public:
	/**
	*@brief function
	*@param[in] x input 1
	*@param[out] py output 1
	*@return result
	*/
	virtual HRESULT STDMETHODCALLTYPE InterfaceExample(int x, int *py) = 0;
};                                                                              


