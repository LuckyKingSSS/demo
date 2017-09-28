/**
* Copyright (c) 2017 Wondershare.inc
* All rights reserved.
*
*@file  INLEMouseProcess.h
*@brief
*@version 1.0
*@author  liyong
*@date    8/15/2017
*/
#ifndef _H_INLEKEYBOARDPROCESS_H_
#define _H_INLEKEYBOARDPROCESS_H_

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            

#include "NLEComPtr.h"

#include "INLEStream.h"
#include "INLEFrame.h"

#include "cKeybStreamCtrl.h"
#include <list>

// {ABCA9857-A181-4E43-AB1D-2772ECB4CD13}
DEFINE_GUID(CLSID_INLEKeyBoardProcess,
    0xabca9857, 0xa181, 0x4e43, 0xab, 0x1d, 0x27, 0x72, 0xec, 0xb4, 0xcd, 0x13);

// {7E76514A-F2DA-49BE-8ED4-7FB03AFFCC20}
DEFINE_GUID(IID_INLEKeyBoardProcess,
    0x7e76514a, 0xf2da, 0x49be, 0x8e, 0xd4, 0x7f, 0xb0, 0x3a, 0xff, 0xcc, 0x20);

class INLEKeyBoardProcess :public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetKeyBoardStream(NLEComPtr<INLEStream> pStream) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetKeyBoardInfo(NLElonglong pts, NLElonglong deviation_time, NLEKeyBoardFrame **ppOutKeyboardInfo) = 0;

    virtual HRESULT STDMETHODCALLTYPE Render(NLEComPtr<INLEFrame> pFrame) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetAllKeyboardMarkerFramePts(std::list<NLElonglong>& all_pts) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetKeyboardFrameType(NLEKeyFrameType type) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetKeyboardFrameType(NLEKeyFrameType& type) = 0;

    virtual HRESULT STDMETHODCALLTYPE ShowSampleText(bool show) = 0;//调节键盘文字效果的时候是否显示样例文字
    virtual bool    STDMETHODCALLTYPE IsShowSampleText() = 0;
};



#endif 
