#pragma once

/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLETimelineInfo.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    4/22/2016 10:46
 */


#include "NLEType.h" 
#include "NLEGraphics.h"
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{6E8AACA9-576C-AA94-3F79-E1DDD3149278}
DEFINE_GUID(CLSID_NLETimelineInfo, 0x6E8AACA9, 0x576C, 0xAA94, 0x3F, 0x79, 0xE1, 0xDD, 0xD3, 0x14, 0x92, 0x78);                   
//{8E2319C8-AC5B-970B-6A01-A203B394DE42}
DEFINE_GUID(IID_INLETimelineInfo, 0x8E2319C8, 0xAC5B, 0x970B, 0x6A, 0x01, 0xA2, 0x03, 0xB3, 0x94, 0xDE, 0x42);


typedef enum FrameIndexType
{
	FrameIndexType_RangeStart,
	FrameIndexType_RangeEnd,
	FrameIndexType_Any,
};

class INLETimelineInfo :public IUnknown
{
public:
	virtual HRESULT __stdcall UpdateSourceInfo() = 0;
	virtual HRESULT __stdcall UpdateRenderInfo() = 0;
	virtual NLEFrameIndex __stdcall GetSourceFrameIndex(const NLEFrameIndex frmIdxRener, FrameIndexType type) = 0;
	virtual NLEFrameIndex __stdcall GetRenderFrameIndex(const NLEFrameIndex frmIdxSource, FrameIndexType type) = 0;
};                                                                              
