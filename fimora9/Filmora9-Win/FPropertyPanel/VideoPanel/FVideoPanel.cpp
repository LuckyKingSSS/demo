#include "stdafx.h"
#include "FVideoPanel.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_CommonLib/externalapi.h"
#include "FPropertySlider.h"
#include "FVideoPanelMgr.h"
#include "FAdvanceVedioDlg.h"
#include <QVBoxLayout>

FVideoPanel::FVideoPanel(QWidget *parent)
: FPropertyPanel(parent)
{
	m_pPanelMgr = new FVideoPanelMgr(this);
	m_pRotate = new FPropertyRotate(this);
	m_pButtonBar = new BottomButtonBar(this);
	m_pContrast = new FPropertySlider(this);
	m_pContrast->SetSliderObjectName("FVideoPanel_Slider");
	m_pSaturation = new FPropertySlider(this);
	m_pSaturation->SetSliderObjectName("FVideoPanel_Slider");
	m_pBrightness = new FPropertySlider(this);
	m_pBrightness->SetSliderObjectName("FVideoPanel_Slider");
	m_pTint = new FPropertySlider(this);
	m_pTint->SetSliderObjectName("FVideoPanel_Slider");
	m_p3DLUT = new FPropertyButton(this);
	m_pEnhance = new FPropertyCheckBox(this);
	m_pSpeed = new FFiniteValuePropertySlider(this);
	m_pSpeed->SetSliderObjectName("FVideoPanel_Slider");
	m_pPlayInReverse = new FPropertyCheckBox(this);
	m_pPlayInReverse->SetPropertyName("");
	m_pMenu = new FMenu(this);
	m_pContrast->SetRange(-100, 100);
	m_pSaturation->SetRange(-100, 100);
	m_pBrightness->SetRange(-100, 100);
	m_pTint->SetRange(-100, 100);
    QList<double> values = QList<double>() << 0.10 << 0.20 << 0.25 << 0.33 << 0.50 << 1.00 << 2.00 << 3.00 << 4.00 << 5.00 << 10.00;
	m_pSpeed->SetFiniteValues(values);
	m_pSpeed->SetDecimalPlaces(2);
	m_pSpeed->SetSuffix("X");

	setStyleSheet(gGetStyleSheet(":/FPropertyPanel/FPropertyPanel.css"));

	setMinimumSize(300, 280);
	Init3DLUTMenu();
	ConnectSignals();
	RetranslateUi();
}

FVideoPanel::~FVideoPanel()
{

}

void FVideoPanel::RetranslateUi()
{
	m_pContrast->SetPropertyName(tr("Contrast:"));
	m_pSaturation->SetPropertyName(tr("Saturation:"));
	m_pBrightness->SetPropertyName(tr("Brightness:"));
	m_pTint->SetPropertyName(tr("Tint:"));
	m_p3DLUT->SetPropertyName(tr("3D LUT:"));
	m_pEnhance->SetPropertyName(tr("Enhance:"));
	m_pEnhance->SetCheckBoxString(tr("Auto enhance"));
	m_pSpeed->SetPropertyName(tr("Speed"));
	m_pPlayInReverse->SetCheckBoxString(tr("Play video clip in reverse"));
}

void FVideoPanel::SetEnabled(bool enabled)
{
	m_pRotate->setEnabled(enabled);
	m_pContrast->setEnabled(enabled);
	m_pSaturation->setEnabled(enabled);
	m_pBrightness->setEnabled(enabled);
	m_pTint->setEnabled(enabled);
	m_p3DLUT->setEnabled(enabled);
	m_pEnhance->setEnabled(enabled);
	m_pSpeed->setEnabled(enabled);
	m_pPlayInReverse->setEnabled(enabled);
	m_pButtonBar->SetEnabledByID(BottomButtonBar::Reset_Btn, enabled);
	m_pButtonBar->SetEnabledByID(BottomButtonBar::Advanced_Btn, enabled);
}

void FVideoPanel::Init()
{
	m_pPanelMgr->SetTimeline(GetTimeline());
	m_pPanelMgr->SetRootTimeline(GetRootTimeline());
}

void FVideoPanel::Show()
{
	m_pContrast->SetValue(m_pPanelMgr->GetContrastValue());
	m_pSaturation->SetValue(m_pPanelMgr->GetSaturationValue());
	m_pBrightness->SetValue(m_pPanelMgr->GetBrightnessValue());
	m_pTint->SetValue(m_pPanelMgr->GetTintValue());
	m_pEnhance->SetCheckState(m_pPanelMgr->Enhanced());
	m_pSpeed->SetValue(m_pPanelMgr->GetSpeedValue());
	m_pPlayInReverse->SetCheckState(m_pPanelMgr->PlayInReverse());
}

void FVideoPanel::Hide()
{
	//hide();
}

void FVideoPanel::Destory()
{
	//destroy();
}

void FVideoPanel::Update()
{
	Show();
}

void FVideoPanel::TranslateText()
{

}

void FVideoPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	const int BottonBarHeight = 24;
	const int RotateHeight = 24;
	left = 0;
	float rate = (1.0 * height() - 240) / (10 * height());
	top = rate*height();
	m_pRotate->setGeometry(left, top, width(), RotateHeight);
	top = m_pRotate->geometry().bottom() + rate*height();
	m_pContrast->setGeometry(left, top, width(), BottonBarHeight);
	top = m_pContrast->geometry().bottom() + rate*height();
	m_pSaturation->setGeometry(left, top, width(), BottonBarHeight);
	top = m_pSaturation->geometry().bottom() + rate*height();
	m_pBrightness->setGeometry(left, top, width(), BottonBarHeight);
	top = m_pBrightness->geometry().bottom() + rate*height();
	m_pTint->setGeometry(left, top, width(), BottonBarHeight);
	top = m_pTint->geometry().bottom() + rate*height();
	m_p3DLUT->setGeometry(left, top, width(), BottonBarHeight);
	top = m_p3DLUT->geometry().bottom() + rate*height();
	m_pEnhance->setGeometry(left, top, width(), BottonBarHeight);
	top = m_pEnhance->geometry().bottom() + rate*height();
	m_pSpeed->setGeometry(left, top, width(), BottonBarHeight);
	top = m_pSpeed->geometry().bottom() + rate*height();
	m_pPlayInReverse->setGeometry(left, top, width(), BottonBarHeight);
	m_pButtonBar->setGeometry(left, height() - BottonBarHeight - 5, width(), BottonBarHeight);
}

void FVideoPanel::ConnectSignals()
{
	connect(m_pRotate, &FPropertyRotate::sigRotateChanged, this, &FVideoPanel::slotRotateChanged);
	connect(m_pContrast, &FPropertySlider::sigValueChanged, this, &FVideoPanel::slotContrastChanged);
	connect(m_pSaturation, &FPropertySlider::sigValueChanged, this, &FVideoPanel::slotSaturationChanged);
	connect(m_pBrightness, &FPropertySlider::sigValueChanged, this, &FVideoPanel::slotBrightnessChanged);
	connect(m_pTint, &FPropertySlider::sigValueChanged, this, &FVideoPanel::slotTintChanged);
	connect(m_pEnhance, &FPropertyCheckBox::sigStateChanged, this, &FVideoPanel::slotEnhanceChanged);
	connect(m_pSpeed, &FFiniteValuePropertySlider::sigValueChanged, this, &FVideoPanel::slotSpeedChanged);
	connect(m_pPlayInReverse, &FPropertyCheckBox::sigStateChanged, this, &FVideoPanel::slotPlayInReverse);
	connect(m_pButtonBar, &BottomButtonBar::sigClicked, this, [&](BottomButtonBar::Button_ID id){
		if (id == BottomButtonBar::Reset_Btn) slotReset();
		else if (id == BottomButtonBar::Advanced_Btn) slotAdvanced();
		else if (id == BottomButtonBar::Ok_Btn) emit sigOk();
	});
	connect(m_p3DLUT, &FPropertyButton::sigTextButtonClicked, this, &FVideoPanel::slotShowMenu);
	connect(m_p3DLUT, &FPropertyButton::sigImageButtonClicked, this, &FVideoPanel::slotDefault3DLUT);
	connect(m_pMenu, &FMenu::triggered, this, &FVideoPanel::slot3DLUTChanged);
}

void FVideoPanel::Init3DLUTMenu()
{
	QAction *pAction = nullptr;
	pAction = m_pMenu->addAction(tr("None"));
	m_lstActions.append(pAction);
	pAction = m_pMenu->addAction(tr("Load New LUT..."));
	m_lstActions.append(pAction);
	m_pMenu->addSeparator();
	QDir dir("C:/ProgramData/Wondershare Video Editor/Resources/LightRoom/CubeLUTFiles");
	if (dir.exists()){
		QStringList filelist = dir.entryList(QDir::Files);
		for (auto file : filelist){
			pAction = m_pMenu->addAction(file.mid(0, file.lastIndexOf(".")));
			m_lstActions.append(pAction);
		}
	}
}

void FVideoPanel::slotShowMenu()
{
	QPoint pos = QCursor::pos();
	QWidget *pWidget = QApplication::widgetAt(pos);
	if (pWidget){
		QPoint menuPos = mapToGlobal(m_p3DLUT->mapToParent(pWidget->geometry().bottomLeft()));
		if (m_pMenu) m_pMenu->exec(menuPos);
	}
}

void FVideoPanel::slotRotateChanged(FPropertyRotate::RotateMode mode)
{
	m_pPanelMgr->SetRotate(mode);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slotContrastChanged(int value)
{
	m_pPanelMgr->SetContrastValue(value);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slotSaturationChanged(int value)
{
	m_pPanelMgr->SetSaturationValue(value);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slotBrightnessChanged(int value)
{
	m_pPanelMgr->SetBrightnessValue(value);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slotTintChanged(int value)
{
	m_pPanelMgr->SetTintValue(value);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slot3DLUTChanged(QAction* pAction)
{
	int index = m_lstActions.indexOf(pAction);
	if (index == 1){
		QStringList filelist = QFileDialog::getOpenFileNames(0, tr("open"), "c:/");
	}
	else{
		QString text = pAction->text();
		m_p3DLUT->SetButtonText(text);
	}
}

void FVideoPanel::slotDefault3DLUT()
{
	m_p3DLUT->SetButtonText(m_lstActions[0]->text());
}

void FVideoPanel::slotEnhanceChanged(bool enabled)
{
	m_pPanelMgr->EnableEnhance(enabled);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slotSpeedChanged(float value)
{
	m_pPanelMgr->SetSpeedValue(1.0 / value);
	emit sigUpdate();
}

void FVideoPanel::slotPlayInReverse(bool enabled)
{
	m_pPanelMgr->EnablePlayInReverse(enabled);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slotReset()
{
	m_pPanelMgr->RotateReset();
	m_pContrast->SetValue(0);
	m_pPanelMgr->SetContrastValue(0);
	m_pSaturation->SetValue(0);
	m_pPanelMgr->SetSaturationValue(0);
	m_pBrightness->SetValue(0);
	m_pPanelMgr->SetBrightnessValue(0);
	m_pTint->SetValue(0);
	m_pPanelMgr->SetTintValue(0);
	m_pEnhance->SetCheckState(false);
	m_pPanelMgr->EnableEnhance(false);
	m_pSpeed->SetValue(1.00);
	m_pPanelMgr->SetSpeedValue(1.00);
	m_pPlayInReverse->SetCheckState(false);
	m_pPanelMgr->EnablePlayInReverse(false);
	emit sigUpdateEditPanel();
}

void FVideoPanel::slotAdvanced()
{
	FAdvanceVedioDlg *m_pAdvance = new FAdvanceVedioDlg(GetTimeline(), this);
	m_pAdvance->exec();
}






