#ifndef FSPAINTPATH_H
#define FSPAINTPATH_H

#include <QPainterPath>
#include <qnamespace.h>

class FSPainterPath : public QObject
{
public:
    FSPainterPath();
    FSPainterPath(const FSPainterPath &Other);
    ~FSPainterPath();

    bool IsNeedFill();
    void SetNeedFill(bool bNeedFill);

    QColor GetPathColor();
    void SetPathColor(QColor color);
    
    QColor GetPathFillColor();
    void SetPathFillColor(QColor color);


    QPainterPath &GetPath();
    void SetPath(const QPainterPath & path);

    float GetPathWidth();
    void SetPathWidth(float width);

    Qt::PenStyle GetStyle();
    void SetStyle(Qt::PenStyle style);

    void SetAwaysFill(bool bAwaysFill);

    void SetRichColor(bool bRichColor);
    bool GetRichColor();

private:
    QColor m_PathColor;
    QColor m_PathFillColor;
    bool m_bNeedFill;
    bool m_bAwaysFill;
    QPainterPath m_Path;
    float m_PathWidth;
    Qt::PenStyle m_style;
    bool m_bRichColor;
};


class FSKeyPoint :public QObject{
    Q_OBJECT
public:
    FSKeyPoint(QPointF centerPt,QObject *pParent);
    FSKeyPoint(const FSKeyPoint &Other);

    ~FSKeyPoint();

    FSPainterPath GetRectPath() const;
    FSPainterPath GetCiyclePath() const;

    void SetCenterPointF(QPointF centerPt);
    QPointF GetCenterPoint() const;

    void SetItemSize(int width,int height);
    QSize GetItemSize() const;

    void SetIndex(int index);
    int GetIndex() const;

    bool IsContiansPt(QPointF pt) const;

    bool IsNeedFill() const;
    void SetNeedFill(bool bNeedFill);

    bool IsPressedOn() const;
    void SetPressedOn(bool bPressedOn);

    void SetFillPathColor(QColor color);
    QColor GetFillPathColor() const;

    void SetPathColor(QColor color);
    QColor GetPathColor() const;

    float GetPathWidth() const;
    void SetPathWidth(float width);

    Qt::PenStyle GetStyle() const;
    void SetStyle(Qt::PenStyle style);

    QRectF GetRectFromCenterPt() const;

private:
    bool m_bNeedFill;
    bool m_bPressedOn;
    int  m_nItemWidth;
    int  m_nItemHeight;
    int  m_nIndex;
    float m_PathWidth;
    Qt::PenStyle m_style;
    QColor m_FillPathColor;
    QColor m_PathColor;
    QPointF m_CenterPt;
};

class FSKeyPointGroup :public QObject{
    Q_OBJECT
public:
    FSKeyPointGroup(QObject *pParent);
    ~FSKeyPointGroup();

    void InitGruop(QList<QPointF> list);

    void AddKeyPoint(FSKeyPoint Pt, QColor pathColor, QColor fillPathColor, Qt::PenStyle style, float width, bool bNeedFill);

    FSKeyPoint *GetKeyPoint(int index);

    QList <FSPainterPath> GetKeyPointRectPath(int start, int num) const;
    QList <FSPainterPath> GetKeyPointCiyclePath(int start, int num) const;
    FSPainterPath GetLinePath(QColor pathColor, Qt::PenStyle style, float width) const;

    FSPainterPath GetLineAreaPath() const;

    int GetKeyPointCount() const;

    void ClearKeyPointState();

    QList<QPointF >GetGuidesPtList(int start,int size);
    int GetPressedOnPtIndex();


private:
    QList <FSKeyPoint> m_ListKeyPoint;
    

};


#endif // FSPAINTPATH_H
