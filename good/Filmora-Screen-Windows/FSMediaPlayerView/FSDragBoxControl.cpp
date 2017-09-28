#include "stdafx.h"
#include "FSDragBoxControl.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_FSCommonlib/ExternalAPI.h"

IFSDragBoxControl::IFSDragBoxControl( QObject *parent)
    : QObject(parent)
    , mControlItemWidht(8)
    , m_bRectBoxPath(true)
    ,m_bPressedShift(false)
    , m_KeyPointGroup(this)
    , m_ColorLinePath(220, 220, 220)
    , m_ColorControlPath(220, 220, 220)
    , m_ColorFillControlBox(255, 255, 255)
    , m_ColorFillRolationBox("#EF3062")
    , m_PathLineWidth(1.6)
    , m_PenStyle(Qt::DashLine)
{
    InitKeyPointGroup();
}

IFSDragBoxControl::~IFSDragBoxControl()
{

}

void IFSDragBoxControl::initTransformInfo(double angle, double scaleX, double scaleY, double posX, double posY, QRectF baseRc)
{
    mAngle  = angle;
    mScaleX = scaleX;
    mScaleY = scaleY;
    mTransX = posX;
    mTransY = posY;
    mBascRc = baseRc;
    UpdataKeyPointGroup();
}

void IFSDragBoxControl::GetTransformInfo(double &angle, double &scaleX, double &scaleY, double &posX, double &posY, QRectF &baseRc)
{
    angle  = mAngle;
    scaleX = mScaleX;
    scaleY = mScaleY;
    posX   = mTransX;
    posY   = mTransY;
    baseRc = mBascRc;
}

void IFSDragBoxControl::SetBaseRc(QRectF baseRc)
{
    mBascRc = baseRc;
}


bool IFSDragBoxControl::DoMousePressEvent(QMouseEvent *e)
{
    m_bPressedShift = (e->modifiers() & Qt::ShiftModifier);
    int index =UpdataKeyPointPressedOnState(e->pos());
    if (index <= CENTER)
    {          
        mPointLast = e->pos();    
        return true;
    }
    else
    {
        m_KeyPointGroup.ClearKeyPointState();
        return false;
    }
}

bool IFSDragBoxControl::DoMouseMoveEvent(QMouseEvent *e)
{
    int index = CENTER+1;
    int Num = UpdataKeyPointNeedFillState(e->pos());
    if (IsPressedOn(index))
    {
        DoTranslateEvent(CropItem(index), e->pos());
        emit CursorShapeChanged(ChangeCursor(index < ANGEL_PT ? (index + TransDragBoxType()) % 8 : index));
        mPointLast=e->pos();
        return true;
    }
    else
    {
        emit CursorShapeChanged(ChangeCursor(Num < ANGEL_PT ? (Num + TransDragBoxType()) % 8 : Num));
        return false;
    }
}

void IFSDragBoxControl::DoTranslateEvent(CropItem item, QPointF currentPt)
{
    QPointF offSet = currentPt - mPointLast;
    double x, y;
    QRectF tempCenterRect = GetTransform().mapRect(mBascRc);
    switch (item)
    {
    case IFSDragBoxControl::CENTER:
        x = offSet.x();
        y = offSet.y();
        mTransX = mTransX + x;
        mTransY = mTransY + y;
        UpdataKeyPointGroup();
        emit TransFormChanged();
        break;
    case IFSDragBoxControl::ANGEL_PT:
    {
        double angle = GetAngle(tempCenterRect.center(), mPointLast, currentPt);
        int angleTemp;
        double angleTemp1;
        mAngle = mAngle + angle;
        angleTemp = mAngle;
        angleTemp1 = qAbs(mAngle - angleTemp);
        if (angleTemp > 0)
        {
            angleTemp = angleTemp % 360;
            mAngle = angleTemp + angleTemp1;
        }
        else
        {
            angleTemp = qAbs(angleTemp % 360);
            mAngle = 360 - angleTemp - angleTemp1;
        }
        RolationSnap(mAngle);
        UpdataKeyPointGroup();
        emit TransFormChanged();
    }        
        break;
    default:
        ItemTranslate(offSet, item);
        break;
    }
}

QTransform IFSDragBoxControl::GetScaleTransform(QPointF destPt, QPointF destPt1, QPointF leftPt, QPointF rightPt, QPointF offSetPt, int index)
{
    QTransform transform;
    if (m_bPressedShift)
    {
        if (IsThreePointOnLine(leftPt, destPt, rightPt))
        {
            qreal distance = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(destPt1));
            qreal distance1 = Distance(destPt, destPt1);
            if (index == 1 || index == 5)
            {
                if (distance <= 2 * mControlItemWidht)
                {
                    transform = QTransform().scale(1, 1);
                }
                else
                {
                    transform = QTransform().scale(1, distance / distance1);
                }
            }
            else
            {
                if (distance <= 2 * mControlItemWidht)
                {
                    transform = QTransform().scale(1, 1);
                }
                else
                {
                    transform = QTransform().scale(distance / distance1, 1);
                }
            }
        }
        else
        {
            
            if (index == 0 || index == 4)
            {
                qreal distanceX1 = 2 * Distance(destPt, rightPt);
                qreal distanceY1 = 2 * Distance(destPt, leftPt);

                qreal distanceX = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(rightPt))+distanceX1/2;
                qreal distanceY = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(leftPt)) + distanceY1 / 2;
                
                qreal scaleX = 1;
                qreal scaleY = 1;

                if (distanceX > 2 * mControlItemWidht)
                {
                    scaleX = distanceX / distanceX1;
                }
                if (distanceY > 2 * mControlItemWidht)
                {
                    scaleY = distanceY / distanceY1;
                }                    
                transform = QTransform().scale(scaleX, scaleY);

            }
            else
            {
                qreal distanceY1 = 2 * Distance(destPt, rightPt);
                qreal distanceX1 = 2 * Distance(destPt, leftPt);

                qreal distanceY = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(rightPt)) + distanceY1 / 2;
                qreal distanceX = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(leftPt)) + distanceX1 / 2;

                qreal scaleX = 1;
                qreal scaleY = 1;

                if (distanceX > 2 * mControlItemWidht)
                {
                    scaleX = distanceX / distanceX1;
                }
                if (distanceY > 2 * mControlItemWidht)
                {
                    scaleY = distanceY / distanceY1;
                }
                transform = QTransform().scale(scaleX, scaleY);
            }
        }
    }
    else
    {
        qreal distance = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(destPt1));
        qreal distance1 = Distance(destPt, destPt1);
        if (IsThreePointOnLine(leftPt, destPt, rightPt))
        {
            if (distance <= 2 * mControlItemWidht)
            {
                transform = QTransform().scale(1, 1);
            }
            else
            {
                transform = QTransform().scale(distance / distance1, distance / distance1);
            }
        }
        else
        {
            if ((Distance(destPt, leftPt) <= mControlItemWidht || Distance(destPt, rightPt) <= mControlItemWidht) && (distance / distance1 < 1))
            {
                transform = QTransform().scale(1, 1);
            }
            else
            {
                transform = QTransform().scale(distance / distance1, distance / distance1);
            }
        }
    }
    return transform;
}

void IFSDragBoxControl::UpdataKeyPointGroup()
{
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    if (m_KeyPointGroup.GetKeyPointCount() >= 11)
    {
        m_KeyPointGroup.GetKeyPoint(0)->SetCenterPointF(baseRc.topLeft());
        m_KeyPointGroup.GetKeyPoint(1)->SetCenterPointF(QPointF(baseRc.center().x(), baseRc.topLeft().y()));
        m_KeyPointGroup.GetKeyPoint(2)->SetCenterPointF(baseRc.topRight());
        m_KeyPointGroup.GetKeyPoint(3)->SetCenterPointF(QPointF(baseRc.topRight().x(), baseRc.center().y()));
        m_KeyPointGroup.GetKeyPoint(4)->SetCenterPointF(baseRc.bottomRight());
        m_KeyPointGroup.GetKeyPoint(5)->SetCenterPointF(QPointF(baseRc.center().x(), baseRc.bottomLeft().y()));
        m_KeyPointGroup.GetKeyPoint(6)->SetCenterPointF(baseRc.bottomLeft());
        m_KeyPointGroup.GetKeyPoint(7)->SetCenterPointF(QPointF(baseRc.topLeft().x(), baseRc.center().y()));
        m_KeyPointGroup.GetKeyPoint(8)->SetCenterPointF(baseRc.center() + QPointF(4 * m_KeyPointGroup.GetKeyPoint(8)->GetItemSize().width(), 0));
        m_KeyPointGroup.GetKeyPoint(9)->SetCenterPointF(baseRc.center());
        m_KeyPointGroup.GetKeyPoint(10)->SetCenterPointF(baseRc.center());
        m_KeyPointGroup.GetKeyPoint(10)->SetItemSize(baseRc.width(), baseRc.height());
    }

    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        m_KeyPointGroup.GetKeyPoint(i)->SetPathColor(m_ColorControlPath);
        m_KeyPointGroup.GetKeyPoint(i)->SetFillPathColor(i != 8 ? m_ColorFillControlBox : m_ColorFillRolationBox);
    }

    mRender = QRectF(baseRc.left() / mBascRc.width(), baseRc.top() / mBascRc.height(), baseRc.width() / mBascRc.width(), baseRc.height() / mBascRc.height());
}

int IFSDragBoxControl::UpdataKeyPointNeedFillState(QPointF pt)
{
    QRectF rc = GetTransform().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    int index = CENTER+1;
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        bool bNeedFill = tempTransform.map(m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath().GetPath()).contains(pt);
        if (!m_KeyPointGroup.GetKeyPoint(i)->IsPressedOn())
        {
            if (bNeedFill != m_KeyPointGroup.GetKeyPoint(i)->IsNeedFill())
            {
                m_KeyPointGroup.GetKeyPoint(i)->SetNeedFill(bNeedFill);
                emit sigRepaintWidget();
            }                           
        }            
        if (bNeedFill&&(index==CENTER+1))
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
    }
    emit CursorShapeChanged(ChangeCursor(index < ANGEL_PT ? (index + TransDragBoxType()) % 8 : index));
    return index;
}

int IFSDragBoxControl::UpdataKeyPointPressedOnState(QPointF pt)
{
    QRectF rc = GetTransform().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    int index = CENTER + 1;
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        bool bPressedOn = tempTransform.map(m_KeyPointGroup.GetKeyPoint(i)->GetRectPath().GetPath()).contains(pt);
        if (bPressedOn&&(index == CENTER + 1))
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
        m_KeyPointGroup.GetKeyPoint(i)->SetPressedOn(bPressedOn);
    }
    return index;
}

void IFSDragBoxControl::DoMouseUpEvent(QMouseEvent *e)
{

    m_KeyPointGroup.ClearKeyPointState();
    UpdataKeyPointNeedFillState(e->pos());
    emit CursorShapeChanged(Qt::ArrowCursor);
}

void IFSDragBoxControl::DoPaintControlBoxEvent(QPainter &painter, QPointF pt)
{
    UpdataKeyPointGroup();
    return;//Íâ²ã»­¿ò
}

void IFSDragBoxControl::DoResizeEvent(QResizeEvent *e)
{
    QRectF realRc = GetRealRenderRectf(mRender);
    QPointF realRenderRectCenter = realRc.center();
    QTransform transform = GetRotatedTransform(realRc, -mAngle);
    mScaleX = mRender.width();
    mScaleY = mRender.height();
    QPointF renderRectCenter = QTransform().scale(mScaleX, mScaleY).mapRect(mBascRc).center();
    mTransX = realRenderRectCenter.x() - renderRectCenter.x();
    mTransY = realRenderRectCenter.y() - renderRectCenter.y();
    UpdataKeyPointGroup();
}

void IFSDragBoxControl::GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt)
{
    index = qBound(0, index, 7);
    QPointF offSet = curPt - lastPt;
    QPointF destPt = m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint();
    QPointF destPt1 = m_KeyPointGroup.GetKeyPoint((index + 4) % 8)->GetCenterPoint();
    QPointF leftPt = m_KeyPointGroup.GetKeyPoint((index + 7) % 8)->GetCenterPoint();
    QPointF rightPt = m_KeyPointGroup.GetKeyPoint((index + 1) % 8)->GetCenterPoint();

    QPointF ptIndex2 = m_KeyPointGroup.GetKeyPoint((index + 2) % 8)->GetCenterPoint();
    QPointF ptIndex5 = m_KeyPointGroup.GetKeyPoint((index + 5) % 8)->GetCenterPoint();
    QPointF ptIndex6 = m_KeyPointGroup.GetKeyPoint((index + 6) % 8)->GetCenterPoint();

    QTransform transform = GetScaleTransform(destPt, destPt1, leftPt, rightPt, offSet, index);
    scalePt.setX(transform.m11());
    scalePt.setY(transform.m22());
}

void IFSDragBoxControl::GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle)
{
    QRectF tempCenterRect = GetTransform().mapRect(mBascRc);
    angle = GetAngle(tempCenterRect.center(), lastPt, curPt);
}

void IFSDragBoxControl::SetScaleWithDelta(QPointF deltaScale, int index)
{
    index = qBound(0, index, 7);
    m_KeyPointGroup.GetKeyPoint(index)->SetPressedOn(true);
    QPointF destPt1 = m_KeyPointGroup.GetKeyPoint((index + 4) % 8)->GetCenterPoint();

    QRectF rc = GetTransform().mapRect(mBascRc);
    QTransform transform1 = GetRotatedTransform(rc, mAngle);
    QPointF Pt = transform1.map(destPt1);

    rc = QTransform().scale(deltaScale.x(), deltaScale.y()).mapRect(rc);
    QTransform transform2 = GetRotatedTransform(rc, mAngle);

    QPointF PT = QTransform().scale(deltaScale.x(), deltaScale.y()).map(destPt1);
    PT = transform2.map(PT);
    QPointF transPt(-PT.x() + Pt.x(), -PT.y() + Pt.y());
    QTransform transform = GetTransform()*QTransform().scale(deltaScale.x(), deltaScale.y())*QTransform().translate(transPt.x(), transPt.y());
    mScaleX = transform.m11();
    mScaleY = transform.m22();
    mTransX = transform.m31();
    mTransY = transform.m32();
    UpdataKeyPointGroup();
    emit TransFormChanged();
}

void IFSDragBoxControl::SetAngleWithDelta(double deltaAngle, bool bSnap)
{
    int angleTemp;
    double angleTemp1;
    mAngle = mAngle + deltaAngle;
    angleTemp = mAngle;
    angleTemp1 = qAbs(mAngle - angleTemp);
    if (angleTemp > 0)
    {
        angleTemp = angleTemp % 360;
        mAngle = angleTemp + angleTemp1;
    }
    else
    {
        angleTemp = qAbs(angleTemp % 360);
        mAngle = 360 - angleTemp - angleTemp1;
    }
    if (bSnap)
        RolationSnap(mAngle);
    UpdataKeyPointGroup();
    emit TransFormChanged();
}

void IFSDragBoxControl::SetPosWithDelta(QPointF deltaPos)
{
    mTransX = mTransX + deltaPos.x();
    mTransY = mTransY + deltaPos.y();
    UpdataKeyPointGroup();
    emit TransFormChanged();
}

void IFSDragBoxControl::DoMouseRealese(QPointF curPt)
{
    ClearKeyPointState();
    UpdataKeyPointNeedFillState(curPt);
}

void IFSDragBoxControl::SetPostion(double x, double y)
{
    mTransX = x;
    mTransY = y;
}

void IFSDragBoxControl::SetScale(double x, double y)
{
    mScaleX = x;
    mScaleY = y;
}

void IFSDragBoxControl::SetAngle(double angle)
{
    mAngle = angle;
}


void IFSDragBoxControl::ItemTranslate(QPointF offSet, int index)
{
    float Scalex, Scaley;
    index = qBound(0, index, 7);
    QPointF destPt = m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint();
    QPointF destPt1 = m_KeyPointGroup.GetKeyPoint((index + 4) % 8)->GetCenterPoint();
    QPointF leftPt = m_KeyPointGroup.GetKeyPoint((index + 7) % 8)->GetCenterPoint();
    QPointF rightPt = m_KeyPointGroup.GetKeyPoint((index + 1) % 8)->GetCenterPoint();

    QPointF ptIndex2 = m_KeyPointGroup.GetKeyPoint((index + 2) % 8)->GetCenterPoint();
    QPointF ptIndex5 = m_KeyPointGroup.GetKeyPoint((index + 5) % 8)->GetCenterPoint();
    QPointF ptIndex6 = m_KeyPointGroup.GetKeyPoint((index + 6) % 8)->GetCenterPoint();

    QTransform transform = GetScaleTransform(destPt, destPt1, leftPt, rightPt, offSet, index);
    Scalex = transform.m11();
    Scaley = transform.m22();
    QRectF rc = GetTransform().mapRect(mBascRc);
    QTransform transform1 = GetRotatedTransform(rc, mAngle);
    QPointF Pt = transform1.map(destPt1);

    rc = QTransform().scale(Scalex, Scaley).mapRect(rc);
    QTransform transform2 = GetRotatedTransform(rc, mAngle);

    QPointF PT = QTransform().scale(Scalex, Scaley).map(destPt1);
    PT = transform2.map(PT);
    QPointF transPt(-PT.x() + Pt.x(), -PT.y() + Pt.y());
    transform = GetTransform()*QTransform().scale(Scalex, Scaley)*QTransform().translate(transPt.x(), transPt.y());
    mScaleX = transform.m11();
    mScaleY = transform.m22();
    mTransX = transform.m31();
    mTransY = transform.m32();
    UpdataKeyPointGroup();
    emit TransFormChanged();
}

Qt::CursorShape IFSDragBoxControl::ChangeCursor(int index)
{
    switch (index)
    {
    case IFSDragBoxControl::BOTTOM_RIGHT:
    case IFSDragBoxControl::TOP_LEFT:
        mCursorShape = Qt::SizeFDiagCursor;
        break;
    case IFSDragBoxControl::TOP_RIGHT:
    case IFSDragBoxControl::BOTTOM_LEFT:
        mCursorShape = Qt::SizeBDiagCursor;
        break;
    case IFSDragBoxControl::TOP_MID:
    case IFSDragBoxControl::BOTTOM_MID:
        mCursorShape = Qt::SizeVerCursor;
        break;
    case IFSDragBoxControl::LEFT_MID:
    case IFSDragBoxControl::RIGHT_MID:
        mCursorShape = Qt::SizeHorCursor;
        break;
    case IFSDragBoxControl::ANGEL_PT:
        mCursorShape = Qt::CrossCursor;
        break;
    case IFSDragBoxControl::CENTER:
        mCursorShape = Qt::SizeAllCursor;
        break;
    default:
        mCursorShape = Qt::ArrowCursor;
        break;
    }
    return mCursorShape;
}



QList<QPointF > IFSDragBoxControl::GetGuidesPtList()
{
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    QTransform transform = GetRotatedTransform(baseRc, mAngle);
    QList<QPointF> ptList = m_KeyPointGroup.GetGuidesPtList(0, 8);
    for (int i = 0; i < ptList.size(); i++)
    {
        ptList[i] = transform.map(ptList.at(i));
    }

    return ptList;
}

QList<QPointF> IFSDragBoxControl::GetTargetPtList(bool isSelect)
{
    if (isSelect)
    {
        int index =m_KeyPointGroup.GetPressedOnPtIndex();
        QRectF baseRc = GetTransform().mapRect(mBascRc);
        QTransform transform = GetRotatedTransform(baseRc, mAngle);
        if (index != -1&&index<8)
        {
            return QList<QPointF>() << transform.map(m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint());
        }
        else
        {
            return GetGuidesPtList();
        }
    }
    return QList<QPointF>();
}

bool IFSDragBoxControl::GetGuidesPt(int index, QPointF &pt)
{
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    QTransform transform = GetRotatedTransform(baseRc, mAngle);
    index = qBound(0, index, m_KeyPointGroup.GetKeyPointCount() - 1);
    pt = transform.map(m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint());
    if (m_KeyPointGroup.GetKeyPoint(index)->IsPressedOn())
    {
        return false;
    }        
    else
    {
        return true;
    }
}

int IFSDragBoxControl::GetKeyPointCount()
{
    return m_KeyPointGroup.GetKeyPointCount();
}

QTransform IFSDragBoxControl::GetTransform()
{
    QTransform transform = QTransform().scale(mScaleX, mScaleY)*QTransform().translate(mTransX, mTransY);
    return transform;
}


qreal IFSDragBoxControl::GetScaleDistance(QPointF currentPt, QPointF destPt, QPointF destPt1)
{
    qreal C = Distance(destPt1, currentPt);
    qreal B = Distance(destPt, destPt1);
    qreal A = Distance(destPt, currentPt);
    return qAbs((C*C + B*B - A*A) / (2 * B*C)*C);
}

bool IFSDragBoxControl::IsThreePointOnLine(QPointF destPt, QPointF leftPt, QPointF rightPt)
{
    if (qAbs((destPt.y() - leftPt.y()) * (destPt.x() - rightPt.x()) - (destPt.y() - rightPt.y()) * (destPt.x() - leftPt.x())) < 1e-4)
        return true;
    else
        return false;
}

qreal IFSDragBoxControl::Distance(QPointF Pt1, QPointF Pt2)
{
    return qSqrt((Pt1.x() - Pt2.x())*(Pt1.x() - Pt2.x()) + (Pt1.y() - Pt2.y())*(Pt1.y() - Pt2.y()));
}

double IFSDragBoxControl::GetAngle(QPointF ptCenter, QPointF ptLast, QPointF ptCurrent)
{
    double dRotateAngle0 = qAtan2(qFabs(ptCenter.x() - ptLast.x()), qFabs(ptCenter.y() - ptLast.y()));
    double dRotateAngle = qAtan2(qFabs(ptCenter.x() - ptCurrent.x()), qFabs(ptCenter.y() - ptCurrent.y()));
    if (ptCurrent.x() >= ptCenter.x())
    {
        if (ptCurrent.y() >= ptCenter.y())
        {
            dRotateAngle = dRotateAngle;
        }
        else
        {
            dRotateAngle = M_PI - dRotateAngle;
        }
    }
    else
    {
        if (ptCurrent.y() >= ptCenter.y())
        {
            dRotateAngle = 2 * M_PI - dRotateAngle;
        }
        else
        {
            dRotateAngle = M_PI + dRotateAngle;
        }
    }

    if (ptLast.x() >= ptCenter.x())
    {
        if (ptLast.y() >= ptCenter.y())
        {
            dRotateAngle0 = dRotateAngle0;
        }
        else
        {
            dRotateAngle0 = M_PI - dRotateAngle0;
        }
    }
    else
    {
        if (ptLast.y() >= ptCenter.y())
        {
            dRotateAngle0 = 2 * M_PI - dRotateAngle0;
        }
        else
        {
            dRotateAngle0 = M_PI + dRotateAngle0;
        }
    }
    if (qAbs((dRotateAngle0 * 180 / M_PI) - (dRotateAngle * 180 / M_PI)) > 270)
        return 0;
    return (dRotateAngle0 - dRotateAngle) * 180 / M_PI;
}

int IFSDragBoxControl::TransDragBoxType()
{
    int nAngle = mAngle;
    int absAngle = nAngle % 360;
    int nTrans = 0;
    if (((absAngle > -45) && (absAngle <= 45)) || (absAngle < -315) || (absAngle>315))
        nTrans = 0;
    else if (((absAngle > 45) && (absAngle <= 135)) || ((absAngle < -225) && (absAngle >= -315)))
        nTrans = 2;
    else if (((absAngle>135) && (absAngle <= 225)) || ((absAngle < -135) && (absAngle >= -225)))
        nTrans = 4;
    else
        nTrans = 6;
    return nTrans;
}

bool IFSDragBoxControl::Intersects(QRect rc)
{
    QPainterPath path,path1;
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    path.addRect(baseRc);
    path1.addRect(rc);
    path = GetRotatedTransform(baseRc, mAngle).map(path);
    return path1.intersects(path);
}

void IFSDragBoxControl::GetPath(QList<FSPainterPath> &listPath, QPointF offset)
{
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    QTransform transform = GetRotatedTransform(baseRc, mAngle);
    FSPainterPath linePath = m_KeyPointGroup.GetLinePath(m_ColorLinePath, m_PenStyle, 1.4);
    for (int i = 0; i < 8 && i < m_KeyPointGroup.GetKeyPointCount()-1; i++)
    {
        QPainterPath path = m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath().GetPath();
        linePath.SetPath(linePath.GetPath().subtracted(path));
        FSPainterPath tempPath = m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath();
        tempPath.SetPath(transform.map(path));
        tempPath.GetPath().translate(offset);
        tempPath.SetRichColor(true);
        listPath.push_back(tempPath);
    }
    linePath.SetPath(transform.map(linePath.GetPath()));
    linePath.GetPath().translate(offset);
    listPath.push_front(linePath);
    FSPainterPath tempPath = m_KeyPointGroup.GetKeyPoint(8)->GetCiyclePath();
    tempPath.SetPath(transform.map(tempPath.GetPath()));
    tempPath.GetPath().translate(offset);
    tempPath.SetAwaysFill(true);
    tempPath.SetRichColor(true);
    listPath.push_back(tempPath);
    FSPainterPath tempPath1 = m_KeyPointGroup.GetKeyPoint(9)->GetCiyclePath();
    tempPath1.SetPath(transform.map(tempPath1.GetPath()));
    tempPath1.GetPath().translate(offset);
    tempPath1.SetRichColor(true);
    listPath.push_back(tempPath1);
}

QSizeF IFSDragBoxControl::GetRenderScale()
{
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    mRender = QRectF(baseRc.left() / mBascRc.width(), baseRc.top() / mBascRc.height(), baseRc.width() / mBascRc.width(), baseRc.height() / mBascRc.height());
    return QSizeF(mRender.width(), mRender.height());
}

double IFSDragBoxControl::Angle()
{
    return mAngle;
}

void IFSDragBoxControl::Position(double &x, double &y)
{
    x = mTransX;
    y = mTransY;
}

void IFSDragBoxControl::Scale(double &x, double &y)
{
    x = mScaleX;
    y = mScaleY;
}


QRectF IFSDragBoxControl::GetRectFromCenterPt(QPointF Center, int width)
{
    return QRectF(Center.x() - width / 2, Center.y() - width / 2, width, width);
}

void IFSDragBoxControl::SetLinePathColor(QColor color)
{
    m_ColorLinePath = color;
}

void IFSDragBoxControl::SetControlPathColor(QColor color)
{
    m_ColorControlPath = color;
}

void IFSDragBoxControl::SetControlBoxFillColor(QColor color)
{
    m_ColorFillControlBox = color;
}

void IFSDragBoxControl::SetRolationFillColor(QColor color)
{
    m_ColorFillRolationBox = color;
}

void IFSDragBoxControl::SetLineWidth(float w)
{
    m_PathLineWidth = w;
}

void IFSDragBoxControl::SetLinePathStyle(Qt::PenStyle style)
{
    m_PenStyle = style;
}

void IFSDragBoxControl::InitKeyPointGroup()
{
    for (int i = 0; i < 8; i++)
    {
        FSKeyPoint pt(QPointF(), NULL);
        pt.SetIndex(i);
        m_KeyPointGroup.AddKeyPoint(pt, m_ColorControlPath, m_ColorFillControlBox, Qt::SolidLine, m_PathLineWidth, false);
    }
    FSKeyPoint pt(QPointF(), NULL);
    pt.SetIndex(8);
    m_KeyPointGroup.AddKeyPoint(pt, m_ColorControlPath, m_ColorFillRolationBox, Qt::SolidLine, m_PathLineWidth, false);
    pt.SetIndex(9);
    m_KeyPointGroup.AddKeyPoint(pt, m_ColorControlPath, m_ColorFillControlBox, Qt::SolidLine, m_PathLineWidth, false);
    m_KeyPointGroup.AddKeyPoint(pt, m_ColorControlPath, m_ColorFillControlBox, Qt::SolidLine, m_PathLineWidth, false);
}

bool IFSDragBoxControl::IsPressedOn(int& index)
{
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        if (m_KeyPointGroup.GetKeyPoint(i)->IsPressedOn())
        {
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
            return true;
        }
    }
    index = CENTER + 1;
    return false;
}

void IFSDragBoxControl::ClearKeyPointState()
{
    m_KeyPointGroup.ClearKeyPointState();
}

bool IFSDragBoxControl::IsPressedOnControlBox(QPointF pos, QPointF offSet,int &index)
{
    QRectF rc = GetTransform().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    bool bPressedOn = false;
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount() - 1; i++)
    {
        bPressedOn = tempTransform.map(m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath().GetPath()).translated(offSet).contains(pos);
        if (bPressedOn)
        {
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
            return true;
        }            
    }
    return bPressedOn;
}

bool IFSDragBoxControl::IsPressedOnRectArea(QPointF pos, QPointF offSet)
{
    QRectF rc = GetTransform().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    QPainterPath path;
    path.addRect(m_KeyPointGroup.GetKeyPoint(m_KeyPointGroup.GetKeyPointCount() - 1)->GetRectFromCenterPt());
    return tempTransform.map(path).translated(offSet).contains(pos);
}

int IFSDragBoxControl::GetPressedOnIndex()
{
    bool bPressedOn = false;
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        if (m_KeyPointGroup.GetKeyPoint(i)->IsPressedOn())
            return m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
    }
    return -1;
}

void IFSDragBoxControl::RolationSnap(double &angle)
{
    if (qAbs(360 - angle) <4 || qAbs(0 - angle) <4)
        angle = 0;    
    else if (qAbs(90 - angle) <4)
        angle = 90;  
    else if (qAbs(180 - angle) <4)
        angle = 180;  
    else if (qAbs(270 - angle) <4)
        angle = 270;
}

void IFSDragBoxControl::SetShiftFlag(bool bPressedShift)
{
    m_bPressedShift = bPressedShift;
}

QTransform IFSDragBoxControl::GetRotatedTransform(QRectF rc, double angle)
{
    QTransform transform;
    transform.rotate(angle);
    QRectF rc1 = transform.mapRect(rc);
    transform = transform*QTransform().translate(rc.center().x() - rc1.center().x(), rc.center().y() - rc1.center().y());
    return transform;
}



QRectF IFSDragBoxControl::GetRealRenderRectf(QRectF renderRect)
{
    if (renderRect.width() < 1e-8 || renderRect.height() < 1e-8)
        return QRectF();
    return QRectF(renderRect.left()*mBascRc.width(), renderRect.top()*mBascRc.height(), renderRect.width()*mBascRc.width(), renderRect.height()*mBascRc.height());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSMaskBoxControl::FSMaskBoxControl(QObject *pParent)
    :IFSDragBoxControl(pParent)
    , m_SourceScaleX(1.0)
    , m_SourceScaleY(1.0)
{
}

FSMaskBoxControl::~FSMaskBoxControl()
{

}

void FSMaskBoxControl::SetScale(double x, double y)
{
    double xchange = x / m_SourceScaleX;
    double ychange = y / m_SourceScaleY;
    m_SourceScaleX = x;
    m_SourceScaleY = y;
    mScaleX = mScaleX*xchange;
    mScaleY = mScaleY*ychange;
    mTransX = (mTransX - m_SourceTransX)*xchange + m_SourceTransX;
    mTransY = (mTransY - m_SourceTransY)*xchange + m_SourceTransY;
}

QString FSMaskBoxControl::MaskPathPoint()
{
    QRectF rc = GetTransform().mapRect(mBascRc);
    QRectF rcSrc = GetOriginTransForm().mapRect(mBascRc);
    rc = QRectF(rc.topLeft() - rcSrc.topLeft(), rc.size());

    QString str = QString("%1,%2,%3,%4,%5,%6,%7,%8").arg(rc.topLeft().x() / rcSrc.width()).arg(rc.topLeft().y() / rcSrc.height())
        .arg(rc.topRight().x() / rcSrc.width()).arg(rc.topRight().y() / rcSrc.height())
        .arg(rc.bottomRight().x() / rcSrc.width()).arg(rc.bottomRight().y() / rcSrc.height())
        .arg(rc.bottomLeft().x() / rcSrc.width()).arg(rc.bottomLeft().y() / rcSrc.height());
    return str;
}

void FSMaskBoxControl::StartMaskMode(const QString &maskPath)
{
    QRectF rcSrc = GetOriginTransForm().mapRect(mBascRc);
    QStringList list = maskPath.split(",",QString::SkipEmptyParts);
    if (list.size() != 8)
    {
        mScaleX = m_SourceScaleX;
        mScaleY = m_SourceScaleY;
        mTransX = m_SourceTransX;
        mTransY = m_SourceTransY;
    }
    else
    {
        QRectF rc(QPointF(list.at(0).toDouble(), list.at(1).toDouble()), QPointF(list.at(4).toDouble(), list.at(5).toDouble()));
        mScaleX = m_SourceScaleX*rc.width();
        mScaleY = m_SourceScaleY*rc.height();
        mTransX = rc.left()*rcSrc.width() + m_SourceTransX;
        mTransY = rc.top()*rcSrc.height() + m_SourceTransY;
    }   
    UpdataKeyPointGroup();
}


void FSMaskBoxControl::DoResize(const QString &maskPath)
{
    QRectF realRc = GetRealRenderRectf(mRender);
    QPointF realRenderRectCenter = realRc.center();
    QTransform transform = GetRotatedTransform(realRc, -mAngle);
    m_SourceScaleX = mRender.width();
    m_SourceScaleY = mRender.height();
    QPointF renderRectCenter = QTransform().scale(m_SourceScaleX, m_SourceScaleY).mapRect(mBascRc).center();
    m_SourceTransX = realRenderRectCenter.x() - renderRectCenter.x();
    m_SourceTransY = realRenderRectCenter.y() - renderRectCenter.y();
    QRectF rcSrc = GetOriginTransForm().mapRect(mBascRc);
    QStringList list = maskPath.split(",", QString::SkipEmptyParts);
    if (list.size() == 8)
    {
        QRectF rc(QPointF(list.at(0).toDouble(), list.at(1).toDouble()), QPointF(list.at(4).toDouble(), list.at(5).toDouble()));
        mScaleX = m_SourceScaleX*rc.width();
        mScaleY = m_SourceScaleY*rc.height();
        mTransX = rc.left()*rcSrc.width() + m_SourceTransX;
        mTransY = rc.top()*rcSrc.height() + m_SourceTransY;
    }
    else
    {
        mScaleX = m_SourceScaleX;
        mScaleY = m_SourceScaleY;
        mTransX = m_SourceTransX;
        mTransY = m_SourceTransY;
    }
    UpdataKeyPointGroup();
}



void FSMaskBoxControl::ItemTranslate(QPointF offSet, int index)
{
    float Scalex = 1.0;
    float Scaley = 1.0;
    index = qBound(0, index, 7);
    QPointF destPt  = m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint();
    QPointF destPt1 = m_KeyPointGroup.GetKeyPoint((index + 4) % 8)->GetCenterPoint();
    QPointF leftPt = m_KeyPointGroup.GetKeyPoint((index + 7) % 8)->GetCenterPoint();
    QPointF rightPt = m_KeyPointGroup.GetKeyPoint((index + 1) % 8)->GetCenterPoint();

    QPointF ptIndex2 = m_KeyPointGroup.GetKeyPoint((index + 2) % 8)->GetCenterPoint();
    QPointF ptIndex5 = m_KeyPointGroup.GetKeyPoint((index + 5) % 8)->GetCenterPoint();
    QPointF ptIndex6 = m_KeyPointGroup.GetKeyPoint((index + 6) % 8)->GetCenterPoint();
    QTransform transform;

    QTransform transformScale = GetScaleTransform(destPt, destPt1, leftPt, rightPt, offSet, index);
    Scalex = transformScale.m11();
    Scaley = transformScale.m22();
    QRectF rc = GetTransform().mapRect(mBascRc);
    QRectF rcSource = GetOriginTransForm().mapRect(mBascRc);

    QPointF Pt = QTransform().scale(Scalex, Scaley).map(destPt1);
    QPointF transPt(-Pt.x()+destPt1.x(), - Pt.y()+destPt1.y());

    transform = GetTransform()*QTransform().scale(Scalex, Scaley)*QTransform().translate(transPt.x(), transPt.y());
    mScaleX = transform.m11();
    mScaleY = transform.m22();
    mTransX = transform.m31();
    mTransY = transform.m32();
    UpdataKeyPointGroup();
    emit MaskPathChanged();
}

void FSMaskBoxControl::DoTranslateEvent(CropItem item, QPointF currentPt)
{
    QPointF offSet = currentPt - mPointLast;
    double x, y;
    switch (item)
    {
    case IFSDragBoxControl::CENTER:
    {
        x = offSet.x();
        y = offSet.y();
        mTransX += x;
        mTransY += y;
        m_SourceTransX += x;
        m_SourceTransY += y;
        UpdataKeyPointGroup();
        emit TransFormChanged();
    }      
        break;
    case IFSDragBoxControl::ANGEL_PT:
    {
        double x, y;
        QRectF srcRc;
        srcRc = GetOriginTransForm().mapRect(mBascRc);
        double angle = GetAngle(srcRc.center(), mPointLast, currentPt);
        int angleTemp;
        double angleTemp1;
        mAngle = mAngle + angle;
        angleTemp  = mAngle;
        angleTemp1 = qAbs(mAngle - angleTemp);
        if (angleTemp > 0)
        {
            angleTemp = angleTemp % 360;
            mAngle = angleTemp + angleTemp1;
        }
        else
        {
            angleTemp = qAbs(angleTemp % 360);
            mAngle = 360 - angleTemp - angleTemp1;
        }
        RolationSnap(mAngle);
        UpdataKeyPointGroup();
        emit TransFormChanged();
    }        
        break;
    default:
        ItemTranslate(offSet, item);
        break;
    }
}


QTransform FSMaskBoxControl::GetScaleTransform(QPointF destPt, QPointF destPt1, QPointF leftPt, QPointF rightPt, QPointF offSetPt, int index)
{
    QTransform transform;
    float Scalex = 1.0;
    float Scaley = 1.0;
    if (IsThreePointOnLine(leftPt, destPt, rightPt))
    {
        qreal distance = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(destPt1));
        qreal distance1 = Distance(destPt, destPt1);
        if (index == 1 || index == 5)
        {
            if (distance <= 2 * mControlItemWidht)
            {
                Scaley = 1.0;
            }
            else
            {
                if (mScaleY*distance / distance1 < m_SourceScaleY&&IsContainPt(destPt,offSetPt))
                    Scaley = distance / distance1;
            }

        }
        else
        {
            if (distance <= 2 * mControlItemWidht)
            {
                Scalex = 1.0;
            }
            else
            {
                if (mScaleX*distance / distance1 < m_SourceScaleX &&IsContainPt(destPt,offSetPt))
                    Scalex = distance / distance1;
            }
        }
    }
    else
    {
        if (index == 0 || index == 4)
        {
            qreal distanceX1 = 2 * Distance(destPt, rightPt);
            qreal distanceY1 = 2 * Distance(destPt, leftPt);

            qreal distanceX = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(rightPt)) + distanceX1 / 2;
            qreal distanceY = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(leftPt)) + distanceY1 / 2;

            if (distanceX > 2 * mControlItemWidht)
            {
                if (mScaleX*distanceX / distanceX1 < m_SourceScaleX&&IsContainPt(destPt, offSetPt) )
                    Scalex = distanceX / distanceX1;
            }
            if (distanceY > 2 * mControlItemWidht)
            {
                if (mScaleY*distanceY / distanceY1 < m_SourceScaleY&&IsContainPt(destPt,offSetPt))
                    Scaley = distanceY / distanceY1;
            }
        }
        else
        {
            qreal distanceY1 = 2 * Distance(destPt, rightPt);
            qreal distanceX1 = 2 * Distance(destPt, leftPt);

            qreal distanceY = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(rightPt)) + distanceY1 / 2;
            qreal distanceX = GetScaleDistance(QTransform().rotate(mAngle).map(destPt) + offSetPt, QTransform().rotate(mAngle).map(destPt), QTransform().rotate(mAngle).map(leftPt)) + distanceX1 / 2;

            if (distanceX > 2 * mControlItemWidht)
            {
                if (mScaleX*distanceX / distanceX1 < m_SourceScaleX&&IsContainPt(destPt,offSetPt))
                    Scalex = distanceX / distanceX1;
            }
            if (distanceY > 2 * mControlItemWidht)
            {
                if (mScaleY*distanceY / distanceY1 < m_SourceScaleY&&IsContainPt(destPt,offSetPt))
                    Scaley = distanceY / distanceY1;
            }
        }
    }
    return QTransform().scale(Scalex, Scaley);
    
}

void FSMaskBoxControl::UpdataKeyPointGroup()
{
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    QRectF srcRc = GetOriginTransForm().mapRect(mBascRc);
    if (m_KeyPointGroup.GetKeyPointCount() >= 11)
    {
        m_KeyPointGroup.GetKeyPoint(0)->SetCenterPointF(baseRc.topLeft());
        m_KeyPointGroup.GetKeyPoint(1)->SetCenterPointF(QPointF(baseRc.center().x(), baseRc.topLeft().y()));
        m_KeyPointGroup.GetKeyPoint(2)->SetCenterPointF(baseRc.topRight());
        m_KeyPointGroup.GetKeyPoint(3)->SetCenterPointF(QPointF(baseRc.topRight().x(), baseRc.center().y()));
        m_KeyPointGroup.GetKeyPoint(4)->SetCenterPointF(baseRc.bottomRight());
        m_KeyPointGroup.GetKeyPoint(5)->SetCenterPointF(QPointF(baseRc.center().x(), baseRc.bottomLeft().y()));
        m_KeyPointGroup.GetKeyPoint(6)->SetCenterPointF(baseRc.bottomLeft());
        m_KeyPointGroup.GetKeyPoint(7)->SetCenterPointF(QPointF(baseRc.topLeft().x(), baseRc.center().y()));
        m_KeyPointGroup.GetKeyPoint(8)->SetCenterPointF(srcRc.center() + QPointF(4 * m_KeyPointGroup.GetKeyPoint(8)->GetItemSize().width(), 0));
        m_KeyPointGroup.GetKeyPoint(9)->SetCenterPointF(srcRc.center());
        m_KeyPointGroup.GetKeyPoint(10)->SetCenterPointF(baseRc.center());
        m_KeyPointGroup.GetKeyPoint(10)->SetItemSize(baseRc.width(), baseRc.height());
    }
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        m_KeyPointGroup.GetKeyPoint(i)->SetPathColor(m_ColorControlPath);
        m_KeyPointGroup.GetKeyPoint(i)->SetFillPathColor(i != 8 ? m_ColorFillControlBox : m_ColorFillRolationBox);
    }

    mRender = QRectF(srcRc.left() / mBascRc.width(), srcRc.top() / mBascRc.height(), srcRc.width() / mBascRc.width(), srcRc.height() / mBascRc.height());
}

void FSMaskBoxControl::GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt)
{
    index = qBound(0, index, 7);
    QPointF offSet = curPt - lastPt;
    QPointF destPt = m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint();
    QPointF destPt1 = m_KeyPointGroup.GetKeyPoint((index + 4) % 8)->GetCenterPoint();
    QPointF leftPt = m_KeyPointGroup.GetKeyPoint((index + 7) % 8)->GetCenterPoint();
    QPointF rightPt = m_KeyPointGroup.GetKeyPoint((index + 1) % 8)->GetCenterPoint();

    QPointF ptIndex2 = m_KeyPointGroup.GetKeyPoint((index + 2) % 8)->GetCenterPoint();
    QPointF ptIndex5 = m_KeyPointGroup.GetKeyPoint((index + 5) % 8)->GetCenterPoint();
    QPointF ptIndex6 = m_KeyPointGroup.GetKeyPoint((index + 6) % 8)->GetCenterPoint();
    QTransform transformScale = GetScaleTransform(destPt, destPt1, leftPt, rightPt, offSet, index);
    scalePt.setX(transformScale.m11());
    scalePt.setY(transformScale.m22());
}

void FSMaskBoxControl::GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle)
{
    QRectF tempCenterRect = GetTransform().mapRect(mBascRc);
    angle = GetAngle(tempCenterRect.center(), lastPt, curPt);
}

void FSMaskBoxControl::SetScaleWithDelta(QPointF deltaScale, int index)
{
    index = qBound(0, index, 7);
    m_KeyPointGroup.GetKeyPoint(index)->SetPressedOn(true);
    QPointF destPt1 = m_KeyPointGroup.GetKeyPoint((index + 4) % 8)->GetCenterPoint();
    QRectF rc = GetTransform().mapRect(mBascRc);
    QRectF rcSource = GetOriginTransForm().mapRect(mBascRc);

    QPointF Pt = QTransform().scale(deltaScale.x(), deltaScale.y()).map(destPt1);
    QPointF transPt(-Pt.x() + destPt1.x(), -Pt.y() + destPt1.y());

    QTransform transform = GetTransform()*QTransform().scale(deltaScale.x(), deltaScale.y())*QTransform().translate(transPt.x(), transPt.y());
    mScaleX = transform.m11();
    mScaleY = transform.m22();
    mTransX = transform.m31();
    mTransY = transform.m32();
    UpdataKeyPointGroup();
    emit MaskPathChanged();
}

void FSMaskBoxControl::SetAngleWithDelta(double deltaAngle, bool bSnap)
{
    int angleTemp;
    double angleTemp1;
    mAngle = mAngle + deltaAngle;
    angleTemp = mAngle;
    angleTemp1 = qAbs(mAngle - angleTemp);
    if (angleTemp > 0)
    {
        angleTemp = angleTemp % 360;
        mAngle = angleTemp + angleTemp1;
    }
    else
    {
        angleTemp = qAbs(angleTemp % 360);
        mAngle = 360 - angleTemp - angleTemp1;
    }
    if (bSnap)
        RolationSnap(mAngle);
    UpdataKeyPointGroup();
    emit TransFormChanged();
}



void FSMaskBoxControl::SetPosWithDelta(QPointF deltaPos)
{
    mTransX = mTransX + deltaPos.x();
    mTransY = mTransY + deltaPos.y();
    m_SourceTransX += deltaPos.x();
    m_SourceTransY += deltaPos.y();
    UpdataKeyPointGroup();
    emit TransFormChanged();
}

void FSMaskBoxControl::DoMouseRealese(QPointF curPt)
{
    ClearKeyPointState();
    UpdataKeyPointNeedFillState(curPt);
}

int FSMaskBoxControl::UpdataKeyPointNeedFillState(QPointF pt)
{
    QRectF rc = GetOriginTransForm().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    int index = CENTER + 1;
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        bool bNeedFill = tempTransform.map(m_KeyPointGroup.GetKeyPoint(i)->GetRectPath().GetPath()).contains(pt);
        if (!m_KeyPointGroup.GetKeyPoint(i)->IsPressedOn())
        {
            if (bNeedFill != m_KeyPointGroup.GetKeyPoint(i)->IsNeedFill())
            {
                m_KeyPointGroup.GetKeyPoint(i)->SetNeedFill(bNeedFill);
                emit sigRepaintWidget();
            }
        }
        if (bNeedFill&&index == CENTER + 1)
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
    }
    emit CursorShapeChanged(ChangeCursor(index < ANGEL_PT ? (index + TransDragBoxType()) % 8 : index));
    return index;
}

int FSMaskBoxControl::UpdataKeyPointPressedOnState(QPointF pt)
{
    QRectF rc = GetOriginTransForm().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    int index = CENTER + 1;
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        bool bPressedOn = tempTransform.map(m_KeyPointGroup.GetKeyPoint(i)->GetRectPath().GetPath()).contains(pt);
        if (bPressedOn&index == CENTER + 1)
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
        m_KeyPointGroup.GetKeyPoint(i)->SetPressedOn(bPressedOn);
    }
    return index;
}



QTransform FSMaskBoxControl::GetOriginTransForm()
{
    QTransform transform = QTransform().scale(m_SourceScaleX, m_SourceScaleY)*QTransform().translate(m_SourceTransX, m_SourceTransY);
    return transform;
}


bool FSMaskBoxControl::IsContainPt(QPointF pt, QPointF offSet)
{
    QPainterPath path;
    QRectF srcRc = GetOriginTransForm().mapRect(mBascRc);
    path.addRect(srcRc);
    QTransform transform = GetRotatedTransform(srcRc, mAngle);
    path = transform.map(path);
    return path.contains(transform.map(pt)+offSet);
}

void FSMaskBoxControl::initTransformInfo(double angle, double scaleX, double scaleY, double posX, double posY, QRectF baseRc)
{
    mAngle = angle;

    double xchange = scaleX / m_SourceScaleX;
    double ychange = scaleY / m_SourceScaleY;
    m_SourceScaleX = scaleX;
    m_SourceScaleY = scaleY;
    mScaleX = mScaleX*xchange;
    mScaleY = mScaleY*ychange;
    mTransX = mTransX - m_SourceTransX + posX;
    mTransY = mTransY - m_SourceTransY + posY;
    m_SourceTransX = posX;
    m_SourceTransY = posY;
    mBascRc = baseRc;
    UpdataKeyPointGroup();
}


void FSMaskBoxControl::DoPaintControlBoxEvent(QPainter &painter, QPointF pt)
{
    UpdataKeyPointGroup();
    return;//Íâ²ã»­¿ò
}

void FSMaskBoxControl::Position(double &x, double &y)
{
    x = m_SourceTransX;
    y = m_SourceTransY;
}

void FSMaskBoxControl::Scale(double &x, double &y)
{
    x = m_SourceScaleX;
    y = m_SourceScaleY;
}

void FSMaskBoxControl::SetPostion(double x, double y)
{
    double changedx = x - m_SourceTransX;
    double changedy = y - m_SourceTransY;
    m_SourceTransY = y;
    m_SourceTransX = x;
    mTransX += changedx;
    mTransY += changedy;
}


QSizeF FSMaskBoxControl::GetRenderScale()
{
    QRectF baseRc = GetOriginTransForm().mapRect(mBascRc);
    mRender = QRectF(baseRc.left() / mBascRc.width(), baseRc.top() / mBascRc.height(), baseRc.width() / mBascRc.width(), baseRc.height() / mBascRc.height());
    return QSizeF(mRender.width(), mRender.height());
}

bool FSMaskBoxControl::Intersects(QRect rc)
{
    QPainterPath path, path1;
    QRectF baseRc = GetTransform().mapRect(mBascRc);
    QRectF srcRc = GetOriginTransForm().mapRect(mBascRc);
    path.addRect(baseRc);
    path1.addRect(rc);
    path = GetRotatedTransform(srcRc, mAngle).map(path);
    return path1.intersects(path);
}

void FSMaskBoxControl::GetPath(QList<FSPainterPath> &listPath, QPointF offset)
{
    QRectF srcRc  = GetOriginTransForm().mapRect(mBascRc);
    QTransform transform = GetRotatedTransform(srcRc, mAngle);
    FSPainterPath linePath = m_KeyPointGroup.GetLinePath(m_ColorLinePath, m_PenStyle, m_PathLineWidth);
    for (int i = 0; i < 8 && i < m_KeyPointGroup.GetKeyPointCount() - 1; i++)
    {
        QPainterPath path = m_KeyPointGroup.GetKeyPoint(i)->GetRectPath().GetPath();
        linePath.SetPath(linePath.GetPath().subtracted(path));
        FSPainterPath tempPath = m_KeyPointGroup.GetKeyPoint(i)->GetRectPath();
        tempPath.SetPath(transform.map(path));
        tempPath.GetPath().translate(offset);
        tempPath.SetRichColor(true);
        listPath.push_back(tempPath);
    }
    linePath.SetPath(transform.map(linePath.GetPath()));
    linePath.GetPath().translate(offset);
    listPath.push_front(linePath);
    FSPainterPath tempPath = m_KeyPointGroup.GetKeyPoint(8)->GetCiyclePath();
    tempPath.SetPath(transform.map(tempPath.GetPath()));
    tempPath.GetPath().translate(offset);
    tempPath.SetAwaysFill(true);
    tempPath.SetRichColor(true);
    listPath.push_back(tempPath);
    FSPainterPath tempPath1 = m_KeyPointGroup.GetKeyPoint(9)->GetCiyclePath();
    tempPath1.SetPath(transform.map(tempPath1.GetPath()));
    tempPath1.GetPath().translate(offset);
    tempPath1.SetRichColor(true);
    listPath.push_back(tempPath1);
}

bool FSMaskBoxControl::IsPressedOnControlBox(QPointF pos, QPointF offSet, int &index)
{
    QRectF rc = GetOriginTransForm().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    bool bPressedOn = false;
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount()-1; i++)
    {
        bPressedOn = tempTransform.map(m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath().GetPath()).translated(offSet).contains(pos);
        if (bPressedOn)
        {
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
            return true;
        }
            
    }
    return bPressedOn;
}

bool FSMaskBoxControl::IsPressedOnRectArea(QPointF pos, QPointF offSet)
{
    QRectF rc = GetOriginTransForm().mapRect(mBascRc);
    QTransform tempTransform;
    tempTransform = GetRotatedTransform(rc, mAngle);
    QPainterPath path;
    path.addRect(m_KeyPointGroup.GetKeyPoint(m_KeyPointGroup.GetKeyPointCount() - 1)->GetRectFromCenterPt());
    return tempTransform.map(path).translated(offSet).contains(pos);
}

QList<QPointF > FSMaskBoxControl::GetGuidesPtList()
{
    QRectF rc = GetOriginTransForm().mapRect(mBascRc);
    QTransform transform;
    transform = GetRotatedTransform(rc, mAngle);
    QList<QPointF> ptList = m_KeyPointGroup.GetGuidesPtList(0, 8);
    for (int i = 0; i < ptList.size(); i++)
    {
        ptList[i] = transform.map(ptList.at(i));
    }
    return ptList;
}

QList<QPointF> FSMaskBoxControl::GetTargetPtList(bool IsSelect)
{
    if (IsSelect)
    {
        int index = m_KeyPointGroup.GetPressedOnPtIndex();
        QRectF rc = GetOriginTransForm().mapRect(mBascRc);
        QTransform transform;
        transform = GetRotatedTransform(rc, mAngle);
        if (index != -1 && index < 8)
        {
            return QList<QPointF>() << transform.map(m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint());
        }
        else
        {
            return GetGuidesPtList();
        }
    }
    return QList<QPointF>();
}

bool FSMaskBoxControl::GetGuidesPt(int index, QPointF &pt)
{
    QRectF srcRc = GetOriginTransForm().mapRect(mBascRc);
    QTransform transform = GetRotatedTransform(srcRc, mAngle);
    index = qBound(0, index, m_KeyPointGroup.GetKeyPointCount() - 1);
    pt = transform.map(m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint());
    if (m_KeyPointGroup.GetKeyPoint(index)->IsPressedOn())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void FSMaskBoxControl::GetTransformInfo(double &angle, double &scaleX, double &scaleY, double &posX, double &posY, QRectF &baseRc)
{
    angle = mAngle;
    scaleX = m_SourceScaleX;
    scaleY = m_SourceScaleY;
    posX = m_SourceTransX;
    posY = m_SourceTransY;
    baseRc = mBascRc;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSScaleBoxControl::FSScaleBoxControl(QObject *pParent)
    :IFSDragBoxControl(pParent)
{

}

FSScaleBoxControl::~FSScaleBoxControl()
{

}