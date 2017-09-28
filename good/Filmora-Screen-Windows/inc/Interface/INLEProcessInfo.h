#pragma once

#ifndef _H_INLEPROCESSINFO_H_
#define _H_INLEPROCESSINFO_H_


#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "NLEComPtr.h"
#include "INLEProperties.h"
                                                                                
//{30F14A4E-C1C8-6D4F-E932-9B53EC17696A}
DEFINE_GUID(CLSID_INLEProcessInfo, 0x30F14A4E, 0xC1C8, 0x6D4F, 0xE9, 0x32, 0x9B, 0x53, 0xEC, 0x17, 0x69, 0x6A);                   
//{01CEC2C6-421E-6DF9-6F48-6BE66328474C}
DEFINE_GUID(IID_INLEProcessInfo, 0x01CEC2C6, 0x421E, 0x6DF9, 0x6F, 0x48, 0x6B, 0xE6, 0x63, 0x28, 0x47, 0x4C);                         
class INLEProcessInfo :public IUnknown
{
public:
	virtual HRESULT __stdcall SetSourceInfo(const NLEPropDecorator   pSourceInfo) = 0;
	virtual HRESULT __stdcall GetSourceInfo(NLEPropDecorator  pSourceInfo) = 0;

	virtual HRESULT __stdcall GetProcessedInfo(NLEPropDecorator  pProcessedInfo) = 0;

	virtual HRESULT __stdcall SetGUID(GUID guid) = 0;

	virtual BOOL  __stdcall IsNeedTask(void) = 0;
};                                                                              


#endif //_H_INLEPROCESSINFO_H_


