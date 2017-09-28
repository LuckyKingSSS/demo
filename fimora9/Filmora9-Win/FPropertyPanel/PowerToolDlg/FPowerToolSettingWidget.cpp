#include "stdafx.h"
#include "FPowerToolSettingWidget.h"
#include "..\AdvanceVedioDlg\FoldWidget\FFoldWidget.h"
#include "..\AdvanceVedioDlg\FFoldWidgetItem.h"


FPowerToolSettingWidget::FPowerToolSettingWidget(QWidget *parent) :FBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

FPowerToolSettingWidget::~FPowerToolSettingWidget()
{
	
}

void FPowerToolSettingWidget::InitUI()
{  
	const int iMinumItemWidth = 305;
	m_FolderWidget = new FFoldWidget(nullptr);
	m_FolderWidget->SetShowMode(true);
	FFoldWidgetItem* TiltShiftItem = new FFoldWidgetItem(m_FolderWidget);
	TiltShiftItem->SetHeaderTitleName(tr("Tilt-Shift"));
	TiltShiftItem->setFixedWidth(iMinumItemWidth);

	m_TiltShiftWidget = new FTiltShiftWidget(TiltShiftItem);
	TiltShiftItem->SetContainsWidget(m_TiltShiftWidget);
	m_FolderWidget->AddItem(TiltShiftItem);
	

	FFoldWidgetItem* MosaciaItem = new FFoldWidgetItem(m_FolderWidget);
	m_FMosaicWidget = new FMosaicWidget(MosaciaItem);
	MosaciaItem->SetContainsWidget(m_FMosaicWidget);
	MosaciaItem->SetHeaderTitleName(tr("Mosaic"));
	m_FolderWidget->AddItem(MosaciaItem);
	MosaciaItem->setFixedWidth(iMinumItemWidth);



	FFoldWidgetItem* FaceOff = new FFoldWidgetItem(m_FolderWidget);
	m_FFaceOffWidget = new FFaceOffWidget(FaceOff);
	FaceOff->SetHeaderTitleName(tr("Face off"));
	FaceOff->SetContainsWidget(m_FFaceOffWidget);
	m_FolderWidget->AddItem(FaceOff);
	FaceOff->setFixedWidth(iMinumItemWidth);

	m_OKBtn = new FButton(nullptr);
	m_OKBtn->setText(tr("OK"));
	m_OKBtn->setFixedSize(QSize(75,25));

	m_CancleBtn = new FButton(nullptr);
	m_CancleBtn->setText(tr("Cancle"));
	m_CancleBtn->setFixedSize(QSize(75, 25));

	QVBoxLayout* vLayout = new QVBoxLayout();
	m_FolderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_FolderWidget->setMinimumHeight(445);
	m_FolderWidget->setMinimumWidth(320);
	
	vLayout->addWidget(m_FolderWidget);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setContentsMargins(140, 30, 10, 10);
	hLayout->setSpacing(10);
	hLayout->addWidget(m_OKBtn);
	hLayout->addWidget(m_CancleBtn);
	vLayout->setContentsMargins(10,20,0,0);
	vLayout->setSpacing(30);
	vLayout->addLayout(hLayout);
	setLayout(vLayout);
}

void FPowerToolSettingWidget::InitConnections()
{

}

FTiltShiftWidget::FTiltShiftWidget(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

void FTiltShiftWidget::InitUI()
{
	const int iFixHeight = 405;
	m_CheckBox = new FCheckBox(this);
	m_CheckBox->setText(tr("Apply Tilt-Shift to the clip"));
	m_CheckBox->setGeometry(0, 10, 200, 30);
	setFixedHeight(iFixHeight);
}

void FTiltShiftWidget::InitConnections()
{

}

FMosaicWidget::FMosaicWidget(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

void FMosaicWidget::InitUI()
{
	const int iFixHeight = 405;
	m_EnableChckebox = new FCheckBox(this);
	m_EnableChckebox->setText(tr("Apply Mosaic to the clip"));
	m_EnableChckebox->setGeometry(0, 10, 180, 30);


	const int iButtonHeight = 30;
	const int iButtonWidth = 85;
	m_AddMosaicBtn = new FButton(this);
	m_AddMosaicBtn->setText(tr("Add"));
	m_AddMosaicBtn->setGeometry(45, 55, iButtonWidth, iButtonHeight);

	const int iBtnSpace = 50;
	m_DeleteMosaicBtn = new FButton(this);
	m_DeleteMosaicBtn->setText(tr("Delete"));
	int iLeft = m_DeleteMosaicBtn->geometry().right() + iBtnSpace;
	m_DeleteMosaicBtn->setGeometry(iLeft, 55, iButtonWidth, iButtonHeight);

	const int iVerticalSpace = 30;
	int iTop = m_DeleteMosaicBtn->geometry().bottom();
	iTop += iVerticalSpace;
	m_PercentLabel = new FLabel(this);
	m_PercentLabel->setText(tr("Percent:"));
	m_PercentLabel->setGeometry(20, iTop, 60, 20);

	iTop = m_PercentLabel->geometry().bottom() + 5;
	m_Slider = new ToolBarSlider(this);
	m_Slider->setDuration(100);
	m_Slider->setGeometry(50, iTop,250,20);
	setFixedHeight(iFixHeight);
}

void FMosaicWidget::InitConnections()
{

}

FFaceOffWidget::FFaceOffWidget(QWidget *parent) :FContexBaseWidget(parent)
{
	InitUI();
	InitConnections();
}

void FFaceOffWidget::resizeEvent(QResizeEvent *event)
{
	const int iCheckBoxHeight = 20;
	m_FaceOff->setGeometry(0, iCheckBoxHeight + 10 + 5, width(), height() - iCheckBoxHeight - 5 - 10);
}

void FFaceOffWidget::InitUI()
{
	const int iHeight = 405;
	const int iCheckBoxHeight = 20;
	m_EnableChckebox = new FCheckBox(this);
	m_EnableChckebox->setText(tr("Apply Face off to clip"));
	m_EnableChckebox->setGeometry(0, 10, 160, iCheckBoxHeight);
	m_FaceOff = new QListWidget(this);
	m_FaceOff->setFrameShape(QFrame::NoFrame);
	setFixedHeight(iHeight);
}

void FFaceOffWidget::InitConnections()
{

}
