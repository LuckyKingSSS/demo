#include "stdafx.h"
#include "FFoldWidgetItem.h"
#include "FFoldWidgetItemPrivate.h"
#include "FCollapseHeaderBase.h"

FFoldWidgetItem::FFoldWidgetItem(QWidget *parent)
	: FBaseWidget(parent)
{
	m_pFoldItemPrivate = new FFoldWidgetItemPrivate(this);
	InitConnections();
}

FFoldWidgetItem::~FFoldWidgetItem()
{

}

void FFoldWidgetItem::SetContainsWidget(FContexBaseWidget* pWidget)
{
	m_pFoldItemPrivate->SetContainsWidget(pWidget);
}

FContexBaseWidget* FFoldWidgetItem::GetItemCentralWidget()
{
	return m_pFoldItemPrivate->GetItemCentralWidget();
}

void FFoldWidgetItem::SetCustomHeader(FCollapseHeaderBase* pHeaderWidegt)
{
	m_pFoldItemPrivate->SetCustomHeader(pHeaderWidegt);
}

FCollapseHeaderBase* FFoldWidgetItem::GetItemHeader()
{
	return m_pFoldItemPrivate->GetItemHeader();
}

void FFoldWidgetItem::HideContainWidget()
{
	m_pFoldItemPrivate->HideContainWidget();
}

void FFoldWidgetItem::ShowContainWidget()
{
	m_pFoldItemPrivate->ShowContainWidget();
}

void FFoldWidgetItem::SetShowMode(ShowMode showMode)
{
	m_pFoldItemPrivate->SetShowMode(showMode);
}

void FFoldWidgetItem::SetCheckBoxName(QString strName)
{
	m_pFoldItemPrivate->SetCheckBoxName(strName);
}

void FFoldWidgetItem::SetIndex(int iIndex)
{
	m_pFoldItemPrivate->SetIndex(iIndex);
}

int FFoldWidgetItem::GetIndex()
{
	return m_pFoldItemPrivate->GetIndex();
}

void FFoldWidgetItem::SetHeaderTitleName(QString strTitleName)
{
	return m_pFoldItemPrivate->SetHeaderTitleName(strTitleName);
}

void FFoldWidgetItem::SetHeaderHeight(int iHeight)
{
	m_pFoldItemPrivate->SetHeaderHeight(iHeight);
}

void FFoldWidgetItem::resizeEvent(QResizeEvent *event)
{
	m_pFoldItemPrivate->resize(width(), height());
}

void FFoldWidgetItem::InitConnections()
{
	connect(m_pFoldItemPrivate, &FFoldWidgetItemPrivate::sigOnCollapse, this, &FFoldWidgetItem::sigOnCollapse);
	connect(m_pFoldItemPrivate, &FFoldWidgetItemPrivate::sigResize, this, [&](int iWidth, int iHeight){
		resize(iWidth, iHeight);
	});
}

