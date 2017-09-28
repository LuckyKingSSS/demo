#ifndef FACTIONLAYER_H
#define FACTIONLAYER_H


#include "inc_FProject/FTrack.h"

#include "FLayer.h"

class FTrackWidget;
class FTracksView;
class FActionLayerManager : public FLayerManager
{
    Q_OBJECT

public:
    FActionLayerManager(QObject *parent);

    bool mousePressEvent(FTracksView *trackView, QMouseEvent *event);
    bool mouseMoveEvent(FTracksView *trackView, QMouseEvent *event);
    bool mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event);

    bool dragEnterEvent(FTracksView *trackView, QDragEnterEvent *event);
    bool dragMoveEvent(FTracksView *trackView, QDragMoveEvent *event);
    bool dragLeaveEvent(FTracksView *trackView, QDragLeaveEvent *event);
    bool dropEvent(FTracksView *trackView, QDropEvent *event);

    bool paintEvent(QPainter *painter, QRect rect, FTracksView *trackView, QPaintEvent *event);

private:
    
};


class FActionLayer : public FLayer
{
    Q_OBJECT

public:

    virtual bool mousePressEvent(FTracksView *trackView, QMouseEvent *event);
    virtual bool mouseMoveEvent(FTracksView *trackView, QMouseEvent *event);
    virtual bool mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event);

    virtual bool dragEnterEvent(FTracksView *trackView, QDragEnterEvent *event);
    virtual bool dragMoveEvent(FTracksView *trackView, QDragMoveEvent *event);
    virtual bool dragLeaveEvent(FTracksView *trackView, QDragLeaveEvent *event);
    virtual bool dropEvent(FTracksView *trackView, QDropEvent *event);

    virtual bool paintEvent(QPainter *painter, QRect rect, FTracksView *trackView, QPaintEvent *event);

protected:
    FActionLayer(QObject *parent);

};



class FSelectionLayer : public FActionLayer
{
    Q_OBJECT

public:
    FSelectionLayer(QObject *parent);

    bool mousePressEvent(FTracksView *trackView, QMouseEvent *event) override;
    bool mouseMoveEvent(FTracksView *trackView, QMouseEvent *event) override;
    bool mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event) override;

    //bool paintEvent(QPainter *painter, QRect rect, FTracksView *trackView, QPaintEvent *event) override;
    
protected:
    bool m_pressDown;
    QPoint m_pressPoint;

    FNLETimelinePtr m_pressTimeline;

    QRect m_selectedRect;
    bool m_bDecideSelectedNow;
};


class FMoveTimelineLayer : public FActionLayer
{
    Q_OBJECT

public:
    FMoveTimelineLayer(QObject *parent);

    bool mousePressEvent(FTracksView *trackView, QMouseEvent *event) override;
    bool mouseMoveEvent(FTracksView *trackView, QMouseEvent *event) override;
    bool mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event) override;

    bool dragEnterEvent(FTracksView *trackView, QDragEnterEvent *event) override;
    bool dragMoveEvent(FTracksView *trackView, QDragMoveEvent *event) override;
    bool dragLeaveEvent(FTracksView *trackView, QDragLeaveEvent *event) override;
    bool dropEvent(FTracksView *trackView, QDropEvent *event)  override;

protected:
    bool m_pressDown;
    QPoint m_pressPoint;

    FNLETimelinePtr m_pressTimeline;

};

//
//class FTrimTimelineLayer : public FActionLayer
//{
//    Q_OBJECT
//
//public:
//    FTrimTimelineLayer(QObject *parent);
//
//    bool mousePressEvent(FTracksView *trackView, QMouseEvent *event) override;
//    bool mouseMoveEvent(FTracksView *trackView, QMouseEvent *event) override;
//    bool mouseReleaseEvent(FTracksView *trackView, QMouseEvent *event) override;
//
//    bool dragEnterEvent(FTracksView *trackView, QDragEnterEvent *event) override;
//    bool dragMoveEvent(FTracksView *trackView, QDragMoveEvent *event) override;
//    bool dragLeaveEvent(FTracksView *trackView, QDragLeaveEvent *event) override;
//    bool dropEvent(FTracksView *trackView, QDropEvent *event)  override;
//
//protected:
//    void MoveTrim(QPoint point, FTrackWidget *item);
//    bool IsTriming();
//    void HintTriming(QPoint point, FTrackWidget *item, int hintMargins = 3);
//    QPoint GetTrimRange(bool bLeftSide);
//
//protected:
//    bool m_pressDown;
//    QPoint m_pressPoint;
//    bool m_bTriming;
//    bool m_bLeftDrag;
//    bool m_bRightDrag;
//
//    FNLETimelinePtr m_pHitClip;
//
//};

#endif // FACTIONLAYER_H
