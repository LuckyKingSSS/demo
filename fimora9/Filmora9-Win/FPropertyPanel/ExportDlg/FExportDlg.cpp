#include "stdafx.h"
#include "FExportDlg.h"
#include "TopLevelWindowTitleBar.h"
#include "CommonWidgets.h"

FExportDlg::FExportDlg(QWidget *parent)
	: FPropertyDialog(nullptr,parent)
{
	setAttribute(Qt::WA_StyledBackground);
	resize(957, 574);
	setFixedSize(957, 574);
	initUI();
}

FExportDlg::~FExportDlg()
{

}
 
void FExportDlg::initUI()
{
	SetTitle(tr("Output"));
	m_pExportDlgPrivate = new FExportDlgPrivate(this);
	m_titlebar->setObjectName("FExportDlg_m_titlebar");
	m_titlebar->GetButton(TopLevelWindowTitleBar::Button::Close)->setObjectName("FExportDlg_mCloseBtn");
}

void FExportDlg::resizeEvent(QResizeEvent *event)
{
	FPropertyDialog::resizeEvent(event);
}

int FExportDlg::getWndTitleHeight()
{
	return m_titlebar->height();
}

void FExportDlg::paintEvent(QPaintEvent *event)
{
	//由于窗口标题栏在创建后做了resize操作，需要到paintEvent获取实际的高度
	int iOffsetLen = getWndTitleHeight();
	m_pExportDlgPrivate->setGeometry(0, iOffsetLen, width(), height() - iOffsetLen);
}
