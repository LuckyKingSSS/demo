#include "stdafx.h"
#include "FExportBar.h"
#include "inc_FCommonLib\FBaseWidget.h"
#include "inc_FCommonLib\FCommonWidgets.h"

FExportBar::FExportBar(QWidget *parent)
	: FBaseWidget(parent)
{
	initUI();
}

FExportBar::~FExportBar()
{

}

FExportBar::FExportBtnType FExportBar::GetCurrentBtn()
{
	return (FExportBtnType)m_pBtnGroup->checkedId();
}

void FExportBar::initUI()
{
	setObjectName("FExportBar");
	setAttribute(Qt::WA_StyledBackground);
	setFixedWidth(90);
	m_pBtnGroup = new QButtonGroup(this);
	m_pVLayout = new QVBoxLayout(this);
	CreateBtn(FE_BTN_LOCAL, "FSExportBar_Local");
	CreateBtn(FE_BTN_PHONE, "FSExportBar_Local");
	CreateBtn(FE_BTN_YUOTOBU, "FSExportBar_Local");
	CreateBtn(FE_BTN_FACEBOOK, "FSExportBar_Local");
	CreateBtn(FE_BTN_VIMEO, "FSExportBar_Local");
	CreateBtn(FE_BTN_DVD, "FSExportBar_Local");
	m_pBtnGroup->button(FE_BTN_LOCAL)->setChecked(true);
	m_pVLayout->addStretch(1);
	m_pVLayout->setMargin(0);
	m_pVLayout->setSpacing(0);
}

void FExportBar::CreateBtn(int btnId, const QString &ObjName)
{
	FButton *pBtn = new FButton(this);
	pBtn->setObjectName(ObjName);
	pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	pBtn->setFixedHeight(90);
	pBtn->setFixedWidth(90);

	m_pBtnGroup->addButton(pBtn, btnId);
	connect(pBtn, &FButton::clicked, [this](){
		FButton *pTempBtn = static_cast<FButton *>(sender());
		emit ButtonClicked(m_pBtnGroup->id(pTempBtn));
	});
	m_pVLayout->addWidget(pBtn);
}
