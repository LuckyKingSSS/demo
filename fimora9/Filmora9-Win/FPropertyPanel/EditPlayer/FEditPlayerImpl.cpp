#include "stdafx.h"
#include "inc_FProject/FNLETimeline.h"
#include "FEditPlayerImpl.h"
#include "FEditPlayerMgr.h"
#include "FEditRectangleView.h"
#include "inc_MediaPlayerView/MediaPlayerControl.h"
#include "inc_MediaPlayerView/MediaPlayerView.h"


FEditPlayerImpl::FEditPlayerImpl(QWidget *parent)
	: FBaseWidget(parent)
{
	m_pPlayerView = new MediaPlayerView(this);
	m_pPlayerCtrl = m_pPlayerView->GetMediaPlayerControl();
	m_pPlayerCtrl->SetRatio(16.0 / 9);
	m_pMgr = new FEditPlayerMgr(GetPlayerRegion(), this);
	m_pRectangleView = new FEditRectangleView(m_pMgr, this);
}

FEditPlayerImpl::~FEditPlayerImpl()
{

}

bool FEditPlayerImpl::Open(FNLETimeline *timeline)
{
	QList<FNLETimeline*> lst;
	lst.append(timeline);
	m_pMgr->SetTimelines(lst);
	return m_pPlayerCtrl->Open(1, timeline);
}

bool FEditPlayerImpl::Close()
{
	return m_pPlayerCtrl->Close();
}

void FEditPlayerImpl::Play()
{
	m_pPlayerCtrl->Play();
}

void FEditPlayerImpl::Stop()
{
	m_pPlayerCtrl->Stop();
}

void FEditPlayerImpl::Pause()
{
	m_pPlayerCtrl->Pause();
}

void FEditPlayerImpl::Seek(int frames)
{
	m_pPlayerCtrl->Seek(frames, 1);
}

void FEditPlayerImpl::SetDuration(int duration)
{
	//m_pPlayerMgr->SetD
}

void FEditPlayerImpl::Forward()
{
	m_pPlayerCtrl->Forward();
}

void FEditPlayerImpl::Backward()
{
	m_pPlayerCtrl->Backward();
}

void FEditPlayerImpl::Restore()
{

}

int FEditPlayerImpl::Duration()
{
	return m_pPlayerCtrl->Duration();
}

bool FEditPlayerImpl::HaveSource()
{
	return m_pPlayerCtrl->HaveSource();
}

int FEditPlayerImpl::CurrentFrame()
{
	return m_pPlayerCtrl->CurrentFrames();
}

bool FEditPlayerImpl::IsPlaying()
{
	return m_pPlayerCtrl->IsPlaying();
}

void FEditPlayerImpl::SetRectangleVisible(bool)
{

}

void FEditPlayerImpl::resizeEvent(QResizeEvent *)
{
	// 重新计算播放器区域
	m_pMgr->SetPlayerRegion(GetPlayerRegion());
	m_pPlayerView->setGeometry(rect());
	m_pRectangleView->setGeometry(rect());
}

void FEditPlayerImpl::slotRectChanged(const QRectF&)
{

}

QRectF FEditPlayerImpl::GetPlayerRegion()
{
	Q_ASSERT(m_pPlayerCtrl);
	float ratio = m_pPlayerCtrl->GetRatio();
	QRectF rc(0, 0, 0, 0);
	if (height()){
		if (width() / height() < ratio){
			float h = width() / ratio;
			rc = QRectF(0, 1.0*height() / 2 - h / 2, width(), h);
		}
		else{
			float w = ratio*height();
			rc = QRectF(1.0*width() / 2 - w / 2, 0, w, height());
		}
	}
	return rc;
}



















