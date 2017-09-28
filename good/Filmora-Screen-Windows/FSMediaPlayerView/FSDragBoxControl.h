#ifndef FSEDITCROPHELPER_H
#define FSEDITCROPHELPER_H

#include <QObject>
#include "FSPainterPath.h"

class FSNLETimeline;
class IFSDragBoxControl : public QObject
{
    Q_OBJECT
public:
    enum CropItem{
        TOP_LEFT = 0,
        TOP_MID,
        TOP_RIGHT,
        RIGHT_MID,
        BOTTOM_RIGHT,
        BOTTOM_MID,
        BOTTOM_LEFT,
        LEFT_MID,
        ANGEL_PT,
        CENTER
    };

signals:
    void TransFormChanged();
    void CursorShapeChanged(Qt::CursorShape shape);
    void sigRepaintWidget();

public:
    IFSDragBoxControl(QObject *parent);
    ~IFSDragBoxControl();

    virtual bool DoMousePressEvent(QMouseEvent *e);
    virtual bool DoMouseMoveEvent(QMouseEvent *e);
    virtual void DoMouseUpEvent(QMouseEvent *e);
    virtual void DoPaintControlBoxEvent(QPainter &paniter,QPointF pt);
    virtual void SetScale(double x, double y);
    virtual void initTransformInfo(double angle, double scaleX, double scaleY, double posX, double posY, QRectF baseRc);
    virtual void GetTransformInfo(double &angle, double &scaleX, double &scaleY, double &posX, double &posY, QRectF &baseRc);
    virtual void DoResizeEvent(QResizeEvent *e);


    virtual void GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt);
    virtual void GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle);
    virtual void SetScaleWithDelta(QPointF deltaScale, int index);
    virtual void SetAngleWithDelta(double deltaAngle, bool bSnap);
    virtual void SetPosWithDelta(QPointF deltaPos);
    virtual void DoMouseRealese(QPointF curPt);

    virtual void Position(double &x, double &y);
    virtual void Scale(double &x, double &y); 
    virtual void SetPostion(double x, double y);
    virtual void UpdataKeyPointGroup();
    virtual bool Intersects(QRect rc);

    virtual void GetPath(QList<FSPainterPath> &listPath, QPointF offSet);
    virtual bool IsPressedOnControlBox(QPointF pos, QPointF offSet, int &index);
    virtual bool IsPressedOnRectArea(QPointF pos, QPointF offSet);
    virtual QSizeF GetRenderScale();
    virtual QList<QPointF >GetGuidesPtList();
    virtual QList<QPointF>GetTargetPtList(bool IsSelect);
    virtual int UpdataKeyPointNeedFillState(QPointF pt);
    virtual int UpdataKeyPointPressedOnState(QPointF pt);

    virtual bool GetGuidesPt(int index, QPointF &pt);
    int GetKeyPointCount();



    double Angle();
    void SetAngle(double angle);


    
   
    void SetBaseRc(QRectF baseRc);
    
    bool   IsPressedOn(int& index);

    void ClearKeyPointState();
    int GetPressedOnIndex();
    void RolationSnap(double &angle);

    void SetShiftFlag(bool bPressedShift);

    void SetLinePathColor(QColor color);
    void SetControlPathColor(QColor color);
    void SetControlBoxFillColor(QColor color);
    void SetRolationFillColor(QColor color);
    void SetLineWidth(float w);
    void SetLinePathStyle(Qt::PenStyle style);
    

protected:
    virtual void ItemTranslate(QPointF offSet, int index);
    virtual void DoTranslateEvent(CropItem item, QPointF currentPt);   
    virtual QTransform GetScaleTransform(QPointF destPt, QPointF destPt1, QPointF letfPt, QPointF rightPt, QPointF offSetPt, int index);


    Qt::CursorShape ChangeCursor(int index);



    QTransform GetTransform();
    QTransform GetRotatedTransform(QRectF rc, double angle);

    QRectF GetRealRenderRectf(QRectF renderRect);
    qreal  GetScaleDistance(QPointF currentPt, QPointF destPt, QPointF destPt1);
    qreal  Distance(QPointF Pt1, QPointF Pt2);
    bool   IsThreePointOnLine(QPointF destPt, QPointF leftPt, QPointF rightPt);
    int    TransDragBoxType();
    double GetAngle(QPointF ptCenter, QPointF ptLast, QPointF ptCurrent);
    QRectF GetRectFromCenterPt(QPointF Center, int width);

    


private:
    void   InitKeyPointGroup();
    
protected:
    Qt::CursorShape  mCursorShape;    
    QPointF          mPointLast;
    int              mControlItemWidht;
    bool             m_bRectBoxPath;
    bool             m_bPressedShift;
    FSKeyPointGroup  m_KeyPointGroup;

    QColor          m_ColorLinePath;
    QColor          m_ColorControlPath;
    QColor          m_ColorFillControlBox;
    QColor          m_ColorFillRolationBox;


    float           m_PathLineWidth;
    Qt::PenStyle    m_PenStyle;

    double          mAngle;
    double          mScaleX;
    double          mScaleY;
    double          mTransX;
    double          mTransY;
    QRectF          mBascRc;
    QRectF          mRender;


};


class FSScaleBoxControl :public IFSDragBoxControl{
    Q_OBJECT
public:
    FSScaleBoxControl(QObject *pParent);
    ~FSScaleBoxControl();

     
private:

};


class FSMaskBoxControl :public IFSDragBoxControl{
    Q_OBJECT

signals:
    void MaskPathChanged();
public:
    FSMaskBoxControl(QObject *pParent);
    ~FSMaskBoxControl();

    virtual void SetScale(double x, double y);
    virtual void initTransformInfo(double angle, double scaleX, double scaleY, double posX, double posY, QRectF baseRc);
    virtual void GetTransformInfo(double &angle, double &scaleX, double &scaleY, double &posX, double &posY, QRectF &baseRc);
    virtual void DoPaintControlBoxEvent(QPainter &paniter, QPointF pt);
    virtual void Position(double &x, double &y);
    virtual void Scale(double &x, double &y);
    virtual void SetPostion(double x, double y);
    virtual void UpdataKeyPointGroup();

    virtual void GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt);
    virtual void GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle);
    virtual void SetScaleWithDelta(QPointF deltaScale, int index);
    virtual void SetAngleWithDelta(double deltaAngle, bool bSnap);
    virtual void SetPosWithDelta(QPointF deltaPos);
    virtual void DoMouseRealese(QPointF curPt);

    virtual QSizeF GetRenderScale();
    virtual bool Intersects(QRect rc);

    virtual void GetPath(QList<FSPainterPath> &listPath, QPointF offSet);
    virtual bool IsPressedOnControlBox(QPointF pos, QPointF offSet, int &index);
    virtual bool IsPressedOnRectArea(QPointF pos, QPointF offSet);
    virtual QList<QPointF >GetGuidesPtList();
    virtual QList<QPointF>GetTargetPtList(bool IsSelect);
    virtual bool GetGuidesPt(int index, QPointF &pt);
    virtual int  UpdataKeyPointNeedFillState(QPointF pt);
    virtual int  UpdataKeyPointPressedOnState(QPointF pt);

    QString MaskPathPoint();
    void StartMaskMode(const QString &maskPath);
    void DoResize(const QString &maskPath);
    
protected:
    virtual void ItemTranslate(QPointF offSet, int index);
    virtual void DoTranslateEvent(CropItem item, QPointF currentPt);
    virtual QTransform GetScaleTransform(QPointF destPt, QPointF destPt1, QPointF letfPt, QPointF rightPt, QPointF offSetPt, int index);

    

private:
    QTransform GetOriginTransForm();
    bool IsContainPt(QPointF pt, QPointF offSet);


private:
    double m_SourceScaleX;
    double m_SourceScaleY;
    double m_SourceTransX;
    double m_SourceTransY;

    double m_MouseTransx;
    double m_MouseTransy;
};
#endif // FSEDITCROPHELPER_H
