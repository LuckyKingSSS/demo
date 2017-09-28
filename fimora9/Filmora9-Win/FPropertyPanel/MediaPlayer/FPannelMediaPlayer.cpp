#include "stdafx.h"
#include "..\include\inc_CommonLib\externalapi.h"
#include "inc_MediaPlayerView\MediaPlayerView.h"
#include "inc_MediaPlayerView\MediaPlayerControl.h"
#include "inc_FCommonLib\FCommonWidgets.h"
#include "inc_FProject/FNLETimeline.h"
#include "FMediaPlayerBarBase.h"
#include "FPannelMediaPlayer.h"
#include "CommonWidgets.h"




FPannelMediaPlayer::FPannelMediaPlayer(QWidget *parent)
	: FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FPannelMediaPlayer::~FPannelMediaPlayer()
{

}

IMediaPlayerControl* FPannelMediaPlayer::GetMediaPlayerControl()
{
	return m_Player->GetMediaPlayerControl();
}


void FPannelMediaPlayer::SetDuration(int iDurationTime)
{
	m_PlayToolbar->SetDuration(iDurationTime);
}

void FPannelMediaPlayer::SetCurPlayTime(int iCurTime)
{
	m_PlayToolbar->SetCurPlayTime(iCurTime);
}

void FPannelMediaPlayer::resizeEvent(QResizeEvent *event)
{
 
}

void FPannelMediaPlayer::InitUI()
{
	m_Player = new MediaPlayerView(nullptr);
	m_PlayToolbar = new FPannelMediaPlayerBar(m_Player->GetMediaPlayerControl(), nullptr);
	const int iLeftOff = 5;
	const int iRightOff = 15;
	const int iTopOff = 25;
	const int iBottomOff = 20;
	const int iToolBarheight = 30;
	QVBoxLayout* vLayout = new QVBoxLayout();
	m_Player->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_PlayToolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_PlayToolbar->setFixedHeight(iToolBarheight);

	vLayout->addWidget(m_Player);
	vLayout->addWidget(m_PlayToolbar);
	vLayout->setContentsMargins(iLeftOff, iTopOff, iRightOff, iBottomOff);
	vLayout->setSpacing(35);
	setLayout(vLayout);
}

void FPannelMediaPlayer::InitConnections()
{

}

FPannelMediaPlayerBar::FPannelMediaPlayerBar(IMediaPlayerControl *control, QWidget *parent) :FMediaPlayerBarBase(control, parent)
{
	InitUI();
	InitConnections();
}

void FPannelMediaPlayerBar::SetDuration(int iDurationTime)
{
	ToolBarSlider* toolBarSlider = GetToolBarSlider();
	toolBarSlider->setDuration(iDurationTime);
	QString strTotalTime = gFrame2String(iDurationTime);
	int iIndex = strTotalTime.lastIndexOf(".");
	strTotalTime = strTotalTime.mid(0, iIndex);
	m_TotalTimeLabel->setText(strTotalTime);
}

void FPannelMediaPlayerBar::SetCurPlayTime(int iCurTime)
{
	QString CurTime = gFrame2String(iCurTime);
	int iIndex = CurTime.lastIndexOf(".");
	CurTime = CurTime.mid(0, iIndex);
	m_PastTimeLabel->setText(CurTime);
}

void FPannelMediaPlayerBar::InitUI()
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	//获取播放按钮
	QMap<int,BaseButton*> allBtns = GetButtons();
	QMap<int, BaseButton*>::iterator iter;
	iter = allBtns.find(ToolBarButton::ToolBar_Play);
	BaseButton* playBtn = *iter;
	playBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	playBtn->setFixedSize(20, 15);
	hLayout->addWidget(playBtn);

	iter = allBtns.find(ToolBarButton::ToolBar_Stop);
	BaseButton* stopBtn = *iter;
	stopBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	stopBtn->setFixedSize(20, 15);
	stopBtn->show();
	hLayout->addWidget(stopBtn);

	iter = allBtns.find(ToolBarButton::ToolBar_Backward);
	BaseButton* preBtn = *iter;
	preBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	preBtn->setFixedSize(20, 15);
	preBtn->show();
	hLayout->addWidget(preBtn);

	iter = allBtns.find(ToolBarButton::ToolBar_Forward);
	BaseButton* ForwardBtn = *iter;
	ForwardBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	ForwardBtn->setFixedSize(20, 15);
	ForwardBtn->show();
	hLayout->addWidget(ForwardBtn);

	ToolBarSlider* toolBarSlider = GetToolBarSlider();
	toolBarSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	toolBarSlider->setFixedHeight(20);

	QHBoxLayout* hTimeLabelLayout = new QHBoxLayout();
	m_PastTimeLabel = new FLabel(nullptr);
	m_PastTimeLabel->setText("00:00:00");
	m_PastTimeLabel->setFixedWidth(55);

	m_SeparateLabel = new FLabel(nullptr);
	m_SeparateLabel->setText("/");
	m_SeparateLabel->setFixedWidth(15);

	m_TotalTimeLabel = new FLabel(nullptr);
	m_TotalTimeLabel->setText("00:00:00");
	m_TotalTimeLabel->setFixedWidth(55);
	hTimeLabelLayout->addWidget(m_PastTimeLabel);
	hTimeLabelLayout->addWidget(m_SeparateLabel);
	hTimeLabelLayout->addWidget(m_TotalTimeLabel);
	hTimeLabelLayout->setContentsMargins(0, 0, 0, 0);
	hTimeLabelLayout->setSpacing(0);
	
	hLayout->addWidget(toolBarSlider);
	hLayout->addLayout(hTimeLabelLayout);
	hLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(hLayout);
}

void FPannelMediaPlayerBar::InitConnections()
{

}
