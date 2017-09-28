#include "stdafx.h"
#include "FAdvanceVedioDlg.h"
#include "FColorSettingWnd.h"
#include "FPreviewPlayer.h"
#include "FAdvanceVedioMgr.h"


FAdvanceVedioDlg::FAdvanceVedioDlg(FNLETimeline* timeline, QWidget *parent) : FPropertyDialog(timeline, parent), m_Timeline(timeline)
{
	InitUI();
	InitData();
	InitConnections();
}

FAdvanceVedioDlg::FAdvanceVedioDlg(FNLETimeline *timeline, FNLETimeline *rootTimeline, QWidget *parent) : FPropertyDialog(timeline, rootTimeline, parent),m_Timeline(timeline)
{
	InitUI();
	InitData();
	InitConnections();
}

FAdvanceVedioDlg::~FAdvanceVedioDlg()
{

}

void FAdvanceVedioDlg::InitUI()
{
	m_titlebar->setObjectName("FAdvanceVedioDlg_m_titlebar");
	m_titlebar->GetButton(TopLevelWindowTitleBar::Button::Close)->setObjectName("FAdvanceVedioDlg_mCloseBtn");
	int iTitleBarHeight = m_titlebar->height();
	m_ColorSettingWnd = new FColorSettingWnd(nullptr);
	m_ColorSettingWnd->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	const int iFixeWidth = 265;
	m_ColorSettingWnd->setFixedWidth(iFixeWidth);
	m_PreviewPlayer = new FPreviewPlayer(nullptr);
	m_DataMgr = new FAdvanceVedioMgr(nullptr);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(m_ColorSettingWnd);
	hLayout->addWidget(m_PreviewPlayer);
	hLayout->setContentsMargins(0, iTitleBarHeight, 0, 0);
	setLayout(hLayout);
}

void FAdvanceVedioDlg::InitData()
{
	m_ColorSettingWnd->SetTimeline(m_Timeline);
	m_PreviewPlayer->SetTimeline(m_Timeline);
}

void FAdvanceVedioDlg::InitConnections()
{
	connect(m_ColorSettingWnd, &FColorSettingWnd::sigOnTimelineChange, m_PreviewPlayer, &FPreviewPlayer::slotOnTimelineChange);
}
