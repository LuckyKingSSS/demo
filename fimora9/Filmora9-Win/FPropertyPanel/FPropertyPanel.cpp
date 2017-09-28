#include "stdafx.h"
#include "FPropertyPanel.h"
#include "inc_FProject/FNLETimeline.h"

FPropertyPanel::FPropertyPanel(QWidget *parent)
:FUpdateBaseWidget(parent)
{

}

FNLETimeline* FPropertyPanel::GetTimeline()
{
    Q_ASSERT(m_timeline);
    return m_timeline;
}

FNLETimeline* FPropertyPanel::GetRootTimeline()
{
    Q_ASSERT(m_rootTimeline);
    return m_rootTimeline;
}


void FPropertyPanel::Init()
{

}

void FPropertyPanel::Show()
{

}

void FPropertyPanel::Hide()
{

}

void FPropertyPanel::SetEnabled(bool enabled)
{

}

void FPropertyPanel::Destory()
{

}

void FPropertyPanel::Update()
{

}

void FPropertyPanel::SetTimeline(FNLETimeline *timeline)
{
	m_timeline = timeline;
}

void FPropertyPanel::SetRootTimeline(FNLETimeline *timeline)
{
	m_rootTimeline = timeline;
}
