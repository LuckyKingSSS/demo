#include "stdafx.h"
#include "FTimelineMenu.h"
#include "FTracksView.h"

#include "inc_CommonLib/TimelineRuler.h"

FTimelineMenu::FTimelineMenu(QObject *parent)
    : QObject(parent)
{
    m_tracksView = qobject_cast<FTracksView*>(parent);
    Q_ASSERT(m_tracksView);


    m_pActDelete = new QAction(this);
    m_pActDelete->setShortcut(QKeySequence::Delete);


    connect(m_pActDelete, &QAction::triggered, this, &FTimelineMenu::slotDelete);

}

FTimelineMenu::~FTimelineMenu()
{

}

void FTimelineMenu::Popup(QPoint pos)
{
    QMenu menu(m_tracksView);

    auto child = qobject_cast<FTrackWidget*>(m_tracksView->childAt(pos));
    if (child)
    {
        int frames = m_tracksView->GetRuler()->Position2FramesF(pos.x() - m_tracksView->GetHeadWidth());
        auto pressTimeline = child->GetTrack()->GetTimeline(frames);

        menu.addAction(m_pActDelete);
        m_pActDelete->setEnabled(pressTimeline);


    }


    RetranslateUi();
    menu.exec(QCursor::pos());
}

void FTimelineMenu::RetranslateUi()
{
    m_pActDelete->setText(tr("Delete"));

}

void FTimelineMenu::slotDelete()
{
    if (m_tracksView->GetSequence()->CanRemove())
        m_tracksView->GetSequence()->RemoveClips();

}
