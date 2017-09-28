#ifndef FPROPERTYSLIDER_H
#define FPROPERTYSLIDER_H

#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "inc_CommonLib/CommonWidgets.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"

class FToolBarSlider;

class FPROPERTYPANEL_EXPORT FPropertySlider : public FBaseWidget
{
	Q_OBJECT

public:
	FPropertySlider(QWidget *parent = 0);
	~FPropertySlider();
	// set value
	void SetValue(float);
	float GetValue();
	// 设置property对齐方式，默认左对齐
	void SetAlignmentOfProperty(Qt::Alignment);
	// 设置属性label宽度
	void SetPropertyWidth(int);
	// 设置value对齐方式，默认左对齐
	void SetAlignmentOfValue(Qt::Alignment);
	// 设置value label宽度
	void SetValueWidth(int);
	// 设置slider数值范围
	void SetRange(float min, float max);
	// 设置slider宽度
	void SetSliderWidth(int);
	// 设置property name
	void SetPropertyName(QString);
	QString GetPropertyName();
	// 设置 decimal places under Float_Format
	void SetDecimalPlaces(int);
	// 设置数值后缀
	void SetSuffix(QString);
	// 设置Slider Object Name
	void SetSliderObjectName(QString);

Q_SIGNAL
	void sigValueChanged(float value);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	int m_PropertyWidth;
	int m_SliderWidth;
	int m_ValueWidth;
	int m_DecimalPlaces;
	QString m_Suffix;
	QString m_PropertyName;
	FLabel *m_pPropertyLbl;
	FToolBarSlider *m_pSlider;
	FLabel *m_pValueLbl;
};

#endif // FPROPERTYSLIDER_H
