#include "stdafx.h"
#include "FAudioProperPanel.h"

#include "inc_FProject/FNLETimeline.h"

FAudioProperPanel::FAudioProperPanel(QWidget *parent)
	: FPropertyPanel(parent)
{
	initUI();
	initConnections();
}

FAudioProperPanel::~FAudioProperPanel()
{

}

void FAudioProperPanel::Init()
{

}

void FAudioProperPanel::Show()
{

}

void FAudioProperPanel::Hide()
{

}

void FAudioProperPanel::SetEnabled(bool enabled)
{

}

void FAudioProperPanel::Destory()
{

}

void FAudioProperPanel::Update()
{

}

void FAudioProperPanel::slotVolumeChanged(int iVolume)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kVolume, iVolume);
}

void FAudioProperPanel::slotFadeInChanged(float fValue)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetDouble(NLEKey::PostProcess::kAudioFadeInTime, fValue);
}

void FAudioProperPanel::slotFadeOutChanged(float fValue)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetDouble(NLEKey::PostProcess::kAudioFadeOutTime, fValue);
}

void FAudioProperPanel::slotPitchChanged(int iValue)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::Render::kRender_AudioPitch, iValue);
}

void FAudioProperPanel::slotConfirmConfig()
{

}

void FAudioProperPanel::slotSpeedChanged(float fValue)
{
	//设置属性前线暂停播放
	emit sigPausePlay();
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	pTimeline->SetRenderScale(1 / fValue);
	emit sigRefreshTimelinePro();
}

void FAudioProperPanel::slotEqualizerChanged(QString strType)
{
	//暂时不支持
}

void FAudioProperPanel::slotDenoiseChnaged(QString strType)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	std::wstring  wstrDenoiseType = strType.toStdWString();
	pProp->SetString(NLEKey::PreProcess::kPreProcess_DenoiseStrength, wstrDenoiseType.c_str());
}

void FAudioProperPanel::resizeEvent(QResizeEvent *e)
{
	return FBaseWidget::resizeEvent(e);
}

void FAudioProperPanel::initUI()
{
	m_SpeedCtl = new FSpeedCtl(nullptr);
	m_SpeedCtl->setMinimumHeight(60);
	m_SpeedCtl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_VolumSlider = new FPropertySlider(nullptr);
	m_VolumSlider->SetRange(0,200);
	m_VolumSlider->SetPropertyName(tr("Volume:"));
	m_VolumSlider->setMinimumHeight(20);
	m_VolumSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);


	m_FadeinSlider = new FPropertySlider(nullptr);
	m_FadeinSlider->SetPropertyName(tr("Fade in:"));
	//m_FadeinSlider->SetAlignmentOfProperty(Qt::AlignRight);
	m_FadeinSlider->SetRange(0.00,5.00);
	m_FadeinSlider->SetSuffix("s");
	m_FadeinSlider->SetDecimalPlaces(2);
	m_FadeinSlider->setMinimumHeight(20);
	m_FadeinSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_FadeoutSlider = new FPropertySlider(nullptr);
	m_FadeoutSlider->SetPropertyName(tr("Fade out:"));
	m_FadeoutSlider->SetRange(0.00, 5.00);
	m_FadeoutSlider->SetDecimalPlaces(2);
	m_FadeoutSlider->SetSuffix("s");
	m_FadeoutSlider->setMinimumHeight(20);
	//m_FadeoutSlider->SetAlignmentOfProperty(Qt::AlignRight);
	m_FadeoutSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);


	m_PitchSlider = new FPropertySlider(nullptr);
	m_PitchSlider->SetPropertyName(tr("Pitch:"));
	m_PitchSlider->SetRange(-12,12);
	m_PitchSlider->setMinimumHeight(20);
	m_PitchSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_EqualizerCtl = new FEqualizerCtl(nullptr);
	m_EqualizerCtl->setMinimumHeight(30);
	m_EqualizerCtl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

	m_DenoiseCtl = new FDenoiseCtl(nullptr);
	m_DenoiseCtl->setMinimumHeight(70);
	m_DenoiseCtl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

	FBaseWidget*  emptyWidget = new FBaseWidget(nullptr); //空白widget做占位
	//设置透明背景属性
	emptyWidget->setStyleSheet("background-color: transparent");

	m_BottomBar = new BottomButtonBar(nullptr);
	m_BottomBar->HideBtnByID(BottomButtonBar::Advanced_Btn);
	m_BottomBar->setFixedHeight(30);
	m_BottomBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	//使用布局布置界面
	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(emptyWidget);
	vLayout->addWidget(m_SpeedCtl);
	vLayout->addWidget(m_VolumSlider);
	vLayout->addWidget(m_FadeinSlider);
	vLayout->addWidget(m_FadeoutSlider);
	vLayout->addWidget(m_PitchSlider);
	vLayout->addWidget(m_EqualizerCtl);
	vLayout->addWidget(m_DenoiseCtl);
	vLayout->addWidget(emptyWidget);
	vLayout->addWidget(m_BottomBar);
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->setSpacing(0);
	setLayout(vLayout);
}

void FAudioProperPanel::initConnections()
{
	connect(m_SpeedCtl, &FSpeedCtl::sigSpeedChanged, this, &FAudioProperPanel::slotSpeedChanged);
	connect(m_VolumSlider, &FPropertySlider::sigValueChanged, this, &FAudioProperPanel::slotVolumeChanged);
	connect(m_FadeinSlider, &FPropertySlider::sigValueChanged, this, &FAudioProperPanel::slotFadeInChanged);
	connect(m_FadeoutSlider, &FPropertySlider::sigValueChanged, this, &FAudioProperPanel::slotFadeOutChanged);
	connect(m_PitchSlider, &FPropertySlider::sigValueChanged, this, &FAudioProperPanel::slotPitchChanged);
	connect(m_EqualizerCtl, &FEqualizerCtl::sigEqualizerChanged, this, &FAudioProperPanel::slotEqualizerChanged);
	connect(m_DenoiseCtl, &FDenoiseCtl::sigDenoiseTypeChanged, this, &FAudioProperPanel::slotDenoiseChnaged);
}


FEqualizerCtl::FEqualizerCtl(QWidget *parent) : FBaseWidget(parent)
{
	initUI();
	initData();
}

void FEqualizerCtl::resizeEvent(QResizeEvent *e)
{
	int left, top;
	top = 0;
	const int iSliderWidth = 240;
	const int iPropertyLblWidth = 80;
	const int iValueLblWidth = 60;
	const int iEqualizerHeight = 25;
	const int iBtnWidth = 70;
	left = width() * 0.5 - iSliderWidth / 2;
	if (left < iPropertyLblWidth) left = iPropertyLblWidth;
	m_EqualizerCombo->setGeometry(left, top, iSliderWidth, 25);
	left = m_EqualizerCombo->geometry().left() - width() * 0.08 - iPropertyLblWidth;
	if (left < 0) left = 0;
	m_EqualizerLabel->setGeometry(left, top, iPropertyLblWidth, iEqualizerHeight);
	left = m_EqualizerCombo->geometry().right() + width() * 0.05;
	m_CustomizeBtn->setGeometry(left, top, iBtnWidth, iEqualizerHeight);

}

void FEqualizerCtl::initUI()
{
	m_EqualizerLabel = new FLabel(this);
	m_EqualizerLabel->setText(tr("Equalizer:"));
	m_EqualizerCombo = new FComboBox(this);
	m_CustomizeBtn = new FButton(this);
	m_CustomizeBtn->setText(tr("Customize"));
}

void FEqualizerCtl::initData()
{
	 
}

FDenoiseCtl::FDenoiseCtl(QWidget *parent) :FBaseWidget(parent)
{
	initUI();
	initData();
	initConnections();
}

void FDenoiseCtl::resizeEvent(QResizeEvent *e)
{
	int left, top;
	top = 0;
	const int iSliderWidth = 240;
	const int iPropertyLblWidth = 80;
	const int iValueLblWidth = 60;
	const int iEqualizerHeight = 25;
	const int iDenoiseSliderheight = 50;
	left = width() * 0.5 - iSliderWidth / 2;
	if (left < iPropertyLblWidth) left = iPropertyLblWidth;
	m_DenoiseCheckBox->setGeometry(left, top, iSliderWidth, 25);
	left = m_DenoiseCheckBox->geometry().left() - width() * 0.08 - iPropertyLblWidth;
	if (left < 0) left = 0;
	m_DenoiseLabel->setGeometry(left, top, iPropertyLblWidth, iEqualizerHeight);
	left = width() * 0.5 - iSliderWidth / 2;
	int iReserLength = m_DenoiseSlider->getReservedLength();
	m_DenoiseSlider->setGeometry(left, height() - iDenoiseSliderheight, iSliderWidth + iReserLength, iDenoiseSliderheight);
}

void FDenoiseCtl::initUI()
{
	m_DenoiseLabel = new FLabel(this);
	m_DenoiseLabel->setText(tr("Denoise:"));
	m_DenoiseCheckBox = new FCheckBox(this);
	m_DenoiseCheckBox->setText(tr("Remove background noise"));
	m_DenoiseCheckBox->setCheckState(Qt::Unchecked);
	m_DenoiseSlider = new FNonLinearSlider(this);
	QList<QString> totalLabels;// = { "Week", "Mid", "Strong" };
	totalLabels << "Week" << "Mid" << "Strong";
	m_DenoiseSlider->setLabels(totalLabels);
	m_DenoiseSlider->setEnabled(false);
}

void FDenoiseCtl::initData()
{
	m_bEnable = false;
}

void FDenoiseCtl::initConnections()
{
	connect(m_DenoiseCheckBox, &FCheckBox::stateChanged, this, [&](int iState){
		if (iState == Qt::Unchecked)
		{
			m_bEnable = false;
			m_DenoiseSlider->setEnabled(false);
		}
		else if (iState == Qt::Checked)
		{
			m_bEnable = true;
			m_DenoiseSlider->setEnabled(true);
		}
	});
}


FSpeedCtl::FSpeedCtl(QWidget *parent) :FBaseWidget(parent)
{
	initUI();
	initData();
	InitConnections();
}

void FSpeedCtl::resizeEvent(QResizeEvent *e)
{
	int left, top;
	top = 0;
	const int iSliderWidth = 240;
	const int iPropertyLblWidth = 80;
	const int iValueLblWidth = 60;
	const int iEqualizerHeight = 25;
	const int iSpeedSliderHeight = 50;
	left = width() * 0.5 - iSliderWidth / 2;
	if (left < iPropertyLblWidth) left = iPropertyLblWidth;
	int iReservedLength = m_SpeedSlider->getReservedLength();
	m_SpeedSlider->setGeometry(left, top, iSliderWidth + iReservedLength, iSpeedSliderHeight);
	left = m_SpeedSlider->geometry().left() - width() * 0.08 - iPropertyLblWidth;
	if (left < 0) left = 0;
	m_SpeedLabel->setGeometry(left, top, iPropertyLblWidth, iEqualizerHeight);
}

void FSpeedCtl::initUI()
{
	m_SpeedLabel = new FLabel(this);
	m_SpeedLabel->setText(tr("Speed:"));
	m_SpeedSlider = new FNonLinearSlider(this);
	QList<QString> totalLabels;// = { "0.5", "1", "1.5", "2" };
	totalLabels << "0.5" << "1" << "1.5" << "2";
	m_SpeedSlider->setLabels(totalLabels);
}

void FSpeedCtl::initData()
{

}

void FSpeedCtl::InitConnections()
{
	connect(m_SpeedSlider, &FNonLinearSlider::sigSelectChanged, this, [&](QString strValue){
		float fValue = strValue.toFloat();
		emit sigSpeedChanged(fValue);
	});

}
