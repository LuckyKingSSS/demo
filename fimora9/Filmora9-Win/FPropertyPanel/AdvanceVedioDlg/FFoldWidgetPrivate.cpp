#include "stdafx.h"
#include  <QtWidgets>
#include "FFoldWidgetPrivate.h"
#include "FFoldWidgetItem.h"

FFoldWidgetPrivate::FFoldWidgetPrivate(QWidget *parent, bool bShowMode) : FBaseWidget(parent), m_bExclusiveShow(bShowMode)
{
	InitUI();
	InitData();
	InitConnections();
}

FFoldWidgetPrivate::~FFoldWidgetPrivate()
{

}

void FFoldWidgetPrivate::AddItem(FFoldWidgetItem* item)
{
	if (m_bExclusiveShow)
	{
		item->SetShowMode(Mode_ShowSingle);
	}
	else
	{
		item->SetShowMode(Mode_ShowMulti);
	}
	/*m_VboxLayout->addWidget(item);*/
	int iListSize = m_AllShrinkWidget.size();
	item->SetIndex(iListSize + 1);
	m_AllShrinkWidget.push_back(item);
	connect(item, &FFoldWidgetItem::sigOnCollapse, this, [&, item](bool bState){
		PreCheckItemList(item);
		collapseItem();
	});
}

void FFoldWidgetPrivate::resizeEvent(QResizeEvent *e)
{
	collapseItem();
}

void FFoldWidgetPrivate::InitUI()
{
	//m_VboxLayout = new QVBoxLayout();
	//m_VboxLayout->setContentsMargins(0, 0, 0, 0);
	//m_VboxLayout->setSpacing(2);
	//setLayout(m_VboxLayout);
	//resize(256, 300);
}

void FFoldWidgetPrivate::InitData()
{
	m_bIsItemShow = false;
}

void FFoldWidgetPrivate::InitConnections()
{

}

void FFoldWidgetPrivate::PreCheckItemList(FFoldWidgetItem* item)
{
	const int iHeaderHeight = 30;
	if (m_bExclusiveShow)
	{
		int iIndex = item->GetIndex();
		int iCurIndex = 1;
		QList<FFoldWidgetItem*>::iterator iter = m_AllShrinkWidget.begin();
		for (; iter != m_AllShrinkWidget.end(); ++iter)
		{
			if (iCurIndex != iIndex)
			{
				(*iter)->HideContainWidget();
				(*iter)->setFixedHeight(iHeaderHeight);
			}
			else
			{
				(*iter)->ShowContainWidget();
				int iHeight = (*iter)->GetItemCentralWidget()->height();
				(*iter)->setFixedHeight(iHeaderHeight + iHeight);
			}
			iCurIndex++;
		}
	}
}

void FFoldWidgetPrivate::AddItems(const QList<FFoldWidgetItem*> items)
{
	for (auto item : items)
	{
		AddItem(item);
	}
}

void FFoldWidgetPrivate::InsertItem(int iRow, FFoldWidgetItem* item)
{


}

void FFoldWidgetPrivate::InsertItems(int iRow, const QList<FFoldWidgetItem*> items)
{
  
}

FFoldWidgetItem* FFoldWidgetPrivate::TakeItem(int iRow)
{
	FFoldWidgetItem* item;
	m_AllShrinkWidget.removeAt(iRow);
	item = m_AllShrinkWidget.at(iRow);
	return item;
}

FFoldWidgetItem* FFoldWidgetPrivate::Item(int iRow) const
{
	return m_AllShrinkWidget.at(iRow);
}

void FFoldWidgetPrivate::collapseItem()
{
	int i = 0;
	int iHeight = 0;
	int iWidth = 0;
	int iTop = 0;
	const int iSpace = 2;
	int iWidgetHeight = 0;
	for (auto item : m_AllShrinkWidget)
	{
		iHeight = item->height();
		item->setGeometry(0, iTop, width(), iHeight);
		iTop = item->geometry().bottom() + iSpace;
		i++;
		iWidgetHeight = item->geometry().bottom();
	}
	int iTmp = width();
	resize(iTmp, iWidgetHeight);
	//更新外层窗口的大小
	emit sigUpdateSize(iWidgetHeight);
}
