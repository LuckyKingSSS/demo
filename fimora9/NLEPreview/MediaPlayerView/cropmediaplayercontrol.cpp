#include "stdafx.h"
#include "cropmediaplayercontrol.h"
#include "inc_CommonLib/NLETimeline.h"
#include "INLETimelineBase.h"
#include "mediaplayerviewtoolbar.h"

CropMediaPlayerControl::CropMediaPlayerControl()
{
	m_pDestPlayerView = nullptr;
	m_pDestTimeline = nullptr;
}

CropMediaPlayerControl::~CropMediaPlayerControl()
{

}

void CropMediaPlayerControl::SetDestViewHwnd(HVIEW hviewDestParent, const RECT &rect)
{
	if (m_pDestPlayerView == nullptr)
	{
		m_pDestPlayerView = m_IPlayerViewMgr->CreatePlayView(hviewDestParent, rect);
	}
	else
	{
		m_pDestPlayerView->SetParentWND(hviewDestParent);
		auto hwnd = m_pDestPlayerView->GetWND();
#ifdef WIN32
		SetWindowPos(m_pDestPlayerView->GetWND(), 0, 0, 0, rect.right, rect.bottom, 0x0010); //最后一个参数SWP_NOACTIVATE，跨平台编译才能通过
#endif
    }
}

bool CropMediaPlayerControl::Open(NLETimeline *pDestTimeline)
{
	Q_ASSERT(pDestTimeline);
	m_pDestTimeline = pDestTimeline;
	m_TimelineBase = pDestTimeline;
	INLERender *pVideoRender = nullptr;
	INLERender *pAudioRender = nullptr;

	if (!m_TimelineBase->GetRender(&pAudioRender, &pVideoRender)) 
		return false;

	INLERender *pVideoOrignalRender;
	HRESULT hr = pVideoRender->QueryInterface(IID_INLEOrignalRender, (LPVOID*)&pVideoOrignalRender);
	Q_ASSERT(SUCCEEDED(hr));

	m_fps = m_pDestTimeline->GetRenderFramerate();
	m_haveSource = true;

	if (!SUCCEEDED(m_IMediaPlayer->SetAudioInput(pAudioRender))) return false;

	if (!SUCCEEDED(m_IPlayerView->SetVideoInput(1, pVideoOrignalRender))) return false;
	if (!SUCCEEDED(m_pDestPlayerView->SetVideoInput(0, pVideoRender))) return false;
	
	if (m_toolbar)
	{
		m_toolbar->Open();
		m_toolbar->SetFPS(m_fps);
		m_toolbar->SetDuration(Duration());
	}
	UpdateRatio();

	return true;
}

