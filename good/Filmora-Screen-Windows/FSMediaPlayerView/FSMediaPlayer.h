#ifndef FSMEDIAPLAYER_H
#define FSMEDIAPLAYER_H
#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include "FSPainterPath.h"
#include "Include/NLEComPtr.h"
#include "Interface/INLECaption.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"

class MediaPlayerView;
class FSBaseToolBar;
class FSRecorderBaseToolBar;
class FSCropWidget;
class FSFullScreenControl;
class IMediaPlayerControl;
class FSNLETimeline;
class INLETimelineBase;
class FSEditHelperManager;
class IFSEditHelperManager;
class ICaptionEnvironment;
class IFSEditHelper;

class FSPreviewEditView :public FSBaseWidget{
    Q_OBJECT
public:
    FSPreviewEditView(IMediaPlayerControl *pMediaPlayerControl, QWidget *pParent = 0);
    ~FSPreviewEditView();

    void SetTimeLineList(QList<FSNLETimeline*> &list, QList<FSNLETimeline*> &selectList, QList<FSNLETimeline*> &OherselectList, bool ScaleState);
    QList<FSNLETimeline*> GetTimelineList();
    IFSEditHelperManager * GetEditHelperManager();
    void ShowEditView(bool bShow);
    bool IsSelectEditHelper();
    QList<IFSEditHelper*> GetEditHelperList(bool bSelecet);
    void RemoveSelecetEditHelper();
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent  *e);
    void paintEvent(QPaintEvent *e);
    void inputMethodEvent(QInputMethodEvent *e);
    void resizeEvent(QResizeEvent *e);
    void SetBaseRenderSize(QSize size);
    void SetRenderSize(QSize widgetSize);
    void ClearEditHelper();
    void GetPreviewPath(QList<FSPainterPath> &pathList);
    void GetPreviewGuidesPath(QList<FSPainterPath > &guidesPath, int width, int height, QPointF offSet);
    QPoint GetPreiewOffsetPoint();

    void SetPlayerBorderWidth(int w);
    int GetPlayerBorderWidth();
    void SetPlayerBorderColor(QColor color);
    void SetPlayerBackgroundColor(QColor color);

    int GetSelectItemNum();

    void MultiSelectByRect(QRect rc);

protected:   
    QPointF GetMouseEventOffSet(QMouseEvent *e);
    void RetranslateUi()override;

    QImage GetImage();

private:
    void ConnectSignals();
    void CreateUI();
    
private:
    QList <FSNLETimeline*>  mTimeLineList;
    IMediaPlayerControl    *mpMediaPlayerControl;
    FSEditHelperManager    *mpEditHelperManager;
    ICaptionEnvironment    *mpCaptionEnv;
    QSize                   m_BaseRenderSize;
    double                  m_Width;
    double                  m_Height;

    int                     m_PlayerBorderWidth;
    QColor                  m_PlayerBorderColor;
    QColor                  m_PlayerBackgroundColor;
};

class FSViewPort;
class FSEditPlayerView;
class FSMediaScrollArea :public QScrollArea{
    Q_OBJECT
signals:
    void sigChangeScale(bool bZoom);
    void sigMenuChangeScale(QString scale);
    

public:
    FSMediaScrollArea(FSEditPlayerView *pWidget, QWidget *pParent);
    ~FSMediaScrollArea();
    void SetScaleSize(bool bFit, float scale);

    void SetBaseRenderSize(QSize size);

    void ResetMarkPoint();


protected:
    void wheelEvent(QWheelEvent *);
    void resizeEvent(QResizeEvent *e);
    void slotMousePressed(QPointF pos);
    void slotMouseReleased(QPointF pos);
    void slotMouseMoved(QPointF pos);
    bool event(QEvent *e);
    void scrollContentsBy(int x,int y);
private:
    void UpdateViewPort(int value);
    void ConnectUpdateViewPort(bool bConnect);
    void slotZoom(bool bFit, QString scale);

private:
    QWidget     *m_pWidget;
    QVBoxLayout *m_pVL;
    QSize       m_BaseSize;
    FSViewPort  *m_pViewPort;
    FSEditPlayerView *m_pEditPlayerView;
    QPointF     m_PointLast;
    QPointF     m_PonitMarkPoint;
    bool        m_bReset;
    bool        m_bFit;
};

class FSEditPlayerView;
class FSViewPort :public QLabel{
    Q_OBJECT
public:
    FSViewPort(QWidget *pParent, FSEditPlayerView *pEditPlayerView);
    ~FSViewPort();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent  *e);
    void inputMethodEvent(QInputMethodEvent *e);
    void wheelEvent(QWheelEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void enterEvent(QEvent *);
    

signals:
    void sigMousePressed(QPointF pos);
    void sigMouseMoved(QPointF pos);
    void sigMouseReleased(QPointF pos);

private:
    FSEditPlayerView  *m_pEditPlayerView;
    FSPreviewEditView *m_pPreviewWidget;
    QColor             m_BoxFillColor;
    QRect              m_MultiSelectRect;
    bool               m_bEnableDrawMultiSelectRect;
    bool               m_bPressed;
    bool               m_bRightPressed;
    bool               m_bRightLongPressed;
};


class FSMediaPlayerView : public QWidget
{
    Q_OBJECT
signals:
    void MousePreesEvent(QMouseEvent *e);
    void MouseDoubleClicked();
public:
    FSMediaPlayerView(QWidget *pParent);
    ~FSMediaPlayerView();
    IMediaPlayerControl *GetMediaPlayerControl();
    MediaPlayerView *GetMediaPlayerView(); 
    void resizeEvent(QResizeEvent *);
    void SetPlayerBorderWidth(int w);
    void SetPlayerBorderColor(QColor color);

    void GetSnapShortImg(QImage & image);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *e);

    void paintEvent(QPaintEvent *);
    

private:
    MediaPlayerView *mpMediaPlayerView;
    QTimer          *m_pTimer;
    bool             m_bClicked;
    QMouseEvent      m_MouseEvent;
    int              m_PlayBorderWidth;
    QColor           m_BorderColor;
};


class FSPlayerMenuControl :public QObject{
    Q_OBJECT

public:
    enum ActionId{
        Action_Reset,
        Action_Copy,
        Action_Paste,
        Action_Cut,
        Action_Delete,
        Action_ProjectSetting,
        Action_PlayResolution_Full,
        Action_PlayResolution_1_2,
        Action_PlayResolution_1_4,
        Action_PlayResolution_1_8,
        Action_PlayResolution_1_16,

        Action_Zoom_Fix,
        Action_Zoom_100,
        Action_Zoom_85,
        Action_Zoom_75,
        Action_Zoom_60,
        Action_Zoom_50,
        Action_Zoom_35,
        Action_Zoom_25,
        Action_Zoom_10,
        Action_Zoom_125,
        Action_Zoom_150,
        Action_Zoom_175,
        Action_Zoom_200       
    };
public:
    FSPlayerMenuControl(QObject *pParent);
    ~FSPlayerMenuControl();


    void EnableAction(int id,bool bEnable);
    void CheckAction(int id);
    void PopUp(QPointF pt);
    void RetranslateUi();

    void UpdataZoomMenuCheckState(bool bfit, double scale);
    void UpdataPlayResolutionMenuCheckState(double scale);

private:
    void InitAction();
    void OnActionTriggered();

signals:
    void ActionTriggered(int id);

private:
    QMenu  *m_pMenuEdit;
    QMenu  *m_pMenuPlayResolution;
    QMenu  *m_pMenuZoom;
    QList <QAction *> m_ActionList;
};


#define Drag_AddPos L"Drag_AddPos"

class IPlayerEditorManager;
class ITimelineViewManager;
class ITitleBarManager;
class FSPlayerShortCutContrl;
class FSEditPlayerView : public FSBaseWidget
{
    Q_OBJECT
signals:
    void ShowEditViewSig(bool bShow);
    void sigEnableEditShotCut(bool bEnable);
    void sigBackForwedShortCutTriged(int frames);
    void sigSpaceLongPress(bool bPress);
    void sigEditModeChanged(bool bScaleMode);
    void sigProjectSettingTrigged();
    void sigZoom(bool bFit, QString scale);
    void sigResolutionChanged(float resolutionScale);

public:
    FSEditPlayerView(QWidget *parent);
    ~FSEditPlayerView();
    bool Open(int bSource, FSNLETimeline *timeline);
    bool Close();
    void Play();
    void Stop();
    void Pause();
    void Seek(int frames);
    void SetDuration(int duration);
    void Forward();
    void Backward();
    void Restore();
    int  Duration();
    int CurrentFrame();
    bool HaveSource();
    void PlayerViewChange2EditView(bool bShowEdit);
    void OnPlayerViewMousePress(QMouseEvent *e);
	bool IsPlaying();
    void SetPlayerEditManager(IPlayerEditorManager*);
    void SetTimelineViewManager(ITimelineViewManager*);
    void SetTitleBarMannager(ITitleBarManager *);

    ITimelineViewManager* GetTimelineViewManager();
    IFSEditHelperManager * GetEditHelperManager();
    ITitleBarManager *GetTitleBarMannager();
    IMediaPlayerControl *GetMediaPlayerControl();
    IPlayerEditorManager *GetPlayerEditorManager();
    FSPlayerMenuControl *GetMenuControl();
    FSPlayerShortCutContrl *GetShortCutControl();
    void SetChunkHeight(int height);
    void OnMediaPlayerBarStartChange();
    void OnMediaPlayerBarEndChange();
    void OnMediaPlayerBarChange(int frame);
    void OnMediaPlayerBarFullScreen();
    void OnMediaPlayerBarCropView(bool bScaleMode);
    void resizeEvent(QResizeEvent *e);
    FSPreviewEditView *GetPreviewEditView();
    void AddDropTimeLineList(QList<FSNLETimeline *> list,QPointF DragPos);
    void AddDropMouseClickSound(QString filePath,QPointF DragPos,QPointF offset);
    void AddDropMouseFliter(FSNLETimeline * pTimeLine, QPointF DragPos, QPointF offset);


    void SetPlayerBorderWidth(int w);
    int GetPlayerBorderWidth();
    void SetPlayerBorderColor(QColor color);
    void GetSnapShortImg(QImage & image);

    bool isPressingSpace();
    void SetBaseRenderSize(QSize size);

    void SetPlayBackResolution(double resolution);
    float GetPlayBackResolution();
    void SetPlayerBackgroundColor(QColor color);
    

protected:
    void OnFullScreen();
    void OnCropView(bool bScaleMode);
    void OnShortCutActived(int id);
    void OnLongPressedShortCut(int id,bool bPress);
   
protected: 
    void keyPressEvent(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void RetranslateUi()override;
    
private:
    void ConnectSignals();
    void CreateUI();
    void OnTimeLineUpdataSlot();
    void OnTimeLineSelectionChangedSlot();
    void OnDurationChangedSlot(int duration);
    void OnTimeLineAddedSlot(FSNLETimeline *pTimeLine);
    void OnTimelineClear();

    void OnTimeLineViewSeekStartSlot();
    void OnTimeLineViewSeekSlot(int frames);
    void OnTimeLineViewSeekEndSlot();
    void OnMouseEffectAddedSlot(FSNLETimeline *pTimeLine, INLETimelineBase *filter);
    void OnMouseEffectRemoveSlot(FSNLETimeline *pTimeLine, INLETimelineBase *filter);
    void OnSelectionChanged(QList<IFSEditHelper *> list);
    void OnTextReDoUnDoSlot(IFSEditHelper *pHelper);
    void OnMousePressedSlot();
    void EnableShortCut(bool bEnable, bool bPlayerShotcutOnly=false);

    

    QSizeF GetTimeLineInitRenderSize(FSNLETimeline *pTimeLine, QSizeF renderSize);

private:
    FSMediaPlayerView   *mpMediaPlayerView;
    FSPreviewEditView   *mpPreviewEditView;
    FSBaseToolBar       *mpPlayerToolBar;
    FSFullScreenControl *mpFullScreenControl;
    QWidget             *mpMainWindow;
    IPlayerEditorManager*mpPlayerEditorManager;
    ITimelineViewManager*mpTimelineViewManager;
    ITitleBarManager    *mpTitleBarMannager;
    IMediaPlayerControl *mpControl;
    FSPlayerShortCutContrl *mpShortCutContrl;
    FSPlayerMenuControl *mpMenuControl;
    bool                 mbSeekManuel;
    double               m_PlayResolutionScale;
    QSize                m_BaseRenderSize;

	int                  m_iBegineSeekTime;//启动seek的事件点
	int                  m_iEndSeekTime;//结束seek的时间点
};

class FSPropertyView;
class FSEditPlayerView;
class FSPropertyEditPlayerView :public QWidget
{
    Q_OBJECT
public:
    FSPropertyEditPlayerView(QWidget *pParent);
    ~FSPropertyEditPlayerView();

    FSEditPlayerView *GetEditPlayerView();

private:
    void CreateUI();
    void ConnectSignals();
    void resizeEvent(QResizeEvent *e);

private:
    FSEditPlayerView *m_pEditPlayerView;
    FSPropertyView *m_pPropertyView;
};


class FSMediaPlayer :public QWidget{
    Q_OBJECT
signals :
    void RulerSeekCallback(int frames);
    void StartRulerSeekCallback(bool bPlaying);
    void EndRulerSeekCallback(bool bPlaying);

public:
    FSMediaPlayer(QWidget *parent);
    ~FSMediaPlayer();
    bool Open(int bSource, FSNLETimeline *timeline);
    bool Close();
    void Play();
    void Stop();
    void Pause();
    void Seek(int frames);
    void SetDuration(int duration);

    void Forward();
    void Backward();
    void Restore();
    int  Duration();

    bool HaveSource();
	bool IsPlaying();
    void SetChunkHeight(int height);
    void Update();

    void GetSnapShortImg(QImage & image);
protected:
    void OnFullScreen();

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    void SetShortCut();

private:
    FSMediaPlayerView     *mpMediaPlayerView;
    FSRecorderBaseToolBar *mpPlayerToolBar;
    FSFullScreenControl   *mpFullScreenControl;
    QWidget               *mpMainWindow;
    IMediaPlayerControl   *mpControl;

    QShortcut              *m_pScStop;
    QShortcut              *m_pScFullScrn;
};

#endif // FSMEDIAPLAYER_H
