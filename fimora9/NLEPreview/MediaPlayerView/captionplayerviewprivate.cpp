#include "stdafx.h"

#include "inc_CommonLib/CommonWidgets.h"


#include "captionplayerviewprivate.h"
#include "mediaplayercontrolprivate.h"



CaptionPlayerView::CaptionPlayerView(QWidget *parent)
:MediaPlayerView(new CaptionPlayerViewPrivate, parent)
{
	auto toolbar = qobject_cast<CaptionPlayerViewPrivate*>(m_p)->GetToolBar();
	connect(toolbar, &ToolBarBase::StartRulerSeekCallback, this, &CaptionPlayerView::StartRulerSeekCallback);
	connect(toolbar, &ToolBarBase::EndRulerSeekCallback, this, &CaptionPlayerView::EndRulerSeekCallback);
	connect(toolbar, &ToolBarBase::RulerSeekCallback, this, &CaptionPlayerView::RulerSeekCallback);

	connect(toolbar, &CaptionMediaPlayerToolBar::Clicked, this, [&](int id){
		if (id == ToolBarBase::ToolBar_Forward)
			emit Forward();
	});
	connect(toolbar, &CaptionMediaPlayerToolBar::Clicked, this, [&](int id){
		if (id == ToolBarBase::ToolBar_Backward)
			emit Backward();
	});
}
//////////////////////////////////////////////////////////////////////////
CaptionPlayerViewPrivate::CaptionPlayerViewPrivate(QWidget *parent)
: MediaPlayerViewPrivate(parent, new MediaPlayerControl())
{
	m_toolbar = new CaptionMediaPlayerToolBar(m_mediaPlayerControl, this);
	connect(m_toolbar, &CaptionMediaPlayerToolBar::Clicked, this, &CaptionPlayerViewPrivate::ButtonClicked);
	m_mediaPlayerControl->SetToolBar(m_toolbar);


}

CaptionPlayerViewPrivate::~CaptionPlayerViewPrivate()
{

}

ToolBarBase* CaptionPlayerViewPrivate::GetToolBar()
{
	return m_toolbar;
}

void CaptionPlayerViewPrivate::ButtonClicked(int id)
{
	
}

void CaptionPlayerViewPrivate::resizeEvent(QResizeEvent *event)
{
	const int toolbarH = 40;
	m_displayView->setGeometry(0, 0, width(), height() - toolbarH);
	int top = m_displayView->geometry().bottom() + 5;
	m_toolbar->setGeometry(0, top, width(), toolbarH + 1);
}

//////////////////////////////////////////////////////////////////////////
CaptionMediaPlayerToolBar::CaptionMediaPlayerToolBar(MediaPlayerControl *control, QWidget *parent)
:ToolBarBase(control, parent)
{
	m_currentFrame = new CurrentFrameLabel(this);
	m_TotalFrame = new CurrentFrameLabel(this);
	m_currentFrame->SetPreciseFrames(false);
	m_TotalFrame->SetPreciseFrames(false);
	m_lb = new QLabel("/", this);
}

void CaptionMediaPlayerToolBar::Open()
{

}

void CaptionMediaPlayerToolBar::Close()
{
	ToolBarBase::Close();
	m_currentFrame->SetCurrentFrame(0);
	m_currentFrame->SetDuration(0);
	m_TotalFrame->SetCurrentFrame(0);
	m_TotalFrame->SetDuration(0);
}

void CaptionMediaPlayerToolBar::Stop()
{
	ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Play);
	m_slider->setValue(0);
	//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(false);
	RulerSeekCallback(0);
	// 去除停止播放后的底层回调的播放停止事件PM_Finish
	//QApplication::processEvents();

	m_currentFrame->SetCurrentFrame(0);
}

void CaptionMediaPlayerToolBar::SeekCallback(int frames, int accurate)
{
	if (m_slider->canSeek())
	{
		ToolBarBase::SeekCallback(frames, accurate);
		m_currentFrame->SetCurrentFrame(frames);
	}

}

void CaptionMediaPlayerToolBar::SeekManual(int frames, int accurate)
{
	if (m_mediaPlayerControl->HaveSource())
	{
		ToolBarBase::SeekManual(frames, accurate);
		m_currentFrame->SetCurrentFrame(frames);
	}
}

void CaptionMediaPlayerToolBar::SetDuration(int duration)
{
	ToolBarBase::SetDuration(duration);
	m_currentFrame->SetDuration(duration);
	m_TotalFrame->SetDuration(duration);
	m_TotalFrame->SetCurrentFrame(duration);
}

void CaptionMediaPlayerToolBar::SetFPS(float fps)
{
	m_currentFrame->SetFPS(fps);
	m_TotalFrame->SetFPS(fps);
}

void CaptionMediaPlayerToolBar::resizeEvent(QResizeEvent *event)
{
	const int margins = 5;
	const int space = margins;
	int left = margins;
	int top = margins;
	const int btnw = 14;
	const int btnh = 14;

	GetButton(ToolBar_Play)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Play)->geometry().right() + space; 
	GetButton(ToolBar_Stop)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Stop)->geometry().right() + space;
	GetButton(ToolBar_Backward)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Backward)->geometry().right() + space;
	GetButton(ToolBar_Forward)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Forward)->geometry().right() + space;
	
	const int frameWidth = 50;
	const int rightbtnw = 2 * frameWidth + 10;
	int sliderw = width() - left - rightbtnw;
	m_slider->setGeometry(left, top, sliderw, m_slider->Height());

	left = m_slider->geometry().right() + space;
	m_currentFrame->setGeometry(left, top, frameWidth, btnh);
	left = m_currentFrame->geometry().right();
	m_lb->setGeometry(left, top, 10, btnh); 
	left = m_lb->geometry().right();
	m_TotalFrame->setGeometry(left, top, frameWidth, btnh);
}
