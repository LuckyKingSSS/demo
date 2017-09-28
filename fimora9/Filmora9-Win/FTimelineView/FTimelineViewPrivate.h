#ifndef FTIMELINEVIEWPRIVATE_H
#define FTIMELINEVIEWPRIVATE_H

#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FProject/FSequence.h"

class FSequence;
class FToolBar;
class FTracksView;
class FRuler;

class FTimelineViewPrivate : public FBaseWidget
{
    Q_OBJECT

public:
    FTimelineViewPrivate(QWidget *parent);
    ~FTimelineViewPrivate();

    bool Load(FSequencePtr sequence);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void slotScroolBarHValueChanged(int value);
    void slotScroolBarVValueChanged(int value);

    void slotToolButtonClicked(int type);
private:
    FToolBar *m_pToolBar;
    FRuler *m_pRuler;
    FTracksView *m_pTrackView;

    QScrollBar *m_pScroolBarH;
    QScrollBar *m_pScroolBarV;


    int m_nRealTopPosition;

};

#endif // FTIMELINEVIEWPRIVATE_H
