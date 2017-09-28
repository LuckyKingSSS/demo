#pragma once

#include "NLERenderBase.h"
#include "NLECommon.h"
#include "NLEMacroDefine.h"

class CNLEAudioRenderBase :
	public CNLERenderBase
{
public:
	CNLEAudioRenderBase(IUnknown *pUnkOuter);
	
	~CNLEAudioRenderBase();

public:
	STDOVERRIDEMETHODIMP NonDelegatingQueryInterface(REFIID riid, LPVOID *ppObj);

public:
	DECLARE_PROPERTY_LISTEN

	virtual HRESULT __stdcall Seek(NLEFrameIndex nFrameIndex, NLEFrameIndex *pRealFrameIndex = NULL) override;
	virtual HRESULT __stdcall Forward(INT nFrameCount, INT *pRealFrameCount) override;
	virtual HRESULT __stdcall Prepare(INLEFrame* pFrame) override;
	virtual HRESULT __stdcall Render(INLEFrame* pFrame) override;
public:
	virtual HRESULT PrepareInternal(INLEFrame** ppOutFrame);
	virtual HRESULT RenderInternal(INLEFrame* ppOutFrame) { return S_OK; }

protected:
	NLEComPtr<INLEFrame> CreateAudioFrame(INT nSize);

	virtual BOOL IsAudioRender() override;

	// 需要放在这里，否则当tlbase里面不适用这函数的时候，mediatl里面就会链接错误，找不到该函数的实现
	__inline void GetCurrentAudioFrameTimestamp(NLEFrameIndex frmIndex, LONGLONG& pts, LONGLONG &dur)
	{
		
		NLEPropDecorator pPropRender = NLECommon::NLEGetProperties(this);
		
		NLEFramerate ralFramerate = pPropRender[kRender_Framerate];
		pts = (LONGLONG)(frmIndex * S_TO_100NS / ralFramerate.Value());
		dur = (LONGLONG)(1		  *	S_TO_100NS / ralFramerate.Value() );
		
		return;
	}

protected:
	virtual INT DecideNextFrameRenderSize();

private:
	virtual NLEComPtr<INLERender> GetRender(INLETimelineBase *clip);

	virtual void UpdateNeedRenderList(NLEFrameIndex nFrameOffset);
	virtual TIMELINE_RENDER_LIST& GetNeedRenderList();

protected:	
	/**
	 *@brief  	设置帧数据大小
	 *@param	pFrame 要设置的帧
	 *@param	nDataSize 帧大小
	 *@param	bReallocatedIfOverflow 取值如下:
	 *-	TRUE	若pFrame的BufferSize<nDataSize，则重新分配内存
	 *-	FALSE	若pFrame的BufferSize<nDataSize，则不重新分配，直接返回失败
	 *@return 	HRESULT
	 */
	HRESULT SetDataSize(NLEComPtr<INLEFrame> pFrame, INT nDataSize, BOOL bReallocatedIfOverflow = FALSE);

protected:
	NLEComPtr<INLEFrame>  GetMixerFrame(BOOL bCreateIfNotExists = TRUE);
	//与背景音合成
	virtual HRESULT WillCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics) override;
	//与背景音合成
	virtual HRESULT DoCompose(INLEFramePtr pCurFrame, INLEFramePtr pGraphics) override;

private:
	///下一帧渲染长度
	UINT m_nNextFrameSize;

private:
	NLEComPtr<INLEFrame> m_pInternalFrame;

private:
	NLEComPtr<INLEFrame> m_pRenderFrame;
	NLEComPtr<INLEFrame> m_pMixerFrame;

};

scombase::CUnknown* CALLBACK CreateAudioRenderBaseInstance(LPUNKNOWN pUnkOuter, HRESULT *phr);
	