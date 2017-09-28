#include "stdafx.h"
#include "FMatterViewPrivate.h"
#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FProject/FMediaData.h"
#include "FMatterViewMgr.h"
#include "FMatterListView.h"
#include "FMatterIconView.h"
#include "FMatterBottomBar.h"


FMatterViewPrivate::FMatterViewPrivate(QWidget *parent)
	: FBaseWidget(parent)
{
	m_pMgr = new FMatterViewMgr(this);
	m_pListView = new FMatterListView(this);
	m_pIconView = new FMatterIconView(this);
	m_pBottomBar = new FMatterBottomBar(this);

	connect(m_pBottomBar, &FMatterBottomBar::sigMatterTypeChanged, this, &FMatterViewPrivate::slotMatterTypeChanged);
	connect(m_pBottomBar, &FMatterBottomBar::sigExport, this, &FMatterViewPrivate::sigExport);
	connect(m_pListView, &FMatterListView::sigFoldStateChanged, this, [&](){
		resizeEvent(nullptr);
	});
}

FMatterViewPrivate::~FMatterViewPrivate()
{
	
}

void FMatterViewPrivate::Load(FMediaData* pMediaData)
{
	m_pMgr->Load(pMediaData);
	//m_pListView->AddFolder("MY ALBUM", 2);
	//m_pListView->AddFolder("ddddddddddddddddddddd", 3);
	m_pListView->InitCategory(m_pMgr->GetCategory());
	//m_pListView->InsertCategory("MY ALBUM", 2, 0);
	//m_pListView->InsertCategory("ddddddddddddddddddddd", 3, 1);
}

void FMatterViewPrivate::resizeEvent(QResizeEvent *)
{
	int left = 0, top = 0;
	const int BottomBarHeight = 60;
	int ListViewWidth = m_pListView->WidthHint();
	m_pListView->setGeometry(left, top, ListViewWidth, height() - BottomBarHeight);
	left = m_pListView->geometry().right() + 1;
	m_pIconView->setGeometry(left, top, width() - left, height() - BottomBarHeight);
	left = 0;
	top = m_pListView->geometry().bottom() + 1;
	m_pBottomBar->setGeometry(left, top, width(), BottomBarHeight);
}

void FMatterViewPrivate::slotMatterTypeChanged(MatterType matterType)
{
	m_pMgr->SetMatterType(matterType);
}
