#include "stdafx.h"

#include <map>
#include <algorithm>
#include <functional>

#include "NLERenderBase.h"
#include "NLECommon.h"
#include "NLEMacroDefine.h"

#include "NLEKey.h"
#include "NLEKeyShortcut.h"
#include "NLESyncEvent.h"
#include "NLECommon_Const.h"
#include "NLEComError.h"

using namespace NLECommon;
using namespace NLECommon::Const;

NLEComPtr<INLESeekTask> CNLERenderBase::m_pSeekTask;

PROPERTY_LISTEN_BEGIN(CNLERenderBase)

PROPERTY_LISTEN_END

CNLERenderBase::CNLERenderBase(IUnknown *pUnkOuter)
: scombase::CUnknown(pUnkOuter)
, m_nFrameCount(0)
, m_nOffset(-1)
, m_bNotEndOfTimeline(true)
, m_nAdvanceMSec(5000) // 4K的定位时间比较长，预加载时间可适当加长
, m_nDiscardMSec(3000)
, m_oPrevSession(-1)
, m_bPropChanged(TRUE)
, m_guidSink(IID_IUnknown)
{
	HRESULT hr = E_NLE_UNEXPECTED;
	if (CNLERenderBase::m_pSeekTask == NULL)
	{
		hr = NLECommon::GetInterface(CLSID_NLESeekTask, NULL, 0, IID_INLESeekTask, (LPVOID*)&CNLERenderBase::m_pSeekTask);
		CHECK(hr);
	}

	PROPERTY_LISTENERS_REGISTER(CNLERenderBase);
}


CNLERenderBase::~CNLERenderBase()
{
	m_lstDiscardRenders.clear();
	m_lstCurrentRenders.clear();
	m_lstAdvanceRenders.clear();
	m_lstNeedRenders.clear();
	m_lstSeekRenders.clear();
}

STDOVERRIDEMETHODIMP CNLERenderBase::NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj)
{
	if (IID_INLERender == riid)
	{
		*ppObj = static_cast<INLERender*>(this);
		this->AddRef();
		return S_OK;
	}
	return scombase::CUnknown::NonDelegatingQueryInterface(riid, ppObj);
}


HRESULT __stdcall CNLERenderBase::Prepare(INLEFrame* pFrame)
{
	NLE_ASSERT(pFrame);

	HRESULT hr = E_FAIL;
	int nPrepareCount = 0;

	NLEPropDecorator pProp = NLECommon::NLEGetProperties(this);
	NLERange rngNumber = GetActualRenderRange(pProp);

	NLEFrameIndex nChildFrameOffset = this->GetOffset() + rngNumber.Start;

	hr = this->UpdateRenderList(nChildFrameOffset, m_bNotEndOfTimeline, true);
	RETURN_IF_FAILED(hr, hr);

#ifdef _DEBUG
	auto &needRenderList = this->GetNeedRenderList();
#endif // _DEBUG

	TIMELINE_RENDER_LIST::iterator iter = this->GetNeedRenderList().begin();
	while (iter != this->GetNeedRenderList().end())
	{
		hr = (*iter)->Prepare(pFrame);
		if (SUCCEEDED(hr))
		{
			nPrepareCount++;
		}
		iter++;
	}
	
	// 即，只要不是片段结束，始终算成功
	if (nPrepareCount > 0 || m_bNotEndOfTimeline)
	{
		return S_OK;
	}

	return E_FAIL;
}

HRESULT __stdcall CNLERenderBase::Render(INLEFrame* pFrame)
{
	NLE_ASSERT(pFrame);
	
	HRESULT hr = S_OK;

#ifdef _DEBUG
	auto &needRenderList = this->GetNeedRenderList();
#endif // _DEBUG

	INT nIndex = 0;
	TIMELINE_RENDER_LIST::iterator iter = this->GetNeedRenderList().begin();
	while (iter != this->GetNeedRenderList().end())
	{
		(*iter)->SendCommand(REDNER_CMD_CHILDREN_START, nIndex);
		HRESULT hrClip = E_FAIL;
		if (FAILED(hrClip = (*iter)->Render(pFrame)))
		{
			LOGGER_ERROR_(L"Clip Failed to render !!!");
		}		
		(*iter)->SendCommand(REDNER_CMD_CHILDREN_END, nIndex);

		iter++;
	}

    // 总是下一次渲染生效
    m_bPropChanged = m_oPrevSession != this->GetCurrentSession();
	m_oPrevSession = this->GetCurrentSession();

	return hr;
}

HRESULT __stdcall  CNLERenderBase::Seek(NLEFrameIndex nFrameIndex, NLEFrameIndex *pRealFrameIndex/* = NULL*/)
{
	HRESULT hr = E_FAIL;
	int seek_ok_cnt = 0;

	NLEPropDecorator pProp = NLECommon::NLEGetProperties(this);
    NLERange rngNumber = GetActualRenderRange(pProp);

	NLEFrameIndex nFrameOffset = nFrameIndex;
	NLEFrameIndex nChildFrameOffset = nFrameOffset +rngNumber.Start;
	
	hr = this->MyPostCompInit();

	do
	{
		hr = this->UpdateRenderList(nChildFrameOffset, m_bNotEndOfTimeline, true);
		BREAK_IF_FAILED(hr);

#ifdef _DEBUG
		auto &needRenderList = this->GetNeedRenderList();
#endif // _DEBUG

		TIMELINE_RENDER_LIST::iterator iter = this->GetNeedRenderList().begin();
		while (iter != this->GetNeedRenderList().end())
		{
			NLEFrameIndex nSeekRet = 0;
			NLEPropDecorator pPropClip = NLECommon::NLEGetProperties(*iter);
			NLEFrameIndex pos = pPropClip[kRender_Position];

			NLEFrameIndex currentChildOffset = (*iter)->GetOffset();
			NLEFrameIndex newChildFrameOffset = nChildFrameOffset - pos;
			if (true/*currentChildOffset != newChildFrameOffset*/) // 避免重复定位
			{
				hr = (*iter)->Seek(newChildFrameOffset, &nSeekRet);  CHECK(hr);
				//NLE_ASSERT(newChildFrameOffset == nSeekRet);
				if (SUCCEEDED(hr))
				{
					seek_ok_cnt++;
				}
			}
			else
			{
				seek_ok_cnt++;
			}

			iter++;
		}
	} while (false);
		
	if (seek_ok_cnt > 0 || m_bNotEndOfTimeline)
	{
        m_nOffset = nFrameOffset;		
		if (pRealFrameIndex)
		{
			*pRealFrameIndex = nFrameOffset;
		}
		return S_OK;
	}

	return E_FAIL;
}

HRESULT __stdcall CNLERenderBase::Forward(INT nFrameCount/* = 1*/, INT *pRealFrameCount/* = NULL*/)
{
	HRESULT hr = E_FAIL;
	int forward_ok_cnt = 0;

	NLEPropDecorator pProp = NLECommon::NLEGetProperties(this);
    NLERange rngNumber = GetActualRenderRange(pProp);

	int nFrameOffset = m_nOffset + nFrameCount;
	NLEFrameIndex nChildFrameOffset = nFrameOffset +rngNumber.Start;

	do 
	{
		hr = this->UpdateRenderList(nChildFrameOffset, m_bNotEndOfTimeline);
		BREAK_IF_FAILED(hr);

#ifdef _DEBUG
		auto &needRenderList = this->GetNeedRenderList();
#endif // _DEBUG

		TIMELINE_RENDER_LIST::iterator iter = this->GetNeedRenderList().begin();
		while (iter != this->GetNeedRenderList().end())
		{
			NLEFrameIndex nSeekRet = 0;
			NLEPropDecorator pPropClip = NLECommon::NLEGetProperties(*iter);
			NLEFrameIndex pos = pPropClip[kRender_Position];

			NLEFrameIndex currentChildOffset = (*iter)->GetOffset();
			NLEFrameIndex newChildFrameOffset = nChildFrameOffset - pos;
			if (currentChildOffset != newChildFrameOffset) // 避免重复定位。更重要的是解决衔接处丢一帧的问题
			{
				hr = (*iter)->Forward(nFrameCount, pRealFrameCount);
				if (SUCCEEDED(hr))
				{
					forward_ok_cnt++;
				}
			}
			else
			{
				forward_ok_cnt++;
			}

			iter++;
		}
	} while (false);

	// 即，只要不是片段结束，始终算成功
	if (forward_ok_cnt > 0 || m_bNotEndOfTimeline)
	{
		m_nOffset = nFrameOffset;
		if (pRealFrameCount)
		{
			*pRealFrameCount = nFrameCount;
		}
		return S_OK;
	}

	return E_FAIL;
}

int __stdcall CNLERenderBase::GetOffset(void)
{
	return m_nOffset;
}

static HRESULT ClearRenderList(TIMELINE_RENDER_LIST &renderList)
{
	for (TIMELINE_RENDER_LIST::iterator it = renderList.begin();
		it != renderList.end(); ++it)
	{
		INLERenderPtr pRender = (*it);

		CONTINUE_IF_NULL(pRender);

		pRender->Cleanup();
	}

	renderList.clear();

	return S_OK;
}

HRESULT __stdcall CNLERenderBase::Cleanup()
{
	HRESULT hr = S_OK;
	do
	{			
		ClearRenderList(m_lstDiscardRenders);
		ClearRenderList(m_lstCurrentRenders);
		ClearRenderList(m_lstAdvanceRenders);

		m_lstNeedRenders.clear();
		m_lstSeekRenders.clear();

	} while (false);

	if (m_pPostProcess)
	{
		m_pPostProcess->Cleanup();
		m_pPostProcess = NULL;
	}

	m_pPostRequest = NULL;

	m_nOffset = NLECommon::Const::FRAMEINDEX_MIN - 1;
	return S_OK;
}

HRESULT __stdcall CNLERenderBase::SendCommand(UINT uCmd, NLEWParam wParam /*= NULL*/, NLELParam lParam /*= NULL*/)
{
	if (uCmd == RENDER_CMD_SINK_GUID)
	{
		const GUID* pGUID = (const GUID*)wParam;
		const WCHAR* pwszDescrip = (const WCHAR *)lParam;

		NLE_ASSERT(pGUID);
		m_guidSink = *pGUID;

		return S_OK;
	}
	return E_NOTIMPL;
}

NLEComPtr<INLETimelineBase> CNLERenderBase::GetTimeline()
{
	NLEComPtr<INLETimelineBase> pTimeline;
	this->QueryInterface(IID_INLETimelineBase, (LPVOID*)&pTimeline);
	return pTimeline; 
}


NLEComPtr<INLEService> CNLERenderBase::GetService()
{
	NLEComPtr<INLEService> pService;
	this->QueryInterface(IID_INLEService, (LPVOID*)&pService);
	return pService;
}

NLEPropDecorator CNLERenderBase::GetTimelineProp()
{
	NLEComPtr<INLETimelineBase> pTimeline = GetTimeline();
	NLEPropDecorator pRenderProp = NLECommon::NLEGetProperties(pTimeline);
	return pRenderProp;
}

bool &CNLERenderBase::IsDirty()
{
	return m_bIsDirty;
}


BOOL CNLERenderBase::PropChanged()
{
    return GetCurrentSession() != m_oPrevSession || m_bPropChanged;
}

NLESession CNLERenderBase::GetCurrentSession()
{
	NLEComPtr<INLEService> pSvr = GetService();
	if (NULL == pSvr)
	{
		NLE_ASSERT(FALSE); return -1;
	}
	return pSvr->GetSession();
}

BOOL CNLERenderBase::IsChildClipRemoved(NLEComPtr<INLETimelineBase> pChildClip)
{
	HRESULT hr = S_OK;

	BOOL is_removed = FALSE;

	NLEComPtr<INLETimelineBase> pTimeline = GetTimeline();

	// 检查子片段是否还有效
	NLEComPtr<INLEIterator> iter;
	pTimeline->GetIterator(&iter);
	NLEComPtr<IUnknown> iterUnk = NULL;
	if (SUCCEEDED(iter->MoveToBegin()))
	{
		iterUnk = iter->Current();
		while (iterUnk != NULL)
		{
			NLEComPtr<INLETimelineBase> iterClip;
			iterUnk.QueryInterface(IID_INLETimelineBase, (void **)&iterClip);

			BREAK_IF_FALSE(iterClip != pChildClip);

			iterUnk = NULL;
			BREAK_IF_FAILED(iter->MoveToNext());
			iterUnk = iter->Current();
		}
	}
	if (iterUnk == NULL)
	{
		// 找不到子片段了，即已经被移除了
		is_removed = TRUE;
	}

	return is_removed;
}

HRESULT CNLERenderBase::RefreshEventPoint(NLEFrameIndex refresh_pos)
{
	HRESULT hr = E_FAIL;

	// 刷新事件点
	m_lstEvtPoint.clear();

	NLEComPtr<INLETimelineBase> pTimeline = GetTimeline();
	NLEPropDecorator pProp = NLEGetProperties(pTimeline);

    NLERange rdrRange = GetActualRenderRange(pProp);

	std::vector<GUID> vecClips;
	hr = pTimeline->GetClips(RANGE_INFINITY, vecClips);
	RETURN_IF_FAILED(hr, hr);

	std::for_each(vecClips.begin(), vecClips.end(), [&](GUID &guid){
		do
		{
			NLEComPtr<INLETimelineBase> pClip = NULL;
			BREAK_IF_FAILED(pTimeline->GetClip(guid, &pClip));

			NLEPropDecorator propClip = NLEGetProperties(pClip);
			NLEFrameIndex pos = (CNLEValue)propClip[kRender_Position];
            NLERange rngClip = GetActualRenderRange(propClip);

			NLERational framerate = propClip[kRender_Framerate];

			NLEFrameIndex adding_pos = pos;
			NLEFrameIndex remove_pos = pos + rngClip.Count();

			// 片段前后的衔接点
			m_lstEvtPoint.push_back(adding_pos);
			m_lstEvtPoint.push_back(remove_pos);

			// 预加载点
			int advFrameGap = (int)(m_nAdvanceMSec * framerate.Value() / 1000);
			NLEFrameIndex adv_adding_pos = std::max<NLEFrameIndex>(pos - advFrameGap, 0);
			m_lstEvtPoint.push_back(adv_adding_pos);

			// 迟卸载点
			int dlyFrameGap = (int)(m_nDiscardMSec * framerate.Value() / 1000);
			NLEFrameIndex dly_remove_pos = std::min<NLEFrameIndex>(pos + rngClip.Count() + dlyFrameGap, rdrRange.End);
			m_lstEvtPoint.push_back(dly_remove_pos);

		} while (false);
	});

	std::sort(m_lstEvtPoint.begin(), m_lstEvtPoint.end(), std::less<NLEFrameIndex>());
	m_lstEvtPoint.erase(std::unique(m_lstEvtPoint.begin(), m_lstEvtPoint.end()), m_lstEvtPoint.end());

	return S_OK;
}

void CNLERenderBase::ProcessSeekList()
{
	if (m_lstSeekRenders.size() > 0)
	{
		if (CNLERenderBase::m_pSeekTask != NULL && !this->IsAudioRender())
		{
#ifdef _DEBUG
			LOGGER_INFO_(L"m_pSeekTask->Seek(%d), isAudio %d\n", m_lstSeekRenders.size(), this->IsAudioRender());
#endif // _DEBUG
			CNLERenderBase::m_pSeekTask->Seek(m_lstSeekRenders);
		}
		else
		{
			std::for_each(m_lstSeekRenders.begin(), m_lstSeekRenders.end(), [&](std::pair<INLERenderPtr, NLEFrameIndex> &v) {
				NLEFrameIndex seekRet = FRAMEINDEX_MIN;
				NLEFrameIndex childOffset = v.first->GetOffset();
				if (v.second != childOffset) // 避免重复多次定位
				{
					(v.first)->Seek(v.second, &seekRet);
				}
			});
		}

		m_lstSeekRenders.clear();
	}
}

static bool cmp_less(const INLERenderPtr &r1, const INLERenderPtr &r2)
{
	// 存在强制转换{被逼的，inlerender不能拿到自己的tl}
	NLEComPtr<INLETimelineBase> c1 = ((CNLERenderBase *)(INLERender *)r1)->GetTimeline();
	NLEComPtr<INLETimelineBase> c2 = ((CNLERenderBase *)(INLERender *)r2)->GetTimeline();
	NLEPropDecorator prop1 = NLECommon::NLEGetProperties(c1);
	NLEPropDecorator prop2 = NLECommon::NLEGetProperties(c2);
	NLE_ASSERT(prop2 && prop1);

	// 1、按层级	
	INT nLvl1 = (CNLEValue)prop1[kLevel];
	INT nLvl2 = (CNLEValue)prop2[kLevel];
	if (nLvl1 != nLvl2)
	{
		return nLvl1 < nLvl2;
	}

	// 2、按起始位置	
	INT pos1 = (CNLEValue)prop1[kRender_Position];
	INT pos2 = (CNLEValue)prop2[kRender_Position];
	if (pos1 != pos2)
	{
		return (pos1 < pos2);
	}

	//3、实在没办法决一高下了
	return r1 < r2;
}

HRESULT CNLERenderBase::UpdateRenderList(int nFrameOffset, bool &notEndOfTimeline, bool seek_op)
{

	/*
	nFrameOffset =       A                      B                      C               D
	                     |    pos               |                      |               |
	---------|-----------v----v-----------------v----------------------v---------------v---------------
	         |                |                                |                  |              |
	        ～   预加载区间   |          正常渲染区间          |   延迟卸载区间   |   清理区间   ～
	         |                |                                |                  |              |
	---------|--[advFrameGap]-|--------[rngClip.Count()]-------|---[dlyFrameGap]--|--------------|-----

	当 nFrameOffset =
	A - 需要预加载
	B - 正在渲染
	C - 进入延迟卸载阶段
	D - 清理
	*/

	HRESULT hr = S_OK;

	NLE_ASSERT(nFrameOffset >= 0);

	//////////////////////////////////////////////////////////////////////////
	// 准备工作
	NLEComPtr<INLETimelineBase> pTimeline = GetTimeline();
	NLEPropDecorator pProp = NLEGetProperties(pTimeline);

    NLERange rngRender = GetActualRenderRange(pProp);

	notEndOfTimeline = (nFrameOffset >= rngRender.Start && nFrameOffset <= rngRender.End);

	const bool isDirty = this->IsDirty();
	this->IsDirty() = false;

	//////////////////////////////////////////////////////////////////////////
	// 优化处理，当不在事件点的位置，则不需要进去更新

	if (seek_op || isDirty || m_lstEvtPoint.size() == 0)
	{
		// 重新定位或者有片段移除
		// 刷新可疑点
		hr = RefreshEventPoint(nFrameOffset); // CHECK(hr);
		RETURN_IF_FAILED(hr, hr);

		//NLE_ASSERT(m_lstEvtPoint.size() > 0);
	}

	do 
	{
		bool at_event_point = std::find(m_lstEvtPoint.begin(), m_lstEvtPoint.end(), nFrameOffset) != m_lstEvtPoint.end();
		BREAK_IF_FALSE(at_event_point || seek_op || isDirty);

		//////////////////////////////////////////////////////////////////////////
		// 开始对渲染器进行分类处理
		NLERational framerate = pProp[kRender_Framerate];
		int advFrameGap = (int)(m_nAdvanceMSec * framerate.Value() / 1000);
		int dlyFrameGap = (int)(m_nDiscardMSec * framerate.Value() / 1000);

		// 进行列表临时转储
		TIMELINE_RENDER_LIST tmpDlyRenders = m_lstDiscardRenders;
		TIMELINE_RENDER_LIST tmpCurRenders = m_lstCurrentRenders;
		TIMELINE_RENDER_LIST tmpAdvRenders = m_lstAdvanceRenders;
		TIMELINE_RENDER_LIST tmpCheckRenders; // 临时存储需要新加入的渲染器

		m_lstDiscardRenders.clear();
		m_lstCurrentRenders.clear();
		m_lstAdvanceRenders.clear();

		// 遍历，将即将进行渲染的片段{正在渲染的和需要预加载}加入{tmpCheckRenders}
		if (notEndOfTimeline)
		{
			std::vector<GUID> vecClips;
			hr = pTimeline->GetClips(RANGE_INFINITY, vecClips);
			std::for_each(vecClips.begin(), vecClips.end(), [&](GUID &guid){
				do
				{
					NLEComPtr<INLETimelineBase> pClip = NULL;
					BREAK_IF_FAILED(pTimeline->GetClip(guid, &pClip));

					NLEPropDecorator propClip = NLEGetProperties(pClip);
					NLEFrameIndex pos = (CNLEValue)propClip[kRender_Position];
                    NLERange rngClip = GetActualRenderRange(propClip);

					if (((pos <= nFrameOffset) && (pos + rngClip.Count() > nFrameOffset)) // 正在渲染的
						|| (pos > nFrameOffset && pos <= nFrameOffset + advFrameGap) // 预加载的
						)
					{
						INLERenderPtr render = this->GetRender(pClip);
						BREAK_IF_NULL(render);

						// 去重，排除已经在临时转储表中的项目
						auto found_cur = std::find(tmpCurRenders.begin(), tmpCurRenders.end(), render);
						BREAK_IF_FALSE(found_cur == tmpCurRenders.end());
						auto found_dly = std::find(tmpDlyRenders.begin(), tmpDlyRenders.end(), render);
						BREAK_IF_FALSE(found_dly == tmpDlyRenders.end());
						auto found_adv = std::find(tmpAdvRenders.begin(), tmpAdvRenders.end(), render);
						BREAK_IF_FALSE(found_adv == tmpAdvRenders.end());

						tmpCheckRenders.push_back(render);
					}
				} while (false);
			});
		}

		//////////////////////////////////////////////////////////////////////////
		// 对四个临时转储列表里面的片段进行分类
		std::for_each(tmpCheckRenders.begin(), tmpCheckRenders.end(), [&](INLERenderPtr &render){
			this->ClassifyRenderList(nFrameOffset, advFrameGap, dlyFrameGap, isDirty, render, tmpAdvRenders);
		});
		tmpCheckRenders.clear();

		std::for_each(tmpDlyRenders.begin(), tmpDlyRenders.end(), [&](INLERenderPtr &render){
			this->ClassifyRenderList(nFrameOffset, advFrameGap, dlyFrameGap, isDirty, render, tmpAdvRenders);
		});
		tmpDlyRenders.clear();

		std::for_each(tmpCurRenders.begin(), tmpCurRenders.end(), [&](INLERenderPtr &render){
			this->ClassifyRenderList(nFrameOffset, advFrameGap, dlyFrameGap, isDirty, render, tmpAdvRenders);
		});
		tmpCurRenders.clear();

		std::for_each(tmpAdvRenders.begin(), tmpAdvRenders.end(), [&](INLERenderPtr &render){
			this->ClassifyRenderList(nFrameOffset, advFrameGap, dlyFrameGap, isDirty, render, tmpAdvRenders);
		});
		tmpAdvRenders.clear();

		//////////////////////////////////////////////////////////////////////////
		// 对渲染列表进行排序，以保证渲染层次正确
		std::sort(m_lstCurrentRenders.begin(), m_lstCurrentRenders.end(), cmp_less);

	} while (false);

	//////////////////////////////////////////////////////////////////////////
	// 确定实际需要渲染的列表
	this->UpdateNeedRenderList(nFrameOffset);
	this->ProcessSeekList();

	return notEndOfTimeline ? S_OK : E_FAIL;
}

void CNLERenderBase::ClassifyRenderList(NLEFrameIndex nFrameOffset, NLEFrameIndex advFrameGap, NLEFrameIndex dlyFrameGap, bool isDirty,
	INLERenderPtr &render, TIMELINE_RENDER_LIST &tmpAdvRenders)
{

	// 存在强制转换{被逼的，inlerender不能拿到自己的tl}
	NLEComPtr<INLETimelineBase> pClip = ((CNLERenderBase *)(INLERender *)render)->GetTimeline();

	// 检查渲染器所依附的片段是否还有效
	if (isDirty && this->IsChildClipRemoved(pClip))
	{
		render->Cleanup();
		return;
	}

	// 依然有效，则进行处理
	NLEPropDecorator propClip = NLEGetProperties(pClip);
	NLEFrameIndex pos = (CNLEValue)propClip[kRender_Position];
    NLERange rngClip = GetActualRenderRange(propClip);

	if ((pos <= nFrameOffset) && (pos + rngClip.Count() > nFrameOffset))
	{
		// 需要渲染了，则转移到渲染列表
		m_lstCurrentRenders.push_back(render);
	}
	else if (pos > nFrameOffset && pos <= nFrameOffset + advFrameGap)
	{
		// 在预加载的范围内，继续留着
		m_lstAdvanceRenders.push_back(render);

		// 刚进入预加载范围的，定位到开头位置
		if (std::find(tmpAdvRenders.begin(), tmpAdvRenders.end(), render) == tmpAdvRenders.end())
		{
			m_lstSeekRenders.push_back(std::make_pair(render, 0));

#ifdef _DEBUG2
			LOGGER_INFO_(L"m_lstSeekRenders.push_back(%p, 0)\n", render);
#endif // _DEBUG
		}
	}
	else if ((pos + rngClip.Count()) <= nFrameOffset && (pos + rngClip.Count()) > (nFrameOffset - dlyFrameGap))
	{
		// 在后处理区间，则转移到后处理列表
		m_lstDiscardRenders.push_back(render);
	}
	else
	{
		// 需要清理的了，则清理
		render->Cleanup();
		render->SendCommand(RENDER_CMD_LEAVE_RENDERING, nFrameOffset);
	}

}

HRESULT CNLERenderBase::MyPostCompInit()
{
	HRESULT hr = E_FAIL;

	if (NULL == m_pPostRequest)
	{
		//NLEComPtr<IUnknown> pUnk = NULL;
		//hr = this->QueryInterface(IID_IUnknown, (void**)&pUnk); 
		if (FAILED(hr = NLECommon::GetInterface(CLSID_NLEPostRequest, NULL, 0, IID_INLEPostRequest, (LPVOID*)&m_pPostRequest)))
		{
			LOGGER_ERROR_(L"Failed to create instance postrequest"); NLE_ASSERT(FALSE);
			return hr;
		}
	}

	if (NULL == m_pPostProcess)
	{
		BOOL bAudioRdr = this->IsAudioRender();
		const GUID &clsID = bAudioRdr ? CLSID_NLEAudioPostProcess : CLSID_NLEVideoPostProcess;
		if (FAILED(hr = NLECommon::GetInterface(clsID, NULL, 0, IID_INLEPostProcess, (LPVOID*)&m_pPostProcess)))
		{
			LOGGER_ERROR_(L"Failed to create instance postprocess");
			return hr;
		}
		NLEPropDecorator pTimelineProp = NLEGetProperties(this->GetTimeline());
		m_pPostRequest->SetProperties(pTimelineProp);
	}

	return S_OK;
}

HRESULT CNLERenderBase::AttachFilterFrame(NLEComPtr<INLEFrame> pFrame, NLEComPtr<INLEFrame> pAttacher, LPCWSTR frameKey/*=kBagFrame*/ )
{
	HRESULT hr = S_OK;
	NLEPropDecorator  pFrameProp;
	NLEComPtr<IUnknown> pUnkInnerFrame;

	if (pFrame == NULL || pAttacher == NULL)
	{
		return E_INVALIDARG;
	}

	pFrameProp = NLECommon::NLEGetProperties(pFrame);
	hr = QUERY_INTERFACE(pAttacher, IID_IUnknown, &pUnkInnerFrame);
	if (SUCCEEDED(hr))
	{
		hr = pFrameProp->SetInterface(frameKey, pUnkInnerFrame);
	}
	return hr;
}

NLEComPtr<INLEFrame> CNLERenderBase::GetAttachedFilterFrame(NLEComPtr<INLEFrame> pFrame, LPCWSTR frameKey/*=kBagFrame*/)
{
	NLEPropDecorator  pProp = NLECommon::NLEGetProperties(pFrame); NLE_ASSERT(pProp);
	NLEComPtr<IUnknown> pUnk = NULL;
	NLEComPtr<INLEFrame> pFrameAttacher = NULL;
	HRESULT hr = pProp->GetInterface(frameKey, &pUnk);
	if (SUCCEEDED(hr))
	{
		hr = QUERY_INTERFACE(pUnk, IID_INLEFrame, &pFrameAttacher);
	}
	return pFrameAttacher;
}

NLEComPtr<INLERender> CNLERenderBase::GetParent()
{
	NLEComPtr<INLETimelineBase> pTimline = GetTimeline(); NLE_ASSERT(pTimline);
	RETURN_IF_NULL(pTimline, NULL);
	NLEComPtr<INLEService> pService = NULL;
	HRESULT hr = QUERY_INTERFACE(pTimline, IID_INLEService, &pService); CHECK(hr);
	RETURN_IF_FAILED(hr, NULL);
	IUnknown* pUnkParent = pService->GetParent();
	RETURN_IF_NULL(pUnkParent, NULL);
	NLEComPtr<INLETimelineBase> pParent = NULL;
	hr = QUERY_INTERFACE(pUnkParent, IID_INLETimelineBase, &pParent); CHECK(hr);
	NLEComPtr<INLERender> pRender = NULL;
	if (IsAudioRender() )
	{
		hr = pParent->GetRender(&pRender, NULL); 
	}
	else
	{
		hr = pParent->GetRender(NULL, &pRender);
	}
	CHECK(hr);
	return pRender;
}

HRESULT CNLERenderBase::WillPostProcess(INLEFramePtr pFrame, INLEFramePtr pGraphics, NLEPropDecorator pTimelineProp)
{
	HRESULT hr = S_OK;

	hr = this->MyPostCompInit();

	m_pPostRequest->SetCurrentFrame(pFrame);
	m_pPostRequest->SetGraphics(pGraphics);
	m_pPostRequest->SetOffset(GetOffset());
	m_pPostRequest->SetProperties(pTimelineProp);
	hr = m_pPostProcess->WillProcess(m_pPostRequest); CHECK(hr);

	return hr;
}
HRESULT CNLERenderBase::DoPostProcess(INLEFramePtr pCurFrame, INLEFramePtr pGraphics, INT nFlag /*= 0*/)
{
	NLE_ASSERT(pCurFrame);
	NLE_ASSERT(pGraphics);
	HRESULT hr = S_OK;

	NLEPropDecorator pProp = NLECommon::NLEGetProperties(this);

	if (m_pPostProcess)
	{
		NLE_ASSERT(m_pPostRequest->GetCurrentFrame() == pCurFrame);
		NLE_ASSERT(m_pPostRequest->GetGraphics() == pGraphics);
		hr = m_pPostProcess->DoProcess(m_pPostRequest); CHECK(hr);
	}
	return hr;
}


HRESULT CNLERenderBase::WillCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics)
{
	return S_OK;
}

HRESULT CNLERenderBase::DoCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics)
{
	return S_OK;
}

HRESULT CNLERenderBase::DetachFilterFrame(NLEComPtr<INLEFrame> pFrame, LPCWSTR frameKey /*=kBagFrame*/)
{
	HRESULT hr = E_FAIL;
	NLEPropDecorator  pFrameProp;

	if (pFrame == NULL)
	{
		return E_INVALIDARG;
	}

	pFrameProp = NLECommon::NLEGetProperties(pFrame);
	hr = pFrameProp->Delete(frameKey);
	return hr;
}

const GUID & CNLERenderBase::GetSink()
{
	return m_guidSink;
}

NLERange GetActualRenderRange(NLEComPtr<INLEProperties> prop) {
    NLERange renderRng;

    prop->GetRange(NLEKey::Render::kRender_RangFrameNumber, &renderRng);
    if (renderRng.Equals(NLECommon::Const::RANGE_INFINITY))
    {
        int totalFrameCount = 0;
        prop->GetInt(NLEKey::Render::kRender_TotalFrameCount, &totalFrameCount);
//         NLE_ASSERT(totalFrameCount > 0);

        renderRng.End = totalFrameCount - 1;
    }

    return renderRng;
}

