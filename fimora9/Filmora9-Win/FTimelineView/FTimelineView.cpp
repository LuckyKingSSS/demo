#include "stdafx.h"
#include "inc_FTimelineView/FTimelineView.h"

#include "FTimelineViewPrivate.h"

FTimelineView::FTimelineView(QWidget *parent)
:QWidget(parent)
{
    m_p = new FTimelineViewPrivate(this);


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(m_p);
    vlayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vlayout);
}

bool FTimelineView::Load(FSequencePtr sequence)
{
    return m_p->Load(sequence);
}
