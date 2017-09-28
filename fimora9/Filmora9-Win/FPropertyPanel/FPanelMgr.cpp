#include "stdafx.h"
#include "FPanelMgr.h"

FPanelMgr::FPanelMgr(QObject *parent)
: QObject(parent)
, m_pTimeline(0)
, m_pRootTimeline(0)
{

}

FPanelMgr::~FPanelMgr()
{

}

void FPanelMgr::SetTimeline(FNLETimeline *timeline)
{
	m_pTimeline = timeline;
}

FNLETimeline* FPanelMgr::GetTimeline()
{
	return m_pTimeline;
}

void FPanelMgr::SetRootTimeline(FNLETimeline *timeline)
{
	m_pRootTimeline = timeline;
}

FNLETimeline* FPanelMgr::GetRootTimeline()
{
	return m_pRootTimeline;
}





