#ifndef TVTRACKVIEW_H
#define TVTRACKVIEW_H

#include "inc_FSCommonlib/FSCommonWidget.h"
#include "TVControl.h"

#define TRACK_WIDGET_HEADER_WIDTH	104
#define TRACK_WIDGET_HEIGHT			56
#define CLIP_THUMBNAIL_WIDTH		80


class TimelineManager;
class TrackBuilder;
class TimelineRuler;
class MenuContextMgr;
class TimelineViewPrivate;
class SelectionWidget;
class TVTrackView : public FSBaseWidget
{
	Q_OBJECT
public:
	TVTrackView(TimelineRuler *pRuler,QWidget *parent);
	~TVTrackView();
	void Resize();
	TimelineRuler* Ruler();

	void ResizeGeometry(int l, int t, int w, int h);
	int Height();
	bool IsKeySpacePressed();
	bool IsDragScroll();
protected:
	bool CanDrag(QPoint pt);

	void dragEnterEvent(QDragEnterEvent *e) override;
	void dragMoveEvent(QDragMoveEvent *e) override;
	void dragLeaveEvent(QDragLeaveEvent *e) override;
	void dropEvent(QDropEvent *e) override;

	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

	void resizeEvent(QResizeEvent *e) override;
	bool eventFilter(QObject *obj, QEvent *e) override;
	void OnMenuPopup(QPoint pt);
Q_SIGNALS:
	void sigDurationChanged(int duration);
private:
	void slotRebuild();
	void slotUpdate();
	void OnCurrentFrameChanged(int frame);
	void CheckDuration();
	void OnRightButtonTimerOut();
	void OnKeySpaceTimerOut();
	void SetHandCursor(bool bSet);
private:
	friend class TrackWidget;
	TrackBuilder *m_pTrackBuilder;
	TimelineRuler *m_pRuler;
	TimelineManager *m_pTimelineMgr;
	MenuContextMgr *m_pMenuContextMgr;

	bool m_bPressed;
	bool m_bEmptyPress; //øÚ—°”√
	bool m_bDoHitSelect;
	bool m_bRectagleSelete;
	bool m_bTimelineDraging;
	QPoint m_ptPressed;
	TimelineViewPrivate *m_pTimelineViewPrivate;

	bool m_bNormalDrag;
	int m_nPressTrackIndex;

	SelectionWidget *m_pSelectionWidget;
	QWidget *m_pLine;

	bool m_bRightButtonPressed;
	QPoint m_ptRightPressed;
	int m_nHscrollBarValue;
	int m_nVscrollBarValue;

	QElapsedTimer m_RightButtonDragElapseTimer;
	QTimer m_RightButtonTimer;
	bool m_bSetCursorHandCursor;

	bool m_bKeySpacePressed;
	QElapsedTimer m_KeySpaceElapseTimer;
	QTimer m_KeySpaceTimer;
};

class TimelineTrack;
class TVTrackWidgetDraw;
class ClipMarkerControl;
class TrackWidget : public FSBaseWidget
{
	Q_OBJECT
public:
	TrackWidget(TimelineTrack* pTrackData,QWidget *parent);
	void SetRuler(TimelineRuler *pRuler);
	TimelineTrack* GetTrackData();
	Clips GetClips();
	Clips GetDragClips();
	Clips GetUnDragClips();
	int Duration();
	TrackWidget *PreTrackWidget();
	TrackWidget *NextTrackWidget();
	int Height();

	void SetButtonHide(bool bHide);
	void SetButtonFreeze(bool bFreeze);
	Clip* IsHitClip(QPoint pt);
	bool IsHitHeader(QPoint pt);

	void SetKeyControlDown(bool bDown);
	bool IsKeyControlDown();

	QSize GetMarkerSize();
	int GetMarkerTop();
protected:
	void OnHide();
	void OnShow();
	void OnFreeze();
	void OnUnFreeze();

	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent *e) override;

	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

	
    void RetranslateUi() override;
	
	
private:
	TimelineTrack* m_pTrackData;
	TimelineRuler *m_pRuler;
	TVTrackWidgetDraw *m_pDraw;

	BaseStateButton *m_btnHide;
	BaseStateButton *m_btnFreeze;
	bool m_bKeyControlDown;

	ClipMarkerControl *m_pClipMarkerControl;
};

class CusMenu;
class MenuContextMgr : public QObject
{
	Q_OBJECT
public:
	MenuContextMgr(QObject *parent ,TVTrackView* pWidget);
	void Popup(QPoint pos);

protected:
	void ConnectSignals();
	void RetranslateUi();
private:
	TimelineManager *m_pTimelineMgr;
	TVTrackView *m_pTVTrackView;

	CusMenu *m_Menu;
	QAction *m_pActCut;
	QAction *m_pActCopy;
	QAction *m_pActPaste;
	QAction *m_pActDelete;
	QAction *m_pActSplit;
	QAction *m_pActMuteAudio;
	QAction *m_pActAudioDetach;
	QAction *m_pActFreezeFrame;
	QAction *m_pActAddMarker;


	QAction *m_pActSelectGroup;
	ColorMenuItem *m_pColorMenuItem;


	QAction *m_pActInserTrack;
	QAction *m_pActRemoveEmptyTrack;

	QAction *m_pActVisible;
	QAction *m_pActUnvisible;
	QAction *m_pActLock;
	QAction *m_pActUnlock;
	int m_nCurTrackIndex;
};

class TLToolTipMgr : public QObject
{
	Q_OBJECT
public:
	static TLToolTipMgr* GetInstance();
	~TLToolTipMgr();
	void SetView(TimelineViewPrivate *view);

Q_SIGNALS:
	void sigShowTip(bool bShow,int startPos,int lenght,QPoint globalPos);

protected:
	enum {timer_none = 0,timer_prepare,timer_showing};
	void OnTimeOut();
	bool eventFilter(QObject *obj, QEvent *e) override;
private:
	TLToolTipMgr(QObject *parent = 0);
private:
	QTimer m_timer;
	bool m_bShowing;
	QPoint m_ptCurrent;
	Clip *m_clip;
	TimelineViewPrivate *m_pTimelineViewPrivate;
	int m_nTimerType;
};

class ClipMarkerControl : public QObject
{
	Q_OBJECT
public:
	ClipMarkerControl(QObject *parent, TrackWidget* pWidget);

protected:
	int IsHitMarker(QPoint globlaPt);
	void RetranslateUi();

	bool eventFilter(QObject *, QEvent *) override;
private:
	TrackWidget* m_pTrackWidget;

	int m_nHitMarker;
	Clip *m_pHitClip;
	bool m_bLeftClick;

	QMenu *m_pMenu;
	QAction *m_pActDelete;
	QAction *m_pActDeleteAll;
};

#endif // TVTRACKVIEW_H
