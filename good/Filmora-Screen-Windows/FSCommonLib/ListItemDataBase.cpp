#include "stdafx.h"
#include "inc_CommonLib/commonlib_global.h"
#include "inc_FSCommonlib/ListItemDataBase.h"
#include "inc_FSCommonlib/FSNLETimeline.h"

MVListItemData::MVListItemData(ItemDataBase *pItemDataBase, QListWidget *view)
	: QListWidgetItem(view)
{
	m_pItemDataBase = pItemDataBase;
}

MVListItemData::~MVListItemData()
{
	SAFE_DELETE(m_pItemDataBase);
}

ItemDataBase * MVListItemData::GetItemDataBase()
{
	return m_pItemDataBase;
}

//////////////////////////////////////////////////////////////////////////
ItemDataBase::ItemDataBase(int type, QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_type = type;
	m_pTimeline = nullptr;
}

ItemDataBase::~ItemDataBase()
{
	SAFE_DELETE(m_pTimeline);
}

QString ItemDataBase::GetTitle()
{
	return m_strTitle;
}

void ItemDataBase::SetTitle(const QString title)
{
	m_strTitle = title;
}

QPixmap ItemDataBase::GetThumbnail()
{
	return m_pxThumbnail;
}

void ItemDataBase::SetThumbnail(QPixmap px)
{
	m_pxThumbnail = px;
}

QPixmap ItemDataBase::GetIcon()
{
	return m_pxIcon;
}

void ItemDataBase::SetIcon(QPixmap px)
{
	m_pxIcon = px;
}

FSNLETimeline* ItemDataBase::GetTimeline()
{
	return m_pTimeline;
}

void ItemDataBase::SetTimeline(FSNLETimeline* timeline)
{
	m_pTimeline = timeline;
}

int ItemDataBase::GetType()
{
	return m_type;
}

//////////////////////////////////////////////////////////////////////////
AddNewItemData::AddNewItemData(QObject *parent /*= 0*/)
	:ItemDataBase(Global_AddNewItemType, parent)
{

}