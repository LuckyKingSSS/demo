#pragma once

/**
* Copyright (c) 2016 Wondershare.inc
* All rights reserved.
*
*@file  INLEResLocator.h
*@brief
*@version 1.0
*@author  34973832
*@date    8/31/2016 11:29
*/

#ifndef _H_INLERESLOCATOR_H_
#define _H_INLERESLOCATOR_H_

#include <string>
#include <vector>
#include <functional>

#include <Guiddef.h>
#include <objbase.h>

#include "INLETimelineBase.h"

// {9F843B2F-0925-4DF0-A97B-FCD864BEA89E}
DEFINE_GUID(CLSID_NLEResLocator, 0x9f843b2f, 0x925, 0x4df0, 0xa9, 0x7b, 0xfc, 0xd8, 0x64, 0xbe, 0xa8, 0x9e);
// {67B45FBD-1A04-4300-B602-37F8235F1D28}
DEFINE_GUID(IID_INLEResLocator, 0x67b45fbd, 0x1a04, 0x4300, 0xb6, 0x2, 0x37, 0xf8, 0x23, 0x5f, 0x1d, 0x28);

#define RES_CLASS_SOURCE	L"resource.class.source" // SOURCE
#define RES_CLASS_MEDIA		L"resource.class.media" // MEDIA
#define RES_CLASS_CAPTION	L"resource.class.caption" // CAPTION
#define RES_CLASS_MONTAGE	L"resource.class.montage" // MONTAGE
#define RES_CLASS_DEFAULT	RES_CLASS_SOURCE // 默认，SOURCE
#define RES_CLASS_ALL		L"resource.class.all"

class INLEResLocator : public IUnknown
{
public:
	/**
	*@brief  	加载/卸载资源
	*@param	res_class 资源类别{加载资源时，禁止取值[RES_CLASS_ALL]}
	*@param	path 资源的路径{文件名全路径；可带环境变量；针对蒙太奇，则为其GUID}
	*@param	res_url 资源定位符{当卸载资源时，此为空，则卸载指定类别下的所有资源}
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall LoadRes(std::wstring &res_class, std::wstring &path, std::wstring &res_url) = 0;
	virtual HRESULT __stdcall UnloadRes(std::wstring &res_class, std::wstring &res_url) = 0;

	/**
	*@brief  	枚举已成功加载的资源
	*@param	res_class 资源类别
	*@param	vec_res_url 资源定位符列表
	*@param	fn 用于接收资源定位符的回调函数指针
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall EnumRes(std::wstring &res_class, std::vector<std::wstring> &vec_res_url) = 0;
	virtual HRESULT __stdcall EnumRes(std::wstring &res_class, std::function<bool(std::wstring res_url)> fn) = 0;

	/**
	*@brief  	根据资源定位符，获取资源的信息
	*@param	res_url 资源定位符
	*@param	prop 资源信息
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall GetResProp(std::wstring &res_url, NLEComPtr<INLEProperties> prop) = 0;
	
	/**
	*@brief  	生成/回收片段
	*@param	res_url 资源定位符
	*@param	ppv 接收新建的片段指针
	*@param	pv 需要回收的片段指针
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall New(std::wstring &res_url, INLETimelineBase **ppv) = 0;
	virtual HRESULT __stdcall Reclaim(INLETimelineBase *pv) = 0;

	/**
	*@brief 设置资源信息，功能与LoadRes相同
	*@param	res_class 资源类别{加载资源时，禁止取值[RES_CLASS_ALL]}
	*@param	path 资源的路径{文件名全路径；可带环境变量；针对蒙太奇，则为其GUID}
	*@param	prop 资源信息
	*@param	res_url 资源定位符{当卸载资源时，此为空，则卸载指定类别下的所有资源}
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall SetResProp(std::wstring &res_class, std::wstring &path, NLEComPtr<INLEProperties> prop, std::wstring &res_url) = 0;

	/**
	*@brief  	根据文件路径，获取资源的信息
	*@param	path 文件路径
	*@param	prop 资源信息
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall GetResPropByPath(std::wstring &res_class, std::wstring &path, NLEComPtr<INLEProperties> prop) = 0;
};

#endif //_H_INLERESLOCATOR_H_


