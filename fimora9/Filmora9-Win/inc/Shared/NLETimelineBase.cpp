#include "stdafx.h"

#include <algorithm>

#include "NLECommon.h"

#include "NLETimelineBase.h"
#include "NLEAudioRenderBase.h"
#include "NLEVideoRenderBase.h"

#include "NLEKey.h"
#include "NLEKeyShortcut.h"
#include "NLEService.h"
#include "NLEMacroDefine.h"
#include "NLEEventDefine.h"
#include "INLEFactory.h"
#include "NLEComError.h"
#include "NLECommon_Const.h"

using namespace NLECommon;
using namespace NLEKey::Transition;

static const LPCWSTR pszItemCurrent = L"Iterator.Current";
static const LPCWSTR pszObjectID = L"Iterrator.ID";

PROPERTY_LISTEN_BEGIN(CNLETimelineBase)

PROPERTY_LISTEN_SET_VALUE(NLEKey::Base::kName, OnSetName)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Base::kLevel, OnSetLevel)

//////////////////////////////////////////////////////////////////////////
PROPERTY_LISTEN_SET_VALUE(NLEKey::Source::kSource_SampleRate, OnSetSourceSampleRate)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Source::kSource_BitsPerSample, OnSetSourceBitsPerSample)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Source::kSource_Channels, OnSetSourceChannels)

PROPERTY_LISTEN_SET_VALUE(NLEKey::Source::kSource_Size, OnSetSourceVideoSize)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Source::kSource_TotalFrameCount, OnSetSourceTotalFrameCount)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Source::kSource_Framerate, OnSetSourceFramerate)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Source::kSource_RangeFrameNumber, OnSetSourceRange)
//------------------------------------------------------------------------
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_SampleRate, OnSetRenderSampleRate)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_BitsPerSample, OnSetRenderBitsPerSample)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_Channels, OnSetRenderChannels)

PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_Size, OnSetRenderVideoSize)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_Region, OnSetRenderRegion)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_Position, OnSetRenderPostion)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_TotalFrameCount, OnSetRenderTotalFrameCount)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_RangFrameNumber, OnSetRenderRangeFrameNumber)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Render::kRender_Framerate, OnSetRenderFrameRate)

//////////////////////////////////////////////////////////////////////////
// 动画
PROPERTY_LISTEN_SET_VALUE(NLEKey::Motion::kMotion_URI, OnSetAnimationURI)
//////////////////////////////////////////////////////////////////////////
// 转场
PROPERTY_LISTEN_SET_VALUE(NLEKey::Transition::kTransition_ID, OnSetTransitionID)
PROPERTY_LISTEN_SET_VALUE(NLEKey::Transition::kTransition_RangeTime, OnSetTransitionRangeFrameNumber)


//////////////////////////////////////////////////////////////////////////

PROPERTY_LISTEN_END

/**@brief 遍历每一个Clip，it为INLETimelinePtr*类型 */
#define TIMELINE_FOR_EACH(it) \
for(TIMELINE_LIST::iterator it = m_lstTimeline.begin(); it != m_lstTimeline.end(); ++it)

 HRESULT CNLETimelineBase::OnSetName(LPCWSTR keyName, CNLEValue& pName)
 {
	 std::wstring name;
	 pName.Get(name);
	 return S_OK;
 }

 HRESULT CNLETimelineBase::OnSetLevel(LPCWSTR keyName, CNLEValue& nleLevel)
 {
	 HRESULT hr = S_OK;
	 INT nLevel = nleLevel;
	 
	 NLEComPtr<INLETimelineBase> pParent = GetParentTimeline();
	 //PARENT_ASSERT(pParent);
	 RETURN_IF_NULL(pParent, S_OK);
	 
	 m_pProperties[kLevel] = nleLevel;
     ((CNLETimelineBase *)(INLETimelineBase*)pParent)->resortClip();
     ((CNLETimelineBase *)(INLETimelineBase*)pParent)->SetRenderDirty(true);

	 return hr;
 }

 HRESULT CNLETimelineBase::SetCrop(LPCWSTR keyName, NLERect rect)
 {
	 return S_OK;
 }

 HRESULT CNLETimelineBase::SetAnchorPoint(LPCWSTR keyName, const NLEPoint& pt)
 {
	 return S_OK;
 }

 HRESULT CNLETimelineBase::SetAttacher(LPCWSTR keyName, IUnknown *pUnk)
 {
	 return S_OK;
 }


CNLETimelineBase::CNLETimelineBase(IUnknown *pOuter)
: scombase::CUnknown(pOuter)
, m_pAudioRenderBase(NULL)
, m_pVideoRenderBase(NULL)
{

}

CNLETimelineBase::CNLETimelineBase()
:m_pAudioRenderBase(NULL)
,m_pVideoRenderBase(NULL)
{

}


HRESULT CNLETimelineBase::Init()
{
	CNLEService::Init();

	PROPERTY_LISTENERS_REGISTER(CNLETimelineBase);
		
    m_timelineGuid = NLECommon::CreateGuid(); //clip的唯一标识符
    {
        CNLESingleLock single_lock(&m_mutexlstTimeline);
        m_lstTimeline.clear();
    }

		
    NLESize sizSize = NLECommon::Const::DEFAULT_RENDER_SIZE;
    NLERational ralFramerate = NLERational(25LL, 1LL);
    INT nFrameCount = 0;
    NLERectF rectFRegion = NLERectF(0.0, 0.0, 1.0, 1.0);

    INT nSamplerate = 44100;
    INT nChannels = 2;
    INT nBitsPerSample = 16;

   	m_pProperties[kCLSID] = CLSID_NLETimelineBase;
	m_pProperties[kIID] = IID_INLETimelineBase;

	m_pProperties[kLevel] = NLECommon::Const::TIMELINE_LEVEL_MIN;
	m_pProperties[kRender_Position] = NLECommon::Const::FRAMEINDEX_MIN;

    m_pProperties[kSource_Size] = sizSize;
	m_pProperties[kRender_Size] = sizSize;

    { // 默认总长为0，渲染范围是全部
        int totalFrameCount = 0;
        m_pProperties[kSource_TotalFrameCount] = totalFrameCount;
        m_pProperties[kRender_TotalFrameCount] = totalFrameCount;

        NLERange defaultRenderRange(0, 0);// (NLECommon::Const::RANGE_INFINITY);
        m_pProperties[kSource_RangeFrameNumber] = defaultRenderRange;
        m_pProperties[kRender_RangFrameNumber] = defaultRenderRange;
    };
    
    m_pProperties[kSource_SampleRate] = nSamplerate;
    m_pProperties[kSource_Channels] = nChannels;
    m_pProperties[kSource_BitsPerSample] = nBitsPerSample;
    int flag = 1;
    m_pProperties[kSource_HasAudioStream] = flag;
    m_pProperties[kSource_HasVideoStream] = flag;

	int fileType = (int)NLE_FILE_TYPE_UNKNOWN;
	m_pProperties[kSource_FileType] = fileType;

	m_pProperties[kSource_Framerate] = ralFramerate;
    m_pProperties[kRender_Framerate] = ralFramerate;
  
	m_pProperties[kRender_SampleRate] = nSamplerate;
	m_pProperties[kRender_Channels] = nChannels;
	m_pProperties[kRender_BitsPerSample] = nBitsPerSample;

    NLERational rational(1, 1);
	m_pProperties[kRender_Scale] = rational;

	double fRotationAngle = 0;
	m_pProperties[kRender_RotationAngle] = fRotationAngle;
	m_pProperties[kRender_AnchorPointF] = NLECommon::Const::POINTF_ORIGNAL;
	m_pProperties[kRender_ResizeScale] = NLECommon::Const::NO_RESIZE_SCALE;


	INT nTransEnable = 0;
	m_pProperties[kTransition_Enable] = nTransEnable;
	NLERange rngTrans(0, 3000);	// 3000毫秒
	m_pProperties[kTransition_RangeTime] = rngTrans;

	INT nConcurrency = 1;
	m_pProperties[kRender_PrepareConcurrency] = nConcurrency;
		
	m_pProperties[kSource_Region] = NLECommon::Const::FULL_GRAPHICS_RENDER;
	m_pProperties[kRender_Region] = NLECommon::Const::FULL_GRAPHICS_RENDER;

	INT nBackplayEnable = 0;
	m_pProperties[kRender_BackplayEnable] = nBackplayEnable;

	INT nAnimationEnable = 0;
	m_pProperties[NLEKey::Motion::kMotion_Enable] = nAnimationEnable;
	LPWSTR url = L"";
	m_pProperties[NLEKey::Motion::kMotion_URI] = url;
	NLERange rngMotion(0, 3500); // 3500毫秒
	m_pProperties[NLEKey::Motion::kMotion_RangeTime] = rngMotion;

	INT bEnableVideoStabilizer = 0;
	m_pProperties[NLEKey::Render::kRender_VideoStabilizerEnable] = bEnableVideoStabilizer;
	
	INT nRenderMode = RenderMode_Fill;
	m_pProperties[NLEKey::Render::kRender_Mode] = nRenderMode;

	INT bEnableAudioStream = 1;
	m_pProperties[NLEKey::Render::kRender_AudioEnable] = bEnableAudioStream;
	INT bEnableVideoStream = 1;
	m_pProperties[NLEKey::Render::kRender_VideoEnable] = bEnableVideoStream;

    m_pProperties[NLEKey::Render::kRender_BackgroundColor] = NLECommon::Const::Color::BLACK_OPAQUE;

	return S_OK;
}


CNLETimelineBase::~CNLETimelineBase()
{
	INT nCount = 0;
    GetClipCount(&nCount);
    {
        CNLESingleLock single_lock(&m_mutexlstTimeline);
        m_lstTimeline.erase(m_lstTimeline.begin(), m_lstTimeline.end());
        m_lstTimeline.clear();
    }

	if (m_pAudioRenderBase)
	{
		m_pAudioRenderBase->NonDelegatingRelease();
		m_pAudioRenderBase = NULL;
	}
	if (m_pVideoRenderBase)
	{
		m_pVideoRenderBase->NonDelegatingRelease();
		m_pVideoRenderBase = NULL;
	}
}

STDOVERRIDEMETHODIMP CNLETimelineBase::NonDelegatingQueryInterface(REFIID riid, LPVOID *ppobj)
{
	HRESULT hr = E_FAIL;
	if (IID_INLETimelineBase == riid)
	{		
		_ASSERT(m_pProperties);
		*ppobj = static_cast<INLETimelineBase *>(this);
		this->AddRef();
		return S_OK;
	}

    if (IID_INLEClonable == riid)
    {
        _ASSERT(m_pProperties);
        *ppobj = static_cast<INLEClonable *>(this);
        this->AddRef();
        return S_OK;
    }
	

	if (IID_INLETimelineInfo == riid)
	{
		_ASSERT(m_pProperties);
		*ppobj = static_cast<INLETimelineInfo *>(this);
		this->AddRef();
		return S_OK;
	}

	if (IID_INLEIterator == riid)
	{
		*ppobj = static_cast<INLEIterator *>(this);
		this->AddRef();
		return S_OK;
	}
	
	
	if (SUCCEEDED(CNLEService::NonDelegatingQueryInterface(riid, ppobj)))
	{
		return S_OK;
	}
	
	if (SUCCEEDED(scombase::CUnknown::NonDelegatingQueryInterface(riid, ppobj)))
	{
		return S_OK;
	}

	if (IID_INLEVideoRender == riid)
	{
		return GetRender(NULL, (INLERender **)ppobj);
	}

	if (IID_INLEAudioRender == riid)
	{
		return GetRender((INLERender **)ppobj, NULL);
	}
	return E_NOINTERFACE;
}

HRESULT __stdcall CNLETimelineBase::AddClip(INLETimelineBase *pTimeline)
{
	_ASSERT(pTimeline);
	RETURN_IF_NULL(pTimeline, E_INVALIDARG);
	HRESULT hr = E_FAIL;

	if (FAILED(hr = this->AppendTimeline(pTimeline) ) )
	{		
		LOGGER_ERROR_(L"Invalid clip, failed to append "); NLE_ASSERT(FALSE);
		return hr;
	}
	
	hr = SyncRenderProp(pTimeline);

	NLEComPtr<INLETimelineInfo> pClipInfo = TIMELINE_INFO_INTERFACE(pTimeline);
	pClipInfo->UpdateRenderInfo();

	Update();
	
	UpdateRenderInfo();
	return hr;
}


//同步Clip的Render参数
HRESULT CNLETimelineBase::SyncRenderProp(INLETimelineBase *pClip)
{
	HRESULT hr = E_FAIL;	
	NLERenderPropComPtr pThis = _P(this);
	NLERenderPropComPtr pThat = _P(pClip);
	
	NLERational oFramerate;
	hr = pThis->GetRenderFrameRate(&oFramerate); CHECK(hr);
	hr = pThat->SetRenderFrameRate(oFramerate); CHECK(hr);
		
	INT nSamplerate;
	hr = pThis->GetRenderSampleRate(&nSamplerate); CHECK(hr);
	hr = pThat->SetRenderSampleRate(nSamplerate); CHECK(hr);
	
	NLESize sizRender;
	hr = pThis->GetRenderSize(&sizRender); CHECK(hr);
	hr = pThat->SetRenderSize(sizRender); CHECK(hr);

	INT nBitsPerSample;
	hr = pThis->GetRenderBitsPerSample(&nBitsPerSample); CHECK(hr);
	hr = pThat->SetRenderBitsPerSample(nBitsPerSample); CHECK(hr);
	INT nChannels;
	hr = pThis->GetRenderChannels(&nChannels); CHECK(hr);
	hr = pThat->SetRenderChannels(nChannels); CHECK(hr);

	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::GetClipCount(INT*pCount)
{
	_ASSERT(pCount);
	if (pCount != NULL)
	{
        CNLESingleLock single_lock(&m_mutexlstTimeline);
		*pCount = (UINT)m_lstTimeline.size();
		return S_OK;
	}
	return E_INVALIDARG;
}

HRESULT __stdcall CNLETimelineBase::MoveClip(UINT iIndexFrom, UINT iIndexTo /*= -1*/)
{
	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::RemoveClip(INLETimelineBase* pClip)
{
	
	HRESULT hr = E_INVALIDARG;
    CNLESingleLock single_lock(&m_mutexlstTimeline);
	TIMELINE_LIST::iterator itClip = std::find(m_lstTimeline.begin(), m_lstTimeline.end(), pClip);
	if (itClip != m_lstTimeline.end() )
	{
        m_lstTimeline.erase(itClip);
        
        NLEComPtr<INLEService> pClipSvr;
        hr = pClip->QueryInterface(IID_INLEService, (LPVOID *)&pClipSvr); CHECK(hr);
        pClipSvr->SetParent(NULL);

		this->SetRenderDirty(true);
	    this->Update();

		hr = S_OK;
	}

	return hr;
}

HRESULT __stdcall CNLETimelineBase::RemoveAll()
{
	HRESULT hr = E_NLE_UNEXPECTED;
    CNLESingleLock single_lock(&m_mutexlstTimeline);

	// 1、搜索自己的枝叶
	m_lstTimeline.erase(m_lstTimeline.begin(), m_lstTimeline.end());
	m_lstTimeline.clear();
	this->SetRenderDirty(true);

	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::GetClip(GUID guid, INLETimelineBase **ppTimeline)
{
	HRESULT hr = E_NLE_UNEXPECTED;
	GUID tempGuid = { 0 };
	// guid不允许为空
	_ASSERT(ppTimeline
		&& !NLECommon::IsEqualGUID(&tempGuid, &guid));
    CNLESingleLock single_lock(&m_mutexlstTimeline);
	// 1、仅搜索自己的枝叶
	TIMELINE_LIST::iterator iter = m_lstTimeline.begin();
	while (iter != m_lstTimeline.end())
	{
		hr = (*iter)->GetGUID(&tempGuid); CHECK(hr);		

		if (SUCCEEDED(hr) && NLECommon::IsEqualGUID(&tempGuid, &guid))
		{
			*ppTimeline = *iter;
			SAFE_ADDREF(*ppTimeline);
			return S_OK;
		}

		iter++;
	}

    return E_NLE_UNEXPECTED;
}

NLEComPtr<INLETimelineBase> CNLETimelineBase::GetClip(INT nIndex)
{
	INT nClipCount;
	GetClipCount(&nClipCount);
	
	if (nIndex < 0 || nIndex >= (INT)nClipCount)
	{
		_ASSERT(FALSE);
		return NULL;
	}
	TIMELINE_LIST::iterator it = m_lstTimeline.begin();
	std::advance(it, nIndex);

	return *it;
}

HRESULT __stdcall CNLETimelineBase::GetClips(NLERange &rng, std::vector<GUID>& vecClips)
{
	HRESULT hr = E_NLE_UNEXPECTED;
	vecClips.clear();

	if (rng.Start > rng.End)
	{
		return E_INVALIDARG;
	}

	// 1、仅搜索自己的枝叶	
    CNLESingleLock single_lock(&m_mutexlstTimeline);
	TIMELINE_FOR_EACH(ppClip)
	{
		NLEPropDecorator propClip = NLEGetProperties(*ppClip);
		
		INT nPosClip = (CNLEValue)propClip[kRender_Position];
		NLERange rngClip = GetActualRenderRange(propClip);

		/*
		A = nPosClip
		B = nPosClip +rngClip.Count() -1
		a = rng.Start
		b = rng.End

		第一种情况：
		+---A----------a-----------------b---------------B-------+
		~   |          |+++++++++++++++++|               |       ~
		+---|----------|-----------------|---------------|-------+

		第二种情况：
		+---a----------A-----------------b---------------B-------+
		~   | ++++++++++++++++++++++++++ |               |       ~
		+---|----------|-----------------|---------------|-------+

		第三种情况：
		+---A----------a-----------------B---------------b-------+
		~   |          | +++++++++++++++++++++++++++++++ |       ~
		+---|----------|-----------------|---------------|-------+

		第四种情况：
		+---a----------A-----------------B---------------b-------+
		~   | ++++++++++++++++++++++++++++++++++++++++++ |       ~
		+---|----------|-----------------|---------------|-------+
		
		*/

		if (rng.Start <= nPosClip +rngClip.Count() -1 && rng.End >= nPosClip)
		{	
			GUID guid;
			(*ppClip)->GetGUID(&guid);
			vecClips.push_back(guid);
		}
	}

	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::Notify(NLEEVENT oEvent, WPARAM wParam, LPARAM lParam)
{
	
	HRESULT hr = S_OK;
	switch (oEvent.eventID)
	{
	case NLEEVENT_CLIP_PROPERTIES_CHANGE:
	{
		LPCWSTR pKeyName = (LPCWSTR)wParam;
		CNLEValue* pValue = (CNLEValue*)lParam;
		Update();
		break;
	}
	default:
		break;
	}
	
	return hr;
}



HRESULT __stdcall CNLETimelineBase::GetRender(INLERender** ppAudioRender, INLERender **ppVideoRender)
{
	HRESULT hr = E_FAIL;
	NLEComPtr<IUnknown> pUnk;
	this->QueryInterface(IID_IUnknown, (LPVOID*)&pUnk);

	// audio
	if (ppAudioRender)
	{
		if (NULL == m_pAudioRenderBase)
		{
			m_pAudioRenderBase = new CNLEAudioRenderBase(pUnk);
			hr = m_pAudioRenderBase ? m_pAudioRenderBase->NonDelegatingAddRef() : E_OUTOFMEMORY;
		}
		if (m_pAudioRenderBase)
		{
			hr = m_pAudioRenderBase->NonDelegatingQueryInterface(IID_INLERender, (LPVOID*)ppAudioRender);
		}
	}

	// video
	if (ppVideoRender)
	{
		if (NULL == m_pVideoRenderBase)
		{
			m_pVideoRenderBase = new CNLEVideoRenderBase(pUnk);
			hr = m_pVideoRenderBase ? m_pVideoRenderBase->NonDelegatingAddRef() : E_OUTOFMEMORY;			
		}
		if (m_pVideoRenderBase)
		{
			hr = m_pVideoRenderBase->NonDelegatingQueryInterface(IID_INLERender, (LPVOID*)ppVideoRender);
		}
	}	
	return hr;
}




void CNLETimelineBase::Update()
{	
	if (!IsCompositeTimeline())
 	{
 		//没有子timeline时就保持本身的状态
 		return;
 	}

	int nTotalFrmCount = 0;

	TIMELINE_FOR_EACH(it)
	{
		INT nPos = 0;
		NLERange rngClip;
		NLEPropComPtr pPropClip = NLECommon::NLEGetProperties((INLETimelineBase*)*it);
		pPropClip->GetPosition(&nPos);
		pPropClip->GetRenderRange(&rngClip);
		
		if (rngClip.Equals(NLECommon::Const::RANGE_INFINITY) ) continue;

		nTotalFrmCount = max(nTotalFrmCount, nPos + rngClip.Count() );
		_ASSERT(nTotalFrmCount <= NLECommon::Const::FRAMEINDEX_MAX + 1);
	}
	
	m_pProperties[kRender_TotalFrameCount] = nTotalFrmCount;

	NLERange rngRender = (CNLEValue)m_pProperties[kRender_RangFrameNumber];
	if (!rngRender.Equals(NLECommon::Const::RANGE_INFINITY) )
	{
        if (nTotalFrmCount > rngRender.Start) {
            rngRender.End = nTotalFrmCount - 1;
        } else {
            rngRender.Start =
                rngRender.End = (nTotalFrmCount > 0) ? (nTotalFrmCount - 1) : (0);
        }

		//rngRender.End = rngRender.Start + nTotalFrmCount -1; // 需要-1，因为rng.count()计算规则是：end -start +1
		m_pProperties[kRender_RangFrameNumber] = rngRender;
	}
	m_oRenderRange = rngRender;
	_ASSERT(rngRender.Count() <= NLECommon::Const::FRAMEINDEX_MAX + 1);
}

HRESULT __stdcall CNLETimelineBase::UpdateSourceInfo()

{
	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::UpdateRenderInfo()
{
	HRESULT hr = S_OK;
	return hr;
}



NLEComPtr<INLETimelineInfo> CNLETimelineBase::TIMELINE_INFO_INTERFACE(NLEComPtr<INLETimelineBase> pClip)
{
	_ASSERT(pClip);
	HRESULT hr = E_FAIL;
	NLEComPtr<INLETimelineInfo> pInfo;
	hr = QUERY_INTERFACE(pClip, IID_INLETimelineInfo, &pInfo); CHECK(hr);
	return pInfo;
}

HRESULT CNLETimelineBase::UpdateClipsRenderInfo()
{
    CNLESingleLock single_lock(&m_mutexlstTimeline);
	if (m_lstTimeline.size() == 0)
	{
		return E_FAIL;
	}

	TIMELINE_FOR_EACH(it)
	{
		SyncRenderProp(*it);
	}
	
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetSourceVideoSize(LPCWSTR keyName, CNLEValue& oSize)
{
	NLESize sz;
	oSize.Get(sz);

	//.video.sizSize = sz; 
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetSourceTotalFrameCount(LPCWSTR keyName, CNLEValue& nFrameCount)
{
	int fc;
	nFrameCount.Get(fc);
	//.video.nTotalFrameCount = fc;
	return S_OK; 
}

HRESULT CNLETimelineBase::OnSetSourceFramerate(LPCWSTR keyName, CNLEValue& ralFramerate)
{
	NLERational fr;
	ralFramerate.Get(fr);
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetSourceRegion(LPCWSTR keyName, CNLEValue& rectFRegion)
{
	NLERectF region;
	rectFRegion.Get(region);
	//.video.rectFRegion = region;
	return S_OK;
}


HRESULT CNLETimelineBase::OnSetSourceSampleRate(LPCWSTR keyName, CNLEValue& nSamplerate)
{
	int sr;
	nSamplerate.Get(sr);
	//.audio.nSamplerate = sr;
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetSourceBitsPerSample(LPCWSTR keyName, CNLEValue& nBitsPerSample)
{
	int bps;
	nBitsPerSample.Get(bps);
	//.audio.nBitsPerSample = bps;
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetSourceChannels(LPCWSTR keyName, CNLEValue& nChannels)
{
	int chs;
	nChannels.Get(chs);
	//.audio.nChannels = chs;
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetRenderVideoSize(LPCWSTR keyName, CNLEValue& oSize)
{
	NLESize sz;
	oSize.Get(sz);
	//m_oRenderInfo.video.sizSize = sz;

	std::for_each(m_lstTimeline.begin(), m_lstTimeline.end(), [&](TIMELINE_LIST::value_type v) {
		NLEComPtr<INLEProperties> prop = NLECommon::NLEGetProperties(v);
		prop->SetSize(NLEKey::Render::kRender_Size, sz);
	});
    
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetRenderRegion(LPCWSTR keyName, CNLEValue& rectFRegion)
{
	//rectFRegion.Get(//m_oRenderInfo.video.rectFRegion);
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetRenderPostion(LPCWSTR keyName, CNLEValue& nleValue)
{
	HRESULT hr = S_OK;
	INT nPos = nleValue;
	NLEComPtr<INLETimelineBase> pParent = GetParentTimeline();
	//PARENT_ASSERT(pParent);
	RETURN_IF_NULL(pParent, S_OK);

	m_pProperties[kRender_Position] = nPos;

    ((CNLETimelineBase *)(INLETimelineBase*)pParent)->resortClip();
    ((CNLETimelineBase *)(INLETimelineBase*)pParent)->Update();
    ((CNLETimelineBase *)(INLETimelineBase*)pParent)->SetRenderDirty(true);

	return S_OK;
}

HRESULT CNLETimelineBase::OnSetRenderTotalFrameCount(LPCWSTR keyName, CNLEValue& nFrameCount)
{
	//nFrameCount.Get(m_oRenderInfo.video.nTotalFrameCount);

	return S_OK;
}

HRESULT CNLETimelineBase::OnSetRenderRangeFrameNumber(LPCWSTR keyName, CNLEValue& nleValue)
{
	HRESULT hr = S_OK;
	NLERange rngNew = nleValue;
// 	LOGGER_INFO_(L"set render range [%d, %d]", rngNew.Start, rngNew.End);

	NLEComPtr<INLETimelineBase> pParent = GetParentTimeline();
	//PARENT_ASSERT(pParent);
	RETURN_IF_NULL(pParent, S_OK);

	//此处相等比较的判断会导致修改media timeline的range之后无法自动更新其父结点的range和total。
	//NLERange rngCur = m_pProperties[keyName];
	//RETURN_IF_FALSE(!rngCur.Equals(rngNew), S_OK); //相等

	m_pProperties[keyName] = rngNew;
	
	// 复合片段的剪切，需要递归调整子片段的渲染范围
	// 存在矛盾：
	// 当范围缩小的时候，将子片段的范围和pos调整好即可；
	// 但是，当范围扩大的时候，子片段的范围该怎么处理？！！！

    ((CNLETimelineBase *)(INLETimelineBase*)pParent)->resortClip();
    ((CNLETimelineBase *)(INLETimelineBase*)pParent)->Update();
    ((CNLETimelineBase *)(INLETimelineBase*)pParent)->SetRenderDirty(true);

	return hr;
}

HRESULT CNLETimelineBase::OnSetRenderFrameRate(LPCWSTR keyName, CNLEValue& ralFramerate)
{
	HRESULT hr = S_OK;
	NLERational ralNewFramerate;
	ralFramerate.Get(ralNewFramerate);
	if (ralNewFramerate.Value() < 0.0)
	{
		_ASSERT(FALSE); LOGGER_ERROR_(L"Invalid frame : %.3lf", ralNewFramerate.Value());
		return E_INVALIDARG;
	}
	TIMELINE_FOR_EACH(ppClip)
	{
		NLEPropDecorator pClipProp = NLECommon::NLEGetProperties(*ppClip);
		NLERational ralOldFramerate;
		pClipProp->GetRational(keyName, &ralOldFramerate);
		if (ralOldFramerate != ralNewFramerate)
		{
			if (FAILED(hr = pClipProp->SetRational(keyName, ralNewFramerate)))
			{
				LOGGER_ERROR_(L"Failed to set framerate:%.3lf", ralNewFramerate.Value());  _ASSERT(FALSE);
				return hr;
			}			
		}
	}
	
	int position = 0;
	m_pProperties->GetInt(NLEKey::Render::kRender_Position, &position);
	NLERational ralCurFramerate;
	m_pProperties->GetRational(keyName, &ralCurFramerate);
	int newPosition = int(position * ralNewFramerate.Value() / ralCurFramerate.Value());
	m_pProperties->SetInt(NLEKey::Render::kRender_Position, newPosition);

	Update();

	return S_OK;
}


HRESULT CNLETimelineBase::OnSetRenderSampleRate(LPCWSTR keyName, CNLEValue& nSamplerate)
{
	//nSamplerate.Get(m_oRenderInfo.audio.nSamplerate);
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetRenderBitsPerSample(LPCWSTR keyName, CNLEValue& nBitsPerSample)
{
	//nBitsPerSample.Get(m_oRenderInfo.audio.nBitsPerSample);
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetRenderChannels(LPCWSTR keyName, CNLEValue& nChannels)
{
	//nChannels.Get(m_oRenderInfo.audio.nChannels);
	return S_OK;
}




HRESULT CNLETimelineBase::OnSetSourceRange(LPCWSTR keyName, CNLEValue& oRange)
{
	//oRange.Get(//.rngFrameNumber);
	return S_OK;
}


HRESULT CNLETimelineBase::OnSetAnimationURI(LPCWSTR keyName, CNLEValue& szAnimationURI)
{
	std::wstring uri;
	szAnimationURI.Get(uri);

	return S_OK;
}

HRESULT CNLETimelineBase::OnSetTransitionID(LPCWSTR keyName, CNLEValue& id)
{
// 	std::wstring transID = id;	
	return S_OK;
}

HRESULT CNLETimelineBase::OnSetTransitionRangeFrameNumber(LPCWSTR keyName, CNLEValue& range)
{
	//NLERange rng;
	//range.Get(rng);	
	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::Clone(LPVOID *ppObj)
{
	HRESULT hr = E_FAIL;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 工厂
	NLEComPtr<INLEFactory> pFactory = NULL;
	hr = NLECommon::GetInterface(CLSID_NLEFactory, NULL, 0, IID_INLEFactory, (LPVOID*)&pFactory); NLE_ASSERT(SUCCEEDED(hr));

	hr = pFactory->Init(NULL);

	NLEComPtr<INLEXml> pXml;
	pXml = pFactory->CreateXML();
	if (!pXml)
	{
		LOGGER_ERROR_(L"create xml failed");
		return E_FAIL;
	}

	NLEComPtr<INLEDocumentSerialize> pSerializer = pFactory->CreateDocumentSerialize(); NLE_ASSERT(pSerializer);

	hr = pSerializer->Save(this, pXml);
	NLE_ASSERT(SUCCEEDED(hr));

	hr = pXml->ResetMainPos(); CHECK(hr);
	
	NLEComPtr<INLETimelineBase> pOutTimeline = NULL;
	hr = pSerializer->Load(&pOutTimeline, pXml); CHECK(hr);
	if (NULL == pOutTimeline)
		return hr;

	hr = pOutTimeline->QueryInterface(IID_INLETimelineBase, ppObj); CHECK(hr);
	
	return hr;
}

HRESULT CNLETimelineBase::AppendTimeline(NLEComPtr<INLETimelineBase> pClip)
{	
	RETURN_IF_NULL(pClip, E_INVALIDARG);

	HRESULT hr = E_NLE_UNEXPECTED;
	
    if (std::find(m_lstTimeline.begin(), m_lstTimeline.end(), pClip) != m_lstTimeline.end())
	{
		return E_NLE_ALLREADY_EXISTS;
	}

	m_lstTimeline.push_back(pClip);
    this->resortClip();

	NLEComPtr<INLEService> pClipSvr;
	hr = pClip->QueryInterface(IID_INLEService, (LPVOID *)&pClipSvr); CHECK(hr);

	NLEComPtr<IUnknown> pUnkThis;
	this->QueryInterface(IID_IUnknown, (LPVOID *)&pUnkThis); CHECK(hr);

	pClipSvr->SetParent(pUnkThis);
	
	this->SetRenderDirty(true);

	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::GetIterator(INLEIterator **pIterator)
{
	return this->QueryInterface(IID_INLEIterator, (LPVOID*)pIterator);
}

HRESULT __stdcall CNLETimelineBase::MoveToBegin()
{
	INT nClipCount = 0;
	GetClipCount(&nClipCount);
	if (nClipCount  == 0)
	{
		return E_FAIL;
	}
	
	_P(this)->SetInt(pszItemCurrent, 0);

	return S_OK;
}

HRESULT __stdcall CNLETimelineBase::MoveToNext()
{
	INT nCurrent = 0;
	HRESULT hr = _P(this)->GetInt(pszItemCurrent, &nCurrent); CHECK(hr);
	nCurrent++; 
	_P(this)->SetInt(pszItemCurrent, nCurrent);

	INT nClipCount = 0;
	GetClipCount(&nClipCount);
	if (nClipCount == 0 || nCurrent >= nClipCount)
	{
		return E_FAIL;
	}
	
	return S_OK;
}




NLEComPtr<IUnknown> __stdcall CNLETimelineBase::Current()
{
    CNLESingleLock single_lock(&m_mutexlstTimeline);
	INT nCurrent = 0;
	HRESULT hr = _P(this)->GetInt(pszItemCurrent, &nCurrent);
	CHECK(hr);
	INT nClipCount = (INT)m_lstTimeline.size();
	if (nCurrent >= nClipCount)
	{		
		return NULL;
	}

	TIMELINE_LIST::iterator it = m_lstTimeline.begin();	
	std::advance(it, nCurrent);

	NLEComPtr<IUnknown> pUnk;
	hr = (*it)->QueryInterface(IID_IUnknown, (LPVOID*)&pUnk);
	return pUnk;
}

NLEFrameIndex __stdcall CNLETimelineBase::GetSourceFrameIndex(const NLEFrameIndex frmIdxRener, FrameIndexType type)
{
	return frmIdxRener;
}

NLEFrameIndex __stdcall CNLETimelineBase::GetRenderFrameIndex(const NLEFrameIndex frmIdxSource, FrameIndexType type)
{
	return frmIdxSource;
}

NLEComPtr<INLETimelineBase> CNLETimelineBase::GetParentTimeline()
{
	NLEComPtr<INLEService> pSrv;
	HRESULT hr = this->QueryInterface(IID_INLEService, (LPVOID *)&pSrv);
	_ASSERT(SUCCEEDED(hr));
	NLEComPtr<IUnknown> pParent = pSrv->GetParent();
	RETURN_IF_NULL(pParent, NULL);

	NLEComPtr<INLETimelineBase> pTimelineBase = NULL;
	hr = pParent->QueryInterface(IID_INLETimelineBase, (LPVOID*)&pTimelineBase); CHECK(hr);
	return pTimelineBase;
}

HRESULT CNLETimelineBase::NotifyParent(LPCWSTR keyName, CNLEValue &nleValue)
{
	HRESULT hr = S_OK;
	NLEComPtr<INLETimelineBase> pParent = GetParentTimeline();
	if (pParent)
	{
		hr = pParent->Notify(NLEEVENT_CLIP_PROPERTIES_CHANGE, (WPARAM)keyName, (LPARAM)&nleValue);
	}
	return hr;
}

HRESULT __stdcall CNLETimelineBase::GetGUID(GUID *pGuid)
{
	RETURN_IF_NULL(pGuid, E_INVALIDARG);

	memcpy(pGuid, &m_timelineGuid, sizeof(GUID));

	return S_OK;
}

void CNLETimelineBase::SetRenderDirty(bool isDirty)
{
	if (m_pAudioRenderBase != NULL)
	{
		m_pAudioRenderBase->IsDirty() = isDirty;
	}
	if (m_pVideoRenderBase != NULL)
	{
		m_pVideoRenderBase->IsDirty() = isDirty;
	}
}

BOOL CNLETimelineBase::IsCompositeTimeline()
{
	CLSID clsid;
	m_pProperties->GetGUID(NLEKey::Base::kCLSID, &clsid);

	return clsid == CLSID_NLETimelineBase;
}

bool MyTimelineCompare(const TIMELINE_LIST::value_type p1, const TIMELINE_LIST::value_type p2) {
    NLEPropDecorator  prop1 = NLECommon::NLEGetProperties(p1);
    NLEPropDecorator  prop2 = NLECommon::NLEGetProperties(p2);
    _ASSERT(prop2 && prop1);

    // 1、按层级	
    INT nLvl1 = (CNLEValue)prop1[kLevel];
    INT nLvl2 = (CNLEValue)prop2[kLevel];
    if (nLvl1 != nLvl2) {
        return nLvl1 < nLvl2;
    }

    //2. 按Position	
    INT pos1 = (CNLEValue)prop1[kRender_Position]; _ASSERT(pos1 >= -1);
    INT pos2 = (CNLEValue)prop2[kRender_Position]; _ASSERT(pos2 >= -1);
    if (pos1 != pos2) {
        return (pos1 <= pos2);
    }

    //按地址长:实在没办法决一高下了
    LPVOID addr1 = p1;
    LPVOID addr2 = p2;
    return (addr1 < addr2);
}

void CNLETimelineBase::resortClip() {
     m_lstTimeline.sort(MyTimelineCompare);
}

//////////////////////////////////////////////////////////////////////////


scombase::CUnknown* CALLBACK CreateTimelineBaseInstance(LPUNKNOWN pUnkOuter, HRESULT *phr)
{
	_ASSERT(phr);

	CNLETimelineBase* pTimelineBase = new CNLETimelineBase(pUnkOuter);
	
	if (pTimelineBase)
	{		
		scombase::CUnknown* pNewObj = (scombase::CUnknown*)pTimelineBase;
		pNewObj->NonDelegatingAddRef();
	
		*phr = pTimelineBase->Init(); CHECK(*phr);
		if (FAILED(*phr))
		{
			delete pTimelineBase;
			return NULL;
		}
		return pNewObj;
	}
	else
	{
		*phr = E_OUTOFMEMORY;
		return NULL;
	}	
}
