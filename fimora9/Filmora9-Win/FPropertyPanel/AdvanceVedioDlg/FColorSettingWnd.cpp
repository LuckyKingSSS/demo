#include "stdafx.h"
#include "FColorSettingWnd.h"
#include "FPresentWnd.h"
#include "FColorAdjustPanel.h"

FColorSettingWnd::FColorSettingWnd(QWidget *parent)
	: FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FColorSettingWnd::~FColorSettingWnd()
{

}

void FColorSettingWnd::SetTimeline(FNLETimeline* timeline)
{
	m_PresentWnd->SetTimeline(timeline);
	m_AdjustWnd->SetTimeline(timeline);
}

void FColorSettingWnd::slotOnTabChanged(AdjustType type)
{
	m_StackWidget->setCurrentIndex(type);
}

void FColorSettingWnd::InitUI()
{
    m_PresentBtn = new FButton(nullptr);
	m_PresentBtn->setText(tr("Preset"));
	m_AdjsutBtn = new FButton(nullptr);
	m_AdjsutBtn->setText(tr("Adjust"));
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setContentsMargins(40,10,40,10);
	hLayout->setSpacing(80);
	hLayout->addWidget(m_PresentBtn);
	hLayout->addWidget(m_AdjsutBtn);

	m_StackWidget = new QStackedWidget();
	m_PresentWnd = new FPresentWnd(nullptr);
	m_AdjustWnd = new FColorAdjustPanel(nullptr);

	m_StackWidget->addWidget(m_PresentWnd);
	m_StackWidget->addWidget(m_AdjustWnd);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addLayout(hLayout);
	vLayout->addWidget(m_StackWidget,1);
	m_StackWidget->setCurrentIndex(1);
	vLayout->setContentsMargins(1,1,1,1);
	vLayout->setSpacing(5);
	setLayout(vLayout);
}

void FColorSettingWnd::InitConnections()
{
	connect(m_PresentBtn, &FButton::clicked, this, [&](){
		slotOnTabChanged(type_Preset);
	});

	connect(m_AdjsutBtn, &FButton::clicked, this, [&](){
		slotOnTabChanged(type_Adjust);
	});
	
	connect(m_AdjustWnd, &FColorAdjustPanel::sigTimelineChanged, this, &FColorSettingWnd::sigOnTimelineChange);
}

void FColorSettingWnd::InitData()
{

}
