#ifndef FEDITRECTANGLE_H
#define FEDITRECTANGLE_H

#include "inc_FCommonLib/FBaseWidget.h"

/*
edit: 移动、缩放、旋转
*/

//class FEditRectangle : public FBaseWidget
//{
//	Q_OBJECT
//
//public:
//	FEditRectangle(QWidget *parent = 0);
//	~FEditRectangle();
//
//	void SetTimeline(QList<FNLETimeline*>);
//	// 设置矩形区域可见性
//	void SetRectangleVisible(bool);
//	void Update();
//	// 获取当前的矩形
//	QRectF GetCurrentRect();  // 外接矩形
//	float GetRotateAngle();
//	void SetRenderRect(const QRectF&);
//
//signals:
//	void sigRectChanged(const QRectF&);   // 拖动矩形框放手信号
//
//protected:
//	void paintEvent(QPaintEvent *) override;
//	void resizeEvent(QResizeEvent *) override;
//	void mousePressEvent(QMouseEvent *) override;
//	void mouseReleaseEvent(QMouseEvent *) override;
//	void mouseMoveEvent(QMouseEvent *) override;
//
//private:
//	void Init();
//	int WhichNamedPointEncounters(const QPointF&, qreal r = 5);  // 返回被鼠标按下的有名点索引（0-7），否则，返回-1
//	QCursor GetCursorShape(const QPointF&);                      // 根据传入点坐标，确定鼠标指针样式
//	// 处于拖动状态
//	void OnDraging();
//	// 处于缩放状态
//	void OnScaling(const QPointF&);
//	// 处于旋转状态
//	void OnRotating(const QPointF&);
//	void OnRotatingAdjust(qreal);
//	bool IsRotatable(const QPointF&, qreal r = 20);
//	void AdjustRectangle();
//	qreal GetRotateAngle(const QPointF&);        // 计算旋转角度，返回角度值
//	void PointLeftTopBeScaled(const QPointF&);   // PointLeftTop被缩放
//	void PointRightTopBeScaled(const QPointF&);  // PointRightTop被缩放
//	void PointRightDownBeScaled(const QPointF&); // PointRightDown被缩放
//	void PointLeftDownBeScaled(const QPointF&);  // PointLeftDown被缩放
//	void PointTopBeScaled(const QPointF&);       // PointTop被缩放
//	void PointRightBeScaled(const QPointF&);     // PointTop被缩放
//	void PointDownBeScaled(const QPointF&);      // PointTop被缩放
//	void PointLeftBeScaled(const QPointF&);      // PointTop被缩放
//
//private:
//	FEditRectangleMgr *m_pMgr;
//	bool m_bVisible;           // 矩形框是否可见
//	bool m_bPressed;           // 鼠标是否击中矩形框区域
//	bool m_bDraged;            // 鼠标在拖动矩形框（前提：m_bPressed为true）
//	bool m_bRotated;           // 旋转状态
//	int m_ScaledIndex;         // 当前缩放点索引
//	qreal m_RotateAngle;       // 矩形框旋转角度
//	QPointF m_CenterPoint;     // 矩形框中心点
//	QPointF m_PressPoint;      // 鼠标按下的点
//	QPointF m_DragPoint;       // 鼠标拖动的点
//	QPointF m_PointLeftTop;    // 索引：0
//	QPointF m_PointTop;        // 索引：1
//	QPointF m_PointRightTop;   // 索引：2
//	QPointF m_PointRight;      // 索引：3
//	QPointF m_PointRightDown;  // 索引：4
//	QPointF m_PointDown;       // 索引：5
//	QPointF m_PointLeftDown;   // 索引：6
//	QPointF m_PointLeft;       // 索引：7
//	QPainterPath m_Rect;
//	QPixmap m_CursorMap;
//	QRectF m_RenderRect;       // 播放器的渲染区域
//};

class FNLETimeline;

class FKeyPoint : public QObject
{
	Q_OBJECT

public:
	enum KeyPoint{
		LeftTop_Pt = 0,
		Top_Pt,
		RightTop_Pt,
		Right_Pt,
		RightDown_Pt,
		Down_Pt,
		LeftDown_Pt,
		Left_Pt,
		Center_Pt
	};

public:
	FKeyPoint(QObject *parent = 0);
	~FKeyPoint();
	void SetPoint(KeyPoint, const QPointF&);
	QPointF GetPoint(KeyPoint);

private:
	QVector<QPointF> m_KeyPoints;
};

class FEditRectangleBase : public QObject
{
	Q_OBJECT

public:
	FEditRectangleBase(FNLETimeline* pTimeline, QRectF playerRegion, QObject* parent = 0);
	~FEditRectangleBase();

	virtual void DoMousePressEvent(QMouseEvent*);
	virtual void DoMouseMoveEvent(QMouseEvent*);
	virtual void DoPaintEvent(QPainter*, QPaintEvent*);
	// 默认情况下，把矩形的中心点设置为旋转点。子类可修改此规则。
	virtual void SetRotatePoint();                           
	// 设置矩形区域是否可见
	void SetRectVisible(bool bVisible);
	bool IsRectVisible();
	// 设置播放器区域
	void SetPlayerRegion(const QRectF&);
	// 获取PainterPath
	QPainterPath GetPainterPath();

signals:
	void sigUpdate();
	void sigCursorShapeChanged(const QCursor&);

private:
	void InitKeyPoints();
	void AdjustKeyPoints();
	int WhichNamedPointEncounters(const QPointF&, qreal r = 5);  // 返回被鼠标按下的有名点索引（0-7），否则，返回-1
	bool IsRotatable(const QPointF&, qreal r = 20);
	QCursor GetCursorShape(const QPointF&);                      // 根据传入点坐标，确定鼠标指针样式
	// 处于拖动状态
	void OnDraging();
	// 处于缩放状态
	void OnScaling(const QPointF&);
	void CornerPointBeScaled(FKeyPoint::KeyPoint, const QPointF&);
	void EdgePointBeScaled(FKeyPoint::KeyPoint, const QPointF&);
	// 处于旋转状态
	void OnRotating(const QPointF&);
	void OnRotatingAdjust(qreal);
	void AdjustRectangle();

protected:
	bool m_bVisible;             // 矩形框是否可见
	bool m_bPressed;             // 鼠标是否击中矩形框区域
	bool m_bDraged;              // 鼠标在拖动矩形框（前提：m_bPressed为true）
	bool m_bRotated;             // 旋转状态
	int m_ScaledIndex;           // 当前缩放点索引
	qreal m_RotateAngle;         // 矩形框旋转角度
	QPointF m_PressPt;           // 鼠标按下的点
	QPointF m_DragPt;            // 鼠标拖动的点
	QPointF m_RotatePt;          // 旋转点
	FNLETimeline *m_pTimeline;
	FKeyPoint m_KeyPoint;
	QRectF m_PlayerRegion;
	QPainterPath m_PainterPath;
	QPixmap m_CursorMap;
};

class FEditPIP : public FEditRectangleBase
{
	Q_OBJECT

public:
	FEditPIP(FNLETimeline* pTimeline, QRectF playerRegion, QObject* parent = 0);
	~FEditPIP();

	//void DoMousePressEvent(QMouseEvent*) override;
};

#endif // FEDITRECTANGLE_H



