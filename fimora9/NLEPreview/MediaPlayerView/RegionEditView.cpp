#include "stdafx.h"
#include "RegionEditView.h"
#include "inc_MediaPlayerView/MediaPlayerView.h"
#include "inc_CommonLib/NLETimeline.h"
#include "mediaplayercontrolprivate.h"
#include "Interface/INLETimelineBase.h"
#include <QtWidgets/QScrollArea>

//CornerWidget
CornerWidget::CornerWidget(int nIndex,QWidget *parent)
:QWidget(parent)
{
	m_nIndex = nIndex;
	SetRotateAngel(0.0);
}

CornerWidget::~CornerWidget()
{
	
}

int CornerWidget::Index()
{
	return m_nIndex;
}

void CornerWidget::SetRotateAngel(float angle)
{
	m_RotateAngel = angle;
	SetCursor();
}

void CornerWidget::SetCursor()
{

}

void CornerWidget::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	QBrush br(QColor(255,255,255));
	p.setBrush(br);
	p.fillRect(rect(), Qt::SolidPattern);
	e->ignore();
}

void CornerWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->type() == Qt::LeftButton)
	{	
		emit sigMouseMove(this,mapToParent(e->localPos().toPoint()));
	}
	e->accept();
}

void CornerWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->type() == Qt::LeftButton)
	{
		emit sigMouseRelease(this, mapToParent(e->localPos().toPoint()));
	}
	e->accept();
}

//RegionEditWidget
RegionEditWidget::RegionEditWidget(QWidget *parent)
:QWidget(parent)
{
	//setAttribute(Qt::WA_TranslucentBackground, true);
	m_RotateAngel = 0.0;
	m_Scale = QSizeF(0.0,0.0);
	m_Center.setX(0.5);
	m_Center.setY(0.5);
	m_pCurEditingTL = nullptr;
	m_MousePressPos = MousePressPos_None;
	m_offset = 0;

	setStyleSheet("background:white;");
	setMouseTracking(true);
	
	CornerWidget* pCW = nullptr;
	for (int i = 0; i < 8; ++i)
	{
		pCW = new CornerWidget(i,this);
		pCW->resize(6, 6);
		m_CornerWidgets[i] = pCW;
		connect(pCW, &CornerWidget::sigMouseMove, this, &RegionEditWidget::slotCornerMove);
		connect(pCW, &CornerWidget::sigMouseRelease, this, &RegionEditWidget::slotCornerRelease);
	}
}

RegionEditWidget::~RegionEditWidget()
{
	
}

NLETimeline* RegionEditWidget::GetEditTimeline()
{
	return m_pCurEditingTL;
}

void RegionEditWidget::SetEditTimeline(NLETimeline *pTimeline)
{
	m_pCurEditingTL = pTimeline;
}

void RegionEditWidget::SetFrameSize(QSize size)
{
	m_FrameSize = size;
}

void RegionEditWidget::SetBkImg(QPixmap px)
{
	m_bkimg = px;
}

void RegionEditWidget::SetOffset(int nOffset)
{
	m_offset = nOffset;
	update();
}

void RegionEditWidget::Init()
{
	UpdateDataFromTimeline();
	CalcRegion();
	UpdateDisplay();
}

void RegionEditWidget::UpdateDataFromTimeline()
{
	m_Center = m_pCurEditingTL->GetRegionCenter();
	m_Scale = m_pCurEditingTL->GetRegionScale();
	m_RotateAngel = m_pCurEditingTL->GetRegionRotateAngle();
}

void RegionEditWidget::CalcRegion()
{
	int nBaseLen = m_FrameSize.width();
	
	//已实际宽度为1来计算
	float fW = m_Scale.width() * (m_FrameSize.width() * 1.0 / nBaseLen);
	float fH = m_Scale.height() * (m_FrameSize.height() * 1.0 / nBaseLen);

	//半径，对角线的一半
	float fRadius = qSqrt(fW * fW + fH * fH) * 0.5;
	
	//对角线与宽的夹角
	float fInterRadians = qAcos(0.5* fW / fRadius);
	
	//点0的xy坐标绝对值,相对于原点(0,0)
	m_CornerPts[0].setX(-qCos(fInterRadians) * fRadius);
	m_CornerPts[0].setY(qSin(fInterRadians) * fRadius);

	//根据点0计算其它7个点的坐标(旋转角度为0的情况)
	m_CornerPts[5] = RotatePoint(m_CornerPts[0], 2 * fInterRadians);
	m_CornerPts[7] = RotatePoint(m_CornerPts[5], 2 * (M_PI_2 - fInterRadians));
	m_CornerPts[2] = RotatePoint(m_CornerPts[7], 2 * fInterRadians);
	m_CornerPts[1] = MiddlePoint(m_CornerPts[0], m_CornerPts[2]);
	m_CornerPts[6] = RotatePoint(m_CornerPts[1], M_PI);
	m_CornerPts[3] = MiddlePoint(m_CornerPts[0], m_CornerPts[5]);
	m_CornerPts[4] = RotatePoint(m_CornerPts[3], M_PI);

	//根据旋转角度，计算点坐标,每个点要加上pCenter的坐标的偏移量
	float fRotateRadians = qDegreesToRadians((float)m_RotateAngel);
	for (int i = 0; i < 8; ++i)
	{
		m_CornerPts[i] = RotatePoint(m_CornerPts[i], 2 * M_PI - fRotateRadians);
		m_CornerPts[i].setX(m_CornerPts[i].x() + m_Center.x());
		m_CornerPts[i].setY(m_CornerPts[i].y() + m_Center.y());
	}

	//计算出8个点的真实坐标
	//1. x,获取最左边的点，x最小值得点
	int nMinXIndex = GetMostLeftPt();
	if (m_CornerPts[nMinXIndex].x()<0)
	{
		m_CornerPtsReal[nMinXIndex].setX(0);
	}
	else
	{
		m_CornerPtsReal[nMinXIndex].setX(m_CornerPts[nMinXIndex].x() * nBaseLen);
	}
	//2. y
	int nMinYIndex = GetMostTopPt();
	if (m_CornerPts[nMinYIndex].y()<0)
	{
		m_CornerPtsReal[nMinYIndex].setY(0);
	}
	else
	{
		m_CornerPtsReal[nMinYIndex].setY(m_CornerPts[nMinYIndex].y() * nBaseLen);
	}
	
	for (int i = 0; i < 8; ++i)
	{
		if (i!= nMinXIndex)
		{
			m_CornerPtsReal[i].setX(m_CornerPtsReal[nMinXIndex].x() + (m_CornerPts[i].x() - m_CornerPts[nMinXIndex].x())* nBaseLen);
		}

		if (i!= nMinYIndex)
		{
			m_CornerPtsReal[i].setY(m_CornerPtsReal[nMinYIndex].y() + (m_CornerPts[i].y() - m_CornerPts[nMinYIndex].x())* nBaseLen);
		}
	}

	//计算Region
	m_regionPts.push_back(m_CornerPtsReal[0]);
	m_regionPts.push_back(m_CornerPtsReal[2]);
	m_regionPts.push_back(m_CornerPtsReal[5]);
	m_regionPts.push_back(m_CornerPtsReal[7]);
	m_EditRegion = QRegion(QPolygon(m_regionPts));
	//计算中心点像素值
	m_CenterReal.setX(m_CornerPtsReal[nMinXIndex].x() + (m_Center.x() - m_CornerPts[nMinXIndex].x()) * nBaseLen);
	m_CenterReal.setY(m_CornerPtsReal[nMinYIndex].y() + (m_Center.y() - m_CornerPts[nMinYIndex].y()) * nBaseLen);
}

void RegionEditWidget::UpdateDisplay()
{
	//设置8个点控件的位置
	ReallocCornerWidgets();
	//画出编辑区域
	update(); 
}

void RegionEditWidget::ReallocCornerWidgets()
{
	for (int i = 0; i < 8; ++i)
	{
		m_CornerWidgets[i]->move(QPoint(m_CornerPtsReal[i].x() - 3,m_CornerPtsReal[i].y()-3));
	}
}
	
void RegionEditWidget::ResizeAfferSetting()
{
	QPointF ptMostLeft = m_CornerPts[GetMostLeftPt()];
	QPointF ptMostTop = m_CornerPts[GetMostTopPt()];
	QPointF ptMostRight = m_CornerPts[GetMostRightPt()];
	QPointF ptMostBottom = m_CornerPts[GetMostBottomPt()];
	
	float l, t, r, b;
	l = ptMostLeft.x() < 0 ? ptMostLeft.x() : 0;
	t = ptMostTop.y() < 0 ? ptMostTop.y() : 0;
	r = ptMostRight.x() > 1 ? ptMostRight.x() : 1;
	b = ptMostBottom.y() > 1 ? ptMostBottom.y() : 1;
	
	resize(m_FrameSize.width() * (r - l), m_FrameSize.height() * (b - t));
	m_WholeRect.setTopLeft(QPoint(0, 0));
	m_WholeRect.setSize(size());
	
}

QPointF RegionEditWidget::RotatePoint(QPointF oldPt, float rotateRadians)
{
	QPointF newPt;
	newPt.setX(qCos(rotateRadians) * oldPt.x() - qSin(rotateRadians) * oldPt.y());
	newPt.setY(qSin(rotateRadians) * oldPt.x() + qCos(rotateRadians) * oldPt.y());
	return newPt;
}

QPointF RegionEditWidget::MiddlePoint(QPointF pt1, QPointF pt2)
{
	return QPointF((pt1.x() + pt2.x()) / 2, (pt1.y() + pt2.y()) / 2);
}

int RegionEditWidget::GetMostLeftPt()
{
	int nIndex = 0;
	float minX = 1000.0;
	for (int i = 0; i < 8; ++i)
	{
		if (m_CornerPts[i].x() < minX)
		{
			minX = m_CornerPts[i].x();
			nIndex = i;
		}
	}
	return nIndex;
}

int RegionEditWidget::GetMostTopPt()
{
	int nIndex = 0;
	float minY = 1000.0;
	for (int i = 0; i < 8; ++i)
	{
		if (m_CornerPts[i].y() < minY)
		{
			minY = m_CornerPts[i].y();
			nIndex = i;
		}
	}
	return nIndex;
}

int RegionEditWidget::GetMostRightPt()
{
	int nIndex = 0;
	float maxX = -1000.0;
	for (int i = 0; i < 8; ++i)
	{
		if (m_CornerPts[i].x() > maxX)
		{
			maxX = m_CornerPts[i].x();
			nIndex = i;
		}
	}
	return nIndex;
}

int RegionEditWidget::GetMostBottomPt()
{
	int nIndex = 0;
	float maxY = -1000.0;
	for (int i = 0; i < 8; ++i)
	{
		if (m_CornerPts[i].y() > maxY)
		{
			maxY = m_CornerPts[i].y();
			nIndex = i;
		}
	}
	return nIndex;
}

void RegionEditWidget::paintEvent(QPaintEvent *e)
{
	QPainter p(this);

	m_Center.setX(0.5);
	m_Center.setY(0.5);

	m_Scale.setWidth(0.5);
	m_Scale.setHeight(0.5);

	p.fillRect(rect(), Qt::white);

	//先缩放
	p.save();
	//p.scale(m_Scale.width(), m_Scale.height());

	//int nScaleDx = (m_Scale.width() - 1.0) * rect().width() ;  //负数表示缩小，中心点x向左偏移
	//int nScaleDy = (m_Scale.height() - 1.0) * rect().height() ; //负数表示缩小，中心点y向上偏移
	//p.restore();

	//再平移
	//p.save();

	int nOrinCenterX = rect().width() * 0.5;
	int nOrinCenterY = rect().height() * 0.5;

	int nCurCenterX = rect().width() * m_Center.x();
	int nCurCenterY = rect().height() * m_Center.y();

	//p.setWindow(/*nCurCenterX - nOrinCenterX, nCurCenterY - nOrinCenterY,*/-rect().width() * 0.5, - rect().height() * 0.5 ,rect().width(), rect().height());
	p.scale(m_Scale.width(), m_Scale.height());


	//p.translate(nCurCenterX - nOrinCenterX, nCurCenterY - nOrinCenterY);
	//p.translate(rect().width()* 0.25, rect().width() * 0.25);

	//绘制区域边框
	QPen pen(QColor(0, 0, 255));
	p.setPen(pen);
	p.drawRect(rect());
	p.restore();
	
	//p.drawLine(m_CornerPtsReal[0], m_CornerPtsReal[2]);
	//p.drawLine(m_CornerPtsReal[2], m_CornerPtsReal[7]);
	//p.drawLine(m_CornerPtsReal[7], m_CornerPtsReal[5]);
	//p.drawLine(m_CornerPtsReal[5], m_CornerPtsReal[0]);

	//绘制旋转区域的圆圈
	//pen.setColor(QColor(255,255,255));
	//pen.setWidth(2);
	//p.drawEllipse(m_CenterReal, 20,20);

	//绘制十字架
	//p.save();
	//p.rotate(m_RotateAngel);
	//p.drawLine(QPoint(m_CenterReal.x() - 9, m_CornerPtsReal->y()), QPoint(m_CenterReal.x() + 9, m_CornerPtsReal->y()));
	//p.drawLine(QPoint(m_CenterReal.x(), m_CornerPtsReal->y()-9), QPoint(m_CenterReal.x(), m_CornerPtsReal->y()+9));
	
	e->ignore();
}

void RegionEditWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (m_MousePressPos == MousePressPos_EditRegion) //平移
	{
		float fCenterDx = (e->pos().x() - m_CenterReal.x()) / m_FrameSize.width();
		float fCenterDy = (e->pos().y() - m_CenterReal.y()) / m_FrameSize.height();
		m_Center += QPointF(fCenterDx, fCenterDy);
		CalcRegion();
		UpdateDisplay();
	}
	else if (m_MousePressPos == MousePressPos_RotateRegion) //旋转
	{

	}
	e->accept();
}

void RegionEditWidget::mousePressEvent(QMouseEvent *e)
{
	if (m_EditRegion.contains(e->pos()))
	{
		m_MousePressPos = MousePressPos_EditRegion;
	}
	else if (m_RotateRegion.contains(e->pos()))
	{
		m_MousePressPos = MousePressPos_RotateRegion;
	}
	else
	{
		m_MousePressPos = MousePressPos_None;
	}
	m_PressPt = e->pos();
	e->accept();
}

void RegionEditWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (m_PressPt == e->pos())
	{
		e->accept();
		return;
	}
	if (m_MousePressPos == MousePressPos_EditRegion)
	{
		m_pCurEditingTL->SetRegionCenter(m_Center);
	}
	else if (m_MousePressPos == MousePressPos_RotateRegion)
	{
	}
	else
	{
		e->accept();
		return;
	}
	//调整控件大小
	ResizeAfferSetting();
	//重刷背景图
	emit sigBkChanged();
	m_MousePressPos = MousePressPos_None;
	e->accept();
}

void RegionEditWidget::slotCornerMove(CornerWidget* pCorner, QPoint pt)
{

}

void RegionEditWidget::slotCornerRelease(CornerWidget* pCorner, QPoint pt)
{

}

//RegionEditView
RegionEditView::RegionEditView(QWidget *parent)
	: QWidget(parent)
{
	//m_scroolArea = new QScrollArea(this);
	//m_bkLabel = new QLabel(m_scroolArea);
	//m_bkLabel->setAlignment(Qt::AlignCenter);
	
	m_pRegionEditWidget = new RegionEditWidget(this);
	//m_pRegionEditWidget->raise();
}

RegionEditView::~RegionEditView()
{

}

void RegionEditView::Init()
{
	//m_bkLabel->setPixmap(QPixmap::fromImage(m_bk));
	m_pRegionEditWidget->Init();
}

void RegionEditView::SetUndoStack(QUndoStack *pUndoStack)
{
	m_pUndoStack = pUndoStack;
}

NLETimeline* RegionEditView::GetEditTimeline()
{
	return m_pRegionEditWidget->GetEditTimeline();
}

void RegionEditView::SetEditTimeline(NLETimeline *pTimeline)
{
	m_pRegionEditWidget->SetEditTimeline(pTimeline);
}

void RegionEditView::SetBkImg(QPixmap px)
{
	m_pRegionEditWidget->SetBkImg(px);
}

void RegionEditView::resizeEvent(QResizeEvent *e)
{
	//m_scroolArea->setGeometry(rect());
	//m_bkLabel->setGeometry(rect());
	m_pRegionEditWidget->setGeometry(rect());
	//m_pRegionEditWidget->SetFrameSize(QSize(rect().width(), rect().height()));
}


//MainMediaPlayerViewEx
MainMediaPlayerViewEx::MainMediaPlayerViewEx(QWidget *parent)
:MainMediaPlayerView(new MainMediaPlayerViewPrivateEx,parent)
{

}


void MainMediaPlayerViewEx::SetLoadType(bool bTimelimeLoaded)
{
	MainMediaPlayerViewPrivateEx *p = dynamic_cast<MainMediaPlayerViewPrivateEx*>(m_p);
	if (!p) return;
	p->slotLoadType(bTimelimeLoaded);
}

void MainMediaPlayerViewEx::SetSeletedTimeline(NLETimeline *pSelected)
{
	MainMediaPlayerViewPrivateEx *p = dynamic_cast<MainMediaPlayerViewPrivateEx*>(m_p);
	if (!p) return;
	p->slotTimelineSelectChanged(pSelected);
}


//MainMediaPlayerViewPrivateEx
MainMediaPlayerViewPrivateEx::MainMediaPlayerViewPrivateEx(QWidget *parent)
:MainMediaPlayerViewPrivate(parent)
{
	m_bTimelineLoaded = false;
	m_pRegionEditView = new RegionEditView(this);

	m_pRegionEditView->setHidden(true);
	//EnterEditModel(false);

	connect(m_mediaPlayerControl, &MediaPlayerControl::RulerSeekCallback, this, &MainMediaPlayerViewPrivateEx::slotSeekToFrame);
	connect(m_mediaPlayerControl, &MediaPlayerControl::PlayPauseCallback, this, &MainMediaPlayerViewPrivateEx::slotPause);
	connect(m_mediaPlayerControl, &MediaPlayerControl::PlayStartCallback, this, &MainMediaPlayerViewPrivateEx::slotPlay);
}

void MainMediaPlayerViewPrivateEx::resizeEvent(QResizeEvent *e)
{
	MainMediaPlayerViewPrivate::resizeEvent(e);

	m_pRegionEditView->setGeometry(m_displayView->GetDisplayWidget()->geometry());
}

void MainMediaPlayerViewPrivateEx::EnterEditModel(bool bEnter)
{
	if (!m_mediaPlayerControl->GetTimelineBase()) return;
	NLETimeline *timeline = m_mediaPlayerControl->GetTimelineBase();
	m_pRegionEditView->SetBkImg(QPixmap::fromImage(timeline->GetCurrentFrameImg(m_displayView->GetDisplayWidget()->geometry().width(), m_displayView->GetDisplayWidget()->geometry().height())));
	m_pRegionEditView->setHidden(!bEnter);
	m_pRegionEditView->Init();
}

void MainMediaPlayerViewPrivateEx::slotTimelineSelectChanged(NLETimeline *pSelected)
{
	m_pRegionEditView->SetEditTimeline(pSelected);
	if (pSelected && !m_mediaPlayerControl->IsPlaying())
	{
		EnterEditModel(true);
		return;
	}
	EnterEditModel(false);
}

void MainMediaPlayerViewPrivateEx::slotLoadType(bool bTimeline)
{
	m_bTimelineLoaded = bTimeline;
}

void MainMediaPlayerViewPrivateEx::slotSeekToFrame(int nFrame)
{
	if (m_bTimelineLoaded 
		&& m_pRegionEditView->GetEditTimeline() 
		&& m_pRegionEditView->GetEditTimeline()->IsFrameIn(nFrame)
		&& !m_mediaPlayerControl->IsPlaying())
	{
		EnterEditModel(true);
		return;
	}
	EnterEditModel(false);
}

void MainMediaPlayerViewPrivateEx::slotPause()
{
	NLETimeline *timeline = m_mediaPlayerControl->GetTimelineBase();
	int nCurFrame = timeline->GetCurrentFrame();
	
	if (m_bTimelineLoaded
		&& m_pRegionEditView->GetEditTimeline()
		&& m_pRegionEditView->GetEditTimeline()->IsFrameIn(nCurFrame))
	{
		EnterEditModel(true);
		return;
	}
	EnterEditModel(false);
}

void MainMediaPlayerViewPrivateEx::slotPlay()
{
	EnterEditModel(false);
}

