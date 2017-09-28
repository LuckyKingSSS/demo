#include "stdafx.h"
#include "FSExportBar.h"
#include <inc_FSCommonlib/FSCommonWidget.h>
#include <QVBoxLayout>
#include <QButtonGroup>

FSExportBar::FSExportBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("FSExportBar");
    setAttribute(Qt::WA_StyledBackground);
    setFixedWidth(60);
    mpBtnGroup = new QButtonGroup(this);
    mpVL       = new QVBoxLayout(this);
    CreateBtn(FSE_BTN_LOCAL, "FSExportBar_Local");
	mpBtnGroup->button(FSE_BTN_LOCAL)->setChecked(true);
    //CreateBtn(FSE_BTN_PHONE, "FSExportBar_Phone");
    //CreateBtn(FSE_BTN_YUOTOBU, "FSExportBar_YuoTobu");
    //CreateBtn(FSE_BTN_FACEBOOK, "FSExportBar_FaceBook");
    //mpBtnGroup->button(FSE_BTN_LOCAL)->setChecked(true);
    mpVL->addStretch(1); 
    mpVL->setMargin(0);
    mpVL->setSpacing(0);
}

FSExportBar::~FSExportBar()
{

}

FSExportBar::FSExportBtn FSExportBar::GetCurrentBtn()
{
    return (FSExportBtn)mpBtnGroup->checkedId();
}

void FSExportBar::CreateBtn(int btnId, const QString &ObjName)
{
    FSButton *pBtn = new FSButton(this);
    pBtn->setObjectName(ObjName);
    pBtn->setCheckable(true);
    pBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBtn->setFixedHeight(45);
	pBtn->setFixedWidth(60);

    mpBtnGroup->addButton(pBtn, btnId);
    connect(pBtn, &FSButton::clicked, [this](){
        FSButton *pTempBtn = static_cast<FSButton *>(sender());
        emit ButtonClicked(mpBtnGroup->id(pTempBtn));
    });
    mpVL->addWidget(pBtn);
}
