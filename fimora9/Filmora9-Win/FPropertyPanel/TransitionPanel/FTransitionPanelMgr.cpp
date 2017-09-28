#include "stdafx.h"
#include "FTransitionPanelMgr.h"
#include "inc_FProject/FNLETimeline.h"

FTransitionPanelMgr::FTransitionPanelMgr(QObject *parent)
: FPanelMgr(parent)
, m_mode(None_Mode)
{

}

FTransitionPanelMgr::~FTransitionPanelMgr()
{

}

void FTransitionPanelMgr::SetDuration(int duration)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetStreamLength(duration);
}

int FTransitionPanelMgr::GetDuration()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	return pTimeline->GetDuration();
}

void FTransitionPanelMgr::SetTransitionMode(TransitionMode mode)
{
	m_mode = mode;
}

FTransitionPanelMgr::TransitionMode FTransitionPanelMgr::GetTransitionMode()
{
	return m_mode;
}




