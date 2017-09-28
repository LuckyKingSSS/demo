#include "stdafx.h"
#include "RecorderPreview.h"

#include <QtXml/QtXml>

#include "inc_MKCollecter/MKCollecter.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_FSCommonlib/GlobalDefine.h"

#include "FSRecorder.h"

#include "FSSettingsWidget.h"

#include "SettingsDataManager.h"
#include "ProjectFilesMgr.h"

#include "inc_FSCommonlib/TimelinePropertyDialog.h"
#include "inc_FSCommonlib/MediaImportDlg.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_FSCommonlib/gathread.h"
#include "Communicator.h"
#include "RecorderViewsMgr.h"

RecorderPreview::RecorderPreview(QWidget *parent)
: FSBaseWidget(parent)
{
	m_previewList = new PreivewListWidget(this);
	m_playerView = new IFSMediaPlayer(this);

    m_cmbExport = new FSComboBox(this);
    setObjectName("RecorderPreview");
	
	connect(m_previewList, &PreivewListWidget::signalLoadTimeline, this, [&](QString path, FSNLETimeline *timeline){
		m_playerView->Close();
        if (timeline)
        {
            //m_previewList->blockSignals(true);
            m_playerView->Open(timeline);
            //m_previewList->blockSignals(false);

        }
		else if (!path.isEmpty())
		{
            FSMessageBox::informationOk(tr(sFilmoraScreen), tr("The file does not exist"), RecorderViewsMgr::GetInstance()->GetRecordWidget());
			m_previewList->RemoveItem(path);
            ExportCurrentIndexChanged();
		}
	}, Qt::QueuedConnection);

	connect(m_previewList, &PreivewListWidget::signalCaptureMore, this, [&](){
		//上传用户使用信息
		//记录用户按键信息
		GAThread::getInstance()->insertEventItem(Your_Clips, "CaptureMoreButtonCount", "");
		m_playerView->Close();
		emit signalCaptureMore();
	});

    connect(m_previewList, &PreivewListWidget::signalItemSelectedChanged, this, &RecorderPreview::SetExportButtonEnabled);
    connect(m_previewList, &PreivewListWidget::signalDeleteItem, this, [&](){
        m_playerView->Close();
    });

	connect(m_cmbExport, SIGNAL(currentIndexChanged(int)), this, SLOT(ExportCurrentIndexChanged()));
	m_btnGo = new FSButton(this);
    m_btnGo->setObjectName("RecorderPreview_m_btn");
	connect(m_btnGo, &FSButton::clicked, this, [&](){
        m_playerView->Pause();

        QList<QString> exportPaths;
        if (m_cmbExport->currentIndex() == 0)
        {
            exportPaths = m_previewList->GetAllPaths();
            m_playerView->Close();
            m_previewList->ClearItems();
            emit signalExport2Editor(exportPaths);
			GAThread::getInstance()->insertEventItem(Your_Clips, "ExportAllClipstoEditorCount", "");
        }
        else
        {
            exportPaths = m_previewList->GetSelectedPaths();
            if (m_cmbExport->currentIndex() == 1)
            {
                m_playerView->Close();
                m_previewList->ClearSelectedItems();
                emit signalExport2Editor(exportPaths);
				GAThread::getInstance()->insertEventItem(Your_Clips, "ExportSelectedClipstoEditorCount", "");
            }
            else
                emit signalExport2Local(exportPaths);
			    GAThread::getInstance()->insertEventItem(Your_Clips, "ExporttoLocalFilesCount", "");
				//屏幕切换需要上传对应的信息
				GAThread::getInstance()->insertEventItem(Change_Page, "Export", "");
        }

        if (!exportPaths.isEmpty())
        {
            ExportCurrentIndexChanged();
        }
	});

	m_btnGo->setFocusPolicy(Qt::NoFocus);
	m_cmbExport->setFocusPolicy(Qt::NoFocus);


	
	m_btnAdd = new FSButton(this);
	m_btnAdd->setText("Add file");
    m_btnAdd->setFocusPolicy(Qt::NoFocus);
#ifndef _DEBUG
	m_btnAdd->hide();
#endif
	connect(m_btnAdd, &FSButton::clicked, this, [&](){
		static QString prePath;
		auto paths = QFileDialog::getOpenFileNames(this, "", "");
        for (auto path : paths)
        {
            prePath = path;

            AddItem(path);
        }
	});




    m_spaceShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    m_spaceShortcut->setContext(Qt::ApplicationShortcut);
    m_spaceShortcut->setEnabled(false);

    connect(m_spaceShortcut, &QShortcut::activated, this, [&](){
        if (m_playerView->IsPlaying())
        {
            m_playerView->Pause();
        }
        else
        {
            m_playerView->Play();
        }
    });


    resize(800, 440);
    RetranslateUi();
}

RecorderPreview::~RecorderPreview()
{

}

void RecorderPreview::AddItem(QString path)
{
    m_previewList->AddItem(path);



    m_previewList->SetCurrentItem(path);

    if (SettingsDataManager::GetInstance()->GetGameData()->GetHaveGameRecord())
    {
        // 游戏全屏录制，停止录制后，窗口会透明,通过resize窗口大小强行刷新窗口, 这是底层的坑，底层暂时找不到原因
        m_playerView->resize(m_playerView->width() + 1, m_playerView->height());
        m_playerView->resize(m_playerView->width() - 1, m_playerView->height());
    }
}


void RecorderPreview::SetCurrentItem(QString path)
{
	m_previewList->SetCurrentItem(path);
}

void RecorderPreview::Reset()
{
	m_previewList->ClearItems();
}

QPixmap RecorderPreview::GetFirstItemThumbnail()
{
	auto datas = RecordingPreviewDataManager::GetInstance()->GetItemDatas();
	if (datas.size() > 0)
	{
		return datas[0]->icon;
	}
	return QPixmap();
}

#include "inc_CommonLib/StringOperation.h"

void RecorderPreview::resizeEvent(QResizeEvent *event)
{
    int left = 52;
	int top = 28;
	const int bottomHeight = 92;
    m_previewList->setGeometry(left, top, 345, height() - top - 10);
	left = m_previewList->geometry().right() + 51;
    top = 44;
	m_playerView->setGeometry(left, top, width() - left-58, height() - bottomHeight-top);
	
    // 设计按钮的长度
    const int btnGoWidth = 70;

    int btnGoNeedWidth = GetRetranslateUiTextLenght(m_btnGo->font(), m_btnGo->text(), btnGoWidth, 5);

    left = width() - 340 - btnGoNeedWidth;
    top = m_playerView->geometry().bottom()+20;
	m_cmbExport->setGeometry(left, top, 274, 30);
	left = m_cmbExport->geometry().right()+10;
    m_btnGo->setGeometry(left, top, btnGoNeedWidth, 30);

	m_btnAdd->setGeometry(m_cmbExport->geometry().x() - 85, top,75, 30);
}

void RecorderPreview::RetranslateUi()
{
	m_cmbExport->clear();
	m_cmbExport->addItem(tr("Export all clips to editor"));
	m_cmbExport->addItem(tr("Export selected clips to editor"));
	m_cmbExport->addItem(tr("Export to local files")+ "...");


	m_btnGo->setText(tr("Go"));
    resizeEvent(nullptr);
}

void RecorderPreview::ExportCurrentIndexChanged()
{
	SetExportButtonEnabled(m_previewList->IsItemSelected());
}

void RecorderPreview::SetExportButtonEnabled(bool isItemSelect)
{
	if (m_cmbExport->currentIndex() != 0)
		m_btnGo->setEnabled(isItemSelect);
	else
		m_btnGo->setEnabled(!m_previewList->GetAllPaths().isEmpty());
}

void RecorderPreview::RemoveItems(QList<QString> paths)
{

}

void RecorderPreview::showEvent(QShowEvent *event)
{
    m_spaceShortcut->setEnabled(true);
    ExportCurrentIndexChanged();

}



void RecorderPreview::hideEvent(QHideEvent *event)
{
    m_spaceShortcut->setEnabled(false);

}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CaptureMoreItem::CaptureMoreItem(QWidget *parent)
:FSBaseWidget(parent)
{
	m_lbMore = new FSLabel(this);
	m_lbMore->setObjectName("RecorderPreview_CaptureMoreItem");

	m_lbMoreText = new FSLabel(this);
    m_lbMoreText->setObjectName("CaptureMoreItem_m_lbMoreText");
	m_lbMoreText->setAlignment(Qt::AlignCenter);
	RetranslateUi();
}

void CaptureMoreItem::resizeEvent(QResizeEvent *event)
{
	static const int TextH = 26;
	m_lbMore->setGeometry(0,0,width(),height() - TextH);
	m_lbMoreText->setGeometry(0, m_lbMore->geometry().bottom()+1, width(), TextH);
	m_lbMoreText->raise();
}



void CaptureMoreItem::RetranslateUi()
{
    //QString tempText(tr("Capture More"));
    QString tempText = StringOperation::GetElidedString(m_lbMoreText->font(), m_lbMoreText->width(), tr("Capture More"));
    m_lbMoreText->setText(tempText);
    m_lbMoreText->setToolTip(tr("Back to recorder to capture more clips"));
}

//////////////////////////////////////////////////////////////////////////
#include "inc_CommonLib/StringOperation.h"
PreviewItem::PreviewItem(PreviewItemData *data, QWidget *parent)
:FSBaseWidget(parent)
, m_data(data)
{
	m_icon = new QLabel(this);
	m_icon->setPixmap(data->icon);
	//m_icon->setStyleSheet("background-color:gray;");
	m_title = new FSButton(this);
	m_time = new QLabel(this);
	m_pSelectedWidget = new SelectedWidget(this);
    m_title->setObjectName("PreviewItem_m_title");
    m_title->setAttribute(Qt::WA_TranslucentBackground);
    m_title->setCheckable(true);
    m_title->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_time->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    SetState(m_pSelectedWidget->GetState());



}

PreviewItem::~PreviewItem()
{

}

void PreviewItem::SetState(SelectedWidget::State s)
{
	m_pSelectedWidget->SetState(s);
    m_title->setEnabled(true);
    m_title->setChecked(false);
    if (s == SelectedWidget::State_Selected)
    {
        m_title->setChecked(true);
    }
    else if (s == SelectedWidget::State_Hover)
    {
        m_title->setEnabled(false);
    }
}

PreviewItemData* PreviewItem::GetData()
{
	return m_data;
}

void PreviewItem::resizeEvent(QResizeEvent *event)
{
	const int margins = 2;
	const int bottomHeight = 22;
	QRect rect = this->rect().adjusted(margins, margins, -margins, -margins);
	int left = margins;
	int top = margins;
	m_icon->setGeometry(left, top, rect.width(), rect.height() - bottomHeight);
	top = m_icon->geometry().bottom();
	int lbTimeWidth = 0;
	m_title->setGeometry(left, top, width() - lbTimeWidth, bottomHeight);
	//left = m_title->geometry().right();
	//m_time->setGeometry(left, top, lbTimeWidth, bottomHeight);


	m_title->setText(StringOperation::GetElidedString(m_title->font(), m_title->width(), m_data->name));
	//m_title->setToolTip(m_data->name);
	//m_time->setText(m_data->duration);

	m_pSelectedWidget->setGeometry(m_icon->geometry());

}

void PreviewItem::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(m_icon->geometry(), Qt::black);
}


void PreviewItem::enterEvent(QEvent *e)
{
	if(m_pSelectedWidget->GetState() != SelectedWidget::State_Selected)
		m_pSelectedWidget->SetState(SelectedWidget::State_Hover);
    m_title->setEnabled(false);
}

void PreviewItem::leaveEvent(QEvent *e)
{
    if (m_pSelectedWidget->GetState() != SelectedWidget::State_Selected)
        m_pSelectedWidget->SetState(SelectedWidget::State_Normal);
    else
        m_title->setChecked(true);
    m_title->setEnabled(true);
}


//////////////////////////////////////////////////////////////////////////
RecordingPreviewDataManager::RecordingPreviewDataManager(QObject *parent)
:QObject(parent)
{
	connect(ProjectMgr::GetInstance(), &ProjectMgr::sigLoaded, this, [&](){
		auto settings = ProjectMgr::GetInstance()->GetPreviewString();
		if (settings.isEmpty()) return;
		LoadProjectSetting(settings);
		emit signalLoad();
	}
	);

}

RecordingPreviewDataManager* RecordingPreviewDataManager::GetInstance()
{
	static RecordingPreviewDataManager ins;
	return &ins;
}

RecordingPreviewDataManager::~RecordingPreviewDataManager()
{
	qDeleteAll(m_datas);
	m_datas.clear();
}


static const char *sItem = "Item";
static const char *sPath = "Path";

extern void sElementAppendChild(QDomDocument doc, QDomElement parent, QString tag, QVariant text);

QString RecordingPreviewDataManager::GetSettingString()
{

	QDomDocument doc;
	auto root = doc.createElement("PreviewMessage");
	doc.appendChild(root);

	//auto screenElem = doc.createElement(sScreen);
	//root.appendChild(screenElem);

	for (auto temp : m_datas)
	{
		auto elem = doc.createElement(sItem);
		sElementAppendChild(doc, elem, sPath, temp->path);
		root.appendChild(elem);
	}

	auto text = doc.toString(4);
	return text;
}



QStringList RecordingPreviewDataManager::GetAllPaths()
{
	QList<QString> paths;
	auto datas = GetItemDatas();
	for (auto temp : datas)
	{
		paths.push_back(temp->path);

	}
	return paths;
}

PreviewItemDatas RecordingPreviewDataManager::GetItemDatas()
{
	return m_datas;
}

void RecordingPreviewDataManager::AddData(PreviewItemData *data)
{
	Q_ASSERT(data);
	m_datas.push_back(data);
	ProjectMgr::GetInstance()->SetContentChanged(true);
}

void RecordingPreviewDataManager::RemoveData(PreviewItemData *data,bool bSetContentChanged/* = true*/)
{
	Q_ASSERT(data);
	m_datas.removeOne(data);
	delete data;
	if(bSetContentChanged) ProjectMgr::GetInstance()->SetContentChanged(true);
}

void RecordingPreviewDataManager::Clear()
{
    for (auto temp : GetItemDatas())
    {
        RemoveData(temp,false);
    }
}

//extern QDomElement sGetChildElementByTag(QDomElement parent, QString tag);
static QString sGetChildElementTextByTag(QDomElement parent, QString tag)
{
	auto elems = parent.elementsByTagName(tag);
	if (!elems.isEmpty())
	{
		return elems.at(0).toElement().text();
	}

	return QString();
}

void RecordingPreviewDataManager::LoadProjectSetting(QString xml)
{
	QDomDocument doc("PreviewMessage");
	QString errorMsg;
	if (!doc.setContent(xml, &errorMsg))
		return;

	auto docElem = doc.documentElement();

	auto items = docElem.elementsByTagName(sItem);
	for (int i = 0; i < items.size(); ++i)
	{
		auto item = items.at(i).toElement();
		PreviewItemData *data = new PreviewItemData();
		data->path = sGetChildElementTextByTag(item, sPath);
		m_datas.push_back(data);
	}
}

//////////////////////////////////////////////////////////////////////////
PreivewListWidget::PreivewListWidget(QWidget *parent)
:FSBaseWidget(parent)
, m_needLoad(false)
, m_timeline(nullptr)
{
	m_listWidget = new ListWidget(this);
	m_listWidget->setObjectName("RecorderPreview_PreivewListWidget");
    m_listWidget->setAttribute(Qt::WA_TranslucentBackground);
	m_listWidget->SetSelectedBorderColor(QColor(0, 0, 0, 0));
	m_listWidget->AddItem(new CaptureMoreItem(this));
	
	m_manager = RecordingPreviewDataManager::GetInstance();
	connect(m_manager, &RecordingPreviewDataManager::signalLoad, this, [&](){
		m_needLoad = true;

	});
	

	connect(m_listWidget, &ListWidget::signalItemSelectedChanged, this, [&](){

		auto widgets = m_listWidget->GetSelectedItems();
		for (auto w : m_listWidget->GetAllItems())
		{
			PreviewItem* piw = qobject_cast<PreviewItem*>(w);
			if (!piw) continue;
			Q_ASSERT(piw);
			piw->SetState(widgets.contains(w) ? SelectedWidget::State_Selected : SelectedWidget::State_Normal);
		}
			
		bool isSelectedItem = !widgets.isEmpty();
		if (widgets.size() == 1 && qobject_cast<PreviewItem*>(widgets[0]) == nullptr)
			isSelectedItem = false;
		signalItemSelectedChanged(isSelectedItem);
	});


    m_actDetails = new QAction(this);
    m_actFindTarget = new QAction(this);
    m_actDelete = new QAction(this);


    m_deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    //m_deleteShortcut->setContext(Qt::ApplicationShortcut);
    //m_deleteShortcut->setEnabled(false);
    connect(m_deleteShortcut, &QShortcut::activated, this, [&](){
        DeleteSelectionItem();
    });

    RetranslateUi();


}

PreivewListWidget::~PreivewListWidget()
{

}

void PreivewListWidget::AddItem(QString path)
{
	if (m_manager->GetAllPaths().contains(path))
		return;

	PreviewItemData *data = new PreviewItemData();
	data->path = path;
	m_manager->AddData(data);

    if (m_needLoad)
    {
        m_needLoadSetCurrentPath = path;        
    }
    else
    {
        // 未加载工程原有视频时，不加载Item,留为LoadProjectItems统一加载
        AddItem(data);
    }
}

void PreivewListWidget::AddItem(PreviewItemData * data)
{
    if (data->timeline == nullptr)
    {
        data->timeline = CreateNLETimeline(data->path);
    }
		
	if (!data->path.isEmpty())
	{
		QFileInfo fileInfo(data->path);
		data->name = fileInfo.completeBaseName();
	}
    QString durationString("00:00:00");
	if (data->timeline)
	{
        data->timeline->SetType(data->timeline->GetNLETimelineType());
		int second = data->timeline->GetDuration();
		int hour = second / (60 * 60);
		second = second % (60 * 60);
		int minute = second  / 60;
		second = second % 60;
		data->duration = QString("%1:%2:%3").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0')).arg(second, 2, 10, QChar('0'));
        if (data->timeline->IsPureAudio())
            data->icon = QPixmap(":/FSRecorder/Resources/light/RecordeAudio.png");
        else
            data->icon = data->timeline->GetThumbnail(130, 80);
        durationString = data->timeline->GetDurationString();
		//获取结束录制后的视频时长信息
		QString strDurationRange = GAThread::getInstance()->getRecordDurarionRange(second);
		GAThread::getInstance()->insertEventItem(Recorder_Widget, "RecordDurationRange", strDurationRange);
	}
	PreviewItem* item = new PreviewItem(data, this);
    item->setToolTip(item->GetData()->name + "\r\n" + durationString);
	m_listWidget->AddItem(item);
}

void PreivewListWidget::RemoveItem(QString path)
{
	auto widgets = m_listWidget->GetSelectedItems();
	for (auto temp : widgets)
	{
		auto widget = qobject_cast<PreviewItem*>(temp);
		if (widget)
		{
			auto data = widget->GetData();
			if (path == data->path)
			{
				m_listWidget->RemoveItem(widget);
				m_manager->RemoveData(data);
			}
			
		}
	}
}

void PreivewListWidget::SetCurrentItem(QString path)
{
	auto widgets = m_listWidget->GetAllItems();
	for (auto temp : widgets)
	{
		auto widget = qobject_cast<PreviewItem*>(temp);
		if (widget)
		{
			auto data = widget->GetData();
			if (path == data->path)
			{
				m_listWidget->SetCurrentItem(widget);

                m_timeline = data->timeline;
				emit signalLoadTimeline(path, data->timeline);

				m_listWidget->ScrollToItem(widget);
				break;
			}

		}
	}
}


bool PreivewListWidget::IsItemSelected()
{
	return !m_listWidget->GetSelectedItems().isEmpty();
}

QList<QString> PreivewListWidget::GetSelectedPaths()
{
	QList<QString> paths;
	auto widgets = m_listWidget->GetSelectedItems();
	for (auto temp : widgets)
	{
		auto widget = qobject_cast<PreviewItem*>(temp);
		if (widget)
		{
			paths.push_back(widget->GetData()->path);
		}
	}
	return paths;
}

QList<QString> PreivewListWidget::GetAllPaths()
{
	QList<QString> paths;
	auto datas = m_manager->GetItemDatas();
	for (auto temp : datas)
	{
		paths.push_back(temp->path);
		
	}
	return paths;
}

void PreivewListWidget::ClearSelectedItems()
{
	auto widgets = m_listWidget->GetSelectedItems();
	for (auto temp : widgets)
	{
		auto widget = qobject_cast<PreviewItem*>(temp);
		if (widget)
		{
			auto data = widget->GetData();
			m_listWidget->RemoveItem(widget);
			m_manager->RemoveData(data);
		}
	}
}



void PreivewListWidget::ClearItems()
{
	auto widgets = m_listWidget->GetAllItems();
	for (auto temp : widgets)
	{
		auto widget = qobject_cast<PreviewItem*>(temp);
		if (widget)
		{
			auto data = widget->GetData();
			m_listWidget->RemoveItem(widget);
			m_manager->RemoveData(data,false);
		}
	}

    m_manager->Clear();
}

FSNLETimeline* PreivewListWidget::CreateNLETimeline(QString path)
{
	auto timeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateTimelineForScreenRecording(path);
	FSNLETimeline *fstimeline = nullptr;
	if (timeline)
	{
		fstimeline = new FSNLETimeline(timeline);
	}
	return fstimeline;
}



void PreivewListWidget::LoadProjectItems()
{
    m_needLoad = false;

	auto datas = m_manager->GetItemDatas();
    QStringList files;
    PreviewItemDatas dontExistFiles;
	for (auto data : datas)
	{
        if (!FileOperation::IsFileExist(data->path))
            dontExistFiles.push_back(data);

        files.push_back(data->path);
	}
   
	if (!files.isEmpty())
	{
		PreviewMediaImportDlg dlg(files, datas, this);
		connect(&dlg, &PreviewMediaImportDlg::signalImport, this, [&](PreviewItemData *data){
			AddItem(data);
		});
		dlg.DoImport();
	}
	
    for (auto data : dontExistFiles)
    {
        AddItem(data);
    }

    if (!m_needLoadSetCurrentPath.isEmpty())
    {
        SetCurrentItem(m_needLoadSetCurrentPath);
        m_needLoadSetCurrentPath.clear();
    }

}


void PreivewListWidget::DeleteSelectionItem()
{
    if (IsItemSelected())
    {
        auto retvalue = FSMEssageConfirmDlg::questionYesNo(sFilmoraScreen, "Delete", "Delete source also", true, RecorderViewsMgr::GetInstance()->GetFSRecorder());
        if (std::get<1>(retvalue) == FSMessageBox::Yes)
        {
            emit signalDeleteItem();
            QTimer::singleShot(0, [&, retvalue](){
                auto selectedPaths = GetSelectedPaths();
                ClearSelectedItems();
                if (std::get<0>(retvalue))
                {
                    for (auto path : selectedPaths)
                    {
                        QFile::remove(path);
                    }
                }
            });

        }
    }

}

void PreivewListWidget::resizeEvent(QResizeEvent *event)
{
	m_listWidget->setGeometry(rect());
}
void PreivewListWidget::RetranslateUi()
{
    m_actDetails->setText(tr("Details..."));
    m_actFindTarget->setText(tr("Find target..."));
    m_actDelete->setText(tr("Delete"));
}



void PreivewListWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}


extern int sMenuWidth;

void PreivewListWidget::mousePressEvent(QMouseEvent *event)
{
    FSBaseWidget::mousePressEvent(event);
    //if (event->button() == Qt::RightButton)
    //{
    //    QList<QWidget*> selectItems = m_listWidget->GetSelectedItems();
    //    if (!selectItems.isEmpty())
    //    {
    //        auto widget = qobject_cast<PreviewItem*>(selectItems.first());
    //        if (widget)
    //        {
    //            QMenu menu;
    //            menu.setFixedWidth(sMenuWidth);
    //            menu.addAction(m_actDetails);
    //            menu.addAction(m_actFindTarget);
    //            menu.addAction(m_actDelete);
    //            m_actDetails->setEnabled(widget->GetData()->timeline);
    //            m_actFindTarget->setEnabled(widget->GetData()->timeline);
    //            auto act = menu.exec(event->globalPos());
    //            if (act == m_actDetails)
    //            {
				//	//记录用户操作信息
				//	GAThread::getInstance()->insertEventItem(Your_Clips, "RightclickDetailsCount", "");
    //                TimelinePropertyDialog dlg(this);
    //                dlg.DoModal(widget->GetData()->timeline);

    //            }
    //            else if (act == m_actFindTarget)
    //            {
				//	//记录用户操作信息
				//	GAThread::getInstance()->insertEventItem(Your_Clips, "RightclickFindTargetCount", "");
    //                FSFindTargetFile(widget->GetData()->path);

    //            }
    //            else if (act == m_actDelete)
    //            {
    //                auto retvalue = FSMEssageConfirmDlg::questionYesNo(sFilmoraScreen, "Delete", "Delete source also", true, RecorderViewsMgr::GetInstance()->GetFSRecorder());
    //                if (std::get<1>(retvalue) == FSMessageBox::Yes)
    //                {
    //                    emit signalDeleteItem();
    //                    auto selectedPaths = GetSelectedPaths();
    //                    ClearSelectedItems();
    //                    if (std::get<0>(retvalue))
    //                    {
    //                        for (auto path : selectedPaths)
    //                        {
    //                            QFile::remove(path);
    //                        }
    //                    }
    //                }
    //                
    //            }
    //        }
    //    }
    //}

}

void PreivewListWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void PreivewListWidget::mouseReleaseEvent(QMouseEvent *event)
{
	auto items = m_listWidget->GetSelectedItems();

	bool isCaptureMore = false;
	FSNLETimeline *timeline = nullptr;
	QString path;
	if (!items.isEmpty())
	{
		auto temp = qobject_cast<PreviewItem*>(items[0]);
		if (temp == nullptr && items.size() == 1)
		{
			isCaptureMore = true;
		}
		else
		{
			if (temp == nullptr)
			{
				if (items.size() > 1)
				{
					temp = qobject_cast<PreviewItem*>(items[1]);
					timeline = temp->GetData()->timeline;
					path = temp->GetData()->path;
				}

			}
			else
			{
				temp = qobject_cast<PreviewItem*>(items[0]);
				timeline = temp->GetData()->timeline;
				path = temp->GetData()->path;
			}
		}


	}
	if (isCaptureMore)
	{
		m_listWidget->ClearSelection();
		m_timeline = nullptr;
		emit signalCaptureMore();
	}
	else
	{
		if ((m_timeline != timeline) || m_timeline == nullptr)
		{
			m_timeline = timeline;
			emit signalLoadTimeline(path, timeline);
		}
	}

    if (event->button() == Qt::RightButton && !isCaptureMore)
    {
        QList<QWidget*> selectItems = m_listWidget->GetSelectedItems();
        if (!selectItems.isEmpty())
        {
            auto widget = qobject_cast<PreviewItem*>(selectItems.first());
            if (widget)
            {
                QMenu menu;
                menu.setFixedWidth(sMenuWidth);
                menu.addAction(m_actDetails);
                menu.addAction(m_actFindTarget);
                menu.addAction(m_actDelete);
                m_actDetails->setEnabled(widget->GetData()->timeline);
                m_actFindTarget->setEnabled(widget->GetData()->timeline);
                auto act = menu.exec(event->globalPos());
                if (act == m_actDetails)
                {
                    //记录用户操作信息
                    GAThread::getInstance()->insertEventItem(Your_Clips, "RightclickDetailsCount", "");
                    TimelinePropertyDialog dlg(this);
                    dlg.DoModal(widget->GetData()->timeline);

                }
                else if (act == m_actFindTarget)
                {
                    //记录用户操作信息
                    GAThread::getInstance()->insertEventItem(Your_Clips, "RightclickFindTargetCount", "");
                    FSFindTargetFile(widget->GetData()->path);

                }
                else if (act == m_actDelete)
                {
                    DeleteSelectionItem();

                }
            }
        }
    }
    FSBaseWidget::mouseReleaseEvent(event);

}

void PreivewListWidget::showEvent(QShowEvent *event)
{
	if (m_needLoad)
	{
		QMetaObject::invokeMethod(this, "LoadProjectItems", Qt::QueuedConnection);
	}
}

//////////////////////////////////////////////////////////////////////////
#include "inc_CommonLib/TopLevelWindowTitleBar.h"

PreviewMediaImportDlg::PreviewMediaImportDlg(const QStringList &list, PreviewItemDatas datas, QWidget *parent)
:MediaImportDlg(list, parent)
, m_datas(datas)
{
	m_titlebar->SetButtons(TopLevelWindowTitleBar::None);
	m_pBtnCancel->hide();

	connect(this, &MediaImportDlg::sigImported, this, [&](QString path, FSNLETimeline *timeline){
		for (PreviewItemData *data : m_datas)
		{
			if (data->path == path)
			{
				data->timeline = timeline;
				signalImport(data);
				break;
			}
		}
	});
}
