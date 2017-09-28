#include "stdafx.h"
#include "FMatterBottomBar.h"
#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FCommonLib/FCommonWidgets.h"
class QHBoxLayout;

FMatterBottomBar::FMatterBottomBar(QWidget *parent)
	: FBaseWidget(parent)
{
	setFocusPolicy(Qt::ClickFocus);
	FMatterBottomBarItem *pItem = nullptr;
	m_Items.append(pItem = new FMatterBottomBarItem(Matter_Media, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("MEDIA"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_MEDIA");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_Music, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("MUSIC"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_MUSIC");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_Text, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("TEXT/CREDIT"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_TEXT/CREDIT");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_Transition, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("TRANSITIONS"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_TRANSITIONS");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_Filter, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("FILTERS"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_FILTERS");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_Overlay, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("OVERLAYS"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_OVERLAYS");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_Element, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("ELEMENTS"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_ELEMENTS");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_SplitScreen, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::slotItemClicked);
	pItem->SetText(tr("SPLIT SCREEN"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_SPLIT SCREEN");

	m_Items.append(pItem = new FMatterBottomBarItem(Matter_SplitScreen + 1, this));
	connect(pItem, &FMatterBottomBarItem::Clicked, this, &FMatterBottomBar::sigExport);
	pItem->SetText(tr("EXPORT"));
	pItem->SetImageLabelObjectName("FMatterBottomBarItem_EXPORT");
}

FMatterBottomBar::~FMatterBottomBar()
{

}

void FMatterBottomBar::resizeEvent(QResizeEvent *)
{
	int left, top;
	int ItemWidth = (width() - 0.02 * 2 * width() - (m_Items.count() - 1) * 0.015*width()) / (1.0*m_Items.count());  // 0.02w*2 + 9x + 0.015w*8 = w
	qDebug() << "ItemWidth=" << ItemWidth;
	left = 0.02*width();
	top = 0;
	for (auto pItem : m_Items){
		pItem->setGeometry(left, top, ItemWidth, height());
		left = pItem->geometry().right() + 0.015*width();
	}
}

void FMatterBottomBar::slotItemClicked(int id)
{
	emit sigMatterTypeChanged((MatterType)id);
}

/*FMatterBottomBarItem*/
FMatterBottomBarItem::FMatterBottomBarItem(int id /*= 0*/, QWidget *parent /*= 0*/)
	:FBaseWidget(parent)
	, m_Id(id)
{
	m_pImageLbl = new FLabel(this);
	m_pTextLbl = new FLabel(this);
	m_pTextLbl->setAlignment(Qt::AlignCenter);
}

FMatterBottomBarItem::~FMatterBottomBarItem()
{

}

void FMatterBottomBarItem::SetText(const QString& text)
{
	QString elideText = m_pTextLbl->fontMetrics().elidedText(text, Qt::ElideRight, m_pTextLbl->width(), Qt::TextShowMnemonic);
	m_pTextLbl->setText(elideText);
}

void FMatterBottomBarItem::SetImageLabelObjectName(const QString& name)
{
	m_pImageLbl->setObjectName(name);
}

void FMatterBottomBarItem::resizeEvent(QResizeEvent *)
{
	int left, top;
	left = top = 0;
	m_pImageLbl->setGeometry(left, top, width(), height() - 25);
	top = m_pImageLbl->geometry().bottom() + 5;
	m_pTextLbl->setGeometry(left, top, width(), 20);
}

void FMatterBottomBarItem::mousePressEvent(QMouseEvent *)
{
	emit Clicked(m_Id);
}
