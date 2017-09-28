#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{97ECAF4C-00D7-BB71-B08E-8FA76D4BFE2C}
DEFINE_GUID(CLSID_NLEPreProcess, 0x97ECAF4C, 0x00D7, 0xBB71, 0xB0, 0x8E, 0x8F, 0xA7, 0x6D, 0x4B, 0xFE, 0x2C);
//{FD679380-D6E7-E826-9865-94319C0E6ECB}
DEFINE_GUID(IID_INLEPreProcess, 0xFD679380, 0xD6E7, 0xE826, 0x98, 0x65, 0x94, 0x31, 0x9C, 0x0E, 0x6E, 0xCB);

class INLEPreProcess :public IUnknown
{
public:

	/**
	 *@brief  	设置SouceTimeline，Proxy相关
	 *@param	pSourceTimeline
	 *@param	bShared pSourceTimeline是否被多个共享的？
	 *@return 	HRESULT __stdcall
	 */
	virtual HRESULT __stdcall SetSourceTimeline(INLETimelineBase* pSourceTimeline, BOOL bShared = FALSE) = 0;
};                                                                              


