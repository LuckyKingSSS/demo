#pragma once

#include <map>

#include "scombase.h"
#include "NLERenderBase.h"
#include "INLETimelineBase.h"

#include "INLEMotion.h"
#include "INLEImageProc.h"
#include "INLETransition.h"
#include "INLEPostProcess.h"

#include "NLECommon.h"
#include "INLEFrameRateCalculator.h"

#include "INLEImageProc.h"
#include "RenderTransition.h"

class CNLEVideoRenderBase :
	public CNLERenderBase
{
public:
	CNLEVideoRenderBase(IUnknown *pUnkOuter);	
	virtual ~CNLEVideoRenderBase();
public:
	STDOVERRIDEMETHODIMP NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj);

public:
	DECLARE_PROPERTY_LISTEN

	virtual HRESULT __stdcall Seek(NLEFrameIndex nFrameIndex, NLEFrameIndex *pRealFrameIndex = NULL);
	virtual HRESULT __stdcall Forward(INT nFrameCount = 1, INT *pRealFrameCount = NULL);
	virtual HRESULT __stdcall Prepare(INLEFrame* pFrame) override;
	virtual HRESULT __stdcall Render(INLEFrame* pFrame) override;
	virtual HRESULT __stdcall Cleanup() override;

protected:
	//与底图合成
	virtual HRESULT WillCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics) override;
	//与底图合成
	virtual HRESULT DoCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics) override;

protected:
	virtual HRESULT PrepareInternal(INLEFrame** ppOutFrame);
	virtual HRESULT RenderInternal(INLEFrame* ppOutFrame);	

protected:
	NLEComPtr<INLEFrame> CreateVideoFrame(int nWidth, int nHeight);

	// 需要放在这里，否则当tlbase里面不适用这函数的时候，mediatl里面就会链接错误，找不到该函数的实现
	__inline void GetCurrentVideoFrameTimestamp(NLEComPtr<INLEFrame> pFrame, LONGLONG& pts, LONGLONG &dur)
	{
		HRESULT hr = pFrame->GetTimestamp(&pts, &dur);

		NLEPropDecorator pPropRender = NLECommon::NLEGetProperties(this);
		{
			NLERational oFrameRate = pPropRender[kRender_Framerate];
			dur = (NLElonglong)(S_TO_100NS / oFrameRate.Value());
			NLEFrameIndex frmOffset = GetOffset();
			pts = (LONGLONG)(frmOffset * S_TO_100NS * 1.0f/ oFrameRate.Value() );
#ifdef _DEBUG
			LONGLONG pts2 = FrameNumberToPTS(frmOffset, oFrameRate);
			LONGLONG delta = pts2 - pts;
			static LONGLONG DELTA_MAX = NLECommon::Time::MSecToNSec(100);
			if (delta < -DELTA_MAX || delta > DELTA_MAX)
			{
				printf("offset:%d, pts(xiaosl):%lld, pts(wangyx):%lld, delta:%.3lf (MS) \n", frmOffset, pts, pts2, delta / 10000.0f);
			}
#endif
			
		}
		return;
	}

	NLEComPtr<INLEImageProc> &GetImageProc();
private:
	NLEComPtr<INLEImageProc> m_pImageProc;
	CNLETransition m_oTransitioner;

private:
	virtual NLEComPtr<INLERender> GetRender(INLETimelineBase *clip);

	virtual BOOL IsAudioRender() override;

	virtual void UpdateNeedRenderList(NLEFrameIndex nFrameOffset);
	virtual TIMELINE_RENDER_LIST& GetNeedRenderList();

private:
	HRESULT ConCurrencyPrepare(NLEComPtr<INLEFrame> pGraphics, BOOL bConcurrentcy);
	HRESULT ChildrenRender(NLEComPtr<INLEFrame> pGraphics, BOOL bConcurrentcy);
	NLEComPtr<INLEFrame>  GetRegionFrame(BOOL bCreateIfNotExists = TRUE);

private:
	NLEComPtr<INLEFrame> m_pInternalFrame;

protected:

	// 用于并发prepare的任务；此任务跟异步seek任务不一样，需要嵌套，因此需要每个视频渲染器有一个任务对象
	/*static*/ NLEComPtr<INLEPrepareTask> m_pPrepareTask;
private:
	NLEComPtr<INLEFrame> m_pRenderFrame; // 源图，当没有源图时，与区域底图相同	
	NLEComPtr<INLEFrame> m_pRegionFrame; //用于渲染到Graphics上的图
	const int m_nCoveredSeekMSec = 7000; // 覆盖定位的片段时间
	NLESize m_oGraphicsSize;
	BOOL	m_bGraphicsChanged;
	NLERect m_rcRegion;
	INT m_nClipCount;					//Clip个数
};

scombase::CUnknown* CALLBACK CreateVideoRenderBaseInstance(LPUNKNOWN pUnkOuter, HRESULT *phr);
	