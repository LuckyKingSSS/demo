#include "stdafx.h"
#include "actioncamplayerviewprivate.h"

#include "INLETimelineBase.h"

#include "inc_CommonLib/CommonWidgets.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/NLETimeline.h"

#include "inc_MediaPlayerView/MediaPlayerView.h"
#include "mediaplayercontrolprivate.h"


ActionCamPlayerView::ActionCamPlayerView(QWidget *parent)
:MediaPlayerView(new ActionCamPlayerviewPrivate, parent)
{
	auto toolbar = qobject_cast<ActionCamPlayerviewPrivate*>(m_p)->GetToolBar();
	connect(toolbar, &ToolBarBase::StartRulerSeekCallback, this, &ActionCamPlayerView::StartRulerSeekCallback);
	connect(toolbar, &ToolBarBase::EndRulerSeekCallback, this, &ActionCamPlayerView::EndRulerSeekCallback);
	connect(toolbar, &ToolBarBase::RulerSeekCallback, this, &ActionCamPlayerView::RulerSeekCallback);

}


void ActionCamPlayerView::Forward()
{
	(qobject_cast<ActionCamPlayerviewPrivate*>(m_p))->Forward();
}

void ActionCamPlayerView::EnableWidgets(bool bEnable)
{
	ActionCamPlayerviewPrivate* p = qobject_cast<ActionCamPlayerviewPrivate*>(m_p);
	p->EnableWidgets(bEnable);
}

void ActionCamPlayerView::Backward()
{
	(qobject_cast<ActionCamPlayerviewPrivate*>(m_p))->Backward();

}

//////////////////////////////////////////////////////////////////////////

ActionCamPlayerviewPrivate::ActionCamPlayerviewPrivate(QWidget *parent)
: MediaPlayerViewPrivate(parent, new MediaPlayerControl())
{
	m_toolbar = new ActionCamMediaPlayerToolBar(m_mediaPlayerControl, this);
	connect(m_toolbar, &ActionCamMediaPlayerToolBar::Clicked, this, &ActionCamPlayerviewPrivate::ButtonClicked);
	m_mediaPlayerControl->SetToolBar(m_toolbar);

}

ActionCamPlayerviewPrivate::~ActionCamPlayerviewPrivate()
{

}

void ActionCamPlayerviewPrivate::EnableWidgets(bool bEnable)
{
	m_toolbar->EnableWidgets(bEnable);
}



ToolBarBase* ActionCamPlayerviewPrivate::GetToolBar()
{
	return m_toolbar;
}

void ActionCamPlayerviewPrivate::Forward()
{
	m_toolbar->Forward();
}

void ActionCamPlayerviewPrivate::Backward()
{
	m_toolbar->Backward();
}



void ActionCamPlayerviewPrivate::ButtonClicked(int id)
{

}

void ActionCamPlayerviewPrivate::resizeEvent(QResizeEvent *event)
{
	const int toolbarH = 40;
	m_displayView->setGeometry(0, 0, width() - 1 , height() - toolbarH);
	int top = m_displayView->geometry().bottom();
	m_toolbar->setGeometry(0, top, width() - 1, toolbarH);
}

//////////////////////////////////////////////////////////////////////////


ActionCamMediaPlayerToolBar::ActionCamMediaPlayerToolBar(MediaPlayerControl *control, QWidget *parent)
:ToolBarBase(control, parent)
{
	
	setObjectName("MediaPlayerView_ActionCamMediaPlayerToolBar");
	m_slider->SetGrooveHeight(2);
	m_slider->SetChunkHeight(8);

	m_currentFrame = new CurrentFrameLabel(this);


	for (int i = 0; i < 6; ++i)
	{
		auto w = new QFrame(this);
		w->setObjectName("MediaPlayerView_ActionCamMediaPlayerToolBar_Separator");
		m_separatorWidgets.push_back(w);
	}

	GetButton(ToolBar_Backward)->setToolTip(tr("Previous Frame") + " (Left)");
	GetButton(ToolBar_Forward)->setToolTip(tr("Next Frame") + " (Right)");

	BaseStateButton *btnPlayPause = qobject_cast<BaseStateButton*>(GetButton(ToolBar_Play));
	btnPlayPause->GetButton(ToolBar_Pause)->setToolTip(tr("Pause") + " (Space)");
	btnPlayPause->GetButton(ToolBar_Play)->setToolTip(tr("Play") + " (Space)");
	GetButton(ToolBar_Stop)->setToolTip(tr("Stop") + " (Ctrl+/)");
}

void ActionCamMediaPlayerToolBar::Open()
{

}

void ActionCamMediaPlayerToolBar::Close()
{
	ToolBarBase::Close();
	m_currentFrame->SetCurrentFrame(0);
	m_currentFrame->SetDuration(0);
}

void ActionCamMediaPlayerToolBar::Stop()
{
	ToolBarBase::Stop();
	m_currentFrame->SetCurrentFrame(0);
}

void ActionCamMediaPlayerToolBar::SeekCallback(int frames, int accurate)
{
	if (m_slider->canSeek())
	{
		ToolBarBase::SeekCallback(frames, accurate);
		m_currentFrame->SetCurrentFrame(frames);
	}
	
}

void ActionCamMediaPlayerToolBar::SeekManual(int frames, int accurate)
{
	if (m_mediaPlayerControl->HaveSource())
	{
		ToolBarBase::SeekManual(frames, accurate);
		m_currentFrame->SetCurrentFrame(frames);
	}
	
}

void ActionCamMediaPlayerToolBar::SetDuration(int duration)
{
	ToolBarBase::SetDuration(duration);
	m_currentFrame->SetDuration(duration);
}


void ActionCamMediaPlayerToolBar::Forward()
{
	int frames = qMin(m_mediaPlayerControl->CurrentFrames() + 1, Duration());
	m_mediaPlayerControl->Seek(frames, 0);
	RulerSeekCallback(frames);

}

void ActionCamMediaPlayerToolBar::Backward()
{
	int frames = qMax(m_mediaPlayerControl->CurrentFrames() - 1, 0);
	m_mediaPlayerControl->Seek(frames, 0);
	RulerSeekCallback(frames);
}

void ActionCamMediaPlayerToolBar::ButtonClicked(int id, QPoint p)
{
	if (id == ToolBarBase::ToolBar_Backward)
	{
		m_mediaPlayerControl->Pause();
		Backward();
	}
	else if (id == ToolBarBase::ToolBar_Forward)
	{
		m_mediaPlayerControl->Pause();
		Forward();
	}
	else
	{
		ToolBarBase::ButtonClicked(id, p);
	}
}


void ActionCamMediaPlayerToolBar::EnableWidgets(bool bEnable)
{
	ToolBarBase::EnableWidgets(bEnable);
	m_currentFrame->setEnabled(bEnable);
}

void ActionCamMediaPlayerToolBar::resizeEvent(QResizeEvent *event)
{
	const int margins = 0;
	const int space = 30;
	int left = space;
	const int btnw = 14;
	const int btnh = 14;

	int top = (height() - btnh) / 2;


	GetButton(ToolBar_Backward)->setGeometry(space / 2, top, btnw, btnh);
	left = GetButton(ToolBar_Backward)->geometry().right() + space; 
	GetButton(ToolBar_Forward)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Forward)->geometry().right() + space;
	GetButton(ToolBar_Play)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Play)->geometry().right() + space;
	GetButton(ToolBar_Stop)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Stop)->geometry().right() + space;

	int sliderwidth = width() - left - 80 - space;
	m_slider->setGeometry(left, top, sliderwidth, btnh);
	left = m_slider->geometry().right() + space;
	m_currentFrame->setGeometry(left, top, sliderwidth, btnh);

	int separatorLeft = 0;
	for (int i = 0; i < 5; ++i)
	{
		m_separatorWidgets[i]->setGeometry(separatorLeft, 0, 2, height());
		separatorLeft += space + btnw - 1;
	}
	m_separatorWidgets[5]->setGeometry(left - space / 2, 0, 2, height());

}
