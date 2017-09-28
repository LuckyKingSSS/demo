#include "stdafx.h"
#include "FRuler.h"

#include "inc_CommonLib/TimelineRuler.h"

FRuler::FRuler(QWidget *parent)
    : QWidget(parent)
{
    m_pRuler = new TimelineRuler(this);

    m_pRuler->SetFPS(25.0f);
    m_pRuler->SetMousePressEmitFramesChangedEnabled(false);
    m_pRuler->SetSingalClickStartEndFrameChangeEnabled(true);
    m_pRuler->SetDrawColor(QColor(127, 156, 175));
    RulerPointerShape shape;
    shape.aligntop = true;
    shape.height = 20;
    shape.width = 13;
    shape.color = QColor(239, 48, 98);
    shape.linewidth = 1;
    QVector<QPointF> pts;
    pts << QPointF(-6, 0) << QPointF(6, 0) << QPointF(6, 13) << QPointF(0, 20) << QPointF(-6, 13);
    shape.shape = QPolygonF(pts);
    shape.showpostext = true;
    shape.postextcolor = Qt::white;
    m_pRuler->SetPointerShape(shape);

}

FRuler::~FRuler()
{

}

TimelineRuler* FRuler::Ruler()
{
    return m_pRuler;
}

void FRuler::resizeEvent(QResizeEvent *event)
{
    m_pRuler->setGeometry(rect());
}
