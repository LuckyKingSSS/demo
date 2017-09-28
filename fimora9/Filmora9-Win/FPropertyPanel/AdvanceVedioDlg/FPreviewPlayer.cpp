#include "stdafx.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "inc_MediaPlayerView\MediaPlayerControl.h"
#include "..\include\inc_CommonLib\externalapi.h"
#include "inc_MediaPlayerView\MediaPlayerView.h"
#include "FPreviewPlayerHeader.h"
#include "FPreviewPlayer.h"
#include "FPannelMediaPlayer.h"
#include "inc_FProject/FNLETimeline.h"

FPreviewPlayer::FPreviewPlayer(QWidget *parent)
	: FBaseWidget(parent)
{
	InitUI();
	InitData();
	InitConnections();
}

FPreviewPlayer::~FPreviewPlayer()
{

}

void FPreviewPlayer::SetTimeline(FNLETimeline* timeline)
{
	m_Timeline = timeline;
	Q_ASSERT(m_Timeline != NULL);
	IMediaPlayerControl* mediaPlayerControl = m_MediaPlayer->GetMediaPlayerControl();
	Q_ASSERT(mediaPlayerControl != NULL);
	mediaPlayerControl->Open(1, m_Timeline);
    //获取多媒体文件的时长
	int iDurationTime = mediaPlayerControl->Duration();  //获取的是帧数
	m_MediaPlayer->SetDuration(iDurationTime);
}

void FPreviewPlayer::slotOnTimelineChange()
{
	m_MediaPlayer->GetMediaPlayerControl()->Update();
}

void FPreviewPlayer::InitUI()
{
	gSetMainWindow(this);
	m_pheader = new FPreviewPlayerHeader(nullptr);
	//m_pheader->setStyleSheet("background-color: red");
	m_MediaPlayer = new FPannelMediaPlayer(nullptr);
	m_SaveBtn = new FButton(nullptr);
	m_SaveBtn->setText(tr("Save as Preset"));
	m_SaveBtn->setFixedSize(QSize(170,26));
	m_OkBtn = new FButton(nullptr);
	m_OkBtn->setText(tr("OK"));
	m_OkBtn->setFixedSize(QSize(75,25));
	m_CancelBtn = new FButton(nullptr);
	m_CancelBtn->setText(tr("Cancle"));
	m_CancelBtn->setFixedSize(QSize(75, 25));

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_pheader);
	//m_pheader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pheader->setFixedHeight(40);

	vLayout->addWidget(m_MediaPlayer);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setContentsMargins(5,10,10,10);
	hLayout->setSpacing(15);
	hLayout->addWidget(m_SaveBtn);
	hLayout->addStretch();
	hLayout->addWidget(m_OkBtn);
	hLayout->addWidget(m_CancelBtn);
	vLayout->addLayout(hLayout);
	vLayout->setContentsMargins(0,0,0,0);
	vLayout->setSpacing(2);
	setLayout(vLayout);
}

void FPreviewPlayer::InitConnections()
{
	IMediaPlayerControl* mediaPlayerControl = m_MediaPlayer->GetMediaPlayerControl();
	connect(mediaPlayerControl, &IMediaPlayerControl::PlayingCallback, this, [&](int iValue){
		m_MediaPlayer->SetCurPlayTime(iValue);
	});

	connect(mediaPlayerControl, &IMediaPlayerControl::SeekFinishCallback, this, [&](int iValue){
		m_MediaPlayer->SetCurPlayTime(iValue);
	});
}

void FPreviewPlayer::InitData()
{
	
}
