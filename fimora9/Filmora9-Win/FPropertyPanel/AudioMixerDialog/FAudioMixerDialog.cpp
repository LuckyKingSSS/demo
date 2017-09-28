#include "stdafx.h"
#include "FAudioMixerDialog.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/CommonWidgets.h"

FAudioMixerDialog::FAudioMixerDialog(QWidget *parent)
: FPropertyDialog(nullptr,parent)
{
	setFixedSize(QSize(600, 600));
	m_titlebar->setObjectName("FAudioMixerDialog_TitleBar");
	
	m_pPanelMgr = new FAudioMixerDialogMgr(this);
	m_pAudioTrackVideo = new FAudioTrack(this);
	m_pAudioTrackVideo->SetImageLabelObjectName("FAudioMixerDialog_FLabel_Video");
	m_pAudioTrackPIP = new FAudioTrack(this);
	m_pAudioTrackPIP->SetImageLabelObjectName("FAudioMixerDialog_FLabel_Pip");
	m_pAudioTrackMusic1 = new FAudioTrack(this);
	m_pAudioTrackMusic1->SetImageLabelObjectName("FAudioMixerDialog_FLabel_Music");
	m_pAudioTrackMusic2 = new FAudioTrack(this);
	m_pAudioTrackMusic2->SetImageLabelObjectName("FAudioMixerDialog_FLabel_Music");
	float values[9] = { -60, -40, -30, -18, -12, -6, 0, 6, 12 };
	m_pAudioTrackVideo->SetMarkValue(values);
	m_pAudioTrackPIP->SetMarkValue(values);
	m_pAudioTrackMusic1->SetMarkValue(values);
	m_pAudioTrackMusic2->SetMarkValue(values);
	m_pRuler1 = new FMarkSliderRuler(values, this);
	m_pRuler2 = new FMarkSliderRuler(values, this);
	m_pMasterSlider = new FMarkSlider(this);
	m_pMasterSlider->SetMarkValue(values);
	m_pMasterSlider->SetSliderObjectName("FVideoPanel_Slider");
	m_pMasterAudioTrackLeft = new FColorGradientSlider(Qt::Vertical, this);
	m_pMasterAudioTrackLeft->setObjectName("FAudioTrack");
	m_pMasterAudioTrackRight = new FColorGradientSlider(Qt::Vertical, this);
	m_pMasterAudioTrackRight->setObjectName("FAudioTrack");
	m_pMasterLineEdit = new FLineEdit(this);
	m_pMasterLbl = new FLabel(this);
	m_pMasterLbl->setAlignment(Qt::AlignCenter);
	m_pMastImageLbl = new FLabel(this);
	m_pMastImageLbl->setObjectName("FAudioMixerDialog_FLabel_Audio");
	m_pPlayBtn = new FStateButton(0, "FStateButton_Play&Pause", this);
	m_pPlayBtn->AddButton(0, "FStateButton_Play");
	m_pPlayBtn->AddButton(1, "FStateButton_Pause");
	m_pStopBtn = new FImageButton("FImageButton_Stop", this);
	m_pDurationSlider = new FToolBarSlider(this);
	m_pDurationSlider->SetSliderObjectName("FVideoPanel_Slider");
	m_pPanLbl = new FLabel(this);
	m_pPanLbl->setText("Pan");
	m_pDBLbl = new FLabel(this);
	m_pDBLbl->setText("dB");
	m_pTimeLbl = new FLabel(this);
	m_pTotalTimeLbl = new FLabel(this);
	m_pOkBtn = new FButton(this);
	m_pCancelBtn = new FButton(this);
	ConnectSignals();
	Init();
	setStyleSheet(gGetStyleSheet(":/FPropertyPanel/FPropertyPanel.css"));
}

FAudioMixerDialog::~FAudioMixerDialog()
{

}

void FAudioMixerDialog::resizeEvent(QResizeEvent * event)	
{
	FPropertyDialog::resizeEvent(event);
	int top, left;
	left = 0;
	top = m_titlebar->height();

	left = 80;
	top += 10;
	m_pAudioTrackVideo->setGeometry(left, top, 85, 450);
	left = m_pAudioTrackVideo->geometry().right() + 10;
	m_pAudioTrackPIP->setGeometry(left, top, 85, 450);
	left = m_pAudioTrackPIP->geometry().right() + 10;
	m_pAudioTrackMusic1->setGeometry(left, top, 85, 450);
	left = m_pAudioTrackMusic1->geometry().right() + 10;
	m_pAudioTrackMusic2->setGeometry(left, top, 85, 450);

	// m_pRuler2
	left = m_pAudioTrackMusic2->geometry().right() + 20;
	top = 231;
	m_pRuler2->setGeometry(left, top, 28, 262);

	// m_pMasterSlider
	left = m_pRuler2->geometry().right() + 15;
	m_pMasterSlider->setGeometry(left, top, 10, 262);

	top -= 30;
	m_pMasterLineEdit->setGeometry(left + 5, top, 50, 20);
	top -= 70;
	m_pMasterLbl->setGeometry(left + 5, top, 50, 20);
	top -= 60;
	m_pMastImageLbl->setGeometry(left, top, 60, 40);

	top = m_pMasterSlider->geometry().top();
	left = m_pMasterSlider->geometry().right() + 20;
	m_pMasterAudioTrackLeft->setGeometry(left, top, 8, 262);
	left = m_pMasterAudioTrackLeft->geometry().right() + 3;
	m_pMasterAudioTrackRight->setGeometry(left, top, 8, 262);

	top = m_pAudioTrackVideo->geometry().bottom() + 30;
	left = 30;
	m_pPlayBtn->setGeometry(left, top, 20, 20);
	left = m_pPlayBtn->geometry().right() + 10;
	m_pStopBtn->setGeometry(left, top, 20, 20);
	left = m_pStopBtn->geometry().right() + 10;
	m_pDurationSlider->setGeometry(left, top + 6, 340, 5);
	left = m_pDurationSlider->geometry().right() + 10;
	m_pTimeLbl->setGeometry(left, top, 50, 20);
	left = m_pTimeLbl->geometry().right();
	m_pTotalTimeLbl->setGeometry(left, top, 55, 20);
	left = width() - 10 - 75;
	top = height() - 10 - 25;
	m_pCancelBtn->setGeometry(left, top, 75, 25);
	left = left - 8 - 75;
	m_pOkBtn->setGeometry(left, top, 75, 25);

	// m_pRuler1
	left = 20;
	top = 227;
	m_pRuler1->setGeometry(left, top, 28, 267);

	// dB
	left += 8;
	top -= 28;
	m_pDBLbl->setGeometry(left, top, 20, 20);

	// Pan
	top -= 60;
	m_pPanLbl->setGeometry(left - 5, top, 25, 20);
}

void FAudioMixerDialog::showEvent(QShowEvent *)
{
	m_pAudioTrackVideo->SetText(tr("Video"));
	m_pAudioTrackPIP->SetText(tr("PIP 1"));
	m_pAudioTrackMusic1->SetText(tr("Music 1"));
	m_pAudioTrackMusic2->SetText(tr("Music 2"));
	m_pOkBtn->setText(tr("OK"));
	m_pCancelBtn->setText(tr("Cancel"));
	m_pMasterLbl->setText(tr("Master"));
}

void FAudioMixerDialog::ConnectSignals()
{
	connect(m_pPlayBtn, &FStateButton::Clicked, this, [&](int id){
		if (id == 0) m_pPanelMgr->Play();
		else if (id == 1) m_pPanelMgr->Pause();
	});
	connect(m_pStopBtn, &FImageButton::clicked, this, [&](){
		m_pDurationSlider->SetValue(0);
		m_pTimeLbl->setText(GetTimeString(0));
		m_pPanelMgr->Stop();
	});
	connect(m_pDurationSlider, &FToolBarSlider::valueChanged, this, [&](float value){
		m_pTimeLbl->setText(GetTimeString(value));
	});
	connect(m_pMasterSlider, &FMarkSlider::sigValueChanged, this, [&](float value){
		m_pMasterLineEdit->setText(QString::number(value, 'f', 1));
	});
	connect(m_pMasterLineEdit, &FLineEdit::editingFinished, this, [&](){
		float value = m_pMasterLineEdit->text().toFloat();
		m_pMasterSlider->SetValue(value);
		emit m_pMasterSlider->sigValueChanged(m_pMasterSlider->GetValue());
	});
}

void FAudioMixerDialog::Init()
{
	auto timeline = m_pPanelMgr->GetTimeline();
	if (timeline) {
		m_pDurationSlider->SetRange(0, m_pPanelMgr->GetDuration());
	}
	m_pTotalTimeLbl->setText("/" + GetTimeString(m_pDurationSlider->Duration()));
	m_pTimeLbl->setText("00:00:00");
	m_pMasterSlider->SetValue(0);
	emit m_pMasterSlider->sigValueChanged(m_pMasterSlider->GetValue());
	m_pAudioTrackVideo->SetValue(0);
	m_pAudioTrackPIP->SetValue(0);
	m_pAudioTrackMusic1->SetValue(0);
	m_pAudioTrackMusic2->SetValue(0);
}

QString FAudioMixerDialog::GetTimeString(int t)
{
	QString timeStr("");
	int s = t % 60;
	int m = t / 60;
	int h = t / 60 / 60;
	timeStr = QString("%1:%2:%3").arg(h, 2, 10, QChar('0')).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
	return timeStr;
}




