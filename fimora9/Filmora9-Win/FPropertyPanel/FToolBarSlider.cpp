#include "stdafx.h"
#include "FToolBarSlider.h"
#include "inc_CommonLib/CommonWidgets.h"

#include <QHBoxLayout>

FToolBarSlider::FToolBarSlider(QWidget *parent /*= 0*/, Qt::Orientation orientation /*= Qt::Horizontal*/)
: QWidget(parent)
, m_minValue(0)
, m_maxValue(0)
{
	m_pToolBarSlider = new ToolBarSlider(0, orientation);
	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->addWidget(m_pToolBarSlider);
	pLayout->setMargin(0);

	connect(m_pToolBarSlider, &ToolBarSlider::valueChanged, this, [&](double value){
		emit valueChanged(m_minValue + value);
	});
	connect(m_pToolBarSlider, &ToolBarSlider::startValueChanged, this, &FToolBarSlider::startValueChanged);
	connect(m_pToolBarSlider, &ToolBarSlider::endValueChanged, this, &FToolBarSlider::endValueChanged);
	connect(m_pToolBarSlider, &ToolBarSlider::startDrag, this, &FToolBarSlider::startDrag);
	connect(m_pToolBarSlider, &ToolBarSlider::endDrag, this, &FToolBarSlider::endDrag);
}

FToolBarSlider::~FToolBarSlider()
{

}

void FToolBarSlider::SetRange(float min, float max)
{
	if (min > max){
		min = max = 0;
	}
	m_minValue = min;
	m_maxValue = max;
	m_pToolBarSlider->setDuration(max - min);
}

void FToolBarSlider::SetValue(float value, bool queueRepaint /*= true*/)
{
	value = qBound(m_minValue, value, m_maxValue);
	m_pToolBarSlider->setValue(value - m_minValue, queueRepaint);
}

float FToolBarSlider::Value()
{
	return m_pToolBarSlider->value() + m_minValue;
}

float FToolBarSlider::Duration()
{
	return m_maxValue - m_minValue;
}

bool FToolBarSlider::CanSeek()
{
	return m_pToolBarSlider->canSeek();
}

int FToolBarSlider::Height()
{
	return m_pToolBarSlider->Height();
}

void FToolBarSlider::SetGrooveHeight(int h)
{
	m_pToolBarSlider->SetGrooveHeight(h);
}

bool FToolBarSlider::ButtonPressDown()
{
	return m_pToolBarSlider->ButtonPressDown();
}

void FToolBarSlider::SetButtonPressDown(bool press)
{
	m_pToolBarSlider->SetButtonPressDown(press);
}

void FToolBarSlider::SetLeftToRight(bool left)
{
	m_pToolBarSlider->SetLeftToRight(left);
}

void FToolBarSlider::SetBottomToTop(bool bottom)
{
	m_pToolBarSlider->SetBottomToTop(bottom);
}

void FToolBarSlider::SetChunkHeight(int height)
{
	m_pToolBarSlider->SetChunkHeight(height);
}

void FToolBarSlider::SetSliderObjectName(QString name)
{
	m_pToolBarSlider->setObjectName(name);
}

float FToolBarSlider::GetMinValue()
{
	return m_minValue;
}

float FToolBarSlider::GetMaxValue()
{
	return m_maxValue;
}









