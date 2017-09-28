/**
 * Copyright (c) 2016 Wondershare.inc
 * All rights reserved.
 *
 *@file  NLERenderPropComPtr.h
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
#include "NLERenderPropComPtr.h"

using namespace NLEKey::Render;

class NLERenderPropComPtr :public NLEComPtr<INLEProperties>
{
public:
	/** 设置渲染模式*/
	__inline HRESULT SetRenderMode(int nRenderMode) { return p->SetInt(kRender_Mode, nRenderMode); }

	/** 获得渲染模式*/
	__inline HRESULT GetRenderMode(int *pRenderMode) { return p->GetInt(kRender_Mode, pRenderMode); }

	/** 设置渲染画布大小*/
	__inline HRESULT SetRenderSize(const NLESize &oSize) { return p->SetSize(kRender_Size, oSize); }

	/** 获取渲染画面大小*/
	__inline HRESULT GetRenderSize(NLESize* pSize) { return p->GetSize(kRender_Size, pSize); }

	/** 设置渲染区域比例*/
	__inline HRESULT SetRenderRegion(const NLERectF& oRegion) { return p->SetRectF(kRender_Region, oRegion); }
	__inline HRESULT GetRenderRegion(NLERectF *pRegion) { return p->GetRectF(kRender_Region, pRegion); }

	/** 设置播放速度*/
	__inline HRESULT SetRenderScale(const NLERational &ralScale) { return p->SetRational(kRender_Scale, ralScale); }
	__inline HRESULT GetRenderScale(NLERational *pScale) { return p->GetRational(kRender_Scale, pScale); }
	__inline HRESULT SetRenderScaleEnable(const NLEBOOL bEnable){ return p->SetInt(kRender_Scale_Enable, bEnable); }
	__inline HRESULT GetRenderScaleEnable(NLEBOOL* bEnable){ return p->GetInt(kRender_Scale_Enable, bEnable); }


	/** 设置是否启用倒放*/
    __inline HRESULT SetRenderBackplay(const BOOL bEnable) { return p->SetInt(kRender_BackplayEnable, (INT)bEnable); }
	__inline HRESULT GetRenderBackplay(BOOL *pEnale) 
	{ 
		INT nEnable = 0;
		HRESULT hr = p->GetInt(kRender_BackplayEnable, &nEnable); 
		*pEnale = nEnable > 0;
		return hr;
	}
	/** 设置是否启用稳像*/
	__inline HRESULT SetRenderVideoStabilizerEnable(const BOOL bEnable) { return p->SetInt(kRender_VideoStabilizerEnable, (INT)bEnable); }
	__inline HRESULT GetRenderVideoStabilizerEnable(BOOL *pEnale)
	{
		INT nEnable = 0;
		HRESULT hr = p->GetInt(kRender_VideoStabilizerEnable, &nEnable);
		*pEnale = nEnable > 0;
		return hr;
	}

	__inline HRESULT SetRenderVideoStabilizerLevel(const INT iLevel) { return p->SetInt(kRender_VideoStabilizerLevel, (INT)iLevel); }
	__inline HRESULT GetRenderVideoStabilizerLevel(INT *pLevel)
	{
		INT iLevel = 0;
		HRESULT hr = p->GetInt(kRender_VideoStabilizerLevel, &iLevel);
		*pLevel = iLevel;
		return hr;
	}
	/** 设置是否启用鱼眼*/

	__inline HRESULT SetRenderFishEyeEnable(const BOOL bEnable) { return p->SetInt(kRender_FishEyeEnable, bEnable); }
	__inline HRESULT GetRenderFishEyeEnable(BOOL *pEnale)
	{
		INT nEnable = 0;
		HRESULT hr = p->GetInt(kRender_FishEyeEnable, &nEnable);
		*pEnale = nEnable > 0;
		return hr;
		return hr;
	}

	__inline HRESULT SetRenderFishEyeFileName(LPCWSTR pFileName) { return p->SetString(kRender_FishEyeFileName, pFileName); }
	__inline HRESULT GetRenderFishEyeFileName(LPWSTR  pFileName, int iLen = MAX_PATH)
	{
		HRESULT hr = p->GetString(kRender_FishEyeFileName, pFileName, iLen);
		if (wcslen(pFileName) < 3)
		{
			hr = S_FALSE;
		}
		return hr;
	}

	__inline HRESULT SetRenderFishEyeLevel(const INT iLevel) { return p->SetInt(kRender_FishEyeLevel, (INT)iLevel); }
	__inline HRESULT GetRenderFishEyeLevel(INT *pLevel)
	{
		INT iLevel = 0;
		HRESULT hr = p->GetInt(kRender_FishEyeLevel, &iLevel);
		*pLevel = iLevel;
		return hr;
	}

	/** 设置是否启用去噪音*/
	__inline HRESULT SetRenderDenoisze(const BOOL bEnable) { return p->SetInt(kRender_Denoise, (INT)bEnable); }
	__inline HRESULT GetRenderDenoisze(BOOL *pEnale)
	{
		INT nEnable = 0;
		HRESULT hr = p->GetInt(kRender_Denoise, &nEnable);
		*pEnale = nEnable > 0;
		return hr;
	}

	/** 设置是否启用去雾*/
	__inline HRESULT SetRenderDefog(const BOOL bEnable) { return p->SetInt(kRender_Defog, (INT)bEnable); }
	__inline HRESULT GetRenderDefog(BOOL *pEnale)
	{
		INT nEnable = 0;
		HRESULT hr = p->GetInt(kRender_Defog, &nEnable);
		*pEnale = nEnable > 0;
		return hr;
	}

	
	/** 设置是否启用重复播放*/
	__inline HRESULT SetRenderReplay(const INT nTimes) { return p->SetInt(kRender_Replay, nTimes); }
	__inline HRESULT GetRenderReplay(INT *pTimes) { return p->GetInt(kRender_Replay, pTimes); };

	__inline HRESULT SetRenderReplayEnable(const NLEBOOL bEnable) { return p->SetInt(kRender_Replay_Enable, bEnable); }
	__inline HRESULT GetRenderReplayEnable(NLEBOOL *pEnale) { return p->GetInt(kRender_Replay_Enable, pEnale); };


	/** 设置渲染帧率*/
	__inline HRESULT SetRenderFrameRate(const NLERational& oRal)  { return p->SetRational(kRender_Framerate, (NLERational)oRal);}
	__inline HRESULT GetRenderFrameRate(NLERational *pRal)  { return p->GetRational(kRender_Framerate, pRal);}

	/** 设置渲染帧数*/
	__inline HRESULT SetRenderTotalFrameCount(INT nCount)  { return p->SetInt(kRender_TotalFrameCount, nCount); }
	__inline HRESULT GetRenderTotalFrameCount(INT *pnCount)  { return p->GetInt(kRender_TotalFrameCount, pnCount); }
	
	/** 设置采样率*/
	__inline HRESULT SetRenderSampleRate(INT nSampleRate) { return p->SetInt(kRender_SampleRate, nSampleRate); }
	__inline HRESULT GetRenderSampleRate(INT *pSampleRate){ return p->GetInt(kRender_SampleRate, pSampleRate); }

	/** 设置声道数*/
	__inline HRESULT SetRenderChannels(INT nChannels) { return p->SetInt(kRender_Channels, nChannels); }
	__inline HRESULT GetRenderChannels(INT *pChannels){	return p->GetInt(kRender_Channels, pChannels); }

	/** 设置每个采样大小*/
	__inline HRESULT SetRenderBitsPerSample(INT nBits) { return p->SetInt(kRender_BitsPerSample, nBits); }
	__inline HRESULT GetRenderBitsPerSample(INT *pBites) { return p->GetInt(kRender_BitsPerSample, pBites); }


	/** 获取每秒字节数*/	
	__inline HRESULT GetRenderBytesPerSecond(INT* pBytes) 
    { 
        RETURN_IF_NULL(pBytes, E_INVALIDARG);

        INT nChannels = 0, nSampleRate = 0, nBits = 0;
        HRESULT hr = GetRenderChannels(&nChannels); NLE_ASSERT(SUCCEEDED(hr));
        hr = GetRenderSampleRate(&nSampleRate); NLE_ASSERT(SUCCEEDED(hr));
        hr = GetRenderBitsPerSample(&nBits); NLE_ASSERT(SUCCEEDED(hr));

        *pBytes = nChannels * nSampleRate * nBits / 8;
        return hr;
    }
	
	__inline HRESULT SetStreamLength(INT64 llStreamLen) { return p->SetInt64(NLEKey::Codec::Video::kDuration, llStreamLen);  }
	__inline HRESULT GetStreamLength(INT64 *pllStreamLen) { return p->GetInt64(NLEKey::Codec::Video::kDuration, pllStreamLen); }

	__inline HRESULT SetRenderRange(NLERange rngCut){ return p->SetRange(kRender_RangFrameNumber, rngCut); }
	__inline HRESULT GetRenderRange(NLERange *pRng) { return p->GetRange(kRender_RangFrameNumber, pRng); }

public:
	NLERenderPropComPtr()
	{
	}
	NLERenderPropComPtr(INLEProperties* p) :NLEComPtr(p)
	{
	}

	NLERenderPropComPtr(NLEComPtr<INLEProperties> p) :NLEComPtr(p)
	{
		
	}
	~NLERenderPropComPtr()
	{

	}

public:

	NLERenderPropComPtr* operator->() const throw()
	{
		return  (NLERenderPropComPtr*)this;
	}
public:
	
};

