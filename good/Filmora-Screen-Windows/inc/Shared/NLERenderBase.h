#pragma once

#include "scombase.h"
#include "INLERender.h"
#include "NLEService.h"
#include "INLETimelineBase.h"
#include "NLEComPtr.h"
#include "NLEKeyShortcut.h"
#include "NLEEventDefine.h"
#include "INLETimelineInfo.h"
#include "NLEKey.h"
#include "INLEPostProcess.h"

#include "INLEAsyncTask.h"
#include "SimpleSync.h"
class CNLETimelineBase;

using namespace NLEKey::Codec::AttachedFrame;

typedef NLEComPtr<INLEFrame> INLEFramePtr;
typedef NLEComPtr<INLERender> INLERenderPtr;

typedef std::vector<INLERenderPtr> TIMELINE_RENDER_LIST;
extern NLERange GetActualRenderRange(NLEComPtr<INLEProperties> prop);

class CNLERenderBase :
	public scombase::CUnknown,	
	public INLERender
{
public:
	CNLERenderBase(IUnknown *pUnkOuter);
	//CNLERenderBase();
	virtual ~CNLERenderBase();
	
public:
	DECLARE_IUNKNOWN
	DECLARE_PROPERTY_LISTEN
	
	STDOVERRIDEMETHODIMP NonDelegatingQueryInterface(REFIID, LPVOID *);

	virtual HRESULT __stdcall Prepare(INLEFrame* pFrame) override;
	virtual HRESULT __stdcall Render(INLEFrame* pFrame) override;
	    
	virtual HRESULT __stdcall Seek(NLEFrameIndex nFrameIndex, NLEFrameIndex *pRealFrameIndex = NULL) override;
    virtual HRESULT __stdcall Forward(INT nFrameCount = 1, INT *pRealFrameCount = NULL) override;
	virtual int __stdcall GetOffset(void) override;
	virtual HRESULT __stdcall Cleanup() override;
	virtual HRESULT __stdcall SendCommand(UINT uCmd, NLEWParam wParam = NULL, NLELParam lParam = NULL) override;
public:
	/**
	 *@brief 绑定Frame，注意引用计数
	 */
	static HRESULT AttachFilterFrame(NLEComPtr<INLEFrame> pFrame, NLEComPtr<INLEFrame> pAttacher, LPCWSTR frameKe = kBagFrame);

	static NLEComPtr<INLEFrame> GetAttachedFilterFrame(NLEComPtr<INLEFrame> pFrame, LPCWSTR frameKey = kBagFrame);
	static HRESULT DetachFilterFrame(NLEComPtr<INLEFrame> pFrame, LPCWSTR frameKey);
	
public:
	virtual BOOL IsAudioRender() = 0;
	virtual HRESULT WillPostProcess(INLEFramePtr pCurFrame, INLEFramePtr pGraphics, NLEPropDecorator pProp);
	virtual HRESULT DoPostProcess(INLEFramePtr pCurFrame, INLEFramePtr pGraphics, INT nFlag = 0);
	virtual HRESULT WillCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics);
	virtual HRESULT DoCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics);
public:
	/**
	 *@brief  	获得所属的Timeline
	 *@return 	NLEComPtr<INLETimelineBase>
	 *@see 		参考资料
	 *@note		注意事项
	 */
	NLEComPtr<INLETimelineBase> GetTimeline();
	
	NLEComPtr<INLEService> GetService();

	/**
	 *@brief 获取Render属性
	 */
	NLEPropDecorator GetTimelineProp();

	NLEComPtr<INLERender> GetParent();

	bool &IsDirty();

protected:
	virtual NLEComPtr<INLERender> GetRender(INLETimelineBase *clip) = 0;

	virtual void UpdateNeedRenderList(NLEFrameIndex nFrameOffset) = 0;
	virtual TIMELINE_RENDER_LIST& GetNeedRenderList() = 0;

	HRESULT UpdateRenderList(int nFrameOffset, bool &notEndOfTimeline, bool seek_op = false);
	void ClassifyRenderList(NLEFrameIndex nFrameOffset, NLEFrameIndex advFrameGap, NLEFrameIndex dlyFrameGap, bool isDirty,
		INLERenderPtr &render, TIMELINE_RENDER_LIST &tmpAdvRenders);

	HRESULT MyPostCompInit();

	BOOL	PropChanged();
	NLESession GetCurrentSession();

	/**
	 *@brief 获取当前渲染目标
	 *@detail 具体如下:
	 *-IID_INLEPlayer 播放器
	 *-IID_INLEEncoder 编码器
	 */
	const GUID &GetSink();

private:
	BOOL IsChildClipRemoved(NLEComPtr<INLETimelineBase> pChildClip);
	HRESULT RefreshEventPoint(NLEFrameIndex refresh_pos);
	void ProcessSeekList();

protected:
	int m_nFrameCount;
	int m_nOffset;

	bool m_bIsDirty; // 是否被污染了{包括子片段的添加、删除、改变顺寻、层次、位置等等}

	// 更新点列表{按帧序，从小到大排序}
	std::vector<NLEFrameIndex> m_lstEvtPoint;

	// 当前正在渲染的
	bool m_bNotEndOfTimeline;
	TIMELINE_RENDER_LIST m_lstCurrentRenders;
	// 最终实际的渲染器列表。是m_lstCurrentRenders的一个子集
	TIMELINE_RENDER_LIST m_lstNeedRenders;
	// 需要进行定位的渲染器列表{即，预加载的、被覆盖的}
	std::vector<std::pair<INLERenderPtr, NLEFrameIndex>> m_lstSeekRenders;

	// 用于提前准备下一阶段的渲染
	int m_nAdvanceMSec; // 提前的时间
	TIMELINE_RENDER_LIST m_lstAdvanceRenders;

	// 用于滞后丢弃渲染器
	int m_nDiscardMSec; // 滞后的时间
	TIMELINE_RENDER_LIST m_lstDiscardRenders;

	// 用于异步seek的任务
	static NLEComPtr<INLESeekTask> m_pSeekTask;

protected://PostProcess
	NLEComPtr<INLEPostProcess> m_pPostProcess;
	NLEComPtr<INLEPostRequest> m_pPostRequest;

private:
	NLESession m_oPrevSession;
    BOOL m_bPropChanged;


	GUID			  m_guidSink;

protected:
    SimpleSync::CSimpleMutex _renderMutex;
};
