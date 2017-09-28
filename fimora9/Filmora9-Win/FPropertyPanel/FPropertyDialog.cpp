#include "stdafx.h"
#include "FPropertyDialog.h"

FPropertyDialog::FPropertyDialog(FNLETimeline *timeline, QWidget *parent)
	: FPropertyDialog(timeline, nullptr, parent)

{
	
}

FPropertyDialog::FPropertyDialog(FNLETimeline *timeline, FNLETimeline *rootTimeline, QWidget *parent)
	: FBaseDialog(parent)
	, m_timeline(timeline)
	, m_rootTimeline(rootTimeline)
{
	setAttribute(Qt::WA_StyledBackground);
}

FPropertyDialog::~FPropertyDialog()
{

}

FNLETimeline* FPropertyDialog::GetTimeline()
{
    return m_timeline;
}

FNLETimeline* FPropertyDialog::GetRootTimeline()
{
    return m_rootTimeline;
}

void FPropertyDialog::SetTimeline(FNLETimeline *timeline)
{
    m_timeline = timeline;
}

void FPropertyDialog::SetRootTimeline(FNLETimeline *timeline)
{
    m_rootTimeline = timeline;
}

void FPropertyDialog::resizeEvent(QResizeEvent *event)
{
	FBaseDialog::resizeEvent(event);
}
