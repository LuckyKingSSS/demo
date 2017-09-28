/**
* Copyright (c) 2016 Wondershare.inc
* All rights reserved.
*
*@file  NLESourcePropComPtr.h
*@brief
*@version 1.0
*@author  wangyx
*@date    4/8/2016 12:05
*/

#pragma once

#include "NLEComPtr.h"
#include "INLEProperties.h"
#include "NLEKey.h"
#include "NLEMacroDefine.h"

using namespace NLEKey::Source;

class NLESourcePropComPtr :public NLEComPtr<INLEProperties>
{
public:
	/** 设置渲染画布大小*/
	__inline HRESULT SetSourceSize(const NLESize& oSize) { return p->SetSize(kSource_Size, oSize); }

	/** 获取渲染画面大小*/
	__inline HRESULT GetSourceSize(NLESize* pSize) { return p->GetSize(kSource_Size, pSize); }

    __inline HRESULT SetSourceTotalFrameCount(int nSrcFrmCount) { return p->SetInt(kSource_TotalFrameCount, nSrcFrmCount); }
    __inline HRESULT GetSourceTotalFrameCount(int *pFrameCount) { return p->GetInt(kSource_TotalFrameCount, pFrameCount); }

  	/** 设置渲染区域比例*/
	__inline HRESULT SetSourceRegion(const NLERectF& oRegion) { return p->SetRectF(kSource_Region, oRegion); }

	__inline HRESULT GetSourceRegion(NLERectF *pRegion) { return p->GetRectF(kSource_Region, pRegion); }

	/** 设置渲染帧率*/
	__inline HRESULT SetSourceFramerate(const NLERational & oRal)  { return p->SetRational(kSource_Framerate, (NLERational)oRal); }
	__inline HRESULT GetSourceFramerate(NLERational *pRal)  { return p->GetRational(kSource_Framerate, pRal); }

	/** 设置采样率*/
	__inline HRESULT SetSourceSampleRate(INT nSrcSmplrate) { return p->SetInt(kSource_SampleRate, nSrcSmplrate); }
	__inline HRESULT GetSourceSampleRate(INT *pSampleRate){ return p->GetInt(kSource_SampleRate, pSampleRate); }

	/** 设置声道数*/
	__inline HRESULT SetSourceChannels(INT nSrcChannls) { return p->SetInt(kSource_Channels, nSrcChannls); }
	__inline HRESULT GetSourceChannels(INT *pChannels){ return p->GetInt(kSource_Channels, pChannels); }

	/** 设置每个采样大小*/
	__inline HRESULT SetSourceBitsPerSample(INT nBits) { return p->SetInt(kSource_BitsPerSample, nBits); }
	__inline HRESULT GetSourceBitsPerSample(INT *pBites) { return p->GetInt(kSource_BitsPerSample, pBites); }

	/** 设置每秒字节数*/
	//__inline HRESULT SetSourceBytesPerSecond(INT nBytes) { return p->SetInt(kSource_BytesPerSecond, nBytes); }
	//__inline HRESULT GetSourceBytesPerSecond(INT* pBytes) { return p->GetInt(kSource_BytesPerSecond, pBytes); }

	/** 设置起始和结束帧序号*/
	__inline HRESULT SetSourceRange(NLERange rngCut) { return p->SetRange(kSource_RangeFrameNumber, rngCut); }
	__inline HRESULT GetSourceRange(NLERange *pRngCut){ return p->GetRange(kSource_RangeFrameNumber, pRngCut); }
	

public:
	NLESourcePropComPtr()
	{
	}
	NLESourcePropComPtr(INLEProperties* p) :NLEComPtr(p)
	{
	}

	NLESourcePropComPtr(NLEComPtr<INLEProperties> p) :NLEComPtr(p)
	{		
	}
	~NLESourcePropComPtr()
	{

	}

public:

	NLESourcePropComPtr* operator->() const throw()
	{
		return  (NLESourcePropComPtr*)this;
	}
};


