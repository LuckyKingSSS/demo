#include "stdafx.h"
#include "FTransitionPanel.h"
#include "inc_CommonLib/externalapi.h"
#include <QPainter>

FTransitionPanel::FTransitionPanel(QWidget *parent)
	: FPropertyPanel(parent)
{
	m_pPanelMgr = new FTransitionPanelMgr(this);
	m_pDurationLbl = new FLabel(this);
	m_pSpinBox = new FSpinBox(this);
	m_pApplyToAllBtn = new FButton(this);
	m_pModeLbl = new FLabel(this);
	m_pOverlap = new TransitionModeWidget(TransitionModeWidget::Overlap_Mode, this);
	m_pPrefix = new TransitionModeWidget(TransitionModeWidget::Prefix_Mode, this);
	m_pPostfix = new TransitionModeWidget(TransitionModeWidget::Postfix_Mode, this);
	m_pRadioBtnHintLbl = new FLabel(this);
	m_pRadioBtn_First = new FRadioButton(this);
	m_pRadioBtn_Second = new FRadioButton(this);
	m_pButtonBar = new BottomButtonBar(this);

	m_pDurationLbl->setAlignment(Qt::AlignRight | Qt::AlignCenter);
	m_pModeLbl->setAlignment(Qt::AlignRight);

	m_pButtonBar->HideBtnByID(BottomButtonBar::Reset_Btn);
	m_pButtonBar->HideBtnByID(BottomButtonBar::Advanced_Btn);

	m_pOverlap->SetImageLblObjectName("FTransitionPanel_Overlap_FLabel");
	m_pPrefix->SetImageLblObjectName("FTransitionPanel_Prefix_FLabel");
	m_pPostfix->SetImageLblObjectName("FTransitionPanel_Poxtfix_FLabel");

	ConnectSignals();
	RetranslateUi();
	setMinimumSize(QSize(400, 300));
	setStyleSheet(gGetStyleSheet(":/FPropertyPanel/FPropertyPanel.css"));
}

FTransitionPanel::~FTransitionPanel()
{

}

void FTransitionPanel::Init()
{
	m_pPanelMgr->SetTimeline(GetTimeline());
	m_pPanelMgr->SetRootTimeline(GetRootTimeline());
}

void FTransitionPanel::Show()
{
	//m_pSpinBox->setValue();
}

void FTransitionPanel::Hide()
{

}

void FTransitionPanel::Destory()
{

}

void FTransitionPanel::Update()
{
	Show();
}

void FTransitionPanel::TranslateText()
{

}

void FTransitionPanel::RetranslateUi()
{
	m_pDurationLbl->setText(tr("Duration:"));
	m_pApplyToAllBtn->setText(tr("Apply to all"));
	m_pModeLbl->setText(tr("Transition Mode:"));
	m_pOverlap->SetText(tr("Overlap"));
	m_pPrefix->SetText(tr("Prefix"));
	m_pPostfix->SetText(tr("Postfix"));
	m_pRadioBtnHintLbl->setText(tr("When there's not enough media:"));
	m_pRadioBtn_First->setText(tr("Use trimmed frames"));
	m_pRadioBtn_Second->setText(tr("Use repeat frames"));
}

void FTransitionPanel::SetEnabled(bool enabled)
{
	m_pDurationLbl->setEnabled(enabled);
	m_pSpinBox->setEnabled(enabled);
	m_pApplyToAllBtn->setEnabled(enabled);
	m_pModeLbl->setEnabled(enabled);
	m_pOverlap->setEnabled(enabled);
	m_pPrefix->setEnabled(enabled);
	m_pPostfix->setEnabled(enabled);
	m_pRadioBtnHintLbl->setEnabled(enabled);
	m_pRadioBtn_First->setEnabled(enabled);
	m_pRadioBtn_Second->setEnabled(enabled);
}

void FTransitionPanel::SetTransitionMode(TransitionModeWidget::TransitionMode mode)
{
	switch (mode)
	{
	case TransitionModeWidget::Overlap_Mode:
		m_pOverlap->Select(true);
		m_pPrefix->Select(false);
		m_pPostfix->Select(false);
		break;
	case TransitionModeWidget::Prefix_Mode:
		m_pOverlap->Select(false);
		m_pPrefix->Select(true);
		m_pPostfix->Select(false);
		break;
	case TransitionModeWidget::Postfix_Mode:
		m_pOverlap->Select(false);
		m_pPrefix->Select(false);
		m_pPostfix->Select(true);
		break;
	default:
		break;
	}
}

void FTransitionPanel::resizeEvent(QResizeEvent *)
{
	int top, left;
	const int ButtonBarHeight = 25;
	left = 0, top = height() - ButtonBarHeight - 4;
	m_pButtonBar->setGeometry(left, top, width(), ButtonBarHeight - 5);
	left = width()*0.14;
	top = height()*0.1;
	m_pDurationLbl->setGeometry(left, top, 120, ButtonBarHeight);
	left = m_pDurationLbl->geometry().right() + 0.05*width();
	m_pSpinBox->setGeometry(left, top, 110, ButtonBarHeight);
	left = m_pSpinBox->geometry().right() + 0.05*width();
	m_pApplyToAllBtn->setGeometry(left, top, 120, ButtonBarHeight);
	left = m_pDurationLbl->geometry().left();
	top = m_pDurationLbl->geometry().bottom() + height()*0.03;
	m_pModeLbl->setGeometry(left, top, 120, ButtonBarHeight);
	left = width() * 0.22;
	top = m_pModeLbl->geometry().bottom() + 0.05*height();
	m_pOverlap->setGeometry(left, top, 90, 80);
	left = m_pOverlap->geometry().right() + 0.05*width();
	m_pPrefix->setGeometry(left, top, 90, 80);
	left = m_pPrefix->geometry().right() + 0.05*width();
	m_pPostfix->setGeometry(left, top, 90, 80);
	top = m_pOverlap->geometry().bottom() + height()*0.05;
	left = width()*0.2;
	m_pRadioBtnHintLbl->setGeometry(left, top, 180, ButtonBarHeight);
	top = m_pRadioBtnHintLbl->geometry().bottom() + 0.03*height();
	left = 0.3*width();
	m_pRadioBtn_First->setGeometry(left, top, 130, ButtonBarHeight);
	top = m_pRadioBtn_First->geometry().bottom() + 0.03*height();
	m_pRadioBtn_Second->setGeometry(left, top, 130, ButtonBarHeight);
}

void FTransitionPanel::ConnectSignals()
{
	connect(m_pButtonBar, &BottomButtonBar::sigClicked, this, [&](BottomButtonBar::Button_ID id){
		if (id == BottomButtonBar::Ok_Btn) emit sigOk();
	});
	connect(m_pOverlap, &TransitionModeWidget::Clicked, this, &FTransitionPanel::sigTransitionModeClicked);
	connect(m_pPrefix, &TransitionModeWidget::Clicked, this, &FTransitionPanel::sigTransitionModeClicked);
	connect(m_pPostfix, &TransitionModeWidget::Clicked, this, &FTransitionPanel::sigTransitionModeClicked);
	connect(m_pRadioBtn_First, &FRadioButton::toggled, this, [&](bool checked){
		if (checked) emit sigRadioButtonChanged(TrimmedFrames_State);
	});
	connect(m_pRadioBtn_Second, &FRadioButton::toggled, this, [&](bool checked){
		if (checked) emit sigRadioButtonChanged(RepeatFrames_State);
	});
}

/*TransitionModeWidget*/
TransitionModeWidget::TransitionModeWidget(TransitionMode mode /*= Overlap_Mode*/, QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
	, m_Mode(mode)
	, m_bSelected(false)
{
	m_pImageLbl = new FLabel(this);
	m_pTextLbl = new FLabel(this);
	m_pTextLbl->setAlignment(Qt::AlignCenter);
}

TransitionModeWidget::~TransitionModeWidget()
{

}

void TransitionModeWidget::SetText(QString text)
{
	m_pTextLbl->setText(text);
}

void TransitionModeWidget::SetImageLblObjectName(QString name)
{
	m_pImageLbl->setObjectName(name);
}

void TransitionModeWidget::Select(bool selected)
{
	m_bSelected = selected;
	update();
}

TransitionModeWidget::TransitionMode TransitionModeWidget::GetTransitionMode()
{
	return m_Mode;
}

void TransitionModeWidget::mousePressEvent(QMouseEvent *)
{
	emit Clicked(m_Mode);
}

void TransitionModeWidget::resizeEvent(QResizeEvent *)
{
	int top, left;
	left = top = 0;
	const int TextBtnHeight = 20;
	top = height() - TextBtnHeight- 1;
	m_pTextLbl->setGeometry(left, top, width(), TextBtnHeight);
	top = 0;
	m_pImageLbl->setGeometry(left, top, width(), height() - TextBtnHeight);
}

void TransitionModeWidget::paintEvent(QPaintEvent *)
{
	if (m_bSelected){
		QPainter p(this);
		p.setPen(QPen(Qt::red, 3, Qt::SolidLine));
		QRect r = m_pImageLbl->geometry();
		p.drawRect(QRect(r.x() + 1, r.y() + 1, r.width() - 3, r.height() - 3));
	}
}
