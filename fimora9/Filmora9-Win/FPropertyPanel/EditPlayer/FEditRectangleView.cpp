#include "stdafx.h"
#include "FEditRectangleView.h"
#include "FEditPlayerMgr.h"

FEditRectangleView::FEditRectangleView(FEditPlayerMgr *mgr, QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
	, m_pMgr(mgr)
{
	setMouseTracking(true);
	setAttribute(Qt::WA_TranslucentBackground, true);
	connect(m_pMgr, &FEditPlayerMgr::sigCursorShapeChanged, this, [&](const QCursor& cursor){
		setCursor(cursor);
	});
	connect(m_pMgr, &FEditPlayerMgr::sigUpdate, this, [&](){
		update();
	});
}

FEditRectangleView::~FEditRectangleView()
{

}

//void FEditRectangleView::SetTimelines(QList<FNLETimeline*> timelines)
//{
//	m_TimelinesLst = timelines;
//}

void FEditRectangleView::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	m_pMgr->DoPaintEvent(&p, e);
}

void FEditRectangleView::mousePressEvent(QMouseEvent *e)
{
	m_pMgr->DoMousePressEvent(e);
	update();
}

void FEditRectangleView::mouseMoveEvent(QMouseEvent *e)
{
	m_pMgr->DoMouseMoveEvent(e);
	update();
}

void FEditRectangleView::mouseReleaseEvent(QMouseEvent *)
{

}
