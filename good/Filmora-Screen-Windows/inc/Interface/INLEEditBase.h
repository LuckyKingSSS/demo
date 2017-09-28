#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
                                                                                
//{82F9F435-BFB8-A45E-5579-8D03133C0E1A}
DEFINE_GUID(CLSID_INLEEditBase, 0x82F9F435, 0xBFB8, 0xA45E, 0x55, 0x79, 0x8D, 0x03, 0x13, 0x3C, 0x0E, 0x1A);                   
//{11F060D5-B930-10B4-0BD8-D06D152B8771}
DEFINE_GUID(IID_INLEEditBase, 0x11F060D5, 0xB930, 0x10B4, 0x0B, 0xD8, 0xD0, 0x6D, 0x15, 0x2B, 0x87, 0x71);  

class INLEEditBase :public IUnknown
{
public:
	/**
	*@brief  	获取渲染引擎
	*@param		pAudioRender 音频渲染
	*@param		pVideoRender 视频渲染
	*@return 	HRESULT
	*@see 		参考资料
	*@note		注意事项
	*/
	virtual HRESULT __stdcall GetRender(INLERender** pAudioRender, INLERender **pVideoRender) = 0;

	/**
	*@brief  	事件通知
	*@param	oEvent 要通知的事件对象
	*@param	wParam 参数1
	*@param	lParam 参数2
	*@return 	标准返回值
	*/
	virtual HRESULT __stdcall Notify(NLEEVENT oEvent, WPARAM wParam, LPARAM lParam) = 0;
};                                                                              


