#include "stdafx.h"
#include "FAudioMixerDialogMgr.h"
#include "inc_FProject/FNLETimeline.h"

FAudioMixerDialogMgr::FAudioMixerDialogMgr(QObject *parent)
	: FPanelMgr(parent)
{

}

FAudioMixerDialogMgr::~FAudioMixerDialogMgr()
{

}

int FAudioMixerDialogMgr::GetDuration()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	return pTimeline->GetDuration();
}

void FAudioMixerDialogMgr::Stop()
{
	
}

void FAudioMixerDialogMgr::Play()
{

}

void FAudioMixerDialogMgr::Pause()
{

}




