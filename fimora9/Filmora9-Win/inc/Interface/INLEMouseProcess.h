/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEMouseProcess.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    2/27/2017 1658
 */
#ifndef _H_INLEMOUSEPROCESS_H_
#define _H_INLEMOUSEPROCESS_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include "NLEComPtr.h"

#include "INLEMouseSetting.h"
#include "INLEStream.h"
#include "INLEXml.h"
#include "INLEFrame.h"
#include "cMouseStreamCtrl.h"

//{2FB7333E-0304-6D37-EBE4-2151D169188A}
DEFINE_GUID(CLSID_INLEMouseProcess, 0x2FB7333E, 0x0304, 0x6D37, 0xEB, 0xE4, 0x21, 0x51, 0xD1, 0x69, 0x18, 0x8A);                   
//{B939A9B6-6248-809C-CE2F-0DBB376E41D6}
DEFINE_GUID(IID_INLEMouseProcess, 0xB939A9B6, 0x6248, 0x809C, 0xCE, 0x2F, 0x0D, 0xBB, 0x37, 0x6E, 0x41, 0xD6);                         
class INLEMouseProcess :public IUnknown
{
public:	
	/**
	*@brief 设置点击鼠标时播放的声音文件
	*@param[in] soundFilePath 
	*@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE SetSoundFilePath(std::wstring soundFilePath) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetOpacity(double opacity) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetAudioMute(int bMute) = 0;

	/**
	*@brief 设置鼠标点击的声音音量
	*@param volume 音量值，范围为[-100, 100], 0表示不调节
	*/	
	virtual HRESULT STDMETHODCALLTYPE SetAudioVolume(int volume) = 0;

	/**
	*@brief 光标图标的缩放百分比，如值为5.0表示500%, 0.2表示20%
	*/
	virtual HRESULT STDMETHODCALLTYPE SetScale(double scale) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetMouseStream(NLEComPtr<INLEStream> pStream) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetMouseInfo(NLElonglong pts, Mouse_Frame **ppOutMouseInfo) = 0;

	virtual HRESULT STDMETHODCALLTYPE Render(NLEComPtr<INLEFrame> pFrame) = 0;


};                                                                              



#endif //_H_INLEMOUSEPROCESS_H_
