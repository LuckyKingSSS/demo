#ifndef FSFULLSCREENCONTROL_H
#define FSFULLSCREENCONTROL_H

#include <QObject>
#include <QtWidgets/QtWidgets>

class FSBaseToolBar;
class QPropertyAnimation;
class QTimer;

class FSFullScreenControl : public QObject
{
    Q_OBJECT

public:
    FSFullScreenControl(QWidget *parent);
    ~FSFullScreenControl();

    void SetToolBar(QWidget *pToolBar){ mpToolBar = pToolBar; }
    void FullScreen();
    bool IsFullScreen();
    bool Restore();
    void ResizeEvent(QWidget *pDisplayWidget, QRect rect);
    void MayShowToolBar(QRect rect, QPoint p);

private:
    void StopAnimation();
    bool IsToolBarShow();
    bool IsAnimationRunning();
    void SetFullScreen(bool bFullScreen);



private:
    QWidget            *mpPlayerWidget;
    QTimer             *mpHideAnimTimer;
    QWidget            *mpToolBar;
    int                 mWflag;

    QPropertyAnimation *mpHideAnimation;
    QPropertyAnimation *mpShowAnimation;
    QPoint              mCurPoint;
    QRect               mDispplayRect;
};

#endif // FSFULLSCREENCONTROL_H
