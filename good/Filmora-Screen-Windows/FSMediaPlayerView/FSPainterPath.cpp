#include "stdafx.h"
#include "FSPainterPath.h"

FSPainterPath::FSPainterPath()
    : QObject(),
    m_bNeedFill(false),
    m_bAwaysFill(false),
    m_bRichColor(false),
    m_PathColor({254,254,254}),
    m_PathFillColor({ 255, 255, 255 }),
    m_PathWidth(1.0),
    m_style(Qt::SolidLine)

{

}

FSPainterPath::FSPainterPath(const FSPainterPath &Other)
    :QObject()
{
    m_bNeedFill = Other.m_bNeedFill;
    m_PathColor = Other.m_PathColor;
    m_PathFillColor = Other.m_PathFillColor;
    m_Path = Other.m_Path;
    m_style = Other.m_style;
    m_PathWidth = Other.m_PathWidth;
    m_bAwaysFill = Other.m_bAwaysFill;
    m_bRichColor = Other.m_bRichColor;
}

FSPainterPath::~FSPainterPath()
{

}

bool FSPainterPath::IsNeedFill()
{
    return m_bNeedFill||m_bAwaysFill;
}

void FSPainterPath::SetNeedFill(bool bNeedFill)
{
    m_bNeedFill = bNeedFill;
}

QColor FSPainterPath::GetPathColor()
{
    return m_PathColor;
}

void FSPainterPath::SetPathColor(QColor color)
{
    m_PathColor = color;
}

QColor FSPainterPath::GetPathFillColor()
{
    return m_PathFillColor;
}

void FSPainterPath::SetPathFillColor(QColor color)
{
    m_PathFillColor = color;
}


QPainterPath & FSPainterPath::GetPath()
{
    return m_Path;
}

void FSPainterPath::SetPath(const QPainterPath & path)
{
    m_Path = path;
}

float FSPainterPath::GetPathWidth()
{
    return m_PathWidth;
}

void FSPainterPath::SetPathWidth(float width)
{
    m_PathWidth = width;
}

Qt::PenStyle FSPainterPath::GetStyle()
{
    return m_style;
}

void FSPainterPath::SetStyle(Qt::PenStyle style)
{
    m_style = style;
}

void FSPainterPath::SetAwaysFill(bool bAwaysFill)
{
    m_bAwaysFill = bAwaysFill;
}

void FSPainterPath::SetRichColor(bool bRichColor)
{
    m_bRichColor = bRichColor;
}

bool FSPainterPath::GetRichColor()
{
    return m_bRichColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
FSKeyPoint::FSKeyPoint(QPointF centerPt, QObject *pParent)
    :QObject(pParent)
    , m_nIndex(0)
    , m_bNeedFill(false)
    , m_bPressedOn(false)
    , m_FillPathColor(QColor(255,255,255))
    , m_PathColor(QColor(254,254,254))
    , m_CenterPt(centerPt)
    , m_nItemWidth(8)
    , m_nItemHeight(8)
{

}

FSKeyPoint::FSKeyPoint(const FSKeyPoint &Other)
{
    m_nIndex = Other.m_nIndex;
    m_bNeedFill = Other.m_bNeedFill;
    m_FillPathColor = Other.m_FillPathColor;
    m_PathColor = Other.m_PathColor;
    m_CenterPt = Other.m_CenterPt;
    m_nItemWidth = Other.m_nItemWidth;
    m_nItemHeight = Other.m_nItemHeight;
    m_bPressedOn = Other.m_bPressedOn;
    m_style = Other.m_style;
    m_PathWidth = Other.m_PathWidth;
}

FSKeyPoint::~FSKeyPoint()
{

}

FSPainterPath FSKeyPoint::GetRectPath() const
{
    FSPainterPath path;
    path.GetPath().addRect(GetRectFromCenterPt());
    path.SetNeedFill(IsNeedFill());
    path.SetPathColor(m_PathColor);
    path.SetPathFillColor(m_FillPathColor);
    path.SetPathWidth(m_PathWidth);
    path.SetStyle(m_style);
    return path;
}

FSPainterPath FSKeyPoint::GetCiyclePath() const
{
    FSPainterPath path;
    path.GetPath().addEllipse(m_CenterPt, 1.0*(m_nItemWidth + 1) / 2, 1.0*(m_nItemHeight + 1) / 2);
    path.SetNeedFill(IsNeedFill());
    path.SetPathColor(m_PathColor);
    path.SetPathFillColor(m_FillPathColor);
    path.SetPathWidth(m_PathWidth);
    path.SetStyle(m_style);
    return path;
}

void FSKeyPoint::SetCenterPointF(QPointF centerPt)
{
    m_CenterPt = centerPt;
}

QPointF FSKeyPoint::GetCenterPoint() const
{
    return m_CenterPt;
}

void FSKeyPoint::SetItemSize(int width,int height)
{
    m_nItemWidth = width;
    m_nItemHeight = height;
}


QSize FSKeyPoint::GetItemSize() const
{
    return QSize(m_nItemWidth,m_nItemHeight);
}

void FSKeyPoint::SetIndex(int index)
{
    m_nIndex = index;
}

int FSKeyPoint::GetIndex() const
{
    return m_nIndex;
}

bool FSKeyPoint::IsContiansPt(QPointF pt) const
{
    FSPainterPath path;
    path.GetPath().addEllipse(m_CenterPt, 1.0*m_nItemWidth / 2, 1.0*m_nItemHeight / 2);
    return path.GetPath().contains(pt);
}

bool FSKeyPoint::IsNeedFill() const
{
    return m_bNeedFill;
}

void FSKeyPoint::SetNeedFill(bool bNeedFill)
{
        m_bNeedFill = bNeedFill;
}

bool FSKeyPoint::IsPressedOn() const
{
    return m_bPressedOn;
}

void FSKeyPoint::SetPressedOn(bool bPressedOn)
{
    m_bPressedOn = bPressedOn;
}

void FSKeyPoint::SetFillPathColor(QColor color)
{
    m_FillPathColor = color;
}

QColor FSKeyPoint::GetFillPathColor() const
{
    return m_FillPathColor;
}

void FSKeyPoint::SetPathColor(QColor color)
{
    m_PathColor = color;
}

QColor FSKeyPoint::GetPathColor() const
{
    return m_PathColor;
}

float FSKeyPoint::GetPathWidth() const
{
    return m_PathWidth;
}

void FSKeyPoint::SetPathWidth(float width)
{
    m_PathWidth = width;
}

Qt::PenStyle FSKeyPoint::GetStyle() const
{
    return m_style;
}

void FSKeyPoint::SetStyle(Qt::PenStyle style)
{
    m_style = style;
}

QRectF FSKeyPoint::GetRectFromCenterPt() const
{
    return QRectF(m_CenterPt.x() - 1.0*m_nItemWidth / 2, m_CenterPt.y() - 1.0*m_nItemHeight / 2, m_nItemWidth, m_nItemHeight);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSKeyPointGroup::FSKeyPointGroup(QObject *pParent)
    :QObject(pParent)
{

}

FSKeyPointGroup::~FSKeyPointGroup()
{

}

void FSKeyPointGroup::AddKeyPoint(FSKeyPoint Pt, QColor pathColor, QColor fillPathColor, Qt::PenStyle style,float width,bool bNeedFil)
{
    //Pt.SetIndex(m_ListKeyPoint.size());
    Pt.SetFillPathColor(fillPathColor);
    Pt.SetPathColor(pathColor);
    Pt.SetNeedFill(bNeedFil);
    Pt.SetPathWidth(width);
    Pt.SetStyle(style);
    m_ListKeyPoint.push_back(Pt);
}

FSKeyPoint * FSKeyPointGroup::GetKeyPoint(int index)
{
    if (m_ListKeyPoint.isEmpty() || index<0 || index>m_ListKeyPoint.size() - 1)
        return NULL;
    return &m_ListKeyPoint[index];
}

QList <FSPainterPath> FSKeyPointGroup::GetKeyPointRectPath(int start, int num) const
{
    QList <FSPainterPath> list;
    for (int i = start; i < m_ListKeyPoint.size() && i < start + num;i++)
    {
        list.push_back(m_ListKeyPoint.at(i).GetRectPath());
    }
    return list;
}

QList <FSPainterPath> FSKeyPointGroup::GetKeyPointCiyclePath(int start, int num) const
{
    QList <FSPainterPath> list;
    for (int i = start; i < m_ListKeyPoint.size() && i < start + num; i++)
    {
        list.push_back(m_ListKeyPoint.at(i).GetCiyclePath());
    }
    return list;
}

FSPainterPath FSKeyPointGroup::GetLinePath(QColor pathColor, Qt::PenStyle style, float width) const
{
    FSPainterPath path;
    path.SetNeedFill(false);
    path.SetPathColor(pathColor);
    path.SetStyle(style);
    path.SetPathWidth(width);
    if (m_ListKeyPoint.size()>=8)
    {
        path.GetPath().moveTo(m_ListKeyPoint.at(0).GetCenterPoint().x(), m_ListKeyPoint.at(0).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(1).GetCenterPoint().x(), m_ListKeyPoint.at(1).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(2).GetCenterPoint().x(), m_ListKeyPoint.at(2).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(3).GetCenterPoint().x(), m_ListKeyPoint.at(3).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(4).GetCenterPoint().x(), m_ListKeyPoint.at(4).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(5).GetCenterPoint().x(), m_ListKeyPoint.at(5).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(6).GetCenterPoint().x(), m_ListKeyPoint.at(6).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(7).GetCenterPoint().x(), m_ListKeyPoint.at(7).GetCenterPoint().y());
        path.GetPath().lineTo(m_ListKeyPoint.at(0).GetCenterPoint().x(), m_ListKeyPoint.at(0).GetCenterPoint().y());
    }
    return path;
    
}

FSPainterPath FSKeyPointGroup::GetLineAreaPath() const
{
    FSPainterPath path;
    path.SetNeedFill(false);
    if (m_ListKeyPoint.size() >= 8)
    {
        path.GetPath().addRect(QRectF(m_ListKeyPoint.at(0).GetCenterPoint(), m_ListKeyPoint.at(4).GetCenterPoint()));
    }
    return path;
}

int FSKeyPointGroup::GetKeyPointCount() const
{
    return m_ListKeyPoint.size();
}

void FSKeyPointGroup::ClearKeyPointState()
{
    for (int i = 0; i < m_ListKeyPoint.size();i++)
    {
        m_ListKeyPoint[i].SetPressedOn(false);
        m_ListKeyPoint[i].SetNeedFill(false);
    }
}

QList<QPointF > FSKeyPointGroup::GetGuidesPtList(int start, int size)
{
    start = qMax(0, start);
    size = qBound(0, size, m_ListKeyPoint.size());
    QList <QPointF> list;
    for (int i = start; i < size; i++)
    {
        list.push_back(m_ListKeyPoint.at(i).GetCenterPoint());
    }
    return list;
}

int FSKeyPointGroup::GetPressedOnPtIndex()
{
    for (int i = 0; i < m_ListKeyPoint.size(); i++)
    {
        if (m_ListKeyPoint.at(i).IsPressedOn())
        {
            return m_ListKeyPoint.at(i).GetIndex();
        }
    }
    return -1;
}