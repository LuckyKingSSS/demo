#include "stdafx.h"
#include "FEditPlayerMgr.h"
#include "FEditRectangle.h"
#include <QPainter>

FEditPlayerMgr::FEditPlayerMgr(QRectF playerRegion, QObject *parent /* = 0*/)
	:QObject(parent)
	, m_PlayerRegion(playerRegion)
{

}

FEditPlayerMgr::~FEditPlayerMgr()
{

}

void FEditPlayerMgr::SetTimelines(QList<FNLETimeline*> timelines)
{
	m_Timelines = timelines;
	m_Rects.clear();  // clear
	for (auto pTimeline : timelines){
		//pTimeline->GetType();
		FEditPIP *pRect = new FEditPIP(pTimeline, m_PlayerRegion, this);
		connect(pRect, &FEditRectangleBase::sigCursorShapeChanged, this, &FEditPlayerMgr::sigCursorShapeChanged);
		m_Rects.append(pRect);
	}
}

void FEditPlayerMgr::SetPlayerRegion(const QRectF& rc)
{
	m_PlayerRegion = rc;
	for (auto pRect : m_Rects){
		pRect->SetPlayerRegion(rc);
	}
}

void FEditPlayerMgr::DoMousePressEvent(QMouseEvent* e)
{
	for (auto pRect : m_Rects){
		pRect->DoMousePressEvent(e);
	}
}

void FEditPlayerMgr::DoMouseMoveEvent(QMouseEvent* e)
{
	for (auto pRect : m_Rects){
		pRect->DoMouseMoveEvent(e);
	}
}

void FEditPlayerMgr::DoPaintEvent(QPainter* painter, QPaintEvent* e /*= nullptr*/)
{
	for (auto pRect : m_Rects){
		pRect->DoPaintEvent(painter, e);
	}
}

void FEditPlayerMgr::DrawRectangles(QPainter* p, const QPointF&)
{

}

void FEditPlayerMgr::Init()
{

}

