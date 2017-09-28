#pragma once
/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEClonable.h
 *@brief  Clone接口。
 *@version 1.0                          
 *@author  wangyx                       
 *@date    3/3/2016 10:52
 */

#include "NLEType.h"
#include <Guiddef.h>
#include <objbase.h>
                                                                                
//{0FAD75AA-A998-BB6C-0AE8-C9C50997D04D}
DEFINE_GUID(CLSID_NLEClonable, 0x0FAD75AA, 0xA998, 0xBB6C, 0x0A, 0xE8, 0xC9, 0xC5, 0x09, 0x97, 0xD0, 0x4D);                   
//{25628A35-47F5-6206-54F5-E1F9AA194226}
DEFINE_GUID(IID_INLEClonable, 0x25628A35, 0x47F5, 0x6206, 0x54, 0xF5, 0xE1, 0xF9, 0xAA, 0x19, 0x42, 0x26);     

/**
 *@brief 克隆接口，组件要尽量实现该接口。
 */
class INLEClonable :public IUnknown
{
public:
	
	/**
	 *@brief  	深复制对象到ppObj
	 *@param[ou] ppObj 复制得到的对象
	 *@return 	标准返回
	 */
	virtual HRESULT __stdcall Clone(LPVOID *ppObj) = 0;
};
