/**                                     
 * Copyright (c) 2016 Wondershare.inc   
 * All rights reserved.                 
 *                                      
 *@file  INLEMouseEffectManager.h
 *@brief                                
 *@version 1.0                          
 *@author  yaoyj                       
 *@date    3/20/2017 15:10
 */

#ifndef _H_INLEMOUSEEFFECTMANAGER_H_
#define _H_INLEMOUSEEFFECTMANAGER_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include "INLETimelineBase.h"
#include "cMouseStreamCtrl.h"
                                                                                
//{96BE27DD-B8E4-F9CD-4D95-5CE8B83BFC0C}
DEFINE_GUID(CLSID_INLEMouseEffectManager, 0x96BE27DD, 0xB8E4, 0xF9CD, 0x4D, 0x95, 0x5C, 0xE8, 0xB8, 0x3B, 0xFC, 0x0C);                   
//{E51D060F-13C7-7272-FDB3-3EFAF75BCD70}
DEFINE_GUID(IID_INLEMouseEffectManager, 0xE51D060F, 0x13C7, 0x7272, 0xFD, 0xB3, 0x3E, 0xFA, 0xF7, 0x5B, 0xCD, 0x70);                         
class INLEMouseEffectManager :public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE AddEffectFilter(INLETimelineBase *pFilter, MouseEvent mouseEvent = MouseEvent_None, int level = 0) = 0;

	virtual int STDMETHODCALLTYPE EffectFilterCount() = 0;

	virtual HRESULT STDMETHODCALLTYPE GetEffectFilter(int index, INLETimelineBase **ppFilter) = 0;

	virtual HRESULT STDMETHODCALLTYPE RemoveEffectFilter(INLETimelineBase *pFilter) = 0;

	/**
	@brief 设置光标的缩放比例
	@param scale 缩放比例，如0.2表示20%
	@return 标准错误
	*/
	virtual HRESULT STDMETHODCALLTYPE SetMouseScale(double scale) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetMouseScale(double *pScale) = 0;

	/**
	@brief 设置光标的透明度
	@param opacity 值为0 - 1.0
	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE SetOpacity(double opacity) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetOpacity(double *pOpacity) = 0;

	/**
	@brief 设置点击鼠标左键时播放的声音文件路径
	@param pSoundFilePath 全路径
	@return 标准错误码
	*/
	virtual HRESULT STDMETHODCALLTYPE SetSoundFilePath(LPCWSTR pSoundFilePath) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetSoundFilePath(LPWSTR pSoundFilePath, int nMaxBufferSize) = 0;

	virtual HRESULT STDMETHODCALLTYPE RemoveSound() = 0;

	/**
	@brief 设置是否启用静音模式
	@param bMute 1表示静音模式， 0表示正常播放音频
	*/
	virtual HRESULT STDMETHODCALLTYPE SetAudioMute(int bMute) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetAudioMute(int *pbMute) = 0;

	/**
	@brief 调节音频的音量
	@param volume 音量值,范围为[-100, 100]
	*/
	virtual HRESULT STDMETHODCALLTYPE SetAudioVolume(int volume) = 0;

	virtual HRESULT STDMETHODCALLTYPE GetAudioVolume(int *pVolume) = 0;
};                                                                              


/**
该接口仅供NLE内部使用
*/
// {B08E7AC3-71C4-44A7-AA9B-98E82469EB35}
DEFINE_GUID(IID_INLEMouseEffectManagerInternal,
	0xb08e7ac3, 0x71c4, 0x44a7, 0xaa, 0x9b, 0x98, 0xe8, 0x24, 0x69, 0xeb, 0x35);

class INLEMouseEffectManagerInternal : public IUnknown
{
public:
	/**
	*@brief 设置SourceTimeline
	*@param[in] pSourceTimeline
	*@return 标准输出
	*/
	virtual HRESULT STDMETHODCALLTYPE InstallSourceTimeline(INLETimelineBase *pSourceTimeline) = 0;
};

#endif //_H_INLEMOUSEEFFECTMANAGER_H_
