#include "stdafx.h"
#include "FTypeSettingWnd.h"
#include "FSettingDataContr.h"

FTypeSettingWnd::FTypeSettingWnd(QWidget *parent)
	: FBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	initUI();
	initConnections();
}

FTypeSettingWnd::~FTypeSettingWnd()
{

}

void FTypeSettingWnd::initUI()
{
	m_pDataControl = new FSettingDataContr(nullptr);
	QList<QString> ConfigList =  m_pDataControl->getDeviceConfig();
	m_ListCtrl = new FListCtrl(this);
	m_ListCtrl->setHeader(tr("Formate"));
	m_ListCtrl->setListData(ConfigList);
}

void FTypeSettingWnd::paintEvent(QPaintEvent *event)
{

}

void FTypeSettingWnd::initConnections()
{
	connect(m_ListCtrl, &FListCtrl::sigItemSelect, this, [&](QString strType){
		emit sigItemSelect(strType);
	});
}

void FTypeSettingWnd::resizeEvent(QResizeEvent *event)
{
	m_ListCtrl->setGeometry(55, 20, width() - 55, height() - 20);
}
