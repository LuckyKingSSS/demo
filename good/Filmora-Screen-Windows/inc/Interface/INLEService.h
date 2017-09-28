#pragma once

#ifndef _H_INLESERVICE_H_
#define _H_INLESERVICE_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEProperties.h"
    

//{0E5F10F5-1C45-7F12-A4B2-47D60493AC1C}
DEFINE_GUID(CLSID_NLEService, 0x0E5F10F5, 0x1C45, 0x7F12, 0xA4, 0xB2, 0x47, 0xD6, 0x04, 0x93, 0xAC, 0x1C);                   
//{15AF3DC7-41BC-7319-D3F3-415E40B37890}
DEFINE_GUID(IID_INLEService, 0x15AF3DC7, 0x41BC, 0x7319, 0xD3, 0xF3, 0x41, 0x5E, 0x40, 0xB3, 0x78, 0x90);                         
/**@ingroup GroupCommon
 *@brief Service接口，具有属性接口的类都可从此类继承
 */
class INLEService :public IUnknown
{
public:
	virtual HRESULT __stdcall Init(void) = 0;

	virtual void __stdcall RegisterListeners() = 0;

	virtual HRESULT __stdcall GetReconstructProperties(INLEProperties **ppProperties) = 0;
	virtual HRESULT __stdcall SetReconstructProperties(INLEProperties *pProperties) = 0;

	/**
	 *@brief 设置父亲接口，不增加引用计数
	 */
	virtual HRESULT __stdcall SetParent(IUnknown *pUnk) = 0;

	/**
	 *@brief 获取父节点，不增加引用计数
	 */
	virtual IUnknown * __stdcall GetParent() = 0;

	/**
	 *@brief 获取服务访问次数
	 *@note 当前用途：访问次数变化，说明属性被更改了
	 */
	virtual NLESession __stdcall GetSession() = 0;
};                       

#endif //_H_INLESERVICE_H_


