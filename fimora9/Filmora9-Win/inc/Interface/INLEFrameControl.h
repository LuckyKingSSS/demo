/*
* Copyright (c) 2013 Wondershare.inc
* All rights reserved.
*
*@file    INLEFrame.h
*@brief   定义音频/视频/文字数据帧接口
*@version 1.0
*@author  wangyx
*@date    2016/03/02 14:11
*/

#pragma once
#ifndef _H_INLEFRAMECONTROL_H_
#define _H_INLEFRAMECONTROL_H_


#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include <vector>
#include "INLEService.h"
#include "INLERender.h"

/** @ingroup GroupTimeline
*  主要包括 INLEFrame
*/

//{F8BA02CA-9151-E710-6BBD-3958446C913F}
DEFINE_GUID(CLSID_NLEFrameControl, 0xF8BA02CA, 0x9151, 0xE710, 0x6B, 0xBD, 0x39, 0x58, 0x44, 0x6C, 0x91, 0x3F);                   
//{2E8383E9-CA01-430B-BD48-6D699572C274}
DEFINE_GUID(IID_INLEFrameControl, 0x2E8383E9, 0xCA01, 0x430B, 0xBD, 0x48, 0x6D, 0x69, 0x95, 0x72, 0xC2, 0x74);
/**
 *@brief 帧控制接口，NLE内部使用即可，能够导出以下接口:
 *	- INLEProperties 帧属性，用于扩展
 *	- INLECloable 复制接口，能够复制帧
 */
class INLEFrameControl :public IUnknown
{
public:
	/**
	 *@brief  	添加要经过的Render
	 *@param	pRender 时间线，通过IUnknown查询得到INLETimelineBase接口
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		PushRender~IsProcessed是否需要暴露给外部？
	 */
	virtual HRESULT __stdcall PushRender(INLERender *pRender) = 0;
	/**
	 *@brief  	移除所有Render	 
	 *@return 	HRESULT __stdcall
	 */
	virtual HRESULT __stdcall ClearAllRender() = 0;
	/**
	 *@brief  	Render个数
	 *@return 	个数
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual UINT    __stdcall GetRenderCount() = 0;
	
	/**
	 *@brief  	得到第iIndex个Render接口
	 *@param	iIndex 序号
	 *@param	ppRender Render接口
	 *@return 	HRESULT __stdcall
	 *@see 		GetRenderCount
	 */
	virtual HRESULT __stdcall GetRender(UINT iIndex, INLERender **ppRender) = 0;
	
	/**
	 *@brief  	获取所有Render
	 *@param	renderList Render列表
	 *@return 	标准
	 */
	virtual HRESULT __stdcall GetAllRenders(std::vector<INLERender*> renderList) = 0;

	/**
	 *@brief  	是否都已被Render处理了。
	 *@return 	BOOL 取值如下：
	 *			- TRUE 已全部处理
	 *			- FALSE 为全部处理
	 *@see 		剩余没有处理的Render可通过GetRender得到。
	 */
	virtual BOOL __stdcall IsProcessed() = 0;

	/*--------------------------------------------------------------------------*/

	
		
	/**
	*@brief  	保存数据到文件
	*@param		pFileFullPath 文件全路径
	*@return 	标准返回
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall SaveToFile(LPCWSTR *pFileFullPath) = 0;
};

#endif //_H_INLEFRAMECONTROL_H_


