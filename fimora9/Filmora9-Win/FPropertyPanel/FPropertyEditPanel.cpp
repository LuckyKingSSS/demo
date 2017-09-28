#include "stdafx.h"
#include "FPropertyEditPanel.h"

FPropertyEditPanel::FPropertyEditPanel(QWidget *parent)
: FPropertyPanel(parent)
{

}

FPropertyEditPanel::~FPropertyEditPanel()
{

}

QObject* FPropertyEditPanel::GetTimelineManager()
{
    return m_timelineManager;
}

void FPropertyEditPanel::GetTimelineManager(QObject *manager)
{
    m_timelineManager = manager;
}
