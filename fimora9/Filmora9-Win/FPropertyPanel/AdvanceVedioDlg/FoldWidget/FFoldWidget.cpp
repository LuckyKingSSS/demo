#include "stdafx.h"
#include "FFoldWidget.h"
#include "FFoldWidgetPrivate.h"
#include "FFoldWidgetItem.h"

FFoldWidget::FFoldWidget(QWidget *parent)
	: FBaseWidget(parent)
{
	Init();
	InitConnections();
}

FFoldWidget::~FFoldWidget()
{

}

void FFoldWidget::AddItem(FFoldWidgetItem* item)
{
	m_FoldWidgetPrivate->AddItem(item);
}

void FFoldWidget::AddItems(const QList<FFoldWidgetItem*> items)
{
	m_FoldWidgetPrivate->AddItems(items);
}

void FFoldWidget::InsertItem(int iRow, FFoldWidgetItem* item)
{
	m_FoldWidgetPrivate->InsertItem(iRow, item);
}

void FFoldWidget::InsertItems(int iRow, const QList<FFoldWidgetItem*> items)
{
	m_FoldWidgetPrivate->InsertItems(iRow, items);
}

void FFoldWidget::SetShowMode(bool bShowMode)
{
	m_FoldWidgetPrivate->SetShowMode(bShowMode);
}

FFoldWidgetItem* FFoldWidget::TakeItem(int iRow)
{
	return m_FoldWidgetPrivate->TakeItem(iRow);
}

FFoldWidgetItem* FFoldWidget::Item(int iRow) const
{
	return m_FoldWidgetPrivate->Item(iRow);
}

void FFoldWidget::resizeEvent(QResizeEvent *event)
{
	m_FoldWidgetPrivate->resize(width(), height());
}

void FFoldWidget::Init()
{
	m_FoldWidgetPrivate = new FFoldWidgetPrivate(this);
}

void FFoldWidget::InitConnections()
{
	connect(m_FoldWidgetPrivate, &FFoldWidgetPrivate::sigUpdateSize, this, &FFoldWidget::sigUpdateSize);
}
