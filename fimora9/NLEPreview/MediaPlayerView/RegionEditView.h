#ifndef REGIONEDITVIEW_H
#define REGIONEDITVIEW_H

#include <QtWidgets/QWidget>
#include <QRect>
#include "mediacropviewprivate.h"


class CornerWidget : public QWidget
{
	Q_OBJECT

public:
	CornerWidget(int nIndex,QWidget *parent);
	~CornerWidget();
	
	int Index();
	void SetRotateAngel(float angle);
Q_SIGNALS:
	void sigMouseMove(CornerWidget *pCorner, const QPoint &pt);
	void sigMouseRelease(CornerWidget *pCorner, const QPoint &pt);

protected:
	void paintEvent(QPaintEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

	void SetCursor();				//根据旋转角度设置光标的形状

private:
	int m_nIndex;
	float m_RotateAngel;
};


class RegionEditWidget : public QWidget
{
	Q_OBJECT

public:
	RegionEditWidget(QWidget *parent);
	~RegionEditWidget();

	NLETimeline *GetEditTimeline();
	void SetEditTimeline(NLETimeline *pTimeline);
	void Init();
	void SetFrameSize(QSize size);
	void SetBkImg(QPixmap px);
	void SetOffset(int nOffset);
Q_SIGNALS:
	void sigBkChanged();

protected:
	virtual void paintEvent(QPaintEvent *e) override;
	virtual void mouseMoveEvent(QMouseEvent *e) override;
	virtual void mousePressEvent(QMouseEvent *e) override;
	virtual void mouseReleaseEvent(QMouseEvent *e) override;
	void slotCornerMove(CornerWidget* pCorner,QPoint pt);
	void slotCornerRelease(CornerWidget* pCorner, QPoint pt);


	void UpdateDataFromTimeline();
	void CalcRegion();
	void UpdateDisplay();
	void ReallocCornerWidgets();
	void ResizeAfferSetting();		//设置属性后，根据需要调整控件的大小
	
	/*
	点oldPt绕原点（0，0）逆时针旋转rotateRadians弧度角后，得到的点坐标
	*/
	QPointF RotatePoint(QPointF oldPt, float rotateRadians);
	QPointF MiddlePoint(QPointF pt1, QPointF pt2);
	int GetMostLeftPt();			//返回点的索引值
	int GetMostTopPt();
	int GetMostRightPt();
	int GetMostBottomPt();

protected:
	enum  MousePressPos
	{
		MousePressPos_None = 0,
		MousePressPos_EditRegion,
		MousePressPos_RotateRegion,
	};
	QPixmap m_bkimg;
	NLETimeline *m_pCurEditingTL;
	CornerWidget* m_CornerWidgets[8];
	QPointF m_CornerPts[8];		//8个拉伸点,0-1
	QPoint m_CornerPtsReal[8];	//8个拉伸点,真实像素点

	QRegion m_EditRegion;		//整个编辑区域
	QVector<QPoint> m_regionPts;	//编辑区域四个顶点
	QRegion m_RotateRegion;		//旋转控制区域

	int m_RotateAngel;			//旋转角度，顺时针方向递增 0-360
	QPointF m_Center;			//中心点		0-1
	QPoint m_CenterReal;		//中心点，像素值
	QSizeF m_Scale;				//缩放比例	0-1
	QSize m_FrameSize;			//播放器的尺寸
	QRect m_WholeRect;			//当前控件的大小

	QPoint m_PressPt;
	MousePressPos m_MousePressPos;

	int m_offset;
};


class QUndoStack;
class RegionEditViewPrivate;
class RegionEditView : public QWidget
{
	Q_OBJECT

public:
	RegionEditView(QWidget *parent);
	~RegionEditView();

	void SetUndoStack(QUndoStack *pUndoStack);
	NLETimeline *GetEditTimeline();
	void SetEditTimeline(NLETimeline *pTimeline);
	void SetBkImg(QPixmap px);
protected:
	virtual void resizeEvent(QResizeEvent *e) override;
	void Init();
private:
	friend class MainMediaPlayerViewPrivateEx;
	QUndoStack *m_pUndoStack;
	QImage m_bk;				 //视频背景

	QScrollArea *m_scroolArea;
	QLabel *m_bkLabel;
	RegionEditWidget *m_pRegionEditWidget;
};

class NLETimeline;
class MainMediaPlayerViewPrivateEx : public MainMediaPlayerViewPrivate
{
	Q_OBJECT
public:
	MainMediaPlayerViewPrivateEx(QWidget *parent = nullptr);
	void slotTimelineSelectChanged(NLETimeline *pSelected);
	void slotLoadType(bool bTimeline);
	void slotSeekToFrame(int nFrame);
	void slotPause();
	void slotPlay();

protected:
	void resizeEvent(QResizeEvent *e) override;

	void EnterEditModel(bool bEnter);

	RegionEditView *m_pRegionEditView;
	bool m_bTimelineLoaded;
};

#endif // REGIONEDITVIEW_H
