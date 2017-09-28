#include "stdafx.h"
#include "CommonWidgets.h"
#include "FPropertyDialog.h"
#include "FPowerToolDlg.h"
#include "FPreViewMediaPlayer.h"
#include "FPowerToolMgr.h"
#include "FPowerToolSettingWidget.h"


FPowerToolDlg::FPowerToolDlg(QWidget *parent)
	: FPropertyDialog(nullptr,parent)
{
	InitUI();
	InitConnections();
}

FPowerToolDlg::~FPowerToolDlg()
{

}

void FPowerToolDlg::InitUI()
{
	m_titlebar->setObjectName("FPowerToolSetting_m_titlebar");
	m_titlebar->GetButton(TopLevelWindowTitleBar::Button::Close)->setObjectName("FPowerToolSetting_mCloseBtn");
	int iTitleBarHeight = m_titlebar->height();
	m_Player = new FPowerToolPlayer(nullptr, nullptr);
	m_PowerToolMgr = new FPowerToolMgr(nullptr);
	m_PowertoolWidget = new FPowerToolSettingWidget(nullptr);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(m_Player);
	hLayout->addWidget(m_PowertoolWidget);
	hLayout->setStretchFactor(m_Player,7);
	hLayout->setStretchFactor(m_PowertoolWidget, 3);
	hLayout->setContentsMargins(0, iTitleBarHeight, 0, 0);
	hLayout->setSpacing(0);
	setLayout(hLayout);
}

void FPowerToolDlg::InitConnections()
{

}
