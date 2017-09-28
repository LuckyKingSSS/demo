#include "stdafx.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "FCropAndZoomPanel.h"
#include "PowerToolDlg\FPreViewMediaPlayer.h"

FCropAndZoomPanel::FCropAndZoomPanel(FNLETimeline *timeline, QWidget *parent)
	: FPropertyDialog(timeline,parent)
{
	InitUI();
	InitConnections();
}

FCropAndZoomPanel::~FCropAndZoomPanel()
{

}

void FCropAndZoomPanel::SetTimeline(FNLETimeline* timeline)
{
	m_PreViewPlayer->SetTimeline(timeline);
}

void FCropAndZoomPanel::InitUI()
{
	m_titlebar->setObjectName("FCropAndZoomPanel_m_titlebar");
	m_titlebar->GetButton(TopLevelWindowTitleBar::Button::Close)->setObjectName("FCropAndZoomPanel_mCloseBtn");
	int iTitleBarHeight = m_titlebar->height();
	m_CropLabel = new FLabel(nullptr);
	m_CropLabel->setText(tr("<u>Crop</u>"));
	m_PanZoomLabel = new FLabel(nullptr);
	m_PanZoomLabel->setText(tr("<u>Pan&Zoom</u>"));

	
	m_PreViewPlayer = new FCropPlayer(nullptr, nullptr);
	m_ConstrainLabel = new FLabel(nullptr);
	m_ConstrainLabel->setText(tr("Constrain:"));
	m_ManuallyBtn = new FRadioButton(nullptr);
	m_ManuallyBtn->setText(tr("Manually"));
	m_SixteenToNineBtn = new FRadioButton(nullptr);
	m_SixteenToNineBtn->setText(tr("16:9"));
	m_FourToThreeBtn = new FRadioButton(nullptr);
	m_FourToThreeBtn->setText(tr("4:3"));
	 
	m_HorizonPixels = new FLineEdit(nullptr);
	m_CrossLabel = new FLabel(nullptr);
	m_CrossLabel->setText("x");
	m_CrossLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	m_VerticalPixels = new FLineEdit(nullptr);

	m_ResetBtn = new FButton(nullptr);
	m_ResetBtn->setText(tr("Reset"));
	m_OKBtn = new FButton(nullptr);
	m_OKBtn->setText(tr("OK"));
	m_CancleBtn = new FButton(nullptr);
	m_CancleBtn->setText(tr("Cancle"));

	QHBoxLayout* hHeaderLayout = new QHBoxLayout();
	hHeaderLayout->addStretch();
	hHeaderLayout->addWidget(m_CropLabel);
	m_CropLabel->setFixedSize(QSize(35,20));
	m_PanZoomLabel->setFixedSize(QSize(90, 20));
	hHeaderLayout->addWidget(m_PanZoomLabel);
	hHeaderLayout->addStretch();
	hHeaderLayout->setContentsMargins(420, 5, 400, 20);
	hHeaderLayout->setSpacing(30);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addLayout(hHeaderLayout);
	//m_PreViewPlayer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_PreViewPlayer->setFixedSize(910, 600);
	m_PreViewPlayer->SetSliderWidth(910);
	vLayout->addWidget(m_PreViewPlayer);

	QHBoxLayout* hParamLayout = new QHBoxLayout();
	hParamLayout->addWidget(m_ConstrainLabel);
	m_ConstrainLabel->setFixedSize(QSize(70,20));
	hParamLayout->addWidget(m_ManuallyBtn);
	m_SixteenToNineBtn->setFixedSize(QSize(70, 20));
	hParamLayout->addWidget(m_SixteenToNineBtn);
	m_FourToThreeBtn->setFixedSize(QSize(70, 20));
	hParamLayout->addWidget(m_FourToThreeBtn);

	QHBoxLayout* hPixLayout = new QHBoxLayout();
	hPixLayout->setContentsMargins(0, 0, 0, 0);
	hPixLayout->setSpacing(2);
	m_HorizonPixels->setFixedSize(QSize(40, 20));
	hPixLayout->addWidget(m_HorizonPixels);
	m_CrossLabel->setFixedSize(QSize(20, 20));
	hPixLayout->addWidget(m_CrossLabel);
	m_VerticalPixels->setFixedSize(QSize(40, 20));
	hPixLayout->addWidget(m_VerticalPixels);
	hParamLayout->addLayout(hPixLayout);
	hParamLayout->addStretch();
	hParamLayout->setContentsMargins(0,0,0,0);
	hParamLayout->setSpacing(50);
	vLayout->addLayout(hParamLayout);

	QHBoxLayout* hBtnLayout = new QHBoxLayout();
	hBtnLayout->addWidget(m_ResetBtn);
	QWidget* emptyWidget = new QWidget(nullptr);
	hBtnLayout->addWidget(emptyWidget);
	emptyWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_OKBtn->setFixedSize(QSize(75,30));
	hBtnLayout->addWidget(m_OKBtn);
	m_CancleBtn->setFixedSize(QSize(75, 30));
	hBtnLayout->addWidget(m_CancleBtn);
	vLayout->addLayout(hBtnLayout);
	vLayout->setContentsMargins(2, iTitleBarHeight, 2, 15);
	vLayout->setSpacing(0);
	setLayout(vLayout);
}

void FCropAndZoomPanel::InitConnections()
{
	connect(m_CancleBtn, &FButton::clicked, this, [&](){
		emit signalCancel();
	});

	connect(m_OKBtn, &FButton::clicked, this, [&](){

		emit signalOK();
	});
}
