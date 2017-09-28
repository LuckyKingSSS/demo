#pragma once

/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEAsyncTask.h
 *@brief                                
 *@version 1.0                          
 *@author  34973832                       
 *@date    8/31/2016 11:29
 */

#ifndef _H_INLEASYNCTASK_H_
#define _H_INLEASYNCTASK_H_

#include <vector>

#include "NLEType.h"
#include <Guiddef.h>
#include <objbase.h>

#include "INLERender.h"
#include "NLEComPtr.h"

// {7A718A08-3D4A-4B1A-9D33-5509F0448124}
DEFINE_GUID(CLSID_NLESeekTask, 0x7A718A08, 0x3D4A, 0x4B1A, 0x9D, 0x33, 0x55, 0x09, 0xF0, 0x44, 0x81, 0x24);
// {71989CD6-F79D-42A0-BC85-E28238E8B2FE}
DEFINE_GUID(IID_INLESeekTask, 0x71989CD6, 0xF79D, 0x42A0, 0xBC, 0x85, 0xE2, 0x82, 0x38, 0xE8, 0xB2, 0xFE);

class INLESeekTask :public IUnknown
{
public:
	/**
	 *@brief  	提交异步定位任务
	 *@param	render 需要定位的渲染器
	 *@param	frameIdx 定位的位置
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料	 
	 */
	virtual HRESULT __stdcall Seek(std::vector<std::pair<NLEComPtr<INLERender>, NLEFrameIndex>> &seekList) = 0;
};

// {8EBA0687-7884-46BF-A63C-6BAAEB53D2B9}
DEFINE_GUID(CLSID_NLEPrepareTask, 0x8eba0687, 0x7884, 0x46bf, 0xa6, 0x3c, 0x6b, 0xaa, 0xeb, 0x53, 0xd2, 0xb9);
// {9CF97967-6F34-4C30-BCC4-624DC676CAFE}
DEFINE_GUID(IID_INLEPrepareTask, 0x9cf97967, 0x6f34, 0x4c30, 0xbc, 0xc4, 0x62, 0x4d, 0xc6, 0x76, 0xca, 0xfe);

class INLEPrepareTask :public IUnknown
{
public:
	/**
	 *@brief  	提交准备任务
	 *@param	renderList 需要准备的渲染器列表
	 *@param	frame 目标帧
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料	 
	 */
	virtual HRESULT __stdcall Prepare(std::vector<NLEComPtr<INLERender>> &renderList, NLEComPtr<INLEFrame> &frame) = 0;
}; 

#endif //_H_INLEASYNCTASK_H_


