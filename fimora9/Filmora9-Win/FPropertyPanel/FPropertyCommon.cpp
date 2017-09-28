#include "stdafx.h"
#include "FPropertyCommon.h"
#include <math.h>
#include <QHBoxLayout>

/*FPropertyRotate*/
FPropertyRotate::FPropertyRotate(QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
{
	m_pPropertyLbl = new FLabel(this);
	m_roateModeList.append(new FImageButton(Rotate90Clockwise, "Rotate90Clockwise_Btn", 0));
	m_roateModeList.append(new FImageButton(Rotate90AntiClockwise, "Rotate90AntiClockwise_Btn", 0));
	m_roateModeList.append(new FImageButton(FlipHorizontal, "FlipHorizontal_Btn", 0));
	m_roateModeList.append(new FImageButton(FlipVertical, "FlipVertical_Btn", 0));

	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->addStretch(1);
	pLayout->addWidget(m_pPropertyLbl, 1);
	pLayout->addStretch(3);
	pLayout->addWidget(m_roateModeList[0], 1);
	pLayout->addStretch(1);
	pLayout->addWidget(m_roateModeList[1], 1);
	pLayout->addStretch(1);
	pLayout->addWidget(m_roateModeList[2], 1);
	pLayout->addStretch(1);
	pLayout->addWidget(m_roateModeList[3], 1);
	pLayout->addStretch(6);
	pLayout->setMargin(0);

	ConnectSignals();
	RetranslateUi();
}

FPropertyRotate::~FPropertyRotate()
{

}

void FPropertyRotate::RetranslateUi()
{
	m_pPropertyLbl->setText(tr("Rotate"));
	m_roateModeList[0]->setToolTip(tr("Rotate 90clockwise"));
	m_roateModeList[1]->setToolTip(tr("Rotate 90Anticlockwise"));
	m_roateModeList[2]->setToolTip(tr("Flip horizontal"));
	m_roateModeList[3]->setToolTip(tr("Flip vertical"));
}

void FPropertyRotate::ConnectSignals()
{
	for (int i = 0; i < m_roateModeList.count(); ++i){
		connect(m_roateModeList[i], &FImageButton::Clicked, this, [&](int id){
			emit sigRotateChanged((RotateMode)id);
		});
	}
}


/*FPropertyCheckBox*/
FPropertyCheckBox::FPropertyCheckBox(QWidget *parent)
	: FBaseWidget(parent)
{
	m_pPropertyLbl = new FLabel(this);
	m_pCheckBox = new FCheckBox(this);
	m_pPropertyLbl->setText("Property");
	connect(m_pCheckBox, &FCheckBox::stateChanged, this, &FPropertyCheckBox::sigStateChanged);
}

FPropertyCheckBox::~FPropertyCheckBox()
{

}

void FPropertyCheckBox::SetPropertyName(const QString& name)
{
	m_pPropertyLbl->setText(name);
}

void FPropertyCheckBox::SetCheckBoxString(const QString& str)
{
	m_pCheckBox->setText(str);
}

void FPropertyCheckBox::SetCheckState(bool checked)
{
	checked ? m_pCheckBox->setCheckState(Qt::Checked) : m_pCheckBox->setCheckState(Qt::Unchecked);
}

bool FPropertyCheckBox::GetCheckState()
{
	if (m_pCheckBox->isChecked()) return true;
	return false;
}

void FPropertyCheckBox::resizeEvent(QResizeEvent *)
{
	int left, top;
	top = 0;
	const int CheckBoxWidth = 240;
	const int PropertyLblWidth = 80;
	left = width() * 0.5 - CheckBoxWidth / 2;
	if (left < PropertyLblWidth) left = PropertyLblWidth;
	m_pCheckBox->setGeometry(left, top, CheckBoxWidth, height());
	left = m_pCheckBox->geometry().left() - width() * 0.08 - PropertyLblWidth;
	if (left < 0) left = 0;
	m_pPropertyLbl->setGeometry(left, top, PropertyLblWidth, height());
}

/*BottomButtonBar*/
BottomButtonBar::BottomButtonBar(QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
{
	m_pResetBtn = new FButton(this);
	m_pAdvancedBtn = new FButton(this);
	m_pOkBtn = new FButton(this);
	setMinimumSize(240, 30);
	ConnectSignals();
	RetranslateUi();
}

void BottomButtonBar::HideBtnByID(Button_ID id)
{
	switch (id)
	{
	case BottomButtonBar::Reset_Btn: m_pResetBtn->hide();
		break;
	case BottomButtonBar::Advanced_Btn: m_pAdvancedBtn->hide();
		break;
	case BottomButtonBar::Ok_Btn: m_pOkBtn->hide();
		break;
	default:
		break;
	}
}

void BottomButtonBar::ShowBtnByID(Button_ID id)
{
	switch (id)
	{
	case BottomButtonBar::Reset_Btn: m_pResetBtn->show();
		break;
	case BottomButtonBar::Advanced_Btn: m_pAdvancedBtn->show();
		break;
	case BottomButtonBar::Ok_Btn: m_pOkBtn->show();
		break;
	default:
		break;
	}
}

void BottomButtonBar::SetEnabledByID(Button_ID id, bool enabled)
{
	switch (id)
	{
	case BottomButtonBar::Reset_Btn: m_pResetBtn->setEnabled(enabled);
		break;
	case BottomButtonBar::Advanced_Btn: m_pAdvancedBtn->setEnabled(enabled);
		break;
	case BottomButtonBar::Ok_Btn: m_pOkBtn->setEnabled(enabled);
		break;
	default:
		break;
	}
}

void BottomButtonBar::RetranslateUi()
{
	m_pResetBtn->setText(tr("Reset"));
	m_pAdvancedBtn->setText(tr("Advanced"));
	m_pOkBtn->setText(tr("OK"));
}

void BottomButtonBar::resizeEvent(QResizeEvent *)
{
	int top, left;
	top = left = 0;
	m_pResetBtn->setGeometry(left, top, 100, height());
	m_pOkBtn->setGeometry(width() - 80, top, 80, height());
	left = width() - m_pOkBtn->geometry().width() - 5 - 120;
	m_pAdvancedBtn->setGeometry(left, top, 120, height());
}

void BottomButtonBar::ConnectSignals()
{
	connect(m_pResetBtn, &FButton::clicked, [&](){
		emit sigClicked(Reset_Btn);
	});
	connect(m_pAdvancedBtn, &FButton::clicked, [&](){
		emit sigClicked(Advanced_Btn);
	});
	connect(m_pOkBtn, &FButton::clicked, [&](){
		emit sigClicked(Ok_Btn);
	});
}

/*FPropertyButton*/
FPropertyButton::FPropertyButton(QWidget *parent /*= 0*/)
	: FBaseWidget(parent)
{
	m_pPropertyLbl = new FLabel(this);
	m_pTextBtn = new FButton(this);
	m_pImageBtn = new FImageButton("FPropertyButton_FImageBtn", this);

	connect(m_pTextBtn, &FButton::clicked, this, &FPropertyButton::sigTextButtonClicked);
	connect(m_pImageBtn, &FImageButton::clicked, this, &FPropertyButton::sigImageButtonClicked);
}

FPropertyButton::~FPropertyButton()
{

}

void FPropertyButton::SetPropertyName(const QString& name)
{
	m_pPropertyLbl->setText(name);
}

void FPropertyButton::SetButtonText(const QString& text)
{
	m_pTextBtn->setText(text);
}

QString FPropertyButton::GetButtonText()
{
	return m_pTextBtn->text();
}

void FPropertyButton::resizeEvent(QResizeEvent *)
{
	int left, top;
	top = 0;
	const int TextButtonWidth = 240;
	const int PropertyLblWidth = 80;
	const int ImageButtonWidth = 25;
	left = width() * 0.5 - TextButtonWidth / 2;
	if (left < PropertyLblWidth) left = PropertyLblWidth;
	m_pTextBtn->setGeometry(left, top, TextButtonWidth, height());
	left = m_pTextBtn->geometry().left() - width() * 0.08 - PropertyLblWidth;
	if (left < 0) left = 0;
	m_pPropertyLbl->setGeometry(left, top, PropertyLblWidth, height());
	left = m_pTextBtn->geometry().right() + width() * 0.05;
	m_pImageBtn->setGeometry(left, top, ImageButtonWidth, height());
}

/*FNonLinearSlider*/
FNonLinearSlider::FNonLinearSlider(QWidget *parent /*= 0*/) :FBaseWidget(parent)
{

}

FNonLinearSlider::~FNonLinearSlider()
{

}

void FNonLinearSlider::setLabels(const QList<QString> totalLabels)
{
	m_Lables = totalLabels;
	initData();
	initUI();
	repaint();
}

void FNonLinearSlider::mousePosDetect(QMouseEvent * event)
{
	int iPartNum = m_Lables.length();
	float fXpos = event->pos().x();
	for (int i = 0; i < iPartNum; ++i)
	{
		float fTmp = ((float)width() / (float)iPartNum - 1);
		fTmp *= i;
		if (qAbs(fTmp - fXpos) <= 2)
		{
			event->accept();
			QString strLabel = m_Lables.at(i);
			
			emit  sigSelectChanged(strLabel);
		}
		else
		{
			event->ignore();
		}
	}
}

int FNonLinearSlider::getIndex(float fXpos)
{
	float fWidth = m_MainSlider->width();
	float fAccuracy = (m_MainSlider->width()) / (m_Lables.length() - 1) / 2;
	float fRealPos = qBound(0.00f, fXpos, fWidth);
	int iCeilValue = ceil(fRealPos / fAccuracy);
	return floor(iCeilValue / 2);
}

void FNonLinearSlider::resizeEvent(QResizeEvent *event)
{
	int iHeight = height();
	int iWidth = width();
	m_MainSlider->setGeometry(0, 0, width() - m_iReservedLength, 30);
	QList<QLabel*>::iterator iter = m_AllLabels.begin();
	for (; iter != m_AllLabels.end(); ++iter)
	{
		int iIndex = iter - m_AllLabels.begin();
		float fSliderWidth = m_MainSlider->width();
		float iSpace = fSliderWidth / (float)(m_AllLabels.length() - 1);
		int iWidth = iSpace / 2;
		int iHeight = height();
		(*iter)->setGeometry(iIndex * iSpace, height() - 20, iSpace, 20);
		if (!m_bIsShowLables)
		{
			(*iter)->hide();
		}
	}
}

void FNonLinearSlider::initUI()
{
	m_MainSlider = new ToolBarSlider(this);

	QString strObjectName = m_MainSlider->objectName();

	//m_MainSlider->setObjectName("FNonLinearSlider_m_MainSlider");
	m_MainSlider->setDuration(m_Lables.length() - 1);
	m_MainSlider->installEventFilter(this);
	QList<QString>::iterator iter = m_Lables.begin();
	for (; iter != m_Lables.end(); ++iter)
	{
		int iIndex = iter - m_Lables.begin();
		int iSpace = width() / m_Lables.length();
		QLabel* label = new QLabel(this);
		label->setText(*iter);
		m_AllLabels.push_back(label);
	}
}

void FNonLinearSlider::initData()
{
	m_iReservedLength = 40; //默认设置右侧的预留长度为40
}

bool FNonLinearSlider::eventFilter(QObject *target, QEvent *e)
{
	//如果当前控件被禁用，不过滤消息
	if (!isEnabled())
	{
		return false;
	}
	if (target == m_MainSlider  &&  target->objectName() == "CommonLib_ToolBarSlider")
	{
		if (e->type() == QEvent::MouseButtonPress ||
			e->type() == QEvent::MouseMove ||
			e->type() == QEvent::MouseButtonRelease ||
			e->type() == QEvent::MouseButtonDblClick)
		{
			float fXpos = ((QMouseEvent*)e)->pos().x();
			qDebug() << "@@@@@@@@@  xpos is: " << fXpos;
			int iIndex = getIndex(fXpos);
			m_MainSlider->setValue(iIndex);
			emit sigSelectChanged(m_Lables.at(iIndex));
			m_strCurValue = m_Lables.at(iIndex);
			qDebug() << "@@@@@@@@@  value is: " << m_strCurValue;
			return true;
		}
	}
	return	FBaseWidget::eventFilter(target, e);
}

/*FFiniteValueSlider*/
FFiniteValueSlider::FFiniteValueSlider(QWidget *parent /*= nullptr*/)
	:FBaseWidget(parent)
	, m_CurValue(0)
{
	m_pSlider = new ToolBarSlider(this);
	m_pSlider->setDuration(1000);
	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->addWidget(m_pSlider);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0,0,0,0);
	m_pSlider->installEventFilter(this);
	connect(m_pSlider, &ToolBarSlider::valueChanged, this, &FFiniteValueSlider::slotValueChanged);
	connect(m_pSlider, &ToolBarSlider::startDrag, this, &FFiniteValueSlider::sigStartDrag);
	connect(m_pSlider, &ToolBarSlider::endDrag, this, &FFiniteValueSlider::sigEndDrag);
}

FFiniteValueSlider::~FFiniteValueSlider()
{

}

void FFiniteValueSlider::SetFiniteValues(const QList<double>& values)
{
	m_Values = values;
	ResetInnerValues();
}

void FFiniteValueSlider::SetValue(double value)
{
	int index = 0;
	double gap = qFabs(value - m_Values[0]);
	for (int i = 0; i < m_Values.count(); ++i){
		double g = qFabs(value - m_Values[i]);
		if (g < gap){
			gap = g;
			index = i;
		}
	}
	m_CurValue = m_Values[index];
	m_pSlider->setValue(m_InnerValues[index]);
	emit sigValueChanged(m_CurValue);
}

double FFiniteValueSlider::GetValue()
{
	return m_CurValue;
}

void FFiniteValueSlider::SetSliderObjectName(const QString& name)
{
	m_pSlider->setObjectName(name);
}

bool FFiniteValueSlider::eventFilter(QObject *obj, QEvent *e)
{
	if ((obj == m_pSlider) && 
		((e->type() == QEvent::MouseButtonPress)
		|| (e->type() == QEvent::MouseButtonRelease))){
		QMouseEvent *me = (QMouseEvent*)e;
		QPoint pt = me->pos();
		double value = 1.0 * (pt.x() - m_pSlider->rect().left()) / m_pSlider->rect().width() * 1000;
		slotValueChanged(value);
		return true;
	}
	return FBaseWidget::eventFilter(obj, e);
}

void FFiniteValueSlider::ResetInnerValues()
{
	m_InnerValues.clear();
	double step = 1000.0 / (m_Values.count() - 1);
	double v = 0;
	for (int i = 0; i < m_Values.count(); ++i){
		m_InnerValues.append(v);
		v += step;
	}
	m_InnerValues[m_InnerValues.count() - 1] = 1000;
}

void FFiniteValueSlider::slotValueChanged(double value)
{
	int index = 0;
	for (index = 0; index < m_InnerValues.count(); ++index){
		if (value < m_InnerValues[index]) break;
	}
	index = qBound(1, index, m_InnerValues.count() - 1);
	double leftGap = value - m_InnerValues[index - 1];
	double rightGap = m_InnerValues[index] - value;
	if (leftGap < rightGap){
		m_pSlider->setValue(m_InnerValues[index - 1]);
		m_CurValue = m_Values[index - 1];
		emit sigValueChanged(m_Values[index - 1]);
	}
	else{
		m_pSlider->setValue(m_InnerValues[index]);
		m_CurValue = m_Values[index];
		emit sigValueChanged(m_Values[index]);
	}
}


/*FFiniteValuePropertySlider*/
FFiniteValuePropertySlider::FFiniteValuePropertySlider(QWidget* parent /*= nullptr*/)
	:FBaseWidget(parent)
	, m_DecimalPlaces(0)
	, m_Suffix("")
	, m_PropertyName("Property")
	, m_PropertyWidth(80)
	, m_SliderWidth(240)
	, m_ValueWidth(60)
{
	m_pSlider = new FFiniteValueSlider(this);
	m_pPropertyLbl = new FLabel(this);
	m_pValueLbl = new FLabel(this);
	connect(m_pSlider, &FFiniteValueSlider::sigValueChanged, this, [&](double value){
		emit sigValueChanged(value);
		QString text;
		text = QString::number(value, 'f', m_DecimalPlaces);
		text += m_Suffix;
		m_pValueLbl->setText(text);
	});
}

FFiniteValuePropertySlider::~FFiniteValuePropertySlider()
{

}

void FFiniteValuePropertySlider::SetFiniteValues(const QList<double>& values)
{
	m_pSlider->SetFiniteValues(values);
}

void FFiniteValuePropertySlider::resizeEvent(QResizeEvent *)
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

void FFiniteValuePropertySlider::SetValue(double value)
{
	m_pSlider->SetValue(value);
}

double FFiniteValuePropertySlider::GetValue()
{
	return m_pSlider->GetValue();
}

void FFiniteValuePropertySlider::SetAlignmentOfProperty(Qt::Alignment align)
{
	m_pPropertyLbl->setAlignment(align);
}

void FFiniteValuePropertySlider::SetPropertyWidth(int w)
{
	m_PropertyWidth = w;
	resizeEvent(nullptr);
}

void FFiniteValuePropertySlider::SetAlignmentOfValue(Qt::Alignment align)
{
	m_pValueLbl->setAlignment(align);
}

void FFiniteValuePropertySlider::SetSliderWidth(int w)
{
	m_SliderWidth = w;
	resizeEvent(nullptr);
}

void FFiniteValuePropertySlider::SetValueWidth(int w)
{
	m_ValueWidth = w;
	resizeEvent(nullptr);
}

void FFiniteValuePropertySlider::SetPropertyName(const QString& name)
{
	m_pPropertyLbl->setText(name);
}

QString FFiniteValuePropertySlider::GetPropertyName()
{
	return m_pPropertyLbl->text();
}

void FFiniteValuePropertySlider::SetDecimalPlaces(int decimalPlaces)
{
	m_DecimalPlaces = qMax(0, decimalPlaces);
}

void FFiniteValuePropertySlider::SetSuffix(const QString& suffix)
{
	m_Suffix = suffix;
}

void FFiniteValuePropertySlider::SetSliderObjectName(const QString& name)
{
	m_pSlider->SetSliderObjectName(name);
}
