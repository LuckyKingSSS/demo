#include "stdafx.h"
#include "NLEAudioRenderBase.h"
#include "INLEFrameControl.h"
#include "INLEFrameOperate.h"
#include "INLETimelineBase.h"
#include "NLEComPtr.h"
#include "NLECommon.h"
#include "NLEMacroDefine.h"

#include "NLEKey.h"
#include "NLEKeyShortcut.h"
#include "NLEComError.h"
#include "NLECommon_Const.h"
#include "INLEClonable.h"

using namespace NLECommon;
using namespace NLECommon::Const;

PROPERTY_LISTEN_BEGIN(CNLEAudioRenderBase)

PROPERTY_LISTEN_END

CNLEAudioRenderBase::CNLEAudioRenderBase(IUnknown *pUnkOuter) :CNLERenderBase(pUnkOuter)
{
	PROPERTY_LISTENERS_REGISTER(CNLEAudioRenderBase);
	m_pMixerFrame = m_pRenderFrame = NULL;
}


CNLEAudioRenderBase::~CNLEAudioRenderBase()
{
}

STDOVERRIDEMETHODIMP CNLEAudioRenderBase::NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj)
{
	if (riid == IID_INLEAudioRender)
	{
		return CNLERenderBase::NonDelegatingQueryInterface(IID_INLERender, ppObj);
	}
	return CNLERenderBase::NonDelegatingQueryInterface(riid, ppObj);
}

HRESULT __stdcall  CNLEAudioRenderBase::Seek(NLEFrameIndex nFrameIndex, NLEFrameIndex *pRealFrameIndex/* = NULL*/)
{
	NLEFrameIndex retIdx = 0;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);

	HRESULT hr = CNLERenderBase::Seek(nFrameIndex, &retIdx);
	if (pRealFrameIndex != NULL)
	{
		*pRealFrameIndex = retIdx;
	}

	// 	LOGGER_INFO_(L"sa %d", nFrameIndex);
	return hr;
}

HRESULT __stdcall CNLEAudioRenderBase::Forward(INT nFrameCount, INT *pRealFrameCount)
{
	NLEFrameIndex retIdx = 0;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);

	HRESULT hr = CNLERenderBase::Forward(nFrameCount, &retIdx);
	if (pRealFrameCount != NULL)
	{
		*pRealFrameCount = retIdx;
	}

	// 	LOGGER_INFO_(L"fa %d, %d", GetOffset(), nFrameCount);

	return hr;
}

HRESULT __stdcall CNLEAudioRenderBase::Prepare(INLEFrame* pGraphics)
{
	HRESULT hr = E_NLE_UNEXPECTED;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);

	RETURN_IF_FALSE(m_bNotEndOfTimeline, S_OK); // 如果已经超出范围，则直接返回

	do
	{
		NLEPropDecorator pTimelineProp = NLECommon::NLEGetProperties(this);
		m_pMixerFrame = this->GetMixerFrame(TRUE); _ASSERT(m_pMixerFrame);
		BREAK_IF_NULL(m_pMixerFrame);

		hr = m_pMixerFrame->Reset(); BREAK_IF_FAILED(hr);

		m_pRenderFrame = NULL;
		hr = this->PrepareInternal(&m_pRenderFrame); CHECK(hr);
		BREAK_IF_FAILED(hr);

		if (GetParent() == NULL) //只有顶层Render才可以更改pGraphics的长度，因此时与Player或Exporter相接。
		{
			hr = this->SetDataSize(pGraphics, m_nNextFrameSize); CHECK(hr);
			BREAK_IF_FAILED(hr);
		}
		_ASSERT(m_pRenderFrame->GetDataSize() == m_nNextFrameSize);
		_ASSERT(m_pMixerFrame->GetDataSize() == m_nNextFrameSize);
		_ASSERT(pGraphics->GetDataSize() == m_nNextFrameSize);

		hr = CNLERenderBase::Prepare(m_pRenderFrame); CHECK(hr);
		BREAK_IF_FAILED(hr);

		hr = WillPostProcess(m_pRenderFrame, m_pMixerFrame, pTimelineProp); CHECK(hr);
		BREAK_IF_FAILED(hr);

		hr = WillCompose(m_pMixerFrame, pGraphics); CHECK(hr);
		BREAK_IF_FAILED(hr);
	} while (FALSE);

	return hr;
}

HRESULT __stdcall CNLEAudioRenderBase::Render(INLEFrame* pGrahics)
{

	HRESULT hr = E_NLE_UNEXPECTED;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);

	do
	{
		BREAK_IF_FALSE(m_bNotEndOfTimeline); // 如果已经超出范围，则直接返回

		_ASSERT(pGrahics->GetDataSize() == m_nNextFrameSize);

		hr = CNLERenderBase::Render(m_pRenderFrame);

		if (S_NLE_NO_RENDER == hr)
		{
			pGrahics->Reset(); break;
		}

		hr = DoPostProcess(m_pRenderFrame, m_pMixerFrame); CHECK(hr);
		BREAK_IF_FAILED(hr);

		hr = DoCompose(m_pMixerFrame, pGrahics); CHECK(hr);
		BREAK_IF_FAILED(hr);
	} while (FALSE);

	if (FAILED(hr))
	{
		pGrahics->Reset();
	}
	// 计算帧的时间戳，
	// 不同的片段，最终出来的时间戳进行连续统一	

	NLElonglong pts = 0, dur = 0;
	NLEFrameIndex frmIndex = GetOffset();
	GetCurrentAudioFrameTimestamp(frmIndex, pts, dur);
	pGrahics->SetTimestamp(pts, dur);
	//LOGGER_INFO_(L"ra %d, %lld", frmOffset, pts);

	NLEComPtr<INLEFrameOperate> pFrameOperator = NULL;
	hr = QUERY_INTERFACE(pGrahics, IID_INLEFrameOperate, &pFrameOperator);
	pFrameOperator->SetMediaFrameNumber(this->GetOffset());

	return hr;
}

HRESULT CNLEAudioRenderBase::PrepareInternal(INLEFrame** ppOutFrame)
{
	_ASSERT(m_nNextFrameSize > 0);
	NLEComPtr<INLEFrameOperate> pOper = NULL;
	HRESULT hr = QUERY_INTERFACE(m_pInternalFrame, IID_INLEFrameOperate, &pOper);
	if (FAILED(hr) || pOper->GetBufferSize() < m_nNextFrameSize)
	{
		m_pInternalFrame = CreateAudioFrame(m_nNextFrameSize);
		if (NULL == m_pInternalFrame)
		{
			LOGGER_ERROR_(L"Failed to create audio frame, size:%d", m_nNextFrameSize);
			_ASSERT(FALSE); return E_OUTOFMEMORY;
		}
	}

	pOper = NULL;
	hr = QUERY_INTERFACE(m_pInternalFrame, IID_INLEFrameOperate, &pOper); _ASSERT(pOper);
	hr = pOper->SetDataSize(m_nNextFrameSize); CHECK(hr);

	hr = QUERY_INTERFACE(m_pInternalFrame, IID_INLEFrame, ppOutFrame); CHECK(hr);

	m_pInternalFrame->Reset();
	return hr;
}

INT CNLEAudioRenderBase::DecideNextFrameRenderSize()
{
	NLEFrameIndex nOffset = GetOffset();
	NLEPropDecorator pProp = GetTimelineProp();
	NLEFramerate  ralFramerate = pProp[kRender_Framerate];
	INT nNextFrameSize = NLECommon::CalcAuidoFrameSize(ralFramerate, pProp->GetRenderBytesPerSecond(), nOffset);
	_ASSERT(nNextFrameSize > 0);
	return nNextFrameSize;
}

NLEComPtr<INLERender> CNLEAudioRenderBase::GetRender(INLETimelineBase *clip)
{
	NLEComPtr<INLERender> pAudioRender = NULL;

	NLE_ASSERT(clip != NULL);

	HRESULT hr = clip->GetRender(&pAudioRender, NULL);
	if (SUCCEEDED(hr))
	{
		return pAudioRender;
	}

	return NULL;
}

NLEComPtr<INLEFrame> CNLEAudioRenderBase::CreateAudioFrame(INT nSize)
{
	NLEComPtr<INLEFactory> pFactory = NULL;
	HRESULT hr = NLECommon::GetInterface(CLSID_NLEFactory, NULL, 0, IID_INLEFactory, (LPVOID *)&pFactory);
	if (SUCCEEDED(hr))
	{
		return pFactory->CreateNLEAudioFrame2(nSize / 4);
	}
	NLE_ASSERT(0);

	return NULL;
}

BOOL CNLEAudioRenderBase::IsAudioRender()
{
	return TRUE;
}

void CNLEAudioRenderBase::UpdateNeedRenderList(NLEFrameIndex nFrameOffset)
{
	m_lstNeedRenders.clear();

	// 倒序{沿Z序负方向}查询所有子层的是否有完全覆盖的情况，以减少不必要的处理
	BOOL seek_uncovered = FALSE;
	NLEFrameIndex nNextChildFrameOffset = FRAMEINDEX_MIN;
	for (auto render = m_lstCurrentRenders.rbegin(); render != m_lstCurrentRenders.rend(); render++)
	{

		// 存在强制转换{被逼的，inlerender不能拿到自己的tl}
		NLEComPtr<INLETimelineBase> pClip = ((CNLERenderBase *)(INLERender *)*render)->GetTimeline();
		NLEPropDecorator propClip = NLEGetProperties(pClip);

		INT bHidden = FALSE;
		if (SUCCEEDED(propClip.GetInt(kRender_Hidden, &bHidden)) && bHidden)
		{
			// 明确指定该片段隐藏，则继续测试下一片断
			continue;
		}
        
        if (SUCCEEDED(propClip.GetInt(kRender_Hidden_Audio, &bHidden)) && bHidden)
        {
            // 明确指定该片段的音频隐藏，则继续测试下一片断
            continue;
        }

		INT audio_enabled = FALSE;
		if (FAILED(propClip.GetInt(kRender_AudioEnable, &audio_enabled)) || !audio_enabled)
		{
			// 音频流禁用
			continue;
		}

		NLERange rngClip = GetActualRenderRange(propClip);
		NLEFrameIndex pos = (CNLEValue)propClip[kRender_Position];

		if (nFrameOffset < pos || nFrameOffset > pos + rngClip.Count() - 1)
		{
			// 还没到该片段的渲染区间，则继续测试下一片断
			continue;
		}

		m_lstNeedRenders.push_back(*render);
	}
	// 需要反序以保证正确的渲染顺序
	std::reverse(m_lstNeedRenders.begin(), m_lstNeedRenders.end());
}

TIMELINE_RENDER_LIST& CNLEAudioRenderBase::GetNeedRenderList()
{
	return m_lstNeedRenders;
}



NLEComPtr<INLEFrame> CNLEAudioRenderBase::GetMixerFrame(BOOL bCreateIfNotExists /*= TRUE*/)
{
	if (FALSE == bCreateIfNotExists)
	{
		return m_pMixerFrame;
	}

	m_nNextFrameSize = DecideNextFrameRenderSize(); _ASSERT(m_nNextFrameSize);
	if (NULL == m_pMixerFrame || m_pMixerFrame->GetDataSize() < m_nNextFrameSize)
	{
		m_pMixerFrame = CreateAudioFrame(m_nNextFrameSize);
	}

	NLEComPtr<INLEFrameOperate> pFrmOperator = NULL;
	QUERY_INTERFACE(m_pMixerFrame, IID_INLEFrameOperate, &pFrmOperator); _ASSERT(pFrmOperator);

	pFrmOperator->SetDataSize(m_nNextFrameSize);

	return m_pMixerFrame;
}

HRESULT CNLEAudioRenderBase::WillCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics)
{
	return S_OK;
}

HRESULT CNLEAudioRenderBase::DoCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics)
{
	NLEComPtr<INLEFrameOperate> pFrmOperator = NULL;
	QUERY_INTERFACE(pGraphics, IID_INLEFrameOperate, &pFrmOperator); _ASSERT(pFrmOperator);
	pFrmOperator->MixAudio(pCurFrame->GetData(), pCurFrame->GetDataSize());

	return S_OK;
}

HRESULT CNLEAudioRenderBase::SetDataSize(NLEComPtr<INLEFrame> pFrame, INT nDataSize, BOOL bReallocatedIfOverflow /*= FALSE*/)
{
	NLEComPtr<INLEFrameOperate> pFrameControl = NULL;
	HRESULT hr = QUERY_INTERFACE(pFrame, IID_INLEFrameOperate, &pFrameControl); CHECK(hr);

	_ASSERT(nDataSize < (INT)pFrameControl->GetBufferSize());

	if (nDataSize >(INT)pFrameControl->GetBufferSize())
	{
		LOGGER_ERROR_(L"Invalid size, DataSize:%d, bufferSize:%d", nDataSize, pFrameControl->GetBufferSize());
		_ASSERT(FALSE);
		if (FALSE == bReallocatedIfOverflow)
		{
			_ASSERT(FALSE); return E_OUTOFMEMORY;
		}
		else
		{
			NLEMediaInfo info;
			pFrame->GetMediaInfo(&info);
			hr = pFrameControl->CreateAudio(info.audio.nSampleRate, info.audio.nChannels, info.audio.wBitsPerSample, nDataSize);
			if (FAILED(hr))
			{
				LOGGER_ERROR_(L"Failed to create audio frame, size:%d", nDataSize); CHECK(hr);
				return hr;
			}
		}
	}
	hr = pFrameControl->SetDataSize(nDataSize); CHECK(hr);
	return hr;
}

scombase::CUnknown* CALLBACK CreateAudioRenderBaseInstance(LPUNKNOWN pUnkOuter, HRESULT *phr)
{
	HRESULT hr = E_OUTOFMEMORY;
	scombase::CUnknown* pNewObj = (scombase::CUnknown*)new CNLEAudioRenderBase(pUnkOuter);
	if (pNewObj)
	{
		pNewObj->NonDelegatingAddRef();
		hr = S_OK;
	}
	if (phr)
	{
		*phr = hr;
	}
	return pNewObj;
}
