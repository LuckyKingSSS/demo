#include "stdafx.h"
#include "FExportLocalPanel.h"
#include "FTypeSettingWnd.h"
#include "FExportSettingWnd.h"

FExportLocalPanel::FExportLocalPanel(QWidget *parent)
	: FBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	initUI();
	initConnections();
}

FExportLocalPanel::~FExportLocalPanel()
{

}

void FExportLocalPanel::initUI()
{
	m_pStyleSettingWnd = new FTypeSettingWnd(this);
	m_pStyleSettingWnd->setObjectName("FTypeSettingWnd_Panel");
	m_pExportSettingWnd = new FExportSettingWnd(this);
	m_pExportSettingWnd->setObjectName("FExportSettingWnd_Panel");
}

void FExportLocalPanel::initConnections()
{
	connect(m_pStyleSettingWnd, &FTypeSettingWnd::sigItemSelect, m_pExportSettingWnd, &FExportSettingWnd::slotOnItemSelect);
}

void FExportLocalPanel::resizeEvent(QResizeEvent *event)
{
	m_pStyleSettingWnd->setGeometry(0, 0, 315, height());
	m_pExportSettingWnd->setGeometry(315, 0, width() - 315, height());
}
