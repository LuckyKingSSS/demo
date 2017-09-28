#include "stdafx.h"
#include "FOverlayPanel.h"
#include "FPropertySlider.h"
#include "FPropertyCommon.h"
#include "inc_FProject/FNLETimeline.h"

FOverlayPanel::FOverlayPanel(QWidget *parent)
	: FPropertyPanel(parent)
{
	initUI();
}

FOverlayPanel::~FOverlayPanel()
{

}

void FOverlayPanel::Init()
{

}

void FOverlayPanel::Show()
{

}

void FOverlayPanel::Hide()
{

}

void FOverlayPanel::SetEnabled(bool enabled)
{

}

void FOverlayPanel::Destory()
{

}

void FOverlayPanel::Update()
{

}

void FOverlayPanel::slotAlphaValueChanged(int iValue)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::Render::kRender_Alpha, iValue);
}

void FOverlayPanel::initUI()
{
	m_AlphaSlider = new FPropertySlider(this);
	m_BottomBar = new BottomButtonBar(this);
	m_BottomBar->setFixedHeight(30);
	m_BottomBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_BottomBar->HideBtnByID(BottomButtonBar::Advanced_Btn);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_AlphaSlider);
	vLayout->addWidget(m_BottomBar);
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->setSpacing(210);
	setLayout(vLayout);
}
