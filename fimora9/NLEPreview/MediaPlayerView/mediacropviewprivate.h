#ifndef MEDIACROPVIEWPRIVATE_H
#define MEDIACROPVIEWPRIVATE_H

#include <QtWidgets/QWidget>
#include "mediaplayerviewprivate.h"

enum AspectRatio
{
	AspectRatio_Manually = 0,
	AspectRatio_16_9,
	AspectRatio_4_3
};

class CropViewToolBar;
class NLETimeline;
class DestDisplayView;
class CropMediaPlayerControl;
class CropEditVView;
class MediaCropViewPrivate : public MediaPlayerViewPrivate
{
	Q_OBJECT

public:
	MediaCropViewPrivate(NLETimeline *pDestTimeLine, QWidget *parent = 0);
	~MediaCropViewPrivate();

	CropMediaPlayerControl *m_pControl;
protected:
	void resizeEvent(QResizeEvent *event) override;
	void showEvent(QShowEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

	void SetRatio(float ratio) override;
	void ButtonClicked(int id);

Q_SIGNALS:
	void signalOK();
	void signalCancel();

protected:
	DestDisplayView *m_destView;
	CropEditVView *m_EditVView;
	CropViewToolBar *m_toolbar;
	
	NLETimeline *m_pDestTimeline;
};

class MagicRect : public QRect
{
public:
	MagicRect();
	~MagicRect();
	inline QRect &operator=(const QRect &rc);

	void SetLeftWithLimit(int nLeft);
	void SetTopWithLimit(int nTop);
	void SetRightWithLimit(int nRight);
	void SetBottomWithLimit(int nBottom);


	void SetRatio(AspectRatio ratio);
	void SetFrameRect(const QRect &rc);

private:
	QRect m_rcFrameRect;
	AspectRatio m_Ratio;
};

class DestDisplayView : public DisplayView
{
	Q_OBJECT
public:
	DestDisplayView(IMediaPlayerControl *control, QWidget *parent);
protected:
	void SetPlayerViewHwnd(const RECT &r) override;
};

class CropLineView;
class CropBlockView;
class CropEditVView : public QObject
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

	CropEditVView(QObject *parent);
	void SetFrameRect(const QRect &rect);
	void SetAspectRatio(AspectRatio ratio);
	QRect GetAdjustRect();
	bool PtInBlock(const QPoint &pt);
	void MousePress(int nId, const QPoint &p);
	void MouseRelease(const QPoint &p);
	void MouseMove(const QPoint &p);
	void Reset();
	void SetAdjustRect(BlockID nId, const QRect &rect);
protected:
	void Init();
	void UpdateUi();
	void SetBlockViewCursor(BlockID nId,CropBlockView *pView);

	void ManuallyRestrict(BlockID nId, const QRect &rect);
	void SpecialRestrict(BlockID nId, const QRect &rect);
	void UpdateDestCropRect();
Q_SIGNALS:
	void signalCropRectUpdate(const QRectF &rc);
private:
	QWidget *m_parentWidget;
	QRect m_rcFrame;
	QRect m_rcAdjust;
	QList<CropLineView*> m_lineViews;
	QList<CropBlockView*> m_blockViews;

	bool m_bPressed;
	QPoint m_pPressPt;
	QPoint m_ptPressCenter;
	QPoint m_pReleasePt;
	BlockID m_pPressId;

	QPoint m_pPrev;
	AspectRatio m_AspectRatio;
	
};


class CropLineView : public QWidget
{
	Q_OBJECT
public:
	CropLineView(Qt::Orientation ori,QWidget *parent);

protected:
	void paintEvent(QPaintEvent *event) override;
private:
	Qt::Orientation m_Orientation;
};

class CropBlockView : public QWidget
{
	Q_OBJECT
public:
	CropBlockView(QWidget *parent);

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



#endif // MEDIACROPVIEW_H
