#include "stdafx.h"
#include "FPaintLayer.h"

#include "inc_CommonLib/TimelineRuler.h"

#include "FTrackWidget.h"

FPaintLayerManager::FPaintLayerManager(QObject *parent)
:FLayerManager(parent)
{
    AddLayer(new FTimelinePaintLayer(this));
}


bool FPaintLayerManager::paintEvent(QPainter *painter, QRect rect, FTrackPtr track, FTrackWidget *trackWidget, QPaintEvent *event)
{
    for (auto layer : GetLayers())
    {
        bool accept = qobject_cast<FPaintLayer*>(layer)->paintEvent(painter,rect, track, trackWidget, event);
        if (accept)
            break;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
FPaintLayer::FPaintLayer(QObject *parent)
: FLayer(parent)
{

}

bool FPaintLayer::paintEvent(QPainter *painter, QRect rect, FTrackPtr track, FTrackWidget *trackWidget, QPaintEvent *event)
{


    return true;
}

//////////////////////////////////////////////////////////////////////////
FTimelinePaintLayer::FTimelinePaintLayer(QObject *parent)
:FPaintLayer(parent)
{

}

bool FTimelinePaintLayer::paintEvent(QPainter *painter, QRect rect, FTrackPtr track, FTrackWidget *trackWidget, QPaintEvent *event)
{
    PainterRestore painterRestore(painter);

    TimelineRuler *pRuler = trackWidget->Ruler();
    //QRect rect = trackWidget->rect();
    int offset = rect.left();
    QColor normalBorderColor(150, 150, 150);
    QColor selectedBorderColor(97, 222, 208);
    int top = 0;

    for (auto timeline : track->GetTimelines())
    {
        auto left = pRuler->Frames2PositionF(timeline->GetStartAtTrack()) + offset;
        auto width = pRuler->Frames2DurationF(timeline->GetLenAtTrack());
        //w = qMax(2, w);
        int height = rect.height() - 1;
        QRect rcTimeline(left, top, width, height);
        //painter->fillRect(rcClip.adjusted(-1,0,0,0), clrClipBk);

        QPen pen = painter->pen();
        pen.setWidth(2);
        if (timeline->GetSelected())
            pen.setColor(selectedBorderColor);
        else
            pen.setColor(normalBorderColor);

        painter->setPen(pen);
        //painter->setBrush(timeline->GetBackgroundColor());
        //painter->drawRect(rcTimeline.adjusted(-1, 0, 0, 0));
        painter->drawRect(rcTimeline);
    }

    for (auto timeline : track->GetDragTimelines())
    {
        auto left = pRuler->Frames2PositionF(timeline.GetStartAtTrack()) + offset;
        auto width = pRuler->Frames2DurationF(timeline.GetLenAtTrack());

        int height = rect.height() - 1;
        QRect rcTimeline(left, top, width, height);

        QPen pen = painter->pen();
        pen.setWidth(2);
        pen.setColor(QColor(255, 255, 0));

        painter->setPen(pen);
        painter->drawRect(rcTimeline);
    }
    return true;
}
