#include "stdafx.h"

#include <algorithm>
#include <functional>

#include "NLEMatrix.h"
#include "NLEMatrix.cpp"

#include "NLEVideoRenderBase.h"
#include "INLEFrameControl.h"
#include "INLEFrameOperate.h"
#include "INLETimelineBase.h"
#include "NLEComPtr.h"
#include "NLECommon.h"
#include "NLEMacroDefine.h"

#include "NLEKey.h"
#include "NLEKeyShortcut.h"
#include "INLEFactory.h"

#include "INLEVideoFilter.h"
#include "INLEFrameRateCalculator.h"
#include "NLECommon_Const.h"
#include "NLEComError.h"
#include "NLECostTimeTest.h"

using namespace NLEKey::Transition;
using namespace NLEKey::Motion;
using namespace NLECommon;
using namespace NLECommon::Const;

PROPERTY_LISTEN_BEGIN(CNLEVideoRenderBase)
PROPERTY_LISTEN_END

CNLEVideoRenderBase::CNLEVideoRenderBase(IUnknown *pUnkOuter) :CNLERenderBase(pUnkOuter)
{
	HRESULT hr = E_NLE_UNEXPECTED;
	m_pRegionFrame = NULL;
	m_nClipCount = 0;
	m_pRenderFrame = NULL;
	m_pRegionFrame = NULL;
	m_pInternalFrame = NULL;
	
	PROPERTY_LISTENERS_REGISTER(CNLEVideoRenderBase);

}
 

CNLEVideoRenderBase::~CNLEVideoRenderBase()
{
}

STDOVERRIDEMETHODIMP CNLEVideoRenderBase::NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj)
{
	if (riid == IID_INLEVideoRender)
	{
		return CNLERenderBase::NonDelegatingQueryInterface(IID_INLERender, ppObj);
	}
	return CNLERenderBase::NonDelegatingQueryInterface(riid, ppObj);
}

HRESULT __stdcall  CNLEVideoRenderBase::Seek(NLEFrameIndex nFrameIndex, NLEFrameIndex *pRealFrameIndex/* = NULL*/)
{
    NLEFrameIndex retIdx = 0;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);
    LOGGER_INFO_(L"seek pos:%d", nFrameIndex);

	HRESULT hr = CNLERenderBase::Seek(nFrameIndex, &retIdx);
	if (pRealFrameIndex != NULL)
	{
		*pRealFrameIndex = retIdx;
	}
	NLEComPtr<INLETimelineBase> pTimeline = GetTimeline();
	hr = pTimeline->GetClipCount(&m_nClipCount); CHECK(hr);
// 	LOGGER_INFO_(L"sv %d", nFrameIndex);
	return hr;
}

HRESULT __stdcall CNLEVideoRenderBase::Forward(INT nFrameCount, INT *pRealFrameCount)
{
	NLEFrameIndex retIdx = 0;
	HRESULT hr = S_OK;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);

	//TEST_COST_TIME_BEGIN

	hr = CNLERenderBase::Forward(nFrameCount, &retIdx);
	if (pRealFrameCount != NULL)
	{
		*pRealFrameCount = retIdx;
	}
	
// 	LOGGER_INFO_(L"fv %d, %d", GetOffset(), nFrameCount);

	//TEST_COST_TIME_END

	return hr;
}

HRESULT __stdcall CNLEVideoRenderBase::Prepare(INLEFrame* pGraphics)
{
	// 视频渲染的Prepare，没有调用基类的CNLERenderBase::Prepare(pFrame)
	// 所以，实际上需要应用保证在Prepare之前，至少调用一次Seek/Forward，以保证渲染列表的正确性

	HRESULT hr = E_NLE_UNEXPECTED;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);
	m_pRenderFrame = NULL;

	//TEST_COST_TIME_BEGIN

	RETURN_IF_FALSE(m_bNotEndOfTimeline, S_OK); // 如果已经超出范围，则直接返回

	NLE_ASSERT(pGraphics);
	NLEPropDecorator pTimelineProp = NLECommon::NLEGetProperties(this);

	NLESize curGrahicsSize = FRAME_SIZE(pGraphics);
	m_bGraphicsChanged = !m_oGraphicsSize.Equals(curGrahicsSize);
	m_oGraphicsSize = curGrahicsSize;
	
	m_pRegionFrame = this->GetRegionFrame(TRUE);
	if(NULL == m_pRegionFrame)
	{
		LOGGER_ERROR_(L"Failed to create region frame "); NLE_ASSERT(FALSE); 
		return E_OUTOFMEMORY;
	}
// 	FRAME_IMAGE(m_pRegionFrame)->FillColor(NLECommon::Const::Color::BLACK_TRANSPARENT);

	if (FAILED(hr = this->PrepareInternal(&m_pRenderFrame)))
	{
		LOGGER_ERROR_(L"Failed to prepare intenal"); CHECK(hr); 
		return hr;
	}

	CNLERenderBase::Prepare(m_pRenderFrame);

// 	// 处理子片段的准备情况
// 	INT bConCurrency = pTimelineProp[kRender_PrepareConcurrency]; //是否使用并发
// 	if (m_nClipCount > 0 && FAILED(hr = this->ConCurrencyPrepare(m_pRenderFrame, bConCurrency) ) )
// 	{
// 		LOGGER_ERROR_(L"concurrency prepare prepare failed"); CHECK(hr);
// 		return hr;
// 	}

	if (FAILED(hr = this->WillPostProcess(m_pRenderFrame, m_pRegionFrame, pTimelineProp)))
	{
		LOGGER_ERROR_(L"Failed to will postprocess"); CHECK(hr);
		return hr;
	}

	if (FAILED(hr = this->WillCompose(m_pRegionFrame, pGraphics) ) )
	{
		LOGGER_ERROR_(L"Failed to will compose"); CHECK(hr);
		return hr;
	}

	//TEST_COST_TIME_END

	return hr;
}

HRESULT __stdcall CNLEVideoRenderBase::Render(INLEFrame* pGraphics)
{
	HRESULT hr = E_NLE_UNEXPECTED;

    SimpleSync::CSimpleMutexLock lock(_renderMutex);

	//TEST_COST_TIME_BEGIN
	do
	{
		BREAK_IF_FALSE(m_bNotEndOfTimeline); // 如果已经超出范围，则直接返回
		BREAK_IF_NULL(m_pRenderFrame);
		NLE_ASSERT(m_pRenderFrame);
		NLE_ASSERT(m_pRegionFrame);
		NLE_ASSERT(pGraphics);

		hr = this->RenderInternal(m_pRenderFrame); CHECK(hr);
		BREAK_IF_FAILED(hr);

		// 递归自己的子片段，将自己的子片段的数据渲染到m_pRenderFrame
		hr = CNLERenderBase::Render(m_pRenderFrame);
		BREAK_IF_FAILED(hr);

		//保留透明度
		NLEComPtr<INLEImage> pSrcImage = m_pRenderFrame->GetImage();		
		if (pSrcImage && !pSrcImage->IsOpaque())
		{
			NLEComPtr<INLEFrameOperate> pOp = NULL;
			QUERY_INTERFACE(m_pRegionFrame, IID_INLEFrameOperate, (void**)&pOp); NLE_ASSERT(pOp);
			pOp->SetOpaque(FALSE);
		}

		hr = this->DoPostProcess(m_pRenderFrame, m_pRegionFrame); CHECK(hr);
		BREAK_IF_FAILED(hr);

		hr = this->DoCompose(m_pRegionFrame, pGraphics); CHECK(hr);
		BREAK_IF_FAILED(hr);
	} while (FALSE);

	// 计算帧的时间戳， 不同的片段，最终出来的时间戳进行连续统一
	LONGLONG pts = 0, dur = 0;
	GetCurrentVideoFrameTimestamp(pGraphics, pts, dur);
	pGraphics->SetTimestamp(pts, dur);

	NLEComPtr<INLEFrameOperate> pFrameOperator = NULL;
	if (SUCCEEDED(QUERY_INTERFACE(pGraphics, IID_INLEFrameOperate, &pFrameOperator)))
		pFrameOperator->SetMediaFrameNumber(this->GetOffset());

	m_pRenderFrame = NULL;
    if ( m_pPostRequest )
    {
        m_pPostRequest->Reset();
    }
	
	//TEST_COST_TIME_END
	return hr;
}

HRESULT __stdcall CNLEVideoRenderBase::Cleanup()
{	
	HRESULT hr =  CNLERenderBase::Cleanup();
	m_pImageProc = NULL;
	m_pRegionFrame = NULL;
//     m_pRenderFrame = NULL;
//     m_pInternalFrame = NULL;
	m_oTransitioner.Reset();

	return hr;
}

HRESULT CNLEVideoRenderBase::WillCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics)
{
	m_oTransitioner.SetRender(this);
	m_oTransitioner.SetFrom(pGraphics);
	m_oTransitioner.SetTo(pCurFrame);
	m_oTransitioner.SetOut(pGraphics);
	m_oTransitioner.CheckTransition();
	if(m_oTransitioner.Enable() )
	{
		return S_OK;
	}
	m_oTransitioner.Reset();

	if (NULL == GetImageProc())
	{
		LOGGER_ERROR_(L"Failed to get image proc"); NLE_ASSERT(FALSE);
		return E_NLE_UNEXPECTED;
	}

	return S_OK;
}

HRESULT CNLEVideoRenderBase::DoCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics)
{
	if (m_oTransitioner.Enable())
	{
		return	m_oTransitioner.DoTransition();
	}

	NLE_ASSERT(GetImageProc());

	NLEComPtr<INLEImage> pCurrentImage = pCurFrame->GetImage();
	NLEComPtr<INLEImage> pGraphicsImage = pGraphics->GetImage();
	if (pCurrentImage->IsOpaque() )
	{
		GetImageProc()->BitBlt(pGraphicsImage, pCurrentImage,
			m_rcRegion.X,
			m_rcRegion.Y,
			m_rcRegion.Width,
			m_rcRegion.Height);		
	}
	else
	{
		GetImageProc()->AlphaComp(pGraphicsImage,
			pCurrentImage,
			m_rcRegion.X,
			m_rcRegion.Y,
			m_rcRegion.Width,
			m_rcRegion.Height);
	}
	return S_OK;
}

HRESULT CNLEVideoRenderBase::PrepareInternal(INLEFrame** ppOutFrame)
{	
	if (PropChanged() )
	{
		NLEPropDecorator pProp = NLECommon::NLEGetProperties(this);
		NLERectF rcRender = pProp[kRender_Region];

		rcRender.Scale((NLEREAL)m_oGraphicsSize.Width, (NLEREAL)m_oGraphicsSize.Height);
		NLERect rcRegion;
		rcRender.GetRect(&rcRegion);
        rcRegion.Width = FRAME_ALIGN(rcRegion.Width, 2);
		if (NULL == m_pInternalFrame ||
			!FRAME_SIZE(m_pInternalFrame).Equals(NLESize(rcRegion.Width, rcRegion.Height)))
		{
			m_pInternalFrame = CreateVideoFrame(rcRegion.Width, rcRegion.Height); NLE_ASSERT(m_pInternalFrame);
			if (m_pInternalFrame == NULL)
			{
				LOGGER_ERROR_(L"Failed to create video frame, size:(%d, %d)", rcRegion.Width, rcRegion.Height);
				return E_OUTOFMEMORY;
			}
		}
	}
	NLE_ASSERT(m_pInternalFrame);

    NLEPropDecorator pProp = NLECommon::NLEGetProperties(this);
    CNLEValue value;
    HRESULT hr = pProp->GetValue(kRender_BackgroundColor, value);
    
    NLEColor color = Color::BLACK_OPAQUE;
    if ( SUCCEEDED(hr) )
    {
        color = value;
    }

	m_pInternalFrame->QueryInterface(IID_INLEFrame, (LPVOID*)ppOutFrame);
    (*ppOutFrame)->GetImage()->FillColor(color);

 	return S_OK;
}

HRESULT CNLEVideoRenderBase::RenderInternal(INLEFrame* ppOutFrame)
{
	return S_OK;
}

NLEComPtr<INLEFrame> CNLEVideoRenderBase::CreateVideoFrame(int nWidth, int nHeight)
{
	NLEComPtr<INLEFactory> pFactory = NULL;
	HRESULT hr = NLECommon::GetInterface(CLSID_NLEFactory, NULL, 0, IID_INLEFactory, (LPVOID *)&pFactory);
	if (SUCCEEDED(hr) )
	{
		return pFactory->CreateNLEVideoFrame(nWidth, nHeight);
	}
	NLE_ASSERT(0);

	return NULL;
}

NLEComPtr<INLEImageProc> &CNLEVideoRenderBase::GetImageProc()
{
	HRESULT hr = E_OUTOFMEMORY;
	if (m_pImageProc == NULL)
	{
		if (FAILED(hr = NLECommon::GetInterface(CLSID_NLEImageProc, NULL, 0, IID_INLEImageProc, (LPVOID*)&m_pImageProc)))
		{
			LOGGER_ERROR_(L"Failed to create imageproc instance "); NLE_ASSERT(FALSE);			
		}
	}
	return m_pImageProc;
}

NLEComPtr<INLERender> CNLEVideoRenderBase::GetRender(INLETimelineBase *clip)
{
	NLEComPtr<INLERender> pVideoRender = NULL;

	NLE_ASSERT(clip != NULL);

	HRESULT hr = clip->GetRender(NULL, &pVideoRender);
	
	return pVideoRender;
}

BOOL CNLEVideoRenderBase::IsAudioRender()
{
	return FALSE;
}

void CNLEVideoRenderBase::UpdateNeedRenderList(NLEFrameIndex nFrameOffset)
{
	// 检查遮盖的情况

	m_lstNeedRenders.clear();

	// 倒序{沿Z序负方向}查询所有子层的是否有完全覆盖的情况，以减少不必要的处理
	BOOL seek_uncovered = FALSE;
	NLEFrameIndex nNextChildFrameOffset = FRAMEINDEX_MIN;
    for (auto render = m_lstCurrentRenders.rbegin(); render != m_lstCurrentRenders.rend(); render++) {
        NLEPropDecorator propClip = NLEGetProperties(render);

        { // 判断子片段是否需要渲染
            int bHidden = 0;
            propClip.GetInt(kRender_Hidden, &bHidden);
            if (bHidden) {
                continue;
            }

            int bVideoHidden = 0;
            propClip.GetInt(kRender_Hidden_Video, &bVideoHidden);
            if (bVideoHidden) {
                continue;
            }

            int video_enabled = 0;
            propClip.GetInt(kRender_VideoEnable, &video_enabled);
            if (!video_enabled) {
                // 视频流禁用
                continue;
            }
        };

        NLERational framerate = (CNLEValue)propClip[NLEKey::Render::kRender_Framerate];
        NLERange rngClip = GetActualRenderRange(propClip);
        NLEFrameIndex pos = (CNLEValue)propClip[kRender_Position];

        if (nFrameOffset < pos || nFrameOffset >= pos + rngClip.Count()) {
            // 不在该片段的渲染区间，则继续测试下一片断
            continue;
        }

        if (seek_uncovered) {
            // 将需要定位的，记录到列表里面，待后续一次性处理

            auto found = std::find_if(m_lstSeekRenders.begin(), m_lstSeekRenders.end(), [&](std::pair<INLERenderPtr, NLEFrameIndex> const &v) {
                return v.first == *render;
            });

            NLEFrameIndex childOffset = (*render)->GetOffset();
            if (nNextChildFrameOffset > childOffset
                && nNextChildFrameOffset < pos + rngClip.Count()) {
                NLEFrameIndex seek_pos = nNextChildFrameOffset - pos;

                if (found == m_lstSeekRenders.end()) {
                    m_lstSeekRenders.push_back(std::make_pair(*render, seek_pos));
                } else {
                    found->second = seek_pos;
                }
            } else if (found != m_lstSeekRenders.end()) {
                m_lstSeekRenders.erase(found);
            }
            continue;
        }

        (*render)->SendCommand(RENDER_CMD_ENTER_RENDERING, nFrameOffset);

        m_lstNeedRenders.push_back(*render);
        if (m_lstCurrentRenders.size() <= 1) {
            break;
        }

        { // 判断子片段是否覆盖其他的层

            int bNoCover = 0;
            propClip.GetInt(kRender_NoCover, &bNoCover);
            if (bNoCover) {
                continue;
            }

            int bFullCover = 0;
            propClip.GetInt(kRender_FullCover, &bFullCover);
            if (!bFullCover) {

                NLERectF rcRegion = propClip[kRender_Region];
                BOOL region_cover = (abs(rcRegion.X) < 0.001 && abs(rcRegion.Y) < 0.001)
                    && (abs(rcRegion.Width - 1.0) < 0.001 && abs(rcRegion.Height - 1.0) < 0.001);
                if (!region_cover) {
                    continue;
                }

                int bMotionEnabled = 0;
                propClip.GetInt(NLEKey::Motion::kMotion_Enable, &bMotionEnabled);
                if (bMotionEnabled) {
                    continue;
                }

                WCHAR szMaskFile[256] = L"";
                propClip.GetString(NLEKey::Render::kRender_MaskFile, szMaskFile, 256);
                if (wcslen(szMaskFile) > 0) {
                    continue;
                }

                int bTranstition = 0;
                propClip.GetInt(NLEKey::Transition::kTransition_Enable, &bTranstition);
                if (bTranstition) {
                    continue;
                }
            }

            nNextChildFrameOffset = pos + rngClip.Count();
            if ((nNextChildFrameOffset - nFrameOffset) < (int)(m_nCoveredSeekMSec * framerate.Value() / 1000)) {
                continue;
            }

            // 已经覆盖
            seek_uncovered = TRUE; // 从下一轮开始，只进行定位操作
        };
    }

	// 需要反序以保证正确的渲染顺序
	std::reverse(m_lstNeedRenders.begin(), m_lstNeedRenders.end());
}

TIMELINE_RENDER_LIST& CNLEVideoRenderBase::GetNeedRenderList()
{
	return m_lstNeedRenders;
}

HRESULT CNLEVideoRenderBase::ConCurrencyPrepare(NLEComPtr<INLEFrame> pGraphics, BOOL bConcurrentcy)
{
	HRESULT hr = S_OK;
	BOOL bPrepareConcurrency = bConcurrentcy;

	if (this->GetNeedRenderList().size() > 1 && bPrepareConcurrency) //并行处理
	{
		if (m_pPrepareTask == NULL) {
			hr = NLECommon::GetInterface(CLSID_NLEPrepareTask, NULL, 0, IID_INLEPrepareTask, (LPVOID*)&m_pPrepareTask);
			RETURN_IF_FAILED(hr, hr);
		}

		hr = m_pPrepareTask->Prepare(this->GetNeedRenderList(), m_pRenderFrame);
	}
	else
	{
		std::vector<NLEComPtr<INLERender> >::iterator iter = this->GetNeedRenderList().begin();
		while (iter != this->GetNeedRenderList().end())
		{
			hr = (*iter)->Prepare(m_pRenderFrame);
			iter++;
		}
	}
	return hr;
}

NLEComPtr<INLEFrame> CNLEVideoRenderBase::GetRegionFrame(BOOL bCreateIfNotExists /*= TRUE*/)
{
	if (FALSE == bCreateIfNotExists)
	{
		return m_pRegionFrame;
	}

	if (PropChanged() || NULL == m_pRegionFrame || m_bGraphicsChanged)
	{
		NLEPropDecorator pProp = NLECommon::NLEGetProperties(this);
		NLERectF rcRender = pProp[kRender_Region];
		NLERect  rcOld = m_rcRegion;

        int rawEnabled = 0;
        pProp.GetInt(NLEKey::Render::kRender_RawEnabled, &rawEnabled);
        if (rawEnabled) { // 裸流
            rcRender = NLERectF(0, 0, 1, 1);
        }

		rcRender.Scale((NLEREAL)m_oGraphicsSize.Width, (NLEREAL)m_oGraphicsSize.Height);
		rcRender.GetRect(&m_rcRegion);

		if (NULL == m_pRegionFrame ||
			!FRAME_SIZE(m_pRegionFrame).Equals(NLESize(m_rcRegion.Width, m_rcRegion.Height)))
		{
			m_pRegionFrame = CreateVideoFrame(m_rcRegion.Width, m_rcRegion.Height); NLE_ASSERT(m_pRegionFrame);			
		}

		if (!rcOld.Equals(m_rcRegion))
		{
			LOGGER_INFO_(L"render region change:%s -->%s ", rcOld.toString().c_str(), m_rcRegion.toString().c_str());
		}
	}
	NLE_ASSERT(m_pRegionFrame);

	return m_pRegionFrame;
}


scombase::CUnknown* CALLBACK CreateVideoRenderBaseInstance(LPUNKNOWN pUnkOuter, HRESULT *phr)
{
	HRESULT hr = E_OUTOFMEMORY;
	scombase::CUnknown* pNewObj = (scombase::CUnknown*)new CNLEVideoRenderBase(pUnkOuter);
	if (pNewObj) {
		pNewObj->NonDelegatingAddRef();
		hr = S_OK;
	}
	if (phr) {
		*phr = hr;
	}
	return pNewObj;
}
