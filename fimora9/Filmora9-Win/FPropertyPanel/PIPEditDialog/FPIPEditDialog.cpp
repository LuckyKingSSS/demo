#include "stdafx.h"
#include "FPIPEditDialog.h"
#include "inc_CommonLib/externalapi.h"

FPIPEditDialog::FPIPEditDialog(QWidget *parent)
	: FPropertyDialog(nullptr,parent)
{
	setFixedSize(QSize(880, 600));
	setAttribute(Qt::WA_StyledBackground);
	m_titlebar->setObjectName("FPIPEditDialog_TitleBar");

	m_pMgr = new FPIPEditDialogMgr(this);
	m_pSettingPanel = new FPIPEditSettingPanel(this);
	m_pPlayer = new FPIPEditPlayer(this);

	m_pSettingPanel->SetMaskThumbFile(m_pMgr->GetMaskThumbFile());

	ConnectSignals();
	QString s = gGetStyleSheet(":/FPropertyPanel/FPropertyPanel.css");
	setStyleSheet(gGetStyleSheet(":/FPropertyPanel/FPropertyPanel.css"));
}

FPIPEditDialog::~FPIPEditDialog()
{

}

void FPIPEditDialog::resizeEvent(QResizeEvent *event)
{
	FPropertyDialog::resizeEvent(event);
	int left, top;
	left = 0, top = m_titlebar->geometry().bottom();
	m_pSettingPanel->setGeometry(left, top, 0.4*width(), height() - top);
	left = m_pSettingPanel->geometry().right();
	m_pPlayer->setGeometry(left, top, width() - left, height() - top);
}

void FPIPEditDialog::showEvent(QShowEvent *)
{
	m_pPlayer->Open(1, GetTimeline());
}

void FPIPEditDialog::ConnectSignals()
{
	connect(m_pPlayer, &FPIPEditPlayer::sigOk, this, [&](){  // ok 会保存当前设置
		close();
		emit sigOk();
	});
	connect(m_pPlayer, &FPIPEditPlayer::sigCancel, this, [&](){ // cancel 会清除当前设置
		close();
		emit sigCancel();
	});
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigMaskItemDoubleClicked, this, [&](int i){
		m_pMgr->SetTimeline(GetTimeline());
		bool bPlaying = m_pPlayer->IsPlaying();
		if(bPlaying) m_pPlayer->Pause();
		m_pMgr->SetMaskEffect(i);
		m_pPlayer->Update();
		if (bPlaying) m_pPlayer->Play();
	});
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigMaskWidthChanged, m_pMgr, &FPIPEditDialogMgr::SetMaskWidth);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigMaskHeightChanged, m_pMgr, &FPIPEditDialogMgr::SetMaskHeight);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigMaskLeftChanged, m_pMgr, &FPIPEditDialogMgr::SetMaskLeft);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigMaskTopChanged, m_pMgr, &FPIPEditDialogMgr::SetMaskTop);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigIntensityLevelChanged, m_pMgr, &FPIPEditDialogMgr::SetIntensityLevel);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigShadowDistanceChanged, m_pMgr, &FPIPEditDialogMgr::SetShadowDistance);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigShadowBlurChanged, m_pMgr, &FPIPEditDialogMgr::SetShadowBlur);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigShadowTransparencyChanged, m_pMgr, &FPIPEditDialogMgr::SetShadowTransparency);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigShadowColorChanged, m_pMgr, &FPIPEditDialogMgr::SetShadowColor);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigShadowDirectionChanged, m_pMgr, &FPIPEditDialogMgr::SetShadowDirection);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigBorderDistanceChanged, m_pMgr, &FPIPEditDialogMgr::SetBorderDistance);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigBorderBlurChanged, m_pMgr, &FPIPEditDialogMgr::SetBorderBlur);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigBorderTransparencyChanged, m_pMgr, &FPIPEditDialogMgr::SetBorderTransparency);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigBorderGradientColorStartChanged, m_pMgr, &FPIPEditDialogMgr::SetBorderGradientColorStart);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigBorderGradientColorEndChanged, m_pMgr, &FPIPEditDialogMgr::SetBorderGradientColorEnd);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigBorderGradientDirectionChanged, m_pMgr, &FPIPEditDialogMgr::SetBorderGradientDirection);
	connect(m_pSettingPanel, &FPIPEditSettingPanel::sigRotateChanged, this, [&](FPropertyRotate::RotateMode mode){
		m_pMgr->SetRotate(mode);
		m_pPlayer->Update();
	});
}


