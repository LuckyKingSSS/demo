#include "stdafx.h"
#include "FPreViewMediaPlayer.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "..\include\inc_MediaPlayerView\MediaPlayerView.h"
#include "FMediaPlayerBarBase.h"
#include "..\CropAndZoom\CropPlayerView.h"


FPreViewMediaPlayer::FPreViewMediaPlayer(QWidget *parent)
	: FBaseWidget(parent)
{

}

FPreViewMediaPlayer::~FPreViewMediaPlayer()
{

}

FCommonPlayerBase::FCommonPlayerBase(IMediaPlayerControl *control, QWidget *parent) :FMediaPlayerBarBase(control,parent)
{
	InitUI();
	InitConnections();
}

void FCommonPlayerBase::SetPlayer(QWidget* player)
{
	if (m_Player != nullptr)
	{
		delete m_Player;
		m_Player = nullptr;
	}
	m_Player = player;
	m_Player->resize(500, 700);
	repaint();
}

void FCommonPlayerBase::SetSliderWidth(int iWidth)
{
	m_ToolBarSlider->setFixedWidth(iWidth);
}

void FCommonPlayerBase::SetSliderHeight(int iHeight)
{
	m_ToolBarSlider->setFixedHeight(iHeight);
}

void FCommonPlayerBase::SetMediaToSliderSpace(int iSpace)
{

}

void FCommonPlayerBase::InitUI()
{
	m_PastTimeLabel = new FLabel(nullptr);
	m_PastTimeLabel->setText("00:00:00");
	m_PastTimeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_PastTimeLabel->setFixedSize(53, 17);

	m_SeparatorLabel = new FLabel(nullptr);
	m_SeparatorLabel->setText("/");
	m_SeparatorLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_SeparatorLabel->setFixedSize(5, 17);

	m_TotalTimeLabel = new FLabel(nullptr);
	m_TotalTimeLabel->setText("00:00:00");
	m_TotalTimeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_TotalTimeLabel->setFixedSize(60, 17);

	//创建播放器
	m_playerWidget = new FBaseWidget(nullptr);
	m_playerWidget->resize(500, 700);
	//playerWidget->setStyleSheet("background-color: red");
	m_Player = new MediaPlayerView(m_playerWidget);
	m_Player->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_Player->resize(500, 700);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_playerWidget);
	//空白窗口设置间隔
	QWidget* emptyWidget = new QWidget(nullptr);
	emptyWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	emptyWidget->setFixedSize(535, 45);
	vLayout->addWidget(emptyWidget);

	m_ToolBarSlider = GetToolBarSlider();
	m_ToolBarSlider->setFixedSize(QSize(width(), 30));
	vLayout->addWidget(m_ToolBarSlider);

	//toolBar->show();
	QHBoxLayout* hLayout = new QHBoxLayout();
	//获取播放按钮
	m_AllBtns = GetButtons();
	QMap<int, BaseButton*>::iterator iter;
	iter = m_AllBtns.find(ToolBarButton::ToolBar_Play);
	BaseButton* playBtn = *iter;
	playBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	playBtn->setFixedSize(20, 15);
	hLayout->addWidget(playBtn);
	iter = m_AllBtns.find(ToolBarButton::ToolBar_Stop);
	BaseButton* stopBtn = *iter;
	stopBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	stopBtn->setFixedSize(20, 15);
	stopBtn->show();
	hLayout->addWidget(stopBtn);

	connect(stopBtn, &BaseButton::clicked, this, [&](){
		QMessageBox box;
		box.setText("111");
		box.show();
	});

	connect(playBtn, &BaseButton::clicked, this, [&](){
		QMessageBox box;
		box.setText("222");
		box.show();
	});

	hLayout->addStretch();
	hLayout->addWidget(m_PastTimeLabel);
	hLayout->addWidget(m_SeparatorLabel);


	hLayout->addWidget(m_TotalTimeLabel);
	hLayout->setContentsMargins(0, 0, 5, 0);
	hLayout->setSpacing(0);
	vLayout->addLayout(hLayout);
	vLayout->setContentsMargins(15, 75, 20, 90);
	vLayout->setSpacing(0);
	setLayout(vLayout);
}

void FCommonPlayerBase::InitConnections()
{

}


FPowerToolPlayer::FPowerToolPlayer(IMediaPlayerControl *control, QWidget *parent) :FCommonPlayerBase(control, parent)
{

}

FPowerToolPlayer::~FPowerToolPlayer()
{

}

void FPowerToolPlayer::resizeEvent(QResizeEvent *event)
{

}

FCropPlayer::FCropPlayer(IMediaPlayerControl *control, QWidget *parent) :FCommonPlayerBase(control, parent)
{

}

void FCropPlayer::SetTimeline(FNLETimeline* timeLine)
{
	FBaseWidget* playerWidget = GetPlayerWidget();
	m_CropPlayer = new CropPlayerView(timeLine, playerWidget);
	SetPlayer(m_CropPlayer);
	repaint();
}

void FCropPlayer::resizeEvent(QResizeEvent *event)
{

}
