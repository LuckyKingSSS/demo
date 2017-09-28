#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>                                                            
#include "INLEMarkdown.h"
#include "INLETimelineBase.h"
#include "RepairInfo.h"
                                                                                
//{9A6A4DD6-72D4-ECCD-47FD-941AD46C676D}
DEFINE_GUID(CLSID_INLEMarkdownInternal, 0x9A6A4DD6, 0x72D4, 0xECCD, 0x47, 0xFD, 0x94, 0x1A, 0xD4, 0x6C, 0x67, 0x6D);                   
//{CB91D868-F034-6E20-314B-6BA0D74AD64C}
DEFINE_GUID(IID_INLEMarkdownInternal, 0xCB91D868, 0xF034, 0x6E20, 0x31, 0x4B, 0x6B, 0xA0, 0xD7, 0x4A, 0xD6, 0x4C);                         
typedef enum 
{	
	/**
	 *@brief 初始时的类型
	 */
	MarkdownType_Invalid,	
	
	/**
	 *@brief 只有空白区域，没有标记点
	 *@note 区域值限制如下：
	 *-		rngMark.Start==rngMark.End
	 *@note MarkPart: MarkDuration和RenderDuration为0
	 */
	MarkdownType_BlankOnly,

	/**
	 *@brief 空白与标记混合， 空白区域在前，标记点在后
	 *@note 区域值限制如下: 
	 *-		1.rngMark.Start < rngMark.End
	 *-		2.空白区域为[Baseline, Start - 1]
	 *-		3.标记区域为[Start, End]
	 */
	MarkdownType_Combine,

	/**
	 *@brief 定帧
	 *@note 区域值限制: rngMark.End == MarkdownRang_FreezeEnd
	 *@note BlankPart:MarkDuration和RenderDuration为0
	 *@note MarkPart:MarkDuration为1,RenderDuration为用户设置的FreezeLen
	 */
	MarkdownType_Freeze,

}MarkdownType;

class INLEMarkdownInternal :
public INLEMarkdown
{
public:
	/**
	 *@brief 设置Markdown类型，必须第一个调用，默认为MarkdownType_Invalid
	 */
	virtual void __stdcall SetType(MarkdownType type) = 0;
	virtual void __stdcall SetBaseline(INT nBaseline) = 0;
	virtual void __stdcall SetMedia(NLEComPtr<INLETimelineBase> pV, NLEComPtr<INLETimelineBase> pA) = 0;
	
	virtual void __stdcall SetRangeInternal(NLEMarkRange &rngMark, BOOL bNotifyParent = TRUE) = 0;

	virtual MarkdownType __stdcall GetType() = 0;
	virtual INT __stdcall GetBaseline() = 0;
	virtual NLERenderDuration __stdcall GetBlankPartDuration() = 0;
	virtual NLERenderDuration __stdcall GetMarkPartDuration() = 0;

	virtual NLERenderDuration  __stdcall GetRenderDuration() = 0;	
	virtual BOOL __stdcall IsFreeze() = 0;
	virtual BOOL __stdcall ShouldApplyCache() = 0;

	virtual NLEMarkPosition __stdcall GetFreezePosition() = 0;
	virtual void			__stdcall SetFreezePosition(NLEMarkPosition nPosition) = 0;
	virtual NLEMarkDuration __stdcall GetFreezeDuration() = 0;

	virtual FreezeAvaliableType	__stdcall GetFreezeAvaliableType() = 0;

	/**
	 *@brief 定帧点到起点的偏移
	 */
	virtual NLERenderDuration __stdcall GetRenderDurationFromFreezePosition(NLEMarkPosition nPosFreeze) = 0;
	virtual NLERenderDuration __stdcall GetRenderDurationFromMarkRange(NLEMarkRange rngMark) = 0;
	virtual NLEMarkDuration   __stdcall GetMarkDurationFromRenderPosition(NLERenderPosition nPosRender) = 0;

	virtual HRESULT __stdcall UpdateRenderInfo(NLEBOOL bNotifyParent = TRUE) = 0;

	virtual HRESULT __stdcall CopyTo(INLEMarkdownInternal *pDst) = 0;

	virtual HRESULT __stdcall UpdateFreezeFrame(NLEComPtr<INLEFrame>  pFrame) = 0;
	virtual NLEComPtr<INLEFrame> __stdcall GetFreezeFrame() = 0;
	virtual NLEBOOL NeedUpdateFreezeFrame(
		const NLESize &oRenderSize, 
		const RepairInfo &oRepairInfo, 
		NLEBOOL bUpdateSelf = FALSE) = 0;
};


