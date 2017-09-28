#include "stdafx.h"
#include "FEditRectangle.h"
#include <QPainter>
#include <QResizeEvent>
#include "inc_FProject/FNLETimeline.h"
//const qreal PI = 3.1415926535897932;

//FEditRectangle::FEditRectangle(QWidget *parent)
//	: FBaseWidget(parent)
//	, m_bVisible(false)
//	, m_bPressed(false)
//	, m_bDraged(false)
//	, m_bRotated(false)
//	, m_ScaledIndex(-1)
//	, m_RotateAngle(0)
//	, m_CenterPoint(0, 0)
//	, m_PressPoint(0, 0)
//	, m_DragPoint(0, 0)
//	, m_CursorMap(":/FPropertyPanel/Resources/Cursor.png")
//{
//	m_pMgr = new FEditRectangleMgr(this);
//	setMouseTracking(true);
//	setAttribute(Qt::WA_TranslucentBackground, true);
//	Init();
//}
//
//FEditRectangle::~FEditRectangle()
//{
//
//}
//
//void FEditRectangle::SetTimeline(QList<FNLETimeline*> timelines)
//{
//	m_pMgr->SetTimeline(timelines);
//}
//
//void FEditRectangle::SetRectangleVisible(bool bVisible)
//{
//	m_bVisible = bVisible;
//	update();
//}
//
//void FEditRectangle::Update()
//{
//	update();
//}
//
//QRectF FEditRectangle::GetCurrentRect()
//{
//	QRectF rc;
//	if (m_RotateAngle < 90){  // m_PointLeftTop位于最上方
//		QLineF line1(m_PointLeftTop, QPointF(m_PointLeftTop.x() - 1, m_PointLeftTop.y()));
//		QLineF line2(m_PointLeftDown, QPointF(m_PointLeftDown.x(), m_PointLeftDown.y() - 1));
//		QPointF p;
//		line1.intersect(line2, &p);
//		rc = QRectF(p, QSizeF(m_PointRightTop.x() - m_PointLeftDown.x(), m_PointRightDown.y() - m_PointLeftTop.y()));
//	}
//	else if (m_RotateAngle < 180){  // m_PointRightTop位于最上方
//		QLineF line1(m_PointRightTop, QPointF(m_PointRightTop.x() - 1, m_PointRightTop.y()));
//		QLineF line2(m_PointLeftTop, QPointF(m_PointLeftTop.x(), m_PointLeftTop.y() - 1));
//		QPointF p;
//		line1.intersect(line2, &p);
//		rc = QRectF(p, QSizeF(m_PointRightDown.x() - m_PointLeftTop.x(), m_PointLeftDown.y() - m_PointRightTop.y()));
//	}
//	else if (m_RotateAngle < 270){  // m_PointRightDown位于最上方
//		QLineF line1(m_PointRightDown, QPointF(m_PointRightDown.x() - 1, m_PointRightDown.y()));
//		QLineF line2(m_PointRightTop, QPointF(m_PointRightTop.x(), m_PointRightTop.y() - 1));
//		QPointF p;
//		line1.intersect(line2, &p);
//		rc = QRectF(p, QSizeF(m_PointLeftDown.x() - m_PointRightTop.x(), m_PointLeftTop.y() - m_PointRightDown.y()));
//	}
//	else{  // m_PointLeftDown位于最上方
//		QLineF line1(m_PointLeftDown, QPointF(m_PointLeftDown.x() - 1, m_PointLeftDown.y()));
//		QLineF line2(m_PointRightDown, QPointF(m_PointRightDown.x(), m_PointRightDown.y() - 1));
//		QPointF p;
//		line1.intersect(line2, &p);
//		rc = QRectF(p, QSizeF(m_PointLeftTop.x() - m_PointRightDown.x(), m_PointRightTop.y() - m_PointLeftDown.y()));
//	}
//	return rc;
//}
//
//void FEditRectangle::paintEvent(QPaintEvent *)
//{
//	if (!m_bVisible) return;
//	QPainter p(this);
//	p.setRenderHint(QPainter::Antialiasing, true);
//	//p.save();
//	// 绘制八个有名点
//	QPen pen;
//	pen.setColor(Qt::white);
//	pen.setBrush(Qt::white);
//	p.setPen(pen);
//	p.drawEllipse(m_PointLeftTop, 3, 3);
//	p.drawEllipse(m_PointTop, 3, 3);
//	p.drawEllipse(m_PointRightTop, 3, 3);
//	p.drawEllipse(m_PointRight, 3, 3);
//	p.drawEllipse(m_PointRightDown, 3, 3);
//	p.drawEllipse(m_PointDown, 3, 3);
//	p.drawEllipse(m_PointLeftDown, 3, 3);
//	p.drawEllipse(m_PointLeft, 3, 3);
//	// 绘制矩形
//	pen.setColor(Qt::yellow);
//	p.drawPath(m_Rect);
//	// 绘制中心白色圆圈
//	pen.setWidthF(2);
//	pen.setColor(Qt::white);
//	pen.setStyle(Qt::SolidLine);
//	p.drawEllipse(m_CenterPoint, 20, 20);
//	//p.restore();
//}
//
//void FEditRectangle::resizeEvent(QResizeEvent *e)
//{
//	QSizeF oldSize = e->oldSize();
//	//qDebug() << "oldSize=" << oldSize;
//	if (oldSize.width() > 0){
//		//QTransform transform;
//		//transform.scale(1.0*width() / oldSize.width(), 1.0*height() / oldSize.height());
//		//QPointF p = QTransform().scale(1.0*width() / oldSize.width(), 1.0*height() / oldSize.height()).map(m_CenterPoint);
//		
//		//QPointF p = transform.map(m_CenterPoint);
//		//transform = transform.translate(m_CenterPoint.x() - p.x(), m_CenterPoint.y() - p.y());
//		//transform.scale(1.0*width() / oldSize.width(), 1.0*height() / oldSize.height());
//
//		//m_PointLeftTop = transform.map(m_PointLeftTop);
//		//m_PointRightTop = transform.map(m_PointRightTop);
//		//m_PointRightDown = transform.map(m_PointRightDown);
//		//m_PointLeftDown = transform.map(m_PointLeftDown);
//		//AdjustRectangle();
//
//	}
//}
//
//void FEditRectangle::mousePressEvent(QMouseEvent *event)
//{
//	m_PressPoint = event->pos();
//	m_ScaledIndex = WhichNamedPointEncounters(m_PressPoint);
//	if ((m_ScaledIndex == -1) && m_Rect.contains(m_PressPoint)) {
//		if (IsRotatable(m_PressPoint)) m_bRotated = true;
//		m_bPressed = true;
//	}
//	else m_bPressed = false;
//}
//
//void FEditRectangle::mouseReleaseEvent(QMouseEvent *)
//{
//	if (m_bDraged || m_bRotated) emit sigRectChanged(GetCurrentRect());
//	m_bPressed = false;
//	m_bDraged = false;
//	m_bRotated = false;
//	m_ScaledIndex = -1;
//}
//
//void FEditRectangle::mouseMoveEvent(QMouseEvent *event)
//{
//	QPoint p = event->pos();
//	setCursor(GetCursorShape(p));
//	if (m_bPressed && !m_bRotated){
//		m_bDraged = true;
//		m_DragPoint = p;
//	}
//	if (m_ScaledIndex != -1) OnScaling(p);
//	if (m_bDraged) OnDraging();
//	if (m_bRotated) OnRotating(p);
//}
//
//void FEditRectangle::Init()
//{
//	// 初始状态下，矩形框位于渲染区域的中心区域
//	int w = qMin(m_RenderRect.width(), m_RenderRect.height());
//	m_PointLeftTop = QPointF(1.0*(width() - w) / 2, m_RenderRect.y());
//	m_PointRightTop = QPointF(1.0*(width() + w) / 2, m_RenderRect.y());
//	m_PointRightDown = QPointF(1.0*(width() + w) / 2, w);
//	m_PointLeftDown = QPointF(1.0*(width() - w) / 2, w);
//	AdjustRectangle();
//	update();
//}
//
//int FEditRectangle::WhichNamedPointEncounters(const QPointF& p, qreal r /*= 5*/)
//{
//	qreal diff = 0;
//	diff = qSqrt(qPow(p.x() - m_PointLeftTop.x(), 2) + qPow(p.y() - m_PointLeftTop.y(), 2));
//	if (diff < r) return 0;
//	diff = qSqrt(qPow(p.x() - m_PointTop.x(), 2) + qPow(p.y() - m_PointTop.y(), 2));
//	if (diff < r) return 1;
//	diff = qSqrt(qPow(p.x() - m_PointRightTop.x(), 2) + qPow(p.y() - m_PointRightTop.y(), 2));
//	if (diff < r) return 2;
//	diff = qSqrt(qPow(p.x() - m_PointRight.x(), 2) + qPow(p.y() - m_PointRight.y(), 2));
//	if (diff < r) return 3;
//	diff = qSqrt(qPow(p.x() - m_PointRightDown.x(), 2) + qPow(p.y() - m_PointRightDown.y(), 2));
//	if (diff < r) return 4;
//	diff = qSqrt(qPow(p.x() - m_PointDown.x(), 2) + qPow(p.y() - m_PointDown.y(), 2));
//	if (diff < r) return 5;
//	diff = qSqrt(qPow(p.x() - m_PointLeftDown.x(), 2) + qPow(p.y() - m_PointLeftDown.y(), 2));
//	if (diff < r) return 6;
//	diff = qSqrt(qPow(p.x() - m_PointLeft.x(), 2) + qPow(p.y() - m_PointLeft.y(), 2));
//	if (diff < r) return 7;
//	return -1;
//}
//
//#include <QDebug>
//QCursor FEditRectangle::GetCursorShape(const QPointF& p)
//{
//	int index = WhichNamedPointEncounters(p);
//	if (index == -1){
//		if (m_Rect.contains(p)) {
//			if (IsRotatable(p)) return QCursor(m_CursorMap);
//			return Qt::SizeAllCursor;
//		}
//		else return Qt::ArrowCursor;
//	}
//	int angle = m_RotateAngle;
//	angle = (angle + index * 45) % 360;
//	qDebug() << "index=" << index << " , angle=" << angle;
//	if (angle == 315) return Qt::SizeHorCursor;
//	else if (angle > 315 || angle < 45) return Qt::SizeFDiagCursor;
//	else if (angle == 45) return Qt::SizeVerCursor;
//	else if (angle > 45 && angle < 135) return Qt::SizeBDiagCursor;
//	else if (angle == 135) return Qt::SizeHorCursor;
//	else if (angle > 135 && angle < 225) return Qt::SizeFDiagCursor;
//	else if (angle == 225) return Qt::SizeVerCursor;
//	else if (angle > 225 && angle < 315) return Qt::SizeBDiagCursor;
//	return Qt::ArrowCursor;
//}
//
//void FEditRectangle::OnDraging()
//{
//	float diffx = m_DragPoint.x() - m_PressPoint.x();
//	float diffy = m_DragPoint.y() - m_PressPoint.y();
//	// 更新八大有名点
//	m_PointLeftTop = QPointF(diffx + m_PointLeftTop.x(), diffy + m_PointLeftTop.y());
//	m_PointTop = QPointF(diffx + m_PointTop.x(), diffy + m_PointTop.y());
//	m_PointRightTop = QPointF(diffx + m_PointRightTop.x(), diffy + m_PointRightTop.y());
//	m_PointRight = QPointF(diffx + m_PointRight.x(), diffy + m_PointRight.y());
//	m_PointRightDown = QPointF(diffx + m_PointRightDown.x(), diffy + m_PointRightDown.y());
//	m_PointDown = QPointF(diffx + m_PointDown.x(), diffy + m_PointDown.y());
//	m_PointLeftDown = QPointF(diffx + m_PointLeftDown.x(), diffy + m_PointLeftDown.y());
//	m_PointLeft = QPointF(diffx + m_PointLeft.x(), diffy + m_PointLeft.y());
//	// 更新中心点
//	m_CenterPoint = QPointF((m_PointLeft.x() + m_PointRight.x()) / 2, (m_PointLeft.y() + m_PointRight.y()) / 2);
//	// 更新矩形框
//	QPainterPath path(m_PointLeftTop);
//	path.lineTo(m_PointRightTop);
//	path.lineTo(m_PointRightDown);
//	path.lineTo(m_PointLeftDown);
//	path.lineTo(m_PointLeftTop);
//	m_Rect = path;
//	// 更新presspoint
//	m_PressPoint = m_DragPoint;
//	update();
//}
//
//void FEditRectangle::OnScaling(const QPointF& p)
//{
//	// 确定两个点：左上角、右下角
//	qreal s = 0;
//	if (m_ScaledIndex == 0){
//		PointLeftTopBeScaled(p);
//	}
//	else if (m_ScaledIndex == 1) {
//		PointTopBeScaled(p);
//	}
//	else if (m_ScaledIndex == 2) {
//		PointRightTopBeScaled(p);
//	}
//	else if (m_ScaledIndex == 3) {
//		PointRightBeScaled(p);
//	}
//	else if (m_ScaledIndex == 4) {
//		PointRightDownBeScaled(p);
//	}
//	else if (m_ScaledIndex == 5) {
//		PointDownBeScaled(p);
//	}
//	else if (m_ScaledIndex == 6) {
//		PointLeftDownBeScaled(p);
//	}
//	else if (m_ScaledIndex == 7) {
//		PointLeftBeScaled(p);
//	}
//	AdjustRectangle();
//	update();
//}
//
//void FEditRectangle::OnRotating(const QPointF& p)
//{
//	// 利用余弦定理计算旋转角度
//	QLineF line1(m_CenterPoint, p);
//	QLineF line2(m_CenterPoint, m_PressPoint);
//	qreal angle = line1.angleTo(line2);
//	bool bClockWise = (angle < 90);  // 顺时针
//	if (!bClockWise){  // 逆时针
//		angle = 360 - angle;
//		angle = -angle;
//	}
//	m_RotateAngle += angle;
//	if (m_RotateAngle >= 360) m_RotateAngle -= 360;
//	if (m_RotateAngle < 0) m_RotateAngle += 360;
//	m_PressPoint = p;
//	//qDebug() << "angle=" << angle;
//	//qDebug() << "m_RotateAngle= " << m_RotateAngle;
//	OnRotatingAdjust(angle);
//	update();
//}
//
//void FEditRectangle::OnRotatingAdjust(qreal angle)
//{
//	QPointF center = m_CenterPoint;
//	QPointF center1 = QTransform().rotate(angle).map(m_CenterPoint);
//	center1 = center - center1;
//	QTransform transform;// = QTransform().rotate(m_RotateAngle)*QTransform().translate(center1.x(),center1.y());
//	transform.translate(center1.x(), center1.y());
//	transform.rotate(angle);
//	m_PointLeftTop = transform.map(m_PointLeftTop);
//	m_PointTop = transform.map(m_PointTop);
//	m_PointRightTop = transform.map(m_PointRightTop);
//	m_PointRight = transform.map(m_PointRight);
//	m_PointRightDown = transform.map(m_PointRightDown);
//	m_PointDown = transform.map(m_PointDown);
//	m_PointLeftDown = transform.map(m_PointLeftDown);
//	m_PointLeft = transform.map(m_PointLeft);
//	//qDebug() << "m_PointLeftTop= " << m_PointLeftTop;
//	//qDebug() << "m_PointTop= " << m_PointTop;
//	QPainterPath path(m_PointLeftTop);
//	path.lineTo(m_PointRightTop);
//	path.lineTo(m_PointRightDown);
//	path.lineTo(m_PointLeftDown);
//	path.lineTo(m_PointLeftTop);
//	m_Rect = path;
//}
//
//bool FEditRectangle::IsRotatable(const QPointF& p, qreal r /*= 20*/)
//{
//	qreal R = qSqrt(qPow(p.x() - m_CenterPoint.x(), 2) + qPow(p.y() - m_CenterPoint.y(), 2));
//	qreal diff = qAbs(R - r);
//	return diff < 4;
//}
//
//void FEditRectangle::AdjustRectangle()
//{
//	// 前提假设：四个角点是确定的。以此两点推测其它点坐标
//	m_PointTop.setX((m_PointLeftTop.x() + m_PointRightTop.x()) / 2);
//	m_PointTop.setY((m_PointLeftTop.y() + m_PointRightTop.y()) / 2);
//	m_PointRight.setX((m_PointRightTop.x() + m_PointRightDown.x()) / 2);
//	m_PointRight.setY((m_PointRightTop.y() + m_PointRightDown.y()) / 2);
//	m_PointDown.setX((m_PointLeftDown.x() + m_PointRightDown.x()) / 2);
//	m_PointDown.setY((m_PointLeftDown.y() + m_PointRightDown.y()) / 2);
//	m_PointLeft.setX((m_PointLeftTop.x() + m_PointLeftDown.x()) / 2);
//	m_PointLeft.setY((m_PointLeftTop.y() + m_PointLeftDown.y()) / 2);
//	m_CenterPoint.setX((m_PointLeft.x() + m_PointRight.x()) / 2);
//	m_CenterPoint.setY((m_PointLeft.y() + m_PointRight.y()) / 2);
//	QPainterPath path(m_PointLeftTop);
//	path.lineTo(m_PointRightTop);
//	path.lineTo(m_PointRightDown);
//	path.lineTo(m_PointLeftDown);
//	path.lineTo(m_PointLeftTop);
//	m_Rect = path;
//}
//
//qreal FEditRectangle::GetRotateAngle(const QPointF& p)
//{
//	// 利用余弦定理计算旋转角度
//	qreal dis_center_press = qSqrt(qPow(m_CenterPoint.x() - m_PressPoint.x(), 2) + qPow(m_CenterPoint.y() - m_PressPoint.y(), 2));
//	qreal dis_center_p = qSqrt(qPow(m_CenterPoint.x() - p.x(), 2) + qPow(m_CenterPoint.y() - p.y(), 2));
//	qreal dis_press_p = qSqrt(qPow(m_PressPoint.x() - p.x(), 2) + qPow(m_PressPoint.y() - p.y(), 2));
//	return qRadiansToDegrees(qAcos((qPow(dis_center_press, 2) + qPow(dis_center_p, 2) - qPow(dis_press_p, 2)) / (2 * dis_center_press * dis_center_p)));
//}
//
//float FEditRectangle::GetRotateAngle()
//{
//	return m_RotateAngle;
//}
//
//void FEditRectangle::SetRenderRect(const QRectF& rc)
//{
//	m_RenderRect = rc;
//}
//
//void FEditRectangle::PointLeftTopBeScaled(const QPointF& p)
//{
//	/*需要确定四个顶点（角上的点）*/
//	QLineF line1(m_PointRightDown, m_PointLeftTop);
//	QLineF line2(m_PointRightDown, m_PointLeftDown);
//	QLineF line3(m_PointRightDown, m_PointRightTop);
//	QLineF line(m_PointRightDown, p);
//	qreal dis1 = qSqrt(qPow(m_PointLeftTop.x() - m_PointRightDown.x(), 2) + qPow(m_PointLeftTop.y() - m_PointRightDown.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointRightDown.x(), 2) + qPow(p.y() - m_PointRightDown.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointLeftTop.x() - p.x(), 2) + qPow(m_PointLeftTop.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	r1 = qMax(60.0, r1);
//
//		// 确定m_PointLeftTop
//		line1.setLength(r1);
//		m_PointLeftTop = line1.p2();
//		qreal b = line1.angleTo(line2);
//		if (b > 90)b = 180 - b;
//		qreal r2 = r1*qCos(qDegreesToRadians(b));
//		qreal r3 = r1*qCos(qDegreesToRadians(90 - b));
//		// 确定m_PointLeftDown
//		line2.setLength(r2);
//		m_PointLeftDown = line2.p2();
//		// 确定m_PointRightTop
//		line3.setLength(r3);
//		m_PointRightTop = line3.p2();
//
//}
//
//void FEditRectangle::PointRightTopBeScaled(const QPointF& p)
//{
//	/*需要确定四个顶点（角上的点）*/
//	QLineF line1(m_PointLeftDown, m_PointRightTop);
//	QLineF line2(m_PointLeftDown, m_PointLeftTop);
//	QLineF line3(m_PointLeftDown, m_PointRightDown);
//	qreal dis1 = qSqrt(qPow(m_PointLeftDown.x() - m_PointRightTop.x(), 2) + qPow(m_PointLeftDown.y() - m_PointRightTop.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointLeftDown.x(), 2) + qPow(p.y() - m_PointLeftDown.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointRightTop.x() - p.x(), 2) + qPow(m_PointRightTop.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	if (r1 > 60){
//		// 确定m_PointRightTop
//		line1.setLength(r1);
//		m_PointRightTop = line1.p2();
//		qreal b = line1.angleTo(line2);
//		if (b > 90)b = 180 - b;
//		qreal r2 = r1*qCos(qDegreesToRadians(b));
//		qreal r3 = r1*qCos(qDegreesToRadians(90 - b));
//		// 确定m_PointLeftTop
//		line2.setLength(r2);
//		m_PointLeftTop = line2.p2();
//		// 确定m_PointRightDown
//		line3.setLength(r3);
//		m_PointRightDown = line3.p2();
//	}
//}
//
//void FEditRectangle::PointRightDownBeScaled(const QPointF& p)
//{
//	/*需要确定四个顶点（角上的点）*/
//	QLineF line1(m_PointLeftTop, m_PointRightDown);
//	QLineF line2(m_PointLeftTop, m_PointRightTop);
//	QLineF line3(m_PointLeftTop, m_PointLeftDown);
//	qreal dis1 = qSqrt(qPow(m_PointLeftTop.x() - m_PointRightDown.x(), 2) + qPow(m_PointLeftTop.y() - m_PointRightDown.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointLeftTop.x(), 2) + qPow(p.y() - m_PointLeftTop.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointRightDown.x() - p.x(), 2) + qPow(m_PointRightDown.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	if (r1*r1 > 4000){
//		// 确定m_PointRightDown
//		line1.setLength(r1);
//		m_PointRightDown = line1.p2();
//		qreal b = line1.angleTo(line2);
//		if (b > 90)b = 180 - b;
//		qreal r2 = r1*qCos(qDegreesToRadians(b));
//		qreal r3 = r1*qCos(qDegreesToRadians(90 - b));
//		// 确定m_PointRightTop
//		line2.setLength(r2);
//		m_PointRightTop = line2.p2();
//		// 确定m_PointLeftDown
//		line3.setLength(r3);
//		m_PointLeftDown = line3.p2();
//	}
//}
//
//void FEditRectangle::PointLeftDownBeScaled(const QPointF& p)
//{
//	/*需要确定四个顶点（角上的点）*/
//	QLineF line1(m_PointRightTop, m_PointLeftDown);
//	QLineF line2(m_PointRightTop, m_PointRightDown);
//	QLineF line3(m_PointRightTop, m_PointLeftTop);
//	qreal dis1 = qSqrt(qPow(m_PointRightTop.x() - m_PointLeftDown.x(), 2) + qPow(m_PointRightTop.y() - m_PointLeftDown.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointRightTop.x(), 2) + qPow(p.y() - m_PointRightTop.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointLeftDown.x() - p.x(), 2) + qPow(m_PointLeftDown.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	if (r1*r1 > 4000){
//		// 确定m_PointLeftDown
//		line1.setLength(r1);
//		m_PointLeftDown = line1.p2();
//		qreal b = line1.angleTo(line2);
//		if (b > 90)b = 180 - b;
//		qreal r2 = r1*qCos(qDegreesToRadians(b));
//		qreal r3 = r1*qCos(qDegreesToRadians(90 - b));
//		// 确定m_PointRightDown
//		line2.setLength(r2);
//		m_PointRightDown = line2.p2();
//		// 确定m_PointLeftTop
//		line3.setLength(r3);
//		m_PointLeftTop = line3.p2();
//	}
//}
//
//void FEditRectangle::PointTopBeScaled(const QPointF& p)
//{
//	QLineF line1(m_PointDown, m_PointTop);
//	QLineF line2(m_PointLeftDown, m_PointLeftTop);
//	QLineF line3(m_PointRightDown, m_PointRightTop);
//	qreal dis1 = qSqrt(qPow(m_PointDown.x() - m_PointTop.x(), 2) + qPow(m_PointDown.y() - m_PointTop.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointDown.x(), 2) + qPow(p.y() - m_PointDown.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointTop.x() - p.x(), 2) + qPow(m_PointTop.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	if (r1*r1 > 4000){
//		// 确定m_PointLeftTop
//		line2.setLength(r1);
//		m_PointLeftTop = line2.p2();
//		// 确定m_PointRightTop
//		line3.setLength(r1);
//		m_PointRightTop = line3.p2();
//	}
//}
//
//void FEditRectangle::PointRightBeScaled(const QPointF& p)
//{
//	QLineF line1(m_PointLeft, m_PointRight);
//	QLineF line2(m_PointLeftTop, m_PointRightTop);
//	QLineF line3(m_PointLeftDown, m_PointRightDown);
//	qreal dis1 = qSqrt(qPow(m_PointLeft.x() - m_PointRight.x(), 2) + qPow(m_PointLeft.y() - m_PointRight.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointLeft.x(), 2) + qPow(p.y() - m_PointLeft.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointRight.x() - p.x(), 2) + qPow(m_PointRight.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	if (r1*r1 > 4000){
//		// 确定m_PointRightTop
//		line2.setLength(r1);
//		m_PointRightTop = line2.p2();
//		// 确定m_PointRightDown
//		line3.setLength(r1);
//		m_PointRightDown = line3.p2();
//	}
//}
//
//void FEditRectangle::PointDownBeScaled(const QPointF& p)
//{
//	QLineF line1(m_PointTop, m_PointDown);
//	QLineF line2(m_PointRightTop, m_PointRightDown);
//	QLineF line3(m_PointLeftTop, m_PointLeftDown);
//	qreal dis1 = qSqrt(qPow(m_PointTop.x() - m_PointDown.x(), 2) + qPow(m_PointTop.y() - m_PointDown.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointTop.x(), 2) + qPow(p.y() - m_PointTop.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointDown.x() - p.x(), 2) + qPow(m_PointDown.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	if (r1*r1 > 4000){
//		// 确定m_PointRightDown
//		line2.setLength(r1);
//		m_PointRightDown = line2.p2();
//		// 确定m_PointLeftDown
//		line3.setLength(r1);
//		m_PointLeftDown = line3.p2();
//	}
//}
//
//void FEditRectangle::PointLeftBeScaled(const QPointF& p)
//{
//	QLineF line1(m_PointRight, m_PointLeft);
//	QLineF line2(m_PointRightDown, m_PointLeftDown);
//	QLineF line3(m_PointRightTop, m_PointLeftTop);
//	qreal dis1 = qSqrt(qPow(m_PointRight.x() - m_PointLeft.x(), 2) + qPow(m_PointRight.y() - m_PointLeft.y(), 2));
//	qreal dis2 = qSqrt(qPow(p.x() - m_PointRight.x(), 2) + qPow(p.y() - m_PointRight.y(), 2));
//	qreal dis3 = qSqrt(qPow(m_PointLeft.x() - p.x(), 2) + qPow(m_PointLeft.y() - p.y(), 2));
//	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
//	qreal r1 = dis2*a;
//	if (r1*r1 > 4000){
//		// 确定m_PointLeftDown
//		line2.setLength(r1);
//		m_PointLeftDown = line2.p2();
//		// 确定m_PointLeftTop
//		line3.setLength(r1);
//		m_PointLeftTop = line3.p2();
//	}
//}


/*FKeyPoint*/
FKeyPoint::FKeyPoint(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_KeyPoints.resize(9);
}

FKeyPoint::~FKeyPoint()
{

}

void FKeyPoint::SetPoint(KeyPoint kp, const QPointF& p)
{
	m_KeyPoints[kp] = p;
}

QPointF FKeyPoint::GetPoint(KeyPoint kp)
{
	return m_KeyPoints[kp];
}


/*FEditRectangleBase*/
FEditRectangleBase::FEditRectangleBase(FNLETimeline* pTimeline, QRectF playerRegion, QObject* parent /*= 0*/)
	: QObject(parent)
	, m_PlayerRegion(playerRegion)
	, m_bVisible(false)
	, m_pTimeline(pTimeline)
	, m_ScaledIndex(-1)
	, m_RotateAngle(0)
	, m_CursorMap(":/FPropertyPanel/Resources/Cursor.png")
{
	InitKeyPoints();
}

FEditRectangleBase::~FEditRectangleBase()
{

}

void FEditRectangleBase::DoMousePressEvent(QMouseEvent* e)
{
	m_PressPt = e->pos();
	m_ScaledIndex = WhichNamedPointEncounters(m_PressPt);
	if ((m_ScaledIndex == -1) && m_PainterPath.contains(m_PressPt)){
		if (IsRotatable(m_PressPt)) m_bRotated = true;
		m_bPressed = true;
		m_bVisible = true;
	}
	else{
		m_bVisible = false;
		m_bPressed = false;
	}
}

void FEditRectangleBase::DoMouseMoveEvent(QMouseEvent* e)
{
	QPoint p = e->pos();
	emit sigCursorShapeChanged(GetCursorShape(p));
	if (m_bPressed && !m_bRotated){
		m_bDraged = true;
		m_DragPt = p;
	}
	if (m_ScaledIndex != -1) OnScaling(p);
	if (m_bDraged) OnDraging();
	if (m_bRotated) OnRotating(p);
}

void FEditRectangleBase::DoPaintEvent(QPainter* painter, QPaintEvent* e)
{
	if (m_bVisible){
		painter->drawPath(m_PainterPath);
	}
}

// 默认情况下，把矩形的中心点设置为旋转点
void FEditRectangleBase::SetRotatePoint()
{
	m_RotatePt.setX((m_KeyPoint.GetPoint(FKeyPoint::Left_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::Right_Pt).x()) / 2);
	m_RotatePt.setY((m_KeyPoint.GetPoint(FKeyPoint::Left_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::Right_Pt).y()) / 2);
}

void FEditRectangleBase::SetRectVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool FEditRectangleBase::IsRectVisible()
{
	return m_bVisible;
}

void FEditRectangleBase::SetPlayerRegion(const QRectF& rc)
{
	m_PlayerRegion = rc;
	AdjustKeyPoints();
}

QPainterPath FEditRectangleBase::GetPainterPath()
{
	return m_PainterPath;
}

void FEditRectangleBase::InitKeyPoints()
{
	// 初始状态下，矩形框位于渲染区域的中心区域
	//int w = qMin(m_PlayerRegion.width(), m_PlayerRegion.height());
	//m_KeyPoint.SetPoint(FKeyPoint::LeftTop_Pt, QPointF(1.0*(m_PlayerRegion.width() - w) / 2, m_PlayerRegion.y()));
	//m_KeyPoint.SetPoint(FKeyPoint::RightTop_Pt, QPointF(1.0*(m_PlayerRegion.width() + w) / 2, m_PlayerRegion.y()));
	//m_KeyPoint.SetPoint(FKeyPoint::RightDown_Pt, QPointF(1.0*(m_PlayerRegion.width() + w) / 2, w));
	//m_KeyPoint.SetPoint(FKeyPoint::LeftDown_Pt, QPointF(1.0*(m_PlayerRegion.width() - w) / 2, w));
	QRectF rc = m_pTimeline->GetRealRenderRegion(m_PlayerRegion);
	m_KeyPoint.SetPoint(FKeyPoint::LeftTop_Pt, QPointF(rc.x(), rc.y()));
	m_KeyPoint.SetPoint(FKeyPoint::RightTop_Pt, QPointF(rc.x() + rc.width(), rc.y()));
	m_KeyPoint.SetPoint(FKeyPoint::RightDown_Pt, QPointF(rc.x() + rc.width(), rc.y() + rc.height()));
	m_KeyPoint.SetPoint(FKeyPoint::LeftDown_Pt, QPointF(rc.x(), rc.y() + rc.height()));

	qreal x, y;
	x = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Top_Pt, QPointF(x, y));
	x = (m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Right_Pt, QPointF(x, y));
	x = (m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Down_Pt, QPointF(x, y));
	x = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Left_Pt, QPointF(x, y));
	// 初始化RotatePt
	SetRotatePoint();
	// 初始化PainterPath
	QPainterPath path(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	m_PainterPath = path;
}

void FEditRectangleBase::AdjustKeyPoints()
{
	// 调整原则：比例不变
	qreal scalex = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).x() - m_PlayerRegion.x()) / m_PlayerRegion.width();
	qreal scaley = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).y() - m_PlayerRegion.y()) / m_PlayerRegion.height();
	qreal scalew = QLineF(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt), m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt)).length() / m_PlayerRegion.width();
	qreal scaleh = QLineF(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt), m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt)).length() / m_PlayerRegion.height();
	QLineF line1(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt), m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt));
	QLineF line2(m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt), m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt));
	QLineF line3(m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt), m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt));
	m_KeyPoint.SetPoint(FKeyPoint::LeftTop_Pt, QPointF(m_PlayerRegion.x() + scalex*m_PlayerRegion.width(), m_PlayerRegion.y() + scaley*m_PlayerRegion.height()));
	line1.translate(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).x() - line1.x1(), m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).y() - line1.y1());
	line1.setLength(m_PlayerRegion.width());
	m_KeyPoint.SetPoint(FKeyPoint::RightTop_Pt, line1.p2());
	line2.translate(m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).x() - line2.x1(), m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).y() - line2.y1());
	line2.setLength(m_PlayerRegion.height());
	m_KeyPoint.SetPoint(FKeyPoint::RightDown_Pt, line2.p2());
	line3.translate(m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).x() - line3.x1(), m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).y() - line3.y1());
	line3.setLength(m_PlayerRegion.width());
	m_KeyPoint.SetPoint(FKeyPoint::LeftDown_Pt, line3.p2());
	qreal x, y;
	x = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Top_Pt, QPointF(x, y));
	x = (m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Right_Pt, QPointF(x, y));
	x = (m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Down_Pt, QPointF(x, y));
	x = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).x() + m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).x()) / 2;
	y = (m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).y() + m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).y()) / 2;
	m_KeyPoint.SetPoint(FKeyPoint::Left_Pt, QPointF(x, y));
	// 调整RotatPt
	SetRotatePoint();
	// 调整PainterPath
	QPainterPath path(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	m_PainterPath = path;
}

int FEditRectangleBase::WhichNamedPointEncounters(const QPointF& p, qreal r /*= 5*/)
{
	qreal diff = 0;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt).y(), 2));
	if (diff < r) return 0;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::Top_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::Top_Pt).y(), 2));
	if (diff < r) return 1;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt).y(), 2));
	if (diff < r) return 2;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::Right_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::Right_Pt).y(), 2));
	if (diff < r) return 3;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt).y(), 2));
	if (diff < r) return 4;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::Down_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::Down_Pt).y(), 2));
	if (diff < r) return 5;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt).y(), 2));
	if (diff < r) return 6;
	diff = qSqrt(qPow(p.x() - m_KeyPoint.GetPoint(FKeyPoint::Left_Pt).x(), 2) + qPow(p.y() - m_KeyPoint.GetPoint(FKeyPoint::Left_Pt).y(), 2));
	if (diff < r) return 7;
	return -1;
}

bool FEditRectangleBase::IsRotatable(const QPointF& p, qreal r /*= 20*/)
{
	qreal R = qSqrt(qPow(p.x() - m_RotatePt.x(), 2) + qPow(p.y() - m_RotatePt.y(), 2));
	qreal diff = qAbs(R - r);
	return diff < 4;
}

QCursor FEditRectangleBase::GetCursorShape(const QPointF& p)
{
	int index = WhichNamedPointEncounters(p);
	if (index == -1){
		if (m_PainterPath.contains(p)) {
			if (IsRotatable(p)) return QCursor(m_CursorMap);
			return Qt::SizeAllCursor;
		}
		else return Qt::ArrowCursor;
	}
	int angle = m_RotateAngle;
	angle = (angle + index * 45) % 360;
	qDebug() << "index=" << index << " , angle=" << angle;
	if (angle == 315) return Qt::SizeHorCursor;
	else if (angle > 315 || angle < 45) return Qt::SizeFDiagCursor;
	else if (angle == 45) return Qt::SizeVerCursor;
	else if (angle > 45 && angle < 135) return Qt::SizeBDiagCursor;
	else if (angle == 135) return Qt::SizeHorCursor;
	else if (angle > 135 && angle < 225) return Qt::SizeFDiagCursor;
	else if (angle == 225) return Qt::SizeVerCursor;
	else if (angle > 225 && angle < 315) return Qt::SizeBDiagCursor;
	return Qt::ArrowCursor;
}

void FEditRectangleBase::OnDraging()
{
	float diffx = m_DragPt.x() - m_PressPt.x();
	float diffy = m_DragPt.y() - m_PressPt.y();
	// 更新八大有名点
	QPointF pt = m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::LeftTop_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	pt = m_KeyPoint.GetPoint(FKeyPoint::Top_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::Top_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	pt = m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::RightTop_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	pt = m_KeyPoint.GetPoint(FKeyPoint::Right_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::Right_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	pt = m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::RightDown_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	pt = m_KeyPoint.GetPoint(FKeyPoint::Down_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::Down_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	pt = m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::LeftDown_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	pt = m_KeyPoint.GetPoint(FKeyPoint::Left_Pt);
	m_KeyPoint.SetPoint(FKeyPoint::Left_Pt, QPointF(diffx + pt.x(), diffy + pt.y()));
	// 更新RotatePt
	SetRotatePoint();
	// 更新矩形框
	QPainterPath path(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	m_PainterPath = path;
	// 更新presspoint
	m_PressPt = m_DragPt;
	emit sigUpdate();
}

void FEditRectangleBase::OnScaling(const QPointF& p)
{
	qreal s = 0;
	if (m_ScaledIndex == 0){
		//PointLeftTopBeScaled(p);
		CornerPointBeScaled(FKeyPoint::KeyPoint::LeftTop_Pt, p);
	}
	else if (m_ScaledIndex == 1) {
		//PointTopBeScaled(p);
		EdgePointBeScaled(FKeyPoint::KeyPoint::Top_Pt, p);
	}
	else if (m_ScaledIndex == 2) {
		CornerPointBeScaled(FKeyPoint::KeyPoint::RightTop_Pt, p);
		//PointRightTopBeScaled(p);
	}
	else if (m_ScaledIndex == 3) {
		EdgePointBeScaled(FKeyPoint::KeyPoint::Right_Pt, p);
		//PointRightBeScaled(p);
	}
	else if (m_ScaledIndex == 4) {
		CornerPointBeScaled(FKeyPoint::KeyPoint::RightDown_Pt, p);
		//PointRightDownBeScaled(p);
	}
	else if (m_ScaledIndex == 5) {
		EdgePointBeScaled(FKeyPoint::KeyPoint::Down_Pt, p);
		//PointDownBeScaled(p);
	}
	else if (m_ScaledIndex == 6) {
		CornerPointBeScaled(FKeyPoint::KeyPoint::LeftDown_Pt, p);
		//PointLeftDownBeScaled(p);
	}
	else if (m_ScaledIndex == 7) {
		EdgePointBeScaled(FKeyPoint::KeyPoint::Left_Pt, p);
		//PointLeftBeScaled(p);
	}
	AdjustRectangle();
	emit sigUpdate();
}

void FEditRectangleBase::CornerPointBeScaled(FKeyPoint::KeyPoint kp, const QPointF& pt)
{
	/*需要确定四个顶点（角上的点）*/
	//QLineF line1(m_PointRightDown, m_PointLeftTop);
	QLineF line1(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)), m_KeyPoint.GetPoint(kp));
	//QLineF line2(m_PointRightDown, m_PointLeftDown);
	QLineF line2(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)), m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 6) % 8)));
	//QLineF line3(m_PointRightDown, m_PointRightTop);
	QLineF line3(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)), m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 2) % 8)));
	//QLineF line(m_PointRightDown, p);
	QLineF line(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)), pt);
	//qreal dis1 = qSqrt(qPow(m_PointLeftTop.x() - m_PointRightDown.x(), 2) + qPow(m_PointLeftTop.y() - m_PointRightDown.y(), 2));
	qreal dis1 = qSqrt(qPow(m_KeyPoint.GetPoint(kp).x() - m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).x(), 2) + qPow(m_KeyPoint.GetPoint(kp).y() - m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).y(), 2));
	//qreal dis2 = qSqrt(qPow(p.x() - m_PointRightDown.x(), 2) + qPow(p.y() - m_PointRightDown.y(), 2));
	qreal dis2 = qSqrt(qPow(pt.x() - m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).x(), 2) + qPow(pt.y() - m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).y(), 2));
	//qreal dis3 = qSqrt(qPow(m_PointLeftTop.x() - p.x(), 2) + qPow(m_PointLeftTop.y() - p.y(), 2));
	qreal dis3 = qSqrt(qPow(m_KeyPoint.GetPoint(kp).x() - pt.x(), 2) + qPow(m_KeyPoint.GetPoint(kp).y() - pt.y(), 2));
	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
	qreal r1 = dis2*a;
	r1 = qMax(60.0, r1);
	// 确定m_PointLeftTop
	line1.setLength(r1);
	//m_PointLeftTop = line1.p2();
	m_KeyPoint.SetPoint(kp, line1.p2());
	qreal b = line1.angleTo(line2);
	if (b > 90) b = 180 - b;
	qreal r2 = r1*qCos(qDegreesToRadians(b));
	qreal r3 = r1*qCos(qDegreesToRadians(90 - b));
	// 确定m_PointLeftDown
	line2.setLength(r2);
	//m_PointLeftDown = line2.p2();
	m_KeyPoint.SetPoint(FKeyPoint::KeyPoint((kp + 6) % 8), line2.p2());
	// 确定m_PointRightTop
	line3.setLength(r3);
	//m_PointRightTop = line3.p2();
	m_KeyPoint.SetPoint(FKeyPoint::KeyPoint((kp + 2) % 8), line3.p2());
}

void FEditRectangleBase::EdgePointBeScaled(FKeyPoint::KeyPoint kp, const QPointF& pt)
{
	//QLineF line1(m_PointDown, m_PointTop);
	QLineF line1(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)), m_KeyPoint.GetPoint(kp));
	//QLineF line2(m_PointLeftDown, m_PointLeftTop);
	QLineF line2(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 5) % 8)), m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 7) % 8)));
	//QLineF line3(m_PointRightDown, m_PointRightTop);
	QLineF line3(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 3) % 8)), m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 1) % 8)));
	//qreal dis1 = qSqrt(qPow(m_PointDown.x() - m_PointTop.x(), 2) + qPow(m_PointDown.y() - m_PointTop.y(), 2));
	qreal dis1 = qSqrt(qPow(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).x() - m_KeyPoint.GetPoint(kp).x(), 2) + qPow(m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).y() - m_KeyPoint.GetPoint(kp).y(), 2));
	//qreal dis2 = qSqrt(qPow(p.x() - m_PointDown.x(), 2) + qPow(p.y() - m_PointDown.y(), 2));
	qreal dis2 = qSqrt(qPow(pt.x() - m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).x(), 2) + qPow(pt.y() - m_KeyPoint.GetPoint(FKeyPoint::KeyPoint((kp + 4) % 8)).y(), 2));
	qreal dis3 = qSqrt(qPow(m_KeyPoint.GetPoint(kp).x() - pt.x(), 2) + qPow(m_KeyPoint.GetPoint(kp).y() - pt.y(), 2));
	qreal a = (dis1*dis1 + dis2*dis2 - dis3*dis3) / (2 * dis1*dis2);  // 余弦定理
	qreal r1 = dis2*a;
	r1 = qMax(60.0, r1);
	// 确定m_PointLeftTop
	line2.setLength(r1);
	//m_PointLeftTop = line2.p2();
	m_KeyPoint.SetPoint(FKeyPoint::KeyPoint((kp + 7) % 8), line2.p2());
	// 确定m_PointRightTop
	line3.setLength(r1);
	//m_PointRightTop = line3.p2();
	m_KeyPoint.SetPoint(FKeyPoint::KeyPoint((kp + 1) % 8), line3.p2());
}

void FEditRectangleBase::OnRotating(const QPointF& p)
{
	// 利用余弦定理计算旋转角度
	QLineF line1(m_RotatePt, p);
	QLineF line2(m_RotatePt, m_RotatePt);
	qreal angle = line1.angleTo(line2);
	bool bClockWise = (angle < 90);  // 顺时针
	if (!bClockWise){  // 逆时针
		angle = 360 - angle;
		angle = -angle;
	}
	m_RotateAngle += angle;
	if (m_RotateAngle >= 360) m_RotateAngle -= 360;
	if (m_RotateAngle < 0) m_RotateAngle += 360;
	m_PressPt = p;
	OnRotatingAdjust(angle);
	emit sigUpdate();
}

void FEditRectangleBase::OnRotatingAdjust(qreal angle)
{
	QPointF center = m_RotatePt;
	QPointF center1 = QTransform().rotate(angle).map(m_RotatePt);
	center1 = center - center1;
	QTransform transform;  // = QTransform().rotate(m_RotateAngle)*QTransform().translate(center1.x(),center1.y());
	transform.translate(center1.x(), center1.y());
	transform.rotate(angle);
	m_KeyPoint.SetPoint(FKeyPoint::LeftTop_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt)));
	m_KeyPoint.SetPoint(FKeyPoint::Top_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::Top_Pt)));
	m_KeyPoint.SetPoint(FKeyPoint::RightTop_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt)));
	m_KeyPoint.SetPoint(FKeyPoint::Right_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::Right_Pt)));
	m_KeyPoint.SetPoint(FKeyPoint::RightDown_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt)));
	m_KeyPoint.SetPoint(FKeyPoint::Down_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::Down_Pt)));
	m_KeyPoint.SetPoint(FKeyPoint::LeftDown_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt)));
	m_KeyPoint.SetPoint(FKeyPoint::Left_Pt, transform.map(m_KeyPoint.GetPoint(FKeyPoint::Left_Pt)));
	SetRotatePoint();
	QPainterPath path(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightTop_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::RightDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftDown_Pt));
	path.lineTo(m_KeyPoint.GetPoint(FKeyPoint::LeftTop_Pt));
	m_PainterPath = path;
}

void FEditRectangleBase::AdjustRectangle()
{

}

/*FEditPIP*/
FEditPIP::FEditPIP(FNLETimeline* pTimeline, QRectF playerRegion, QObject* parent /*= 0*/)
	: FEditRectangleBase(pTimeline, playerRegion, parent)
{

}

FEditPIP::~FEditPIP()
{

}
