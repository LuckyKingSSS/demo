#include "stdafx.h"
#include "inc_FMatterView/FMatterView.h"
#include "FMatterViewPrivate.h"
#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_CommonLib/externalapi.h"

#include <QHBoxLayout>

FMatterView::FMatterView(QWidget *parent /*= nullptr*/)
	:FBaseWidget(parent)
{
	m_pImpl = new FMatterViewPrivate;
	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->setMargin(0);
	pLayout->addWidget(m_pImpl);
	setMinimumSize(QSize(600, 300));
	connect(m_pImpl, SIGNAL(FMatterViewPrivate::sigExport), this, SLOT(FMatterView::sigExport));
	setStyleSheet(gGetStyleSheet(":/FMatterView/Resources/FMatterView.qss"));
}

FMatterView::~FMatterView()
{

}

void FMatterView::Load(FMediaData* pMediaData)
{
	m_pImpl->Load(pMediaData);
}
