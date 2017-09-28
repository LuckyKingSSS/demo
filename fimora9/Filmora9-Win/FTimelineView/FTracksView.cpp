#include "stdafx.h"
#include "FTracksView.h"
#include "FTrackWidget.h"
#include "FActionLayer.h"
#include "FTimelineMenu.h"
#include "inc_CommonLib/TimelineRuler.h"

FTracksView::FTracksView(TimelineRuler *ruler, QWidget *parent)
: FBaseWidget(parent)
{
    setAcceptDrops(true);

    m_pRuler = ruler;
    m_layerManager = new FActionLayerManager(this);

    m_painterWidget = new FTracksViewPainterWidget(this);
    m_painterWidget->SetCurrentFrames(m_pRuler->CurrentFrames() + GetHeadWidth());

    m_menu = new FTimelineMenu(this);

    connect(m_pRuler, &TimelineRuler::ZoomChanged, this, &FTracksView::slotZoomChanged);
    connect(m_pRuler, &TimelineRuler::FramesChanged, this, &FTracksView::slotFramesChanged);
}

FTracksView::~FTracksView()
{

}

bool FTracksView::Load(FSequencePtr sequence)
{
    m_sequence = sequence;

    connect(m_sequence, &FSequence::sigTrackUpdate, this, &FTracksView::slotTrackUpdate);
    connect(m_sequence, &FSequence::sigTrackRebuild, this, &FTracksView::slotTrackRebuild);
    connect(m_sequence, &FSequence::sigSelectionChanged, this, &FTracksView::slotSelectionChanged);
    connect(m_sequence, &FSequence::sigPause, this, &FTracksView::slotPause);

    BuildTracks();




    return true;
}

FSequence* FTracksView::GetSequence()
{
    return m_sequence;
}

void FTracksView::SetCurrentFrames(int frames)
{
    m_pRuler->SetCurrentFrames(frames);
    slotFramesChanged(frames);
}

int FTracksView::GetCurrentFrames()
{
    return m_pRuler->CurrentFrames();
}

int FTracksView::Height()
{
    return 300;
}

int FTracksView::GetHeadWidth()
{
    return 100;
}

FTrackWidgets FTracksView::GetTracks()
{
    return m_tracks;
}

TimelineRuler* FTracksView::GetRuler()
{
    return m_pRuler;
}

void FTracksView::Resize()
{
    resizeEvent(nullptr);
}

void FTracksView::slotTrackUpdate()
{
    update();
}

void FTracksView::slotTrackRebuild()
{
    BuildTracks();
}

void FTracksView::slotSelectionChanged()
{
    update();

}

void FTracksView::slotPause()
{

}

void FTracksView::slotZoomChanged(int zoom)
{
    m_painterWidget->SetCurrentFrames(m_pRuler->Frames2PositionF(m_pRuler->CurrentFrames()) + GetHeadWidth());
    update();
}

void FTracksView::slotFramesChanged(int frames)
{
    GetSequence()->SetCurrentFrames(frames);
    m_painterWidget->SetCurrentFrames(m_pRuler->Frames2PositionF(frames) + GetHeadWidth());
}

void FTracksView::RectangleSelect(QRect rect)
{
    m_painterWidget->RectangleSelect(rect);
}

void FTracksView::BuildTracks()
{
    qApp->processEvents();
    qDeleteAll(m_tracks);
    m_tracks.clear();
    auto tracks = m_sequence->GetTracks();
    for (auto track : tracks)
    {
        FTrackWidget* pTrack = new FTrackWidget(track, this);
        pTrack->SetRuler(m_pRuler);
        pTrack->SetHeadWidth(GetHeadWidth());
        pTrack->show();
        m_tracks.push_back(pTrack);

    }
    Resize();
}

FTrackWidget* FTracksView::CreateTrack(FTrackPtr trackData)
{
    FTrackWidget* pTrack = new FTrackWidget(trackData, this);
    //pTrack->SetRuler(m_pRuler);
    pTrack->show();
    return pTrack;
}

void FTracksView::resizeEvent(QResizeEvent *event)
{
    int left = 0;
    int top = 0;
    for (auto track : m_tracks)
    {
        track->setGeometry(left, top, width(), track->Height());
        top = track->geometry().bottom();
    }

    m_painterWidget->setGeometry(rect());
    m_painterWidget->raise();
    //m_pLine->setGeometry(TRACK_WIDGET_HEADER_WIDTH - 1, 0, 1, height());

    //m_pSelectionWidget->raise();
    //m_pSelectionWidget->setGeometry(rect());
}

void FTracksView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    m_layerManager->paintEvent(&painter, rect(), this, event);

}

void FTracksView::mousePressEvent(QMouseEvent *event)
{
    m_layerManager->mousePressEvent(this, event);
}

void FTracksView::mouseMoveEvent(QMouseEvent *event)
{
    m_layerManager->mouseMoveEvent(this, event);

}

void FTracksView::mouseReleaseEvent(QMouseEvent *event)
{
    m_layerManager->mouseReleaseEvent(this, event);

}

void FTracksView::dragEnterEvent(QDragEnterEvent *event)
{
    m_layerManager->dragEnterEvent(this, event);

}

void FTracksView::dragMoveEvent(QDragMoveEvent *event)
{
    m_layerManager->dragMoveEvent(this, event);

}

void FTracksView::dragLeaveEvent(QDragLeaveEvent *event)
{
    m_layerManager->dragLeaveEvent(this, event);

}

void FTracksView::dropEvent(QDropEvent *event)
{
    m_layerManager->dropEvent(this, event);

}

void FTracksView::contextMenuEvent(QContextMenuEvent *event)
{
    m_menu->Popup(event->pos());
}

//////////////////////////////////////////////////////////////////////////
FTracksViewPainterWidget::FTracksViewPainterWidget(QWidget *parent)
:QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    m_curFrames = 0;
}

void FTracksViewPainterWidget::RectangleSelect(QRect rect)
{
    m_rectangle = rect;
    update();
}



void FTracksViewPainterWidget::SetCurrentFrames(int frames)
{
    m_curFrames = frames;
    update();
}

void FTracksViewPainterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!m_rectangle.isNull())
    {
        painter.drawRect(m_rectangle);
    }

    QPen pen = painter.pen();
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawLine(QPoint(m_curFrames, 0), QPoint(m_curFrames, height()));
}
