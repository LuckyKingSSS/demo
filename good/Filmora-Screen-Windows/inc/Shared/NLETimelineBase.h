#pragma once
#include "INLETimelineBase.h"
#include "scombase.h"
#include "INLEProperties.h"
#include "NLEService.h"
#include "NLEConfig.h"
#include "NLEComPtr.h"
#include "INLETimelineInfo.h"
#include "INLESerialize.h"
#include "INLEClonable.h"

#include "INLEMotion.h"
#include "NLEPropDecorator.h"
#include "NLEAudioRenderBase.h"
#include "NLEVideoRenderBase.h"
#include "NLESingleLock.h"
#include <map>
#include <list>

typedef NLEComPtr<IUnknown> INLERenderObj;
typedef std::list<NLEComPtr<INLETimelineBase>> TIMELINE_LIST;

class CNLETimelineBase :
	public scombase::CUnknown,
	public INLETimelineBase,	
	public INLETimelineInfo,
    public INLEClonable,
	public INLEIterator,
	public CNLEService
{
	
	friend scombase::CUnknown* CALLBACK CreateTimelineBaseInstance(LPUNKNOWN pUnkOuter, HRESULT *phr);

public:
	CNLETimelineBase(IUnknown *pOuter);
	CNLETimelineBase();
	virtual ~CNLETimelineBase();

public:
	HRESULT RecalcRenderInfo();

public:
	virtual HRESULT __stdcall Init();

public:
	DECLARE_IUNKNOWN
	DECLARE_PROPERTY_LISTEN

	STDOVERRIDEMETHODIMP NonDelegatingQueryInterface(REFIID, LPVOID *);

	
	// INLETimelineBase
	virtual HRESULT __stdcall AddClip(INLETimelineBase *pTimeline) override;

	virtual HRESULT __stdcall GetClipCount(INT*pCount) override;

	virtual HRESULT __stdcall MoveClip(UINT iIndexFrom, UINT iIndexTo = -1) override;

	virtual HRESULT __stdcall RemoveClip(INLETimelineBase* pClip) override;

	virtual HRESULT __stdcall RemoveAll() override;

	virtual HRESULT __stdcall GetClip(GUID guid, INLETimelineBase **ppTimeline) override;

	virtual HRESULT __stdcall GetClips(NLERange &rng, std::vector<GUID>& vecClips) override;

	virtual HRESULT __stdcall Notify(NLEEVENT oEvent, WPARAM wParam, LPARAM lParam) override;

	virtual HRESULT __stdcall GetRender(INLERender** pAudioRender, INLERender **pVideoRender) override;

	virtual HRESULT __stdcall GetIterator(INLEIterator **pIterator) override;

	virtual HRESULT __stdcall GetGUID(GUID *pGuid) override;

public: //INLEClonable
    virtual HRESULT __stdcall Clone(LPVOID *ppObj) override;

public:
	virtual HRESULT __stdcall UpdateSourceInfo() override;
	virtual HRESULT __stdcall UpdateRenderInfo() override;
	virtual NLEFrameIndex __stdcall GetSourceFrameIndex(const NLEFrameIndex frmIdxRener, FrameIndexType type) override;

	virtual NLEFrameIndex __stdcall GetRenderFrameIndex(const NLEFrameIndex frmIdxSource, FrameIndexType type) override;
	//INLEIterator
public:
	
	virtual HRESULT  __stdcall MoveToBegin() override;
	virtual HRESULT  __stdcall MoveToNext() override;
	virtual NLEComPtr<IUnknown> __stdcall Current() override;
	
public:
	static NLEComPtr<INLETimelineInfo> TIMELINE_INFO_INTERFACE(NLEComPtr<INLETimelineBase> pClip);	
	
	virtual HRESULT UpdateClipsRenderInfo();

	virtual BOOL IsCompositeTimeline();
protected:	
	BOOL IsChildClip(const INLETimelineBase *pClip);
	HRESULT SyncRenderProp(INLETimelineBase *pClip);
	void Update();

	NLEComPtr<INLETimelineBase> GetParentTimeline();
	HRESULT NotifyParent(LPCWSTR keyName, CNLEValue &nleValue);
protected:	
	HRESULT AppendTimeline(NLEComPtr<INLETimelineBase> pClip);
	NLEComPtr<INLETimelineBase> GetClip(INT nIndex);

    void resortClip();
	
private:
	//source info
	
	virtual HRESULT OnSetSourceVideoSize(LPCWSTR keyName, CNLEValue& oSize);
	virtual HRESULT OnSetSourceTotalFrameCount(LPCWSTR keyName, CNLEValue& nFrameCount);
	virtual HRESULT OnSetSourceFramerate(LPCWSTR keyName, CNLEValue& ralFramerate);
	virtual HRESULT OnSetSourceRegion(LPCWSTR keyName, CNLEValue& rectFRegion);

	virtual HRESULT OnSetSourceSampleRate(LPCWSTR keyName, CNLEValue& nSamplerate);
	virtual HRESULT OnSetSourceBitsPerSample(LPCWSTR keyName, CNLEValue& nBitsPerSample);
	virtual HRESULT OnSetSourceChannels(LPCWSTR keyName, CNLEValue& nChannels);
	
	virtual HRESULT OnSetSourceRange(LPCWSTR keyName, CNLEValue& oRange);

	//render info
	 
	virtual HRESULT OnSetRenderVideoSize(LPCWSTR keyName, CNLEValue& oSize);
	virtual HRESULT OnSetRenderRegion(LPCWSTR keyName, CNLEValue& rectFRegion);
	virtual HRESULT OnSetRenderPostion(LPCWSTR keyName, CNLEValue& pos);
	virtual HRESULT OnSetRenderTotalFrameCount(LPCWSTR keyName, CNLEValue& nFrameCount);
	virtual HRESULT OnSetRenderRangeFrameNumber(LPCWSTR keyName, CNLEValue& rngFrameNumber);
	virtual HRESULT OnSetRenderFrameRate(LPCWSTR keyName, CNLEValue& ralFramerate);
    virtual HRESULT OnSetRenderScale(LPCWSTR keyName, CNLEValue& oRenderScale);
    virtual HRESULT OnSetRenderScaleEnable(LPCWSTR keyName, CNLEValue& bRenderScaleEnable);

	virtual HRESULT OnSetRenderSampleRate(LPCWSTR keyName, CNLEValue& nSamplerate);
	virtual HRESULT OnSetRenderBitsPerSample(LPCWSTR keyName, CNLEValue& nBitsPerSample);
	virtual HRESULT OnSetRenderChannels(LPCWSTR keyName, CNLEValue& nChannels);

	 // motion
	virtual HRESULT OnSetAnimationURI(LPCWSTR keyName, CNLEValue& szAnimationURI);
	 // transition
	virtual HRESULT OnSetTransitionID(LPCWSTR keyName, CNLEValue& id);
	virtual HRESULT OnSetTransitionRangeFrameNumber(LPCWSTR keyName, CNLEValue& range);

	// 属性感应函数 
	virtual HRESULT OnSetName(LPCWSTR keyName, CNLEValue& pName);
	virtual HRESULT OnSetLevel(LPCWSTR keyName, CNLEValue& nLevel);

	virtual HRESULT SetCrop(LPCWSTR keyName, NLERect rect);
	virtual HRESULT SetAnchorPoint(LPCWSTR keyName, const NLEPoint& pt);
	virtual HRESULT SetAttacher(LPCWSTR keyName, IUnknown *pUnk);

	void SetRenderPosition(NLEComPtr<INLERender> pRender);
	void SetRenderFrameCount(NLEComPtr<INLERender> pRender);
	void SetRenderRangeFrameNumber(NLEComPtr<INLERender> pRender);

protected:
	virtual void SetRenderDirty(bool isDirty);

protected:
	TIMELINE_LIST m_lstTimeline;
    CNLEMutex     m_mutexlstTimeline;

protected:
	GUID m_timelineGuid;

private:
	CNLEAudioRenderBase *m_pAudioRenderBase;
	CNLEVideoRenderBase *m_pVideoRenderBase;
	NLERange m_oRenderRange;
};

scombase::CUnknown* CALLBACK CreateTimelineBaseInstance(LPUNKNOWN pUnkOuter, HRESULT *phr);
