#pragma once
/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLESerialize.h
 *@brief                                
 *@version 1.0                          
 *@author  wangyx                       
 *@date    3/3/2016 14:38
 */

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEXml.h"
#include "INLETimelineBase.h"
                                                                                
//{B09298AE-5D75-2C74-8B25-6E2ACA86BE9C}
DEFINE_GUID(CLSID_NLESerialize, 0xB09298AE, 0x5D75, 0x2C74, 0x8B, 0x25, 0x6E, 0x2A, 0xCA, 0x86, 0xBE, 0x9C);                   
//{49778407-97EA-71A8-9974-DF65E9336575}
DEFINE_GUID(IID_INLESerialize, 0x49778407, 0x97EA, 0x71A8, 0x99, 0x74, 0xDF, 0x65, 0xE9, 0x33, 0x65, 0x75);   

/**
 *@brief 序列化接口
 */
class INLESerialize :public IUnknown
{
public:
	virtual HRESULT __stdcall Save(INLEXml *pArchive) = 0;
	virtual HRESULT __stdcall Load(INLEXml *pArchive) = 0;
};  



