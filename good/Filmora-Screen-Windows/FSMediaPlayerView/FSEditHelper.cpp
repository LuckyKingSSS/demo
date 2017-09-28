#include "stdafx.h"
#include "FSEditHelper.h"
#include "FSDragBoxControl.h"
#include "inc_CommonLib/NLEWraper.h"
#include "INLEMouseEffectManager.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "FSUnDoReDoCommand.h"
#include "inc_MediaPlayerView/MediaPlayerControl.h"


IFSEditHelper::IFSEditHelper(FSNLETimeline *pTimeLine, IMediaPlayerControl *pPlayerControl, QObject *parent)
    :QObject(parent),
    mControlItemWidht(8),
    mBorderWidth(0),
    mPositionScaleX(1.0),
    mPositionScaleY(1.0),
    mbSelected(false), 
    m_bScaleMode(true),
    mCursorShape(Qt::CursorShape::ArrowCursor),
    m_bDragingFlag(false)
{
    Q_ASSERT(parent);
    Q_ASSERT(pTimeLine);
    mpTimeLine = pTimeLine;
    mpParentWidget = static_cast<QWidget *> (parent);
    mpPlayerControl = pPlayerControl;
   
}



IFSEditHelper::~IFSEditHelper()
{

}


bool IFSEditHelper::Resetable()
{
    if (GetEditType() == Edit_Audio)
        return false;
    else{
        double scalex, scaley, transx, transy, angle;
        double scalex1, scaley1, transx1, transy1, angle1;
        GetDefaultAngle(angle);
        GetDefaultScale(scalex, scaley);
        GetDefaultPosiont(transx, transy);
        GetPosition(transx1, transy1);
        GetScale(scalex1, scaley1);
        angle1 = GetAngle();
        return qAbs(scalex - scalex1) > 1e-2
            ||qAbs(scaley - scaley1) > 1e-2
            ||qAbs(transx - transx1) > 1e-2
            ||qAbs(transy - transy1) > 1e-2
            ||qAbs(angle - angle1) > 1e-2;
    }    
}

IFSEditHelper::EditType IFSEditHelper::GetEditType()
{
    TimelineType type;
    if (mpTimeLine)
        type = mpTimeLine->GetType();
    EditType tempType;
    tempType = (type == TimelineType::TimelineType_Annotaions_Caption || type == TimelineType::TimelineType_Annotaions_Element) ? Edit_Text : (type == TimelineType::TimelineType_Video || type == TimelineType::TimelineType_VideoAudio || type == TimelineType::TimelineType_Annotaions_Overlay) ? Edit_Video : (type == TimelineType::TimelineType_Audio) ? Edit_Audio : Edit_Picture;
    return tempType;
};

bool IFSEditHelper::GetDefaultPosiont(double &x, double &y)
{
    QString value= property(Default_Position).toString();
    QStringList list;
    if (value.isEmpty() || (list=value.split(":", QString::SkipEmptyParts)).size() < 2)
    {
        return false;
    }
    
    x = list.first().toDouble();
    y = list.last().toDouble();
    return true;
}

bool IFSEditHelper::GetDefaultScale(double &x, double &y)
{
    QString value = property(Default_Scale).toString();
    QStringList list;
    if (value.isEmpty() || (list = value.split(":", QString::SkipEmptyParts)).size() < 2)
    {
        return false;
    }
    x = list.first().toDouble();
    y = list.last().toDouble();
    return true;
}

bool IFSEditHelper::GetDefaultAngle(double &angle)
{
    QString value = property(Default_Scale).toString();
    if (value.isEmpty())
    {
        return false;
    }
    angle = value.toDouble();
    return true;
}

bool IFSEditHelper::GetDefaultVolume(int &volume)
{
    QString value = property(Default_AudioVolume).toString();
    if (value.isEmpty())
    {
        return false;
    }
    volume = value.toDouble();
    return true;
}

bool IFSEditHelper::GetDefaultTextBgColor(QColor &color)
{
    QString value = property(Default_TextBgColor).toString();
    if (value.isEmpty())
    {
        return false;
    }
    color = QColor(value);
    return true;
}

bool IFSEditHelper::GetDefaultTextBgOptacity(int &optacity)
{
    QString value = property(Default_TextBgOptacity).toString();
    if (value.isEmpty())
    {
        return false;
    }
    optacity = value.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultTextShapeBgColor(QColor &color)
{
    QString value = property(Default_TextShapeBgColor).toString();
    if (value.isEmpty())
    {
        return false;
    }
    color = QColor(value);
    return true;
}

bool IFSEditHelper::GetDefaultTextBorderBgColor(QColor &color)
{
    QString value = property(Default_TextBorderBgColor).toString();
    if (value.isEmpty())
    {
        return false;
    }
    color = QColor(value);
    return true;
}

bool IFSEditHelper::GetDefaultTextBorderWidht(int &width)
{
    QString value = property(Default_TextBorderWidth).toString();
    if (value.isEmpty())
    {
        return false;
    }
    width = value.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultFontName(QString &fontName)
{
    QString value = property(Default_FontName).toString();
    if (value.isEmpty())
    {
        return false;
    }
    fontName = value;
    return true;
}

bool IFSEditHelper::GetDefaultFontSize(int &size)
{
    QString value = property(Default_FontSize).toString();
    if (value.isEmpty())
    {
        return false;
    }
    size = value.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultFontBold(bool &bBold)
{
    QString value = property(Default_FontBold).toString();
    if (value.isEmpty())
    {
        return false;
    }
    bBold = value.toInt()>0;
    return true;
}

bool IFSEditHelper::GetDefaultFontItalic(bool &bItalic)
{
    QString value = property(Default_FontItalic).toString();
    if (value.isEmpty())
    {
        return false;
    }
    bItalic = value.toInt() > 0;
    return true;
}

bool IFSEditHelper::GetDefaultFontColor(QColor &color)
{
    QString value = property(Default_FontTextColor).toString();
    if (value.isEmpty())
    {
        return false;
    }
    color = QColor(value);
    return true;
}

bool IFSEditHelper::GetDefaultShadowDerection(int &derection)
{
    QString value = property(Default_ShadowDerection).toString();
    if (value.isEmpty())
    {
        return false;
    }
    derection = value.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultTextAlignType(NLEAlignType &type)
{
    QString value = property(Default_TextAlignType).toString();
    if (value.isEmpty())
    {
        return false;
    }
    type = (NLEAlignType)value.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultCursorScale(double &scale)
{
    QString value = property(Default_CurssorScale).toString();
    if (value.isEmpty())
    {
        return false;
    }
    scale = value.toDouble();
    return true;
}

bool IFSEditHelper::GetDefaultCursorOptacity(double &optacity)
{
    QString value = property(Default_CurssorOptacity).toString();
    if (value.isEmpty())
    {
        return false;
    }
    optacity = value.toDouble();
    return true;
}

bool IFSEditHelper::GetDefaultCursorVolume(int &volume)
{
    QString value = property(Default_CurssorVolume).toString();
    if (value.isEmpty())
    {
        return false;
    }
    volume = value.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultCursorRingColor(QColor &color)
{
    QString value = property(Default_CurssorRingColor).toString();
    if (value.isEmpty())
    {
        return false;
    }
    color = QColor(value);
    return true;
}

bool IFSEditHelper::GetDefaultCursorHighlight(QColor &color)
{
    QString value = property(Default_CurssorHighlight).toString();
    if (value.isEmpty())
    {
        return false;
    }
    color = QColor(value);
    return true;
}

bool IFSEditHelper::GetDefaultDenoiseLevel(int &level)
{
    QVariant val = property(DefaultDenoiseLevel);
    if (val.isNull())
    {
        return false;
    }
    level = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultHotKeyEnable(bool &bEnable)
{
    QVariant val = property(HotKeyTipEnable);
    if (val.isNull())
    {
        return false;
    }
    bEnable = val.toBool();
    return true;
}

bool IFSEditHelper::GetDefaultHotKeySize(QString &size)
{
    QVariant val = property(HotKeySize);
    if (val.isNull())
    {
        return false;
    }
    size = val.toString();
    return true;
}

bool IFSEditHelper::GetDefaultHotKeyPosition(QString & pos)
{
    QVariant val = property(HotKeyPoision);
    if (val.isNull())
    {
        return false;
    }
    pos = val.toString();
    return true;
}

bool IFSEditHelper::GetDefaultHotKeyStyle(QString &style)
{
    QVariant val = property(HotKeyStyle);
    if (val.isNull())
    {
        return false;
    }
    style = val.toString();
    return true;
}

bool IFSEditHelper::GetDefaultHotKeyColor(QColor & color)
{
    QVariant val = property(HotKeyColor);
    if (val.isNull())
    {
        return false;
    }
    color = QColor(val.toString());
    return true;
}

bool IFSEditHelper::GetDefaultHotKeyOpacity(int &Opacity)
{
    QVariant val = property(HotKeyOpacity);
    if (val.isNull())
    {
        return false;
    }
    Opacity = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefaultHotKeyFliter(int &type)
{
    QVariant val = property(HotKeyFliter);
    if (val.isNull())
    {
        return false;
    }
    type = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtTextBorderColor(QColor &color)
{
    QVariant val = property(Defualt_TextBorderColor);
    if (val.isNull())
    {
        return false;
    }
    color = QColor(val.toString());
    return true;
}

bool IFSEditHelper::GetDefualtTextBorderSize(int &size)
{
    QVariant val = property(Default_TextBorderSize);
    if (val.isNull())
    {
        return false;
    }
    size = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtTextBorderOpacity(int &Opacity)
{
    QVariant val = property(Default_TextBorderOpacity);
    if (val.isNull())
    {
        return false;
    }
    Opacity = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtTextBorderBlur(int &blur)
{
    QVariant val = property(Default_TextBorderBlur);
    if (val.isNull())
    {
        return false;
    }
    blur = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtTextShadowDerection(int &derection)
{
    QVariant val = property(Defualt_TextShadowDerection);
    if (val.isNull())
    {
        return false;
    }
    derection = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtTextShadowColor(QColor &color)
{
    QVariant val = property(Defualt_TextShadowColor);
    if (val.isNull())
    {
        return false;
    }
    color = QColor(val.toString());
    return true;
}

bool IFSEditHelper::GetDefualtTextShadowDistance(int &distance)
{
    QVariant val = property(Default_TextShadowDistance);
    if (val.isNull())
    {
        return false;
    }
    distance = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtTextShadowOpacity(int &Opacity)
{
    QVariant val = property(Default_TextShadowOpacity);
    if (val.isNull())
    {
        return false;
    }
    Opacity = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtTextShadowBlur(int &blur)
{
    QVariant val = property(Default_TextShadowBlur);
    if (val.isNull())
    {
        return false;
    }
    blur = val.toInt();
    return true;
}

bool IFSEditHelper::GetDefualtSvgCaptionColors(QStringList &list)
{
    QVariant val = property(Default_CaptionSvgColors);
    if (val.isNull())
    {
        return false;
    }
    list = val.toString().split(",", QString::SkipEmptyParts);
    return true;
}

void IFSEditHelper::UpdateMouseEffectOrder(QString Effect)
{
    QString value = property(MouseEffectOrder).toString();
    if (value.isEmpty())
    {
        setProperty(MouseEffectOrder, Effect);
    }
    else
    {
        QStringList list = value.split(",",QString::SkipEmptyParts);
        list.removeOne(Effect);
        list.push_back(Effect);
        setProperty(MouseEffectOrder, list.join(","));
    }
}

QStringList IFSEditHelper::GetMouseEffectOrderList()
{
    return property(MouseEffectOrder).toString().split(",", QString::SkipEmptyParts);
}

void IFSEditHelper::Updata()
{
    if (mpParentWidget&&mpParentWidget->isVisible())
    {
        emit sigRepaintWidget();
    }
}

bool IFSEditHelper::IsScaleMode()
{
    return m_bScaleMode;
}

void IFSEditHelper::EnableScaleMode(bool bEnable)
{
    m_bScaleMode = bEnable;
}

bool IFSEditHelper::ControlBoxContains(QPointF pos, QPointF offSet, int &index)
{
    return false;
}

IFSEditHelper::SnapType IFSEditHelper::GetSnapType()
{
    return UnKnown;
}

void IFSEditHelper::GetPath(QList<FSPainterPath> &listPath, QPointF offSet)
{

}

QRectF IFSEditHelper::GetBaseRect()
{
    QSizeF size = mpPlayerControl->GetTimeline()->GetRenderSize();
    QSizeF size1 = mpParentWidget->size() - QSize(2 * mBorderWidth, 2 * mBorderWidth);
    if (size1.width()*size.height() > size1.height()*size.width())
    {
        double w = 1.0*size1.height()*size.width() / size.height();
        return QRectF(0, 0, w, size1.height());
    }
    else
    {
        double h = 1.0*size1.width()*size.height() / size.width();
        return QRectF(0, 0, size1.width(), h);
    }
}

QRectF IFSEditHelper::GetRealRenderRectf(QRectF renderRect,QRectF baseRc)
{
    if (renderRect.width() < 1e-8 || renderRect.height() < 1e-8)
        return QRectF();
    return QRectF(renderRect.left()*baseRc.width(), renderRect.top()*baseRc.height(), renderRect.width()*baseRc.width(), renderRect.height()*baseRc.height());
}

void IFSEditHelper::InitTransformInfo(QRectF renderRect, QRectF baseRc,double angle,double& scaleX, double& scaleY, double& transx, double& transy)
{
    //TODO
    QSizeF sizef = mpTimeLine->GetRenderResizeScale();
    QRectF realRc = GetRealRenderRectf(renderRect, baseRc);
    QPointF realRenderRectCenter = realRc.center();
    QTransform transform = GetRotatedTransform(realRc, -angle);
    scaleX = sizef.width();
    scaleY = sizef.height();
    QPointF renderRectCenter = QTransform().scale(scaleX, scaleY).mapRect(baseRc).center();
    transx = realRenderRectCenter.x()-renderRectCenter.x();
    transy = realRenderRectCenter.y()-renderRectCenter.y();
}

QRectF IFSEditHelper::GetRectFromCenterPt(QPointF Center, int width)
{
    return QRectF(Center.x() - 1.0*width / 2, Center.y() - 1.0*width / 2, width, width);
}

QTransform IFSEditHelper::GetRotatedTransform(QRectF rc, double angle)
{
    QTransform transform;
    transform.rotate(angle);
    QRectF rc1 = transform.mapRect(rc);
    transform = transform*QTransform().translate(rc.center().x() - rc1.center().x(), rc.center().y() - rc1.center().y());
    return transform;
}

double IFSEditHelper::GetRolationAngle(QPointF ptCenter, QPointF ptLast, QPointF ptCurrent)
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

bool IFSEditHelper::IsThreePointOnLine(QPointF destPt, QPointF leftPt, QPointF rightPt)
{
    if (qAbs((destPt.y() - leftPt.y()) * (destPt.x() - rightPt.x()) - (destPt.y() - rightPt.y()) * (destPt.x() - leftPt.x())) < 1e-2)
        return true;
    else
        return false;
}

qreal IFSEditHelper::Distance(QPointF Pt1, QPointF Pt2)
{
    return qAbs(qSqrt((Pt1.x() - Pt2.x())*(Pt1.x() - Pt2.x()) + (Pt1.y() - Pt2.y())*(Pt1.y() - Pt2.y())));
}

qreal IFSEditHelper::GetScaleDistance(QPointF currentPt, QPointF destPt, QPointF destPt1)
{
    qreal C = Distance(destPt1, currentPt);
    qreal B = Distance(destPt, destPt1);
    qreal A = Distance(destPt, currentPt);
    return qAbs((C*C + B*B - A*A) / (2 * B*C)*C);
}

void IFSEditHelper::InitDefaultPropertyValue()
{

}

void IFSEditHelper::SetBorderWidth(int width)
{
    mBorderWidth = width;
}

void IFSEditHelper::SetLastDenoiseLevel(int level)
{
    m_LastDenoiseLevel = qBound(0, level, 3);
}

int IFSEditHelper::GetLastDenoiseLevel()
{
    return m_LastDenoiseLevel;
}

void IFSEditHelper::SetPositionScale(double scaleX, double scaleY)
{
    mPositionScaleX = qBound(0.09, scaleX, 2.1);
    mPositionScaleY = qBound(0.09, scaleY, 2.1);
}

void IFSEditHelper::GetPositionScale(double &scaleX, double &scaleY)
{
    scaleX = mPositionScaleX;
    scaleY = mPositionScaleY;
}


void IFSEditHelper::SetDragingFlag(bool bDraging)
{
    m_bDragingFlag = bDraging;
}

void IFSEditHelper::DoResizeEvent(QResizeEvent *e)
{

}



///////////////////////////////////////////////////////////////////
/*******************文本编辑FSTextEditHelper***********************/
///////////////////////////////////////////////////////////////////

FSTextEditHelper::FSTextEditHelper(FSNLETimeline *TimeLine, IMediaPlayerControl *pControl, QObject *pParent)
    :IFSEditHelper(TimeLine, pControl, pParent)
    , mbChanaged(false)
    , m_pTextChangedCallBack(NULL)
    , m_pCaptionObjBeforeChangeCallBack(NULL)
    , mpCaption(NULL)
    , m_KeyPointGroup(this)
    ,m_ColorLinePath(220, 220, 220)
    , m_ColorControlPath(220, 220, 220)
    ,m_ColorFillControlBox(255, 255, 255)
    ,m_ColorFillRolationBox("#EF3062")
    ,m_PathLineWidth(1.6)
    , m_PenStyle(Qt::DashLine)
{
    InitTextChangeCallBack();
    InitCaptionObjBeforeChangeCallBack();
    
    
}

FSTextEditHelper::~FSTextEditHelper()
{

}

bool FSTextEditHelper::DoMousePressEvent(QMouseEvent *e)
{
    if (!mpParentWidget->isVisible())
        return false;
    mpParentWidget->setFocus();
    NLEShiftState ss = NLE_SS_NONE_TYPE;
    if (e->modifiers() & Qt::ShiftModifier)
        ss |= NLE_SS_SHIFT_TYPE;
    if (e->modifiers() & Qt::AltModifier)
        ss |= NLE_SS_ALT_TYPE;
    if (e->modifiers() & Qt::ControlModifier)
        ss |= NLE_SS_CTRL_TYPE;
    if (e->button() == Qt::LeftButton)
        ss |= NLE_SS_LEFT_TYPE;

    NLEMouseButton mb;
    if (e->button() == Qt::LeftButton)
        mb = NLE_MB_LEFT_TYPE;
    if (e->button() == Qt::RightButton)
        mb = NLE_MB_RIGHT_TYPE;
    if (e->button() == Qt::MiddleButton)
        mb = NLE_MB_MIDDLE_TYPE;
    mbChanaged = mpCaption->MouseDownEvent(e->pos().x() , e->pos().y(), ss, mb);
    
    if (mbChanaged)
    {
        UpdataKeyPointPressedOnState(e->pos());
    }
    return mbChanaged;
}

void FSTextEditHelper::DoMouseMoveEvent(QMouseEvent *event)
{
    
    if (!mpParentWidget->isVisible())
        return;
    NLEShiftState ss = NLE_SS_NONE_TYPE;
    if (event->modifiers() & Qt::ShiftModifier)
        ss |= NLE_SS_SHIFT_TYPE;
    if (event->modifiers() & Qt::AltModifier)
        ss |= NLE_SS_ALT_TYPE;
    if (event->modifiers() & Qt::ControlModifier)
        ss |= NLE_SS_CTRL_TYPE;
    if (event->buttons() & Qt::LeftButton)
        ss |= NLE_SS_LEFT_TYPE;

    if (IsSelected())
    {
        NLEMouseType mt;
        //UpdataKeyPointNeedFillState(event->pos());
        mpCaption->MouseMoveEvent(event->pos().x(), event->pos().y(), ss, &mt);
        //UpdataKeyPointGroup();
        //ChangeCurrsor(mt);
        /*if (mbChanaged)
        {
            emit TransformChanged();
        }*/
            
    }
    
}

void FSTextEditHelper::DoMouseUpEvent(QMouseEvent *event)
{
    if (!mpParentWidget->isVisible())
        return;
    NLEShiftState ss = NLE_SS_NONE_TYPE;
    if (event->modifiers() & Qt::ShiftModifier)
        ss |= NLE_SS_SHIFT_TYPE;
    if (event->modifiers() & Qt::AltModifier)
        ss |= NLE_SS_ALT_TYPE;
    if (event->modifiers() & Qt::ControlModifier)
        ss |= NLE_SS_CTRL_TYPE;
    if (event->button() == Qt::LeftButton)
        ss |= NLE_SS_LEFT_TYPE;

    NLEMouseButton mb;
    if (event->button() == Qt::LeftButton)
        mb = NLE_MB_LEFT_TYPE;
    if (event->button() == Qt::RightButton)
        mb = NLE_MB_RIGHT_TYPE;
    if (event->button() == Qt::MiddleButton)
        mb = NLE_MB_MIDDLE_TYPE;
    mpCaption->MouseUpEvent(event->pos().x(), event->pos().y(), ss, mb);
    if (mbChanaged)
    {
        emit TransformChanged();
        mbChanaged = false;       
    }
    m_KeyPointGroup.ClearKeyPointState();
    UpdataKeyPointNeedFillState(event->pos());
    emit sigRepaintWidget();
}

void FSTextEditHelper::DoMouseDoubleClickEvent(QMouseEvent *event)
{
    mpParentWidget->setFocus();
    NLEShiftState ss = NLE_SS_NONE_TYPE;
    if (event->modifiers() & Qt::ShiftModifier)
        ss |= NLE_SS_SHIFT_TYPE;
    if (event->modifiers() & Qt::AltModifier)
        ss |= NLE_SS_ALT_TYPE;
    if (event->modifiers() & Qt::ControlModifier)
        ss |= NLE_SS_CTRL_TYPE;
    if (event->buttons() & Qt::LeftButton)
        ss |= NLE_SS_LEFT_TYPE;
    NLEMouseButton mb;
    if (event->button() == Qt::LeftButton)
        mb = NLE_MB_LEFT_TYPE;
    if (event->button() == Qt::RightButton)
        mb = NLE_MB_RIGHT_TYPE;
    if (event->button() == Qt::MiddleButton)
        mb = NLE_MB_MIDDLE_TYPE;
    NLEMouseType mt;
    bool reg=mpCaption->MouseDoubleClickEvent(event->pos().x(), event->pos().y(), ss,mb,&mt);
}

void FSTextEditHelper::DoPaintEvent(QPainter &painter, QPointF pt)
{  
	return;
}

void FSTextEditHelper::DoPaintControlBoxEvent(QPainter &painter, QPointF pt)
{
    UpdataKeyPointGroup();
    return;//外部画框
}

bool FSTextEditHelper::DoKeyPressEvent(QKeyEvent *event)
{
    if (!mpParentWidget->isVisible())
        return false;
    NLEShiftState ss = NLE_SS_NONE_TYPE;
    if (event->modifiers() & Qt::ShiftModifier)
        ss |= NLE_SS_SHIFT_TYPE;
    if (event->modifiers() & Qt::AltModifier)
        ss |= NLE_SS_ALT_TYPE;
    if (event->modifiers() & Qt::ControlModifier)
        ss |= NLE_SS_CTRL_TYPE;
    bool bHandled;
    QString  q_str = event->text();
    auto strText = (BSTR)q_str.utf16();

    mpCaption->KeyPressEvent(event->key(), ss, strText, &bHandled);
    mbChanaged = bHandled;

    if (mbChanaged)
    {
        mbChanaged = false;
    }
    return bHandled;
}

bool FSTextEditHelper::DoinputMethodEvent(QInputMethodEvent *event)
{
    if (!mpParentWidget->isVisible())
        return false;
    NLEShiftState ss = NLE_SS_NONE_TYPE;
    bool bHandled;
    BSTR strText;
    QString  q_str = event->commitString();
    if (!q_str.isEmpty())
    {
         auto strText = (BSTR)q_str.utf16();
        mpCaption->KeyPressEvent(0, ss, strText, &bHandled);
    }
    mbChanaged = bHandled;
    return bHandled;
}

void FSTextEditHelper::DoResizeEvent(QResizeEvent *e)
{
    
    float ratio = mpPlayerControl->GetRatio();
    QSizeF size = mpParentWidget->size() - QSize(2 * mBorderWidth, 2 * mBorderWidth);
    int w, h;
    if (size.width() / size.height() > ratio)
    {
        h = size.height();
        w = h*ratio;
    }
    else
    {
        w = size.width();
        h = w / ratio;       
    }
    mpCaption->SetDesSize(w, h);
    UpdataKeyPointGroup();
}


void FSTextEditHelper::SetPosWithDelta(QPointF OffSetPt)
{
    OffSetPt.setX(OffSetPt.x() / mPositionScaleX);
    OffSetPt.setY(OffSetPt.y() / mPositionScaleY);
    double x, y;
    GetPosition(x, y);
    SetPosition(x + OffSetPt.x(), y + OffSetPt.y());
    UpdataKeyPointGroup();
}

void FSTextEditHelper::DoMouseRealese(QPointF curPt)
{
    ClearKeyPointState();
    UpdataKeyPointNeedFillState(curPt);
}

void FSTextEditHelper::GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt)
{
    float Posx, Posy;
    GetCaptionBaseObj().pObj->GetPos(&Posx, &Posy);
    QPointF offSet = curPt - lastPt;    
    if (!IsLineShape())
    {
        float Scalex = 1, Scaley = 1;
        index = qBound(0, index, 7);
        QPointF destPt = m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint();
        QPointF destPt1 = m_KeyPointGroup.GetKeyPoint((index + 4) % 8)->GetCenterPoint();
        QPointF leftPt = m_KeyPointGroup.GetKeyPoint((index + 7) % 8)->GetCenterPoint();
        QPointF rightPt = m_KeyPointGroup.GetKeyPoint((index + 1) % 8)->GetCenterPoint();

        QPointF ptIndex2 = m_KeyPointGroup.GetKeyPoint((index + 2) % 8)->GetCenterPoint();
        QPointF ptIndex5 = m_KeyPointGroup.GetKeyPoint((index + 5) % 8)->GetCenterPoint();
        QPointF ptIndex6 = m_KeyPointGroup.GetKeyPoint((index + 6) % 8)->GetCenterPoint();

        if (m_bPressedShift)
        {
            if (IsThreePointOnLine(destPt, leftPt, rightPt))
            {
                qreal distance = GetScaleDistance(destPt + offSet, destPt, destPt1);
                qreal distance1 = Distance(destPt, destPt1);
                if (index == 1 || index == 5)
                {
                    Scaley = distance / distance1;
                }
                else
                {
                    Scalex = distance / distance1;
                }
            }
            else
            {
                if (index == 0 || index == 4)
                {
                    qreal distanceX1 = 2 * Distance(destPt, rightPt);
                    qreal distanceY1 = 2 * Distance(destPt, leftPt);

                    qreal distanceX = GetScaleDistance(destPt + offSet, destPt, rightPt) + distanceX1 / 2;
                    qreal distanceY = GetScaleDistance(destPt + offSet, destPt, leftPt) + distanceY1 / 2;
                    Scalex = distanceX / distanceX1;
                    Scaley = distanceY / distanceY1;

                }
                else
                {
                    qreal distanceY1 = 2 * Distance(destPt, rightPt);
                    qreal distanceX1 = 2 * Distance(destPt, leftPt);

                    qreal distanceX = GetScaleDistance(destPt + offSet, destPt, leftPt) + distanceX1 / 2;
                    qreal distanceY = GetScaleDistance(destPt + offSet, destPt, rightPt) + distanceY1 / 2;
                    Scalex = distanceX / distanceX1;
                    Scaley = distanceY / distanceY1;
                }
            }
        }
        else
        {
            qreal distance = GetScaleDistance(destPt + offSet, destPt, destPt1);
            qreal distance1 = Distance(destPt, destPt1);           
            Scalex = distance / distance1;
            Scaley = Scalex;       
        }       
        scalePt.setX(Scalex);
        scalePt.setY(Scaley);
    }
}

void FSTextEditHelper::GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle)
{
    float x, y;
    GetCaptionBaseObj().pObj->GetCenterPos(&x, &y);
    angle = GetRolationAngle(QPointF(x, y), lastPt, curPt);
}

void FSTextEditHelper::SetScaleWithDelta(QPointF deltaScale, int index)
{
    double x, y;
    double tempy, tempx;
    GetScale(x, y);
    if (m_bPressedShift)
    {
        x = x*deltaScale.x() < 0.01 ? 0.01 : x*deltaScale.x();
        y = y*deltaScale.y() < 0.01 ? 0.01 : y*deltaScale.y();

    }
    else
    {
        if (x > y)
        {
            tempy = y*deltaScale.y() < 0.01 ? 0.01 : y*deltaScale.y();
            x = tempy*x / y;
            y = tempy;
        }
        else
        {
            tempx = x*deltaScale.x() < 0.01 ? 0.01 : x*deltaScale.x();
            y = tempx*y / x;
            x = tempx;
        }
    }
    
    if (!IsLineShape())
    {
        SetScale(x, y);
        m_KeyPointGroup.GetKeyPoint(qBound(0, index, 7))->SetPressedOn(true);
    }       
    UpdataKeyPointGroup();
}

void FSTextEditHelper::SetAngleWithDelta(double deltaAngle, bool bSnap)
{
    double Angle=GetAngle();
    Angle += deltaAngle;
    if (bSnap)
        RolationSnap(Angle);
    SetAngle(Angle);
    UpdataKeyPointGroup();
}

void FSTextEditHelper::SetCtlrPos(QPointF curPt, int index)
{
    INLECaptionShapeObj *pObj = GetCaptionShapeObj();
    if (pObj)
    {
        QPointF pt0;        
        if (GetLineCtrlPt(index, pt0))
        {
            pt0 = curPt;
            NLECGPoint p;
            p.fPosX = pt0.x();
            p.fPosY = pt0.y();
            pObj->SetCtrlPoint(p, index);
        }  
    }
    UpdataKeyPointGroup();
}

bool FSTextEditHelper::IsSelected()
{
    bool bSelected;
    GetCaptionBaseObj().pObj->GetFocused(&bSelected);
    return bSelected;
}

void FSTextEditHelper::SetSelected(bool bSelected)
{
    GetCaptionBaseObj().pObj->SetFocused(bSelected);
    if (!bSelected)
        mCursorShape = Qt::ArrowCursor;
}

bool FSTextEditHelper::IsEditState()
{
    bool editType = false;
    bool docEdit = false;
    mpCaption->GetDocState(&docEdit, &editType);
    return editType;
}

void FSTextEditHelper::SetEditState(bool bSelected)
{

}

bool FSTextEditHelper::Contains(QPointF pos, QPointF offSet, bool &bEdit)
{
    QPointF pt = QPointF(pos - offSet).toPoint();
    NLEMouseType type = NLE_MT_DEFAULT;
    GetCaptionBaseObj().pObj->GetMouseInAreaType(pt.x(), pt.y(), &type);
    bEdit = (type == NLE_MT_TEXT_EDIT);   
    return (type == NLE_MT_SIZE_ALL || type == NLE_MT_FRAME_SIZE_ALL || type == NLE_MT_TEXT_EDIT);
}

bool FSTextEditHelper::IsChanged()
{
    return mbChanaged;
}


double FSTextEditHelper::GetAngle()
{
    float angle=0;
    if (!IsLineShape())
    {
        GetCaptionBaseObj().pObj->GetDragAngle(&angle);
    }
    else
    {
        GetCaptionBaseObj().pObj->GetAngle(&angle);
    }
    
    if (angle < 0)
        angle += 360;
    return angle;
}

void FSTextEditHelper::SetAngle(double angle)
{
    float tempAngle = angle;
    if (tempAngle>180)
        tempAngle = tempAngle - 360;
    GetCaptionBaseObj().pObj->SetAngle(tempAngle);
}

void FSTextEditHelper::GetScale(double &x, double &y)
{
    float xTemp, yTemp;
    GetCaptionBaseObj().pObj->GetScale(&xTemp, &yTemp);
    x = xTemp;
    y = yTemp;
}

void FSTextEditHelper::SetScale(double x, double y)
{
    GetCaptionBaseObj().pObj->SetScale((float)x, (float)y);
}

void FSTextEditHelper::GetPosition(double &x, double &y)
{
    float xTemp, yTemp;
    if (!IsLineShape())
        GetCaptionBaseObj().pObj->GetCenterPos(&xTemp, &yTemp);
    else
        GetCaptionBaseObj().pObj->GetPos(&xTemp, &yTemp);
    x = xTemp/mPositionScaleX;
    y = yTemp/mPositionScaleY;
}

void FSTextEditHelper::SetPosition(double x, double y)
{
    x = x*mPositionScaleX;
    y = y*mPositionScaleY;
    if (IsLineShape())
    {
        INLECaptionShapeObj *pObj = GetCaptionShapeObj();
        if (pObj)
        {
            NLECGPoint pt;
            pt.fPosX = x;
            pt.fPosY = y;
            QPointF pt0, pt1;
            if (GetLineCtrlPt(0, pt0) && GetLineCtrlPt(1, pt1))
            {
                double tempx, tempy;
                tempx = pt0.x() - x;
                tempy = pt0.y() - y;
                NLECGPoint p;
                p.fPosX = pt1.x() - tempx;
                p.fPosY = pt1.y() - tempy;
                pObj->SetCtrlPoint(p, 1);
            }

            pObj->SetCtrlPoint(pt, 0);
        }
    }
    else{
        GetCaptionBaseObj().pObj->SetCenterPos((float)x, (float)y);
    }
}


void FSTextEditHelper::GetPath(QList<FSPainterPath> &listPath, QPointF offset)
{
    float angle;
    GetCaptionBaseObj().pObj->GetAngle(&angle);
    if (!IsLineShape())
    {
        FSPainterPath linePath = m_KeyPointGroup.GetLinePath(m_ColorLinePath, m_PenStyle, m_PathLineWidth);
        for (int i = 0; i < 8 && i < m_KeyPointGroup.GetKeyPointCount(); i++)
        {
            QPainterPath path = m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath().GetPath();
            linePath.SetPath(linePath.GetPath().subtracted(path));
            FSPainterPath tempPath = m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath();
            tempPath.SetPath(path);
            tempPath.GetPath().translate(offset);
            tempPath.SetRichColor(true);
            listPath.push_back(tempPath);
        }
        linePath.GetPath().translate(offset);
        listPath.push_front(linePath);
        FSPainterPath tempPath = m_KeyPointGroup.GetKeyPoint(8)->GetCiyclePath();
        tempPath.GetPath().translate(offset);
        tempPath.SetAwaysFill(true);
        tempPath.SetRichColor(true);
        listPath.push_back(tempPath);
    }
    
    int index = IsLineShape() ? 0 : qBound(0,9, m_KeyPointGroup.GetKeyPointCount()-1);
    for (int i = index; i < m_KeyPointGroup.GetKeyPointCount();i++)
    {
        FSPainterPath tempPath = m_KeyPointGroup.GetKeyPoint(i)->GetCiyclePath();
        tempPath.GetPath().translate(offset);
        tempPath.SetRichColor(true);
        listPath.push_back(tempPath);
    }
}

bool  FSTextEditHelper::ControlBoxContains(QPointF pos, QPointF offSet , int &index)
{
    for (int i = m_KeyPointGroup.GetKeyPointCount()-1; i >=0 ; i--)
    {
        bool bContain = m_KeyPointGroup.GetKeyPoint(i)->GetRectPath().GetPath().translated(offSet).contains(pos);
        if (bContain)
        {            
            index = m_KeyPointGroup.GetKeyPoint(i)->GetIndex();
            return true;
        }           
    }
    return false;
}

IFSEditHelper::SnapType FSTextEditHelper::GetSnapType()
{
    if (IsLineShape())
    {
        for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
        {
            if (m_KeyPointGroup.GetKeyPoint(i)->IsPressedOn())
            {
                return CtrlLine;
            }
        }
        if (mCursorShape == Qt::SizeAllCursor)
            return IFSEditHelper::Trans;
    }
    else
    {
        for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
        {
            if (m_KeyPointGroup.GetKeyPoint(i)->IsPressedOn())
            {
                return m_KeyPointGroup.GetKeyPoint(i)->GetIndex()<8 ? ZoomInOut : m_KeyPointGroup.GetKeyPoint(i)->GetIndex()==8?Roriate:CtrlShape;
            }
        }
        if (mCursorShape == Qt::SizeAllCursor)
            return IFSEditHelper::Trans;
    }
    return UnKnown;
}

bool FSTextEditHelper::Intersects(QRect rc)
{
    NLECaptionObjPointsInfo pointInfo;
    auto size = mpPlayerControl->GetTimeline()->GetRenderSize();
    GetCaptionBaseObj().pObj->GetObjPointsInfo(&pointInfo, size.width(), size.height());
    GetCaptionBaseObj().pObj->GetObjPointsInfo(&pointInfo, size.width(), size.height());
    QPainterPath path,path1;
    if (!IsLineShape())
    {
        path.moveTo(QPointF(pointInfo.SizePt[0].X, pointInfo.SizePt[0].Y));
        path.lineTo(QPointF(pointInfo.SizePt[2].X, pointInfo.SizePt[2].Y));
        path.lineTo(QPointF(pointInfo.SizePt[4].X, pointInfo.SizePt[4].Y));
        path.lineTo(QPointF(pointInfo.SizePt[6].X, pointInfo.SizePt[6].Y));
        path.lineTo(QPointF(pointInfo.SizePt[0].X, pointInfo.SizePt[0].Y));
    }
    else
    {
        path.moveTo(pointInfo.CtrlPt[0].X, pointInfo.CtrlPt[0].Y);
        path.lineTo(pointInfo.CtrlPt[1].X, pointInfo.CtrlPt[1].Y);
    }
    
    path1.addRect(rc);
    return path.intersects(path1);
}

QList<QPointF > FSTextEditHelper::GetGuidesPtList()
{
    if (IsLineShape())
    {
        return m_KeyPointGroup.GetGuidesPtList(0, m_KeyPointGroup.GetKeyPointCount()-1);
    }
    else
    {
        QList<QPointF> list = m_KeyPointGroup.GetGuidesPtList(0, 8);
        list.append(m_KeyPointGroup.GetGuidesPtList(9, m_KeyPointGroup.GetKeyPointCount() - 9));
        return list;
    }
}

QList<QPointF> FSTextEditHelper::GetTargetPtList()
{
    if (IsSelected())
    {
        int index = m_KeyPointGroup.GetPressedOnPtIndex();
        if (index != -1)
        {
            index = IsLineShape() ? index - 10 : index;
            return QList<QPointF>()<< m_KeyPointGroup.GetKeyPoint(index)->GetCenterPoint();
        }
        else
        {
            return GetGuidesPtList();
        }
    }
    return QList<QPointF>();
}

bool FSTextEditHelper::GetGuidesPt(int index, QPointF &pt)
{    
    int index1 = qBound(0, index, m_KeyPointGroup.GetKeyPointCount()-1);
    pt = m_KeyPointGroup.GetKeyPoint(index1)->GetCenterPoint();
    if (m_KeyPointGroup.GetKeyPoint(index1)->IsPressedOn()||index1!=index)
    {
        return false;
    }        
    else
    {       
        return true;
    }
}

int FSTextEditHelper::GetKeyPointCount()
{
    return m_KeyPointGroup.GetKeyPointCount();
}

void FSTextEditHelper::SetCaption(NLEComPtr<INLECaption> pCaption)
{
    Q_ASSERT(pCaption);
    mpCaption = pCaption;
    SetTextChangeCallBack();
    InitDefaultPropertyValue();
    mpCaption->SetObjBeforeChangeCallBack(m_pCaptionObjBeforeChangeCallBack, this);   
    mpCaption->CanUnDoReDo(true);
    mpCaption->SetDrawFrameType(false);
    mpCaption->SetDocState(true, true);
    
    //设置文字对象不主动响应Delete键删除文字对象，解决应用层和底层delete删除冲突问题。
    mpCaption->CanDeleteObj(false);
    //拖入文字资源到非当前帧的位置，要先获取一帧和播放器大小一致的图片以确保caption初始化文字对象数据。
    QSizeF size = mpPlayerControl->GetTimeline()->GetRenderSize();
	//NLEComPtr<INLEFrame> frame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFrame(size.width(), size.height());
	mpCaption->SetDesSize(size.width(), size.height());
    SetSelected(true);
    EnableBorder(true);
    EnableTextBorder(true);
    EnableTextShadow(true);
    InitKeyPointGroup();
}


void FSTextEditHelper::SetDocState(bool bEdit, bool bchanged)
{
    mpCaption->SetDocState(bEdit, bchanged);
}

void FSTextEditHelper::GetDocState(bool &bEdit, bool &bTextEdit)
{
    bool temp1, temp2;
    mpCaption->GetDocState(&temp1, &temp2);
    bEdit = temp1;
    bTextEdit = temp2;
}

bool FSTextEditHelper::GetLineCtrlPt(int index, QPointF &pt)
{
    index = qBound(0, index, 7);
    //if (IsLineShape())
    {
        float xTemp, yTemp;
        
       /* if (index == 0)
        {
            GetCaptionBaseObj().pObj->GetPos(&xTemp, &yTemp);
            pt = QPointF(xTemp, yTemp);
        }          
        else
        {*/
            NLECaptionObjPointsInfo pointInfo;
            auto size = mpPlayerControl->GetTimeline()->GetRenderSize();
            GetCaptionBaseObj().pObj->GetObjPointsInfo(&pointInfo, size.width(), size.height());
            pt = QPointF(pointInfo.CtrlPt[index].X, pointInfo.CtrlPt[index].Y);

        //}
        return true;
    }
    //return true;
}

void FSTextEditHelper::SetTextChangeCallBack()
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetTextChangeCallBack(m_pTextChangedCallBack, this);
    }
}

QString FSTextEditHelper::GetText()
{
    BSTR ff=NULL;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
   if (pTextObj)
    {
        INLECaptionBaseObj * pBase = NULL;
        pTextObj->QueryInterface(IID_INLECaptionBaseObj, (void **)&pBase);
        if (pBase)
            pBase->GetText(&ff);
    }
    QString ffStr = QString::fromUtf16((ushort *)ff);
    if (ff)
        SysFreeString(ff);
    mpTimeLine->SetDisplayName(ffStr.mid(2, ffStr.length() - 2));
    return ffStr.mid(2, ffStr.length() - 2);   
}

void FSTextEditHelper::ChangeCurrsor(NLEMouseType type)
{
    if (!mpParentWidget->isVisible())
        return;
    switch (type)
    {
    case NLEMouseType::NLE_MT_TL:
    case NLEMouseType::NLE_MT_RB:
    {
        mCursorShape=Qt::CursorShape::SizeFDiagCursor;
        break;
    }


    case NLEMouseType::NLE_MT_TR:
    case NLEMouseType::NLE_MT_LB:
    {
        mCursorShape = Qt::CursorShape::SizeBDiagCursor;
        break;
    }

    case NLEMouseType::NLE_MT_TC:
    case NLEMouseType::NLE_MT_BC:
    {
        mCursorShape = Qt::CursorShape::SizeVerCursor;
        break;
    }
    case NLEMouseType::NLE_MT_LC:
    case NLEMouseType::NLE_MT_RC:
    {
        mCursorShape = Qt::CursorShape::SizeHorCursor;
        break;
    }
    case NLEMouseType::NLE_MT_FRAME_SIZE_ALL:
    case NLEMouseType::NLE_MT_SIZE_ALL:
    {
        mCursorShape = Qt::CursorShape::SizeAllCursor;
        break;
    }

    case NLEMouseType::NLE_MT_TEXT_EDIT:
    {
        mCursorShape = Qt::CursorShape::IBeamCursor;
        break;
    }
    case NLEMouseType::NLE_MT_DEFAULT:
    {
        mCursorShape = Qt::CursorShape::ArrowCursor;
        break;
    }
    case NLEMouseType::NLE_MT_ROT:
    {
        mCursorShape = Qt::CursorShape::CrossCursor;
        break;
    }
    case  NLEMouseType::NLE_MT_CTRL_POINT:
    {
        mCursorShape = Qt::PointingHandCursor;
    }
        break;
    default:
        mCursorShape = Qt::CursorShape::ArrowCursor;
    }
}


NLECaptionObjData FSTextEditHelper::GetCaptionBaseObj()
{
    NLECaptionObjData captionData;   
    mpCaption->GetObjParaByIndex(0, &captionData);
    return captionData;
}


INLECaptionShapeObj * FSTextEditHelper::GetCaptionShapeObj()
{
    NLECaptionObjData objData = GetCaptionBaseObj();
    if (objData.ObjType == NLECaptionObjType::NLE_DOT_SHAPE)
    {
        void * pHapeObj = NULL;

        if (objData.pObj)
            objData.pObj->QueryInterface(IID_INLECaptionShapeObj, &pHapeObj);
        return static_cast<INLECaptionShapeObj *>(pHapeObj);
    }
    else if (objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT)
    {
        void * pTipTextObj = NULL;
        if (objData.pObj)
        {
            objData.pObj->QueryInterface(IID_INLECaptionTipTextObj, &pTipTextObj);
            if (pTipTextObj)
            {
                void * pHapeObj = NULL;
                static_cast<INLECaptionTipTextObj *>(pTipTextObj)->GetShapeInterface(&pHapeObj);
                return static_cast<INLECaptionShapeObj *>(pHapeObj);
            }
        }            
    }
    return NULL;
}

INLECaptionTipTextObj * FSTextEditHelper::GetCaptionTipTextObj()
{
    
    NLECaptionObjData objData = GetCaptionBaseObj();
    if (objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT)
    {
        void * pTipTextObj = NULL;

        if (objData.pObj)
            objData.pObj->QueryInterface(IID_INLECaptionTipTextObj, &pTipTextObj);
        return (INLECaptionTipTextObj *)(pTipTextObj);
    }
    return NULL;
}

INLECaptionTextObj * FSTextEditHelper::GetCaptionTextObj()
{
    NLECaptionObjData objData = GetCaptionBaseObj();
    if (objData.ObjType == NLECaptionObjType::NLE_DOT_TEXT)
    {
        INLECaptionTextObj * pTextObj = NULL;
        if (objData.pObj)
            objData.pObj->QueryInterface(IID_INLECaptionTextObj, (void **)&pTextObj);
        return pTextObj;
    }
    else if (objData.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT)
    {
        INLECaptionTipTextObj *pTipTextObj=NULL;
        if (objData.pObj)
            objData.pObj->QueryInterface(IID_INLECaptionTipTextObj,(void **) &pTipTextObj);
        if (pTipTextObj)
        {
            INLECaptionTextObj * pTextObj = NULL;
            pTipTextObj->GetTextInterface((void **)&pTextObj);
            return pTextObj;
        }
    }
    return NULL;
}

INLECaptionSVGTextObj * FSTextEditHelper::GetCaptionSvgTextObj()
{
    NLECaptionObjData objData = GetCaptionBaseObj();
    INLECaptionSVGTextObj * pSvgTextObj = NULL;
    if (objData.ObjType == NLECaptionObjType::NLE_DOT_SVGTEXT)
    {
        if (objData.pObj)
            objData.pObj->QueryInterface(IID_INLECaptionSVGTextObj, (void **)&pSvgTextObj);
    }
    return pSvgTextObj;
}

void FSTextEditHelper::InitKeyPointGroup()
{
    NLECaptionObjPointsInfo pointInfo;
    auto size = mpPlayerControl->GetTimeline()->GetRenderSize();
    GetCaptionBaseObj().pObj->GetObjPointsInfo(&pointInfo, size.width(), size.height());
    if (!IsLineShape())
    {
        for (int i = 0; i < 8; i++)
        {
            FSKeyPoint pt(QPointF(pointInfo.SizePt[i].X, pointInfo.SizePt[i].Y), NULL);
            pt.SetIndex(i);
            m_KeyPointGroup.AddKeyPoint(pt, m_ColorControlPath, m_ColorFillControlBox, Qt::SolidLine, m_PathLineWidth, false);
        }
        FSKeyPoint pt(QPointF(pointInfo.RotatePt.X, pointInfo.RotatePt.Y), NULL);
        pt.SetIndex(8);
        m_KeyPointGroup.AddKeyPoint(pt, m_ColorControlPath, m_ColorFillRolationBox, Qt::SolidLine, m_PathLineWidth, false);

        FSKeyPoint pt1(QPointF((pointInfo.SizePt[0].X + pointInfo.SizePt[4].X) / 2, (pointInfo.SizePt[0].Y + pointInfo.SizePt[4].Y) / 2), NULL);
        pt1.SetIndex(9);
        m_KeyPointGroup.AddKeyPoint(pt1, m_ColorControlPath, m_ColorFillControlBox, Qt::SolidLine, m_PathLineWidth, false);
        for (int i = 0; i < 8; i++)
        {
            if (pointInfo.CtrlPt[i].X == 0 && pointInfo.CtrlPt[i].Y == 0)
                break;
            FSKeyPoint pt(QPointF(pointInfo.CtrlPt[i].X, pointInfo.CtrlPt[i].Y), NULL);
            pt.SetIndex(i+10);
            m_KeyPointGroup.AddKeyPoint(pt, m_ColorControlPath, QColor(Qt::yellow), Qt::SolidLine, m_PathLineWidth, false);
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            if (pointInfo.CtrlPt[i].X == 0 && pointInfo.CtrlPt[i].Y == 0)
                break;
            FSKeyPoint pt(QPointF(pointInfo.CtrlPt[i].X, pointInfo.CtrlPt[i].Y), NULL);
            pt.SetIndex(i+10);
            m_KeyPointGroup.AddKeyPoint(pt,m_ColorControlPath, QColor(Qt::yellow), Qt::SolidLine, m_PathLineWidth, false);
        }
    }
    
}

void FSTextEditHelper::UpdataKeyPointGroup()
{
    NLECaptionObjPointsInfo pointInfo;
    auto size = mpPlayerControl->GetTimeline()->GetRenderSize();
    GetCaptionBaseObj().pObj->GetObjPointsInfo(&pointInfo, size.width(), size.height());
    if (!IsLineShape())
    {
        for (int i = 0; i < 8 && i < m_KeyPointGroup.GetKeyPointCount(); i++)
        {
            m_KeyPointGroup.GetKeyPoint(i)->SetCenterPointF(QPointF(pointInfo.SizePt[i].X, pointInfo.SizePt[i].Y));          
            m_KeyPointGroup.GetKeyPoint(i)->SetPathColor(m_ColorControlPath);
            m_KeyPointGroup.GetKeyPoint(i)->SetFillPathColor(m_ColorFillControlBox);
        }
        if (m_KeyPointGroup.GetKeyPointCount() >= 9)
        {
            m_KeyPointGroup.GetKeyPoint(8)->SetCenterPointF(QPointF(pointInfo.RotatePt.X, pointInfo.RotatePt.Y));
            m_KeyPointGroup.GetKeyPoint(8)->SetPathColor(m_ColorControlPath);
            m_KeyPointGroup.GetKeyPoint(8)->SetFillPathColor(m_ColorFillRolationBox);
        }
        if (m_KeyPointGroup.GetKeyPointCount() >= 10)
        {
            m_KeyPointGroup.GetKeyPoint(9)->SetCenterPointF(QPointF((pointInfo.SizePt[0].X + pointInfo.SizePt[4].X) / 2, (pointInfo.SizePt[0].Y + pointInfo.SizePt[4].Y) / 2));
            m_KeyPointGroup.GetKeyPoint(9)->SetPathColor(m_ColorControlPath);
            m_KeyPointGroup.GetKeyPoint(9)->SetFillPathColor(m_ColorFillControlBox);
        }
        for (int i = 10; i < m_KeyPointGroup.GetKeyPointCount(); i++)
        {
            if (pointInfo.CtrlPt[i - 10].X == 0 && pointInfo.CtrlPt[i - 10].Y == 0)
                break;
            m_KeyPointGroup.GetKeyPoint(i)->SetCenterPointF(QPointF(pointInfo.CtrlPt[i - 10].X, pointInfo.CtrlPt[i - 10].Y));
            m_KeyPointGroup.GetKeyPoint(i)->SetPathColor(m_ColorControlPath);
        }
    }
    else
    {
        for (int i =0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
        {
            if (pointInfo.CtrlPt[i].X == 0 && pointInfo.CtrlPt[i].Y == 0)
                break;
            m_KeyPointGroup.GetKeyPoint(i)->SetCenterPointF(QPointF(pointInfo.CtrlPt[i].X, pointInfo.CtrlPt[i].Y));
            m_KeyPointGroup.GetKeyPoint(i)->SetPathColor(m_ColorControlPath);
        }
    }
    
}

void FSTextEditHelper::RolationSnap(double &angle)
{
    if (qAbs(360 - angle) < 3 || qAbs(0 - angle) < 3)
        angle = 0;
    else if (qAbs(90 - angle) < 3)
        angle = 90;
    else if (qAbs(180 - angle) < 3)
        angle = 180;
    else if (qAbs(270 - angle) < 3)
        angle = 270;
}

void FSTextEditHelper::UpdataKeyPointNeedFillState(QPointF pt)
{
    float angle;
    GetCaptionBaseObj().pObj->GetAngle(&angle);
    if (IsSelected())
    {
        NLEMouseType type = NLE_MT_DEFAULT;
        int index = -1;
        GetCaptionBaseObj().pObj->GetMouseInAreaType(pt.x(), pt.y(), &type);    
        if (!ControlBoxContains(pt, QPointF(0, 0), index))
        {
            type = qBound(NLE_MT_DEFAULT, type, NLE_MT_TEXT_EDIT);
            /*if (index <= 8)
                type = qBound(NLE_MT_TL, type, NLE_MT_ROT);*/
        }
        ChangeCurrsor(type);
    }
    for (int  i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        QPainterPath path = m_KeyPointGroup.GetKeyPoint(i)->GetRectPath().GetPath();
        if (!m_KeyPointGroup.GetKeyPoint(i)->IsPressedOn())
        {
            bool bNeedFill = path.contains(pt);
            if (bNeedFill != m_KeyPointGroup.GetKeyPoint(i)->IsNeedFill())
            {
                m_KeyPointGroup.GetKeyPoint(i)->SetNeedFill(bNeedFill);
                emit sigRepaintWidget();
            }
        }
    }
}

void FSTextEditHelper::UpdataKeyPointPressedOnState(QPointF pt)
{
    float angle;
    QRectF rc;
    GetCaptionBaseObj().pObj->GetAngle(&angle);
    for (int i = 0; i < m_KeyPointGroup.GetKeyPointCount(); i++)
    {
        QPainterPath path = m_KeyPointGroup.GetKeyPoint(i)->GetRectPath().GetPath();
        m_KeyPointGroup.GetKeyPoint(i)->SetPressedOn(path.contains(pt));
    }
}

void FSTextEditHelper::ClearKeyPointState()
{
    m_KeyPointGroup.ClearKeyPointState();
}

void FSTextEditHelper::InitTextChangeCallBack()
{
    m_pTextChangedCallBack = [](void *pContext, wchar_t* str, int nCaretRow, int nCaretCol)->HRESULT{
        FSTextEditHelper *pTextHelper = static_cast<FSTextEditHelper *>(pContext);
        QString strTemp = QString(QString::fromWCharArray(str));
        pTextHelper->mpTimeLine->SetDisplayName(strTemp.mid(2, strTemp.length()-2));
        emit pTextHelper->TextChanged(pTextHelper->mpTimeLine->GetDisplayName());
        return S_OK;
    };
}

void FSTextEditHelper::InitCaptionObjBeforeChangeCallBack()
{
    m_pCaptionObjBeforeChangeCallBack = [](void* pUserObj)->HRESULT{
        FSTextEditHelper *pTextHelper = static_cast<FSTextEditHelper *>(pUserObj);
        pTextHelper->mpCaption->PrepareUndoGroupID();
        emit pTextHelper->TextUnDoReDo();
        qDebug() << "InitCaptionObjBeforeChangeCallBack";
        return S_OK;
    };
}

void FSTextEditHelper::SetBackGroundColor(QColor color)
{
    NLECaptionObjData objData = GetCaptionBaseObj();
    INLECaptionShapeObj * pShapeObj = NULL;
    if (objData.ObjType == NLE_DOT_TIPTEXT)
    {
        INLECaptionTipTextObj * pTemp = GetCaptionTipTextObj();
        pTemp->GetShapeInterface(&pShapeObj);      
    }
    else if (objData.ObjType == NLE_DOT_SHAPE)
    {
        pShapeObj = GetCaptionShapeObj();
    }

    if (pShapeObj)
    {
        int r, g, b;
        color.getRgb(&r, &g, &b);
        pShapeObj->SetCGFaceSingleColor(MYRGB(r, g, b));
        pShapeObj->SetCGFaceAlpha(color.alpha());
    }
}

QColor FSTextEditHelper::GetBackGroundColor()
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    QColor regColor;
    NLECGFace face;
    if (pShapeObj)
    {
        pShapeObj->GetCGFace(&face);
        if (face.iFillType == 0)
        {
            regColor=QColor(face.sColor.iR, face.sColor.iG, face.sColor.iB,face.sColor.iA);
        }
    }
    return regColor;
}



void FSTextEditHelper::SetBorderBackGroundColor(QColor color)
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    if (pShapeObj)
    {
        pShapeObj->SetCGBorderSingleColor(MYRGB(color.red(), color.green(), color.blue()));
        pShapeObj->SetCGBorderAlpha(color.alpha());
    }
}

QColor FSTextEditHelper::GetBorderBackGroundColor()
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    QColor regColor;
    NLECGBorder face;
    if (pShapeObj)
    {
        pShapeObj->GetCGBorder(&face);
        if (face.iFillType == 0)
        {
            regColor = QColor(face.sColor.iR, face.sColor.iG, face.sColor.iB, face.sColor.iA);
        }
    }
    return regColor;
}

void FSTextEditHelper::SetBorderWidth(int w)
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    if (pShapeObj)
    {
        pShapeObj->SetCGBorderWidth(w);
    }
}

int FSTextEditHelper::GetBorderWidth()
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    NLECGBorder face;
    int w = 0;
    if (pShapeObj)
    {
        pShapeObj->GetCGBorder(&face);
        if (face.bEnable)
        {
            w=face.iWidth;
        }
    }
    return w;
}

void FSTextEditHelper::SetTextColor(QColor color)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        int r, g, b;
        color.getRgb(&r, &g, &b);
        pTextObj->SetFaceColor(MYRGB(r, g, b));
    }
}

QColor FSTextEditHelper::GetTextColor()
{
    QColor color;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEFace face;
        pTextObj->GetFace(&face);
        color.setRgb(GetRValue(face.Color1), GetGValue(face.Color1), GetBValue(face.Color1));
    }
    return color;
}

void FSTextEditHelper::SetTextBorderColor(QColor color)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        int r, g, b;
        color.getRgb(&r, &g, &b);
        pTextObj->SetBorderColor1(MYRGB(r, g, b));
    }
}

QColor FSTextEditHelper::GetTextBorderColor()
{
    QColor color;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEBorder border;
        pTextObj->GetBorder(&border);
        color.setRgb(GetRValue(border.Color1), GetGValue(border.Color1), GetBValue(border.Color1));
    }
    return color;
}

void FSTextEditHelper::SetTextBorderOpacity(int opacity)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetBorderAlpha(opacity);
    }
}

int FSTextEditHelper::GetTextBorderOpacity()
{
    int alpha = 255;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEBorder border;
        pTextObj->GetBorder(&border);
        alpha = border.nAlpha;
    }
    return alpha;
}

void FSTextEditHelper::SetTextBorderBlur(int blur)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetBorderBlurRadius(blur);
    }
}

int FSTextEditHelper::GetTextBorderBlur()
{
    int blur = 0;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEBorder border;
        pTextObj->GetBorder(&border);
        blur = border.nBlurRadius;
    }
    return blur;
}

void FSTextEditHelper::SetTextBorderSize(int size)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetBorderSize(size);
    }
}

int FSTextEditHelper::GetTextBorderSize()
{
    int size = 0;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEBorder border;
        pTextObj->GetBorder(&border);
        size = border.nSize;
    }
    return size;
}

void FSTextEditHelper::EnableTextBorder(bool bEnable)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetBorderEnable(bEnable ? 1 : 0);
    }
}

bool FSTextEditHelper::IsEnableTextBorder()
{
    bool bEnable = false;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEBorder border;
        pTextObj->GetBorder(&border);
        bEnable = border.IsEnable>0?true:false;
    }
    return bEnable;
}

void FSTextEditHelper::SetTextShadowColor(QColor color)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetShadowColor(MYRGB(color.red(), color.green(), color.blue()));
    }
}

QColor FSTextEditHelper::GetTextShadowColor()
{
    QColor color;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEShadow shadow;
        pTextObj->GetShadow(&shadow);
        color.setRgb(GetRValue(shadow.Color), GetGValue(shadow.Color), GetBValue(shadow.Color));
    }
    return color;
}

void FSTextEditHelper::SetTextShadowOpacity(int opacity)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetShadowAlpha(opacity);
    }
}

int FSTextEditHelper::GetTextShadowOpacity()
{
    int opacity=255;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEShadow shadow;
        pTextObj->GetShadow(&shadow);
        opacity = shadow.nAlpha;
    }
    return opacity;
}

void FSTextEditHelper::SetTextShadowBlur(int blur)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetShadowBlurRadius(blur);
    }
}

int FSTextEditHelper::GetTextShadowBlur()
{
    int blur = 0;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEShadow shadow;
        pTextObj->GetShadow(&shadow);
        blur = shadow.nBlurRadius;
    }
    return blur;
}

void FSTextEditHelper::SetTextShadowDistance(int distance)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetShadowDistance(distance);
    }
}

int FSTextEditHelper::GetTextShadowDistance()
{
    int distance = 0;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEShadow shadow;
        pTextObj->GetShadow(&shadow);
        distance = shadow.nDistance;
    }
    return distance;
}

int FSTextEditHelper::GetTextShadowDerection()
{
    int derection=-1;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEShadow Shadow;
        pTextObj->GetShadow(&Shadow);
        derection = Shadow.IsEnable?Shadow.nDirection:-1;
    }
    return derection;
}

void FSTextEditHelper::SetTextShadowDerection(int derection)
{
    INLECaptionTextObj *pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        if (derection >= 0 && derection < 13)
        {
            pTextObj->SetShadowEnable(true);
            pTextObj->SetShadowDirection(derection);
        }
    }
}

void FSTextEditHelper::SetCharSelect(int start, int len, int posStart /*= 0*/)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetCharSelect(start, len, posStart);
    }
}

void FSTextEditHelper::SetTextAlign(NLEAlignType type)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetAlignType(type);
    }
}

NLEAlignType FSTextEditHelper::GetTextAlign()
{
    NLEAlignType type;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->GetAlignType(&type);
    }
    return type;
}

void FSTextEditHelper::SetFont(QString fontStr)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetFontName((BSTR)(fontStr.toStdWString().c_str()));
    }
}

QString FSTextEditHelper::GetFont()
{
    QString fontStr;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    NLEFont font;
    if (pTextObj)
    {
        pTextObj->GetFont(&font);
        fontStr = QString::fromWCharArray(font.wszName);
    }
    return fontStr;
}

void FSTextEditHelper::SetFontSize(int size)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetFontSize(size);
    }
}

int FSTextEditHelper::GetFontSize()
{
    int size = -1;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEFont font;
        pTextObj->GetFont(&font);
        size = font.nSize;
    }
    return size;
}

void FSTextEditHelper::SetFontBold(bool bBold)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetFontBold(bBold ? 1 : 0);
    }
}

bool FSTextEditHelper::GetFontBold()
{
    bool bBold = false;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEFont font;
        pTextObj->GetFont(&font);
        bBold = (font.IsBold>0);
    }
    return bBold;
}

void FSTextEditHelper::SetFontItalic(bool bItalic)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetFontItalic(bItalic ? 1 : 0);
    }
}

bool FSTextEditHelper::GetFontIatlic()
{
    bool bItalic = false;
    NLECaptionObjData objData = GetCaptionBaseObj();
    INLECaptionTipTextObj * pTipTextObj = NULL;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();

    if (pTextObj)
    {
        NLEFont font;
        pTextObj->GetFont(&font);
        bItalic = (font.IsItalic > 0);
    }
    return bItalic;
}

void FSTextEditHelper::SetOpacity(int value)
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    QColor regColor;
    NLECGFace face;
    if (pShapeObj)
    {
        pShapeObj->SetCGFaceAlpha(value);
       
    }
}

int FSTextEditHelper::GetOpacity()
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    int regOpacity;
    NLECGFace face;
    if (pShapeObj)
    {
        pShapeObj->GetCGFace(&face);
        regOpacity = face.sColor.iA;
    }
    return regOpacity;
}

void FSTextEditHelper::SetText(QString text)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        BSTR bstr_str;
        bstr_str = SysAllocString((OLECHAR *)text.utf16());
        pTextObj->SetText(bstr_str);
    }
}


void FSTextEditHelper::EnableBorder(bool bEnable)
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    if (pShapeObj)
    {
        pShapeObj->SetCGBorderEnable(bEnable);
    }
}

void FSTextEditHelper::EnableTextShadow(bool bEnable)
{
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        pTextObj->SetShadowEnable(bEnable);
    }
}

bool FSTextEditHelper::IsEnableTextShadow()
{
    bool bEnable = false;
    INLECaptionTextObj * pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEShadow shadow;
        pTextObj->GetShadow(&shadow);
        bEnable=shadow.IsEnable;
    }
    return bEnable;
}

bool FSTextEditHelper::GetCaptionSvgCount(int &count)
{
    int tempCount=0;
    INLECaptionSVGTextObj *pObj = GetCaptionSvgTextObj();
    if (pObj)
    {
        if (pObj->GetOverlayCount(&tempCount))
        {
            count = tempCount;
            return true;
        }
    }
    return false;
}

bool FSTextEditHelper::GetCaptionSvgColor(int index, QColor &color)
{
    DWORD tempColor;
    INLECaptionSVGTextObj *pObj = GetCaptionSvgTextObj();
    if (pObj)
    {
        if (pObj->GetOverlayColorByIdx(&tempColor,index))
        {
            color = QColor(GetRValue(tempColor), GetGValue(tempColor), GetBValue(tempColor));
            return true;
        }
    }
    return false;
}

bool FSTextEditHelper::SetCaptionSvgColor(int index, QColor color)
{
    INLECaptionSVGTextObj *pObj = GetCaptionSvgTextObj();
    if (pObj)
    {
        if (pObj->SetOverlayColorByIdx(MYRGB(color.red(),color.green(),color.blue()), index))
        {
            return true;
        }
    }
    return false;
}

NLEComPtr<INLECaption> FSTextEditHelper::GetCaption()
{
    return mpCaption;
}

void FSTextEditHelper::InitDefaultPropertyValue()
{
    double posx, posy, scalex, scaley, angle;
    int Optacity;
    NLEAlignType type;
    QString str;
    posx = (mpParentWidget->width() - 2 * mBorderWidth) / (2 * mPositionScaleX);
    posy = (mpParentWidget->height() - 2 * mBorderWidth) / (2 * mPositionScaleY);
    GetScale(scalex, scaley);
    angle=GetAngle();
    setProperty(Default_Position, QString("%1:%2").arg(posx).arg(posy));
    setProperty(Default_Scale, QString("%1:%2").arg(scalex).arg(scaley));
    setProperty(Default_Angle, QString::number(angle));
    QColor color;
    NLECaptionObjData data = GetCaptionBaseObj();
    if (data.ObjType == NLECaptionObjType::NLE_DOT_SHAPE || data.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT)
    {
        setProperty(Default_TextBgColor, GetBackGroundColor().name(QColor::HexArgb));
        setProperty(Default_TextBgOptacity, GetOpacity());
        setProperty(Default_TextBorderBgColor, GetBorderBackGroundColor().name(QColor::HexArgb));
        setProperty(Default_TextBorderWidth, QString::number(1));
        SetBorderWidth(1);

    }
    if (data.ObjType == NLECaptionObjType::NLE_DOT_TIPTEXT || data.ObjType == NLECaptionObjType::NLE_DOT_TEXT)
    {
        setProperty(Default_FontName, GetFont());
        setProperty(Default_FontSize, QString::number(GetFontSize()));
        setProperty(Default_FontTextColor, GetTextColor().name(QColor::HexArgb));
        setProperty(Default_TextAlignType, QString::number(GetTextAlign()));
        setProperty(Default_FontBold, GetFontBold() ? QString::number(1) : QString::number(0));
        setProperty(Default_FontItalic, GetFontIatlic() ? QString::number(1) : QString::number(0));
        SetTextShadowColor(QColor(Qt::black));
        setProperty(Defualt_TextShadowColor, "#000000");
        setProperty(Defualt_TextShadowDerection, QString::number(4));
        setProperty(Default_TextShadowOpacity, QString::number(255));
        setProperty(Default_TextShadowDistance, QString::number(3));
        SetTextShadowDerection(4);
        setProperty(Default_TextShadowBlur, QString::number(3));
        setProperty(Default_TextBorderWidth, QString::number(1));
        SetBorderWidth(1);
        setProperty(Defualt_TextBorderColor, "#ffffff");
        setProperty(Default_TextBorderOpacity, QString::number(255));
        setProperty(Default_TextBorderBlur, QString::number(0));
        setProperty(Default_TextBorderSize, QString::number(0));
        SetTextBorderSize(0);
    }
    if (data.ObjType == NLECaptionObjType::NLE_DOT_SVGTEXT)
    {
        int count = 0;
        if (GetCaptionSvgCount(count))
        {
            QStringList list;
            for (int i = 0; i < count; i++)
            {
                QColor color;
                if (GetCaptionSvgColor(i, color))
                {
                    list.push_back(color.name(QColor::HexRgb));
                }
            }
            if (!list.isEmpty())
            {
                setProperty(Default_CaptionSvgColors, list.join(","));
            }
        }
    }
}

bool FSTextEditHelper::IsLineShape()
{
    INLECaptionShapeObj * pShapeObj = GetCaptionShapeObj();
    if (!pShapeObj)
        return false;
    CG_Graphic_Type shapeId = (CG_Graphic_Type)pShapeObj->GetGraphicID();
    return (shapeId == CG_Graphic_Type::CG_OpenedArrowLine || shapeId == CG_Graphic_Type::CG_OpenedLine || shapeId == CG_Graphic_Type::CG_TwoWayArrawLine);
}


void FSTextEditHelper::SetLinePathColor(QColor color)
{
    m_ColorLinePath = color;
}

void FSTextEditHelper::SetControlPathColor(QColor color)
{
    m_ColorControlPath = color;
}

void FSTextEditHelper::SetControlBoxFillColor(QColor color)
{
    m_ColorFillControlBox = color;
}

void FSTextEditHelper::SetRolationFillColor(QColor color)
{
    m_ColorFillRolationBox = color;
}

void FSTextEditHelper::SetLineWidth(float w)
{
    m_PathLineWidth = w;
}



void FSTextEditHelper::SetLinePathStyle(Qt::PenStyle style)
{
    m_PenStyle = style;
}

void FSTextEditHelper::SetShiftFlag(bool bPressedShift)
{
    m_bPressedShift = bPressedShift;
}

///////////////////////////////////////////////////////////////////
/***************图片视频编辑FSVideoPicEditHelper********************/
///////////////////////////////////////////////////////////////////



FSVideoPicEditHelper::FSVideoPicEditHelper(FSNLETimeline *TimeLine, IMediaPlayerControl *pControl, QObject *parent)
    : IFSEditHelper(TimeLine, pControl, parent)
    , m_pPlayerControl(pControl)
    , m_bMoved(false)
{
    InitScaleControlBoxInfo();
    InitMaskControlBoxInfo();
    InitDefaultPropertyValue();
    ConnectSignals();
}

FSVideoPicEditHelper::~FSVideoPicEditHelper()
{

}


bool FSVideoPicEditHelper::DoMousePressEvent(QMouseEvent *e)
{
    if (!mpParentWidget->isVisible())
    {
        return false;
    }

    if (IsScaleMode())
    {
        if (m_pScaleBoxControl->DoMousePressEvent(e))
        {
            if ((e->modifiers() & Qt::ControlModifier) && IsSelected())
            {
                m_pScaleBoxControl->ClearKeyPointState();
            }
            double posx, posy, scalex, scaley, angle;
            QRectF rc;
            m_pScaleBoxControl->GetTransformInfo(angle, scalex, scaley, posx, posy, rc);
            m_StartChangeTransInfoStr = QString("%1:%2:%3:%4:%5").arg(scalex).arg(scaley).arg(posx).arg(posy).arg(angle);
            return true;
        }
    }
    else
    {
        if (m_pMaskBoxControl->DoMousePressEvent(e))
        {
            if ((e->modifiers() & Qt::ControlModifier) && IsSelected())
            {
                m_pMaskBoxControl->ClearKeyPointState();
            }
            double posx, posy, scalex, scaley, angle;
            QRectF rc;
            m_pMaskBoxControl->GetTransformInfo(angle, scalex, scaley, posx, posy, rc);
            m_StartChangeTransInfoStr = QString("%1:%2:%3:%4:%5").arg(scalex).arg(scaley).arg(posx).arg(posy).arg(angle);
            m_StartChangeMaskPathStr = m_pMaskBoxControl->MaskPathPoint();
            return true;
        }
    }
    return false;
}

void FSVideoPicEditHelper::DoMouseMoveEvent(QMouseEvent *e)
{
    if (!mpParentWidget->isVisible())
        return ;
    if (IsSelected())
    {
        if (IsScaleMode())
        {
            if (m_pScaleBoxControl->DoMouseMoveEvent(e))
            {
                m_bMoved = true;
            }
        }
        else
        {
            if (m_pMaskBoxControl->DoMouseMoveEvent(e))
            {
                m_bMoved = true;
            }
        }
    }
}

void FSVideoPicEditHelper::DoMouseUpEvent(QMouseEvent *e)
{
    if (!mpParentWidget->isVisible())
        return;
    int index = IFSDragBoxControl::CENTER + 1;
    if (IsScaleMode())
    {
        if (m_pScaleBoxControl->IsPressedOn(index))
        {
            if (m_bMoved)
            {
                QStringList list = m_StartChangeTransInfoStr.split(":", QString::SkipEmptyParts);
                if (list.size() == 5)
                {
                    UndoCommand *pCmd = new FSVisaulUndoCommand(this, { list[0].toDouble(), list[1].toDouble() }, { list[2].toDouble(), list[3].toDouble() }, list[4].toDouble(), true);
                    emit EndChanged(pCmd);
                }               
                m_bMoved = false;
            }            
            
        }
    }
    else
    {
        if (m_pMaskBoxControl->IsPressedOn(index))
        {
            if (m_bMoved)
            {
                int type = index;
                UndoCommand *pCmd = NULL;
                if (type == IFSDragBoxControl::ANGEL_PT || type == IFSDragBoxControl::CENTER)
                {
                    QStringList list = m_StartChangeTransInfoStr.split(":", QString::SkipEmptyParts);
                    if (list.size() == 5)
                    {
                        pCmd = new FSVisaulUndoCommand(this, { list[0].toDouble(), list[1].toDouble() }, { list[2].toDouble(), list[3].toDouble() }, list[4].toDouble(), true);                       
                        m_StartChangeTransInfoStr.clear();
                    }
                }
                else
                {
                    if (!m_StartChangeMaskPathStr.isEmpty() && m_StartChangeMaskPathStr.split(",", QString::SkipEmptyParts).size() == 8)
                    {
                        pCmd = new FSMaskPathChangeCommand(this, m_StartChangeMaskPathStr);
                        m_StartChangeMaskPathStr.clear();
                    }
                }
                if (pCmd)
                    emit EndChanged(pCmd);
                m_bMoved = false;
            }
        }
    }
    m_pScaleBoxControl->DoMouseUpEvent(e);
    m_pMaskBoxControl->DoMouseUpEvent(e);
}

void FSVideoPicEditHelper::DoPaintEvent(QPainter &painter, QPointF Offset)
{   
    if (!mpParentWidget->isVisible())
        return ;
}

void FSVideoPicEditHelper::DoPaintControlBoxEvent(QPainter &painter, QPointF Offset)
{
    if (!IsSelected() || !mpParentWidget->isVisible())
        return;
    if (IsScaleMode())
    {
        m_pScaleBoxControl->DoPaintControlBoxEvent(painter,Offset);
    }
    else
    {
        m_pMaskBoxControl->DoPaintControlBoxEvent(painter,Offset);//crop
    }

}

void FSVideoPicEditHelper::DoResizeEvent(QResizeEvent *e)
{
    QString path = m_pMaskBoxControl->MaskPathPoint();
    m_pScaleBoxControl->SetBaseRc(GetBaseRect());
    m_pMaskBoxControl->SetBaseRc(GetBaseRect());
    m_pScaleBoxControl->DoResizeEvent(e);
    m_pMaskBoxControl->DoResize(mpTimeLine->GetMaskPath());
}

void FSVideoPicEditHelper::SetPosWithDelta(QPointF offset)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->SetPosWithDelta(offset);
    }
    else
    {
        m_pMaskBoxControl->SetPosWithDelta(offset);
    }
}

void FSVideoPicEditHelper::DoMouseRealese(QPointF curPt)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->DoMouseRealese(curPt);
    }
    else
    {
        m_pMaskBoxControl->DoMouseRealese(curPt);
    }
}

void FSVideoPicEditHelper::GetDeltaScale(QPointF curPt, QPointF lastPt, int index, QPointF &scalePt)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->GetDeltaScale(curPt,lastPt,index,scalePt);
    }
    else
    {
        m_pMaskBoxControl->GetDeltaScale(curPt, lastPt, index, scalePt);
    }
}

void FSVideoPicEditHelper::GetDeltaAngle(QPointF curPt, QPointF lastPt, double &angle)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->GetDeltaAngle(curPt, lastPt, angle);
    }
    else
    {
        m_pMaskBoxControl->GetDeltaAngle(curPt, lastPt,angle);
    }
}

void FSVideoPicEditHelper::SetScaleWithDelta(QPointF deltaScale, int index)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->SetScaleWithDelta(deltaScale, index);
    }
    else
    {
        m_pMaskBoxControl->SetScaleWithDelta(deltaScale, index);
    }
}

void FSVideoPicEditHelper::SetAngleWithDelta(double deltaAngle, bool bSnap)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->SetAngleWithDelta(deltaAngle, bSnap);
    }
    else
    {
        m_pMaskBoxControl->SetAngleWithDelta(deltaAngle,bSnap);
    }
}

void FSVideoPicEditHelper::SetAngle(double angle)
{
        m_pScaleBoxControl->SetAngle(angle);
        m_pMaskBoxControl->SetAngle(angle);//crop
}

double FSVideoPicEditHelper::GetAngle()
{
    if (IsScaleMode())
        return m_pScaleBoxControl->Angle();
    else
       return m_pMaskBoxControl->Angle();//crop
}

void FSVideoPicEditHelper::SetPosition(double x, double y)
{
    x = x * mPositionScaleX;
    y = y * mPositionScaleY;
    m_pScaleBoxControl->SetPostion(x, y);
    m_pMaskBoxControl->SetPostion(x,y);//crop
}

void FSVideoPicEditHelper::GetPosition(double &x, double &y)
{
    if (IsScaleMode())
        m_pScaleBoxControl->Position(x, y);
    else
        m_pMaskBoxControl->Position(x,y);//crop
    x = x/mPositionScaleX;
    y = y/mPositionScaleY;
}

void FSVideoPicEditHelper::SetScale(double x, double y)
{
        m_pScaleBoxControl->SetScale(x, y);
        m_pMaskBoxControl->SetScale(x,y);//crop
}


void FSVideoPicEditHelper::GetScale(double &x, double &y)
{
    if (IsScaleMode())
        m_pScaleBoxControl->Scale(x, y);
    else
        m_pMaskBoxControl->Scale(x,y);//crop
}

bool FSVideoPicEditHelper::IsChanged()
{
    int index = -1;
    if (IsScaleMode())
    {
        return m_pScaleBoxControl->IsPressedOn(index);
    }
    else
    {
        return m_pMaskBoxControl->IsPressedOn(index);
    }
}


void FSVideoPicEditHelper::Updata()
{
    double angle, scalex, scaley, transx, transy;
    QRectF baseRc;
    if (IsScaleMode())
    {     
        m_pScaleBoxControl->GetTransformInfo(angle,scalex,scaley,transx,transy,baseRc);
        
    }
    else
    {
        m_pMaskBoxControl->GetTransformInfo(angle, scalex, scaley, transx, transy, baseRc);
        //cropTodo
    }
    UpdateTimeLineParam(angle, scalex, scaley, transx, transy, baseRc);
    IFSEditHelper::Updata();
}

bool FSVideoPicEditHelper::Contains(QPointF pos, QPointF offSet, bool &bEdit)
{
    bEdit = false;
    if (IsScaleMode())
    {
        return m_pScaleBoxControl->IsPressedOnRectArea(pos, offSet);
    }
    else
    {
        return m_pMaskBoxControl->IsPressedOnRectArea(pos, offSet);
    }
}

void FSVideoPicEditHelper::EnableScaleMode(bool bEnable)
{
    IFSEditHelper::EnableScaleMode(bEnable);
    m_pMaskBoxControl->GetRenderScale();
    m_pScaleBoxControl->GetRenderScale();
    if (!bEnable)
    {   
        m_pMaskBoxControl->StartMaskMode(mpTimeLine->GetMaskPath());
    }
    else
    {   
        DoResizeEvent(NULL);
    }
}


void FSVideoPicEditHelper::GetPath(QList<FSPainterPath> &listPath, QPointF offSet)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->GetPath(listPath, offSet);
    }
    else
    {
        m_pMaskBoxControl->GetPath(listPath, offSet);
    }
    
}

bool FSVideoPicEditHelper::ControlBoxContains(QPointF pos, QPointF offSet, int &index)
{
    if (IsScaleMode())
    {
        return m_pScaleBoxControl->IsPressedOnControlBox(pos, offSet,index);
    }
    else
    {
        return m_pMaskBoxControl->IsPressedOnControlBox(pos, offSet,index);
    }
}

IFSEditHelper::SnapType FSVideoPicEditHelper::GetSnapType()
{
    int index = -1;
    if (IsScaleMode())
    {
        
        index= m_pScaleBoxControl->GetPressedOnIndex();
    }
    else
    {
        index = m_pMaskBoxControl->GetPressedOnIndex();
    }
    if (index < 0)
        return UnKnown;
    if (index == 8)
        return Roriate;
    if (index == 9)
        return Trans;
    if (index >= 0 && index < 8)
        return ZoomInOut;
    return UnKnown;
}

bool FSVideoPicEditHelper::Intersects(QRect rc)
{
    if (IsScaleMode())
    {
        return m_pScaleBoxControl->Intersects(rc);
    }
    else
    {
        return m_pMaskBoxControl->Intersects(rc);
    }
}

QList<QPointF > FSVideoPicEditHelper::GetGuidesPtList()
{
    return IsScaleMode() ? m_pScaleBoxControl->GetGuidesPtList() : m_pMaskBoxControl->GetGuidesPtList();
    
}

QList<QPointF> FSVideoPicEditHelper::GetTargetPtList()
{
    return IsScaleMode() ? m_pScaleBoxControl->GetTargetPtList(IsSelected()) : m_pMaskBoxControl->GetTargetPtList(IsSelected());
}

bool FSVideoPicEditHelper::GetGuidesPt(int index, QPointF &pt)
{
    return IsScaleMode() ? m_pScaleBoxControl->GetGuidesPt(index ,pt) : m_pMaskBoxControl->GetGuidesPt(index,pt);
}

int FSVideoPicEditHelper::GetKeyPointCount()
{
    return IsScaleMode() ? m_pScaleBoxControl->GetKeyPointCount() : m_pMaskBoxControl->GetKeyPointCount();
}

void FSVideoPicEditHelper::UpdataKeyPointNeedFillState(QPointF pt)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->UpdataKeyPointNeedFillState(pt);
    }
    else
    {
        m_pMaskBoxControl->UpdataKeyPointNeedFillState(pt);
    }
}

void FSVideoPicEditHelper::UpdataKeyPointPressedOnState(QPointF pt)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->UpdataKeyPointPressedOnState(pt);
    }
    else
    {
        m_pMaskBoxControl->UpdataKeyPointPressedOnState(pt);
    }
}

void FSVideoPicEditHelper::ClearKeyPointState()
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->ClearKeyPointState();
    }
    else
    {
        m_pMaskBoxControl->ClearKeyPointState();
    }
}

void FSVideoPicEditHelper::SetShiftFlag(bool bPressedShift)
{
    if (IsScaleMode())
    {
        m_pScaleBoxControl->SetShiftFlag(bPressedShift);
    }
    else
    {
        m_pMaskBoxControl->SetShiftFlag(bPressedShift);
    }
}

void FSVideoPicEditHelper::SetLinePathColor(QColor color)
{
    m_pScaleBoxControl->SetLinePathColor(color);
    m_pMaskBoxControl->SetLinePathColor(color);
}

void FSVideoPicEditHelper::SetControlPathColor(QColor color)
{
    m_pScaleBoxControl->SetControlPathColor(color);
    m_pMaskBoxControl->SetControlPathColor(color);
}

void FSVideoPicEditHelper::SetControlBoxFillColor(QColor color)
{
    m_pScaleBoxControl->SetControlBoxFillColor(color);
    m_pMaskBoxControl->SetControlBoxFillColor(color);
}

void FSVideoPicEditHelper::SetRolationFillColor(QColor color)
{
    m_pScaleBoxControl->SetRolationFillColor(color);
    m_pMaskBoxControl->SetRolationFillColor(color);
}

void FSVideoPicEditHelper::SetLineWidth(float w)
{
    m_pScaleBoxControl->SetLineWidth(w);
    m_pMaskBoxControl->SetLineWidth(w);
}

void FSVideoPicEditHelper::SetLinePathStyle(Qt::PenStyle style)
{
    m_pScaleBoxControl->SetLinePathStyle(style);
    m_pMaskBoxControl->SetLinePathStyle(style);
}

bool FSVideoPicEditHelper::NeedToDrawPath()
{
    if (m_bDragingFlag&&IsDestopVideo())
        return true;
    return IsSelected();
}

bool FSVideoPicEditHelper::IsDestopVideo()
{
    if (GetEditType() == IFSEditHelper::Edit_Picture)
    {
        return false;
    }
    return mpTimeLine->IsDesktopVideo();
}



double FSVideoPicEditHelper::GetMouseScale()
{
    if (!IsDestopVideo())
    {
        return -1.0;
    }
    double scale = -1.0;
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        HRESULT reg = pINLEMouseEffectManager->GetMouseScale(&scale);
        reg = -1;
    }
    return scale;
}

void FSVideoPicEditHelper::SetMouseScale(double scale)
{
    if (!IsDestopVideo())
    {
        return ;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        pINLEMouseEffectManager->SetMouseScale(scale);
    }
}

double FSVideoPicEditHelper::GetMouseOpacity()
{
    if (!IsDestopVideo())
    {
        return -1.0;
    }
    double opacity = -1.0;
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        HRESULT reg = pINLEMouseEffectManager->GetOpacity(&opacity);
        reg = -1;
    }
    return opacity;
}

void FSVideoPicEditHelper::SetMouseOpacity(double Opacity)
{
    if (!IsDestopVideo())
    {
        return ;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        pINLEMouseEffectManager->SetOpacity(Opacity);
    }
}

QColor FSVideoPicEditHelper::GetHighlightColor()
{
    if (!IsDestopVideo())
    {
        return QColor();
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = 0; i < count; i++)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEVideoFilter> pTempFilter = NULL;
                pFilter->QueryInterface(IID_INLEVideoFilter, (void**)&pTempFilter);
                if (pTempFilter)
                {
                    NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                    int value = -1;
                    prop->GetInt(Key_MouseFilterType, &value);
                    if (prop)
                    {
                        if (value == Value_MouseFilterType_Highlight)
                        {
                            NLEColorF color;
                            CNLEValue val = GetVideoFilterProperty(pTempFilter, L"Color", NLE_FILTER_PROPERTY_TYPE_FCOLOR);
                            val.Get(color);
                            return QColor(color.r * 255, color.g * 255, color.b * 255);
                        }
                    }
                }                           
            }
        }
    }
    return QColor();
}

void FSVideoPicEditHelper::SetHighlightColor(QColor color)
{
    if (!IsDestopVideo())
    {
        return ;
    } 
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = 0; i < count; i++)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEVideoFilter> pTempFilter = NULL;
                pFilter->QueryInterface(IID_INLEVideoFilter, (void**)&pTempFilter);
                if (pTempFilter)
                {
                    NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                    int value = -1;
                    prop->GetInt(Key_MouseFilterType, &value);
                    if (prop)
                    {
                        if (value == Value_MouseFilterType_Highlight)
                        {
                            qreal R, G, B;
                            color.getRgbF(&R, &G, &B);
                            NLEColorF Tempcolor(R, G, B);
                            CNLEValue val;
                            val.Set(Tempcolor);
                            HRESULT reg = SetVideoFilterProperty(pTempFilter, L"Color", NLE_FILTER_PROPERTY_TYPE_FCOLOR, val);
                            reg = 0;
                        }
                    }
                }
               
            }
        }
    }
    return;
}

QColor FSVideoPicEditHelper::GetMouseRingColor()
{
    if (!IsDestopVideo())
    {
        return QColor();
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = 0; i < count; i++)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEVideoFilter> pTempFilter = NULL;
                pFilter->QueryInterface(IID_INLEVideoFilter, (void**)&pTempFilter);
                if (pTempFilter)
                {
                    NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                    int value = -1;
                    prop->GetInt(Key_MouseFilterType, &value);
                    if (prop)
                    {
                        if (value == Value_MouseFilterType_Rings)
                        {
                            NLEColorF color;
                            CNLEValue val = GetVideoFilterProperty(pTempFilter, L"Color", NLE_FILTER_PROPERTY_TYPE_FCOLOR);
                            val.Get(color);
                            return QColor(color.r * 255, color.g * 255, color.b * 255);
                        }
                    }
                }                
            }
        }
    }
    return QColor();
}

void FSVideoPicEditHelper::SetMouseRingColor(QColor color)
{
    if (!IsDestopVideo())
    {
        return;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = 0; i < count; i++)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEVideoFilter> pTempFilter = NULL;
                pFilter->QueryInterface(IID_INLEVideoFilter, (void**)&pTempFilter);
                if (pTempFilter)
                {
                    NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                    int value = -1;
                    prop->GetInt(Key_MouseFilterType, &value);
                    if (prop)
                    {
                        if (value == Value_MouseFilterType_Rings)
                        {
                            qreal R, G, B;
                            color.getRgbF(&R, &G, &B);
                            NLEColorF Tempcolor(R, G, B);
                            CNLEValue val;
                            val.Set(Tempcolor);
                            HRESULT reg = SetVideoFilterProperty(pTempFilter, L"Color", NLE_FILTER_PROPERTY_TYPE_FCOLOR, val);
                        }
                    }
                }
            }
        }
    }
    return;
}

int FSVideoPicEditHelper::GetMouseClickSound()
{
    if (!IsDestopVideo())
    {
        return -200;
    }
    int voluem = -200;
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
       HRESULT hr= pINLEMouseEffectManager->GetAudioVolume(&voluem);
       if (!SUCCEEDED(hr))
       {
           pINLEMouseEffectManager->SetAudioVolume(0);
       }
       hr = pINLEMouseEffectManager->GetAudioVolume(&voluem);
    }
    return voluem;
}

void FSVideoPicEditHelper::SetMouseClickSound(int voluem)
{
    if (!IsDestopVideo())
    {
        return ;
    }
    voluem = qBound(-100,voluem,100);
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        HRESULT reg = pINLEMouseEffectManager->SetAudioVolume(voluem);
        reg = 0;
    }
}

void FSVideoPicEditHelper::SetHotKeySize(QString Size)
{
    HotKeyParms parmas = m_MapHotkeyParms.value("Size");
    INLECaptionBaseObj *pBaseObj = GetCaptionBaseObj();
    if (!pBaseObj)
        return;
    for (auto item : parmas)
    {
        if (item.first == Size)
        {
            pBaseObj->SetScale(item.second.toFloat(), item.second.toFloat());
        }
    }
}

void FSVideoPicEditHelper::GetHotKeySize(QStringList &list, QString &CurSize)
{
    HotKeyParms parmas = m_MapHotkeyParms.value("Size");
    INLECaptionBaseObj *pBaseObj = GetCaptionBaseObj();
    if (pBaseObj)
    {
        float x, y,sub=0;
        pBaseObj->GetScale(&x, &y);
        if (!parmas.isEmpty())
        {
            sub = qAbs(x - parmas.first().second.toFloat()) + qAbs(y - parmas.first().second.toFloat());
            CurSize = parmas.first().first;
        }
            
        for (auto  item:parmas)
        {
            float temp = qAbs(x - item.second.toFloat()) + qAbs(y - item.second.toFloat());
            if (temp <= sub)
            {
                sub = temp;
                CurSize = item.first;
            }
            list.push_back(item.first);
        }
    }
}

void FSVideoPicEditHelper::SetHotKeyPosition(QString Position)
{
    HotKeyParms parmas = m_MapHotkeyParms.value("Position");
    INLECaptionBaseObj *pBaseObj = GetCaptionBaseObj();
    if (!pBaseObj)
        return;
    for (auto item : parmas)
    {
        if (item.first == Position)
        {
            QPointF pt = item.second.toPointF();
            QSizeF size=mpTimeLine->GetSourceSize();
            pBaseObj->SetCenterPos(pt.x()*size.width(), pt.y()*size.height());
            qDebug() << pt.x()*size.width() << pt.y()*size.height();
            return;
        }
    }
}

void FSVideoPicEditHelper::GetHotKeyPosition(QStringList &list, QString &CurPosition)
{
    HotKeyParms parmas = m_MapHotkeyParms.value("Position");
    INLECaptionBaseObj *pBaseObj = GetCaptionBaseObj();
    if (pBaseObj)
    {
        float x, y, sub = 0;
        pBaseObj->GetCenterPos(&x, &y);
        QSizeF size = mpTimeLine->GetSourceSize();
        if (!parmas.isEmpty())
        {
            QPointF pt = parmas.first().second.toPointF();            
            sub = qAbs(x / size.width() - pt.x()) + qAbs(y / size.height() - pt.y());
            CurPosition = parmas.first().first;
        }

        for (auto item : parmas)
        {
            QPointF pt = item.second.toPointF();
            float temp = qAbs(x / size.width() - pt.x()) + qAbs(y / size.height() - pt.y());
            if (temp <= sub)
            {
                sub = temp;
                CurPosition = item.first;
            }
            list.push_back(item.first);
        }
    }
}

void FSVideoPicEditHelper::SetHotKeyStyle(QString style)
{
    HotKeyParms parmas = m_MapHotkeyParms.value("Style");
    INLECaptionTextObj *pTextObj = GetCaptionTextObj();
    if (!pTextObj)
        return;
    for (auto item : parmas)
    {
        if (item.first == style)
        {
            NLEFont font;
            pTextObj->GetFont(&font);
            item.second.toString().toWCharArray(font.wszName);
            font.wszName[item.second.toString().length()] = 0;
            pTextObj->SetFont(&font);
            return;
        }
    }
}

void FSVideoPicEditHelper::GetHotKeyStyle(QStringList &list, QString &CurStyles)
{
    HotKeyParms parmas = m_MapHotkeyParms.value("Style");
    INLECaptionTextObj *pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEFont font;
        pTextObj->GetFont(&font);
        QString fontStr = QString::fromWCharArray(font.wszName);
        if (!parmas.isEmpty())
        {
            CurStyles = parmas.first().first;
        }
        for (auto item : parmas)
        {
            if (item.second.toString() == fontStr)
            {
                CurStyles = item.first;
            }
            list.push_back(item.first);
        }
    }
}

QColor FSVideoPicEditHelper::GetHotKeyColor()
{
    INLECaptionTextObj *pTextObj = GetCaptionTextObj();
    QColor color;
    if (pTextObj)
    {
        NLEFace face;
        pTextObj->GetFace(&face);
        color.setRgb(GetRValue(face.Color1), GetGValue(face.Color1), GetBValue(face.Color1));
    }
    return color;
}

void FSVideoPicEditHelper::SetHotKeyColor(QColor color)
{
    INLECaptionTextObj *pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        NLEFace face;
        pTextObj->GetFace(&face);
        face.Color1 = MYRGB(color.red(), color.green(), color.blue());
        pTextObj->SetFace(&face);
        //pTextObj->SetFaceColor(MYRGB(color.red(), color.green(), color.blue()));
    }
}

bool FSVideoPicEditHelper::IsHotKeyEnable()
{
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        INLETimelineBase *pTimelineBase = NULL;
        pINLEKeyboardStreamManager->GetKeyboardCaptionTimeline(pTimelineBase);
        if (pTimelineBase)
        {
            NLEPropComPtr prop=NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pTimelineBase);
            INT value = 0;
            prop->GetInt(kRender_Hidden, &value);
            return value == 0;
        }
    }
    return false;
}

void FSVideoPicEditHelper::SetHotKeyEnable(bool bEnable)
{
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        INLETimelineBase *pTimelineBase = NULL;
        pINLEKeyboardStreamManager->GetKeyboardCaptionTimeline(pTimelineBase);
        if (pTimelineBase)
        {
            NLEPropComPtr prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pTimelineBase);
            prop->SetInt(kRender_Hidden, bEnable ? 0 : 1);
        }
    }
}

void FSVideoPicEditHelper::SetHotKeyOpacity(int Opacity)
{
    INLECaptionTextObj *pTextObj = GetCaptionTextObj();
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        pINLEKeyboardStreamManager->SetKeyTextOpacity(1.0*Opacity/255);
    }
}

int FSVideoPicEditHelper::GetHotKeyOpacity()
{
    double Opacity=1;
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        pINLEKeyboardStreamManager->GetKeyTextOpacity(Opacity);
    }
    return 255*Opacity;
}

void FSVideoPicEditHelper::SetHotKeyFliterType(int type)
{
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        pINLEKeyboardStreamManager->SetKeyFilterType((NLEKeyFrameType)type);
    }
}

int FSVideoPicEditHelper::GetHotKeyFliterType()
{
    NLEKeyFrameType type;
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        pINLEKeyboardStreamManager->GetKeyFilterType(type);
    }
    return type;
}

QString FSVideoPicEditHelper::GetHotKeyText()
{
    BSTR ff = NULL;
    QString ffStr;
    INLECaptionBaseObj *pBaseObj = GetCaptionBaseObj();
    if (pBaseObj)
    {
        pBaseObj->GetText(&ff);
        ffStr = QString::fromUtf16((ushort *)ff);
        if (ff)
            SysFreeString(ff);
    }
    return ffStr;
}

void FSVideoPicEditHelper::SetHotKeyText(QString str)
{
    INLECaptionTextObj *pTextObj = GetCaptionTextObj();
    if (pTextObj)
    {
        BSTR bstr_str;
        bstr_str = SysAllocString((OLECHAR *)str.utf16());
        pTextObj->SetText(bstr_str);
    }
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        pINLEKeyboardStreamManager->ShowSampleText(str.isEmpty()?false:true);
    }
}

INLECaptionTextObj * FSVideoPicEditHelper::GetCaptionTextObj()
{
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        INLETimelineBase *pTimelineBase = NULL;
        pINLEKeyboardStreamManager->GetKeyboardCaptionTimeline(pTimelineBase);
        if (pTimelineBase)
        {
            NLEComPtr<INLECaption> pCaption = NULL;
            pTimelineBase->QueryInterface(IID_INLECaption, (LPVOID*)&pCaption);
            NLECaptionObjData objData;
            pCaption->GetObjParaByIndex(0, &objData);
            QSizeF size = mpTimeLine->GetSourceSize();
            pCaption->SetDesSize(size.width(), size.height());
            if (objData.ObjType == NLECaptionObjType::NLE_DOT_TEXT)
            {
                void * pTextObj = NULL;
                if (objData.pObj)
                    objData.pObj->QueryInterface(IID_INLECaptionTextObj, &pTextObj);
                return (INLECaptionTextObj *)(pTextObj);
            }
        }
    }
    return NULL;
}

INLECaptionBaseObj * FSVideoPicEditHelper::GetCaptionBaseObj()
{
    NLEComPtr<INLEKeyboardStreamManager> pINLEKeyboardStreamManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEKeyBoardStreamManager, (void**)&pINLEKeyboardStreamManager);
    if (pINLEKeyboardStreamManager)
    {
        INLETimelineBase *pTimelineBase = NULL;
        pINLEKeyboardStreamManager->GetKeyboardCaptionTimeline(pTimelineBase);
        if (pTimelineBase)
        {
            NLEComPtr<INLECaption> pCaption = NULL;
            pTimelineBase->QueryInterface(IID_INLECaption, (LPVOID*)&pCaption);
            QSizeF size = mpTimeLine->GetSourceSize();
            pCaption->SetDesSize(size.width(), size.height());
            pCaption->CanUnDoReDo(false);
            NLECaptionObjData objData;
            pCaption->GetObjParaByIndex(0, &objData);
            return objData.pObj;
        }
    }
    return NULL;
}

void FSVideoPicEditHelper::DeleteMouseClickSound()
{
    if (!IsDestopVideo())
    {
        return ;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        LPWSTR path = new WCHAR[MAX_PATHLENGTH + 1];
        memset(path, 0, (MAX_PATHLENGTH + 1)*sizeof(WCHAR));
        pINLEMouseEffectManager->SetSoundFilePath(path);
        delete path;
    }
}

void FSVideoPicEditHelper::DeleteMouseHighlight()
{
    if (!IsDestopVideo())
    {
        return;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = count-1; i >=0; i--)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                int value = -1;
                prop->GetInt(Key_MouseFilterType, &value);
                if (prop)
                {
                    if (value == Value_MouseFilterType_Highlight)
                    {
                        pINLEMouseEffectManager->RemoveEffectFilter(pFilter);
                    }
                }
            }
        }
    }
}

void FSVideoPicEditHelper::DeleteMouseRing()
{
    if (!IsDestopVideo())
    {
        return;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = count - 1; i >= 0; i--)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                int value = -1;
                prop->GetInt(Key_MouseFilterType, &value);
                if (prop)
                {
                    if (value == Value_MouseFilterType_Rings)
                    {
                        pINLEMouseEffectManager->RemoveEffectFilter(pFilter);
                    }
                }
            }
        }
    }
}

bool FSVideoPicEditHelper::IsContainMouseSound(QString &filePath)
{
    if (!IsDestopVideo())
    {
        return false;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        LPWSTR path = new WCHAR[MAX_PATHLENGTH + 1];
        pINLEMouseEffectManager->GetSoundFilePath(path, MAX_PATHLENGTH);
        filePath = QString::fromWCharArray(path);
        delete path;
        return !(filePath.isEmpty());
    }
    return false;
}

bool FSVideoPicEditHelper::IsContainMouseRings(INLETimelineBase **pFilterTimeLine)
{
    if (!IsDestopVideo())
    {
        return false;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = 0; i < count; i++)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                int value = -1;
                prop->GetInt(Key_MouseFilterType, &value);
                if (value == Value_MouseFilterType_Rings)
                {
                    *pFilterTimeLine = pFilter;
                    return true;
                }
            }
        }
    }
    return false;
}

bool FSVideoPicEditHelper::IsContainMouseHighlight(INLETimelineBase **pFilterTimeLine)
{
    if (!IsDestopVideo())
    {
        return false;
    }
    NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
    mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
    if (pINLEMouseEffectManager)
    {
        int count = pINLEMouseEffectManager->EffectFilterCount();
        for (int i = 0; i < count; i++)
        {
            INLETimelineBase *pFilter = NULL;
            pINLEMouseEffectManager->GetEffectFilter(i, &pFilter);
            if (pFilter)
            {
                NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pFilter);
                int value = -1;
                prop->GetInt(Key_MouseFilterType, &value);
                if (value == Value_MouseFilterType_Highlight)
                {
                    *pFilterTimeLine = pFilter;
                    return true;
                }
            }
        }
    }
    return false;
}

void FSVideoPicEditHelper::AddMouseSound(QString filePath)
{
    if (IsDestopVideo())
    {
        NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
        mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
        if (pINLEMouseEffectManager)
        {           
             pINLEMouseEffectManager->SetSoundFilePath(filePath.toStdWString().c_str());
        }
    }
}

void FSVideoPicEditHelper::AddMouseRings(INLETimelineBase *pFilter)
{
    Q_ASSERT(pFilter);
    if (IsDestopVideo())
    {
        NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
        mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
        if (pINLEMouseEffectManager)
        {
            if (pFilter)
                pINLEMouseEffectManager->AddEffectFilter(pFilter);
        }
    }
}

void FSVideoPicEditHelper::AddMouseHighLight(INLETimelineBase *pFilter)
{
    Q_ASSERT(pFilter);
    if (IsDestopVideo())
    {
        NLEComPtr<INLEMouseEffectManager> pINLEMouseEffectManager = nullptr;
        mpTimeLine->GetSrc()->QueryInterface(IID_INLEMouseEffectManager, (void**)&pINLEMouseEffectManager);
        if (pINLEMouseEffectManager)
        {
            if (pFilter)
                pINLEMouseEffectManager->AddEffectFilter(pFilter);
        }
    }
}

void FSVideoPicEditHelper::InitDefaultPropertyValue()
{
    double posx, posy, scalex, scaley,angle;
    double Optacity;
    QString str;
    QRectF baseRc = GetBaseRect();
    QSizeF sizef = mpTimeLine->GetRenderResizeScale();
    QRectF realRc = GetRealRenderRectf(mpTimeLine->GetRenderRegion(), baseRc);

    posx = 1.0*(baseRc.width() - realRc.width()) / 2;
    posx = posx / mPositionScaleX;

    posy = 1.0*(baseRc.height() - realRc.height()) / 2;
    posy = posy / mPositionScaleY;

    scalex = 1.0*realRc.width() / baseRc.width();
    scaley = 1.0*realRc.height() / baseRc.height();
    angle = 0;
    setProperty(Default_Position, QString("%1:%2").arg(posx).arg(posy));
    setProperty(Default_Scale, QString("%1:%2").arg(scalex).arg(scaley));
    setProperty(Default_Angle, QString::number(angle));

    if (mpTimeLine->GetType() == TimelineType::TimelineType_Audio || mpTimeLine->GetType() == TimelineType::TimelineType_VideoAudio)
    {
        setProperty(Default_AudioVolume, QString::number(mpTimeLine->GetVolume()));
        setProperty(DefaultDenoiseLevel, QString::number(mpTimeLine->GetDenoiseLevel()));
        m_LastDenoiseLevel = mpTimeLine->GetDenoiseLevel();
    }
    if (IsDestopVideo())
    {
        setProperty(Default_CurssorScale, QString::number(GetMouseScale()));
        setProperty(Default_CurssorOptacity, QString::number(GetMouseOpacity()));
        INLETimelineBase *pBase = NULL;
        if (IsContainMouseHighlight(&pBase))
        {
            setProperty(Default_CurssorHighlight, GetHighlightColor().name());
        }
        if (IsContainMouseRings(&pBase))
        {
            setProperty(Default_CurssorRingColor, GetMouseRingColor().name());
        }
        if (IsContainMouseSound(str))
        {
            setProperty(Default_CurssorVolume, QString::number(GetMouseClickSound()));
        }
        QStringList list;
        QString cur;
        GetHotKeyPosition(list, cur);
        if (!list.isEmpty())
        {
            setProperty(HotKeyPoision, list.last());
            SetHotKeyPosition(list.last());
        }
        
        cur.clear();
        list.clear();
        GetHotKeySize(list, cur);
        if (list.size()>2)
        {
            setProperty(HotKeySize, list.at(1));
            SetHotKeySize(list.at(1));
        }
        cur.clear();
        GetHotKeyStyle(list, cur);
        SetHotKeyStyle(cur);
        setProperty(HotKeyStyle, cur);
        setProperty(HotKeyTipEnable, false);
        SetHotKeyEnable(false);
        setProperty(HotKeyColor, GetHotKeyColor().name(QColor::HexRgb));
        setProperty(HotKeyOpacity, 255);
        setProperty(HotKeyFliter, QString::number(7));
        SetHotKeyOpacity(255);
        SetHotKeyFliterType(7);
    }
    GetCaptionBaseObj();
}

void FSVideoPicEditHelper::InitHotKeyParmaMap(const QMap<QString, HotKeyParms> &parmMap)
{
    m_MapHotkeyParms = parmMap;
}

void FSVideoPicEditHelper::SetDefaultCursorVolume()
{
    QString str;
    if (IsDestopVideo())
    {
        if (IsContainMouseSound(str))
        {
            setProperty(Default_CurssorVolume, QString::number(GetMouseClickSound()));
        }
    }
}

void FSVideoPicEditHelper::SetDefaultCursorRingColor()
{
    INLETimelineBase *pBase = NULL;
    if (IsDestopVideo())
    {
        if (IsContainMouseRings(&pBase))
        {
            setProperty(Default_CurssorRingColor, GetMouseRingColor().name());
        }
    }
}

void FSVideoPicEditHelper::SetDefaultCursorHighlight()
{
    if (IsDestopVideo())
    {
        INLETimelineBase *pBase = NULL;
        if (IsContainMouseHighlight(&pBase))
        {
            setProperty(Default_CurssorHighlight, GetHighlightColor().name());
        }
    }
}




QString FSVideoPicEditHelper::GetMaskPath()
{
    QString str;
    if (!IsScaleMode())
    {
        str = m_pMaskBoxControl->MaskPathPoint();
    }
    return str;
}


void FSVideoPicEditHelper::SetMaskPath(const QString &path)
{
    mpTimeLine->SetMaskPath(path);
}

void FSVideoPicEditHelper::UpdataControlBoxInfo()
{
    double scaleX, scaleY, transx, transy, angle;
    QRectF baseRc = GetBaseRect();
    angle = mpTimeLine->GetRegionRotateAngle();
    InitTransformInfo(mpTimeLine->GetRenderRegion(), baseRc, angle, scaleX, scaleY, transx, transy);
    m_pScaleBoxControl->initTransformInfo(angle, scaleX, scaleY, transx, transy, baseRc);
    m_pScaleBoxControl->UpdataKeyPointGroup();
    m_pMaskBoxControl->initTransformInfo(angle, scaleX, scaleY, transx, transy, baseRc);
    m_pMaskBoxControl->UpdataKeyPointGroup();
}


void FSVideoPicEditHelper::UpdateTimeLineParam(double angle, qreal scareX, qreal scareY, qreal transX, qreal transY , QRectF baseRc)
{
    QTransform tempTransform;
    tempTransform.rotate(angle);
    QRectF rc = (QTransform().scale(scareX, scareY)*QTransform().translate(transX, transY)).mapRect(baseRc);
    tempTransform = GetRotatedTransform(rc, angle);
    rc = tempTransform.mapRect(rc);
    mpTimeLine->SetRenderRegion(QRectF(rc.left() / baseRc.width(), rc.top() / baseRc.height(), rc.width() / baseRc.width(), rc.height() / baseRc.height()));
    mpTimeLine->SetRegionRotateAngle(angle);
    QSizeF renderScale = IsScaleMode() ? m_pScaleBoxControl->GetRenderScale() : m_pMaskBoxControl->GetRenderScale();
    mpTimeLine->SetRenderResizeScale(renderScale);
}


void FSVideoPicEditHelper::ConnectSignals()
{
    connect(m_pScaleBoxControl, &FSScaleBoxControl::TransFormChanged,   this, &FSVideoPicEditHelper::OnScaleTransFormChangedSlot);
    connect(m_pScaleBoxControl, &FSScaleBoxControl::CursorShapeChanged, this, &FSVideoPicEditHelper::OnCursorShapeChangedSlot);
    connect(m_pScaleBoxControl, &FSScaleBoxControl::sigRepaintWidget, this, &FSVideoPicEditHelper::sigRepaintWidget);
    connect(m_pMaskBoxControl,  &FSMaskBoxControl::TransFormChanged,    this, &FSVideoPicEditHelper::OnMaskTransFormChangedSlot);
    connect(m_pMaskBoxControl,  &FSMaskBoxControl::CursorShapeChanged,  this, &FSVideoPicEditHelper::OnCursorShapeChangedSlot);
    connect(m_pMaskBoxControl,  &FSMaskBoxControl::sigRepaintWidget, this, &FSVideoPicEditHelper::sigRepaintWidget);
}

void FSVideoPicEditHelper::InitScaleControlBoxInfo()
{
    m_pScaleBoxControl = new FSScaleBoxControl(this);
    double scaleX, scaleY, transx, transy, angle;
    QRectF baseRc = GetBaseRect();
    angle = mpTimeLine->GetRegionRotateAngle();
    InitTransformInfo(mpTimeLine->GetRenderRegion(), baseRc, angle, scaleX, scaleY, transx, transy);
    m_pScaleBoxControl->initTransformInfo(angle, scaleX, scaleY, transx, transy, baseRc);
    m_pScaleBoxControl->UpdataKeyPointGroup();
}


void FSVideoPicEditHelper::InitMaskControlBoxInfo()
{
    m_pMaskBoxControl = new FSMaskBoxControl(this);
    double scaleX, scaleY, transx, transy, angle;
    QRectF baseRc = GetBaseRect();
    angle = mpTimeLine->GetRegionRotateAngle();
    InitTransformInfo(mpTimeLine->GetRenderRegion(), baseRc, angle, scaleX, scaleY, transx, transy);
    m_pMaskBoxControl->initTransformInfo(angle, scaleX, scaleY, transx, transy, baseRc);
    m_pMaskBoxControl->UpdataKeyPointGroup();
    connect(m_pMaskBoxControl, &FSMaskBoxControl::MaskPathChanged, this, &FSVideoPicEditHelper::OnMaskPathChnagedSlot);
}

void FSVideoPicEditHelper::OnScaleTransFormChangedSlot()
{
    if (IsScaleMode())
    {
        double angle, scalex, scaley, transx, transy;
        QRectF baseRc;
        m_pScaleBoxControl->GetTransformInfo(angle, scalex, scaley, transx, transy,baseRc);      
        m_pMaskBoxControl->initTransformInfo(angle, scalex, scaley, transx, transy, baseRc);
        UpdateTimeLineParam(angle, scalex, scaley, transx, transy, GetBaseRect());
        emit TransformChanged();
    }    
}

void FSVideoPicEditHelper::OnCursorShapeChangedSlot(Qt::CursorShape shape)
{
    mCursorShape = shape;
}

void FSVideoPicEditHelper::OnMaskTransFormChangedSlot()
{
    if (!IsScaleMode())
    {
        double angle, scalex, scaley, transx, transy;
        QRectF baseRc;        
        m_pMaskBoxControl->GetTransformInfo(angle, scalex, scaley, transx, transy,baseRc);     
        m_pScaleBoxControl->initTransformInfo(angle, scalex, scaley, transx, transy, baseRc);
        UpdateTimeLineParam(angle, scalex, scaley, transx, transy, GetBaseRect());
        emit sigRepaintWidget();
        emit TransformChanged();
    }
}

void FSVideoPicEditHelper::OnMaskPathChnagedSlot()
{
    FSMaskBoxControl *pControl = static_cast<FSMaskBoxControl *>(sender());
    mpTimeLine->SetMaskPath(pControl->MaskPathPoint());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSAudioEditHelper::FSAudioEditHelper(FSNLETimeline *timeLine, IMediaPlayerControl *pControl, QObject *parent)
    :IFSEditHelper(timeLine, pControl, parent)
{
    InitDefaultPropertyValue();
}

FSAudioEditHelper::~FSAudioEditHelper()
{

}

void FSAudioEditHelper::InitDefaultPropertyValue()
{
    if (mpTimeLine->GetType() == TimelineType::TimelineType_Audio)
    {
        setProperty(Default_AudioVolume, QString::number(mpTimeLine->GetVolume()));
    }
}
