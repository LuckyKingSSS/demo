#include "stdafx.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "FSMatterViewPrivate.h"
#include "MVGlobal.h"

FSMatterView::FSMatterView(QWidget *parent)
	:FSBaseWidget(parent)
{
	m_p = new FSMatterViewPrivate(this);
}

void FSMatterView::AddRecordFils(QStringList files)
{
	MVGlobal::GetInstance()->ImportMediaItems(files);
}

IFSMatterViewManager * FSMatterView::GetMatterViewManager()
{
	return m_p->GetMatterViewManager();
}

void FSMatterView::resizeEvent(QResizeEvent *e)
{
	m_p->setGeometry(rect()); 
}
