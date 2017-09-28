#include "stdafx.h"
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "FPathSelectEdit.h"


FPathSelectEdit::FPathSelectEdit(QWidget *parent)
	: FLineEdit(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	initUI();
	initConnectios();
}

FPathSelectEdit::~FPathSelectEdit()
{

}

void FPathSelectEdit::initUI()
{
	m_FindTargetBtn = new FButton(nullptr);
	m_FindTargetBtn->setFixedSize(20,20);
	m_FindTargetBtn->setObjectName("FPathSelectEdit_Btn");
	m_hLayout = new QHBoxLayout();
	m_hLayout->addStretch();
	m_hLayout->addWidget(m_FindTargetBtn);
	m_hLayout->setSpacing(0);
	m_hLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(m_hLayout);
}

void FPathSelectEdit::initConnectios()
{
	connect(m_FindTargetBtn, &FButton::clicked, this, [&](){

		m_strSelectedPath = QFileDialog::getExistingDirectory(this, tr("Please select directory"), "C:\\Users\\ws\\Documents\\Wondershare Filmora\\Output\\");
		setText(m_strSelectedPath);
	});
}

void FPathSelectEdit::paintEvent(QPaintEvent *event)
{

	FLineEdit::paintEvent(event);
}
