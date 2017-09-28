#include "stdafx.h"
#include "FSMatterViewPrivate.h"
#include "MVListWidget.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "INLETimelineBase.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_CommonLib/PathOperation.h"
#include "NLEKey.h"
#include "cMouseStreamCtrl.h"
#include "inc_FSCommonlib/MediaImportDlg.h"
#include "MVListItemData.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "MVGlobal.h"
#include "inc_CommonLib/StringOperation.h"
#include "inc_FSCommonlib/TimelinePropertyDialog.h"
#include "inc_FSCommonlib/gathread.h"
#include <QtConcurrent/QtConcurrent>
#define  ID_MEDIA			0
#define  ID_ANNOTATION		1
#define  ID_MOUSEEFFECTT	2


//Video files *.MP4; *.FLV; *.AVI; *.VOB; *.3GP; *.ASF; *.MOV; *.F4V; *.M4V; *.MPG; *.MPEG; *.DPG; *.TS; *.TRP; *.M2TS; *.MTS; *.DAT; *.WMV; *.MKV; 
//*.DVR-MS; *.DV; *.TOD; *.EVO; *.RM; *.RMVB; *.DIVX; *.OGV; *.VRO
//Audio files *.AAC; *.AC3; *.AIF; *.AIFF; *.AU; *.FLAC; *.M4A; *.MKA; *.MP2; *.MP3; *.OGG; *.WAV; *.WMA; *.CAF; *.RA; *.CUE
//Image files *.BMP; *.JPG; *.PNG; *.GIF; *.TIF; *.ICO; *.DIB; *.JPEG; *.JPE; *.JFIF; *.TIFF; *.EMF
QStringList sVideoFilters()
{
	QStringList filters;
	filters << "*.mp4" << "*.flv" << "*.avi" << "*.vob" << "*.3pg" << "*.asf" << "*.mov" << "*.f4v" << "*.m4v" << "*.mpg"
		<< "*.mpeg" << "*.dpg" << "*.ts" << "*.trp" << "*.m2ts" << "*.mts" << "*.dat" << "*.wmv" << "*.mkv" << "*.dvr-ms"
		<< "*.dv" << "*.tod" << "*.evo" << "*.rm" << "*.rmvb" << "*.divx" << "*.ogv" << "*.vro";
	return filters;
}
QStringList sAudioFilters()
{
	QStringList filters;
	filters << "*.aac" << "*.ac3" << "*.aif" << "*.aiff" << "*.au" << "*.flac" << "*.m4a" << "*.mka" << "*.mp2" << "*.mp3" << "*.ogg"
		<< "*.wav" << "*.wma" << "*.caf" << "*.ra" << "*.cue";
	return filters;
}
QStringList sImageFilters()
{
	QStringList filters;
	filters << "*.bmp" << "*.jpg" << "*.png" << "*.gif" << "*.tif" << "*.ico" << "*.dib" << "*.jpeg" << "*.jpe" << "*.jfif" << "*.tiff" << "*.emf";
	return filters;
}

QString sGetVideoFilterString()
{
	QString str;
	for (auto one : sVideoFilters())
	{
		str += (one + " ");
	}
	return str;
}
	
QString sGetAudioFilterString()
{
	QString str;
	for (auto one : sAudioFilters())
	{
		str += (one + " ");
	}
	return str;
}

QString sGetImageFilterString()
{
	QString str;
	for (auto one : sImageFilters())
	{
		str += (one + " ");
	}
	return str;
}

QString sGetMediaFilterString()
{
	QString s1 = QObject::tr("All supported files") + "(" + sGetVideoFilterString() + sGetAudioFilterString() + sGetImageFilterString() + ");;";
	QString s2 = QObject::tr("Videos") + "(" + sGetVideoFilterString() + ");;";
	QString s3 = QObject::tr("Audios") + "(" + sGetAudioFilterString() + ");;";
	QString s4 = QObject::tr("Images") + "(" + sGetImageFilterString() + ")";
	QString s5 = QObject::tr("All Files") + "(*.*)";
	return s1 + s2 + s3 + s4 /*+ s5*/;
}
bool sIsIamgeFile(const QString &file)
{
	if (!QFile::exists(file)) return false;
	QFileInfo info(file);
	if (sGetImageFilterString().contains(info.suffix())) return true;
	return false;
}
bool sIsVideoFile(const QString &file)
{
	if (!QFile::exists(file)) return false;
	QFileInfo info(file);
	if (sGetVideoFilterString().contains(info.suffix())) return true;
	return false;
}

bool sIsAudioFile(const QString &file)
{
	if (!QFile::exists(file)) return false;
	QFileInfo info(file);
	if (sGetAudioFilterString().contains(info.suffix())) return true;
	return false;
}

static int sPageTitleHeight = 40;

FSMatterViewPrivate::FSMatterViewPrivate(QWidget *parent)
	: FSBaseWidget(parent)
{

	m_pMatterViewManager = MatterViewManager::GetInstance();
	MatterViewManager::GetInstance()->SetFSMatterViewPrivate(this);
	m_pMediaPage = new MediaPage(this);
	m_pAnnotationPage = new AnnotationPage(this);
	m_pMouseEffectPage = new MouseEffectPage(this);
	m_pStackedWidget = new QStackedWidget(this);
	m_pStackedWidget->addWidget(m_pMediaPage);
	m_pStackedWidget->addWidget(m_pAnnotationPage);
	m_pStackedWidget->addWidget(m_pMouseEffectPage);
    m_pStackedWidget->setObjectName("FSMatterViewPrivate_m_pStackedWidget");
    m_pCategoryBar = new CategoryBar("FSMatterView_CategoryBar", this);
	QTimer::singleShot(0, [&]{
		m_pCategoryBar->AddItem(ID_MEDIA, "FSMatterView_CategoryBar_MediaItem", tr("Media"), "Media");
        m_pCategoryBar->AddItem(ID_ANNOTATION, "FSMatterView_CategoryBar_AnnotationItem", tr("Annotations"), "Annotations");
        m_pCategoryBar->AddItem(ID_MOUSEEFFECTT, "FSMatterView_CategoryBar_MouseEffectItem", tr("Cursor"), "Cursor");
		ConnectSignals();
		m_pCategoryBar->SelectItem(ID_MEDIA);
	});
	qApp->installEventFilter(this);

    m_pBtnShowHideMatterView = new FSStateBaseButton(0, "", this); 
    m_pBtnShowHideMatterView->AddButton(0, "FSMatterViewPrivate_ShowMatterViewBtn");
    m_pBtnShowHideMatterView->AddButton(1, "FSMatterViewPrivate_pHideMatterViewBtn");

    connect(m_pBtnShowHideMatterView, &FSStateBaseButton::Clicked, this, [&](int id){
        SetShowHideButtonState(id == 1);
    });
    QTimer::singleShot(0, [&](){
        SetShowHideButtonState(true);
    });

    QShortcut *shortcutCtrlLeft = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left), this);
    shortcutCtrlLeft->setContext(Qt::ApplicationShortcut);
    connect(shortcutCtrlLeft, &QShortcut::activated, this, [&](){
        bool bid = m_pBtnShowHideMatterView->CurrentButtonId();    
        SetShowHideButtonState(bid);
        m_pBtnShowHideMatterView->SetCurrentButtonId(bid ? 0 : 1);
    });



	connect(m_pMediaPage, &MediaPage::sigActivate, this, [&](){
		m_pCategoryBar->SelectItem(ID_MEDIA);
	});

    RetranslateUi();
}

FSMatterViewPrivate::~FSMatterViewPrivate()
{

}

IFSMatterViewManager * FSMatterViewPrivate::GetMatterViewManager()
{
	return m_pMatterViewManager;
}

QString FSMatterViewPrivate::GetProjectString()
{
	return m_pMediaPage->GetConfigString();
}

void FSMatterViewPrivate::LoadProjectString(QString strMatter)
{
	m_pMediaPage->LoadProjectString(strMatter);
}

MediaPage* FSMatterViewPrivate::GetMediaPage()
{
	return m_pMediaPage;
}

void FSMatterViewPrivate::UnselectAll()
{
	m_pMediaPage->UnselectAllItem();
	m_pAnnotationPage->UnselectAllItem();
	m_pMouseEffectPage->UnselectAllItem();
}

void FSMatterViewPrivate::ConnectSignals()
{
	connect(m_pCategoryBar, &CategoryBar::sigItemChanged, this, [&](int oldid,int newid){

		if (oldid == ID_MEDIA)
			m_pMediaPage->UnselectAllItem();
		else if (oldid == ID_ANNOTATION)
			m_pAnnotationPage->UnselectAllItem();
		else if (oldid == ID_MOUSEEFFECTT)
			m_pMouseEffectPage->UnselectAllItem();

		QWidget *pCurWidget = nullptr;
		if (newid == ID_MEDIA)
			pCurWidget = m_pMediaPage;
		else if (newid == ID_ANNOTATION)
			pCurWidget = m_pAnnotationPage;
		else if (newid == ID_MOUSEEFFECTT)
			pCurWidget = m_pMouseEffectPage;
		if (pCurWidget) m_pStackedWidget->setCurrentWidget(pCurWidget);
	});
}

bool FSMatterViewPrivate::eventFilter(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mse = (QMouseEvent*)e;
		auto pos = mapFromGlobal(mse->globalPos());
		if (geometry().contains(pos))
		{
			emit m_pMatterViewManager->sigFocusIn();
		}
	}
	return false;
}

void FSMatterViewPrivate::resizeEvent(QResizeEvent *e)
{
	static const int CategoryBarH = 40;
	m_pCategoryBar->setGeometry(0, 0, width(), CategoryBarH);
	m_pStackedWidget->setGeometry(0, CategoryBarH, width(), height() - CategoryBarH);

}

void FSMatterViewPrivate::RetranslateUi()
{
    for (CategoryBarButton* item : m_pCategoryBar->GetItems())
    {
        auto bytearray = item->property("QByteArray").toByteArray();
        item->SetText(tr(bytearray.constData()));
    }

    m_pBtnShowHideMatterView->GetButton(0)->setToolTip(tr("Hide(Ctrl+Left)"));
    m_pBtnShowHideMatterView->GetButton(1)->setToolTip(tr("Show(Ctrl+Right)"));
}

void FSMatterViewPrivate::SetShowHideButtonState(bool showState)
{
    emit MatterViewManager::GetInstance()->sigShowMaximum(showState);
    m_pCategoryBar->setVisible(showState);
    m_pStackedWidget->setVisible(showState);
    m_pBtnShowHideMatterView->setFixedSize(showState ? QSize(35, 35) : QSize(24, 35));
    m_pBtnShowHideMatterView->move(width() - m_pBtnShowHideMatterView->width(), (m_pCategoryBar->height()-m_pBtnShowHideMatterView->height())/2+m_pCategoryBar->geometry().top());
    m_pBtnShowHideMatterView->raise();
}

//////////////////////////////////////////////////////////////////////////
static const char* sMatterRootTag = "MatterMessage";
static const char* sMediaInfoTag = "MediaInfo";
static const char* sPathAttr = "Path";
static const char* sTitleAttr = "Title";

#define MenuItemH	22
#define ItemNameRole		Qt::UserRole + 1
#define ItemDurationRole	Qt::UserRole + 2
#define ItemTypeRole		Qt::UserRole + 3
#define ItemDateCreateRole	Qt::UserRole + 4

MediaPage::MediaPage(QWidget *parent)
	:FSBaseWidget(parent)
{
    setObjectName("MediaPage");
    m_strFileLostTip = tr("The file does not exist and can not be opened.");
	m_curSortAction = SortAction_Name;
	m_bDeleteConfirm = true;
	m_pListWidget = new MVListWidget(this);
	m_pListWidget->setObjectName("FSMatterView_MediaPage_ListWidget");
	m_pListWidget->SetSpacing(17);
	m_pListWidget->SetSingleSelect(false);
	m_pBtnImport = new BaseButton("FSMatterView_MediaPage_ImportButton", this);
	m_pBtnFilter = new BaseButton("FSMatterView_MediaPage_FilterButton", this);
	m_pBtnSort = new BaseButton("FSMatterView_MediaPage_SortButton", this);
	m_pActImportFile = new QAction(this);
	m_pActImportFolder = new QAction(this);
	m_pActFilterAll = new QAction(this);
	m_pActFilterAll->setCheckable(true);
	m_pActFilterAll->setChecked(true);
	m_pActFilterVideo = new QAction(this);
	m_pActFilterVideo->setCheckable(true);
	m_pActFilterAudio = new QAction(this);
	m_pActFilterAudio->setCheckable(true);
	m_pActFilterImage = new QAction(this);
	m_pActFilterImage->setCheckable(true);
	m_pActSortName = new QAction(this);
	m_pActSortName->setCheckable(true);
	m_pActSortName->setChecked(true);
	m_pActSortDuration = new QAction(this);
	m_pActSortDuration->setCheckable(true);
	m_pActSortType = new QAction(this);
	m_pActSortType->setCheckable(true);
	m_pActSortDateCreated = new QAction(this);
	m_pActSortDateCreated->setCheckable(true);
	m_pActSortAscending = new QAction(this);
	m_pActSortAscending->setCheckable(true);
	m_pActSortAscending->setChecked(true);
	m_pActSortDescending = new QAction(this);
	m_pActSortDescending->setCheckable(true);
	m_pActAdd2Timeline = new QAction(this);
	m_pActRename = new QAction(this);
	m_pActDelete = new QAction(this);
	m_pActFindTarget = new QAction(this);
    m_pActDetails = new QAction(this);

	m_pMenuImport = new QMenu(this);
	m_pMenuImport->setFixedWidth(260);
	m_pMenuImport->addAction(m_pActImportFile);
	m_pMenuImport->addAction(m_pActImportFolder);
	m_pMenuFilter = new QMenu(this);
	m_pMenuFilter->setFixedWidth(180);
	m_pMenuFilter->addAction(m_pActFilterAll);
	m_pMenuFilter->addAction(m_pActFilterVideo);
	m_pMenuFilter->addAction(m_pActFilterAudio);
	m_pMenuFilter->addAction(m_pActFilterImage);
	m_pMenuSort = new QMenu(this);
	m_pMenuSort->setFixedWidth(180);
	m_pMenuSort->addAction(m_pActSortName);
	m_pMenuSort->addAction(m_pActSortDuration);
	m_pMenuSort->addAction(m_pActSortType);
	m_pMenuSort->addAction(m_pActSortDateCreated);	
	m_pMenuSort->addSeparator();
	m_pMenuSort->addAction(m_pActSortAscending);
	m_pMenuSort->addAction(m_pActSortDescending);
	m_pMenuItemRClicked = new QMenu(this);
    //m_pMenuItemRClicked->setFixedWidth(245);
	m_pMenuItemRClicked->addAction(m_pActAdd2Timeline);
	m_pMenuItemRClicked->addAction(m_pActRename);
	m_pMenuItemRClicked->addAction(m_pActDelete);
	m_pMenuItemRClicked->addSeparator();
    m_pMenuItemRClicked->addAction(m_pActDetails);
    m_pMenuItemRClicked->addAction(m_pActFindTarget);

	//setContentsMargins(0,0,0,0);
    m_lbTitle = new FSPageTitle(this);

	DeleteItemConfirm();
	InitList();
	ConnectSignals();
	RetranslateUi();
	connect(MatterViewManager::GetInstance(), &MatterViewManager::sigCBImportMediaFiles, this, &MediaPage::ImportMediaFiles);
	connect(MatterViewManager::GetInstance(), &MatterViewManager::sigCBImportMediaFolder, this, &MediaPage::ImportMediaFolder);
}

QString MediaPage::GetConfigString()
{
	QDomDocument doc;
	auto root = doc.createElement("MatterMessage");
	doc.appendChild(root);

	QList<MVListItemData *>items = m_pListWidget->Items();
	for (auto item : items)
	{
		MediaData* pMediaData = dynamic_cast<MediaData*>(item->GetItemDataBase());
		if (!pMediaData) continue;
		auto elem = XmlOperation::AddXmlElement(doc, root, "MediaInfo", "");
		elem.setAttribute("Path", pMediaData->GetSourceFile());
		elem.setAttribute("Title", pMediaData->GetTitle());
	}
	QString str = doc.toString();
	return str;
}

bool MediaPage::CanDelete()
{
	return m_pListWidget->selectedItems().size() > 0;
}

void MediaPage::Delete()
{
	DeleteSelectedItems();
}

void MediaPage::ImportMediaFiles()
{
    auto files = QFileDialog::getOpenFileNames(GetEditorMainView(), tr("Open"), GetPreOpenDir(), sGetMediaFilterString());
	if (files.isEmpty())
	{
		m_pListWidget->ItemConfuseFix();
		return;
	}
	m_strPreOpenDir = files[0];
    SavePreOpenDir(m_strPreOpenDir);
	AddItems(files);
	emit((IFSMatterViewManager*)MatterViewManager::GetInstance())->sigContentChanged(true);
	//记录用户的操作信?
	GAThread::getInstance()->insertEventItem(Import, "Import Source", "MediaFile");
	int iFileCounts = files.length();
	GAThread::getInstance()->insertEventItem(Import, "Import files Count", QString::number(iFileCounts));
}

void MediaPage::ImportMediaFolder()
{
	QString strDir = PathOperation::GetUserSelectDir(tr("Please select directory"), this->winId());
	if (!strDir.isEmpty()) m_strPreOpenDir = strDir;
	QDir dir(strDir);
	QStringList filters = sVideoFilters() + sAudioFilters() + sImageFilters();
	QStringList lstFiles = dir.entryList(filters, QDir::Files);
	for (QString &one : lstFiles)
		one = strDir + "/" + one;
	AddItems(lstFiles);
	if (!lstFiles.isEmpty()) emit((IFSMatterViewManager*)MatterViewManager::GetInstance())->sigContentChanged(true);
	//记录用户的操作信?
	GAThread::getInstance()->insertEventItem(Import, "Import Source", "MediaFolder");
	int iFileCounts = lstFiles.length();
	GAThread::getInstance()->insertEventItem(Import, "Import files Count", QString::number(iFileCounts));
}

void MediaPage::ConnectSignals()
{
	connect(MVGlobal::GetInstance(), &MVGlobal::sigImportMediaItems, this, [&](QStringList files){
		emit sigActivate();
		AddItems(files);
		if (!files.isEmpty()) emit((IFSMatterViewManager*)MatterViewManager::GetInstance())->sigContentChanged(true);
	});
	connect(m_pListWidget, &MVListWidget::sigItemRClicked, this, [&](QPoint pt, MVListItemData *pItemData){
		if (pItemData->GetItemDataBase()->GetType() == MVItemDataType_AddNewItem)
			return;
		m_pCurClickItem = pItemData;

        int w = GetRetranslateUiTextLenght(m_pActAdd2Timeline->font(), m_pActAdd2Timeline->text(), 200 - 70, 20) + 70;

        m_pMenuItemRClicked->setFixedWidth(w);
		m_pMenuItemRClicked->popup(pt);
	});
	connect(m_pListWidget, &MVListWidget::sigBlankRClicked, this, [&](QPoint pt){
		m_pMenuImport->popup(pt);
	});
	connect(m_pListWidget, &MVListWidget::itemClicked, this, [&](QListWidgetItem* item){
		MVListItemData *itemData = dynamic_cast<MVListItemData*>(item);
		if (itemData->GetItemDataBase()->GetType() != MVItemDataType_AddNewItem)
			return;
		ImportMediaFiles();
	});
	connect(m_pListWidget, &MVListWidget::itemDoubleClicked, this, [&](QListWidgetItem* item){
		MVListItemData *itemData = dynamic_cast<MVListItemData*>(item);
		if (itemData->GetItemDataBase()->GetType() == MVItemDataType_AddNewItem)
			return;
		MatterViewManager::GetInstance()->sigLoad(itemData->GetItemDataBase()->GetTimeline());
	});
	connect(m_pBtnImport, &BaseButton::clicked, this, [&]{
		QPoint pt = m_pBtnImport->geometry().bottomLeft();
		m_pMenuImport->exec(mapToGlobal(pt));
		UpdateButtons();
	});
	connect(m_pBtnFilter, &BaseButton::clicked, this, [&]{
		QPoint pt = m_pBtnFilter->geometry().bottomLeft();
		m_pMenuFilter->exec(mapToGlobal(pt));
		UpdateButtons();
	});
	connect(m_pBtnSort, &BaseButton::clicked, this, [&]{
		QPoint pt = m_pBtnSort->geometry().bottomLeft();
		m_pMenuSort->exec(mapToGlobal(pt));
		UpdateButtons();
	});
	connect(m_pActImportFile, &QAction::triggered, this, &MediaPage::ImportMediaFiles);
	connect(m_pActImportFolder, &QAction::triggered, this, &MediaPage::ImportMediaFolder);
	connect(m_pActFilterAll, &QAction::triggered, this, [&]{
		OnFilterAll();
	});
	connect(m_pActFilterVideo, &QAction::triggered, this, [&]{
		OnFilterVideo();
	});
	connect(m_pActFilterAudio, &QAction::triggered, this, [&]{
		OnFilterAudio();
	});
	connect(m_pActFilterImage, &QAction::triggered, this, [&]{
		OnFilterImage();	
	});
	connect(m_pActSortName, &QAction::triggered, this, [&]{
		SetSortActionChecked(SortAction_Name);
		SortByName();
	});
	connect(m_pActSortDuration, &QAction::triggered, this, [&]{
		SetSortActionChecked(SortAction_Duration);
		SortByDuration();
	});
	connect(m_pActSortType, &QAction::triggered, this, [&]{
		SetSortActionChecked(SortAction_Type);
		SortByType();
	});
	connect(m_pActSortDateCreated, &QAction::triggered, this, [&]{
		SetSortActionChecked(SortAction_DateCreate);
		SortByDateCreate();
	});
	connect(m_pActSortAscending, &QAction::triggered, this, [&]{
		SetSortActionChecked(SortAction_Ascending);
		SortByCurrentAction();
	});
	connect(m_pActSortDescending, &QAction::triggered, this, [&]{
		SetSortActionChecked(SortAction_Descending);
		SortByCurrentAction();
	});

	connect(m_pActAdd2Timeline, &QAction::triggered, this, [&]{
		MediaData* pMediaData = dynamic_cast<MediaData*>(m_pCurClickItem->GetItemDataBase());
		auto cloneTl = pMediaData->GetTimeline()->Clone();
		cloneTl->SetRenderFrameRate(25.0);
		cloneTl->_FetchThumbnail(0);
		emit MatterViewManager::GetInstance()->sigAdd(cloneTl);
	});
	connect(m_pActRename, &QAction::triggered, this, [&]{
		MVItemWidgetBase* pItemWidget = qobject_cast<MVItemWidgetBase*>(m_pListWidget->itemWidget(m_pCurClickItem));
		if (pItemWidget) pItemWidget->Rename();
	});
	connect(m_pActDelete, &QAction::triggered, this, [&]{
		DeleteSelectedItems();
	});
    connect(m_pActDetails, &QAction::triggered, this, [&]{
        MediaData* pMediaData = dynamic_cast<MediaData*>(m_pCurClickItem->GetItemDataBase());
        if (!pMediaData) return;
        TimelinePropertyDialog dlg(this);
        dlg.DoModal(pMediaData->GetTimeline());
    }); 
    
    connect(m_pActFindTarget, &QAction::triggered, this, [&]{
		MediaData* pMediaData = dynamic_cast<MediaData*>(m_pCurClickItem->GetItemDataBase());
		if (!pMediaData) return;
		FSFindTargetFile(pMediaData->GetSourceFile());
	});
	connect(m_pListWidget, &QListWidget::itemSelectionChanged, MatterViewManager::GetInstance(), &IFSMatterViewManager::sigMediaSelectChanged);
}

void MediaPage::InitList()
{
	InsertNewItem();
	//if (m_strConfig.isEmpty()) return;
	//QDomDocument doc;
	//QString strError;
	//if (!doc.setContent(m_strConfig, &strError))
	//	return;
	//QList<MediaConfInfo> files;
	//QDomElement rootElem = doc.documentElement();
	//QDomElement mediaElem = rootElem.firstChildElement(sMediaInfoTag);
	//while (!mediaElem.isNull())
	//{
	//	MediaConfInfo info;
	//	info.path = mediaElem.attribute(sPathAttr);
	//	info.title = mediaElem.attribute(sTitleAttr);
	//	files.push_back(info);
	//	mediaElem = mediaElem.nextSiblingElement(sMediaInfoTag);
	//}
	//AddItems(files);
}

void MediaPage::InsertNewItem()
{
	auto *pData = new AddNewItemData();
	pData->SetThumbnail(QPixmap(":/FSMatterView/light/Resources/light/add_new_files.png"));
	pData->SetTitle(tr("Add Files"));
	auto *pItemdata = new MVListItemData(pData, /*m_pListWidget*/0);   //μú?t??2?êy±?D??a0￡?·??òinsert2???è￥
	auto *pItemWidget = new AddNewItemWidget(m_pListWidget);
	connect(pItemWidget, &AddNewItemWidget::sigMouseEnter, m_pListWidget, [&]{
		if(m_pListWidget->count() == 1) m_pListWidget->SetNeedItemFix(true); 
	});

	//m_pListWidget->AddItem(pItemdata, pItemWidget);
	m_pListWidget->InsertItem(0, pItemdata, pItemWidget);
}

void MediaPage::AddVideoItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline)
{
	if (!QFile::exists(fileinfo.path)) return;

	if (timeline->GetSourceFramerate() > 30.0)
	{
		timeline->SetRenderFrameRate(30.0);
	}
	
	auto *pData = new VideoData;
	pData->SetSourceFile(fileinfo.path);
	//pData->SetThumbnail(timeline->GetThumbnail());
	pData->SetTitle(fileinfo.title);
	pData->SetIcon(QPixmap(":/FSMatterView/light/Resources/light/video.png"));
	pData->SetTimeline(timeline);
	auto *pItemdata = new MVListItemData(pData, /*m_pListWidget*/0);
	auto *pItemWidget = new VideoItemWidget(m_pListWidget);
	pItemWidget->setToolTip(QString("%1\r\n%2").arg(pData->GetTitle()).arg(timeline->GetDurationString()));
	m_pListWidget->AddItem(pItemdata, pItemWidget);

	connect(timeline, &FSNLETimeline::sigFetchedThumbnail, this, [&, pData,pItemWidget,timeline](FSNLETimeline * tl){
		qDebug() << "---------" << tl;
		pData->SetThumbnail(timeline->GetThumbnail()); 
		pItemWidget->Resize();
	});
	QtConcurrent::run([this, timeline]{
		timeline->_FetchThumbnail(); 
	});

	connect(pItemWidget, &MVItemWidgetBase::sigAddBtnClicked, this, [&](MVItemWidgetBase* item){
		if (QFile::exists(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetSourceFilePath()))
		{
			auto cloneTl = item->GetItemData()->GetItemDataBase()->GetTimeline()->Clone();
			cloneTl->SetRenderFrameRate(25.0);
			cloneTl->_FetchThumbnail(0);
			emit MatterViewManager::GetInstance()->sigAdd(cloneTl);
		}
	});
	connect(pItemWidget, &MVItemWidgetBase::sigEndEdit, this, [&](MVItemWidgetBase* item,QString text){
		item->GetItemData()->GetItemDataBase()->GetTimeline()->SetDisplayName(text);
		emit MatterViewManager::GetInstance()->sigDisplayNameChanged(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetAncestralGuidString(), text);
	});
	connect(pItemWidget, &VideoItemWidget::sigSourceFileLost, this, [&](MVItemWidgetBase* item,QString file){
		FSMessageBox::informationOk(tr("Filmora Scrn"), m_strFileLostTip, this);
		m_pListWidget->RemoveItem(item->GetItemData());
	});
	connect(pItemWidget, &MediaItemWidget::sigDrag, this, &MediaPage::OnItemDrag);
}

void MediaPage::AddAudioItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline)
{
	if (!QFile::exists(fileinfo.path)) return;

	auto *pData = new AudioData;
	pData->SetSourceFile(fileinfo.path);
    QPixmap pixmap = GetThumbnailImageFromFile(fileinfo.path, 150, 150);
    if (pixmap.isNull())
        pixmap = QPixmap(":/FSMatterView/light/Resources/light/RecordeAudio.png");
    pData->SetThumbnail(pixmap);
	pData->SetTitle(fileinfo.title);
    pData->SetIcon(QPixmap(":/FSMatterView/light/Resources/light/audio.png"));
	pData->SetTimeline(timeline);

	auto *pItemdata = new MVListItemData(pData, /*m_pListWidget*/0);
	auto *pItemWidget = new AudioItemWidget(m_pListWidget);
	pItemWidget->setToolTip(QString("%1\r\n%2").arg(pData->GetTitle()).arg(timeline->GetDurationString()));
	m_pListWidget->AddItem(pItemdata, pItemWidget);
	connect(pItemWidget, &MVItemWidgetBase::sigAddBtnClicked, this, [&](MVItemWidgetBase* item){
		if (QFile::exists(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetSourceFilePath()))
		{
			auto cloneTl = item->GetItemData()->GetItemDataBase()->GetTimeline()->Clone();
			cloneTl->SetRenderFrameRate(25.0);
			cloneTl->_FetchThumbnail(0);
			emit MatterViewManager::GetInstance()->sigAdd(cloneTl);
		}
	});
	connect(pItemWidget, &MVItemWidgetBase::sigEndEdit, this, [&](MVItemWidgetBase* item, QString text){
		item->GetItemData()->GetItemDataBase()->GetTimeline()->SetDisplayName(text);
		emit MatterViewManager::GetInstance()->sigDisplayNameChanged(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetAncestralGuidString(), text);
	});
	connect(pItemWidget, &AudioItemWidget::sigSourceFileLost, this, [&](MVItemWidgetBase* item, QString file){
		FSMessageBox::informationOk(tr("Filmora Scrn"), m_strFileLostTip, this);
		m_pListWidget->RemoveItem(item->GetItemData());
	});
	connect(pItemWidget, &MediaItemWidget::sigDrag, this, &MediaPage::OnItemDrag);
}

void MediaPage::AddImageItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline)
{
	if (!QFile::exists(fileinfo.path)) return;

	timeline->SetRange(0, 25 * 20);

	auto *pData = new ImageData;
	pData->SetSourceFile(fileinfo.path);
	//pData->SetThumbnail(/*QPixmap(fileinfo.path)*/timeline->GetThumbnail());
	pData->SetTitle(fileinfo.title);
	pData->SetIcon(QPixmap(":/FSMatterView/light/Resources/light/image.png"));
	pData->SetTimeline(timeline);
	auto *pItemdata = new MVListItemData(pData, /*m_pListWidget*/0);
	auto *pItemWidget = new ImageItemWidget(m_pListWidget);
	pItemWidget->setToolTip(pData->GetTitle());
	m_pListWidget->AddItem(pItemdata, pItemWidget);

	connect(timeline, &FSNLETimeline::sigFetchedThumbnail, this, [&, pData, pItemWidget, timeline](FSNLETimeline * tl){
		pData->SetThumbnail(timeline->GetThumbnail());
		pItemWidget->Resize();
	});
	QtConcurrent::run([this, timeline]{
		timeline->_FetchThumbnail();
	});

	connect(pItemWidget, &MVItemWidgetBase::sigAddBtnClicked, this, [&](MVItemWidgetBase* item){
		if (QFile::exists(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetSourceFilePath()))
		{
			auto cloneTl = item->GetItemData()->GetItemDataBase()->GetTimeline()->Clone();
			cloneTl->SetRenderFrameRate(25.0);
			cloneTl->_FetchThumbnail(0);
			emit MatterViewManager::GetInstance()->sigAdd(cloneTl);
		}
	});
	connect(pItemWidget, &MVItemWidgetBase::sigEndEdit, this, [&](MVItemWidgetBase* item, QString text){
		item->GetItemData()->GetItemDataBase()->GetTimeline()->SetDisplayName(text);
		emit MatterViewManager::GetInstance()->sigDisplayNameChanged(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetAncestralGuidString(), text);
	});
	connect(pItemWidget, &ImageItemWidget::sigSourceFileLost, this, [&](MVItemWidgetBase* item, QString file){
		FSMessageBox::informationOk(tr("Filmora Scrn"), m_strFileLostTip, this);
		m_pListWidget->RemoveItem(item->GetItemData());
	});
	connect(pItemWidget, &MediaItemWidget::sigDrag, this, &MediaPage::OnItemDrag);
}

void MediaPage::AddRecordItem(const MediaConfInfo &fileinfo, FSNLETimeline *timeline)
{
	if (!QFile::exists(fileinfo.path)) return;
	if (timeline->GetSourceFramerate() > 30.0)
	{
		timeline->SetRenderFrameRate(30.0);
	}
	//AncestralGuid
	bool bHasVideo = false;
	QList<NLEComPtr<INLETimelineBase>> timelines = NLEWraper::GetInstance().GetPlayerMaterProvider()->GetSubTimelines(timeline->GetSrc());
	for (auto srctl : timelines)
	{
		FSNLETimeline fstl(srctl);
		fstl.SetAncestralGuidString(timeline->GetGuidString());
		fstl.SetRecordAncestralGuidString(fstl.GetGuidString());
		
		fstl.SetType(fstl.GetNLETimelineType(),false);
		if (fstl.IsDesktopVideo() || fstl.IsGameVideo() || fstl.IsCameraVideo())
		{
			if (bHasVideo == false)
				bHasVideo = true;
		}
	}

	auto *pData = new RecordData;
	pData->SetSourceFile(fileinfo.path);
	if (!bHasVideo)
		pData->SetThumbnail(QPixmap(":/FSMatterView/light/Resources/light/RecordeAudio.png"));
	pData->SetTitle(fileinfo.title);
	pData->SetIcon(QPixmap(bHasVideo ? ":/FSMatterView/light/Resources/light/video.png" : ":/FSMatterView/light/Resources/light/audio.png"));
	pData->SetTimeline(timeline);
	auto *pItemdata = new MVListItemData(pData, /*m_pListWidget*/0);
	auto *pItemWidget = new RecordItemWidget(m_pListWidget);
	pItemWidget->setToolTip(QString("%1\r\n%2").arg(pData->GetTitle()).arg(timeline->GetDurationString()));
	m_pListWidget->AddItem(pItemdata, pItemWidget);

	if (bHasVideo)
	{
		connect(timeline, &FSNLETimeline::sigFetchedThumbnail, this, [&, pData, pItemWidget, timeline](FSNLETimeline * tl){
			pData->SetThumbnail(timeline->GetThumbnail());
			pItemWidget->Resize();
		});
		QtConcurrent::run([this, timeline]{
			timeline->_FetchThumbnail();
		});
	}

	connect(pItemWidget, &MVItemWidgetBase::sigAddBtnClicked, this, [&](MVItemWidgetBase* item){
		if (QFile::exists(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetSourceFilePath()))
		{
			auto cloneTl = item->GetItemData()->GetItemDataBase()->GetTimeline()->Clone();
			cloneTl->SetRenderFrameRate(25.0);
			cloneTl->_FetchThumbnail(0);
			emit MatterViewManager::GetInstance()->sigAdd(cloneTl);
		}
	});
	connect(pItemWidget, &MVItemWidgetBase::sigEndEdit, this, [&](MVItemWidgetBase* item, QString text){
		item->GetItemData()->GetItemDataBase()->GetTimeline()->SetDisplayName(text);
		emit MatterViewManager::GetInstance()->sigDisplayNameChanged(item->GetItemData()->GetItemDataBase()->GetTimeline()->GetAncestralGuidString(), text);
	});
	connect(pItemWidget, &RecordItemWidget::sigSourceFileLost, this, [&](MVItemWidgetBase* item, QString file){
		FSMessageBox::informationOk(tr("Filmora Scrn"), m_strFileLostTip, this);
		m_pListWidget->RemoveItem(item->GetItemData());
	});
	connect(pItemWidget, &MediaItemWidget::sigDrag, this, &MediaPage::OnItemDrag);
}

void MediaPage::AddItems(QStringList files)
{
	GAThread::getInstance()->startTime();
	OnFilterAll();
	CheckFiles(files);
	MediaImportDlg dlg(files, this);
	connect(&dlg, &MediaImportDlg::sigImported, this, [&](QString srcfile, FSNLETimeline *fstimeline){
		MediaConfInfo info;
		info.path = srcfile;
		QFileInfo fileinfo(srcfile);
		info.title = fileinfo.completeBaseName();
		TimelineType type = fstimeline->GetNLETimelineType();
		fstimeline->SetType(type,false);
		fstimeline->SetAncestralGuidString(fstimeline->GetGuidString());
		fstimeline->SetDisplayName(info.title);
		switch (type)
		{
		case TimelineType_Unknown:
			delete fstimeline;
			fstimeline = nullptr;
			break;
		case TimelineType_Video:
		case TimelineType_VideoAudio:
			AddVideoItem(info, fstimeline);
			break;
		case TimelineType_Audio:
			AddAudioItem(info, fstimeline);
			break;
		case TimelineType_Image:
			AddImageItem(info, fstimeline);
			break;
		case TimelineType_Complex:
			AddRecordItem(info, fstimeline);
			break;
		default:
			Q_ASSERT(false);
			break;
		}
		//上传用户数据
		if (type == TimelineType_Unknown || !QFile::exists(fileinfo.path()))
		{
            //加载资源文件失败
			QString strFileName = fileinfo.fileName();
			QString strFilePath = fileinfo.filePath();
			float fFileSize = (fileinfo.size() >> 20);
			QString strFileSize = QString::number(fFileSize);
			strFileSize.append("MB");
			QString strFileFormat = fileinfo.suffix();
			GAThread::getInstance()->insertEventItem(Import_Failed, "FileFormat", strFileFormat);
			GAThread::getInstance()->insertEventItem(Import_Failed, "FileName", strFileName);
			GAThread::getInstance()->insertEventItem(Import_Failed, "FileSize", strFileSize);
			GAThread::getInstance()->insertEventItem(Import_Failed, "FilePath", strFilePath);
		}
		//上传文件属性信息
		if (type == TimelineType_Video || type == TimelineType_VideoAudio || type == TimelineType_Complex)
		{
			GAThread::getInstance()->insertVedioFileInfo(fstimeline);
			GAThread::getInstance()->insertAudioInfo(fstimeline);
		}
		if (type == TimelineType_Audio)
		{
			GAThread::getInstance()->insertAudioInfo(fstimeline);
		}	
		if (type == TimelineType_Image)
		{
			//获取图片的分辨率
			NLESize picSize;
			fstimeline->GetProp()->GetSize(NLEKey::Source::kSource_Stream, &picSize);
			QString strPicSize =  GAThread::getInstance()->getPicResolutionRange(picSize);
			GAThread::getInstance()->insertEventItem(Import_Media, "PhotoResolutionRange", strPicSize);
		}
	});
	dlg.DoImport();
}

void MediaPage::AddItems(QList<MediaConfInfo> files)
{
	OnFilterAll();
	for (auto one : files)
	{
		auto timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateTimelineForScreenRecording(one.path);
		if (!timeline) continue;
		auto fstimeline = new FSNLETimeline(timeline);
		TimelineType type = fstimeline->GetNLETimelineType();
		fstimeline->SetType(type,false);
		fstimeline->SetAncestralGuidString(fstimeline->GetGuidString());
		fstimeline->SetDisplayName(one.title);

		switch (type)
		{
		case TimelineType_Unknown:
			delete timeline;
			break;
		case TimelineType_Video:
		case TimelineType_VideoAudio:
			AddVideoItem(one, fstimeline);
			break;
		case TimelineType_Audio:
			AddAudioItem(one, fstimeline);
			break;
		case TimelineType_Image:
			AddImageItem(one, fstimeline);
			break;
		case TimelineType_Complex:
			AddRecordItem(one, fstimeline);
			break;
		}
		//上传用户数据
		QFileInfo fileinfo(fstimeline->GetFilePath());
		if (type == TimelineType_Unknown || !QFile::exists(fileinfo.path()))
		{
			//加载资源文件失败
			QString strFileName = fileinfo.fileName();
			QString strFilePath = fileinfo.filePath();
			float fFileSize = (fileinfo.size() >> 20);
			QString strFileSize = QString::number(fFileSize);
			strFileSize.append("MB");
			QString strFileFormat = fileinfo.suffix();
			GAThread::getInstance()->insertEventItem(Import_Failed, "FileFormat", strFileFormat);
			GAThread::getInstance()->insertEventItem(Import_Failed, "FileName", strFileName);
			GAThread::getInstance()->insertEventItem(Import_Failed, "FileSize", strFileSize);
			GAThread::getInstance()->insertEventItem(Import_Failed, "FilePath", strFilePath);
		}

		//上传文件属性信息
		if (type == TimelineType_Video || type == TimelineType_VideoAudio || type == TimelineType_Complex)
		{
			GAThread::getInstance()->insertVedioFileInfo(fstimeline);
			GAThread::getInstance()->insertAudioInfo(fstimeline);
		}
		if (type == TimelineType_Audio)
		{
			GAThread::getInstance()->insertAudioInfo(fstimeline);
		}
		if (type == TimelineType_Image)
		{
			//获取图片的分辨率
			NLESize picSize;
			fstimeline->GetProp()->GetSize(NLEKey::Source::kSource_Stream, &picSize);
			QString strPicSize = GAThread::getInstance()->getPicResolutionRange(picSize);
			GAThread::getInstance()->insertEventItem(Import_Media, "PhotoResolutionRange", strPicSize);
		}
	}
}

void MediaPage::CheckFiles(QStringList &files)
{
	QStringList lstExist = GetItemSourceFiles();
	QStringList lst2remove;
	for (auto one : files)
	{
		if (lstExist.contains(one) || !QFile::exists(one))
			lst2remove.push_back(one);
	}
	if (lst2remove.isEmpty()) return;
	for (auto one : lst2remove)
		files.removeOne(one);
}

void MediaPage::DeleteSelectedItems()
{
	if (m_pListWidget->selectedItems().isEmpty()) return;
	if (m_bDeleteConfirm)
	{
		std::tuple<bool, FSMessageBox::Button> ret = MVConfirmDlg::question(this);
		if (std::get<1>(ret) != FSMessageBox::Yes) return;
		if (std::get<0>(ret)) DisableDeleteItemConfirm();
	}
	m_pListWidget->RemoveSelectedItems();

	emit((IFSMatterViewManager*)MatterViewManager::GetInstance())->sigContentChanged(true);

}

QStringList MediaPage::GetItemSourceFiles()
{
	QStringList lstExist;
	QList<MVListItemData*> itemdatas = m_pListWidget->Items();
	for (auto data : itemdatas)
	{
		MediaData* pMediaData = dynamic_cast<MediaData*>(data->GetItemDataBase());
		if (!pMediaData) continue;
		lstExist.push_back(pMediaData->GetSourceFile());
	}
	return lstExist;
}

void MediaPage::RetranslateUi()
{
	m_pActImportFile->setText(tr("Import Media files..."));
	m_pActImportFolder->setText(tr("Import a Media folder..."));
	m_pActFilterAll->setText(tr("All"));
	m_pActFilterVideo->setText(tr("Video"));
	m_pActFilterAudio->setText(tr("Audio"));
	m_pActFilterImage->setText(tr("Image"));
	m_pActSortName->setText(tr("Name"));
	m_pActSortDuration->setText(tr("Duration"));
	m_pActSortType->setText(tr("Type"));
	m_pActSortDateCreated->setText(tr("Date Created"));
	m_pActSortAscending->setText(tr("Ascending"));
	m_pActSortDescending->setText(tr("Descending"));
	m_pActAdd2Timeline->setText(tr("Add to timeline at playhead"));
	m_pActRename->setText(tr("Rename"));
	m_pActDelete->setText(tr("Delete"));
    m_pActDetails->setText(tr("Details..."));
    m_pActFindTarget->setText(tr("Find target..."));
	auto lists = m_pListWidget->Items();
	auto pWidget = (MVItemWidgetBase*)m_pListWidget->itemWidget(lists[0]);
	pWidget->SetTitle(tr("Add Files"));

    m_pBtnImport->setToolTip(tr("Import"));
    m_pBtnFilter->setToolTip(tr("Filter"));
    m_pBtnSort->setToolTip(tr("Sort"));

    m_lbTitle->setText(tr("Media"));

	m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() + 5);
	m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() - 5);
}

void MediaPage::resizeEvent(QResizeEvent *e)
{
	static const int HeaderHeight = 40;
	int rightMargin = 2;
	int spacing = 8;
	int iBtnWidth = 19;
	int iBtnHeight = 19;

    m_lbTitle->setGeometry(0, 0, width(), sPageTitleHeight);

	int iLeftPos = width() -  (iBtnWidth * 3 + spacing * 2 + rightMargin);
	int iTopPos = (HeaderHeight - iBtnHeight) * 0.5 + m_lbTitle->geometry().bottom();

	m_pBtnImport->setGeometry(iLeftPos, iTopPos, iBtnWidth, iBtnHeight);
	iLeftPos = m_pBtnImport->geometry().right() + spacing;
	m_pBtnSort->setGeometry(iLeftPos, iTopPos, iBtnWidth, iBtnHeight);
	iLeftPos = m_pBtnSort->geometry().right() + spacing;
	m_pBtnFilter->setGeometry(iLeftPos, iTopPos, iBtnWidth, iBtnHeight);
    iTopPos = sPageTitleHeight + HeaderHeight;
    m_pListWidget->setGeometry(0, iTopPos, width(), height() - iTopPos);
}


QString MediaPage::GetPreOpenDir()
{
    QSettings settings(FSGetFilmoraScreenDirectory() + "/FilmoraScrn.ini", QSettings::IniFormat);
    settings.beginGroup("Common");
    QString preOpenDir = settings.value("MatterViewOpenDir").toString();
    settings.endGroup();
    return preOpenDir;
}

void MediaPage::SavePreOpenDir(QString)
{
    QSettings settings(FSGetFilmoraScreenDirectory() + "/FilmoraScrn.ini", QSettings::IniFormat);
    settings.beginGroup("Common");
    settings.setValue("MatterViewOpenDir", m_strPreOpenDir);
    settings.endGroup();
}

void MediaPage::OnFilterAll()
{
	SetFilterActionChecked(FilterAction_All);
	auto items = m_pListWidget->Items();
	for (auto item : items)
	{
		if (item->isHidden())
			m_pListWidget->SetItemHidden(item, false);
	}
	m_pListWidget->ItemConfuseFix();
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() + 5);
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() - 5);
}

void MediaPage::OnFilterVideo()
{
	SetFilterActionChecked(FilterAction_Video);
	auto items = m_pListWidget->Items();
	for (auto item : items)
	{
		MediaData* pMediaData = dynamic_cast<MediaData*>(item->GetItemDataBase());
		if (!pMediaData) continue;
		MVItemDataType type = (MVItemDataType)pMediaData->GetType();
		if (type == MVItemDataType_Video)
		{
			m_pListWidget->SetItemHidden(item, false);
		}
		else if (type == MVItemDataType_Record)
		{
			m_pListWidget->SetItemHidden(item, item->GetItemDataBase()->GetTimeline()->IsPureAudio());
		}
		else
			m_pListWidget->SetItemHidden(item, true);
	}
	m_pListWidget->ItemConfuseFix();
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() + 5);
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() - 5);
}

void MediaPage::OnFilterAudio()
{
	SetFilterActionChecked(FilterAction_Audio);
	auto items = m_pListWidget->Items();
	for (auto item : items)
	{
		MediaData* pMediaData = dynamic_cast<MediaData*>(item->GetItemDataBase());
		if (!pMediaData) continue;
		MVItemDataType type = (MVItemDataType)pMediaData->GetType();
		if (type == MVItemDataType_Audio)
		{
			m_pListWidget->SetItemHidden(item, false);
		}
		else if (type == MVItemDataType_Record)
		{
			m_pListWidget->SetItemHidden(item, !(item->GetItemDataBase()->GetTimeline()->IsPureAudio()));
		}
		else
			m_pListWidget->SetItemHidden(item, true);
	}
	m_pListWidget->ItemConfuseFix();
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() + 5);
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() - 5);
}

void MediaPage::OnFilterImage()
{
	SetFilterActionChecked(FilterAction_Image);
	auto items = m_pListWidget->Items();
	for (auto item : items)
	{
		MediaData* pMediaData = dynamic_cast<MediaData*>(item->GetItemDataBase());
		if (!pMediaData) continue;
		MVItemDataType type = (MVItemDataType)pMediaData->GetType();
		if (type == MVItemDataType_Imgage)
			m_pListWidget->SetItemHidden(item, false);
		else
			m_pListWidget->SetItemHidden(item, true);
	}
	m_pListWidget->ItemConfuseFix();
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() + 5);
	//m_pListWidget->resize(m_pListWidget->width(), m_pListWidget->height() - 5);
}

void MediaPage::OnItemDrag(MVListItemData *pData)
{
	auto items = m_pListWidget->selectedItems();
	if (items.isEmpty()) return;
	
	QList<FSNLETimeline*> list;
	for (auto item : items)
	{
		auto cloned = ((MVListItemData*)(item))->GetItemDataBase()->GetTimeline()->Clone();
		cloned->SetRenderFrameRate(25);
		cloned->_FetchThumbnail(0);
		list.push_back(cloned);
	}
	auto data = new QMimeData;
	data->setData("MatterView_BaseItem", "");
	data->setProperty("MatterView_BaseItem", qptrdiff(&list));
	QDrag drag(this);
	drag.setMimeData(data);
	if (drag.exec() == Qt::IgnoreAction)
		qDeleteAll(list);
}

void MediaPage::UnselectAllItem()
{
	m_pListWidget->UnselectAllItems();
}

QPixmap MediaPage::Get1stItemPixmap()
{
	auto items = m_pListWidget->Items();
	if (items.size() >= 2)
	{
		return items[1]->GetItemDataBase()->GetThumbnail();
	}
	return QPixmap();
}

void MediaPage::SetSortActionChecked(SortAction sa)
{
	switch (sa)
	{
	case MediaPage::SortAction_Name:
	case MediaPage::SortAction_Duration:
	case MediaPage::SortAction_Type:
	case MediaPage::SortAction_DateCreate:
		m_curSortAction = sa;
		m_pActSortName->setChecked(sa == SortAction_Name);
		m_pActSortDuration->setChecked(sa == SortAction_Duration);
		m_pActSortType->setChecked(sa == SortAction_Type);
		m_pActSortDateCreated->setChecked(sa == SortAction_DateCreate);
		break;
	case MediaPage::SortAction_Ascending:
	case MediaPage::SortAction_Descending:
		m_pActSortAscending->setChecked(sa == SortAction_Ascending);
		m_pActSortDescending->setChecked(sa == SortAction_Descending);
		break;
	}
}

void MediaPage::SetFilterActionChecked(FilterAction fa)
{
	m_pActFilterAll->setChecked(fa == FilterAction_All);
	m_pActFilterVideo->setChecked(fa == FilterAction_Video);
	m_pActFilterAudio->setChecked(fa == FilterAction_Audio);
	m_pActFilterImage->setChecked(fa == FilterAction_Image);
}

void MediaPage::UpdateButtons()
{
	m_pBtnFilter->update();
	m_pBtnImport->update();
	m_pBtnSort->update();
}

void MediaPage::LoadProjectString(QString strMatter)
{
	m_pListWidget->clear();
	InsertNewItem();
	m_strConfig = strMatter;
	if (m_strConfig.isEmpty()) return;

	QDomDocument doc;
	QString strError;
	if (!doc.setContent(m_strConfig, &strError))
		return;
	QList<MediaConfInfo> files;
	QDomElement rootElem = doc.documentElement();
	QDomElement mediaElem = rootElem.firstChildElement(sMediaInfoTag);
	while (!mediaElem.isNull())
	{
		MediaConfInfo info;
		info.path = mediaElem.attribute(sPathAttr);
		info.title = mediaElem.attribute(sTitleAttr);
		files.push_back(info);
		mediaElem = mediaElem.nextSiblingElement(sMediaInfoTag);
	}
	AddItems(files);
}

void MediaPage::SortByName()
{
	m_pListWidget->RemoveItem((MVListItemData*)m_pListWidget->item(0));

	m_pListWidget->setSortingEnabled(true);
	for (auto item : m_pListWidget->Items())
	{
		item->setData(Qt::DisplayRole, item->GetItemDataBase()->GetTitle());
	}
	m_pListWidget->sortItems(m_pActSortAscending->isChecked() ? Qt::AscendingOrder : Qt::DescendingOrder);
	m_pListWidget->setSortingEnabled(false);

	InsertNewItem();
}

void MediaPage::SortByDuration()
{
	m_pListWidget->RemoveItem((MVListItemData*)m_pListWidget->item(0));
	m_pListWidget->setSortingEnabled(true);
	for (auto item : m_pListWidget->Items())
	{
		auto type = item->GetItemDataBase()->GetType();
		if (type == MVItemDataType_Imgage)
		{
			item->setData(Qt::DisplayRole, 20);
		}
		else
		{
			item->setData(Qt::DisplayRole, item->GetItemDataBase()->GetTimeline()->GetDuration());
		}
	}
		
	m_pListWidget->sortItems(m_pActSortAscending->isChecked() ? Qt::AscendingOrder : Qt::DescendingOrder);
	m_pListWidget->setSortingEnabled(false);
	InsertNewItem();
}

void MediaPage::SortByType()
{
	m_pListWidget->RemoveItem((MVListItemData*)m_pListWidget->item(0));
	m_pListWidget->setSortingEnabled(true);
	for (auto item : m_pListWidget->Items())
	{
		auto type = item->GetItemDataBase()->GetType();
		if (type == MVItemDataType_Record)
		{
			auto isAudio = item->GetItemDataBase()->GetTimeline()->IsPureAudio();
			item->setData(Qt::DisplayRole, isAudio ? MVItemDataType_Audio : MVItemDataType_Video);
		}
		else
		{
			item->setData(Qt::DisplayRole,type);
		}
	}
	m_pListWidget->sortItems(m_pActSortAscending->isChecked() ? Qt::AscendingOrder : Qt::DescendingOrder);
	m_pListWidget->setSortingEnabled(false);
	InsertNewItem();
}

void MediaPage::SortByDateCreate()
{
	m_pListWidget->RemoveItem((MVListItemData*)m_pListWidget->item(0));
	m_pListWidget->setSortingEnabled(true);
	for (auto item : m_pListWidget->Items())
	{
		MediaData *pMediaData = dynamic_cast<MediaData *>(item->GetItemDataBase());
		Q_ASSERT(pMediaData);
		if (!pMediaData) continue;
		QFileInfo fileinfo(pMediaData->GetSourceFile());
		item->setData(Qt::DisplayRole, fileinfo.created());
	}
	m_pListWidget->sortItems(m_pActSortAscending->isChecked() ? Qt::AscendingOrder : Qt::DescendingOrder);
	m_pListWidget->setSortingEnabled(false);
	InsertNewItem();
}

void MediaPage::SortByCurrentAction()
{

	switch (m_curSortAction)
	{
	case MediaPage::SortAction_Name:
		SortByName();
		break;
	case MediaPage::SortAction_Duration:
		SortByDuration();
		break;
	case MediaPage::SortAction_Type:
		SortByType();
		break;
	case MediaPage::SortAction_DateCreate:
		SortByDateCreate();
		break;
	}
	
}

void MediaPage::DeleteItemConfirm()
{
	QString settingfile = FSGetSettingsPath();
	QDomDocument doc;
	XmlOperation::OpenXmlFile(doc, settingfile);
	auto rootElem = doc.documentElement();
	if (rootElem.isNull()) return;

	auto advElme = rootElem.firstChildElement("Advanced");
	if (advElme.isNull()) return;

	auto dontElem = advElme.firstChildElement("DontShowDeleteTip");
	if (!dontElem.isNull()) 
	{
		m_bDeleteConfirm = false;
	}
}

void MediaPage::DisableDeleteItemConfirm()
{
	m_bDeleteConfirm = false;

	QString settingfile = FSGetSettingsPath();
	QDomDocument doc;
	XmlOperation::OpenXmlFile(doc, settingfile);
	auto rootElem = doc.documentElement();
	if (rootElem.isNull()) return;

	auto advElme = rootElem.firstChildElement("Advanced");
	if (advElme.isNull()) return;

	auto dontElem = advElme.firstChildElement("DontShowDeleteTip");
	if (!dontElem.isNull()) return;

	XmlOperation::AddXmlElement(doc, advElme, "DontShowDeleteTip", "true");
	XmlOperation::SaveXmlContent2File(doc, settingfile);
}

//////////////////////////////////////////////////////////////////////////
AnnotationPage::AnnotationPage(QWidget *parent)
	:FSBaseWidget(parent)
{
    setObjectName("AnnotationPage");
    m_pListWidget = new MVListWidget(this);
    m_lbTitle = new FSPageTitle(this);
    m_pCombCatalog = new FSComboBox(this);
    m_pCombCatalog->setFocusPolicy(Qt::NoFocus);
    m_dataHelper = new AnnotationDataHelper(this);

	InitList();
	ConnectSignals();
    RetranslateUi();
}

void AnnotationPage::UnselectAllItem()
{
	m_pListWidget->UnselectAllItems();
}

void AnnotationPage::ConnectSignals()
{
    connect(m_pCombCatalog, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(OnShowItemFromCatalog(QString)));
}

void AnnotationPage::InitList()
{
	m_pListWidget->SetItemSize(115, 115);
	m_pListWidget->SetSingleSelect(true);
	m_pListWidget->SetSpacing(20);
    m_dataHelper->Load();

    for (auto item : m_dataHelper->GetItems())
    {
        AddItem(item);
    }
    auto catalogs = m_dataHelper->GetCatagoryStrings();
    for (auto temp : catalogs)
    {
        m_pCombCatalog->addItem(tr(temp.c_str()), QString::fromStdString(temp));
    }

    QString catalog;
    int index = 0;
    if (catalogs.size() > 1)
    {
        catalog = tr(catalogs[1].c_str());
        index = 1;
    }
    else
    {
        catalog = catalog[0];
    }
    m_pCombCatalog->blockSignals(true);
    m_pCombCatalog->setCurrentIndex(index);
    m_pCombCatalog->blockSignals(false);

    OnShowItemFromCatalog(catalog);
}


void AnnotationPage::AddItem(AnnotationItemData *item)
{
    auto *pItemdata = new MVListItemData(item, m_pListWidget);
    auto *pItemWidget = new AnnotationItemWidget(m_pListWidget);
    connect(pItemWidget, &MVItemWidgetBase::sigAddBtnClicked, this, [&](MVItemWidgetBase* item){
		auto cloneTl = item->GetItemData()->GetItemDataBase()->GetTimeline()->Clone();
		emit MatterViewManager::GetInstance()->sigAdd(cloneTl);
    });
    connect(pItemWidget, &MediaItemWidget::sigDrag, this, &AnnotationPage::OnItemDrag);

    connect(pItemWidget, &AnnotationItemWidget::sigMenuPopup, this, &AnnotationPage::OnMenuPopup);
    connect(pItemWidget, &AnnotationItemWidget::sigAddFavorite, this, [&](){OnAddFavorite(true); });
    connect(pItemWidget, &AnnotationItemWidget::sigRemoveFavorite, this, [&](){OnAddFavorite(false); });


    m_pListWidget->AddItem(pItemdata, pItemWidget);


}

void AnnotationPage::resizeEvent(QResizeEvent *e)
{
    m_lbTitle->setGeometry(0, 0, width(), sPageTitleHeight);
    int top = sPageTitleHeight + 10;
    m_pCombCatalog->setGeometry(20, top, 250, 30);
    top = m_pCombCatalog->geometry().bottom() + 2;
    m_pListWidget->setGeometry(0, top, width(), height() - top);
}

void AnnotationPage::RetranslateUi()
{
    auto items = m_pListWidget->Items();
    for (auto item : items)
    {
        auto bytearray = item->GetItemDataBase()->property("QByteArray").toByteArray();
        auto pItemWidget = qobject_cast<MVItemWidgetBase*>(m_pListWidget->itemWidget(item));
        pItemWidget->SetTitle(tr(bytearray.constData()));

        QString tooltip = QString("%1 \n%2").arg(tr(bytearray.constData())).arg(tr("Drag to the timeline to apply"));
        pItemWidget->setToolTip(tooltip);
    }
	m_dataHelper->RetranslateText();
	m_pCombCatalog->blockSignals(true);
	int index = 0;
	for (auto text : m_dataHelper->GetCatagoryStrings())
		m_pCombCatalog->setItemText(index++,tr(text.c_str()));

	m_pCombCatalog->blockSignals(false);
    m_lbTitle->setText(tr("Annotations"));
}

void AnnotationPage::TranslateText()
{
	//category
	tr("Captions");
	tr("Overlays");
	tr("Arrows");
	tr("Banners");
	tr("Bubbles");

	//Caption
    tr("Speech Bubble");
    tr("Text Arrow");
    tr("Arrow1");
    tr("Line");
    tr("Ellipse");
    tr("Square");
    tr("Text");
    tr("Round");
    tr("Triangle");
    tr("Pentagon");
    tr("Hexagon");
    tr("Arrow");

	//overlay
    tr("Artboard 1");
    tr("Artboard 2");
    tr("Artboard 3");
    tr("Artboard 4");
    tr("Artboard 5");
    tr("Artboard 6");
    tr("Artboard 7");
    tr("Artboard 8");
    tr("Artboard 9");
    tr("Artboard 10");
    tr("Artboard 11");
    tr("Artboard 12");
    tr("Artboard 13");
    tr("Artboard 14");
    tr("Artboard 15");
    tr("Artboard 16");
    tr("Artboard 17");

	//element
	tr("Arrow 1");
	tr("Arrow 2");
	tr("Arrow 3");
	tr("Arrow 4");
	tr("Arrow 5");
	tr("Arrow 6");
	tr("Arrow 7");
	tr("Arrow 8");
	tr("Arrow 9");
	tr("Arrow 10");
	tr("Arrow 11");
	tr("Arrow 12");
	tr("Arrow 13");
	tr("Arrow 14");
	tr("Arrow 15");
	tr("Arrow 16");
	tr("Arrow 17");
	tr("Arrow 18");
	tr("Arrow 19");
	
	tr("Banner 1");
	tr("Banner 2");
	tr("Banner 3");
	tr("Banner 4");
	tr("Banner 5");
	tr("Banner 6");
	tr("Banner 7");
	tr("Banner 8");
	tr("Banner 9");
	tr("Banner 10");
	tr("Banner 11");
	tr("Banner 12");

	tr("Bubble 1");
	tr("Bubble 2");
	tr("Bubble 3");
	tr("Bubble 4");
	tr("Bubble 5");
	tr("Bubble 6");
	tr("Bubble 7");
	tr("Bubble 8");
	tr("Bubble 9");
	tr("Bubble 10");
	tr("Bubble 11");
	tr("Bubble 12");
	tr("Bubble 13");
	tr("Bubble 14");
	tr("Bubble 15");
	tr("Bubble 16");

	tr("Medal 1");
	tr("Medal 2");
	tr("Medal 3");
	tr("Medal 4");
	tr("Medal 5");
	tr("Medal 6");
}

void AnnotationPage::OnItemDrag(MVListItemData *pData)
{
	auto items = m_pListWidget->selectedItems();
	if (items.isEmpty()) return;

	QList<FSNLETimeline*> list;
	for (auto item : items)
	{
        auto timeline = ((MVListItemData*)(item))->GetItemDataBase()->GetTimeline();
        if (timeline == nullptr)
            continue;
        auto cloned = timeline->Clone();
		cloned->SetRenderFrameRate(25);
		list.push_back(cloned);
	}
    if (!list.isEmpty())
    {
        auto data = new QMimeData;
        data->setData("MatterView_BaseItem", "");
        data->setProperty("MatterView_BaseItem", qptrdiff(&list));
        QDrag drag(this);
        drag.setMimeData(data);
        if (drag.exec() == Qt::IgnoreAction)
            qDeleteAll(list);
    }

}

void AnnotationPage::OnShowItemFromCatalog(QString catalog)
{
    catalog = m_pCombCatalog->currentData().toString();
    auto items = m_pListWidget->Items();
    for (auto item : items)
    {
        AnnotationItemData* itemData = qobject_cast<AnnotationItemData*>(item->GetItemDataBase());
        Q_ASSERT(itemData);
        m_pListWidget->SetItemHidden(item, !m_dataHelper->BelongCatalog(itemData, catalog));
        AnnotationItemWidget *itemWidth = qobject_cast<AnnotationItemWidget*>(m_pListWidget->itemWidget(item));
        itemWidth->ShowFavorite(catalog != m_dataHelper->GetFavoriteString(), m_dataHelper->IsFavorite(itemData));
    }
    //m_pListWidget->ItemConfuseFix();
}

void AnnotationPage::OnMenuPopup(QPoint point)
{
    AnnotationItemWidget *item = qobject_cast<AnnotationItemWidget*>(sender());
    Q_ASSERT(item);
    AnnotationItemData *itemdata = qobject_cast<AnnotationItemData*>(item->GetItemData()->GetItemDataBase());
    Q_ASSERT(itemdata);

    QMenu menu(this);
    auto actAddFavorite = menu.addAction(tr("Add to favorite"));
    auto actRemoveFavorite = menu.addAction(tr("Remove from favorite"));

    actAddFavorite->setVisible(!m_dataHelper->IsFavorite(itemdata));
    actRemoveFavorite->setVisible(m_dataHelper->IsFavorite(itemdata));

    menu.setFixedWidth(250);

    auto act = menu.exec(point);
    if (act == actAddFavorite)
    {
        m_dataHelper->AddItem2Favorite(itemdata);
    }
    else if (act == actRemoveFavorite)
    {
        m_dataHelper->RemoveItemFromFavorite(itemdata);
    }
    if (act)
    {
        OnShowItemFromCatalog(m_pCombCatalog->currentText());
        m_dataHelper->Save();
    }
}

void AnnotationPage::OnAddFavorite(bool add)
{
    AnnotationItemWidget *item = qobject_cast<AnnotationItemWidget*>(sender());
    Q_ASSERT(item);
    AnnotationItemData *itemdata = qobject_cast<AnnotationItemData*>(item->GetItemData()->GetItemDataBase());
    Q_ASSERT(itemdata);
    if (add)
    {
        m_dataHelper->AddItem2Favorite(itemdata);
    }
    else
    {
        m_dataHelper->RemoveItemFromFavorite(itemdata);
    }
    OnShowItemFromCatalog(m_pCombCatalog->currentText());
    m_dataHelper->Save();
  
}

//////////////////////////////////////////////////////////////////////////
#define CombTypeIndex_All		0
#define CombTypeIndex_Cursor	1
#define CombTypeIndex_LeftClick	2
MouseEffectPage::MouseEffectPage(QWidget *parent)
	:FSBaseWidget(parent)
{
	m_pLbType = new QLabel(this);
	m_pCombType = new FSComboBox(this);
	m_pCombType->addItem("");
	m_pCombType->addItem("");
	m_pCombType->addItem("");
	m_pLbType->hide();
	m_pCombType->hide();

	m_pListWidget = new MVListWidget(this);
    m_pListWidget->SetSpacing(17);
    m_lbTitle = new FSPageTitle(this);
	InitList();
	ConnectSignals();
	RetranslateUi();
    setObjectName("MouseEffectPage");
}

void MouseEffectPage::UnselectAllItem()
{
	m_pListWidget->UnselectAllItems();
}

void MouseEffectPage::ConnectSignals()
{
	connect(m_pCombType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboxIndextChanged(int)));
}

void MouseEffectPage::InitList()
{
	m_pListWidget->SetSingleSelect(true);
	InitHightline();
	InitLeftClick();
}

void MouseEffectPage::InitHightline()
{
	auto *pData = new CursorHightlineData;
	pData->SetThumbnail(QPixmap(":/FSMatterView/light/Resources/light/Hightline.png"));
	auto timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_CircleHighlight));
	auto fstimeline = new FSNLETimeline(timeline);
	fstimeline->SetAncestralGuidString(fstimeline->GetGuidString());
	fstimeline->SetType(TimelineType_MouseHightlineFilter);
	pData->SetTimeline(fstimeline);
	auto *pItemdata = new MVListItemData(pData, m_pListWidget);
	auto *pItemWidget = new MouseEffectItemWidget(m_pListWidget);
	m_pListWidget->AddItem(pItemdata, pItemWidget);
	connect(pItemWidget, &MouseEffectItemWidget::sigDragBegin, this, [&]{emit MatterViewManager::GetInstance()->sigMouseEffectDraging(true); });
	connect(pItemWidget, &MouseEffectItemWidget::sigDragEnd, this, [&]{emit MatterViewManager::GetInstance()->sigMouseEffectDraging(false); });
}

void MouseEffectPage::InitLeftClick()
{
	auto *pData = new ClickRingData;
	pData->SetThumbnail(QPixmap(":/FSMatterView/light/Resources/light/ClickRing.png"));
	auto timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_Rings));
	auto fstimeline = new FSNLETimeline(timeline);
	fstimeline->SetAncestralGuidString(fstimeline->GetGuidString());

	fstimeline->SetType(TimelineType_MouseClickRingsFilter);
	fstimeline->GetProp()->SetInt(NLEKey::Mouse::kMouse_Event, MouseEvent_L16_LeftClick);
	pData->SetTimeline(fstimeline);
	auto *pItemdata = new MVListItemData(pData, m_pListWidget);
	auto *pItemWidget = new MouseEffectItemWidget(m_pListWidget);
	m_pListWidget->AddItem(pItemdata, pItemWidget);
	connect(pItemWidget, &MouseEffectItemWidget::sigDragBegin, this, [&]{emit MatterViewManager::GetInstance()->sigMouseEffectDraging(true); });
	connect(pItemWidget, &MouseEffectItemWidget::sigDragEnd, this, [&]{emit MatterViewManager::GetInstance()->sigMouseEffectDraging(false); });

	auto pSoundData = new ClickSoundData;
	pSoundData->SetThumbnail(QPixmap(":/FSMatterView/light/Resources/light/ClickSound.png"));
	pSoundData->SetSoundFile(GetSoundFile());
	pItemdata = new MVListItemData(pSoundData, m_pListWidget);
	pItemWidget = new MouseEffectItemWidget(m_pListWidget);
	m_pListWidget->AddItem(pItemdata, pItemWidget);
	connect(pItemWidget, &MouseEffectItemWidget::sigDragBegin, this, [&]{emit MatterViewManager::GetInstance()->sigMouseEffectDraging(true); });
	connect(pItemWidget, &MouseEffectItemWidget::sigDragEnd, this, [&]{emit MatterViewManager::GetInstance()->sigMouseEffectDraging(false); });
}

QString MouseEffectPage::GetSoundFile()
{
	return PathOperation::MakeWorkPath("Sounds/ClickDown.wav");
}

void MouseEffectPage::RetranslateUi()
{
	m_pLbType->setText(tr("Type"));
	m_pCombType->setItemText(CombTypeIndex_All, tr("All"));
	m_pCombType->setItemText(CombTypeIndex_Cursor, tr("Cursor"));
	m_pCombType->setItemText(CombTypeIndex_LeftClick, tr("Left Click"));


    QString dragCursorTooltip = tr("Drag cursor effect to clips on timeline");
#define SET_TITLE(item, text) qobject_cast<MVItemWidgetBase*>(m_pListWidget->itemWidget(item))->SetTitle(text);
#define SET_TOOLTIP(item) item->setToolTip(QString("%1 \n%2").arg(item->GetItemDataBase()->GetTitle()).arg(dragCursorTooltip));

    auto items = m_pListWidget->Items();
    Q_ASSERT(items.size() == 3);
    Q_ASSERT(items[0]->GetItemDataBase()->GetTimeline()->GetType() == TimelineType_MouseHightlineFilter);
    Q_ASSERT(items[1]->GetItemDataBase()->GetTimeline()->GetType() == TimelineType_MouseClickRingsFilter);

    MVListItemData *data = items[0];

    SET_TITLE(data, tr("Cursor Highlight"));
    SET_TOOLTIP(data);

    data = items[1];
    SET_TITLE(data, tr("Left Click Rings"));
    SET_TOOLTIP(data);

    data = items[2];
    SET_TITLE(data, tr("Left Click Sound"));
    SET_TOOLTIP(data);

    m_lbTitle->setText(tr("Cursor"));
}

void MouseEffectPage::resizeEvent(QResizeEvent *e)
{
    m_lbTitle->setGeometry(0, 0, width(), sPageTitleHeight);
    m_pListWidget->setGeometry(0, sPageTitleHeight, width(), height() - sPageTitleHeight);
}


void MouseEffectPage::OnComboxIndextChanged(int index)
{
	auto items = m_pListWidget->Items();
	if (index == CombTypeIndex_All)
	{

		for (auto item : items)
			m_pListWidget->SetItemHidden(item, false);
	}
	else if (index == CombTypeIndex_Cursor)
	{
		for (auto item : items)
		{
			auto pItemData = item->GetItemDataBase();
			if (!pItemData) continue;
			MVItemDataType type = (MVItemDataType)pItemData->GetType();
			if (type == MVItemDataType_CursorHightline)
				m_pListWidget->SetItemHidden(item, false);
			else
				m_pListWidget->SetItemHidden(item, true);
		}
	}
	else if (index == CombTypeIndex_LeftClick)
	{
		for (auto item : items)
		{
			auto pItemData = item->GetItemDataBase();
			if (!pItemData) continue;
			MVItemDataType type = (MVItemDataType)pItemData->GetType();
			if (type == MVItemDataType_CursorHightline)
				m_pListWidget->SetItemHidden(item, true);
			else
				m_pListWidget->SetItemHidden(item, false);
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
MVConfirmDlg::MVConfirmDlg(QWidget *parent)
	:FSMessageBox(tr("Filmora Scrn"), tr("Are you sure to delete the selected resource?"),Qt::AlignRight, FSMessageBox::Yes | FSMessageBox::No, parent)
{
	m_pCbDontShow = new QCheckBox(this);
	m_pLbDonShow = new QLabel(this);
    m_pLbDonShow->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_pLbDonShow->setObjectName("FSConfirmDlg_m_pLbDonShow");
    m_pCbDontShow->setObjectName("FSConfirmDlg_m_pCbDontShow");
	m_pLbDonShow->setText(tr("Don't show it next time."));
}

std::tuple<bool, FSMessageBox::Button> MVConfirmDlg::question(QWidget *parent)
{
	MVConfirmDlg dlg(parent);
	int button = dlg.DoModal();
	bool bCheck = dlg.IsCheck();
	return std::make_tuple(bCheck, (FSMessageBox::Button)button);
}

bool MVConfirmDlg::IsCheck()
{
	return m_pCbDontShow->isChecked();
}

void MVConfirmDlg::resizeEvent(QResizeEvent *e)
{
	FSMessageBox::resizeEvent(e);
    QPoint lb = mapFromGlobal(m_lbText->mapToGlobal(m_lbText->geometry().bottomLeft()));
    int l = m_lbIcon->width() + 53;
    int t = m_lbIcon->height() + 28;
    m_pCbDontShow->setGeometry(l, t, 20, 20);
    m_pLbDonShow->setGeometry(m_pCbDontShow->geometry().right() + 5, t, width() - m_pCbDontShow->geometry().right() - 5, 20);
}




//////////////////////////////////////////////////////////////////////////
MatterViewManager::MatterViewManager(QObject *parent /*= 0*/)
{
	m_pFSMatterViewPrivate = nullptr;
}

MatterViewManager* MatterViewManager::GetInstance()
{
	static MatterViewManager ins;
	return &ins;
}

void MatterViewManager::SetFSMatterViewPrivate(FSMatterViewPrivate* pFSMatterViewPrivate)
{
	m_pFSMatterViewPrivate = pFSMatterViewPrivate;
}

void MatterViewManager::ImportMediaFiles()
{
	emit sigCBImportMediaFiles();
}

void MatterViewManager::ImportMediaFolder()
{
	emit sigCBImportMediaFolder();
}

void MatterViewManager::UnselectAll()
{
	m_pFSMatterViewPrivate->UnselectAll();
}

bool MatterViewManager::CanDelete()
{
	return m_pFSMatterViewPrivate->GetMediaPage()->CanDelete();
}

void MatterViewManager::Delete()
{
	m_pFSMatterViewPrivate->GetMediaPage()->Delete();
}

QString MatterViewManager::GetProjectString()
{
	return m_pFSMatterViewPrivate->GetProjectString();
}

void MatterViewManager::LoadProjectString(QString strMatter)
{
	m_pFSMatterViewPrivate->LoadProjectString(strMatter);
}

void MatterViewManager::Clear()
{
	m_pFSMatterViewPrivate->LoadProjectString("");
}

QPixmap MatterViewManager::Get1stItemPixmap()
{
	return m_pFSMatterViewPrivate->GetMediaPage()->Get1stItemPixmap();
}
