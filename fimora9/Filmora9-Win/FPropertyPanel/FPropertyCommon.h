#ifndef FPROPERTYCOMMON_H
#define FPROPERTYCOMMON_H

#include "stdafx.h"
#include <QtWidgets/QWidget>
#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "inc_CommonLib/CommonWidgets.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"


/*FPropertyRotate*/
class FPropertyRotate : public FBaseWidget
{
	Q_OBJECT

public:
	enum RotateMode{
		Rotate90Clockwise,
		Rotate90AntiClockwise,
		FlipHorizontal,
		FlipVertical
	};

public:
	FPropertyRotate(QWidget *parent = 0);
	~FPropertyRotate();
	void RetranslateUi() override;
	
Q_SIGNAL
	void sigRotateChanged(RotateMode);

private:
	void ConnectSignals();

private:
	FLabel *m_pPropertyLbl;
	QList<FImageButton*> m_roateModeList;
};

/*FPropertyCheckBox*/
class FPropertyCheckBox : public FBaseWidget
{
	Q_OBJECT

public:
	FPropertyCheckBox(QWidget *parent);
	~FPropertyCheckBox();

	void SetPropertyName(const QString&);
	void SetCheckBoxString(const QString&);
	void SetCheckState(bool checked);
	bool GetCheckState();

Q_SIGNAL
	void sigStateChanged(bool checked);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	FLabel *m_pPropertyLbl;
	FCheckBox *m_pCheckBox;
};

/*BottomButtonBar*/
class BottomButtonBar : public FBaseWidget
{
	Q_OBJECT

public:
	enum Button_ID{
		Reset_Btn,
		Advanced_Btn,
		Ok_Btn
	};

public:
	BottomButtonBar(QWidget *parent = 0);
	void HideBtnByID(Button_ID id);
	void ShowBtnByID(Button_ID id);
	void SetEnabledByID(Button_ID id, bool enabled);
	void RetranslateUi();

Q_SIGNAL
	void sigClicked(Button_ID);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void ConnectSignals();

private:
	FButton *m_pResetBtn;
	FButton *m_pAdvancedBtn;
	FButton *m_pOkBtn;
};

/*FPropertyButton*/
class FPropertyButton : public FBaseWidget
{
	Q_OBJECT

public:
	FPropertyButton(QWidget *parent = 0);
	~FPropertyButton();

	void SetPropertyName(const QString&);
	void SetButtonText(const QString&);
	QString GetButtonText();

signals:
	void sigTextButtonClicked();
	void sigImageButtonClicked();

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	FLabel *m_pPropertyLbl;
	FButton *m_pTextBtn;
	FImageButton *m_pImageBtn;
};

class FPROPERTYPANEL_EXPORT FNonLinearSlider : public FBaseWidget
{
   Q_OBJECT
public:
	FNonLinearSlider(QWidget *parent = 0);
	~FNonLinearSlider();
	void setLabels(const QList<QString> totalLabels);
	void setReservedLength(int iLength){ m_iReservedLength = iLength; };
	int getReservedLength(){ return m_iReservedLength; };
	void ShowLables(bool bShow){ m_bIsShowLables = bShow; };
	QString GetCurValue(){ return m_strCurValue; };

Q_SIGNALS:
	void sigSelectChanged(QString strUserSelect);

public:
	bool eventFilter(QObject *target, QEvent *e);

protected:
	void resizeEvent(QResizeEvent *event)override;
	
private:
	void initUI();
	void initData();
	void mousePosDetect(QMouseEvent * event);
	int getIndex(float fXpos);
private:
	ToolBarSlider* m_MainSlider;
	QList<QString> m_Lables;
	QList<QLabel*> m_AllLabels;
	bool m_bIsShowLables; //slider下面是否显示label
	int m_iReservedLength;  //右边预留的区域长度
	QString m_strCurValue;
};

class FFiniteValueSlider : public FBaseWidget
{
	Q_OBJECT

public:
	FFiniteValueSlider(QWidget *parent = nullptr);
	~FFiniteValueSlider();

	void SetFiniteValues(const QList<double>&);
	void SetValue(double);
	double GetValue();
	void SetSliderObjectName(const QString&);

protected:
	bool eventFilter(QObject *, QEvent *) override;

signals:
	void sigValueChanged(double);
	void sigStartDrag();
	void sigEndDrag();

private:
	void ResetInnerValues();
	void slotValueChanged(double);

private:
	double m_CurValue;
	ToolBarSlider *m_pSlider;
	QList<double> m_Values;
	QList<double> m_InnerValues;
};

class FFiniteValuePropertySlider : public FBaseWidget
{
	Q_OBJECT

public:
	FFiniteValuePropertySlider(QWidget* parent = nullptr);
	~FFiniteValuePropertySlider();
	
	void SetFiniteValues(const QList<double>&);
	void SetValue(double);
	double GetValue();
	// 设置property对齐方式，默认左对齐
	void SetAlignmentOfProperty(Qt::Alignment);
	// 设置属性label宽度
	void SetPropertyWidth(int);
	// 设置value对齐方式，默认左对齐
	void SetAlignmentOfValue(Qt::Alignment);
	// 设置slider宽度
	void SetSliderWidth(int);
	// 设置value label宽度
	void SetValueWidth(int);
	// 设置property name
	void SetPropertyName(const QString&);
	QString GetPropertyName();
	// 设置 decimal places under Float_Format
	void SetDecimalPlaces(int);
	// 设置数值后缀
	void SetSuffix(const QString&);
	// 设置Slider Object Name
	void SetSliderObjectName(const QString&);

signals:
	void sigValueChanged(double);
	void SetValues();

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	FFiniteValueSlider *m_pSlider;
	FLabel *m_pPropertyLbl;
	FLabel *m_pValueLbl;
	QString m_Suffix;
	QString m_PropertyName;
	int m_PropertyWidth;
	int m_SliderWidth;
	int m_ValueWidth;
	int m_DecimalPlaces;
};

#endif // FPROPERTYCOMMON_H
