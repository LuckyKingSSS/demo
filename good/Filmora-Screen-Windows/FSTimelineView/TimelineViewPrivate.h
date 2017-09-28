#ifndef TIMLINEVIEWPRIVATE_H
#define TIMLINEVIEWPRIVATE_H

#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSTimelineView/fstimelineview.h"
#include "Include/NLEComPtr.h"
#include "inc_FSCommonlib/FSNLETimeline.h"

class TVToolBar;
class TVTrackView;
class TimelineRuler;
class TimelineManager;
class IPlayerEditorManager;
class ITimelineViewManager;
class TitleBarManager;
class RulerBkGround;
class TopMaskView;
class TLMarkLayer;
class TimelineViewPrivate : public FSBaseWidget
{
	Q_OBJECT

public:
	TimelineViewPrivate(QWidget *parent);
	IPlayerEditorManager* GetPlayerEditManager();
	ITimelineViewManager* GetTimelineViewManager();
	ITitleBarManager* GetTitleBarManager();
	void Resize();
	QScrollBar* GetHScrollBar();
	QScrollBar* GetVScrollBar();
	TVTrackView* GetTrackView();
	void EnableShortcuts(bool bEnable);
	void EnableToolBar();
	void TransFromGaInfo();
	void Zoom2FitTimeline();
	TopMaskView* GetTopMaskView();
public slots:
	void slotButtonClicked(int id);
	void slotReprtGaInfo(FSNLETimeline* timeline);
protected:
	void HorizontalScrollBar(bool bDurationChange = false);
	void OnAdjustVScroolBar(FSNLETimeline *addedTimeline);
	void OnAddMarker();
	
	void resizeEvent(QResizeEvent *e) override;
	void wheelEvent(QWheelEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
    bool eventFilter(QObject *, QEvent *) override;
	void closeEvent(QCloseEvent *event)override;
private:
	TVToolBar *m_pToolBar;
	TimelineRuler *m_pRuler;
	TVTrackView *m_pTrackView;
	QScrollBar *m_pScroolBarH;
	QScrollBar *m_pScroolBarV;
	TimelineManager* m_pTimelineMgr;
	ITimelineViewManager * m_pTimelineViewManager;

	QWidget *m_pRulerToolBarMask;
	RulerBkGround *m_pRulerBkGround;
	TopMaskView *m_pTopMaskView;
	TLMarkLayer *m_pTLMarkLayer;

	int m_nRealTopPosition;
	bool m_bSeeking;

	QMenu *m_pMenuTrackMark;
	QAction *m_pActAddTrackMark;
};

class ScaleUnitSlider;
class TVToolBar : public FSBaseWidget
{
	Q_OBJECT
public:
	enum ButtonId
	{
		ButtonId_BeginFlag = 0,
		ButtonId_Undo,
		ButtonId_Redo,
		ButtonId_Split,
		ButtonId_Mark,
		ButtonId_Zoom2FitTimeline,
		ButtonId_EndFlag
	};
public:
	TVToolBar(QWidget *parent);
	void SetScaleUnitValue(int value);
	int GetScaleUnitValue();
	int GetScaleUnitDuration();
	void SetButtonEnabled(ButtonId id, bool enable);

	void EnableShortCuts(bool bEnable);
Q_SIGNALS:
	void sigClicked(ButtonId id);
	void sigScaleValueChanged(int value);
	void sigStartValueChanged();
protected:
	void InitButtons();
	void AddButton(ButtonId id);
	BaseButton* GetButton(ButtonId id);
	void resizeEvent(QResizeEvent *e) override;
    void RetranslateUi() override;
    QString GetButtonTooltipFromButtonID(ButtonId id);

private:
	QList<BaseButton*> m_lstButtons;
	ScaleUnitSlider *m_pSlider;	

	QList<QShortcut*> m_lstShortcuts;
};

class ScrollMgr : public QObject
{
	Q_OBJECT
public:
	static ScrollMgr* GetInstance();
	void Init(TimelineViewPrivate* view);
	void AutoScroll(bool bOn);
protected:
	void OnTimer();
private:
	ScrollMgr(QObject *parent = 0);
private:
	TimelineViewPrivate* m_TimelineViewPrivate;
	bool m_bAutoScrollOn;
	QTimer m_timer;
};

class TimelineViewManager : public ITimelineViewManager
{
	Q_OBJECT
public:
	static TimelineViewManager* GetInstance();
	void SetTimelineViewPrivate(TimelineViewPrivate *pTimelineViewPrivate);
	void SetCurrentFrame(int frame) override;
	int CurrentFrame() override;
	FSNLETimeline* GetRootTimeline() override;
	void FetchAudioWaveData(FSNLETimeline *audioTimeline) override;
	void UpdateView() override;
	QString GetProjectString() override;
	void LoadProjectString(const QString projString) override;
	void Clear() override;
	void EnableShortcuts(bool bEnable) override;
	void SetSpeedScale(FSNLETimeline* timeline, float scale) override;
	void DenoiseLevelChanged(FSNLETimeline *timeline) override;
	void SetMouseEffectDraging(bool bDraging) override;
Q_SIGNALS:
	void sigCBCurrentFrameChanged(int frame);
	void sigCBUpdateView();
private:
	TimelineViewManager(QObject *parent = 0);

	TimelineViewPrivate *m_pTimelineViewPrivate;
};

class INLEDocumentSerialize;
class INLEDocumentSerializeEx;
class SerializeHelper : public QObject
{
	Q_OBJECT
public:
	static SerializeHelper* GetInstance();
	void Load(const QString &str);
	QString GetString();
	void Clear();
private:
	SerializeHelper(QObject *parent = 0);

	NLEComPtr<INLEDocumentSerialize> m_pSerialize;
	INLEDocumentSerializeEx* m_pISerailizeEx;
};

class RulerBkGround : public QWidget
{
	Q_OBJECT
public:
	RulerBkGround(QWidget *parent);

protected:
	void paintEvent(QPaintEvent *e) override;
};

class TrimToolTip;
class TLMarkWidget;
class MarkPoint;
class TopMaskView : public QWidget
{
	Q_OBJECT
public:
	TopMaskView(QWidget *parent);

	void Update();
protected:
	void paintEvent(QPaintEvent *e) override;
	void ShowToolTip(bool bShow, QString text = "", QPoint globalPos = QPoint());
private:
	TrimToolTip *m_pToolTipWidget;
};

class TrimToolTip : public QWidget
{
	Q_OBJECT
public:
	TrimToolTip(QWidget *parent);
	void SetToolTip(const QString &text);
protected:
	void resizeEvent(QResizeEvent *e) override;
protected:
	QLabel *m_pLbText;
};


class CusMenu;
class ColorMenuItem;
class TLMarkLayer : public FSBaseWidget
{
	Q_OBJECT
public:
	TLMarkLayer(TVTrackView* pTrackView,QWidget *parent);
protected:
	void RetranslateUi() override;
	MarkPoint* IsHitMark(QPoint globalPt);
	void OnRightClick();

	void paintEvent(QPaintEvent *e) override;
	bool eventFilter(QObject *, QEvent *) override;

private:
	int m_MarkWidgetW;
	int m_MarkWidgetH;
	TVTrackView *m_pTVTrackView;
	MarkPoint *m_pHitMark;
	bool m_bLeftClicked;
	MarkPoint m_MovingMark;

	CusMenu *m_pMenu;
	QAction *m_pActDelete;
	QAction *m_pActDeleteAll;
	ColorMenuItem *m_pActColor;
};



#endif // TIMLINEVIEWPRIVATE_H
