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

#ifndef _H_INLEENVIRONMENT_H_
#define _H_INLEENVIRONMENT_H_

#include <string>

#include <Guiddef.h>
#include <objbase.h>

// {5A704834-BCC4-437B-B1E4-51427271200C}
DEFINE_GUID(CLSID_NLEEnviroment, 0x5a704834, 0xbcc4, 0x437b, 0xb1, 0xe4, 0x51, 0x42, 0x72, 0x71, 0x20, 0xc);
// {92B2F536-D238-4EE3-90FB-2F6CFAC1F25D}
DEFINE_GUID(IID_INLEEnviroment, 0x92b2f536, 0xd238, 0x4ee3, 0x90, 0xfb, 0x2f, 0x6c, 0xfa, 0xc1, 0xf2, 0x5d);

// 目录最后统一不带斜杠或者反斜杠
#define ENV_CURRENT_DIR		L"CurrentDir"
#define ENV_RESOURCE_DIR	L"ResourceDir"

class INLEEnviroment : public IUnknown
{
public:
	/**
	*@brief  	设置/获取环境变量
	*@param	k 环境变量的名称
	*@param	v 环境变量的值
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall SetEnv(std::wstring &k, std::wstring &v) = 0;
	virtual HRESULT __stdcall GetEnv(std::wstring &k, std::wstring &v) = 0;

	/**
	*@brief  	将含有环境变量的字符串转换成普通的字符串{即，其中的环境变量被替换成对应的值}
	*@param	envString 源字符串
	*@param	plainString 普通字符串
	*@return 	HRESULT __stdcall
	*@see 		参考资料
	*/
	virtual HRESULT __stdcall ReplaceEnvByValue(std::wstring &envString, std::wstring &plainString) = 0;
};

#endif //_H_INLEENVIRONMENT_H_


