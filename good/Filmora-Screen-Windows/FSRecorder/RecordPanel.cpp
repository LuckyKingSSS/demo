#include "stdafx.h"
#include "RecordPanel.h"
#include "FSTitleBar.h"
#include "inc_CommonLib/StringOperation.h"
#include "SettingsDataManager.h"
#include "CaptureManger.h"
#include "inc_MKCollecter/MKCollecter.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "SettingsDataManager.h"
#include "FSRecorder.h"
#include "ProjectFilesMgr.h"
#include "RecorderViewsMgr.h"
#include "FSSettingsWidget.h"
#include <QSystemTrayIcon>
#include "inc_CommonLib/CommonWidgets.h"
#include "inc_FSCommonlib/GlobalDefine.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_FSCommonlib/TUATracker.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_CommonLib/externalapi.h"
#include "Communicator.h"
#include "RecorderPreview.h"
#include "MiniRecordPanel.h"
#include "PrimaryScreenManager.h"

#include "ProjectAutoSaveMgr.h"

int sMenuWidth = 240;


void SaveProjThumbnail()
{
	QString paht;
	if (!ProjectMgr::GetInstance()->IsEditorProject())
	{
		auto thumbnail = ((PreviewPage*)RecorderViewsMgr::GetInstance()->GetFSRecorderMainView()->GetPageView(FSRecorderMainView::Page_Preview))->GetFirstItemThumbnail();
		paht = FSGetProjectFileThumbnailPath() + QString("/%1.png").arg(::GetTickCount());
		thumbnail.save(paht);
	}
	else
	{
		paht = Communicator::GetInstance()->GetEditorThumbnail();
	}
	ProjectMgr::GetInstance()->SetThumbnailPath(paht);
}

QString GetDefaultPath()
{
	QString path = PathOperation::GetDocumentPath() + "/FilmoraScreen";
	QDir dir(path);
	if (!dir.exists())
	{
		dir.mkpath(path);
	}
	return path;
}

QString GetSaveProjPath()
{
	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc,FSGetFilmoraScreenDirectory() + "/settingdatas.xml"))
		return GetDefaultPath();
	QString path;
	QDomElement rootElem = doc.documentElement();
	QDomElement subElem = rootElem.firstChildElement("SavePath");
	Q_ASSERT(!subElem.isNull());
	if (!subElem.isNull())
	{
		path = subElem.text();
	}
	return path.isEmpty() ? GetDefaultPath() : path;
}


bool DoClose(QWidget *parentWidget)
{
	bool bCancel = false;
	if (ProjectMgr::GetInstance()->IsContentChanged())
	{
		int ret = FSMessageBox::questionYesNoCancel(QObject::tr(sFilmoraScreen), 
			QObject::tr("The project has been modified,Do you want to save the changes?"), 
			parentWidget,500,220);

		if (ret == FSMessageBox::Yes)
		{
			if (ProjectMgr::GetInstance()->IsNewProject())
			{
				QString newpath = QFileDialog::getSaveFileName(parentWidget, QObject::tr("Please select directory"), GetSaveProjPath(), "Project Files(*.wfsproj)");
				if (!newpath.isEmpty())
				{
					SaveProjThumbnail();
					ProjectMgr::GetInstance()->SaveNew(newpath);
					qApp->quit();
				}
				else
				{
					bCancel = true;
				}
			}
			else
			{
				SaveProjThumbnail();
				ProjectMgr::GetInstance()->Save();
				qApp->quit();
			}
		}
		if (ret == FSMessageBox::No)
		{
            ProjectAutoSaveMgr::GetInstance()->QuitDontSave();
			qApp->quit();
		}
		if (ret == FSMessageBox::Cancel)
		{
			bCancel = true;
		}
	}
	else
		qApp->quit();
	return bCancel;
}

RecordPanel::RecordPanel(QWidget *parent)
: FSBaseWidget(parent)
{
	m_strNoEnoughSpace = tr("There isn't enough space for recording, please delete unnecessary files to free enough space.");
    setWindowTitle("FSRecordPanel");
	RecorderViewsMgr::GetInstance()->SetRecordPanel(this);
	gFSSetRecordPanel(this);
	m_UiState = UiState_Stop;
	m_nCommontTipState = 1;
	m_bPreviewShowed = false;
	setObjectName("RecordPanel");
	m_pFreeSpaceChecker = new FreeSpaceChecker(this);

	m_pBlackMask = new QWidget(this);
	m_pBlackMask->hide();
	m_pTitleBar = new TopLevelWindowTitleBar(this);
	m_pTitleBar->SetSystemBorderVisible(false);
	m_pTitleBar->SetButtons(TopLevelWindowTitleBar::None);
	m_pTitleBar->SetResizeble(false);
	m_pTitleBar->SetDragMaximum(false);
	m_pTitleBar->GetControl()->SetBorderColor(QColor(102,102,102));
    connect(m_pTitleBar, &TopLevelWindowTitleBar::DashedMove, this, &RecordPanel::sigDashedMove);
    connect(m_pTitleBar, &TopLevelWindowTitleBar::StartDashedMove, this, &RecordPanel::sigStartDashedMove);
    connect(m_pTitleBar, &TopLevelWindowTitleBar::EndDashedMove, this, &RecordPanel::sigEndDashedMove);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
	m_pBtnMinimized = new BaseButton("FSRecorder_TitleBar_MinimizeButton",this);
    m_pBtnMiniPanel = new BaseButton("FSRecorder_TitleBar_MiniPanelButton", this);

	m_pBtnClosed = new BaseButton("FSRecorder_TitleBar_CloseButton", this);

    m_pBtnSettings = new QPushButton(this);
    m_pBtnSettings->setObjectName("RecordPanel_MenuButton");
    m_pBtnPreview = new QPushButton(this);
    m_pBtnPreview->setObjectName("RecordPanel_MenuButton");
    m_pBtnEditor = new QPushButton(this);
    m_pBtnEditor->setObjectName("RecordPanel_MenuButton");




	m_pBtnRecord = new RecordButton("RecordPanel_RecordButton",this);

	m_pBtnPauseResume = new BaseStateButton(0,"RecordPanel_PauseResumeButton", this);
	m_pBtnPauseResume->AddButton(0, "RecordPanel_PauseButton");
	m_pBtnPauseResume->AddButton(1, "RecordPanel_ResumeButton");

	m_pCountBackwardCtrl = new CountBackwardCtrl("RecordPanel_CountBackwardCtrl",this);
	m_pBtnDelete = new BaseButton("RecordPanel_DeleteButton", this);
	m_pLbComments = new QLabel(this);
	m_pLbComments->setObjectName("RecordPanel_CommentLabel");
	m_pPreviewCtrlWidget = new PreviewCtrlWidget("RecordPanel_PreviewCtrlWidget", this);

    QLabel  *pIconLabel = new QLabel(this);
    pIconLabel->setObjectName("RecordPanel_TitleBar_m_pLabelIcon");
    pIconLabel->setFixedSize(100, 18);
    pIconLabel->move(12, 6);
	pIconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

	m_pLine1 = new SeperaterLine(this);
	m_pLine2 = new SeperaterLine(this);
	m_pLine3 = new SeperaterLine(this);
	m_pLine5 = new SeperaterLine(this);
	m_pLine5->setVisible(false);


    m_pMiniRecordPanel = new MiniRecordPanel(this);
    //m_pMiniRecordPanel->show();
    m_pSyncPanelState = new SyncRecordPanelAndMiniPanelState(this, m_pMiniRecordPanel, this);

    m_pBtnLock = new BaseStateButton(0, "", this);
    m_pBtnLock->AddButton(0, "RecordPanel_Title_PinUnlock");
    m_pBtnLock->AddButton(1, "RecordPanel_Title_Pinlock");
    connect(m_pBtnLock, &BaseStateButton::Clicked, this, [&](int id){
        m_pTitleBar->GetControl()->SetMoveable(id);
    });

	ConnectSignals();
	RetranslateUi();
	Resize();
	SetUiState(UiState_Stop);
    OnRecordStateChanged();

}

RecordPanel::~RecordPanel()
{
    
}

void RecordPanel::setVisible(bool visible)
{
    FSBaseWidget::setVisible(visible);
}

void RecordPanel::ConnectSignals()
{
	//connect(m_pBtnMinimized, &BaseButton::clicked, this, [&]{
	//	setVisible(false);
	//});
	connect(m_pBtnClosed, &BaseButton::clicked, this, [&]{
		if (RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None)
		{
            if (CanGo2PreviewWin())
            {
                RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);
            }
            else if (Communicator::GetInstance()->IsEditorRunning())
            {
                RecorderViewsMgr::GetInstance()->GotoEditor();
            }
            else
            {
                RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_GettingStart);
            }
            //DoClose(RecorderViewsMgr::GetInstance()->GetRecordWidget());
		}
		else
		{
			setVisible(false);
		}
	});
	connect(SysTrayIcon::GetInstance(), &SysTrayIcon::sigOpen, this, [&]{
		//::SetFocus((HWND)winId());
		//setVisible(true);
        //m_pSyncPanelState->setVisible(true);
        RecorderViewsMgr::GetInstance()->ShowRecordWidget();
	});
	connect(SysTrayIcon::GetInstance(), &SysTrayIcon::sigQuit, this, [&]{
        DoClose(RecorderViewsMgr::GetInstance()->GetRecordWidget());
	});
	connect(RecorderCenter::GetInstance(), &RecorderCenter::sigRecordStateChange, this, [&]{
		if (RecorderViewsMgr::GetInstance()->GetFSRecorderMainView()->isVisible())
			return;
		SysTrayIcon::GetInstance()->SetVisible(true);
	});

    connect(RecorderCenter::GetInstance(), &RecorderCenter::sigRecordStateChange, this, &RecordPanel::OnRecordStateChanged, Qt::QueuedConnection);


    connect(RecorderViewsMgr::GetInstance(), &RecorderViewsMgr::sigLoadingFinish, this, [&]{
		DoStart();
	});
    connect(RecorderViewsMgr::GetInstance(), &RecorderViewsMgr::sigLoadingAbort, this, [&]{
		SetUiState(UiState_Stop);
        LoadingAbort();
		RecorderViewsMgr::GetInstance()->ShowRecordWidget();
	});

    connect(m_pSyncPanelState, &SyncRecordPanelAndMiniPanelState::sigButtonRecordClicked, this, [&](int id){
	//connect(m_pBtnRecord, &RecordButton::sigClicked, this, [&](RecordButton::ButtonId id){
		if (id == RecordButton::ButtonId_Start)
		{
			OnStartButtonClicked();
		}
		else if (id == RecordButton::ButtonId_Stop)
		{
			DoStop();
		}
	});

    connect(m_pSyncPanelState, &SyncRecordPanelAndMiniPanelState::sigButtonPauseResumeClicked, this, [&](int id){
        if (id == 0)
            DoPause();
        else if (id == 1)
            DoResume();
    });

	//connect(m_pBtnPauseResume->GetButton(0), &BaseButton::clicked, this, [&]{
	//	DoPause();
	//});
	////Resume
	//connect(m_pBtnPauseResume->GetButton(1), &BaseButton::clicked, this, [&]{
	//	DoResume();
	//});
    connect(m_pSyncPanelState, &SyncRecordPanelAndMiniPanelState::sigButtonDeleteClicked, this, [&](QWidget *parent){
    //connect(m_pBtnDelete, &BaseButton::clicked, this, [&]{
		//记录用户删除操作
		GAThread::getInstance()->insertEventItem(Recorder_Widget, "DeleteRecordingbuttonCount", "");
        int ret = FSMessageBox::questionYesNo(tr(sFilmoraScreen), tr("Will you delete the file you are recording?"), RecorderViewsMgr::GetInstance()->GetActionWidget(), 500, 220);
		if (ret == FSMessageBox::Yes)
		{
			SetUiState(UiState_Stop);
			RecorderCenter::GetInstance()->Abort(); 
			RecorderViewsMgr::GetInstance()->ShowRecordWidget();
		}
	});
    connect(m_pBtnSettings, &QPushButton::clicked, this, [&]{
		//记录用户行为
		GAThread::getInstance()->insertEventItem(Recorder_Widget, "Menu", "Setup");
		RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Setting);
		GAThread::getInstance()->insertEventItem(Change_Page, "Setup","");
	});
    connect(m_pBtnPreview, &QPushButton::clicked, this, [&]{
		//记录用户行为
		GAThread::getInstance()->insertEventItem(Recorder_Widget, "Menu", "Previewer");
		RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);
		GAThread::getInstance()->insertEventItem(Change_Page, "YourClips","");
	});
    connect(m_pBtnEditor, &QPushButton::clicked, this, [&]{
		//记录用户行为
		GAThread::getInstance()->insertEventItem(Recorder_Widget, "Menu", "Editor");
		RecorderViewsMgr::GetInstance()->GotoEditor();
	});
	connect(SettingsDataManager::GetInstance()->GetAdvancedData(), &AdvancedData::signalStartStop, this, [&]{
		if (RecorderCenter::GetInstance()->GetReordState() != RecorderCenter::RecordState_None)
		{
			//记录用户操作
			GAThread::getInstance()->insertEventItem(Recorder_Widget, "StopRecordingHotKeyCount", "");
			DoStop();
		}
		else if(RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None 
			&& RecorderViewsMgr::GetInstance()->GetCurrentViewPage() == RecorderViewsMgr::ViewPage_Record)
		{
            
			DoLoading();

			//记录用户操作
			GAThread::getInstance()->insertEventItem(Recorder_Widget, "StartRecordingHotKeyCount", "");

		}
		else if (RecorderViewsMgr::GetInstance()->GetCurrentViewPage() == RecorderViewsMgr::ViewPage_Loading)
		{
			//记录用户操作
			GAThread::getInstance()->insertEventItem(Recorder_Widget, "StopRecordingHotKeyCount", "");
			SetUiState(UiState_Stop);
			RecorderViewsMgr::GetInstance()->ShowRecordWidget();
		}
	});
	connect(SettingsDataManager::GetInstance()->GetAdvancedData(), &AdvancedData::signalPauseResume, this, [&]{	
		if (RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_Pause)
		{
			//记录用户操作
			GAThread::getInstance()->insertEventItem(Recorder_Widget, "ResumeRecordingHotKeyCount", "");
			DoResume();
		}
		else if (RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_Recording)
		{
			//记录用户操作
			GAThread::getInstance()->insertEventItem(Recorder_Widget, "PauseRecordingHotKeyCount", "");
			DoPause();
		}
	});

    connect(SettingsDataManager::GetInstance()->GetCameraData(), &CameraData::signalCurrentDeviceLost, this, [&](QString name){
        if (RecorderCenter::GetInstance()->GetReordState() != RecorderCenter::RecordState_None)
        {
            DoStop();
        }
        else
        {
            RecorderViewsMgr::GetInstance()->GetCameraPreview()->setVisible(false);
        }
		m_pPreviewCtrlWidget->Reflesh();
	});
	connect(SettingsDataManager::GetInstance()->GetAudioData(), &AudioData::signalCurrentDeviceLost, this, [&](QString name){
		if (RecorderCenter::GetInstance()->GetReordState() != RecorderCenter::RecordState_None)
		{
			DoStop();
		}
        else
        {
            RecorderViewsMgr::GetInstance()->GetAudioPreview()->setVisible(false);
        }
		m_pPreviewCtrlWidget->Reflesh();
	});
	connect(SettingsDataManager::GetInstance()->GetGameData(), &GameData::signalCurrentDeviceLost, this, [&](QString name){
		if (RecorderCenter::GetInstance()->GetReordState() != RecorderCenter::RecordState_None)
		{
			DoStop();
		}
        else
        {
            RecorderViewsMgr::GetInstance()->GetGamePreview()->setVisible(false);
        }
        m_pPreviewCtrlWidget->Reflesh();
	});

	connect(RecorderCenter::GetInstance(), &RecorderCenter::sigStartFail, this, [&](QString errString){

		SetUiState(UiState_Stop);
		RecorderViewsMgr::GetInstance()->ShowRecordWidget();
		SysTrayIcon::GetInstance()->sigOpen();
        FSMessageBox::informationOk(tr(sFilmoraScreen), errString, RecorderViewsMgr::GetInstance()->GetRecordWidget());

	});

	connect(m_pFreeSpaceChecker, &FreeSpaceChecker::sigSpaceNotEnough, this, [&]{
		DoPause();
        FSMessageBox::informationOk(tr("Filmora Scrn"), m_strNoEnoughSpace, RecorderViewsMgr::GetInstance()->GetRecordWidget());
		DoStop();
	});

    connect(Communicator::GetInstance(), &Communicator::sigShowRecorder, RecorderViewsMgr::GetInstance()->GetRecordWidget(), [&]{
        if (!RecordingPreviewDataManager::GetInstance()->GetAllPaths().isEmpty())
            SetPreviewWindowShowed();
    }, Qt::QueuedConnection);

}

void RecordPanel::DoStart()
{

    
    SetUiState(UiState_Recording);

    m_pSyncPanelState->ButtonRecordSetEnabled(false);
    m_pSyncPanelState->ButtonPauseResumeSetEnabled(false);
    m_pSyncPanelState->ButtonDeleteSetEnabled(false);


    RecorderViewsMgr::GetInstance()->ShowRecordingView();
    //m_pSyncPanelState->StartRecorder();
    m_pFreeSpaceChecker->StartCheck();

	RecorderCenter::GetInstance()->Start();
    m_pSyncPanelState->CountBackwardCtrlStart();

    QTimer::singleShot(2000, [&](){
        m_pSyncPanelState->ButtonRecordSetEnabled(true);
        m_pSyncPanelState->ButtonPauseResumeSetEnabled(true);
        m_pSyncPanelState->ButtonDeleteSetEnabled(true);

    });
}

void RecordPanel::DoStop()
{ 
	GAThread::getInstance()->startTime();
	m_pFreeSpaceChecker->StopCheck();

    //hide();
    // 需要先将摄像头预览关闭，才能停止录制，底层有这个限制
    RecorderViewsMgr::GetInstance()->GetCameraPreview()->setVisible(false);

    RecorderCenter::GetInstance()->Stop();
    emit sigNormalStop(RecorderCenter::GetInstance()->GetOutputFile());

    RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);
	//上传GA数据
	GAThread::getInstance()->insertEventItem(Change_Page, "YourClips","");
	SetUiState(UiState_Stop);
    m_pSyncPanelState->StopRecorder();

    // 录制完成修改Editor导入路径为录制文件夹
    QSettings settings(FSGetFilmoraScreenDirectory() + "/FilmoraScrn.ini", QSettings::IniFormat);
    settings.beginGroup("Common");
    settings.setValue("MatterViewOpenDir", SettingsDataManager::GetInstance()->GetAdvancedData()->GetRecordFilePath());
    settings.endGroup();

    //QTimer::singleShot(0, [&](){
    //    StopRecordMediaImportDlg dlg(this, RecorderViewsMgr::GetInstance()->GetFSRecorderMainView());
    //    dlg.DoModal();
    //});
	//上传GA数据
	int iCostTime = GAThread::getInstance()->getElapse();
	GAThread::getInstance()->insertEventItem(YourClipsLaunchFromRecorder, "YourClipsTime", QString::number(iCostTime));
}

void RecordPanel::DoPause()
{
	SetUiState(UiState_Pause);
	//m_pCountBackwardCtrl->Pause();
    m_pSyncPanelState->CountBackwardCtrlPause();
	RecorderViewsMgr::GetInstance()->GetActionWidget()->SetPaused(true);
	RecorderCenter::GetInstance()->Pause();
}

void RecordPanel::DoResume()
{
	SetUiState(UiState_Recording);
	//m_pCountBackwardCtrl->Resume();
    m_pSyncPanelState->CountBackwardCtrlResume();
	RecorderViewsMgr::GetInstance()->GetActionWidget()->SetPaused(false);
	RecorderCenter::GetInstance()->Resume();
}

void RecordPanel::DoLoading()
{
	if (m_pFreeSpaceChecker->DoCheck())
	{
        FSMessageBox::informationOk(tr("Filmora Scrn"), m_strNoEnoughSpace, RecorderViewsMgr::GetInstance()->GetRecordWidget());
		return;
	}

    SettingsDataManager::GetInstance()->GetGameData()->SetHaveGameRecord();
    // 全屏游戏录制马上开始
    bool ret = SettingsDataManager::GetInstance()->GetGameData()->IsGameFullScreen();
	if (!ret)
	{
		RecorderCenter::GetInstance()->PrepareCameraStream();
		RecorderViewsMgr::GetInstance()->ShowLoadingView();
	}
    else
    {
        DoStart();
    }
}

void RecordPanel::Resize()
{
	m_nCommontlabelHeight = SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled() ? 30 : 0;
	int w = 300;
	int h = 276 + m_pPreviewCtrlWidget->Height() + m_nCommontlabelHeight;

	if (size().width() == w && size().height() == h)
	{
		resizeEvent(nullptr);
	}
	else
	{
        setFixedSize(300, 286 + m_pPreviewCtrlWidget->Height() + m_nCommontlabelHeight);
	}
}

void RecordPanel::SetCommentTip(int state)
{
	m_nCommontTipState = state;
	QString text = m_nCommontTipState == 0 ? tr("press Shift to lock aspect ratio") : tr("press %1 to switch to full screen mode").arg(SettingsDataManager::GetInstance()->GetAdvancedData()->GetFullScreen());
    QString elideText = StringOperation::GetElidedString(m_pLbComments->font(), m_pLbComments->width(), text);
    m_pLbComments->setText(elideText);
    m_pLbComments->setToolTip(text);
}

void RecordPanel::SetUiState(UiState s)
{
	m_UiState = s;
	if (s == UiState_Stop)
	{
		//m_pBtnPauseResume->SetEnabled(false);
		//m_pBtnDelete->SetEnabled(false);
		//m_pBtnRecord->SetCurrentId(RecordButton::ButtonId_Start);
        m_pSyncPanelState->ButtonRecordSetCurrentId(RecordButton::ButtonId_Start);
        m_pSyncPanelState->ButtonPauseResumeSetEnabled(false);
        m_pSyncPanelState->ButtonDeleteSetEnabled(false);
        m_pPreviewCtrlWidget->SetIndicatorOn(false);

		//m_pCountBackwardCtrl->Stop();
        m_pSyncPanelState->CountBackwardCtrlStop();
	}
	else if (s == UiState_Pause)
	{
		//m_pBtnPauseResume->SetEnabled(true);
        //m_pBtnPauseResume->SetCurrentButtonId(1);
        m_pSyncPanelState->ButtonPauseResumeSetEnabled(true);
        m_pSyncPanelState->ButtonPauseResumeSetCurrentId(1);
        m_pSyncPanelState->ButtonDeleteSetEnabled(true);
        //m_pBtnDelete->SetEnabled(true);
		m_pPreviewCtrlWidget->SetIndicatorOn(true);
	}
	else if (s == UiState_Recording)
	{
		//m_pBtnPauseResume->SetEnabled(true);
		//m_pBtnPauseResume->SetCurrentButtonId(0);
        m_pSyncPanelState->ButtonPauseResumeSetEnabled(true);
        m_pSyncPanelState->ButtonPauseResumeSetCurrentId(0);
        m_pSyncPanelState->ButtonDeleteSetEnabled(true);
        //m_pBtnDelete->SetEnabled(true);
		//m_pBtnRecord->SetCurrentId(RecordButton::ButtonId_Stop);
        m_pSyncPanelState->ButtonRecordSetCurrentId(RecordButton::ButtonId_Stop);
        m_pPreviewCtrlWidget->SetIndicatorOn(true);
	}

    if (s == UiState_Recording)
        SettingsDataManager::GetInstance()->GetAdvancedData()->SetScreenSaver(false);
    else
        SettingsDataManager::GetInstance()->GetAdvancedData()->SetScreenSaver(true);

}

void RecordPanel::EnableStopButton(bool bEnable)
{
	//m_pBtnRecord->setEnabled(bEnable);
    m_pSyncPanelState->ButtonRecordSetEnabled(bEnable);
}

void RecordPanel::SetCommentVisible(bool bVisible)
{
	m_pLbComments->setVisible(bVisible);
}

bool RecordPanel::IsCameraPreviewEnabled()
{
	return m_pPreviewCtrlWidget->IsCameraPreviewOn();
}

bool RecordPanel::IsGamePreviewEnabled()
{
	return m_pPreviewCtrlWidget->IsGamePreviewOn();

}

bool RecordPanel::IsAudioPreviewEnabled()
{
	return m_pPreviewCtrlWidget->IsMicrophonePreviewOn();
}

void RecordPanel::SetPreviewWindowShowed()
{
	m_bPreviewShowed = true;
}

PreviewCtrlWidget * RecordPanel::GetPreviewCtrlWidget()
{
	return m_pPreviewCtrlWidget;
}

bool RecordPanel::CanGo2PreviewWin()
{
	return m_bPreviewShowed;
}

void RecordPanel::SetVisible(bool visible, bool force)
{
    if (force)
    {
        m_pSyncPanelState->setVisible(visible);
    }
    else
    {
        if (visible)
        {
            //FSBaseWidget::setVisible(true);
            m_pSyncPanelState->setVisible(true);
        }
        else
        {
            // 非LOCK且在录制区内
            if (!GetLock() && PrimaryScreenManager::GetInstance()->PanelInRecordingRectangle(GetRealRecordWidget()))
            {
                m_pSyncPanelState->setVisible(false);
            }
        }
    }

}

bool RecordPanel::GetLock()
{
    return m_pBtnLock->CurrentButtonId();
}


QWidget* RecordPanel::GetRealRecordWidget()
{
    return m_pSyncPanelState->GetRealRecordWidget();

}

void RecordPanel::ShowLoadingView()
{
    m_pSyncPanelState->StartRecorder();
}

void RecordPanel::LoadingAbort()
{
    m_pSyncPanelState->StopRecorder();
}

void RecordPanel::RetranslateUi()
{
    m_pBtnSettings->setText(tr("Settings"));
    m_pBtnPreview->setText(tr("Preview"));
    m_pBtnEditor->setText(tr("Editor"));


	SetCommentTip(m_nCommontTipState);
    m_pBtnDelete->setToolTip(tr("Delete recording file"));
    m_pBtnPauseResume->GetButton(0)->setToolTip(tr("Pause capturing"));
    m_pBtnPauseResume->GetButton(1)->setToolTip(tr("Resume capturing"));
    m_pBtnMinimized->setToolTip(tr("Minimize"));
    m_pBtnMiniPanel->setToolTip(tr("MiniPanel"));
    m_pBtnClosed->setToolTip(tr("Close"));
    m_pBtnLock->GetButton(0)->setToolTip(tr("Lock"));
    m_pBtnLock->GetButton(1)->setToolTip(tr("UnLock"));

    m_pMiniRecordPanel->SetToolTip();

    resizeEvent(nullptr);
}

void RecordPanel::OnStartButtonClicked()
{
	if (m_pFreeSpaceChecker->DoCheck())
	{
        FSMessageBox::informationOk(tr("Filmora Scrn"), m_strNoEnoughSpace, RecorderViewsMgr::GetInstance()->GetRecordWidget());
		QTimer::singleShot(0, [&]{
            //m_pBtnRecord->SetCurrentId(RecordButton::ButtonId_Start); 
            m_pSyncPanelState->ButtonRecordSetCurrentId(RecordButton::ButtonId_Start);
        });
		return;
	}
	//RecorderViewsMgr::GetInstance()->ShowLoadingView();
    SettingsDataManager::GetInstance()->GetGameData()->SetHaveGameRecord();
    // 全屏游戏录制马上开始
    bool ret = SettingsDataManager::GetInstance()->GetGameData()->IsGameFullScreen();
	if (!ret)
	{
		RecorderCenter::GetInstance()->PrepareCameraStream();
		RecorderViewsMgr::GetInstance()->ShowLoadingView();
	}
    else
    {
        DoStart();
    }

}

void RecordPanel::OnRecordStateChanged()
{
    if (RecorderCenter::GetInstance()->GetReordState() != RecorderCenter::RecordState_None)
    {
        m_pBtnSettings->setEnabled(false);
        m_pBtnPreview->setEnabled(false);
        m_pBtnEditor->setEnabled(false);
    }
    else
    {
        m_pBtnSettings->setEnabled(true);
        m_pBtnEditor->setEnabled(true);
        m_pBtnPreview->setEnabled(m_bPreviewShowed || RecordingPreviewDataManager::GetInstance()->GetItemDatas().size() > 0);
    }
}


//
//void RecordPanel::closeEvent(QCloseEvent *e)
//{
//	if (ProjectSaver::GetInstance()->IsContentChanged())
//	{
//		int ret = FSMessageBox::questionYesNoCancel(tr(sFilmoraScreen), tr("The project has been modified,Do you want to save the changes?"), this);
//
//		if (ret == FSMessageBox::Yes)
//		{
//			if (ProjectSaver::GetInstance()->Save())
//				qApp->quit();
//
//		}
//		if (ret == FSMessageBox::No)
//		{
//			qApp->quit();
//		}
//		if (ret == FSMessageBox::Cancel)
//		{
//			e->ignore();
//		}
//	}
//	else
//		qApp->quit();
//	
//}

void RecordPanel::resizeEvent(QResizeEvent *e)
{
	QRect rc = rect().adjusted(1, 1, -1, -1);
	const int titlebarH = 28;
	const int menuBarH = 31;
    const int margins = 10;

    m_pTitleBar->setGeometry(rc.left(), rc.top(), rc.width(), titlebarH);
	m_pLine1->setGeometry(rc.left(), m_pTitleBar->geometry().bottom() + 1, rc.width(), 1);

	m_pBtnClosed->setGeometry(m_pTitleBar->geometry().right() - 30,
		m_pTitleBar->geometry().top(),
		30,
		30);
	m_pBtnMinimized->setGeometry(m_pTitleBar->geometry().right() - 30 * 2, 
		m_pTitleBar->geometry().top(), 
		30, 
		30);

    m_pBtnMiniPanel->setGeometry(m_pTitleBar->geometry().right() - 30 * 3,
        m_pTitleBar->geometry().top(),
        30,
        30);

    m_pBtnLock->setGeometry(m_pTitleBar->geometry().right() - 30 * 4,
        m_pTitleBar->geometry().top(),
        30,
        30);


	int buttonW = 65;
	int buttonH = menuBarH;
	int l = rc.left() + 9;
	int t = m_pLine1->geometry().bottom();
    buttonW = StringOperation::GetTextTrueLenght(m_pBtnSettings->font(), m_pBtnSettings->text()) + margins;
	m_pBtnSettings->setGeometry(l, t, buttonW, buttonH);
    l = m_pBtnSettings->geometry().right();
    buttonW = StringOperation::GetTextTrueLenght(m_pBtnPreview->font(), m_pBtnPreview->text()) + margins;
    m_pBtnPreview->setGeometry(l, t, buttonW, buttonH);
    l = m_pBtnPreview->geometry().right();
    buttonW = StringOperation::GetTextTrueLenght(m_pBtnEditor->font(), m_pBtnEditor->text()) + margins;
    m_pBtnEditor->setGeometry(l, t, buttonW, buttonH);


	m_pLine2->setGeometry(rc.left(), titlebarH + menuBarH, rc.width(), 1);

	int aH = 157;
	buttonW = width();
	buttonH = 113;
	l = (rc.width() - buttonW)* 0.5;
	t = m_pLine2->geometry().bottom() + 20;
	m_pBtnRecord->setGeometry(l, t, buttonW, buttonH);

    buttonW = 30;
    buttonH = 30;
	l = width()-75;
	t = m_pLine2->geometry().bottom() + 40;
	
	m_pBtnPauseResume->setGeometry(l, t, buttonW, buttonH);

	t = m_pLine2->geometry().bottom() + aH;
	l = 20;
	m_pLine3->setGeometry(l, t, rc.width() - l * 2, 1);

	int w = 130;
	int h = 39;
	l = 27;
	aH = 65;
	t = m_pLine3->geometry().bottom() + (aH - h) * 0.5;
	m_pCountBackwardCtrl->setGeometry(l, t, w, h);

	buttonW = 20;
	buttonH = 20;
	l = rc.width() - 16 - buttonW;
    t = m_pLine3->geometry().bottom() + (aH - buttonH) * 0.5;
	m_pBtnDelete->setGeometry(l, t, buttonW, buttonH);

	t = m_pLine3->geometry().bottom() + 60;

	m_pPreviewCtrlWidget->setGeometry(rc.left(), t, rc.width(), m_pPreviewCtrlWidget->Height() - 2);
	m_pPreviewCtrlWidget->Resize();

	m_pLine5->setGeometry(rc.left(), m_pPreviewCtrlWidget->geometry().bottom() + 1, rc.width(), 1);
	if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
	{
        const int margins = 10;
        m_pLbComments->setGeometry(rc.left() + margins, m_pLine5->geometry().bottom(), rc.width() - 2 * margins, rc.height() - m_pLine5->geometry().bottom());
		//m_pLbComments->setContentsMargins(23, 0, 23, 0);
	}
	//m_pBlackMask->setGeometry(rc.left(), m_pLine1->geometry().bottom(), rc.width(), m_pLine4->geometry().top() - m_pLine1->geometry().bottom());


}

void RecordPanel::showEvent(QShowEvent *e)
{
    
	RetranslateUi();
	m_pPreviewCtrlWidget->Reflesh();
	SysTrayIcon::GetInstance()->SetVisible(true);
	if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled() 
		&&RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None)
	{
		//RecorderViewsMgr::GetInstance()->ShowRecordWidget();
		if (SettingsDataManager::GetInstance()->GetDesktopData()->GetType() == DesktopData::CaputerType_Fullscreen)
		{
			GetPreviewCtrlWidget()->GetDesktopPreviewWidget()->SetDesktopWH();
		}
		else
		{
			QRect dsk = RecordHelper::GetInstance()->DesktopRect();
			QRect rcCrop = SettingsDataManager::GetInstance()->GetDesktopData()->GetCropRect();
			if (rcCrop.width() == dsk.width() && rcCrop.height() == dsk.height())
			{
				auto cusRect = RecordHelper::GetInstance()->CustomRect();
				if (cusRect.isEmpty())
				{
                    cusRect = PrimaryScreenManager::GetInstance()->DefaultRecordRect();
				}
				RecordHelper::GetInstance()->SetRecordRect(cusRect);
				GetPreviewCtrlWidget()->GetDesktopPreviewWidget()->SetWidthEdit(cusRect.width());
				GetPreviewCtrlWidget()->GetDesktopPreviewWidget()->SetHeightEdit(cusRect.height());
			}
		}
	}

    PrimaryScreenManager::GetInstance()->SetDefaultRecordPanelPosition();
    OnRecordStateChanged();
    resizeEvent(nullptr);
}

//////////////////////////////////////////////////////////////////////////

SeperaterLine::SeperaterLine(QWidget *parent/* = 0*/)
	:FSBaseWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setObjectName("RecordPanel_SeperaterLine");
}


//////////////////////////////////////////////////////////////////////////
#define  ITEM_WIDTH 350
#define  ITEM_HEIGHT 51
#define  TOP_MARGIN 6
PreviewCtrlWidget::PreviewCtrlWidget(const QString &objname /*= ""*/,QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	setObjectName(objname);
	m_pDesktopItem = new DesktopPreviewItemWidget("RecorlPanel_PreviewItemWidget_DesktopItem", this);
	m_pGameItem = new GamePreviewItemWidget("RecorlPanel_PreviewItemWidget_GameItem", this);
	m_pCameraItem = new CameraPreviewItemWidget("RecorlPanel_PreviewItemWidget_CameraItem", this);
	m_pMicrophoneItem = new MicrophonePreviewItemWidget("RecorlPanel_PreviewItemWidget_MircophoneItem", this);
	m_pSystemAudioItem = new SystemAudioPreviewItemWidget("RecorlPanel_PreviewItemWidget_SystemAudioItem", this);
	connect(m_pGameItem->GetPreviewButton()->GetButton(0), &BaseButton::clicked, this, [&]{
		RecorderViewsMgr::GetInstance()->GetGamePreview()->setVisible(true);
	});
	connect(m_pGameItem->GetPreviewButton()->GetButton(1), &BaseButton::clicked, this, [&]{
        RecorderViewsMgr::GetInstance()->GetGamePreview()->SetManualClose();
    });
	connect(m_pCameraItem->GetPreviewButton()->GetButton(0), &BaseButton::clicked, this, [&]{
		RecorderViewsMgr::GetInstance()->GetCameraPreview()->setVisible(true);
	});
	connect(m_pCameraItem->GetPreviewButton()->GetButton(1), &BaseButton::clicked, this, [&]{
        RecorderViewsMgr::GetInstance()->GetCameraPreview()->SetManualClose();
	});
	connect(m_pMicrophoneItem->GetPreviewButton()->GetButton(0), &BaseButton::clicked, this, [&]{
		RecorderViewsMgr::GetInstance()->GetAudioPreview()->setVisible(true);
    });
	connect(m_pMicrophoneItem->GetPreviewButton()->GetButton(1), &BaseButton::clicked, this, [&]{
        RecorderViewsMgr::GetInstance()->GetAudioPreview()->SetManualClose();
    });
	//Reflesh(false);
}

int PreviewCtrlWidget::Height()
{
	return TOP_MARGIN + m_listEnbleItems.size() * ITEM_HEIGHT;
}

void PreviewCtrlWidget::Reflesh(bool bResizeParentWidget/* = true*/)
{
	m_listEnbleItems.clear();
	if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
		m_listEnbleItems.push_back(m_pDesktopItem);
	m_pDesktopItem->setVisible(SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled());
	((RecordPanel*)parentWidget())->SetCommentVisible(SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled());
	if (SettingsDataManager::GetInstance()->GetGameData()->GetDeviceEnabled())
	{
		m_pGameItem->RetranslateUi();
		m_listEnbleItems.push_back(m_pGameItem);
	}
	m_pGameItem->setVisible(SettingsDataManager::GetInstance()->GetGameData()->GetDeviceEnabled());

	if (SettingsDataManager::GetInstance()->GetCameraData()->GetDeviceEnabled())
	{
		m_pCameraItem->RetranslateUi();
		m_listEnbleItems.push_back(m_pCameraItem);
	}
	m_pCameraItem->setVisible(SettingsDataManager::GetInstance()->GetCameraData()->GetDeviceEnabled());

	if (SettingsDataManager::GetInstance()->GetAudioData()->GetDeviceEnabled())
	{
		m_pMicrophoneItem->RetranslateUi();
		m_listEnbleItems.push_back(m_pMicrophoneItem);
	}
	m_pMicrophoneItem->setVisible(SettingsDataManager::GetInstance()->GetAudioData()->GetDeviceEnabled());

	if (SettingsDataManager::GetInstance()->GetAudioData()->GetRecordSystemAudio())
		m_listEnbleItems.push_back(m_pSystemAudioItem);
	m_pSystemAudioItem->setVisible(SettingsDataManager::GetInstance()->GetAudioData()->GetRecordSystemAudio());

	((RecordPanel*)parentWidget())->EnableStopButton(!m_listEnbleItems.isEmpty());

	if (bResizeParentWidget)
	{
		((RecordPanel*)parentWidget())->Resize();
	}
}

void PreviewCtrlWidget::SetIndicatorOn(bool bOn)
{
	for (auto itemWidget : m_listEnbleItems)
		itemWidget->SetIndicatorOn(bOn);

}

void PreviewCtrlWidget::SetGamePreviewButtonOff()
{
	m_pGameItem->SetPreviewButtonOff();
}

void PreviewCtrlWidget::SetCameraPreviewButtonOff()
{
	m_pCameraItem->SetPreviewButtonOff();
}

void PreviewCtrlWidget::SetMicrophonePreviewButtonOff()
{
	m_pMicrophoneItem->SetPreviewButtonOff();
}

bool PreviewCtrlWidget::IsCameraPreviewOn()
{
	return m_pCameraItem->IsPreviewOn();
}

bool PreviewCtrlWidget::IsGamePreviewOn()
{
	return m_pGameItem->IsPreviewOn();

}

bool PreviewCtrlWidget::IsMicrophonePreviewOn()
{
	return m_pMicrophoneItem->IsPreviewOn();
}

void PreviewCtrlWidget::Resize()
{
	resizeEvent(nullptr);
}

DesktopPreviewItemWidget* PreviewCtrlWidget::GetDesktopPreviewWidget()
{
	return m_pDesktopItem;
}

void PreviewCtrlWidget::resizeEvent(QResizeEvent *e)
{
	int l = 0;
	int t = TOP_MARGIN;
	for (auto itemWidget : m_listEnbleItems)
	{
		itemWidget->setGeometry(l, t, width() - l * 2 , ITEM_HEIGHT);
		t += ITEM_HEIGHT;
	}
	//m_pCameraItem->setGeometry(l,t,ITEM_WIDTH,ITEM_HEIGHT);
}

//////////////////////////////////////////////////////////////////////////

PreviewItemWidgetBase::PreviewItemWidgetBase(const QString &objname /*= ""*/, QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	setObjectName(objname);
	m_pIndicator = new Flicker("RecordPanel_PreviewItemWidget_Flicker",this);
	m_pIcon = new QLabel(this);
	m_pIcon->setAlignment(Qt::AlignCenter);
	m_pIcon->setObjectName("RecordPanel_PreviewItemWidget_Icon");
	m_pTitle = new QLabel(this);
	m_pTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pTitle->setObjectName("RecordPanel_PreviewItemWidget_m_pTitle");
	m_pBtnPreview = new BaseStateButton(0,"RecordPanel_PreviewItemWidget_PreviewButton", this);
	m_pBtnPreview->AddButton(new BaseButton(0, "RecordPanel_PreviewItemWidget_PreviewButton_Enable", m_pBtnPreview));
	m_pBtnPreview->AddButton(new BaseButton(1, "RecordPanel_PreviewItemWidget_PreviewButton_Disenable", m_pBtnPreview));
}

void PreviewItemWidgetBase::SetIndicatorOn(bool bOn)
{
	m_pIndicator->SetOn(bOn);
}

void PreviewItemWidgetBase::SetPreviewButtonOff()
{
	m_pBtnPreview->SetCurrentButtonId(0);
}

bool PreviewItemWidgetBase::IsPreviewOn()
{
	return m_pBtnPreview->CurrentButtonId() == 1;
}


BaseStateButton * PreviewItemWidgetBase::GetPreviewButton()
{
	return m_pBtnPreview;
}

void PreviewItemWidgetBase::resizeEvent(QResizeEvent *e)
{
	int w = 9;
	int h = 9;
	int l = 27;
	int t = (height() - h)*0.5;
	m_pIndicator->setGeometry(l, t, w, h);
	
	l = m_pIndicator->geometry().right() + 10;
	w = 24;
	h = 24;
	t = (height() - h)*0.5;
	m_pIcon->setGeometry(l, t, w, h);

	w = 24;
	h = 20;
	l = width()- w-16;
	t = (height() - h)*0.5;
	m_pBtnPreview->setGeometry(l, t, w, h);


	l = m_pIcon->geometry().right() + 10;
	t = 0;
	w = m_pBtnPreview->geometry().left() - 20-l;
	h = height();
	
	m_pTitle->setGeometry(l, t, w, h);
}


void PreviewItemWidgetBase::RetranslateUi()
{
    m_pBtnPreview->setToolTip(tr("Preview"));
}

//////////////////////////////////////////////////////////////////////////
DesktopPreviewItemWidget::DesktopPreviewItemWidget(const QString &objname /*= ""*/, QWidget *parent /*= 0*/)
	:PreviewItemWidgetBase(objname,parent)
{
	m_pIcon->setObjectName("RecordPanel_PreviewItemWidget_DesktopIcon");
    m_pEditWidth = new FSLineEdit("RecordPanel_PreviewItemWidget_DesktopItem_WidthEdit", this);
	m_pEditWidth->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	//m_pEditWidth->setObjectName("RecordPanel_PreviewItemWidget_DesktopItem_WidthEdit");
	m_pEditWidth->setValidator(new QIntValidator(0, 90000, this));
	m_pEditWidth->setText(QString("%1").arg(RecordHelper::GetInstance()->RecordRect().width()));

    m_pEditHeight = new FSLineEdit("RecordPanel_PreviewItemWidget_DesktopItem_HeightEdit", this);
	m_pEditHeight->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	//m_pEditHeight->setObjectName("RecordPanel_PreviewItemWidget_DesktopItem_HeightEdit");
	m_pEditHeight->setValidator(new QIntValidator(0, 90000, this));
	m_pEditHeight->setText(QString("%1").arg(RecordHelper::GetInstance()->RecordRect().height()));


	m_pLbX = new QLabel(this);
	m_pLbX->setAlignment(Qt::AlignCenter);
	m_pLbX->setObjectName("RecordPanel_PreviewItemWidget_DesktopItem_XLabel");
	m_pLbX->setText("x");
	m_pBtnLock = new BaseStateButton(0,"",this);
	m_pBtnLock->AddButton(new BaseButton(0,"RecordPanel_PreviewItemWidget_DesktopItem_LockEnableButton",m_pBtnLock));
	m_pBtnLock->AddButton(new BaseButton(1, "RecordPanel_PreviewItemWidget_DesktopItem_LockDisnableButton", m_pBtnLock));
	m_pTitle->setVisible(false);
	m_pBtnPreview->setVisible(false);

	connect(m_pBtnLock->GetButton(0), &BaseButton::clicked, this, [&]{
		RecordHelper::GetInstance()->SetLock(true);
	});
	connect(m_pBtnLock->GetButton(1), &BaseButton::clicked, this, [&]{
		RecordHelper::GetInstance()->SetLock(false);
	});
	connect(m_pEditWidth, &QLineEdit::editingFinished, this, [&]{

		QRect recordRect = RecordHelper::GetInstance()->RecordRect();
		int v = m_pEditWidth->text().toUInt();
		if (v == recordRect.width()) return;
		if (recordRect.left() + v > RecordHelper::GetInstance()->GetMaxW() || v < RecordHelper::GetInstance()->GetMinW())
		{
			SetWidthEdit(recordRect.width());
			return;
		}
		if (!RecordHelper::GetInstance()->IsLock())
		{
			recordRect.setWidth(v);
			SetWidthEdit(v);
		}
		else
		{
			double dRatio = RecordHelper::GetInstance()->GetRatio();
			QRect rcTemp = recordRect;
			recordRect.setWidth(v);
			recordRect.setHeight(v / dRatio);
			if (recordRect.height() < RecordHelper::GetInstance()->GetMinH() || recordRect.bottom() >= RecordHelper::GetInstance()->GetMaxH())
			{
				recordRect = rcTemp;
			}
			SetWidthEdit(recordRect.width());
			SetHeightEdit(recordRect.height());
		}
		RecordHelper::GetInstance()->SetRecordRect(recordRect);
		emit RecordHelper::GetInstance()->sigResolutionChangedByEdit(recordRect);

	});
	connect(m_pEditHeight, &QLineEdit::editingFinished, this, [&]{

		QRect recordRect = RecordHelper::GetInstance()->RecordRect();
		int v = m_pEditHeight->text().toUInt();
		if (v == recordRect.height()) return;
		if (recordRect.top() + v > RecordHelper::GetInstance()->GetMaxH() || v < RecordHelper::GetInstance()->GetMinH())
		{
			SetHeightEdit(recordRect.height());
			return;
		}

		if (!RecordHelper::GetInstance()->IsLock())
		{
			recordRect.setHeight(v);
			SetHeightEdit(v);
		}
		else
		{
			double dRatio = RecordHelper::GetInstance()->GetRatio();
			QRect rcTemp = recordRect;
			recordRect.setHeight(v);
			recordRect.setWidth(v * dRatio);
			if (recordRect.width() < RecordHelper::GetInstance()->GetMinW() || recordRect.right() >= RecordHelper::GetInstance()->GetMaxW())
			{
				recordRect = rcTemp;
			}
			SetWidthEdit(recordRect.width());
			SetHeightEdit(recordRect.height());
		}
		RecordHelper::GetInstance()->SetRecordRect(recordRect);
		emit RecordHelper::GetInstance()->sigResolutionChangedByEdit(recordRect);
	});

	connect(RecordHelper::GetInstance(), &RecordHelper::sigResolutionChangedByDrag, this, [&](QRect rc){
		SetWidthEdit(rc.width());
		SetHeightEdit(rc.height());
	});
	connect(RecordHelper::GetInstance(), &RecordHelper::sigShiftLock, this, [&](bool bLock){
		m_pBtnLock->SetCurrentButtonId(bLock ? 1 : 0);
	});
	connect(SettingsDataManager::GetInstance()->GetAdvancedData(), &AdvancedData::signalFullscreen, this, [&]{
		//记录用户按键信息
		GAThread::getInstance()->insertEventItem(Recorder_Widget, "FullCustomSwitchHotkeyCount", "");
		if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled() 
			&& RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None)
		{
			RecordHelper *pHelper = RecordHelper::GetInstance();
			if (SettingsDataManager::GetInstance()->GetDesktopData()->GetType() == DesktopData::CaputerType_Custom)
			{
				SetWidthEdit(pHelper->DesktopRect().width());
				SetHeightEdit(pHelper->DesktopRect().height());
				pHelper->SetRecordRect(pHelper->DesktopRect());
				pHelper->UpdateRatio();
                pHelper->sigResolutionChangedByEdit(pHelper->DesktopRect());
			}
			else if (SettingsDataManager::GetInstance()->GetDesktopData()->GetType() == DesktopData::CaputerType_Fullscreen)
			{
				SetWidthEdit(pHelper->PreviousRect().width());
				SetHeightEdit(pHelper->PreviousRect().height());
				pHelper->SetRecordRect(pHelper->PreviousRect());
				pHelper->UpdateRatio();
                pHelper->sigResolutionChangedByEdit(pHelper->PreviousRect());
            }
		}
	});
	connect(RecorderCenter::GetInstance(), &RecorderCenter::sigRecordStateChange, this, [&]{
		m_pEditWidth->setEnabled(RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None);
		m_pEditHeight->setEnabled(RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None);
        m_pBtnLock->SetEnabled(RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None);
        m_pBtnSwitchScreen->setEnabled(RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None);
	});

    m_pBtnSwitchScreen = new BaseStateButton(0, "", this);
    m_pBtnSwitchScreen->AddButton(new BaseButton(0, "RecordPanel_PreviewItemWidget_DesktopItem_SwitchScreen_Primary", this));
    m_pBtnSwitchScreen->AddButton(new BaseButton(1, "RecordPanel_PreviewItemWidget_DesktopItem_SwitchScreen_SecondScreen", this));
    m_pBtnSwitchScreen->setVisible(PrimaryScreenManager::GetInstance()->ScreenCount() > 1);

    connect(m_pBtnSwitchScreen, &BaseStateButton::Clicked, this, [&](int id){
        PrimaryScreenManager::GetInstance()->SwitchRecordScreen();
    });

    RetranslateUi();
}

void DesktopPreviewItemWidget::SetDesktopWH()
{
    SetWidthEdit(PrimaryScreenManager::GetInstance()->PrimaryScreenWidth());
    SetHeightEdit(PrimaryScreenManager::GetInstance()->PrimaryScreenHeight());
    //SetWidthEdit(DesktopMgr::GetInstance()->MainScreen()->width());
	//SetHeightEdit(DesktopMgr::GetInstance()->MainScreen()->height());
}

void DesktopPreviewItemWidget::SetWidthEdit(int w)
{
	m_pEditWidth->setText(QString("%1").arg(w));
}

void DesktopPreviewItemWidget::SetHeightEdit(int h)
{
	m_pEditHeight->setText(QString("%1").arg(h));
}

void DesktopPreviewItemWidget::resizeEvent(QResizeEvent *e)
{
	PreviewItemWidgetBase::resizeEvent(e);
	int w = 60;
	int h = 24;
	int l = m_pIcon->geometry().right() + 10;
	int t = (height() - h) * 0.5;
	m_pEditWidth->setGeometry(l, t, w, h);

	w = 8;
	h = 24;
	l = m_pEditWidth->geometry().right() + 4;
	t = (height() - h) * 0.5;
	m_pLbX->setGeometry(l, t, w, h);

	w = 60;
	h = 24;
	l = m_pLbX->geometry().right() + 4;
	t = (height() - h) * 0.5;
	m_pEditHeight->setGeometry(l, t, w, h);

	w = 13;
	h = 13;
	l = m_pEditHeight->geometry().right() + 8;
	t = (height() - h) * 0.5;
	m_pBtnLock->setGeometry(l, t, w, h);

    w = 30;
    h = 24;
    l = width() - 40;
    t = (height() - h) * 0.5;
    m_pBtnSwitchScreen->setGeometry(l, t, w, h);
}


void DesktopPreviewItemWidget::RetranslateUi()
{
    m_pBtnLock->GetButton(0)->setToolTip(tr("Lock aspect ratio"));
    m_pBtnLock->GetButton(1)->setToolTip(tr("Lock aspect ratio"));
    PreviewItemWidgetBase::RetranslateUi();
}

//////////////////////////////////////////////////////////////////////////
SystemAudioPreviewItemWidget::SystemAudioPreviewItemWidget(const QString &objname /*= ""*/, QWidget *parent /*= 0*/)
	:PreviewItemWidgetBase(objname, parent)
{
	m_pIcon->setObjectName("RecordPanel_PreviewItemWidget_SystemAudioIcon");
	m_pBtnPreview->setVisible(false);
	RetranslateUi();
}

void SystemAudioPreviewItemWidget::RetranslateUi()
{
    QString text(tr("Desktop Audio"));
    QString strText = StringOperation::GetElidedString(m_pTitle->font(), m_pTitle->width(), text);
    m_pTitle->setText(strText);
    m_pTitle->setToolTip(text);
    PreviewItemWidgetBase::RetranslateUi();
}

void SystemAudioPreviewItemWidget::resizeEvent(QResizeEvent *e)
{
    PreviewItemWidgetBase::resizeEvent(e);
    RetranslateUi();
}

//////////////////////////////////////////////////////////////////////////
MicrophonePreviewItemWidget::MicrophonePreviewItemWidget(const QString &objname /*= ""*/, QWidget *parent /*= 0*/)
	:PreviewItemWidgetBase(objname, parent)
{
	m_pIcon->setObjectName("RecordPanel_PreviewItemWidget_MicrophoneIcon");
	RetranslateUi();
	connect(RecorderViewsMgr::GetInstance()->GetAudioPreview(), &MicrophonePreview::signalHide, this, [&]{
		m_pBtnPreview->SetCurrentButtonId(0);
	});
    connect(RecorderViewsMgr::GetInstance()->GetAudioPreview(), &MicrophonePreview::signalShow, this, [&]{
        m_pBtnPreview->SetCurrentButtonId(1);
    });
}

void MicrophonePreviewItemWidget::RetranslateUi()
{
    QString text(QString::fromWCharArray(SettingsDataManager::GetInstance()->GetAudioData()->GetCurrentDevice().displayName));
    QString strText = StringOperation::GetElidedString(m_pTitle->font(), m_pTitle->width(), text);
	m_pTitle->setText(strText);
    m_pTitle->setToolTip(text);
    PreviewItemWidgetBase::RetranslateUi();
}

void MicrophonePreviewItemWidget::resizeEvent(QResizeEvent *e)
{
	PreviewItemWidgetBase::resizeEvent(e);
	RetranslateUi();
}

//////////////////////////////////////////////////////////////////////////
CameraPreviewItemWidget::CameraPreviewItemWidget(const QString &objname /*= ""*/, QWidget *parent /*= 0*/)
	:PreviewItemWidgetBase(objname, parent)
{
	m_pIcon->setObjectName("RecordPanel_PreviewItemWidget_CameraIcon");
	RetranslateUi();
    connect(RecorderViewsMgr::GetInstance()->GetCameraPreview(), &PreviewBase::signalHide, this, [&]{
        m_pBtnPreview->SetCurrentButtonId(0);
    });
    connect(RecorderViewsMgr::GetInstance()->GetCameraPreview(), &PreviewBase::signalShow, this, [&]{
        m_pBtnPreview->SetCurrentButtonId(1);
    });
}

void CameraPreviewItemWidget::RetranslateUi()
{
    QString text(QString::fromWCharArray(SettingsDataManager::GetInstance()->GetCameraData()->GetCurrentDevice().displayName));
    QString strText = StringOperation::GetElidedString(m_pTitle->font(), m_pTitle->width(), text);
	m_pTitle->setText(strText);
    m_pTitle->setToolTip(text);
    PreviewItemWidgetBase::RetranslateUi();
}

void CameraPreviewItemWidget::resizeEvent(QResizeEvent *e)
{
	PreviewItemWidgetBase::resizeEvent(e);
	RetranslateUi();
}

//////////////////////////////////////////////////////////////////////////
GamePreviewItemWidget::GamePreviewItemWidget(const QString &objname /*= ""*/, QWidget *parent /*= 0*/)
	:PreviewItemWidgetBase(objname, parent)
{
	m_pIcon->setObjectName("RecordPanel_PreviewItemWidget_GameIcon");
	RetranslateUi();
	connect(RecorderViewsMgr::GetInstance()->GetGamePreview(), &PreviewBase::signalHide, this, [&]{
		m_pBtnPreview->SetCurrentButtonId(0);
	});
    connect(RecorderViewsMgr::GetInstance()->GetGamePreview(), &PreviewBase::signalShow, this, [&]{
        m_pBtnPreview->SetCurrentButtonId(1);
    });
}

void GamePreviewItemWidget::RetranslateUi()
{
    QString strText = StringOperation::GetElidedString(m_pTitle->font(), m_pTitle->width(), SettingsDataManager::GetInstance()->GetGameData()->GetGameName());
	m_pTitle->setText(strText);
    PreviewItemWidgetBase::RetranslateUi();
}

void GamePreviewItemWidget::resizeEvent(QResizeEvent *e)
{
	PreviewItemWidgetBase::resizeEvent(e);
	RetranslateUi();
}

//////////////////////////////////////////////////////////////////////////
Flicker::Flicker(const QString &objname /*= ""*/, QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	setObjectName(objname);
	m_pLbOn = new QLabel(this);
	m_pLbOn->setAlignment(Qt::AlignCenter);
	m_pLbOn->setObjectName("RecordPanel_PreviewItemWidget_Flicker_OnLabel");
	m_pLbOff = new QLabel(this);
	m_pLbOff->setAlignment(Qt::AlignCenter);
	m_pLbOff->setObjectName("RecordPanel_PreviewItemWidget_Flicker_OffLabel");
	SetOn(false);
}

void Flicker::SetOn(bool bOn)
{
	m_pLbOn->setVisible(bOn);
	m_pLbOff->setVisible(!bOn);
}

void Flicker::resizeEvent(QResizeEvent *e)
{
	m_pLbOn->setGeometry(rect());
	m_pLbOff->setGeometry(rect());
}

//////////////////////////////////////////////////////////////////////////
#define  RecordButton_Edge 70
RecordButton::RecordButton(const QString &objname/* = ""*/, QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	setObjectName(objname);
	m_pLbTip = new QLabel(this);
	m_pLbTip->setObjectName("RecordPanel_RecordButton_StateTextlabel");
    m_pLbTip->setAlignment(Qt::AlignCenter);
	m_pLbShortcut = new QLabel(this);
	m_pLbShortcut->setObjectName("RecordPanel_RecordButton_Shortcutlabel");
    m_pLbShortcut->setAlignment(Qt::AlignVCenter);


	BaseButton *pBtn = new BaseButton(ButtonId_Start,"RecordPanel_RecordButton_Start", this);
	m_lstButtons.push_back(pBtn);
	connect(pBtn, &BaseButton::clicked, this, [&](){
		emit sigClicked(ButtonId_Start);
		SetCurrentId(ButtonId_Stop);
	});
	pBtn = new BaseButton(ButtonId_Stop, "RecordPanel_RecordButton_Stop", this);
	m_lstButtons.push_back(pBtn);
	connect(pBtn, &BaseButton::clicked, this, [&](){
		emit sigClicked(ButtonId_Stop);
		SetCurrentId(ButtonId_Start);
	});

	SetCurrentId(ButtonId_Start);
	RetranslateUi();
}

void RecordButton::showEvent(QShowEvent *e)
{    
	RetranslateUi();
    resizeEvent(nullptr);
}

void RecordButton::resizeEvent(QResizeEvent *e)
{
	int ButtonEdge = RecordButton_Edge;
	int LabelH = 30;
	int margin = 5;
	
	for (auto btn : m_lstButtons)
	{
		btn->setGeometry((width() - ButtonEdge) * 0.5, 0, ButtonEdge, ButtonEdge);
	}
    int len1 = StringOperation::GetTextTrueLenght(m_pLbTip->font(), m_pLbTip->text());
    int len2 = StringOperation::GetTextTrueLenght(m_pLbShortcut->font(), m_pLbShortcut->text());
	int len = len1 + len2;

	int l = (width() - len) * 0.5;
	int t = m_lstButtons[0]->geometry().bottom() + 17;
	m_pLbTip->setGeometry(l,t,len1,LabelH);
	l = m_pLbTip->geometry().right() + 1;
    m_pLbShortcut->setGeometry(l, t, len2, LabelH);
}

void RecordButton::RetranslateUi()
{ 
	if (m_CurrentID == ButtonId_Start)
	{
		m_pLbTip->setText(tr("Start  capturing "));
		m_pLbShortcut->setText(" " + SettingsDataManager::GetInstance()->GetAdvancedData()->GetStartStop());
	}
	else if (m_CurrentID == ButtonId_Stop)
	{
		m_pLbTip->setText(tr("Stop "));
		m_pLbShortcut->setText(" " + SettingsDataManager::GetInstance()->GetAdvancedData()->GetStartStop());
	}
	//else if (m_CurrentID == ButtonId_Resume)
	//{
	//	m_pLbTip->setText(tr("Resume"));
	//	m_pLbShortcut->setText(" " + SettingsDataManager::GetInstance()->GetAdvancedData()->GetPauseResume());
	//}
    m_lstButtons[ButtonId_Start]->setToolTip(tr("Start capturing"));
    m_lstButtons[ButtonId_Stop]->setToolTip(tr("Stop capturing"));
}

void RecordButton::SetCurrentId(ButtonId id)
{
	m_CurrentID = id;
	for (auto btn : m_lstButtons)
		btn->setVisible(false);
	m_lstButtons[id]->setVisible(true);
	RetranslateUi();
	resizeEvent(0);
}

//////////////////////////////////////////////////////////////////////////
CountBackwardCtrl::CountBackwardCtrl(const QString &objname , QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
	m_nCount = 0;
	m_bPause = false;
	setObjectName(objname);
	m_pLbTime = new QLabel(this);
	m_pLbTime->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_pLbTime->setObjectName("RecordPanel_CountBackwardCtrl_TimeLabel");
	connect(&m_timer, &QTimer::timeout, this, [&]{
		if (m_bPause) return;
		m_nCount++;
		Update();
	});
	Reset();
}

void CountBackwardCtrl::Reset()
{
	m_nCount = 0;
	Update();
}

void CountBackwardCtrl::Start()
{
	Reset();
	m_bPause = false;
	m_timer.start(1000);
}

void CountBackwardCtrl::Pause()
{
	m_bPause = true;
}

void CountBackwardCtrl::Resume()
{
	m_bPause = false;
}

void CountBackwardCtrl::Stop()
{
	m_timer.stop();
	Reset();
}

void CountBackwardCtrl::Update()
{
	int sec = m_nCount % 60;
	int min = (m_nCount % (60 * 60)) / 60;
	int h = m_nCount / (60 * 60);
	QString strTime = QString("%1:%2:%3").arg(h, 2,10, QChar('0')).arg(min, 2,10, QChar('0')).arg(sec, 2,10, QChar('0'));
	m_pLbTime->setText(strTime);
}

void CountBackwardCtrl::resizeEvent(QResizeEvent *e)
{
	m_pLbTime->setGeometry(rect());
}

//////////////////////////////////////////////////////////////////////////
RecordLoadingCtrl::RecordLoadingCtrl(QWidget *parent /*= 0*/)
: FSBaseWidget(parent)
,m_containerSize(258, 380)
{
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	m_pLbContainer = new QLabel(this);
	m_pLbContainer->setObjectName("RecordLoadingCtrl_ContainerLabel");
	m_pCountCtrl = new CountCtrl(this);
	m_pCountCtrl->setObjectName("RecordLoadingCtrl_CountCtrl");
	m_pLbTitle = new QLabel(this);
	m_pLbTitle->setObjectName("RecordLoadingCtrl_TitleLabel");
	m_pLbTitle->setAlignment(Qt::AlignCenter);
	m_pLbStopHotkey = new QLabel(this);
	m_pLbStopHotkey->setObjectName("RecordLoadingCtrl_StopHotkeyLabel");
	m_pLbStopHotkey->setAlignment(Qt::AlignCenter);
	m_pLbPauseHotkey = new QLabel(this);
	m_pLbPauseHotkey->setObjectName("RecordLoadingCtrl_PauseHotkeyLabel");
	m_pLbPauseHotkey->setAlignment(Qt::AlignCenter);
	m_pLbStopTip = new QLabel(this);
	m_pLbStopTip->setObjectName("RecordLoadingCtrl_StopTipLabel");
	m_pLbPauseTip = new QLabel(this);
	m_pLbPauseTip->setObjectName("RecordLoadingCtrl_PauseTipLabel");
	m_pBtnCancel = new QPushButton(this);
	m_pBtnCancel->setObjectName("RecordLoadingCtrl_CancelButton");
	
	connect(m_pCountCtrl, &CountCtrl::sigFinish, this, [&]{
		SettingsDataManager::GetInstance()->GetAdvancedData()->disconnect(this);
		hide();
		emit sigLoadingFinish();
	});
	connect(m_pBtnCancel, &QPushButton::clicked, this, [&]{
		m_pCountCtrl->Abort();
		emit sigLoadingAbort();
	});

	//setGeometry(DesktopMgr::GetInstance()->MainScreen()->rect());

    m_recordPanelMark = new RecordLoadingCtrlRecordPanelMask(parent);
    m_recordPanelMark->hide();
}

void RecordLoadingCtrl::Start()
{
	//connect(SettingsDataManager::GetInstance()->GetAdvancedData(), &AdvancedData::signalStartStop, this, [&]{
	//	SettingsDataManager::GetInstance()->GetAdvancedData()->disconnect(this);
	//	m_pCountCtrl->Abort();
	//	hide();
	//	emit sigLoadingAbort();
	//});

    QRect recordRect = PrimaryScreenManager::GetInstance()->PrimaryScreen();
    QRect tempRect = PrimaryScreenManager::GetInstance()->PrimaryScreen();
    if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
        tempRect = SettingsDataManager::GetInstance()->GetDesktopData()->GetCropRect();

    m_recordRect.setWidth(m_containerSize.width());
    m_recordRect.setHeight(m_containerSize.height());
    
    m_recordRect.moveTo(qMax(0, tempRect.x() + (tempRect.width() - m_recordRect.width()) / 2), qMax(0, tempRect.y() + (tempRect.height() - m_recordRect.height()) / 2));
    
    // 不要露出屏幕
    m_recordRect.moveTo(qBound(0, m_recordRect.left(), tempRect.right() - m_recordRect.width()), qBound(0, m_recordRect.top(), tempRect.bottom() - m_recordRect.height()));
    

    setGeometry(recordRect);
    resizeEvent(nullptr);
    QApplication::processEvents();
    show();
    QApplication::processEvents();

    raise();
    activateWindow();

	m_pCountCtrl->SetCount(3);
	m_pCountCtrl->Start();
}

void RecordLoadingCtrl::RetranslateUi()
{
    QString srcText = QString(tr("Get ready to capture") + "...");
    QString strText = StringOperation::GetElidedString(m_pLbTitle->font(), m_pLbTitle->width(), srcText);
    m_pLbTitle->setText(strText);
    m_pLbTitle->setToolTip(srcText);

    m_pLbStopHotkey->setText(SettingsDataManager::GetInstance()->GetAdvancedData()->GetStartStop());
	m_pLbPauseHotkey->setText(SettingsDataManager::GetInstance()->GetAdvancedData()->GetPauseResume());

    srcText = tr("Start/Stop");
    strText = StringOperation::GetElidedString(m_pLbStopTip->font(), m_pLbStopTip->width(), srcText);
    m_pLbStopTip->setText(strText);
    m_pLbStopTip->setToolTip(srcText);

    srcText = tr("Pause/Resume");
    strText = StringOperation::GetElidedString(m_pLbPauseTip->font(), m_pLbPauseTip->width(), srcText);
    m_pLbPauseTip->setText(strText);
    m_pLbPauseTip->setToolTip(srcText);

	m_pBtnCancel->setText(tr("Cancel"));
}

void RecordLoadingCtrl::showEvent(QShowEvent *e)
{
	RetranslateUi();

    auto panel = RecorderViewsMgr::GetInstance()->GetRecordPanel()->GetRealRecordWidget();
    m_recordPanelMark->setParent(panel);
    m_recordPanelMark->setGeometry(QRect(QPoint(0, 0),panel->size()));
    m_recordPanelMark->setVisible(true);
    m_recordPanelMark->raise();

}

void RecordLoadingCtrl::hideEvent(QHideEvent *e)
{
    m_recordPanelMark->setVisible(false);
    FSBaseWidget::hideEvent(e);
}

void RecordLoadingCtrl::resizeEvent(QResizeEvent *e)
{
	int w = m_containerSize.width();
	int h = m_containerSize.height();
	int l = (width() - w) *0.5;
	int t = (height() - h) * 0.5;
    //m_pLbContainer->setGeometry(l, t, w, h);
    m_pLbContainer->setGeometry(m_recordRect);
	QRect rc = m_pLbContainer->geometry();

	w = 82;
	h = 82;
	l = rc.left() + (rc.width() - w)*0.5;
	t = rc.top() + 48;
	m_pCountCtrl->setGeometry(l, t, w, h);

	w = rc.width();
	h = 20;
	l = rc.left();
	t = m_pCountCtrl->geometry().bottom() + 20;
	m_pLbTitle->setGeometry(l, t, w, h);

	w = 65;
	h = 31;
	l = rc.left() + 46;
	t = m_pLbTitle->geometry().bottom() + 18;
	m_pLbStopHotkey->setGeometry(l, t, w, h);
	
	t = m_pLbStopHotkey->geometry().bottom() + 10;
	m_pLbPauseHotkey->setGeometry(l, t, w, h);

	w = 130;
	h = 31;
	l = m_pLbStopHotkey->geometry().right() + 12;
    t = m_pLbStopHotkey->geometry().top();
	m_pLbStopTip->setGeometry(l, t, w, h);

    t = m_pLbPauseHotkey->geometry().top();
	m_pLbPauseTip->setGeometry(l, t, w, h);

	w = 130;
	h = 45;
	l = rc.left() + (rc.width() - w)*0.5;
	t = m_pLbPauseHotkey->geometry().bottom() + 25;
	m_pBtnCancel->setGeometry(l, t, w, h);
}

void RecordLoadingCtrl::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
    p.fillRect(rect(), QColor(0, 0, 0, 10));
    QRect drawRect = rect();
    if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
    {
        drawRect = SettingsDataManager::GetInstance()->GetDesktopData()->GetCropRect();
    }

    p.fillRect(drawRect, QColor(0, 0, 0, 120));
}

void RecordLoadingCtrl::closeEvent(QCloseEvent *e)
{
    e->ignore();
}

void RecordLoadingCtrl::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        m_pCountCtrl->Abort();
        emit sigLoadingAbort();
    }
}

//////////////////////////////////////////////////////////////////////////
CountCtrl::CountCtrl(QWidget *parent /*= 0*/)
	:FSBaseWidget(parent)
{
	connect(&m_timer, &QTimer::timeout, this, [&]{
		m_nCount3++;
		if (m_nCount3 >= m_nCount2)
		{
			Finish();
			return;
		}
		repaint();
	});
	SetCount(3);
}

void CountCtrl::SetCount(int cnt)
{
	if (cnt <= 0) cnt = 5;
	m_nCount = cnt;
	m_nCount2 = 1.0 * m_nCount * 1000 / m_timerspan;
	m_nCount3 = 0;
}

void CountCtrl::Start()
{
	m_timer.start(m_timerspan);
	update();
}

void CountCtrl::Abort()
{
	m_timer.stop();
}

void CountCtrl::Finish()
{
	m_timer.stop();
	emit sigFinish();
}

void CountCtrl::paintEvent(QPaintEvent *e)
{
	int w = 5;
	QRect rc;
	if (width() == height())
		rc = rect();
	else if (width() > height())
		rc = QRect((width() - height()) * 0.5, 0, height(), height());
	else
		rc = QRect(0, (height() - width()) * 0.5, width(), width());
	rc.adjust(3, 3, -3, -3);

	QPainter p(this);
	p.save();
	p.setRenderHint(QPainter::Antialiasing,true);
	QPen pen;
	pen.setWidth(w);
	pen.setColor(Qt::white);
	p.setPen(pen);
	p.drawArc(rc,0,16 * 360);

	int cnt = m_nCount - (m_timerspan * m_nCount3 / 1000);
	QFont fnt;
	fnt.setBold(true);
	fnt.setPixelSize(30);
	p.setFont(fnt);
	QTextOption textop;
	textop.setAlignment(Qt::AlignCenter);
	p.drawText(rc, QString("%1").arg(cnt), textop);

	QPen pen2;
	pen2.setWidth(w);
	pen2.setColor(QColor(65, 199, 219));
	p.setPen(pen2);
	int startAngle = 16 * 90;
	int spanAngle = 0 - 16 * 360.0 * m_nCount3 / m_nCount2;
	//qDebug() << "m_nCount3: " << m_nCount3 << " m_nCount2: " << m_nCount2 << " spanAngle:" << spanAngle;
	p.drawArc(rc, startAngle, spanAngle);

	p.restore();
}

//////////////////////////////////////////////////////////////////////////

SysTrayIcon::SysTrayIcon(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_icoNormal = new QIcon(QPixmap(":/FSRecorder/Resources/light/systemtray_icon_normal.png"));
	m_icoRecording = new QIcon(QPixmap(":/FSRecorder/Resources/light/systemtray_icon_recording.png"));
	m_icoPause = new QIcon(QPixmap(":/FSRecorder/Resources/light/systemtray_icon_pause.png"));
	m_pMenu = new QMenu();
    m_pMenu->setFixedWidth(sMenuWidth);
	m_pActOpen = new QAction(m_pMenu);
	m_pActQuit = new QAction(m_pMenu);
	m_pMenu->addAction(m_pActOpen);
	m_pMenu->addAction(m_pActQuit);
	m_pTrayIcon = new QSystemTrayIcon;
	m_pTrayIcon->setContextMenu(m_pMenu);

    m_doublieTimer = new QTimer(this);
    m_doublieTimer->setSingleShot(true);

    static QPoint cursorPoint;
    connect(m_doublieTimer, &QTimer::timeout, this, [&](){
        // 实现单击Tray也弹出Menu,使用QMenu鼠标点击其它地方Menu不会消失，使用GlobalMenu解决
        GlobalMenu menu(0);
        menu.setFixedWidth(sMenuWidth);
        menu.addAction(m_pActOpen);
        menu.addAction(m_pActQuit);
        menu.exec(cursorPoint);
    });

	connect(m_pTrayIcon, &QSystemTrayIcon::activated, this, [&](QSystemTrayIcon::ActivationReason reason){
		if (reason == QSystemTrayIcon::DoubleClick)
		{
            m_doublieTimer->stop();
			emit sigOpen();
		}
		else if (reason == QSystemTrayIcon::Context || reason == QSystemTrayIcon::Trigger)
		{
			m_pActQuit->setEnabled(RecorderCenter::GetInstance()->GetReordState() == RecorderCenter::RecordState_None); 
		}

        if (reason == QSystemTrayIcon::Trigger)
        {
            m_doublieTimer->start(QApplication::doubleClickInterval());
            cursorPoint = QCursor::pos();
        }

	});
	connect(m_pActOpen, &QAction::triggered, this, [&]{
		emit sigOpen();
	});
	connect(m_pActQuit, &QAction::triggered, this, [&]{
		emit sigQuit();
	});
	RetranslateUi();
}

void SysTrayIcon::RetranslateUi()
{
	m_pActOpen->setText(tr("Open recorder"));
	m_pActQuit->setText(tr("Quit recorder"));
}

SysTrayIcon::~SysTrayIcon()
{
	Hide();
}

void SysTrayIcon::SetVisible(bool bVisible)
{
	if (!bVisible) 
	{
		Hide();
		return;
	}
	RecorderCenter::RecordState state = RecorderCenter::GetInstance()->GetReordState();
	switch (state)
	{
	case RecorderCenter::RecordState_None:
		m_pTrayIcon->setIcon(*m_icoNormal);
		break;
	case RecorderCenter::RecordState_Recording:
		m_pTrayIcon->setIcon(*m_icoRecording);
		break;
	case RecorderCenter::RecordState_Pause:
		m_pTrayIcon->setIcon(*m_icoPause);
		break;
	}
	Show();
}	

SysTrayIcon* SysTrayIcon::GetInstance()
{
	static SysTrayIcon ins;
	return &ins;
}

void SysTrayIcon::Show()
{
	RetranslateUi();
	m_pTrayIcon->show();
}

void SysTrayIcon::Hide()
{
	m_pTrayIcon->hide();
}

void SysTrayIcon::IgnoreDoubleClick()
{

}

//////////////////////////////////////////////////////////////////////////
#include <QtConcurrent/QtConcurrent>

StopRecordMediaImportDlg::StopRecordMediaImportDlg(RecordPanel *panel, QWidget *parent)
:QDialog(parent)
, m_panel(panel)
, m_arcAngle(0)
{
    //Q_ASSERT(panel);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QTimer *timerTemp = new QTimer(this);
    connect(timerTemp, &QTimer::timeout, this, [&](){
        m_arcAngle -= 8;
        update();
    });
    timerTemp->start(20);

    QSize size(120, 90);
    resize(size);
}

void StopRecordMediaImportDlg::DoModal()
{
    exec();
}


void StopRecordMediaImportDlg::showEvent(QShowEvent *event)
{
    QtConcurrent::run(this, &StopRecordMediaImportDlg::Run);
}

void StopRecordMediaImportDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //painter.fillRect(rect(), QColor(150, 150, 150, 150));
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(150, 150, 150, 150));
    painter.drawRoundedRect(rect(), 6, 6);
    painter.restore();

    painter.setBrush(Qt::NoBrush);
    QPen pen = painter.pen();
    pen.setWidth(3);
    pen.setColor(QColor(85, 200, 193));
    painter.setPen(pen);
    const int arcwidth = 60;
    QRect arcRect((width()-arcwidth)/2, (height()-arcwidth)/2, arcwidth, arcwidth);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawArc(arcRect, (30 + m_arcAngle)* 16, 180 * 16);
}

void StopRecordMediaImportDlg::Run()
{
    RecorderCenter::GetInstance()->Stop();
    QMetaObject::invokeMethod(this, "NormalStop", Qt::QueuedConnection, Q_ARG(QString, RecorderCenter::GetInstance()->GetOutputFile()));

}


Q_INVOKABLE void StopRecordMediaImportDlg::NormalStop(QString path)
{
    m_panel->sigNormalStop(path);
    done(1);
}

//////////////////////////////////////////////////////////////////////////

#include "inc_MKCollecter/MKCollecter.h"

GlobalMenu::GlobalMenu(QWidget *parent)
:QMenu(parent)
{
    m_bMouseHook = MKCollecter::GetInstance()->GetMouseHook();
    MKCollecter::GetInstance()->SetMouseHook(true);

    connect(MKCollecter::GetInstance(), &MKCollecter::sigRButtonDown, this, [&](QPoint pt){
        TestClose(pt);
    });
    connect(MKCollecter::GetInstance(), &MKCollecter::sigLButtonDown, this, [&](QPoint pt){
        TestClose(pt);
    });
}

GlobalMenu::~GlobalMenu()
{
    if (!m_bMouseHook)
        MKCollecter::GetInstance()->SetMouseHook(false);

}

void GlobalMenu::TestClose(QPoint pt)
{
    if (!rect().contains(mapFromGlobal(pt)))
    {
        qDebug() << "GlobalMenu close";
        close();
    }
}

//////////////////////////////////////////////////////////////////////////
FreeSpaceChecker::FreeSpaceChecker(QObject *parent)
	:QObject(parent)
{
	connect(&m_timer, &QTimer::timeout, this, [&]{
		if (DoCheck())
		{
			StopCheck();
			emit sigSpaceNotEnough();
		}
	});
}

bool FreeSpaceChecker::DoCheck()
{
	auto driver = SettingsDataManager::GetInstance()->GetAdvancedData()->GetRecordFilePath().left(2);
	auto xMB = gGetDiskFreeSpace(driver);
	return xMB < 500;
}

void FreeSpaceChecker::StartCheck()
{
	m_timer.start(5 * 1000);
}

void FreeSpaceChecker::StopCheck()
{
	m_timer.stop();
}

//////////////////////////////////////////////////////////////////////////
RecordLoadingCtrlRecordPanelMask::RecordLoadingCtrlRecordPanelMask(QWidget *parent /*= 0*/)
:FSBaseWidget(parent)
{
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void RecordLoadingCtrlRecordPanelMask::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(0, 0, 0, 1));
}
