#pragma once

/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  NLECommon_OS.h
 *@brief 平台相关性的函数声明
 *@version 1.0                          
 *@author  wangyx                       
 *@date    7/22/2016 09:45
 */

#ifndef _H_NLECOMMON_OS_H_
#define _H_NLECOMMON_OS_H_

#include <objbase.h>
#include "NLEConfig.h"
#include "NLECommon_Const.h"

namespace NLECommon
{
	namespace OS
	{
		typedef enum
		{
			SystemID_Windows,
			SystemID_Mac,
			SystemID_iOS,
			SystemID_Android,
		}SystemID;
		
		/**@brief 当前系统ID*/
		NLECOMMON_API SystemID GetSystemID();

		/**@brief 系统版本号*/
		NLECOMMON_API LPCWSTR GetSystemVersion();

		/**@brief 动态库文件的后缀名，例如".dll"*/
		NLECOMMON_API LPCWSTR GetDynamicLibraryPostfix();

	};
};

#endif //_H_NLECOMMON_OS_H_


