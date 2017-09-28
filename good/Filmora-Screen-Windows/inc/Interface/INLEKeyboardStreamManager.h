
/**
* Copyright (c) 2016 Wondershare.inc
* All rights reserved.
*
*@file  NLEMouseEffectManager.h
*@brief
*@version 1.0
*@author  liyong
*@date    8/22/2017 15:28
*/

#ifndef _H_NLEKEYBOARDSTREAMMANAGER_H_
#define _H_NLEKEYBOARDSTREAMMANAGER_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include "INLETimelineBase.h"
#include "cKeybStreamCtrl.h"

// {C2293A20-0194-4929-A7C0-F0E374D9516C}
DEFINE_GUID(CLSID_INLEKeyBoardStreamManager,0xc2293a20, 0x194, 0x4929, 0xa7, 0xc0, 0xf0, 0xe3, 0x74, 0xd9, 0x51, 0x6c);
//{E51D060F-13C7-7272-FDB3-3EFAF75BCD70}
// {7C01003C-9753-46E4-BCF4-526777BFF80D}
DEFINE_GUID(IID_INLEKeyBoardStreamManager,0x7c01003c, 0x9753, 0x46e4, 0xbc, 0xf4, 0x52, 0x67, 0x77, 0xbf, 0xf8, 0xd);
class INLEKeyboardStreamManager :public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE CreateKeyboardCaptionTimeline() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetKeyboardCaptionTimeline(INLETimelineBase* & pCaptionTimeline) = 0;
    virtual HRESULT STDMETHODCALLTYPE PrepareKeyboardMarkers() = 0;//准备所有需要遍历的Marker点
    virtual HRESULT STDMETHODCALLTYPE GetNextMarkerPosition(NLEFrameIndex& index) = 0;//取得下一个Marker点
    virtual HRESULT STDMETHODCALLTYPE SetKeyFilterType(NLEKeyFrameType type) = 0; //设置显示按键的类型
    virtual HRESULT STDMETHODCALLTYPE GetKeyFilterType(NLEKeyFrameType& type) = 0; 
    virtual HRESULT STDMETHODCALLTYPE SetKeyTextOpacity(double opacity) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetKeyTextOpacity(double& opacity) = 0;
    virtual HRESULT STDMETHODCALLTYPE ShowSampleText(bool show) = 0;

    //for NLE internal
    virtual HRESULT STDMETHODCALLTYPE InstallSourceTimeline(INLETimelineBase *pSourceTimeline) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRenderFrameRate(NLEFramerate render_frame_rate) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRenderScale(NLERational render_scale) = 0;//设置变速倍率
};

#endif
