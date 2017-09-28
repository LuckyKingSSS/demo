/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLERepository.h
 *@brief 存储接口，用于管理dll动态库及资源路径等。
 *@version 1.0                          
 *@author  wangyx                       
 *@date    3/3/2016 11:14
 */

#ifndef _H_INLEREPOSITORY_H_
#define _H_INLEREPOSITORY_H_


#include "NLEType.h"
#include <Guiddef.h>
#include "objbase.h"
#include "INLEProperties.h"
                                        
///@ingroup GroupGUID
//{8D91AD35-1109-8BC5-518E-AC68D4C9990B}
DEFINE_GUID(CLSID_NLERepository, 0x8D91AD35, 0x1109, 0x8BC5, 0x51, 0x8E, 0xAC, 0x68, 0xD4, 0xC9, 0x99, 0x0B);  

///@ingroup GroupGUID
//{91C4ADCA-B92C-9937-BA77-5A225358E886}
DEFINE_GUID(IID_INLERepository, 0x91C4ADCA, 0xB92C, 0x9937, 0xBA, 0x77, 0x5A, 0x22, 0x53, 0x58, 0xE8, 0x86);   

/**@ingroup GroupExtension
 *@brief 仓库管理，用于管理NLE中库信息和对象创建等。
 */
class INLERepository :public IUnknown
{
public:
	
	/**
	 *@brief  	初始化
	 *@param	szDiNLERectory 目录，其中存放一些xml信息，例如clsid表等。
	 *@return 	HRESULT __stdcall
	 *@see 		参考资料
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall Initialize(LPCWSTR szDiNLERectory = NULL) = 0;

	/**
	 *@brief  	获得版本信息
	 *@param	name 对应的dll名称，资源名称，底层SDK名称等，为NULL代表NLE。
	 *@return 	const LPCWSTR 版本号及说明
	 */
	virtual LPCWSTR __stdcall GetGetVersion(LPCWSTR *name = NULL) = 0;
	
	/**
	 *@brief  	获得VideoFilter资源相关信息
	 *@return 	信息表	 
	 *@note		信息表子段各个Resouce独立定义
	 */
	virtual INLEProperties* __stdcall  GetVideoFilterInfo() = 0;

	/**
	*@brief  	获得AudioFilter资源相关信息
	*@return 	信息表
	*@note		信息表子段各个Resouce独立定义
	*/
	virtual INLEProperties* __stdcall  GetAudioFilterInfo() = 0;

	/**
	*@brief  	获得MaskFilter资源相关信息
	*@return 	信息表
	*@note		信息表子段各个Resouce独立定义
	*/
	virtual INLEProperties* __stdcall  GetMaskFilterInfo() = 0;
	
    virtual LPCWSTR __stdcall GetLoggerPath() = 0;

	/**
	 *@brief  	创建对象
	 *@param	rclsid CLSID
	 *@param	outer  
	 *@param	context
	 *@param	riid
	 *@param	ppv
	 *@return 	HRESULT __stdcall
	 *@see 		内部查找clsid所在的dll，然后进行创建。
	 *@note		注意事项
	 */
	virtual HRESULT __stdcall CreateInstance(REFCLSID rclsid, LPUNKNOWN outer, DWORD context, REFIID riid, LPVOID *ppv) = 0;

	/**
	*@brief  	获得Motion资源相关信息
	*@return 	信息表
	*@note		信息表子段各个Resouce独立定义
	*/
	virtual INLEProperties* __stdcall  GetMotionTemplateInfo() = 0;

	/**
	*@brief  	获得Montage资源相关信息
	*@return 	信息表
	*@note		信息表子段各个Resouce独立定义
	*/
	virtual INLEProperties* __stdcall  GetMontageTemplateInfo() = 0;

	/**
	*@brief  	获得FishEye资源相关信息
	*@return 	信息表
	*@note		信息表子段各个Resouce独立定义
	*/
	virtual INLEProperties* __stdcall  GetFishEyeTemplateInfo() = 0;
};


#endif //_H_INLEREPOSITORY_H_

