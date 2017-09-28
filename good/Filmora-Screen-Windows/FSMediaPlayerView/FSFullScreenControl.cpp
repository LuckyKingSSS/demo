#include "stdafx.h"
#include "FSBaseToolBar.h"
#include "FSFullScreenControl.h"


FSFullScreenControl::FSFullScreenControl(QWidget *parent)
    : QObject(parent)
    , mpToolBar(nullptr)
{
    
    mpPlayerWidget = parent;
    SetFullScreen(false);
    mWflag = mpPlayerWidget->windowFlags();
    mpHideAnimation = new QPropertyAnimation(this);
    mpHideAnimation->setDuration(500);

    mpHideAnimTimer = new QTimer(this);
    mpHideAnimTimer->setSingleShot(true);
    mpHideAnimTimer->setInterval(500);
    connect(mpHideAnimTimer, &QTimer::timeout, this, [&](){
        QPoint start = mpToolBar->pos();
        QPoint end = start + QPoint(0, mpToolBar->height());
        mpHideAnimation->setTargetObject(mpToolBar);
        mpHideAnimation->setPropertyName(QByteArray("pos"));
        mpHideAnimation->setStartValue(start);
        mpHideAnimation->setEndValue(end);
        mpHideAnimation->start();
    });

    mpShowAnimation = new QPropertyAnimation(this);
    mpShowAnimation->setDuration(100);
}

FSFullScreenControl::~FSFullScreenControl()
{

}

void FSFullScreenControl::FullScreen()
{
    if (IsFullScreen())
    {
        SetFullScreen(false);
        mpPlayerWidget->setWindowFlags((Qt::WindowFlags)mWflag);
        mpPlayerWidget->showNormal();
        StopAnimation();
    }
    else
    {
        SetFullScreen(true);
        auto fullscreen = QApplication::desktop()->screenGeometry();       
        mpPlayerWidget->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);  
        mpPlayerWidget->showFullScreen();
        mpPlayerWidget->setGeometry(fullscreen);       
        mpPlayerWidget->setVisible(true);
        mpPlayerWidget->activateWindow();
    }
}

bool FSFullScreenControl::Restore()
{
    bool reg = IsFullScreen();
    if (reg)
    {
        mpPlayerWidget->setWindowFlags((Qt::WindowFlags)mWflag);
        mpPlayerWidget->showNormal();
        StopAnimation();
        SetFullScreen(false);
    }  
    return reg;
}

void FSFullScreenControl::ResizeEvent(QWidget *pDisplayWidget, QRect rect)
{
    // 是全屏时进行隐藏动画
    if (IsFullScreen() && mpToolBar)
    {
        pDisplayWidget->setGeometry(rect);
        mpHideAnimTimer->start();
    }
}

void FSFullScreenControl::MayShowToolBar(QRect rect, QPoint p)
{
    // 是全屏时进行显示动画
    if (IsFullScreen() && mpToolBar)
    {
        mCurPoint = p;
        mDispplayRect = rect;
        const int hitHeight = mpToolBar->height() * 2;
        QRect hitRect(0, mDispplayRect.height() - hitHeight, mDispplayRect.width(), hitHeight);
        if (hitRect.contains(mCurPoint))
        {
            mpHideAnimTimer->stop();
            if (!IsAnimationRunning()&&!IsToolBarShow())
            {
                QPoint start = mpToolBar->pos();
                QPoint end   = start - QPoint(0, mpToolBar->height());
                mpShowAnimation->setTargetObject(mpToolBar);
                mpShowAnimation->setPropertyName(QByteArray("pos"));
                mpShowAnimation->setStartValue(start);
                mpShowAnimation->setEndValue(end);
                mpShowAnimation->start();
            }
        }
        else
        {
            if (!IsAnimationRunning() && IsToolBarShow())
            {
                mpHideAnimTimer->start();
            }
        }
    }
}

bool FSFullScreenControl::IsFullScreen()
{
    return mpPlayerWidget->property("FullScreen").toBool();
}

void FSFullScreenControl::StopAnimation()
{
    mpHideAnimTimer->stop();
    mpHideAnimation->stop();
}

bool FSFullScreenControl::IsToolBarShow()
{
    if (mpToolBar)
    {
        auto r1 = mpToolBar->geometry();
        auto r2 = mDispplayRect;
        auto ret = r2.contains(r1);
        return mDispplayRect.contains(mpToolBar->geometry());
    }
    return false;
}

bool FSFullScreenControl::IsAnimationRunning()
{
    return mpHideAnimation->state() == QAbstractAnimation::Running ||
        mpShowAnimation->state() == QAbstractAnimation::Running;
}

void FSFullScreenControl::SetFullScreen(bool bFullScreen)
{
    mpPlayerWidget->setProperty("FullScreen", bFullScreen);
}

