#include "stdafx.h"
#include "FSCropWidget.h"
#include "FSEditHelper.h"
#include <QTimer>

#include <QDebug>
FSCropWidget::FSCropWidget(QWidget *parent)
    : QLabel()
    , mpParentWidget(parent)
    , mActionInfo(CENTER+1,SCALE+1)
    , mPointLast(0,0)
    , mWidht(6)
    , mAngle(0)
{
    setAttribute(Qt::WA_StyledBackground);
    setWindowFlags(Qt::FramelessWindowHint |Qt::SubWindow);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    Q_ASSERT(parent);
    parent->installEventFilter(this);   
    this->installEventFilter(this);
    //mpDrawHelper = new FSVideoPicEditHelper(IFSEditHelper::Edit_Picture, this);
    //mpDrawHelper->SetLimit(false);
}

FSCropWidget::~FSCropWidget()
{

}

QRectF FSCropWidget::CropRect()
{
    return QRectF(mTransform.mapRect(QRect(rect().topLeft(), rect().bottomRight() - QPoint(1, 1))));
}

void FSCropWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QImage image(this->width(), this->height(), QImage::Format_RGB32);
    image.fill(0);
    //mpDrawHelper->DoPaintEvent(image);
    QWidget::paintEvent(e);
}

bool FSCropWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == mpParentWidget)
    {
        if (e->type() == QEvent::WindowActivate)
        {            
            activateWindow();
        }
        else if (e->type()==QEvent::Hide)
        {
            hide();
        }
        else if (e->type() == QEvent::Show)
        {
            QTimer::singleShot(300, [this](){show();});
        }
    }
    else if (obj == this)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            mpParentWidget->raise();
            QTimer::singleShot(5, [this](){raise(); });
        }
    }
    return QLabel::eventFilter(obj, e);
}

void FSCropWidget::resizeEvent(QResizeEvent *e)
{
   
}

void FSCropWidget::mouseMoveEvent(QMouseEvent *ev)
{
   // mpDrawHelper->DoMouseMoveEvent(ev);
    QLabel::mouseMoveEvent(ev);
}

void FSCropWidget::mousePressEvent(QMouseEvent *ev)
{
    //mpDrawHelper->DoMousePressEvent(ev);
    QLabel::mousePressEvent(ev);
}


void FSCropWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    //mpDrawHelper->DoMouseUpEvent(ev);
    QLabel::mouseReleaseEvent(ev);
}