#include "stdafx.h"
#include "FRulerWidget.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"


FRulerWidget::FRulerWidget(QWidget *parent, QColor beginColor, QColor endColor) : FBaseWidget(parent)
{
	initUI();
	initData();
	m_BeginColor = beginColor;
	m_EndColor = endColor;
}

FRulerWidget::~FRulerWidget()
{

}

void FRulerWidget::setRange(int iMinValue, int iMaxValue)
{
	m_ScaleWidget->setRange(iMinValue, iMaxValue);
	repaint();
}

void FRulerWidget::setRange(float fMinValue, float fMaxValue)
{
	m_ScaleWidget->setRange(fMinValue, fMaxValue);
	repaint();
}

void FRulerWidget::resizeEvent(QResizeEvent *e)
{
	const int iLeft = 10;
	const int iOffRight = 20;
	const int iLabelWidth = 80;
	const int iLabelHeight = 20;
	const int iValueWidth = 25;
	const int iValueHeight = 20;
	m_LabelName->setGeometry(iLeft, (height() - iLabelHeight) / 2, iLabelWidth, iLabelHeight);
	m_LabelValue->setGeometry(width() - iValueWidth - iOffRight, (height() - iValueHeight) / 2, iValueWidth, iValueHeight);
	m_ScaleWidget->setGeometry(0, 0, width(), height());
	m_fCurXpos = getPosByValue(m_CurValue);
}

void FRulerWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	//绘制渐变底色
	QLinearGradient linear(QPointF(0, 0), QPointF(width(), height()));
	linear.setColorAt(0, m_BeginColor);  // QColor(9, 99, 245)
	linear.setColorAt(1, m_EndColor); //QColor(244, 173, 12)
	linear.setSpread(QGradient::PadSpread);
	painter.setBrush(linear);
	painter.drawRect(QRect(0, 0, width(), height()));
	painter.restore();
	painter.save();
	QPen pen;
	pen.setColor(QColor(255, 255, 255));
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(5);
	painter.setPen(pen);
	painter.drawLine(QPointF(m_fCurXpos, 0), QPointF(m_fCurXpos, height()));
	QRectF maskRect = m_FillRect;
	painter.restore();
	drawMaskLayer(painter);
}

void FRulerWidget::mousePressEvent(QMouseEvent *e)
{
	m_fCurXpos = e->pos().x();
	m_CurValue = getValueByPos(e->pos().x());
	QString strValue = QString::number(m_CurValue, 10);
	m_LabelValue->setText(strValue);
	emit sigValueChanged(m_CurValue);
	emit sigStrValueChanged(strValue);
	repaint();
}

void FRulerWidget::mouseMoveEvent(QMouseEvent *e)
{
	//鼠标拖拽的时候做边界判断
	if (e->pos().x() >width())
	{
		m_fCurXpos = width();
	}
	else if (e->pos().x() < 0)
	{
		m_fCurXpos = 0;
	}
	else
	{
		m_fCurXpos = e->pos().x();
	}
	m_CurValue = getValueByPos(m_fCurXpos);
	QString strValue = QString::number(m_CurValue, 10);
	m_LabelValue->setText(strValue);
	emit sigValueChanged(m_CurValue);
	emit sigStrValueChanged(strValue);
	repaint();
}

void FRulerWidget::initUI()
{
	m_LabelName = new FLabel(this);
	m_LabelValue = new FLabel(this);
	m_LabelValue->setText("0");
	m_ScaleWidget = new FScaleWidget(this);
	//子控件均不接受事件响应
	m_LabelName->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_LabelName->setAttribute(Qt::WA_TranslucentBackground);
	m_LabelValue->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_LabelValue->setAttribute(Qt::WA_TranslucentBackground);
	m_ScaleWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_ScaleWidget->setAttribute(Qt::WA_TranslucentBackground);
}

void FRulerWidget::initData()
{
	m_FillRect = QRect(0, 0, 0, 0);
	m_CurValue = 0;
	m_ScaleWidget->setRange(-100.00f, 100.00f);
}

void FRulerWidget::drawMaskLayer(QPainter& painter)
{
	painter.save();
	QPen pen(Qt::NoPen);
	QColor fillColor(RGB(255, 255, 255));
	fillColor.setAlpha(50);
	QBrush brush(fillColor);
	brush.setColor(fillColor);
	painter.setBrush(brush);
	painter.setPen(pen);
	//覆盖层要从0值位置开始绘制
	float fRefValue = getPosByValue(0);
	float fLeft = qMin(fRefValue, m_fCurXpos);
	float fRight = qMax(fRefValue, m_fCurXpos);
	/*float fLeft = qMin(m_fCurXpos, (float)width() / 2);
	float fRight = qMax(m_fCurXpos, (float)width() / 2);*/
	QRectF rect(fLeft, 0, fRight - fLeft, height());
	painter.drawRect(rect);
	painter.restore();
}

float FRulerWidget::getPosByValue(int iValue)
{
	float fPos = 0.00;
	float fMaxValue = m_ScaleWidget->getMaxValueF();
	if (qAbs(m_ScaleWidget->getMaxValueF() - m_ScaleWidget->getMinValueF()) < 0.01)
	{ 
		fPos = 0.00;
	}
	else
	{
		float iRalativeValue = (float)iValue - m_ScaleWidget->getMinValueF();
		fPos = ((float)iRalativeValue / (m_ScaleWidget->getMaxValueF() - m_ScaleWidget->getMinValueF())) * width();
	}
	return fPos;
}

int FRulerWidget::getValueByPos(float fPos)
{
	int iValue = 0;
	float fValue = (fPos / (float)width());
	float fRange = (m_ScaleWidget->getMaxValueF() - m_ScaleWidget->getMinValueF());
	iValue = (fPos / (float)width()) * (m_ScaleWidget->getMaxValueF() - m_ScaleWidget->getMinValueF());
	iValue = m_ScaleWidget->getMinValueF() + iValue;
	return iValue;
}

FScaleWidget::FScaleWidget(QWidget *parent) :FBaseWidget(parent)
{
	initData();
}

void FScaleWidget::setRange(int iMinValue, int iMaxValue)
{
	m_iMinumNum = iMinValue;
	m_iMaxMunNum = iMaxValue;
}

void FScaleWidget::setRange(float fMinValue, float fMaxValue)
{
	m_fMinumNum = fMinValue;
	m_fMaxMunNum = fMaxValue;
}

void FScaleWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.save();


	float  fDistance = (float)width() / (float)m_iDivideNum;
	QPen pen;
	pen.setColor(QColor(255,255,255));
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(1);
	painter.setPen(pen);
	setScaleSize();
	for (int i = 1; i < m_iDivideNum; ++i)
    {
		float fXpos = i * fDistance;
		if (i%5  != 0)
		{
			painter.drawLine(QPointF(fXpos, 0.00), QPointF(fXpos, m_fSmallScaleLen));
		}
		else
		{
			painter.drawLine(QPointF(fXpos, 0.00), QPointF(fXpos, m_fBigScalelen));
		}
    }

	painter.restore();
}

void FScaleWidget::setScaleSize()
{
	const int iLevel1 = 60;
	const int iLevel2 = 150;
	int iHeight = height();
	if (iHeight < iLevel1 )
	{
		m_fSmallScaleLen = 5.00;
		m_fBigScalelen = 10.00;
	}
	else if (iHeight <iLevel2 && iHeight> iLevel1)
	{
		m_fSmallScaleLen = 10.00;
		m_fBigScalelen = 20.00;
	}
	else
	{
		m_fSmallScaleLen = 15.00;
		m_fBigScalelen = 25.00;
	}
}

void FScaleWidget::initData()
{
	m_iDivideNum = 40;
	m_iMaxMunNum = 0;
	m_iMinumNum = 0;
	m_fMinumNum = 0.00;
	m_fMaxMunNum = 0.00;
}
