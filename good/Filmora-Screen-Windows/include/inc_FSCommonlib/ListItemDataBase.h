#ifndef LISTITEMDATABASE_H
#define LISTITEMDATABASE_H

#include <QObject>
#include <QListWidgetItem>
#include "inc_FSCommonlib/fscommonlib_global.h"


#define  Global_AddNewItemType	0

class FSNLETimeline;
class ItemDataBase;
class FSCOMMONLIB_EXPORT MVListItemData : public QListWidgetItem
{
public:
	MVListItemData(ItemDataBase *pItemDataBase, QListWidget *view);
	virtual ~MVListItemData();
	ItemDataBase *GetItemDataBase();
private:
	ItemDataBase *m_pItemDataBase;
};

class FSCOMMONLIB_EXPORT ItemDataBase : public QObject
{
	Q_OBJECT
public:
	ItemDataBase(int type, QObject *parent = 0);
	virtual ~ItemDataBase();
	QString GetTitle();
	void SetTitle(const QString title);
	QPixmap GetThumbnail();
	void SetThumbnail(QPixmap px);
	QPixmap GetIcon();
	void SetIcon(QPixmap px);
	virtual FSNLETimeline* GetTimeline();
	void SetTimeline(FSNLETimeline* timeline);
	int GetType();
protected:
	QString m_strTitle;
	QPixmap m_pxThumbnail;
	QPixmap m_pxIcon;
	FSNLETimeline* m_pTimeline;
	int m_type;
};

class FSCOMMONLIB_EXPORT AddNewItemData : public ItemDataBase
{
	Q_OBJECT
public:
	AddNewItemData(QObject *parent = 0);
};

#endif // LISTITEMDATABASE_H
