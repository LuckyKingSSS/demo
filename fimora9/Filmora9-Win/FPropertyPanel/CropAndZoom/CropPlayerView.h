#ifndef CROPPLAYERVIEW_H
#define CROPPLAYERVIEW_H

#include "inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"
#include "inc_MediaPlayerView\MediaPlayerView.h"

class FEditView;
class MediaPlayerView;
class FNLETimeline;
class FPannelMediaPlayerBar;
class IMediaPlayerControl;
class FAfterEditView;
class FPauseStateView;
class FPROPERTYPANEL_EXPORT CropPlayerView : public FBaseWidget
{
	Q_OBJECT

public:
	CropPlayerView(FNLETimeline* timeline ,QWidget *parent);
	~CropPlayerView();
	void SetTimeline(FNLETimeline* timeline);
	void SetDuration(int iDurationTime);

protected:
	void resizeEvent(QResizeEvent *event)override;

private:
	void InitUI();
	void InitConnections();
	void InitData();
private:
	FEditView* m_EditView;
	FPauseStateView* m_PauseStateWidget;
	MediaPlayerView* m_PreViewPlayer;
	FPannelMediaPlayerBar* m_CropPlayerToolbar;
	FNLETimeline* m_Timeline;
	IMediaPlayerControl* m_PlayerControl; //²¥·Å¿ØÖÆÆ÷
};


class CropEditMgr;
class IMediaPlayerControl;
class FEditViewMouseEvent;
class FCropRegionRect;
class FPauseStateView;
class FEditView : public FBaseWidget
{
	Q_OBJECT
public:
	FEditView(FNLETimeline* timeline,IMediaPlayerControl *control, QWidget *parent);
	~FEditView(){};
	void SetPlayerRender();
	void Open();
	HWND getHwnd(){ return (HWND)m_PreViewPlayer->winId(); }
	void SetCropRegion(QRect regionRec);

Q_SIGNALS:
	void sigPlayViewChanged(const QRect &r);
	void sigPausePlay(QRect rect);
	void sigStartPlay(QRect rect);

protected:
	void resizeEvent(QResizeEvent *event)override;
	void mousePressEvent(QMouseEvent *event)override;
	void mouseMoveEvent(QMouseEvent *event)override;
	void mouseReleaseEvent(QMouseEvent *event)override;

private:
	void InitUI();
	void InitConnections();
	void InitData();
	

private:
	FBaseWidget* m_PreViewPlayer;
	CropEditMgr*     m_CropEditMgr;
	FCropRegionRect* m_CropRegion;
	NLEComPtr<IPlayerView> m_CropEditPlayerView;
	IMediaPlayerControl*  m_MediaPlayerCtl;
	FEditViewMouseEvent*  m_EditViewMouseEvent;
	FNLETimeline* m_Timeline;
};

class FPauseStateView :public FBaseWidget
{
  Q_OBJECT

public:
	FPauseStateView(IMediaPlayerControl* pPlayerControl,QWidget* parent);
	~FPauseStateView(){};

public:
	void SetPlayerBackgroundColor(QColor color);
	void SetCropRegion(QRect rec);
	HWND getHwnd();
	void ResetRegion();

protected:
	void paintEvent(QPaintEvent *event)override;
	void mousePressEvent(QMouseEvent *event)override;
	void mouseReleaseEvent(QMouseEvent *event)override;
	void mouseMoveEvent(QMouseEvent *event)override;
	void resizeEvent(QResizeEvent *event)override;

Q_SIGNALS:
	void sigStartPlay(QRect rec);

private:
	QImage GetCurBackgroundImg();
	void InitUI();
	void InitConnections();
	QImage GetDefaultBackImage();

private:
	FEditViewMouseEvent*  m_PauseStateViewEvent;
	IMediaPlayerControl*   m_pPlayerControl;
	CropEditMgr*    m_CropRegionMgr;
	QColor                  m_PlayerBackgroundColor;
};


class FCropRegionRect :public FBaseWidget
{
   Q_OBJECT

public:
	FCropRegionRect(QWidget* parent);
	~FCropRegionRect(){};
protected:
	void paintEvent(QPaintEvent *event)override;
};


class FEditViewMouseEvent :public QAbstractNativeEventFilter
{
public:
	FEditViewMouseEvent(FBaseWidget *view);
	~FEditViewMouseEvent();
public:
	bool nativeEventFilter(const QByteArray& eventType, void * message, long * result) override;
	void SetPauseState(bool bIsPauseState){ m_bIsPauseState = bIsPauseState; };
private:
	int PointDPIRatio(int x);
	HWND GetMsgHander(MSG *msg);
private:
	FBaseWidget *m_displayView;
	QElapsedTimer m_elapsedTimer;
	bool m_bIsPauseState;
};


enum AspectRatio
{
	AspectRatio_Manually = 0,
	AspectRatio_16_9,
	AspectRatio_4_3
};
class CropLineWidget;
class CropBlockWidget;
class CropEditMgr :public QObject
{
	Q_OBJECT
public:
	enum BlockID
	{
		BlockID_0 = 0,
		BlockID_1,
		BlockID_2,
		BlockID_3,
		BlockID_4,
		BlockID_5,
		BlockID_6,
		BlockID_7,
		BlockID_100 = 100
	};
	CropEditMgr(QObject *parent);
	~CropEditMgr(){};
	void SetFrameRect(const QRect &rect);
	void SetAspectRatio(AspectRatio ratio);
	QRect GetAdjustRect();
	bool PtInBlock(const QPoint &pt);
	void MousePress(int nId, const QPoint &p);
	void MouseRelease(const QPoint &p);
	void MouseMove(const QPoint &p);
	void Reset();
	void SetAdjustRect(BlockID nId, const QRect &rect);
	QRect GetFrameRect(){ return m_rcFrame; };
protected:
	void Init();
	void UpdateUi();
	void SetBlockViewCursor(BlockID nId, CropBlockWidget *pView);

	void ManuallyRestrict(BlockID nId, const QRect &rect);
	void SpecialRestrict(BlockID nId, const QRect &rect);
	void UpdateDestCropRect();
Q_SIGNALS:
	void signalCropRectUpdate(const QRectF &rc);
private:
	QWidget *m_parentWidget;
	QRect m_rcFrame;
	QRect m_rcAdjust;
	QList<CropLineWidget*> m_lineViews;
	QList<CropBlockWidget*> m_blockViews;

	bool m_bPressed;
	QPoint m_pPressPt;
	QPoint m_ptPressCenter;
	QPoint m_pReleasePt;
	BlockID m_pPressId;

	QPoint m_pPrev;
	AspectRatio m_AspectRatio;
};


class CropLineWidget : public FBaseWidget
{
	Q_OBJECT
public:
	CropLineWidget(Qt::Orientation ori, QWidget *parent);

protected:
	void paintEvent(QPaintEvent *event) override;
private:
	Qt::Orientation m_Orientation;
};


class CropBlockWidget : public FBaseWidget
{
	Q_OBJECT
public:
	CropBlockWidget(QWidget *parent);

Q_SIGNALS:
	void signalLBtnPressed(const QPoint &p);
	void signalLBntRelease(const QPoint &p);
	void signalMouseMove(const QPoint &p);
protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
private:
	static const int w = 8;
	static const int h = 8;
};


#endif // CROPPLAYERVIEW_H
