#include "stdafx.h"
#include "FFoldWidgetItemPrivate.h"
#include "inc_FCommonLib\FCommonWidgets.h"
#include <QBoxLayout>
#include <QGridLayout>
#include "FCollapseHeaderBase.h"
#include "F3DLUTDataMgr.h"

FFoldWidgetItemPrivate::FFoldWidgetItemPrivate(QWidget *parent)
	: FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FFoldWidgetItemPrivate::~FFoldWidgetItemPrivate()
{

}

void FFoldWidgetItemPrivate::SetContainsWidget(FContexBaseWidget* pWidget)
{
	if (m_ContainWidget != nullptr)
	{
		delete m_ContainWidget;
		m_ContainWidget = pWidget;
	}
	InitConnections();
}

void FFoldWidgetItemPrivate::SetCustomHeader(FCollapseHeaderBase* pHeaderWidegt)
{
	if (m_Header != nullptr)
	{
		delete m_Header;
		m_Header = pHeaderWidegt;
	}
	InitConnections();
}

void FFoldWidgetItemPrivate::HideContainWidget()
{
	int iHeight = m_Header->height();
	resize(width(), iHeight);
	m_Header->SetCurState(false);
}

void FFoldWidgetItemPrivate::ShowContainWidget()
{
	m_ContainWidget->show();
}

void FFoldWidgetItemPrivate::SetCheckBoxName(QString strName)
{
	FHeaderWidget* header = qobject_cast<FHeaderWidget*>(m_Header);
	if (header != 0)
	{
		header->SetCheckBoxName(strName);
	}
}

void FFoldWidgetItemPrivate::SetHeaderTitleName(QString strTitleName)
{
	((FDefaultHeaderWidget*)m_Header)->setTitleName(strTitleName);
}

void FFoldWidgetItemPrivate::SetHeaderHeight(int iHeight)
{
	m_Header->setFixedHeight(iHeight);
}

void FFoldWidgetItemPrivate::slotOnCollaspse(bool bIsCollapse)
{
	if (!bIsCollapse)
	{
		if (m_CurMode == Mode_ShowSingle)
		{
			return;
		}
		int iHeight = m_Header->height();
		resize(width(), iHeight);
		emit sigResize(width(), iHeight);
		//m_ContainWidget->hide();
	}
	else
	{
		//m_ContainWidget->show();
		int iContainHeight = m_ContainWidget->height();
		int iHeaderHeight = m_Header->height();
		iContainHeight += iHeaderHeight;
		resize(width(), iContainHeight);
		emit sigResize(width(), iContainHeight);
	}
	emit sigOnCollapse(bIsCollapse);
}

void FFoldWidgetItemPrivate::resizeEvent(QResizeEvent *e)
{
	m_Header->setGeometry(0, 0, width(), 30);
	int iHeight = height();
	m_ContainWidget->setGeometry(0, 31, width(), height() - 30);
	m_ContainWidget->show();
}

void FFoldWidgetItemPrivate::InitUI()
{
	//如果不做设置则界面为初始值
	m_ContainWidget = new FContexBaseWidget(this);
	m_ContainWidget->setStyleSheet("background-color: red");
	m_Header = new FDefaultHeaderWidget(this);
	m_Header->SetCurState(false);
}

void FFoldWidgetItemPrivate::InitConnections()
{
  disconnect(m_Header, &FHeaderWidget::sigCollapse,0, 0);
  connect(m_Header, &FHeaderWidget::sigCollapse, this, [&](bool bIsShow){
		slotOnCollaspse(bIsShow);
	});
  disconnect(m_Header, &FHeaderWidget::sigRefresh, 0, 0);
  connect(m_Header, &FHeaderWidget::sigRefresh, m_ContainWidget, &FContexBaseWidget::resetConfig);
}

FHeaderWidget::FHeaderWidget(QWidget *parent) :FCollapseHeaderBase(parent)
{
	InitUI();
	InitData();
	InitConnections();
}

FHeaderWidget::~FHeaderWidget()
{

}

void FHeaderWidget::SetCheckBoxName(QString strName)
{
	m_HeaderCheckBox->setText(strName);
	repaint();
}

void FHeaderWidget::InitUI()
{
	m_CollapseBtn = new FButton(this);
	m_CollapseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_CollapseBtn->setFixedHeight(20);
	m_CollapseBtn->setFixedWidth(20);
	m_HeaderCheckBox = new FCheckBox(this);
	m_HeaderCheckBox->setChecked(false);
	m_RefreshBtn = new FButton(this);
	FBaseWidget* emptyWidget = new FBaseWidget(nullptr);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(m_CollapseBtn);
	hLayout->addWidget(m_HeaderCheckBox);
	hLayout->addWidget(emptyWidget);
	hLayout->addWidget(m_RefreshBtn);
	m_RefreshBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_RefreshBtn->setFixedHeight(20);
	m_RefreshBtn->setFixedWidth(20);
	hLayout->setContentsMargins(5, 0, 5, 0);
	setLayout(hLayout);
}

void FHeaderWidget::InitData()
{
	SetCurState(false);
}

void FHeaderWidget::InitConnections()
{
	connect(m_CollapseBtn, &FButton::clicked, this, [&](){
		bool bCurState = GetCurState();
		emit sigCollapse(!bCurState);
		qDebug() << "FHeaderWidget m_CollapseBtn  emit";
		SetCurState(!bCurState);
	});
}

FWhiteBanlacePannel::FWhiteBanlacePannel(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

void FWhiteBanlacePannel::InitUI()
{
	const QColor qBuleColor(9, 99, 24);
	const QColor qYellowColor(244, 173, 12);
	const QColor qGreenColor(1, 253, 23);
	const QColor qPurpleColor(158, 46, 202);
	m_TintRuler = new FRulerWidget(this, qBuleColor, qYellowColor);
	m_TintRuler->setLabelText(tr("Tint"));
	//使用固定大小策略 
	m_TintRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_TintRuler->setFixedHeight(30);
	m_TemperatureRuler = new FRulerWidget(this, qGreenColor, qPurpleColor);
	m_TemperatureRuler->setLabelText(tr("Temperature"));
	m_TemperatureRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_TemperatureRuler->setFixedHeight(30);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_TemperatureRuler);
	vLayout->addWidget(m_TintRuler);
	vLayout->setContentsMargins(15, 13, 25, 25);
	vLayout->setSpacing(15);
	setLayout(vLayout);
	setMinimumHeight(115);
	//setFixedSize(260, 115);
}

void FWhiteBanlacePannel::InitConnections()
{
	connect(m_TemperatureRuler, &FRulerWidget::sigValueChanged, this, &FWhiteBanlacePannel::sigOnTempratuerChange);
	connect(m_TintRuler, &FRulerWidget::sigValueChanged, this, &FWhiteBanlacePannel::sigOnTintChange);
}

void FWhiteBanlacePannel::ResetConfig()
{

}

FDefaultHeaderWidget::FDefaultHeaderWidget(QWidget *parent) :FCollapseHeaderBase(parent)
{
	InitUI();
	InitData();
	InitConnections();
}

void FDefaultHeaderWidget::InitUI()
{
	setObjectName("FDefaultHeaderWidgetID");
	const int iHeight = 25;
	m_CollapseBtn = new FButton(nullptr);
	m_CollapseBtn->setObjectName("FDefaultHeaderWidget_mCollapseBtn");
	m_TitleName = new FLabel(nullptr);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(m_TitleName);
	FBaseWidget* emptyWidget = new FBaseWidget(nullptr);
	hLayout->addWidget(emptyWidget);
	hLayout->addWidget(m_CollapseBtn);
	m_TitleName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_CollapseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_TitleName->setFixedHeight(iHeight);
	m_CollapseBtn->setFixedHeight(iHeight);
	hLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(hLayout);
}

void FDefaultHeaderWidget::InitData()
{
	SetCurState(false);
	m_TitleName->setText("");
}

void FDefaultHeaderWidget::InitConnections()
{
	connect(m_CollapseBtn, &FButton::clicked, this, [&](){
		bool bCurState = GetCurState();
		emit sigCollapse(!bCurState);
		qDebug() << "FDefaultHeaderWidget m_CollapseBtn  emit";
		SetCurState(!bCurState);
	});
}

F3DLUTPannel::F3DLUTPannel(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitData();
	InitConnectios();
}

void F3DLUTPannel::InitUI()
{
	m_Combo = new FComboBox(nullptr);
	m_RevertBtn = new FButton(nullptr);
	m_RevertBtn->setFixedSize(QSize(25,25));
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(m_Combo);
	hLayout->addWidget(m_RevertBtn);
	hLayout->setContentsMargins(30, 20, 25, 15);
	hLayout->setSpacing(10);
	setLayout(hLayout);
	setFixedHeight(75);
}

void F3DLUTPannel::ResetConfig()
{

}

void F3DLUTPannel::InitConnectios()
{
	connect(m_RevertBtn, &FButton::clicked, this, [&](){
		m_Combo->setEditText(tr("None"));
	});

	connect(m_Combo, &FComboBox::currentTextChanged, this, &F3DLUTPannel::sigOnLUTChange);
}

void F3DLUTPannel::InitData()
{
	m_DataMgr = new F3DLUTDataMgr(nullptr);
	m_DataToLoad = m_DataMgr->GetDataToLoad();
}

FColorAdjustWidget::FColorAdjustWidget(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitData();
	InitConnectios();
}

void FColorAdjustWidget::InitUI()
{
	const int iFixHeight = 30;
	m_ExposureRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(190, 190, 190));
	m_ExposureRuler->setLabelText(tr("Exposure"));
	m_ExposureRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_ExposureRuler->setFixedHeight(iFixHeight);
	m_BrightnessRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(190, 190, 190));
	m_BrightnessRuler->setLabelText(tr("Brightness"));
	m_BrightnessRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_BrightnessRuler->setFixedHeight(iFixHeight);

	m_ContrastRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(190, 190, 190));
	m_ContrastRuler->setLabelText(tr("Contrast"));
	m_ContrastRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_ContrastRuler->setFixedHeight(iFixHeight);

	m_VibranceRuler = new FRulerWidget(nullptr, QColor(162, 163, 164), QColor(251, 108, 5));
	m_VibranceRuler->setLabelText(tr("Vibrance"));
	m_VibranceRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_VibranceRuler->setFixedHeight(iFixHeight);

	m_SaturationRuler = new FRulerWidget(nullptr, QColor(162, 163, 164), QColor(251, 108, 5));
	m_SaturationRuler->setLabelText(tr("Stauration"));
	m_SaturationRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_SaturationRuler->setFixedHeight(iFixHeight);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_ExposureRuler);
	vLayout->addWidget(m_BrightnessRuler);
	vLayout->addWidget(m_ContrastRuler);
	vLayout->addWidget(m_VibranceRuler);
	vLayout->addWidget(m_SaturationRuler);
	vLayout->setContentsMargins(35, 20, 25, 15);
	vLayout->setSpacing(10);
	setLayout(vLayout);
	setMinimumHeight(230);
}

void FColorAdjustWidget::ResetConfig()
{

}

void FColorAdjustWidget::InitConnectios()
{
	connect(m_ExposureRuler, &FRulerWidget::sigValueChanged, this, &FColorAdjustWidget::sigOnColorExposureChange);
	connect(m_BrightnessRuler, &FRulerWidget::sigValueChanged, this, &FColorAdjustWidget::sigOnColorBrightChange);
	connect(m_ContrastRuler, &FRulerWidget::sigValueChanged, this, &FColorAdjustWidget::sigOnColorContrastChange);
	connect(m_VibranceRuler, &FRulerWidget::sigValueChanged, this, &FColorAdjustWidget::sigOnColorVibranceChange);
	connect(m_SaturationRuler, &FRulerWidget::sigValueChanged, this, &FColorAdjustWidget::sigOnColorSaturationChange);
}

void FColorAdjustWidget::InitData()
{

}

FLightAdjustPannel::FLightAdjustPannel(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitData();
	InitConnectios();
}

void FLightAdjustPannel::InitUI()
{
	const int iFixHeight = 30;
	m_Highlights = new FRulerWidget(nullptr, QColor(147, 147, 147), QColor(201, 201, 201));
	m_Highlights->setLabelText(tr("Highlights"));
	m_Highlights->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_Highlights->setFixedHeight(iFixHeight);

	m_Shadows = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(190, 190, 190));
	m_Shadows->setLabelText(tr("Shadows"));
	m_Shadows->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_Shadows->setFixedHeight(iFixHeight);

	m_Whites = new FRulerWidget(nullptr, QColor(147, 147, 147), QColor(201, 201, 201));
	m_Whites->setLabelText(tr("Whites"));
	m_Whites->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_Whites->setFixedHeight(iFixHeight);

	m_Blacks = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(190, 190, 190));
	m_Blacks->setLabelText(tr("Blacks"));
	m_Blacks->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_Blacks->setFixedHeight(iFixHeight);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_Highlights);
	vLayout->addWidget(m_Shadows);
	vLayout->addWidget(m_Whites);
	vLayout->addWidget(m_Blacks);
	vLayout->setContentsMargins(35, 20, 25, 15);
	vLayout->setSpacing(10);
	setLayout(vLayout);
	setMinimumHeight(195);
}

void FLightAdjustPannel::ResetConfig()
{

}

void FLightAdjustPannel::InitConnectios()
{

}

void FLightAdjustPannel::InitData()
{
	connect(m_Highlights, &FRulerWidget::sigValueChanged, this, &FLightAdjustPannel::sigOnLightHighlightsChange);
	connect(m_Shadows, &FRulerWidget::sigValueChanged, this, &FLightAdjustPannel::sigOnLightShadowsChange);
	connect(m_Whites, &FRulerWidget::sigValueChanged, this, &FLightAdjustPannel::sigOnLightWhitesChange);
	connect(m_Blacks, &FRulerWidget::sigValueChanged, this, &FLightAdjustPannel::sigOnLightBlacksChange);
}


FHslAdjustPannel::FHslAdjustPannel(QWidget *parent) :FContexBaseWidget(parent)
{
	InitData();
	InitUI();
	InitConnectios();
}


void FHslAdjustPannel::slotOnColorChanged(QColor curColor)
{
	if (m_RedColorLabel->GetBackgroundColor() != curColor)
	{
		m_RedColorLabel->SetChecked(false);
	}
	if (m_OrangeColorLabel->GetBackgroundColor() != curColor)
	{
		m_OrangeColorLabel->SetChecked(false);
	}
	if (m_YellowColorLabel->GetBackgroundColor() != curColor)
	{
		m_YellowColorLabel->SetChecked(false);
	}
	if (m_GreenLabel->GetBackgroundColor() != curColor)
	{
		m_GreenLabel->SetChecked(false);
	}
	if (m_MagentaLabel->GetBackgroundColor() != curColor)
	{
		m_MagentaLabel->SetChecked(false);
	}
	if (m_PurpleLabel->GetBackgroundColor() != curColor)
	{
		m_PurpleLabel->SetChecked(false);
	}
	if (m_BlueLabel->GetBackgroundColor() != curColor)
	{
		m_BlueLabel->SetChecked(false);
	}
	if (m_CyanLabel->GetBackgroundColor() != curColor)
	{
		m_CyanLabel->SetChecked(false);
	}
}

void FHslAdjustPannel::InitUI()
{
	const int iRoundDiameter = 30;
	m_RedColorLabel = new FRoundLabel(nullptr);
	m_RedColorLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_RedColorLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_AllColorLables.push_back(m_RedColorLabel);

	m_OrangeColorLabel = new FRoundLabel(nullptr);
	m_OrangeColorLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_OrangeColorLabel->SetBackgroundColor(m_OrangeColor);
	m_OrangeColorLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_AllColorLables.push_back(m_OrangeColorLabel);

	m_YellowColorLabel = new FRoundLabel(nullptr);
	m_YellowColorLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_YellowColorLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_YellowColorLabel->SetBackgroundColor(m_YellowColor);
	m_AllColorLables.push_back(m_YellowColorLabel);

	m_GreenLabel = new FRoundLabel(nullptr);
	m_GreenLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_GreenLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_GreenLabel->SetBackgroundColor(m_GreenColor);
	m_AllColorLables.push_back(m_GreenLabel);

	m_MagentaLabel = new FRoundLabel(nullptr);
	m_MagentaLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_MagentaLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_MagentaLabel->SetBackgroundColor(m_MagentaColor);
	m_AllColorLables.push_back(m_MagentaLabel);

	m_PurpleLabel = new FRoundLabel(nullptr);
	m_PurpleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_PurpleLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_PurpleLabel->SetBackgroundColor(m_PurpleColor);
	m_AllColorLables.push_back(m_PurpleLabel);

	m_BlueLabel = new FRoundLabel(nullptr);
	m_BlueLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_BlueLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_BlueLabel->SetBackgroundColor(m_BlueColor);
	m_AllColorLables.push_back(m_BlueLabel);

	m_CyanLabel = new FRoundLabel(nullptr);
	m_CyanLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_CyanLabel->setFixedSize(QSize(iRoundDiameter, iRoundDiameter));
	m_CyanLabel->SetBackgroundColor(m_CyanColor);
	m_AllColorLables.push_back(m_CyanLabel);

	QGridLayout* gLayout = new QGridLayout();
	gLayout->addWidget(m_RedColorLabel, 1, 1);
	gLayout->addWidget(m_OrangeColorLabel, 1, 2);
	gLayout->addWidget(m_YellowColorLabel, 1, 3);
	gLayout->addWidget(m_GreenLabel, 1, 4);
	gLayout->addWidget(m_MagentaLabel, 2, 1);
	gLayout->addWidget(m_PurpleLabel, 2, 2);
	gLayout->addWidget(m_BlueLabel, 2, 3);
	gLayout->addWidget(m_CyanLabel, 2, 4);
	//gLayout->setContentsMargins(60, 10, 55, 3);
	gLayout->setContentsMargins(25, 0, 53, 8);
	gLayout->setSpacing(20);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addLayout(gLayout);
	const int iFixHeight = 30;
	m_HueRuler = new FRulerWidget(nullptr, QColor(147, 147, 147), QColor(201, 201, 201));
	m_HueRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_HueRuler->setFixedHeight(iFixHeight);
	m_HueRuler->setLabelText(tr("Hue"));

	m_Saturation = new FRulerWidget(nullptr, QColor(147, 147, 147), QColor(201, 201, 201));
	m_Saturation->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_Saturation->setFixedHeight(iFixHeight);
	m_Saturation->setLabelText(tr("Saturation"));

	m_Luminance = new FRulerWidget(nullptr, QColor(147, 147, 147), QColor(201, 201, 201));
	m_Luminance->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_Luminance->setFixedHeight(iFixHeight);
	m_Luminance->setLabelText(tr("Luminance"));

	vLayout->addWidget(m_HueRuler);
	vLayout->addWidget(m_Saturation);
	vLayout->addWidget(m_Luminance);
	vLayout->setContentsMargins(35, 0, 25, 15);
	vLayout->setSpacing(10);
	setLayout(vLayout);
	setFixedHeight(190);
}

void FHslAdjustPannel::ResetConfig()
{

}

void FHslAdjustPannel::InitConnectios()
{
	for each (auto var in m_AllColorLables)
	{
		connect(var, &FRoundLabel::sigOnChecked, this, &FHslAdjustPannel::slotOnColorChanged);
	}
	connect(m_HueRuler, &FRulerWidget::sigValueChanged, this, &FHslAdjustPannel::sigOnHSLHueChange);
	connect(m_Saturation, &FRulerWidget::sigValueChanged, this, &FHslAdjustPannel::sigOnSaturationChange);
	connect(m_Luminance, &FRulerWidget::sigValueChanged, this, &FHslAdjustPannel::sigOnLuminanceChange);
}

void FHslAdjustPannel::InitData()
{
	m_RedColor = QColor(222, 58, 60);
	m_OrangeColor = QColor(221, 140, 60);
	m_YellowColor = QColor(222, 221, 60);
	m_GreenColor = QColor(128, 217, 38);
	m_MagentaColor = QColor(221, 60, 221);
	m_PurpleColor = QColor(153, 81, 225);
	m_BlueColor = QColor(103, 103, 230);
	m_CyanColor = QColor(34, 196, 195);
}

FRoundLabel::FRoundLabel(QWidget *parent, QColor color) :FLabel(parent), 
m_BackgroundColor(color), m_bCurCheckState(false)
{

}

void FRoundLabel::paintEvent(QPaintEvent *e)
{ 
	const int iRingLineSize = 2;
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true); //消除锯齿效果
	painter.setPen(Qt::NoPen);
	//设置画刷
	painter.setBrush(QBrush(m_BackgroundColor, Qt::SolidPattern));
	painter.drawEllipse(QRect(iRingLineSize, iRingLineSize, width() - iRingLineSize * 2, height() - iRingLineSize * 2));
	painter.restore();
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(iRingLineSize);
	pen.setColor(QColor(85, 185, 195));
	painter.setPen(pen);
	//绘制外层包围圆
	if (m_bCurCheckState)
	{
		int iWidth = width();
		int iheight = height();
		painter.drawEllipse(QRect(0, 0, width(), height()));
	}
	painter.restore();
	e->accept();//不再向父类传递消息
}

void FRoundLabel::mousePressEvent(QMouseEvent *event)
{
	SetChecked(true);
	repaint();
	emit sigOnChecked(m_BackgroundColor);
}

FVignettePannel::FVignettePannel(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitData();
	InitConnectios();
}

void FVignettePannel::InitUI()
{
	const int iWidgetHeeight = 30;
	m_AmountRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(186, 186, 186));
	m_AmountRuler->setLabelText(tr("Amount"));
	m_AmountRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_AmountRuler->setFixedHeight(iWidgetHeeight);

	m_SizeRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(186, 186, 186));
	m_SizeRuler->setLabelText(tr("Size"));
	m_SizeRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_SizeRuler->setFixedHeight(iWidgetHeeight);
	m_SizeRuler->setRange(0.00f, 100.00f);

	m_RoundnessRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(186, 186, 186));
	m_RoundnessRuler->setLabelText(tr("Roundness"));
	m_RoundnessRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_RoundnessRuler->setFixedHeight(iWidgetHeeight);
	m_RoundnessRuler->setRange(0.00f, 100.00f);

	m_FeatherRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(186, 186, 186));
	m_FeatherRuler->setLabelText(tr("Feather"));
	m_FeatherRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_FeatherRuler->setFixedHeight(iWidgetHeeight);
	m_FeatherRuler->setRange(0.00f, 100.00f);

	m_ExposureRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(186, 186, 186));
	m_ExposureRuler->setLabelText(tr("Exposure"));
	m_ExposureRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_ExposureRuler->setFixedHeight(iWidgetHeeight);


	m_HighlightsRuler = new FRulerWidget(nullptr, QColor(55, 54, 54), QColor(186, 186, 186));
	m_HighlightsRuler->setLabelText(tr("Highight"));
	m_HighlightsRuler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_HighlightsRuler->setFixedHeight(iWidgetHeeight);
	m_HighlightsRuler->setRange(0.00f, 100.00f);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_AmountRuler);
	vLayout->addWidget(m_SizeRuler);
	vLayout->addWidget(m_RoundnessRuler);
	vLayout->addWidget(m_FeatherRuler);
	vLayout->addWidget(m_ExposureRuler);
	vLayout->addWidget(m_HighlightsRuler);
	vLayout->setContentsMargins(35, 20,25, 15);
	vLayout->setSpacing(10);
	setLayout(vLayout);
	setFixedHeight(250);
}

void FVignettePannel::ResetConfig()
{

}

void FVignettePannel::InitConnectios()
{
	connect(m_AmountRuler, &FRulerWidget::sigValueChanged, this, &FVignettePannel::sigOnVignetteAmountChange);
	connect(m_SizeRuler, &FRulerWidget::sigValueChanged, this, &FVignettePannel::sigOnVignetteSizeChange);
	connect(m_RoundnessRuler, &FRulerWidget::sigValueChanged, this, &FVignettePannel::sigOnVignetteRoundnessChange);
	connect(m_FeatherRuler, &FRulerWidget::sigValueChanged, this, &FVignettePannel::sigOnVignetteFeatherChange);
	connect(m_ExposureRuler, &FRulerWidget::sigValueChanged, this, &FVignettePannel::sigOnVignetteExposureChange);
	connect(m_HighlightsRuler, &FRulerWidget::sigValueChanged, this, &FVignettePannel::sigOnVignetteHiglightsChange);
}

void FVignettePannel::InitData()
{

}
