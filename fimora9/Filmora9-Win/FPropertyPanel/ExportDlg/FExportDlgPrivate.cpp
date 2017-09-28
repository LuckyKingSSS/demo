#include "stdafx.h"
#include "FExportDlgPrivate.h"
#include "FExportBar.h"
#include "FExportLocalPanel.h"

FExportDlgPrivate::FExportDlgPrivate(QWidget *parent)
	: FBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	initUI();
}

FExportDlgPrivate::~FExportDlgPrivate()
{

}

void FExportDlgPrivate::initUI()
{
	m_ExportBar = new FExportBar(this);
	m_ExportBar->setObjectName("FExportBarID");
	m_ExportBar->setStyleSheet("background-color: red");
	m_ExportSettingWidget = new QStackedWidget(this);
	FExportLocalPanel* formatePanel = new FExportLocalPanel(nullptr);
	FExportLocalPanel* exportLocalPanel = new FExportLocalPanel(nullptr);
	m_ExportSettingWidget->addWidget(formatePanel);
	m_ExportSettingWidget->addWidget(exportLocalPanel);
	m_ExportSettingWidget->setCurrentWidget(formatePanel);
	m_ExportSettingWidget->setContentsMargins(0, 0, 0, 0);
}

void FExportDlgPrivate::paintEvent(QPaintEvent *event)
{
	
}

void FExportDlgPrivate::resizeEvent(QResizeEvent *event)
{
	m_ExportBar->setGeometry(0, 0, 90, height());
	m_ExportSettingWidget->setGeometry(90, 0, width() - 90, height());
}
