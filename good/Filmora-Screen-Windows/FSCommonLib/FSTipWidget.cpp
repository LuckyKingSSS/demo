#include "stdafx.h"
#include "inc_FSCommonlib/FSTipWidget.h"
#include <QTimer>

FSTipWidget::FSTipWidget()
    : mpParent(NULL)
    , mbEnter(false)
    , mnTransparent(0.9)
    , mCountNum(0)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAlignment(Qt::AlignCenter);
    setWordWrap(false);
    mpTimer = new QTimer(this);
    connect(mpTimer, &QTimer::timeout, this, &FSTipWidget::OnTimer);
    setObjectName("FSTipWidget");
    installEventFilter(this);
}

FSTipWidget::~FSTipWidget()
{
    deleteLater();
}


void FSTipWidget::SetMesseage(const QString &msg, const QPoint* pPoint /*= NULL*/)
{
    if (msg.isEmpty())
        return;
    QFontMetrics fm1(font());
    setFixedSize(fm1.width(msg) + 30, 32);
    setText(msg);
    if (NULL != pPoint)
    {
        move(*pPoint);
    }
    setVisible(true);
    mnTransparent = 0.9;
    mpTimer->start(20);
}

void FSTipWidget::OnTimer()
{
    if (mbEnter)
    {
        return;
    }
    mCountNum++;
    mnTransparent =0.9-1.0*(mCountNum*mCountNum)/11111;
    if (mnTransparent > 0)
    {     
        setWindowOpacity(mnTransparent);
    }
    else
    {
        mpTimer->stop();
        setVisible(false);
        mCountNum = 0;
    }
}

void FSTipWidget::enterEvent(QEvent *)
{
    mbEnter = true;
    mCountNum = 0;
    mnTransparent = 0.9;
    setWindowOpacity(mnTransparent);
}

void FSTipWidget::leaveEvent(QEvent *)
{
    mbEnter = false;
}

bool FSTipWidget::eventFilter(QObject *obj, QEvent *e)
{   
        if (obj == this&&e->type() == QEvent::MouseButtonPress)
        {
            if (!rect().contains(mapFromGlobal(static_cast<QMouseEvent *>(e)->globalPos())))
                setVisible(false);
        }
        return false;
}

FSTipWidget & FSTipWidget::Instance()
{
    static FSTipWidget tipWidget;
    return tipWidget;
}