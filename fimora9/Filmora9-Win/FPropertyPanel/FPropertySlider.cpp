#include "stdafx.h"
#include "FPropertySlider.h"
#include "FToolBarSlider.h"

/*FPropertySlider*/
FPropertySlider::FPropertySlider(QWidget *parent /*= 0*/)
: FBaseWidget(parent)
, m_DecimalPlaces(0)
, m_Suffix("")
, m_PropertyName("Property")
, m_PropertyWidth(80)
, m_SliderWidth(240)
, m_ValueWidth(60)
{
	m_pPropertyLbl = new FLabel(this);
	m_pSlider = new FToolBarSlider(this);
	m_pValueLbl = new FLabel(this);
	SetPropertyName("Property:");

	connect(m_pSlider, &FToolBarSlider::valueChanged, this, [&](float value){
		emit sigValueChanged(value);
		QString text;
		text = QString::number(value, 'f', m_DecimalPlaces);
		text += m_Suffix;
		m_pValueLbl->setText(text);
	});
	SetValue(m_pSlider->GetMinValue());
	setMinimumWidth(400);
}

FPropertySlider::~FPropertySlider()
{

}

void FPropertySlider::SetValue(float value)
{
	m_pSlider->SetValue(value);
	emit m_pSlider->valueChanged(m_pSlider->Value());
}

float FPropertySlider::GetValue()
{
	return m_pSlider->Value();
}

void FPropertySlider::SetAlignmentOfProperty(Qt::Alignment alignment)
{
	m_pPropertyLbl->setAlignment(alignment);
}

void FPropertySlider::SetPropertyWidth(int w)
{
	m_PropertyWidth = w;
	resizeEvent(nullptr);
}

void FPropertySlider::SetAlignmentOfValue(Qt::Alignment alignment)
{
	m_pValueLbl->setAlignment(alignment);
}

void FPropertySlider::SetValueWidth(int w)
{
	m_ValueWidth = w;
	resizeEvent(nullptr);
}

void FPropertySlider::SetRange(float min, float max)
{
	m_pSlider->SetRange(min, max);
	SetValue(m_pSlider->GetMinValue());
}

void FPropertySlider::SetSliderWidth(int w)
{
	m_SliderWidth = w;
	resizeEvent(nullptr);
}

void FPropertySlider::SetPropertyName(QString propertyName)
{
	m_PropertyName = propertyName;
	m_pPropertyLbl->setText(m_PropertyName);
}

QString FPropertySlider::GetPropertyName()
{
	return m_PropertyName;
}

void FPropertySlider::SetDecimalPlaces(int decimalPlaces)
{
	if (decimalPlaces < 0) decimalPlaces = 0;
	m_DecimalPlaces = decimalPlaces;
	SetValue(GetValue());  // 刷新显示
}

void FPropertySlider::SetSuffix(QString suffix)
{
	m_Suffix = suffix;
	SetValue(GetValue());  // 刷新显示
}

void FPropertySlider::SetSliderObjectName(QString name)
{
	m_pSlider->SetSliderObjectName(name);
}

void FPropertySlider::resizeEvent(QResizeEvent *)
{
	int left, top;
	top = 0;
	left = width() * 0.5 - m_SliderWidth / 2;
	if (left < m_PropertyWidth) left = m_PropertyWidth;
	m_pSlider->setGeometry(left, top, m_SliderWidth, height());
	left = m_pSlider->geometry().left() - width() * 0.08 - m_PropertyWidth;
	if (left < 0) left = 0;
	m_pPropertyLbl->setGeometry(left, top, m_PropertyWidth, height());
	left = m_pSlider->geometry().right() + width() * 0.05;
	m_pValueLbl->setGeometry(left, top, m_ValueWidth, height());
}



