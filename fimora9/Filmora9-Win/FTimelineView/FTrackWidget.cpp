#include "stdafx.h"

#include "inc_FCommonLib/FCommonWidgets.h"

#include "FTrackWidget.h"
#include "FPaintLayer.h"

FTrackWidget::FTrackWidget(FTrackPtr track, QWidget *parent)
    : FBaseWidget(parent)
    , m_track(track)
    , m_headWidth(0)
{
    m_headWidget = new FTrackWidgetHead(this);

    layerManager = new FPaintLayerManager(this);
}

FTrackWidget::~FTrackWidget()
{

}

int FTrackWidget::Height()
{
    return 50;
}

int FTrackWidget::GetHeadWidth()
{
    return m_headWidth;
}

void FTrackWidget::SetHeadWidth(int width)
{
    m_headWidth = width;
}

FTrackPtr FTrackWidget::GetTrack()
{
    return m_track;
}

int FTrackWidget::GetIndex()
{
    return m_track->GetIndex();
}

void FTrackWidget::SetRuler(TimelineRuler *ruler)
{
    m_pRuler = ruler;
}

TimelineRuler* FTrackWidget::Ruler()
{
    return m_pRuler;
}

void FTrackWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(49, 60, 72)); 
    layerManager->paintEvent(&painter, rect().adjusted(GetHeadWidth(), 0, 0, 0), m_track, this, event);
}

void FTrackWidget::resizeEvent(QResizeEvent *event)
{
    m_headWidget->setGeometry(0, 0, GetHeadWidth(), height());
}

//////////////////////////////////////////////////////////////////////////
FTrackWidgetHead::FTrackWidgetHead(QWidget *parent)
:QWidget(parent)
{
    m_icon = new FLabel(this);
}

void FTrackWidgetHead::resizeEvent(QResizeEvent *event)
{
    const int iconw = 22;
    const int iconh = 22;
    const int margins = 10;

    int left = margins;
    int top = (height() - iconw) / 2;
    m_icon->setGeometry(left, top, iconw, iconh);

}
