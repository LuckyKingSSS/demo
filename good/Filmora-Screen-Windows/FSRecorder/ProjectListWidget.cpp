#include "stdafx.h"
#include "ProjectListWidget.h"
#include "ProjectFilesMgr.h"

ProjectListWidget::ProjectListWidget(QWidget *parent)
	: MVListWidget(parent)
{
	SetItemSize(160, 107);
	SetSingleSelect(true);
	setDragEnabled(false);
    SetSpacing(50);
	connect(this, &QListWidget::itemClicked, this, [&](QListWidgetItem* item){
		emit sigItemClicked((MVListItemData *)item);
	});
	connect(this, &QListWidget::itemDoubleClicked, this, [&](QListWidgetItem* item){
		emit sigItemDoubleClicked((MVListItemData *)item);
	});
	Setup();
    setAttribute(Qt::WA_TranslucentBackground);
}


void ProjectListWidget::Setup()
{
	QList<ProjectFileInfo*> infos = ProjectFilesMgr::GetInstance()->GetInfos();

	auto database = new AddNewItemData;
	database->SetTitle(tr("Create New"));
	database->SetThumbnail(QPixmap(":/FSRecorder/Resources/light/add_new_item.png"));
	auto newitemData = new MVListItemData(database, 0);
	auto newitemWidget = new AddNewItemWidget(this);
    newitemWidget->setObjectName("ProjectListWidget_AddNewItemWidget");
	this->AddItem(newitemData, newitemWidget);
    newitemWidget->setToolTip(tr("To start new workflow for recording and editing"));
    int num = 0;
	for (auto info : infos)
	{
		auto projdata= new OpenProjData(info,this);
		projdata->SetTitle(info->GetBaseName());
		projdata->SetThumbnail(QPixmap(info->imgpath));
		auto itemData = new MVListItemData(projdata, 0);
		auto itemWidget = new OpenProjItemWidget(this);
        itemWidget->setObjectName("ProjectListWidget_itemWidget");
		this->AddItem(itemData, itemWidget);
        itemWidget->setToolTip(tr("Double click to open project"));
        num++;
	}
}


//////////////////////////////////////////////////////////////////////////
OpenProjData::OpenProjData(ProjectFileInfo* pInfo, QListWidget *parent /*= 0*/)
	: ItemDataBase(ItemType_OpenProj, parent)
{
	m_pInfo = pInfo;
}

ProjectFileInfo* OpenProjData::GetProjectFileInfo()
{
	return m_pInfo;
}

//////////////////////////////////////////////////////////////////////////
OpenProjItemWidget::OpenProjItemWidget(QWidget *parent /*= 0*/)
	: MVItemWidgetBase(parent)
{
	m_pLbIcon->setVisible(false);
	SetRenamable(false);
}

