#include "stdafx.h"
#include "AudioIndicator.h"

AudioIndicator::AudioIndicator(QWidget *parent)
: QWidget(parent)
    , mILeftChannel(1)
    , mIRightChannel(1)
{
    //setWindowFlags(Qt:: Qt::CustomizeWindowHint);
    //setFixedWidth(21);
    /*********************component test*********************
    QtConcurrent::run([this](){
        while (1)
        {
            SetChannelValue(rand() % 100, rand() % 100);
            QThread::msleep(150);
        }
    }); 
    /*********************component test*********************/
}

AudioIndicator::~AudioIndicator()
{

}

void AudioIndicator::SetChannelValue(int left, int right)
{
    mILeftChannel = qBound(0, left, 100);
    mIRightChannel = qBound(0, right, 100);
    update();
}

void AudioIndicator::paintEvent(QPaintEvent *e)
{
    int margin = 3;
    int radius = 3;

    QPainterPath mPath;
    QList<QPainterPath> list;
    list.append(QPainterPath());
    list.append(QPainterPath());
    list.append(QPainterPath()); 

    QPainter painter(this);
    painter.setPen(QPen(QColor(200, 200, 200)));
    int ellipseNum = height() / (margin + 2 * radius);

    for (int i = 0; i < ellipseNum; i++)
    {
        mPath.addEllipse(GetCenterPoint(radius, margin, 1, i + 1), radius, radius);
        mPath.addEllipse(GetCenterPoint(radius, margin, 2, i + 1), radius, radius);

        if (mILeftChannel / 100.0 * height() >= (radius + margin) + i*(radius * 2 + margin)){                  
            FillPathData(list, GetCenterPoint(radius, margin, 1, i + 1), radius, 1.0*(i + 1) / ellipseNum);          
        }

        if (mIRightChannel / 100.0 * height() >= (radius + margin) + i*(radius * 2 + margin)){        
            FillPathData(list, GetCenterPoint(radius, margin, 2, i + 1), radius, 1.0*(i + 1) / ellipseNum);
        }
    }
    painter.fillRect(rect(), QColor(0, 200, 130));
    painter.drawPath(mPath);
    if (!list.at(0).isEmpty())
        painter.fillPath(list.at(0), QBrush(Qt::white));
    if (!list.at(1).isEmpty())
        painter.fillPath(list.at(1), QBrush(QColor(230, 230, 0)));
    if (!list.at(2).isEmpty())
        painter.fillPath(list.at(2), QBrush(QColor(230, 120, 0)));

    QWidget::paintEvent(e);
}



QPoint AudioIndicator::GetCenterPoint(int radius, int margin, int indexH, int indexV)
{
    indexH = qMax(1, indexH);
    indexV = qMax(1, indexV);
    return QPoint((2 * indexH - 1)*radius + indexH*margin,height()-((2 * indexV - 1)*radius + indexV*margin));
}

void AudioIndicator::FillPathData(QList<QPainterPath> &list, QPoint center, int raduis, float precent)
{
    int index;
    index= precent<0.75 ? 0 : precent>=0.9 ? 2 : 1;
    index = qBound(0, index, list.size() - 1);
    list[index].addEllipse(center, raduis, raduis);
}
