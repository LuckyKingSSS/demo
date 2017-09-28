#include "stdafx.h"
#include <QPushButton>
#include "..\include\inc_FCommonLib\FCommonWidgets.h"
#include "FMotionListItemWidget.h"


FMotionListItemWidget::FMotionListItemWidget(QWidget *parent, const QString strFilePath) : FBaseWidget(parent), m_StrBackgroundPath(strFilePath)
{
	InitUI();
	setAttribute(Qt::WA_TransparentForMouseEvents);
}

FMotionListItemWidget::~FMotionListItemWidget()
{

}

void FMotionListItemWidget::SetIconWidgetImage(const QIcon icon)
{
	m_IConWidget->setStyleSheet("background: ");
}

QRect FMotionListItemWidget::GetIconWidgetSize()
{
	QRect ItemRect = this->geometry();
	QRect IconRect = m_IConWidget->geometry();

	ItemRect.setHeight(IconRect.height());
	ItemRect.setWidth(IconRect.width());
	return ItemRect;
}

void FMotionListItemWidget::resizeEvent(QResizeEvent *event)
{
	//const int iFixHeight = 20;
	//int iWidth = width();
	//int iHeight = height();
	//m_IConWidget->setGeometry(0, 0, iWidth, iHeight - iFixHeight);
	////m_IConWidget->setContentsMargins(0, 0, 0, 0);
	//m_Icon = m_Icon.scaled(iWidth, iHeight - iFixHeight);
	//m_IConWidget->setIconSize(QSize(iWidth, iHeight - iFixHeight));
	//m_IConWidget->setIcon(m_Icon);
	//m_Text->setGeometry(0, iHeight - iFixHeight,iWidth,iFixHeight);
}

void FMotionListItemWidget::InitUI()
{
	const int iFixHeight = 20;
	m_IConWidget = new FLabel(nullptr);
	m_IConWidget->setStyleSheet("border-image:url(G:/Code/Filmora-Screen-Windows/bin/x64/Release/MotionTemplate/Bounce to zoom in 1.JPG)");
	m_IConWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//m_IConWidget->setIconSize(QSize(m_IConWidget->width(), m_IConWidget->height()));
	m_Text = new FLabel(nullptr);
	m_Text->setText(tr("testetetetetetet"));
	m_Text->setAlignment(Qt::AlignHCenter| Qt::AlignHCenter);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(m_IConWidget);
	vLayout->addWidget(m_Text);
	vLayout->setContentsMargins(0, 0, 0, 0);
	m_Text->setFixedHeight(iFixHeight);
	setLayout(vLayout);
}
