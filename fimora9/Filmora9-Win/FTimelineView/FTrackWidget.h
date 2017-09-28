#ifndef FTRACKWIDGET_H
#define FTRACKWIDGET_H


#include "inc_FProject/FTrack.h"
#include "inc_FCommonLib/FBaseWidget.h"

class TimelineRuler;
class FPaintLayerManager;
class FTrackWidgetHead;

class FTrackWidget : public FBaseWidget
{
    Q_OBJECT

public:
    FTrackWidget(FTrackPtr track, QWidget *parent);
    ~FTrackWidget();

    int Height();
    int GetHeadWidth();
    void SetHeadWidth(int width);

    FTrackPtr GetTrack();
    int GetIndex();

    void SetRuler(TimelineRuler *ruler);
    TimelineRuler* Ruler();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    FTrackPtr m_track;
    TimelineRuler *m_pRuler;
    FPaintLayerManager *layerManager;
    FTrackWidgetHead *m_headWidget;
    int m_headWidth;
};

typedef QList<FTrackWidget*> FTrackWidgets;


class FLabel;

class FTrackWidgetHead : public QWidget
{
    Q_OBJECT

public:
    FTrackWidgetHead(QWidget *parent);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    FLabel *m_icon;

};






#endif // FTRACKWIDGET_H
