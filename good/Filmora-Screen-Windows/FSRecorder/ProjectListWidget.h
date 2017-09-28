#ifndef PROJECTLISTWIDGET_H
#define PROJECTLISTWIDGET_H

#include <QWidget>
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSCommonlib/ListWidgetBase.h"
#include "inc_FSCommonlib/ListItemDataBase.h"


enum ItemType
{
	ItemType_NewProj =  Global_AddNewItemType,
	ItemType_OpenProj,
};

class ListItemData;
class ProjectListWidget : public MVListWidget
{
	Q_OBJECT
public:
	ProjectListWidget(QWidget *parent);
	void Setup();

Q_SIGNALS:
	void sigItemClicked(MVListItemData *item);
	void sigItemDoubleClicked(MVListItemData *item);
};

struct ProjectFileInfo;
class OpenProjData : public ItemDataBase
{
public:
	OpenProjData(ProjectFileInfo* pInfo,QListWidget *parent = 0);
	ProjectFileInfo* GetProjectFileInfo();

private:
	ProjectFileInfo* m_pInfo;
};


class OpenProjItemWidget : public MVItemWidgetBase
{
	Q_OBJECT
public:
	OpenProjItemWidget(QWidget *parent = 0);
};


#endif // PROJECTLISTWIDGET_H
