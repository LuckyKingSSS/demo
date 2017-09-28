#ifndef FTRACKSVIEW_H
#define FTRACKSVIEW_H


#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FProject/FSequence.h"

#include "FTrackWidget.h"

class FSequence;
class FTrack;
class TimelineRuler;
class FActionLayerManager;
class FTracksViewPainterWidget;
class FTimelineMenu;

class FTracksView : public FBaseWidget
{
    Q_OBJECT

public:
    FTracksView(TimelineRuler *ruler, QWidget *parent);
    ~FTracksView();

    bool Load(FSequencePtr sequence);
    FSequence* GetSequence();

    void SetCurrentFrames(int frames);
    int GetCurrentFrames();

    int Height();
    int GetHeadWidth();
    FTrackWidgets GetTracks();

    TimelineRuler* GetRuler();

    void Resize();

private slots:
    
    void slotTrackUpdate();
    void slotTrackRebuild();
    void slotSelectionChanged();
    void slotPause();

    void slotZoomChanged(int zoom);
    void slotFramesChanged(int frames);
public:
    void RectangleSelect(QRect rect);

private:
    void BuildTracks();
    FTrackWidget* CreateTrack(FTrackPtr trackData);

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *e) override;

    void contextMenuEvent(QContextMenuEvent *event);

private:
    FSequencePtr m_sequence;
    FTrackWidgets m_tracks;
    TimelineRuler *m_pRuler;

    FActionLayerManager *m_layerManager;
    FTracksViewPainterWidget* m_painterWidget;
    FTimelineMenu *m_menu;
};


class FTracksViewPainterWidget : public QWidget
{
    Q_OBJECT
public:
    FTracksViewPainterWidget(QWidget *parent);

    void RectangleSelect(QRect rect);
    void SetCurrentFrames(int frames);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QRect m_rectangle;

    int m_curFrames;
};

#endif // FTRACKSVIEW_H
