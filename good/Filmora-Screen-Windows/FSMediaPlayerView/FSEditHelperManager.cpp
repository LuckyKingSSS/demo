#include "stdafx.h"
#include "FSEditHelperManager.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/GlobalResources.h"
#include "Interface/INLEFrame.h"
#include "NLEPropComPtr/NLEKeyShortcut.h"
#include "Shared/NLECommon.h"
#include "inc_CommonLib/NLEWraper.h"
#include "Interface/INLECaption.h"
#include "Interface/INLEFactory.h"

#include "Include/NLECaptionDefine.h"
#include "Interface/INLECaption.h"
#include "Interface/INLECaptionTipTextObj.h"
#include "Interface/INLECaptionBaseObj.h"
#include "Interface/INLECaptionTextObj.h"
#include "FSUnDoReDoCommand.h"

#include "inc_FSTimelineView/fstimelineview.h"

#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/StringOperation.h"
#include "inc_CommonLib/XmlOperation.h"

#define SnapWidth 4
#define HotKeyFileName "HotkeyParma.dat"


FSEditHelperManager::FSEditHelperManager(IMediaPlayerControl *pControl, QObject *parent)
: IFSEditHelperManager(parent)
, mPressedIndex(-1)
, mBorderWidth(0)
, mPositionScale(1.0,1.0)
, m_PressedType(PRESSED_UNKNOW)
, mbMultiSelcet(false)
, mbPressed(false)
, m_bEnableSnap(true)
, m_bMove(false)
, mpFSEditHelper(NULL)
, mpPlayerEditorManager(NULL)
, m_DefaultPathColor(220,220,220)
, m_DragingPathColor(Qt::yellow)
, m_SelectDragingPathColor("#EF3062")
{
    
    Q_ASSERT(parent);
    Q_ASSERT(pControl);
    mpParentWidget  = static_cast<QWidget*>(parent);
    mpPlayerControl = pControl;
    InitHotKeyParmaMap();
}

FSEditHelperManager::~FSEditHelperManager()
{

}

void FSEditHelperManager::DoMousePressEvent(QMouseEvent *e, QPointF offSet)
{
    mbPressed = true;
    OnMousePressEvent(e, offSet);   
    UpdataDeltaInfo(true);
    if (mpFSEditHelper&&mpPlayerEditorManager&&m_PressedType!=PRESSED_UNKNOW)
    {
        QList <IFSEditHelper *> selecetHelperList = GetEditHelpers(true);
        for (auto item : selecetHelperList)
        {
            if (item->GetEditType()==IFSEditHelper::Edit_Text)
            {
                static_cast<FSTextEditHelper *>(item)->GetCaptionBaseObj().pObj->SetUnDoReDoStep();
            }
        }
    }
}

void FSEditHelperManager::DoMouseMoveEvent(QMouseEvent *e, QPointF offSet)
{
    OnMouseMoveEvent(e, offSet);
}

void FSEditHelperManager::DoMouseUpEvent(QMouseEvent *e, QPointF offSet)
{
    OnMouseReleaseEvent(e, offSet);
    emit SelectionChanged(GetEditHelpers(true));
    mbPressed = false;
}

void FSEditHelperManager::DoMouseDoubleClickEvent(QMouseEvent *e, QPointF offSet)
{
    OnMouseDoubleClickEvent(e, offSet);
}

void FSEditHelperManager::DoPaintEvent(QPainter &painter, QPointF pt)
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;   
    /*for (int i = list.size() - 1; i >= 0; i--)
    {
        list.at(i)->DoPaintEvent(painter,pt);
    }*/
    for (int i = list.size() - 1; i >= 0; i--)
    {
        list.at(i)->DoPaintControlBoxEvent(painter,pt);
    }
}

void FSEditHelperManager::DoKeyPressEvent(QKeyEvent *e)
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    for (auto item : list)
    {
        item->DoKeyPressEvent(e);
    }
    emit sigRepaintWidget();
}

void FSEditHelperManager::DoinputMethodEvent(QInputMethodEvent *e)
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    for (auto item : list)
    {
        item->DoinputMethodEvent(e);
    }
    emit sigRepaintWidget();
}


void FSEditHelperManager::DoResizeEvent(QResizeEvent *e , QSizeF scale)
{
    QList<IFSEditHelper *> list = mpEditHelperMap.values();
    for (auto item : list)
    {
        item->DoResizeEvent(e);
        item->SetPositionScale(scale.width(),scale.height());
    }
    mPositionScale = scale;
}


void FSEditHelperManager::UpdateEidtHelperList(QList<FSNLETimeline*> &list, QList<FSNLETimeline*> &selectList, QList<FSNLETimeline*> &OherselectList, bool ScaleState)
{
    m_CurrentEditerList.clear();
    
    QList<FSNLETimeline*> templist = list;
    QList<FSNLETimeline*> tempOherselectList = OherselectList;
    SortFSEditHelperList(templist);
    SortFSEditHelperList(tempOherselectList);
    templist.append(tempOherselectList);
    for (auto item : templist)
    {
        connect(item, &FSNLETimeline::sigBeforeDelete, this, &FSEditHelperManager::OnTimelineDelete, Qt::UniqueConnection);
        if (!mpEditHelperMap.contains(item))
        {
            IFSEditHelper *pHelper = NULL;
            if (item->GetType() == TimelineType::TimelineType_Annotaions_Caption || item->GetType() ==TimelineType_Annotaions_Element )
            {
                pHelper = new FSTextEditHelper(item, mpPlayerControl, mpParentWidget);
                mpEditHelperMap.insert(item, pHelper);
                NLEComPtr<INLECaption> pCaption;
                item->GetSrc()->QueryInterface(IID_INLECaption, (LPVOID*)&pCaption);
                static_cast<FSTextEditHelper *>(pHelper)->SetCaption(pCaption); 
                connect(static_cast<FSTextEditHelper *>(pHelper), &FSTextEditHelper::TextChanged, this, &FSEditHelperManager::TextChanged);
                connect(static_cast<FSTextEditHelper *>(pHelper), &FSTextEditHelper::TextUnDoReDo, this, &FSEditHelperManager::TextReDoUnDoSlot);
            }
            else if (item->GetType() == TimelineType::TimelineType_Image || 
				item->GetType() == TimelineType::TimelineType_Video || 
				item->GetType() == TimelineType::TimelineType_VideoAudio || 
				item->GetType() == TimelineType::TimelineType_Annotaions_Overlay)
            {
                pHelper = new FSVideoPicEditHelper(item,mpPlayerControl,mpParentWidget);
                static_cast<FSVideoPicEditHelper *>(pHelper)->InitHotKeyParmaMap(m_MapHotkeyParms);
                mpEditHelperMap.insert(item, pHelper);
                connect(static_cast<FSVideoPicEditHelper *>(pHelper), &FSVideoPicEditHelper::EndChanged, this, &FSEditHelperManager::EndChanged);
            }
            else if (item->GetType() == TimelineType::TimelineType_Audio)
            {
                pHelper = new FSAudioEditHelper(item, mpPlayerControl, mpParentWidget);
                mpEditHelperMap.insert(item, pHelper);
            }

            if (pHelper)
            {
                pHelper->SetBorderWidth(mBorderWidth);
                pHelper->SetPositionScale(mPositionScale.width(), mPositionScale.height());
            }
        }
		auto helper = mpEditHelperMap.value(item);
        if (helper)
        {
            helper->EnableScaleMode(ScaleState);
            m_CurrentEditerList.push_back(helper);
            connect(helper, &IFSEditHelper::sigRepaintWidget, this, &FSEditHelperManager::sigRepaintWidget,Qt::UniqueConnection);
            if (helper->GetEditType() != IFSEditHelper::Edit_Audio)
                connect(helper, &IFSEditHelper::TransformChanged, this, &FSEditHelperManager::OnTransformInfoChanged,Qt::UniqueConnection);
        }
    }
    templist = selectList;
    templist.append(OherselectList);
    QList<FSNLETimeline *> &tempeditlist = mpEditHelperMap.keys();

    for (auto item : tempeditlist)
    {
        if (!templist.contains(item))
        {
            mpEditHelperMap.value(item)->SetSelected(false);
        }            
    }
    
    for (auto item : templist)
    {   
        if (mpEditHelperMap.contains(item))
			mpEditHelperMap.value(item)->SetSelected(true);
    }
    emit sigRepaintWidget();
    emit CurrentEditHelperChanged(GetCurrentEditHelper(), false);    
}

void FSEditHelperManager::AddTextClip(QString &text)
{
    
}

void FSEditHelperManager::SortFSEditHelperList(QList<FSNLETimeline *> &list)
{
    qSort(list.begin(), list.end(), [](FSNLETimeline * l, FSNLETimeline * r) {return l->GetLevel()>= r->GetLevel(); });
}


QList<IFSEditHelper *> FSEditHelperManager::GetEditHelpers(bool bSelected)
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    QList<IFSEditHelper *> listEditHelper;
    for (size_t i = 0; i < list.size(); i++)
    {
        if (list.at(i)->IsSelected()==bSelected)
            listEditHelper.push_back(list.at(i));
    }
    return listEditHelper;
}

QList<IFSEditHelper *> FSEditHelperManager::GetResetableEditHelpers()
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    QList<IFSEditHelper *> listEditHelper;
    for (size_t i = 0; i < list.size(); i++)
    {
        if (list.at(i)->IsSelected()&&list.at(i)->Resetable())
            listEditHelper.push_back(list.at(i));
    }
    return listEditHelper;
}

void FSEditHelperManager::RemoveSelecetEditHelper()
{
    for (int i = m_CurrentEditerList.size() - 1; i >= 0;i--)
    {
        if (m_CurrentEditerList.at(i)->IsSelected())
        {
            m_CurrentEditerList.removeAt(i);
        }
    }
    emit sigRepaintWidget();
}

void FSEditHelperManager::TimeLineAddedMouseEficet(FSNLETimeline *pTimeLine,INLETimelineBase *pfilter)
{
    if (mpEditHelperMap.contains(pTimeLine))
    {
        IFSEditHelper *pHelper=mpEditHelperMap.value(pTimeLine);
        int value = -1;
        if (pfilter)
        {
            NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pfilter);            
            prop->GetInt(Key_MouseFilterType, &value);
            if (prop)
            {
                if (value == Value_MouseFilterType_Highlight)
                {
                    pHelper->setProperty(Default_CurssorHighlight, static_cast<FSVideoPicEditHelper *>(pHelper)->GetHighlightColor().name());
                    pHelper->UpdateMouseEffectOrder(Default_CurssorHighlight);
                }
                else if (value == Value_MouseFilterType_Rings)
                {
                    pHelper->setProperty(Default_CurssorRingColor, static_cast<FSVideoPicEditHelper *>(pHelper)->GetMouseRingColor().name());
                    pHelper->UpdateMouseEffectOrder(Default_CurssorRingColor);
                }
            }
        }
        else
        {
            pHelper->setProperty(Default_CurssorVolume, static_cast<FSVideoPicEditHelper *>(pHelper)->GetMouseClickSound());
            pHelper->UpdateMouseEffectOrder(Default_CurssorVolume);
        }

        emit MouseEffictAdded(value);
    }
}

void FSEditHelperManager::TimeLineRemoveMouseEficet(FSNLETimeline *pTimeLine, INLETimelineBase *filter)
{
    if (mpEditHelperMap.contains(pTimeLine))
    {
        IFSEditHelper *pHelper = mpEditHelperMap.value(pTimeLine);
        emit pHelper->CmdUpdate();
    }
}

void FSEditHelperManager::UpdataEidyHelperData(FSNLETimeline *pKeyTimeLine)
{
    Q_ASSERT(pKeyTimeLine);
    TimelineType type = pKeyTimeLine->GetNLETimelineType();
    if (mpEditHelperMap.contains(pKeyTimeLine) && (type == TimelineType_Image || 
        type == TimelineType_Video || type == TimelineType_VideoAudio||type==TimelineType_Annotaions_Overlay))
    {
        static_cast<FSVideoPicEditHelper *> (mpEditHelperMap.value(pKeyTimeLine))->UpdataControlBoxInfo();
    }
}

void FSEditHelperManager::ClearEditHelper()
{
    m_CurrentEditerList.clear();
    qDeleteAll(mpEditHelperMap.values());
    mpEditHelperMap.clear();
    emit sigRepaintWidget();
    emit CurrentEditHelperChanged(GetCurrentEditHelper(),false);
}

void FSEditHelperManager::GetControlPath(QList<FSPainterPath> &pathList, QPointF offSet)
{
    for (auto item : m_CurrentEditerList)
    {
        if (item->NeedToDrawPath())
        {
            item->GetPath(pathList,offSet);
        }
    }
}

void FSEditHelperManager::GetGuidesPointList(QList<QPointF> &hPointList, QList<QPointF> &vPointList, QPointF offset)
{
    if (!mbPressed)
        return;
    
    QList<QPointF >targetPointList, guidesPointList;
    if (!isPreesedOnRolation())
    {
        for (auto item : m_CurrentEditerList)
        {
            targetPointList.append(item->GetTargetPtList());
            if (!item->IsSelected())
                guidesPointList.append(item->GetGuidesPtList());
        }
    }
    else
    {
        for (auto item : m_CurrentEditerList)
        {
            if (item->IsSelected())
            {
                QPointF pt1;              
                if(!item->GetGuidesPt(8, pt1))
                        targetPointList.push_back(pt1);
                if (item->GetGuidesPt(9, pt1))
                {
                       guidesPointList.push_back(pt1);
                }                
            }
        }
    }
    
    GetGuidesPathPointList(targetPointList, guidesPointList, hPointList, vPointList);
    for (int i = 0; i < hPointList.size(); i++)
    {
        hPointList[i].setY(hPointList.at(i).y() + offset.y());
    }
    for (int i = 0; i < vPointList.size(); i++)
    {
        vPointList[i].setX(vPointList.at(i).x() + offset.x());
    }
}

void FSEditHelperManager::GetMainGuidesPointList(QList<QPointF> &hPointList, QList<QPointF> &vPointList, QPointF offset)
{
    if (!mbPressed||isPreesedOnRolation())
        return;
    QList<QPointF >targetPointList, guidesPointList;

    for (auto item : m_CurrentEditerList)
    {
        targetPointList.append(item->GetTargetPtList());
    }
    QRectF rc;
    rc.setTopLeft(QPointF(0, 0));
    rc.setSize(QSizeF(mpParentWidget->width() - 2 * offset.x(), mpParentWidget->height() - 2 * offset.y()));
    guidesPointList.push_back(rc.topLeft());
    guidesPointList.push_back(QPointF(rc.center().x(), rc.top()));
    guidesPointList.push_back(rc.topRight());
    guidesPointList.push_back(QPointF(rc.right(), rc.center().y()));

    guidesPointList.push_back(rc.bottomRight());
    guidesPointList.push_back(QPointF(rc.center().x(), rc.bottom()));
    guidesPointList.push_back(rc.bottomLeft());
    guidesPointList.push_back(QPointF(rc.left(), rc.center().y()));

    GetGuidesPathPointList(targetPointList, guidesPointList,hPointList,vPointList);
    for (int i = 0; i < hPointList.size(); i++)
    {        
        hPointList[i].setY(hPointList.at(i).y() + offset.y());
    }
    for (int i = 0; i < vPointList.size(); i++)
    {
        vPointList[i].setX(vPointList.at(i).x() + offset.x());
    }
}

bool FSEditHelperManager::IsPressedOnSelectControlBox(QPointF pos, QPointF offSet)
{
    QList <IFSEditHelper *> list = m_CurrentEditerList;
    int index;
    for (int i = 0; i < list.length(); i++)
    {
        if (list.at(i)->IsSelected())
        {
            if (list.at(i)->ControlBoxContains(pos,offSet,index))
                return true;
        }
    }
    return false;
}


IFSEditHelper * FSEditHelperManager::GetPressedOnControlBoxEditHelper(QPointF pos, QPointF offSet, PressedType &Type)
{
    QList <IFSEditHelper *> selecetList = GetEditHelpers(true);
    IFSEditHelper * pHelper = NULL;
    if (selecetList.isEmpty())
    {
        Type = PRESSED_UNKNOW;
        return NULL;
    }
        
    int index = -1;
    for (int i = 0; i < selecetList.length(); i++)
    {
        if (selecetList.at(i)->ControlBoxContains(pos, offSet, index))
        {
            Type = qBound(PRESSED_UNKNOW, (PressedType)index, PRESSED_CTRL2);
            selecetList.at(i)->UpdataKeyPointPressedOnState(pos - offSet);
            return selecetList.at(i);
        }
    }
    Type = PRESSED_UNKNOW;
    return NULL;
}

IFSEditHelper * FSEditHelperManager::GetPressedOnEditHelper(QPointF pos, QPointF offSet, PressedType &Type)
{
    QList <IFSEditHelper *> List = m_CurrentEditerList;
    IFSEditHelper * pHelper = NULL;
    if (List.isEmpty())
    {
        Type = PRESSED_UNKNOW;
        return NULL;
    }
        
    for (int i = 0; i <List.size(); i++)
    {
        bool bEdit = false;
        if (List.at(i)->Contains(pos, offSet, bEdit))
        {
            Type = bEdit ? PRESSED_EDIT : PRESSED_CENTER;
            List.at(i)->UpdataKeyPointPressedOnState(pos - offSet);
            return List.at(i);
        }
    }
    Type = PRESSED_UNKNOW;
    return NULL;
}

bool FSEditHelperManager::IsEditState(IFSEditHelper * &pHelper)
{
    QList <IFSEditHelper *> List = m_CurrentEditerList;
    for (int i = 0; i < List.length(); i++)
    {
        if (List.at(i)->IsEditState())
        {
            pHelper = List.at(i);
            return true;
        }
    }
    pHelper = NULL;
    return false;
}

void FSEditHelperManager::InitDefaultParam(FSNLETimeline *pTimeLine)
{
    if (mpEditHelperMap.contains(pTimeLine))
    {
        mpEditHelperMap.value(pTimeLine)->InitDefaultPropertyValue();
    } 
    if (GetCurrentEditHelper())
        emit TransformInfoChanged(GetCurrentEditHelper());
}


void FSEditHelperManager::SetBorderWidth(int width)
{
    mBorderWidth = width;
}

Qt::CursorShape FSEditHelperManager::GetCursor(QList<Qt::CursorShape> CursorList)
{
    Qt::CursorShape Cursor = Qt::ArrowCursor;
    for (int i = 0; i < CursorList.size(); i++)
    {
        if (CursorList.at(i) == Qt::ArrowCursor)
        {
            continue;
        }
        if (CursorList.at(i) == Qt::SizeAllCursor || CursorList.at(i) == Qt::IBeamCursor || CursorList.at(i) == Qt::PointingHandCursor)
        {
            Cursor = CursorList.at(i);
            continue;
        }

        if (CursorList.at(i) != Qt::ArrowCursor&&CursorList.at(i) != Qt::SizeAllCursor)
        {
            Cursor = CursorList.at(i);
            break;
        }
    }
    return Cursor;
}

void FSEditHelperManager::GetGuidesPathPointList(QList<QPointF >targetPtList, QList<QPointF > guidesPtList,QList<QPointF> &hPointList, QList<QPointF> &vPointList)
{
    if (!m_bEnableSnap)
        return;
    QList<QPointF>list;
    for (auto TargetPt:targetPtList)
    {
        for (auto GuidesPt : guidesPtList)
        {
            QPointF pt(0, 0),pt1(0,0);
            if (qAbs(qRound(TargetPt.x() - GuidesPt.x())) < 1.1)
            {
                pt.setX(qRound(TargetPt.x()));
                if (!vPointList.contains(pt))
                    vPointList.push_back(pt);
            }
            if (qAbs(qRound(TargetPt.y() - GuidesPt.y()))<1.1)
            {
                pt1.setY(qRound(TargetPt.y()));
                if (!hPointList.contains(pt1))
                    hPointList.push_back(pt1);
            }
        }
    }
}

QPointF FSEditHelperManager::GetSnapOffSetFromGuidesPtList(QList<QPointF > targetPtList, QList<QPointF > guidesPtList)
{
    QPointF pt(SnapWidth, SnapWidth);
    for (int i = 0; i < guidesPtList.size(); i++)
    {
        for (int j = 0; j < targetPtList.size(); j++)
        {
            if (qAbs(guidesPtList.at(i).x() - targetPtList.at(j).x()) < qAbs(pt.x()))
            {
                pt.setX(guidesPtList.at(i).x() - targetPtList.at(j).x());
            }
            if (qAbs(guidesPtList.at(i).y() - targetPtList.at(j).y()) < qAbs(pt.y()))
            {
                pt.setY(guidesPtList.at(i).y() - targetPtList.at(j).y());
            }
        }
    }
    return pt;
}

QPointF FSEditHelperManager::GetSnapOffSet(QPointF offset)
{
    QList<QPointF > targetPtList,guidesPtList;
    for (auto item : m_CurrentEditerList)
    {
        targetPtList.append(item->GetTargetPtList());
        if (!item->IsSelected())
        {
            guidesPtList.append(item->GetGuidesPtList());
        }          
        else
        {
            if (item->GetEditType() == IFSEditHelper::EditType::Edit_Text)
            {
                if (static_cast<FSTextEditHelper *>(item)->IsLineShape())
                {
                    QPointF pt1, pt2;
                    bool reg1 = item->GetGuidesPt(0, pt1);
                    bool reg2 = item->GetGuidesPt(1, pt2);
                    if (!(reg1&&reg2))
                    {
                        if (reg1)
                        {
                            guidesPtList.push_back(pt1);
                        }
                        if (reg2)
                        {
                            guidesPtList.push_back(pt2);
                        }
                    }
                }
            }
        }
    }
    QRectF rc;
    rc.setTopLeft(QPointF(0, 0));
    rc.setSize(QSizeF(mpParentWidget->width() - 2 * offset.x(), mpParentWidget->height() - 2 * offset.y()));
    guidesPtList.push_back(rc.topLeft());
    guidesPtList.push_back(QPointF(rc.center().x(), rc.top()));
    guidesPtList.push_back(rc.topRight());
    guidesPtList.push_back(QPointF(rc.right(), rc.center().y()));

    guidesPtList.push_back(rc.bottomRight());
    guidesPtList.push_back(QPointF(rc.center().x(), rc.bottom()));
    guidesPtList.push_back(rc.bottomLeft());
    guidesPtList.push_back(QPointF(rc.left(), rc.center().y()));
    return GetSnapOffSetFromGuidesPtList(targetPtList, guidesPtList);
}

void FSEditHelperManager::Dotranslate(QMouseEvent *e, QPointF offSet)
{
    QPointF CurrentPt = e->pos() - offSet;
    QPointF lastPt = m_LastPoint - offSet;
    QList <IFSEditHelper *> selecetHelperList = GetEditHelpers(true);
    QMouseEvent e1 = TransMouseEvent(e, offSet);
    int index = -1;
    switch (m_PressedType)
    {
    case FSEditHelperManager::PRESSED_UNKNOW:
        mpFSEditHelper = NULL;
        for (auto item : selecetHelperList)
        {
            item->UpdataKeyPointNeedFillState(CurrentPt);
        }
        break;
    case FSEditHelperManager::PRESSED_TOP_LEFT:
    case FSEditHelperManager::PRESSED_TOP_MID:
    case FSEditHelperManager::PRESSED_TOP_RIGHT:
    case FSEditHelperManager::PRESSED_RIGHT_MID:
    case FSEditHelperManager::PRESSED_BOTTOM_RIGHT:
    case FSEditHelperManager::PRESSED_BOTTOM_MID:
    case FSEditHelperManager::PRESSED_BOTTOM_LEFT:
    case FSEditHelperManager::PRESSED_LEFT_MID:
    {
        index = m_PressedType;
        QPointF scalePt(1, 1);

        if (mpFSEditHelper)
        {
            mpFSEditHelper->GetDeltaScale(CurrentPt, lastPt, index, scalePt);
            UpdataDeltaInfo(false, scalePt, QPointF(0, 0), 0);
        }       
        for (auto item : selecetHelperList)
        {       
            item->SetScaleWithDelta(scalePt, index);           
        }
        emit sigRepaintWidget();
    }
        break;
    case FSEditHelperManager::PRESSED_ROTATION:
    {

        index = m_PressedType;
        double angle = 0;
        if (mpFSEditHelper)
        {
            mpFSEditHelper->GetDeltaAngle(CurrentPt, lastPt, angle);
            UpdataDeltaInfo(false, QPointF(1, 1), QPointF(0, 0), angle);
        }
        
        for (auto item : selecetHelperList)
        {            
            item->SetAngleWithDelta(angle, m_bEnableSnap);
        }
        emit sigRepaintWidget();
    }
        break;
    case FSEditHelperManager::PRESSED_CENTER:
    {
        index = m_PressedType;
        QPointF offSet = CurrentPt - lastPt;
        if (mpFSEditHelper)
            UpdataDeltaInfo(false, QPointF(1, 1), offSet, 0);
        for (auto item : selecetHelperList)
        {
            item->SetPosWithDelta(offSet);         
        }
        emit sigRepaintWidget();
    }        
        break;
    case FSEditHelperManager::PRESSED_CTRL1:
    {
        if (mpFSEditHelper)
        {
            mpFSEditHelper->SetCtlrPos(CurrentPt, 0);
        }
    }
        emit sigRepaintWidget();
        break;
    case FSEditHelperManager::PRESSED_CTRL2:
    {
        if (mpFSEditHelper)
        {
            mpFSEditHelper->SetCtlrPos(CurrentPt,1);
        }
    }
        emit sigRepaintWidget();
        break;
    case FSEditHelperManager::PRESSED_EDIT:
    {
        if (mpFSEditHelper)
        {
            mpFSEditHelper->DoMouseMoveEvent(&e1);
        }
    }
        emit sigRepaintWidget();
        break;
    default:
        break;
    }
}

bool FSEditHelperManager::isPreesedOnRolation()
{
    for (auto item : m_CurrentEditerList)
    {
        if (item->IsSelected()&&item->GetSnapType()==IFSEditHelper::Roriate)
        {
            return true;
        }
    }
    return false;
}

int FSEditHelperManager::GetSelecetItemNum()
{
    int num = 0;
    for (auto item:m_CurrentEditerList)
    {
        if (item->IsSelected())
        {
            num++;
        }
    }
    return num;
}

void FSEditHelperManager::MultiSelectByRect(QRect rc)
{
    for (auto item : m_CurrentEditerList)
    {    
         item->SetSelected(item->Intersects(rc));
    }
    emit sigRepaintWidget();
    mbMultiSelcet = true;
}

void FSEditHelperManager::SetPlayerEditorManager(IPlayerEditorManager *pIPlayerEditorManager)
{
    mpPlayerEditorManager = pIPlayerEditorManager;
}

void FSEditHelperManager::SetMouseEffectDraging(bool bDraging)
{
    UpdataDragingPathColor(bDraging);
    emit sigRepaintWidget();
}

bool FSEditHelperManager::CanAddMouseEffect()
{
    for (auto item : m_CurrentEditerList)
    {
        if (item->GetEditType() == IFSEditHelper::Edit_Text || item->GetEditType() == IFSEditHelper::Edit_Audio)
            continue;
        FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(item);
        if (pHelper->IsDestopVideo())
            return true;
    }
    return false;
}

void FSEditHelperManager::OnTransformInfoChanged()
{
    IFSEditHelper *pSender = NULL;
    pSender = static_cast<IFSEditHelper *>(sender());
    Q_ASSERT(pSender); 
    IFSEditHelper *ptemp=GetCurrentEditHelper();
    emit TransformInfoChanged( ptemp?ptemp:pSender);
}

IFSEditHelper * FSEditHelperManager::GetCurrentEditHelper()
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    IFSEditHelper *pEditor = NULL;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i)->IsSelected())
        {
            pEditor = list.at(i);
            return pEditor;
        }
    }
    return NULL;
}


void FSEditHelperManager::UpdataDragingPathColor(bool bDraging)
{
    QList<IFSEditHelper *> list = mpEditHelperMap.values();
    for (auto item : list)
    {
        item->SetDragingFlag(bDraging);
        if (item->GetEditType() == IFSEditHelper::Edit_Text || item->GetEditType() == IFSEditHelper::Edit_Audio)
            continue;  
        FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(item);
        if (pHelper->IsDestopVideo())
            item->SetLinePathColor(bDraging ? m_DragingPathColor : m_DefaultPathColor);
    }
}


void FSEditHelperManager::UpdataDragEnterPathColor(QPointF pos, QPointF offSet)
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    bool reg = false;
    for (int i = list.size() - 1; i >= 0;i--)
    {
        if (list.at(i)->GetEditType() == IFSEditHelper::Edit_Text || list.at(i)->GetEditType() == IFSEditHelper::Edit_Audio)
            continue;
        FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(list.at(i));
        if (pHelper->IsDestopVideo())
        {
            bool bEdit;
            if (reg)
            {
                list.at(i)->SetLinePathColor(m_DragingPathColor);
            }
            else
            {
                reg = list.at(i)->Contains(pos, offSet, bEdit);
                list.at(i)->SetLinePathColor(reg ? m_SelectDragingPathColor : m_DragingPathColor);
            }
        }
    }
}

IFSEditHelper * FSEditHelperManager::GetTopLevelCanAddMouseEffcetEditHelper(QPointF pos, QPointF offSet)
{
    QList<FSNLETimeline *> timeLineList;
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    QPointF pt = pos - offSet;
    for (int i = list.size() - 1; i >= 0; i--)
    {
        if (list.at(i)->GetEditType() == IFSEditHelper::Edit_Text || list.at(i)->GetEditType() == IFSEditHelper::Edit_Audio)
            continue;
        FSVideoPicEditHelper *pHelper = static_cast<FSVideoPicEditHelper *>(list.at(i));
        bool bEdit;
        if (pHelper->IsDestopVideo() && list.at(i)->Contains(pos, offSet, bEdit))
        {
            return list.at(i);
        }
    }
    return NULL;
}

void FSEditHelperManager::TextReDoUnDoSlot()
{
    IFSEditHelper *pHelper = static_cast<FSTextEditHelper *>(sender());
    emit TextReDoUnDo(pHelper);
}

QMouseEvent FSEditHelperManager::TransMouseEvent(QMouseEvent *e, QPointF offSet)
{
    return QMouseEvent(e->type(), e->localPos() - offSet, e->button(), e->buttons(), e->modifiers());
}

void FSEditHelperManager::OnTimelineDelete()
{
    FSNLETimeline *pKeyTimeLine = static_cast<FSNLETimeline *>(sender());
    if (mpEditHelperMap.contains(pKeyTimeLine))
    {
        IFSEditHelper *pHelper = mpEditHelperMap.value(pKeyTimeLine);
        m_CurrentEditerList.removeOne(pHelper);
        mpEditHelperMap.remove(pKeyTimeLine);
        pHelper->deleteLater();
    }
}

void FSEditHelperManager::OnMousePressEvent(QMouseEvent *e, QPointF offSet)
{
    QMouseEvent e1 = TransMouseEvent(e, offSet);
    m_LastPoint = e->pos();
    mbMultiSelcet = (e->modifiers() & Qt::ControlModifier);
    if (mbMultiSelcet)
    {
        mpFSEditHelper = GetPressedOnControlBoxEditHelper(m_LastPoint, offSet, m_PressedType);
        if (mpFSEditHelper)
        {
            mpFSEditHelper->SetSelected(false);
        }
        else
        {
            mpFSEditHelper = GetPressedOnEditHelper(m_LastPoint, offSet, m_PressedType);
            if (mpFSEditHelper)
            {
                mpFSEditHelper->SetSelected(!mpFSEditHelper->IsSelected());
            }
        }
    }
    else
    {
        if (IsEditState(mpFSEditHelper))
        {
            if (mpFSEditHelper)
            {
                if (mpFSEditHelper->DoMousePressEvent(&e1))
                {
                    IFSEditHelper *pEditHelper = NULL;
                    if (IsEditState(pEditHelper))
                    {
                        mbPressed = false;
                        m_PressedType = PRESSED_EDIT;
                    }
                    else
                    {
                        if (mpFSEditHelper->IsSelected())
                        {
                            int index=-1;
                            mpFSEditHelper->ControlBoxContains(m_LastPoint, offSet, index);
                            m_PressedType = qBound(PRESSED_UNKNOW, (PressedType)index, PRESSED_CTRL2);
                            if (m_PressedType == PRESSED_UNKNOW)
                            {
                                bool bEdit = false;
                                if (mpFSEditHelper->Contains(m_LastPoint, offSet, bEdit))
                                {
                                    m_PressedType = PRESSED_CENTER;
                                }
                            }
                        }
                    }
                }
                else
                {
                    QList <IFSEditHelper *> selecetHelperList = GetEditHelpers(true);
                    mpFSEditHelper = GetPressedOnEditHelper(m_LastPoint, offSet, m_PressedType);
                    if (mpFSEditHelper)
                    {
                        if (!mpFSEditHelper->IsSelected())
                        {
                            mpFSEditHelper->SetSelected(true);
                            for (auto item : selecetHelperList)
                            {
                                item->SetSelected(false);
                            }
                        }
                        mpFSEditHelper->SetShiftFlag(e->modifiers() & Qt::ShiftModifier);
                    }
                    else
                    {
                        for (auto item : selecetHelperList)
                        {
                            item->SetSelected(false);
                        }
                        emit SelectionChanged(GetEditHelpers(true));
                    }
                }
            }                
        }
        else
        {
            QList <IFSEditHelper *> selecetHelperList = GetEditHelpers(true);
            mpFSEditHelper = GetPressedOnControlBoxEditHelper(m_LastPoint, offSet, m_PressedType);
            if (!mpFSEditHelper)
            {
                mpFSEditHelper = GetPressedOnEditHelper(m_LastPoint, offSet, m_PressedType);
                if (mpFSEditHelper)
                {
                    if (!mpFSEditHelper->IsSelected())
                    {
                        mpFSEditHelper->SetSelected(true);
                        for (auto item : selecetHelperList)
                        {
                            item->SetSelected(false);
                        }
                    }
                    mpFSEditHelper->SetShiftFlag(e->modifiers() & Qt::ShiftModifier);
                }
                else
                {                  
                    for (auto item : selecetHelperList)
                    {   
                        item->SetSelected(false);        
                    }
                    emit SelectionChanged(GetEditHelpers(true));
                }
            }
            else
            {
                mpFSEditHelper->SetShiftFlag(e->modifiers() & Qt::ShiftModifier);
            }
        }
    }
    emit CurrentEditHelperChanged(GetCurrentEditHelper(), true);
    emit sigRepaintWidget();
}

void FSEditHelperManager::OnMouseMoveEvent(QMouseEvent *e, QPointF offSet)
{  
    QList <IFSEditHelper *> selecetHelperList = GetEditHelpers(true);
    mbMultiSelcet = (selecetHelperList.size() > 1);
    if (m_PressedType != PRESSED_UNKNOW)
    {
        if (!m_bMove)
        {
            m_bMove = true;
            if (mbMultiSelcet&&IsContainTextHelper(selecetHelperList))
            {
                mpPlayerEditorManager->BeginComposite();
            }
        }
    }
        
    Dotranslate(e, offSet);   
    if (mpFSEditHelper&&m_bEnableSnap)
    {
        QPointF PT;
        IFSEditHelper::SnapType Type = mpFSEditHelper->GetSnapType();
        if (Type == IFSEditHelper::ZoomInOut || Type == IFSEditHelper::Trans || Type == IFSEditHelper::CtrlLine || Type == IFSEditHelper::CtrlShape)
        {
            PT = GetSnapOffSet(offSet);
            PT.setX(qAbs(PT.x()) < SnapWidth ? -PT.x() : 0);
            PT.setY(qAbs(PT.y()) < SnapWidth ? -PT.y() : 0);
            if (qAbs(PT.x()) > 1e-4 || qAbs(PT.y()) > 1e-4)
            {
                m_LastPoint = e->pos();
                QMouseEvent e1 = TransMouseEvent(e, PT);
                Dotranslate(&e1, offSet);            
            }
        }
    }
    QList <Qt::CursorShape> CursorList;
    for (int i = 0; i < selecetHelperList.size(); i++)
    {
        CursorList.push_back(selecetHelperList.at(i)->GetCursor());
    }
    mpParentWidget->setCursor(GetCursor(CursorList));
    m_LastPoint = e->pos();
}

void FSEditHelperManager::OnMouseReleaseEvent(QMouseEvent *e, QPointF offSet)
{
    m_LastPoint = e->pos();
    PressedType type;
    
    QList <IFSEditHelper *> selecetHelperList = GetEditHelpers(true);
    for (auto item : selecetHelperList)
    {
        if (mpFSEditHelper != item&&!mbMultiSelcet)
        {
            if (item != GetPressedOnControlBoxEditHelper(m_LastPoint, offSet, type) && item != GetPressedOnEditHelper(m_LastPoint,offSet,type))
                item->SetSelected(false);
        } 
        item->DoMouseRealese(m_LastPoint - offSet);
    }
    selecetHelperList = GetEditHelpers(true);
    if (mpPlayerEditorManager&&mpFSEditHelper&&m_PressedType != PRESSED_UNKNOW&&m_bMove&&!IsAllTextHelper(selecetHelperList))
    {
        mpPlayerEditorManager->PushCommand(new FSDeltaVisualCommand(selecetHelperList, m_Deltascale, m_DeltaTrans, m_DeltaAngle, m_PressedType));
    }
    if (mbMultiSelcet&&mpPlayerEditorManager&&mpFSEditHelper&&m_PressedType != PRESSED_UNKNOW&&m_bMove&&IsContainTextHelper(selecetHelperList))
    {
        mpPlayerEditorManager->EndComposite();
    }
    mbMultiSelcet = false;
    m_bMove = false;
    m_PressedType = PRESSED_UNKNOW;
    mpFSEditHelper = NULL;
}

void FSEditHelperManager::OnMouseDoubleClickEvent(QMouseEvent *e, QPointF offSet)
{
    QMouseEvent e1 = TransMouseEvent(e, offSet);
    QList <IFSEditHelper *> selecetHelperList = GetEditHelpers(true);
    mpFSEditHelper = GetPressedOnEditHelper(m_LastPoint, offSet, m_PressedType);
    if (mpFSEditHelper)
    {
        if (!mpFSEditHelper->IsSelected())
            mpFSEditHelper->SetSelected(true);
        mpFSEditHelper->DoMouseDoubleClickEvent(&e1);
    }
}

void FSEditHelperManager::UpdataDeltaInfo(bool bReset, QPointF deltaScale, QPointF deltaPos, double deltaAngle)
{
    if (bReset)
    {
        m_DeltaAngle = 0;
        m_DeltaTrans = { 0, 0 };
        m_Deltascale = { 1, 1 };
    }
    else
    {
        m_DeltaAngle += deltaAngle;
        m_DeltaTrans += deltaPos;
        m_Deltascale.setX(m_Deltascale.x()*deltaScale.x());
        m_Deltascale.setY(m_Deltascale.y()*deltaScale.y());
    }
}

bool FSEditHelperManager::IsAllTextHelper(QList<IFSEditHelper *> list)
{
    for (auto item:list)
    {
        if (item->GetEditType() != IFSEditHelper::Edit_Text&&item->GetEditType() != IFSEditHelper::Edit_Audio)
            return false;
    }
    return true;
}

bool FSEditHelperManager::IsContainTextHelper(QList<IFSEditHelper *> list)
{
    for (auto item : list)
    {
        if (item->GetEditType() == IFSEditHelper::Edit_Text)
            return true;
    }
    return false;
}

bool FSEditHelperManager::OpenXmlFile(QDomDocument &doc, QString strFile)
{
    QFile file(strFile);
    QString strError = "";
    int nRow = 0, nColumn = 0;
    if (!file.open(QIODevice::ReadOnly)) return false;
    if (!doc.setContent(&file, false, &strError, &nRow, &nColumn))
    {
        file.close();
        return false;
    }
    file.close();
    return true;
}

void FSEditHelperManager::InitHotKeyParmaMap()
{
    QString formatFile = PathOperation::MakeWorkPath(HotKeyFileName);
    bool bExisted = FileOperation::IsFileExist(formatFile);
    Q_ASSERT(bExisted);
    QDomDocument doc;
    if (!OpenXmlFile(doc, formatFile))
    {
        return;
    }
    QDomElement rootElem = doc.documentElement();
    QDomElement formatinfoElem = rootElem.firstChildElement();
    if (formatinfoElem.isNull() || rootElem.tagName() != "HotkeyParma") return;

    while (!formatinfoElem.isNull())
    {
        QString name = formatinfoElem.tagName();
        HotKeyParms parms;
        QDomElement tmpElem = formatinfoElem.firstChildElement();
        while (!tmpElem.isNull())
        {
            QPair<QString, QVariant> parm;
            parm.first = tmpElem.tagName();
            if (name == "Size")
                parm.second = QVariant(tmpElem.text().toFloat());
            else if (name == "Position")
            {
                QStringList list = tmpElem.text().split(",", QString::SkipEmptyParts);
                if (list.size() != 2)
                {
                    tmpElem = tmpElem.nextSiblingElement();
                    continue;
                }
                parm.second = QVariant(QPointF(list.first().toFloat(), list.last().toFloat()));
            }
            else if (name == "Style")
            {
                parm.second = tmpElem.text();
            }
            else
            {
                tmpElem = tmpElem.nextSiblingElement();
                continue;
            }

            parms.push_back(parm);
            tmpElem = tmpElem.nextSiblingElement();
        }

        if (!parms.isEmpty())
        {
            m_MapHotkeyParms.insert(name, parms);
        }


        formatinfoElem = formatinfoElem.nextSiblingElement();
    }
}

void FSEditHelperManager::SetDocState(bool bEdit)
{
    for (auto item : m_CurrentEditerList)
    {
        if (item->GetTimeLine()->GetType() == TimelineType::TimelineType_Annotaions_Caption)
        {
            static_cast<FSTextEditHelper *>(item)->SetDocState(bEdit, false);
        }
    }
}

bool FSEditHelperManager::isSelectEditHelper()
{
    return GetEditHelpers(true).size() > 0;
}

bool FSEditHelperManager::isEditTextHelper()
{
    QList<IFSEditHelper *>list= GetEditHelpers(true);
    for (auto item:list)
    {
        if (item->GetTimeLine()->GetType() == TimelineType::TimelineType_Annotaions_Caption)
        {
            bool bEdit, bTextEdit;
            static_cast<FSTextEditHelper *>(item)->GetDocState(bEdit,bTextEdit);
            if (bTextEdit)
                return true;
        }
    }
    return false;
}

void FSEditHelperManager::SetScaleMode(bool bScale)
{
    QList<IFSEditHelper *> list = m_CurrentEditerList;
    for (auto item : list)
    {
        item->EnableScaleMode(bScale);
    }
    emit sigRepaintWidget();
}

void FSEditHelperManager::EnableSnap(bool bEnable)
{
    m_bEnableSnap = bEnable;
}
