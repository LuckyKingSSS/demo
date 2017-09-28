#include "stdafx.h"
#include "FPhotoPanel.h"
#include "inc_CommonLib/externalapi.h"

FPhotoPanel::FPhotoPanel(QWidget *parent)
: FPropertyPanel(parent)
{
	m_pPanelMgr = new FPhotoPanelMgr(this);
	m_pRotate = new FPropertyRotate(this);
	m_pButtonBar = new BottomButtonBar(this);

	m_pAdjust = new FLabel(this);
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
	m_pDuration = new FPropertySpinBox(this);
	m_pMenu = new FMenu(this);

	m_pContrast->SetRange(-100, 100);
	m_pSaturation->SetRange(-100, 100);
	m_pBrightness->SetRange(-100, 100);
	m_pTint->SetRange(-100, 100);

	setStyleSheet(gGetStyleSheet(":/FPropertyPanel/FPropertyPanel.css"));

	setMinimumSize(240, 300);
	Init3DLUTMenu();
	ConnectSignals();
	RetranslateUi();
}

FPhotoPanel::~FPhotoPanel()
{

}

void FPhotoPanel::Init()
{
	m_pPanelMgr->SetTimeline(GetTimeline());
	m_pPanelMgr->SetRootTimeline(GetRootTimeline());
}

void FPhotoPanel::Show()
{
	m_pContrast->SetValue(m_pPanelMgr->GetContrastValue());
	m_pSaturation->SetValue(m_pPanelMgr->GetSaturationValue());
	m_pBrightness->SetValue(m_pPanelMgr->GetBrightnessValue());
	m_pTint->SetValue(m_pPanelMgr->GetTintValue());
	m_pEnhance->SetCheckState(m_pPanelMgr->Enhanced());
	m_pDuration->SetValue(m_pPanelMgr->GetDuration());
}

void FPhotoPanel::Hide()
{

}

void FPhotoPanel::SetEnabled(bool enabled)
{
	m_pRotate->setEnabled(enabled);
	m_pContrast->setEnabled(enabled);
	m_pSaturation->setEnabled(enabled);
	m_pBrightness->setEnabled(enabled);
	m_pTint->setEnabled(enabled);
	m_p3DLUT->setEnabled(enabled);
	m_pEnhance->setEnabled(enabled);
	m_pDuration->setEnabled(enabled);
	m_pButtonBar->SetEnabledByID(BottomButtonBar::Reset_Btn, enabled);
	m_pButtonBar->SetEnabledByID(BottomButtonBar::Advanced_Btn, enabled);
}

void FPhotoPanel::Destory()
{

}

void FPhotoPanel::Update()
{
	Show();
}

void FPhotoPanel::TranslateText()
{

}

void FPhotoPanel::RetranslateUi()
{
	m_pAdjust->setText(tr("Adjust"));
	m_pContrast->SetPropertyName(tr("Contrast:"));
	m_pSaturation->SetPropertyName(tr("Saturation:"));
	m_pBrightness->SetPropertyName(tr("Brightness:"));
	m_pTint->SetPropertyName(tr("Tint:"));
	m_p3DLUT->SetPropertyName(tr("3D LUT:"));
	m_pEnhance->SetPropertyName(tr("Enhance:"));
	m_pEnhance->SetCheckBoxString(tr("Auto enhance"));
	m_pDuration->SetPropertyName(tr("Duration:"));
	m_pDuration->SetButtonText(tr("Apply to all"));
}

void FPhotoPanel::resizeEvent(QResizeEvent *)
{
	int left, top;
	const int BottonBarHeight = 24;
	const int RotateHeight = 24;
	left = 0;
	float rate = (1.0 * height() - 10 * BottonBarHeight) / (10 * height());
	top = rate*height();
	m_pRotate->setGeometry(left, top, width(), RotateHeight);
	top = m_pRotate->geometry().bottom() + rate*height();
	m_pAdjust->setGeometry(0.04*width(), top, width(), BottonBarHeight);
	top = m_pAdjust->geometry().bottom() + rate*height();
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
	m_pDuration->setGeometry(left, top, width(), BottonBarHeight);
	m_pButtonBar->setGeometry(left, height() - BottonBarHeight - 5, width(), BottonBarHeight);
}

void FPhotoPanel::ConnectSignals()
{
	connect(m_pRotate, &FPropertyRotate::sigRotateChanged, this, &FPhotoPanel::slotRotateChanged);
	connect(m_pContrast, &FPropertySlider::sigValueChanged, this, &FPhotoPanel::slotContrastChanged);
	connect(m_pSaturation, &FPropertySlider::sigValueChanged, this, &FPhotoPanel::slotSaturationChanged);
	connect(m_pBrightness, &FPropertySlider::sigValueChanged, this, &FPhotoPanel::slotBrightnessChanged);
	connect(m_pTint, &FPropertySlider::sigValueChanged, this, &FPhotoPanel::slotTintChanged);
	connect(m_pEnhance, &FPropertyCheckBox::sigStateChanged, this, &FPhotoPanel::slotEnhanceChanged);
	connect(m_pButtonBar, &BottomButtonBar::sigClicked, this, [&](BottomButtonBar::Button_ID id){
		if (id == BottomButtonBar::Reset_Btn) slotReset();
		else if (id == BottomButtonBar::Advanced_Btn) emit slotAdvanced();
		else if (id == BottomButtonBar::Ok_Btn) emit sigOk();
	});
	connect(m_p3DLUT, &FPropertyButton::sigTextButtonClicked, this, &FPhotoPanel::slotShowMenu);
	connect(m_pMenu, &FMenu::triggered, this, &FPhotoPanel::slot3DLUTChanged);
	connect(m_p3DLUT, &FPropertyButton::sigImageButtonClicked, this, &FPhotoPanel::slotDefault3DLUT);
	connect(m_pDuration, SIGNAL(sigValueChanged(int)), this, SLOT(slotDurationChanged(int)));
	connect(m_pDuration, &FPropertySpinBox::sigClicked, this, &FPhotoPanel::slotDurationApplyToAll);
}

void FPhotoPanel::Init3DLUTMenu()
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

void FPhotoPanel::slotRotateChanged(FPropertyRotate::RotateMode mode)
{
	m_pPanelMgr->SetRotate(mode);
	emit sigUpdateEditPanel();
}

void FPhotoPanel::slotContrastChanged(int value)
{
	m_pPanelMgr->SetContrastValue(value);
	emit sigUpdateEditPanel();
}

void FPhotoPanel::slotSaturationChanged(int value)
{
	m_pPanelMgr->SetSaturationValue(value);
	emit sigUpdateEditPanel();
}

void FPhotoPanel::slotBrightnessChanged(int value)
{
	m_pPanelMgr->SetBrightnessValue(value);
	emit sigUpdateEditPanel();
}

void FPhotoPanel::slotTintChanged(int value)
{
	m_pPanelMgr->SetTintValue(value);
	emit sigUpdateEditPanel();
}

void FPhotoPanel::slot3DLUTChanged(QAction* pAction)
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

void FPhotoPanel::slotDefault3DLUT()
{
	m_p3DLUT->SetButtonText(m_lstActions[0]->text());
}

void FPhotoPanel::slotEnhanceChanged(bool enabled)
{
	m_pPanelMgr->EnableEnhance(enabled);
	emit sigUpdateEditPanel();
}

void FPhotoPanel::slotDurationChanged(int duration)
{
	m_pPanelMgr->SetDuration(duration);
	emit sigUpdate();
}

void FPhotoPanel::slotDurationApplyToAll()
{
	m_pPanelMgr->DurationApplyToAll();
	emit sigUpdate();
}

void FPhotoPanel::slotReset()
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
	emit sigUpdateEditPanel();
}

void FPhotoPanel::slotAdvanced()
{

}

void FPhotoPanel::slotShowMenu()
{
	QPoint pos = QCursor::pos();
	QWidget *pWidget = QApplication::widgetAt(pos);
	if (pWidget){
		QPoint menuPos = mapToGlobal(m_p3DLUT->mapToParent(pWidget->geometry().bottomLeft()));
		if (m_pMenu) m_pMenu->exec(menuPos);
	}
}


/*FPropertySpinBox*/
FPropertySpinBox::FPropertySpinBox(QWidget *parent /*= 0*/)
: FBaseWidget(parent)
{
	m_pPropertyLbl = new FLabel(this);
	m_pSpinBox = new FSpinBox(this);
	m_pTextBtn = new FButton(this);

	connect(m_pTextBtn, &FButton::clicked, this, &FPropertySpinBox::sigClicked);
	connect(m_pSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
	connect(m_pSpinBox, SIGNAL(valueChanged(const QString&)), this, SLOT(slotValueChanged(const QString&)));
}

FPropertySpinBox::~FPropertySpinBox()
{

}

void FPropertySpinBox::SetPropertyName(QString name)
{
	m_pPropertyLbl->setText(name);
}

QString FPropertySpinBox::GetPropertyName()
{
	return m_pPropertyLbl->text();
}

void FPropertySpinBox::SetButtonText(QString text)
{
	m_pTextBtn->setText(text);
}

int FPropertySpinBox::GetValue()
{
	return m_pSpinBox->value();
}

void FPropertySpinBox::SetValue(int value)
{
	m_pSpinBox->setValue(value);
}

FSpinBox * FPropertySpinBox::GetSpinBox()
{
	return m_pSpinBox;
}

void FPropertySpinBox::resizeEvent(QResizeEvent *)
{
	int left, top;
	top = 0;
	const int SpinBoxWidth = 110;
	const int TextButtonWidth = 125;
	const int Gap = 5;
	const int PropertyLblWidth = 80;
	left = width() * 0.5 - (SpinBoxWidth + TextButtonWidth + Gap) / 2;
	if (left < PropertyLblWidth) left = PropertyLblWidth;
	m_pSpinBox->setGeometry(left, top, SpinBoxWidth, height());
	left = m_pSpinBox->geometry().right() + Gap;
	m_pTextBtn->setGeometry(left, top, TextButtonWidth, height());
	left = m_pSpinBox->geometry().left() - width() * 0.08 - PropertyLblWidth;
	if (left < 0) left = 0;
	m_pPropertyLbl->setGeometry(left, top, PropertyLblWidth, height());
}

void FPropertySpinBox::slotValueChanged(int value)
{
	emit sigValueChanged(value);
}

void FPropertySpinBox::slotValueChanged(const QString& text)
{
	emit sigValueChanged(text);
}





