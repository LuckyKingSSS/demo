#ifndef FSEDITHELPERMANAGER_H
#define FSEDITHELPERMANAGER_H

#include <memory>
#include <QObject>
#include <QList>
#include <QPainter>
#include <QMap>
#include "FSPainterPath.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "FSEditHelper.h"

class QWidget;
class IMediaPlayerControl;
class IFSEditHelper;
class FSNLETimeline;

class QMouseEvent;
class QKeyEvent;
class QInputMethodEvent;
class QResizeEvent;
class INLETimelineBase;
class IPlayerEditorManager;

typedef std::shared_ptr<IFSEditHelper> IFSEditHelperPtr;

class FSEditHelperManager : public IFSEditHelperManager
{
    Q_OBJECT

public:
    enum PressedType{
        PRESSED_UNKNOW=-1,
        PRESSED_TOP_LEFT,
        PRESSED_TOP_MID,
        PRESSED_TOP_RIGHT,
        PRESSED_RIGHT_MID,
        PRESSED_BOTTOM_RIGHT,
        PRESSED_BOTTOM_MID,
        PRESSED_BOTTOM_LEFT,
        PRESSED_LEFT_MID,
        PRESSED_ROTATION,
        PRESSED_CENTER,
        PRESSED_CTRL1,
        PRESSED_CTRL2,
        PRESSED_EDIT=20,
    };
signals:
    void TextReDoUnDo(IFSEditHelper *pHelper);
    void MousePressedSig();
    void sigRepaintWidget();
    void SelectionChanged(QList<IFSEditHelper *> list);
public:
    FSEditHelperManager(IMediaPlayerControl *pControl,QObject *parent);
    ~FSEditHelperManager();

    void DoMousePressEvent(QMouseEvent *e, QPointF offSet);
    void DoMouseMoveEvent(QMouseEvent *e, QPointF offSet);
    void DoMouseUpEvent(QMouseEvent *e, QPointF offSet);
    void DoMouseDoubleClickEvent(QMouseEvent *e, QPointF offSet);
    void DoPaintEvent(QPainter &painter,QPointF pt);
    void DoKeyPressEvent(QKeyEvent *e);
    void DoinputMethodEvent(QInputMethodEvent *e);
    void DoResizeEvent(QResizeEvent *e, QSizeF scale);

    void UpdateEidtHelperList(QList<FSNLETimeline*> &list, QList<FSNLETimeline*> &selectList, QList<FSNLETimeline*> &OherselectList, bool ScaleState);
    void AddTextClip(QString &text);
    void SetDocState(bool bEdit);

    bool isSelectEditHelper();
    bool isEditTextHelper();

    void SetScaleMode(bool bScale);
    void EnableSnap(bool bEnable);

    QList<IFSEditHelper *>GetEditHelpers(bool bSelected);
    QList<IFSEditHelper *>GetResetableEditHelpers();

    void RemoveSelecetEditHelper();

    void TimeLineAddedMouseEficet(FSNLETimeline *pTimeLine,INLETimelineBase *filter);
    void TimeLineRemoveMouseEficet(FSNLETimeline *pTimeLine, INLETimelineBase *filter);

    void UpdataEidyHelperData(FSNLETimeline *pKeyTimeLine);

    void ClearEditHelper();

    void GetControlPath(QList<FSPainterPath> &pathList,QPointF offSet);
    void GetGuidesPointList(QList<QPointF> &hPointList, QList<QPointF> &vPointList,QPointF offset);
    void GetMainGuidesPointList(QList<QPointF> &hPointList, QList<QPointF> &vPointList, QPointF offset);

    bool IsPressedOnSelectControlBox(QPointF pos, QPointF offSet);

    IFSEditHelper * GetPressedOnControlBoxEditHelper(QPointF pos, QPointF offSet,PressedType &Type);
    IFSEditHelper * GetPressedOnEditHelper(QPointF pos, QPointF offSet, PressedType &Type);

    bool IsEditState(IFSEditHelper * &pHelper);

    void InitDefaultParam(FSNLETimeline *pTimeLine);
    void SetBorderWidth(int width);

    int  GetSelecetItemNum();
    void MultiSelectByRect(QRect rc);

    void SetPlayerEditorManager(IPlayerEditorManager *);
    IFSEditHelper * GetCurrentEditHelper();
    void UpdataDragEnterPathColor(QPointF pos, QPointF offSet);

    IFSEditHelper * GetTopLevelCanAddMouseEffcetEditHelper(QPointF pos, QPointF offSet);


	void SetMouseEffectDraging(bool bDraging) override;
    bool CanAddMouseEffect() override;
private:
    void SortFSEditHelperList(QList<FSNLETimeline *> &list);
    void UpdataDragingPathColor(bool bDraging);
    
    Qt::CursorShape GetCursor(QList<Qt::CursorShape> list);

    void GetGuidesPathPointList(QList<QPointF > targetPtList, QList<QPointF > guidesPtList, QList<QPointF> &hPointList, QList<QPointF> &vPointList);
    
    QPointF GetSnapOffSetFromGuidesPtList(QList<QPointF > targetPtList, QList<QPointF > guidesPtList);
    QPointF GetSnapOffSet(QPointF offset);
    void Dotranslate(QMouseEvent *e, QPointF offSet);

    bool isPreesedOnRolation();

    void OnTransformInfoChanged();

    
    void TextReDoUnDoSlot();

    QMouseEvent TransMouseEvent(QMouseEvent *e,QPointF offSet);
    void OnTimelineDelete();

    void OnMousePressEvent(QMouseEvent *e, QPointF offSet);
    void OnMouseMoveEvent(QMouseEvent *e, QPointF offSet);
    void OnMouseReleaseEvent(QMouseEvent *e, QPointF offSet);
    void OnMouseDoubleClickEvent(QMouseEvent *e, QPointF offSet);

    void UpdataDeltaInfo(bool bReset, QPointF deltaScale = { 1, 1 }, QPointF deltaPos = {0,0}, double deltaAngle=0);

    bool IsAllTextHelper(QList<IFSEditHelper *> list);
    bool IsContainTextHelper(QList<IFSEditHelper *> list);
    bool OpenXmlFile(QDomDocument &doc, QString strFile);
    void InitHotKeyParmaMap();

private:
    int                      mPressedIndex;
    int                      mBorderWidth;
    bool                     mbMultiSelcet;
    bool                     mbPressed;
    bool                     m_bEnableSnap;
    bool                     m_bMove;
    bool                     m_bDraging;
    QPointF                  m_Deltascale;
    QPointF                  m_DeltaTrans;
    double                   m_DeltaAngle;

    QColor                   m_DefaultPathColor;
    QColor                   m_DragingPathColor;
    QColor                   m_SelectDragingPathColor;

    PressedType              m_PressedType;
    QPointF                  m_LastPoint;
    QSizeF                   mPositionScale;
    QWidget                 *mpParentWidget;
    IMediaPlayerControl     *mpPlayerControl;
    IFSEditHelper           *mpFSEditHelper;
    IPlayerEditorManager    *mpPlayerEditorManager;
    QMap<FSNLETimeline*, IFSEditHelper *> mpEditHelperMap;
    QList<IFSEditHelper *> m_CurrentEditerList;

    QMap<QString, HotKeyParms>  m_MapHotkeyParms;
};

#endif // FSEDITHELPERMANAGER_H
