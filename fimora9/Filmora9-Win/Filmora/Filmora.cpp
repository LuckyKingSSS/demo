#include "stdafx.h"
#include "Filmora.h"

#include "inc_FTimelineView/FTimelineView.h"

#include "inc_FProject/FNLETimelineFactory.h"
#include "inc_FProject/FProjectFactory.h"

#include "inc_FProject/FDateDefine.h"

Filmora::Filmora(QWidget *parent)
    : QWidget(parent)
{

    setMinimumSize(600, 600);

    m_matterView = new QWidget(this);
    m_playerView = new QWidget(this); 
    
    m_timelineView = new FTimelineView(this);

    {
        FProjectFactory::GetInstance()->CreateProject();
        auto sequence = FNLETimelineFactory::GetInstance()->CreateSequence();
        sequence->InsertTrack(0);
        sequence->InsertTrack(0);
        sequence->InsertTrack(0);

        auto timeine = FNLETimelineFactory::GetInstance()->CreateTimeline("C:\\Users\\Public\\Videos\\Sample Videos\\video\\12.mp4");
        sequence->AddTimeline(0, 0, timeine);
        
        timeine = timeine->clone();
        sequence->AddTimeline(1, 500, timeine);

        timeine = timeine->clone();
        sequence->AddTimeline(2, 1000, timeine);

        m_timelineView->Load(sequence);
    }

}

Filmora::~Filmora()
{
    FPROJECT_FACTORY->DestoryProject(FCURRENT_PROJECT);
}

void Filmora::resizeEvent(QResizeEvent *event)
{
    int left = 0;
    int top = 0;
    m_matterView->setGeometry(left, top, width() / 2, height() / 2);
    left = m_matterView->geometry().right();
    m_playerView->setGeometry(left, top, width() / 2, height() / 2);
    left = 0;
    top = m_matterView->geometry().bottom();
    m_timelineView->setGeometry(left, top, width(), height() - top);

}
