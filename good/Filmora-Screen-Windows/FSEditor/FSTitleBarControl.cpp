#include "FSTitleBarControl.h"
#include "FSMainTitleBar.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "inc_FSTimelineView/fstimelineview.h"
#include "inc_CommonLib/FileOperation.h"
#include "CommunicatorClient.h"
#include "FSMainTitleBar.h"
#include "FSEditor.h"
#include "inc_UpdateRegister/RegisterMain.h"
#include "inc_UpdateRegister/UpdateMain.h"
#include "AboutDialog.h"
#include "inc_FSCommonlib/CBSUrlReader.h"
#include "SettingDataMgr.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/StringOperation.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "SettingDialog.h"
#include "ProjectSettingDialog.h"

#include <QTimer>
extern FSEditor* g_FSEditor;
FSTitleBarControl::FSTitleBarControl(FSMainTitleBar *titleBar, FSMatterView *matterview, IFSEditMediaPlayer *mediaPlayerView, FSTimelineView *timelineview, QObject *parent)
	: QObject(parent)
{
	m_bFocusInTimeline = false;
	UpdateRegisterInit::SetConfigFileDir(qApp->applicationDirPath());
	UpdateInit::SetUpdateUrl("http://cbs.wondershare.com/go.php?m=upgrade_info&pid=&version=&email=");

	m_pRegisterMain = nullptr;
	m_pTitleBar = titleBar;
	m_pMatterView = matterview;
	m_pMediaplayView = mediaPlayerView;
	m_pTimelineView = timelineview;
	//m_pRegisterMain = new RegisterMain(0);
	//m_pRegisterMain->hide();
    m_pUpdateMain = new UpdateMain();
	m_pUpdateMain->hide();
	m_pCBSUrlReader = new CBSUrlReader(this);

	RegisterInit::SetBuyNowUrl(m_pCBSUrlReader->ReadCustomerUrl(SettingDataMgr::GetInstance()->GetLangInfo().simplename1, "OrderPage"));
	RebuildRegisterMain();

	//connect(m_pRegisterMain, &RegisterMain::RegisterResult, this, &FSTitleBarControl::OnRegistered);
	connect(m_pTitleBar, &FSMainTitleBar::sigActionTriggled, this, &FSTitleBarControl::OnActionTriggled);
	connect(m_pTitleBar, &FSMainTitleBar::sigRegister, this, &FSTitleBarControl::OnRegister);
	connect(m_pTitleBar, &FSMainTitleBar::sigPuschase, this, &FSTitleBarControl::OnPuschase);
	connect(m_pTitleBar->GetMenuEdit(), &QMenu::aboutToShow, this, [&]{OnMenuEditAbout2Show(true);});
	connect(m_pTitleBar->GetMenuFile(), &QMenu::aboutToShow, this, &FSTitleBarControl::OnMenuFileAbout2Show);
	connect(m_pTitleBar->GetMenuRecentOpen(), &QMenu::aboutToShow, this, &FSTitleBarControl::OnMenuRecentOpenAbout2Show);
	connect(m_pTitleBar->GetMenuExport(), &QMenu::aboutToShow, this, &FSTitleBarControl::OnMenuExportAbout2Show);
	connect(m_pUpdateMain, &UpdateMain::BeginInstall, this, &FSTitleBarControl::OnBeginInstall);
	connect(m_pUpdateMain, &UpdateMain::InstallStarted, this, &FSTitleBarControl::OnInstallStarted);
	
	connect(matterview->GetMatterViewManager(), &IFSMatterViewManager::sigMediaSelectChanged, this, [&]{OnMenuEditAbout2Show(); });
	connect(timelineview->GetPlayerEditManager(), &IPlayerEditorManager::signalTimelineUpdate, this, [&]{OnMenuEditAbout2Show(); });
	connect(ProjPreproccessorClient::GetInstance(), &ProjPreproccessorClient::sigContentChanged, this, &FSTitleBarControl::OnMenuFileAbout2Show);
	connect(ProjPreproccessorClient::GetInstance(), &ProjPreproccessorClient::sigIsNewProjectChanged, this, &FSTitleBarControl::OnMenuFileAbout2Show);
	connect(matterview->GetMatterViewManager(), &IFSMatterViewManager::sigFocusIn, this, [&]{
		m_bFocusInTimeline = false;
	});
	connect(m_pTimelineView->GetTimelineViewManager(), &ITimelineViewManager::sigFocusIn, this, [&]{ 
		m_bFocusInTimeline = true;
	});
	connect(m_pMediaplayView, &IFSEditMediaPlayer::sigEnableEditShotCut, this, [&](bool bEnable){
		m_bFocusInTimeline = true; 
		OnMenuEditAbout2Show();
	});
	//
	//connect(m_pRegisterMain,&RegisterMain::singalStartRegister, this, [&](const QDateTime time){
	//	GAThread::getInstance()->setStartRegisterTime(time);
	//});
	//connect(m_pRegisterMain, &RegisterMain::signalCheckOnRegist, this, [&](bool bResult, QDateTime time){
	//	QDateTime iStatrtTime = GAThread::getInstance()->getStartRegisterTime();
	//	int iCost = iStatrtTime.secsTo(time);
	//	GAThread::getInstance()->insertEventItem(RegisterTimer, "RegsterFinished", QString::number(iCost*1000));
	//	if (bResult)
	//	{
 //          //获得安装的时间
	//		QDateTime installTime = GAThread::getInstance()->getAppInstallTime();
	//		QDateTime CurTime = QDateTime::currentDateTime();
	//		int iTotalCost = installTime.daysTo(CurTime);
	//		GAThread::getInstance()->insertEventItem(RegisterTimer, "RegisterDate", QString::number((iTotalCost + 1)*1000));
	//	}
	//    //上传注册接口耗时
	//	QDateTime CurTime = QDateTime::currentDateTime();
	//	int iCostCount = time.secsTo(CurTime);
	//	GAThread::getInstance()->insertEventItem(RegisterTimer, "RegsterFinished", QString::number(iCostCount*1000));
	//});
	connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigRegisterResult, this, [&](bool bSucc){
		m_pTitleBar->SetRegistered(bSucc);
		RebuildRegisterMain();
	});
	OnMenuFileAbout2Show();
	OnMenuEditAbout2Show();
	m_pTitleBar->SetRegistered(m_pRegisterMain->CheckRegister());
}

FSTitleBarControl::~FSTitleBarControl()
{

}

void FSTitleBarControl::SetEditorView(FSEditor *view)
{
	m_pEditorView = view;
}

void FSTitleBarControl::LoadProjFromMenu(const QString &projfile)
{
	if (!QFile::exists(projfile)) return;
	if (IsRecorderProj(projfile))
	{
		//从Editor打开的recorder工程
		FSMessageBox dlg(tr("Filmora Srcn"),
			tr("Will you open this project and import all recorded clips to the editor?") + "\r\n\r\n" +
			tr("Press \"Import\": Open the project in editor and import all clips.") + "\r\n" +
			tr("Press \"Display\": Open the project in YourClips window and display clip list."),
			Qt::AlignCenter,
			FSMessageBox::Yes | FSMessageBox::No | FSMessageBox::Cancel,
			0);
		dlg.SetIconObjectName("MessageBox_Icon_Information");
		dlg.SetButtonText(FSMessageBox::Yes, tr("Import"));
		dlg.SetButtonText(FSMessageBox::No, tr("Display"));
		dlg.SetButtonText(FSMessageBox::Cancel, tr("Cancel"));
		FSMessageBox::Button btn = (FSMessageBox::Button)dlg.DoModal();

		if (btn == FSMessageBox::Yes)
		{
			ProjPreproccessorClient::GetInstance()->ImportLoad(projfile);
		}
		else if (btn == FSMessageBox::No)
		{
			ProjPreproccessorClient::GetInstance()->Load(projfile);
		}
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Project_Management, "OpenProject", "RecorderType");
	}
	else
	{
		ProjPreproccessorClient::GetInstance()->Load(projfile);
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Project_Management, "OpenProject", "EditorType");
	}
}

void FSTitleBarControl::OnRegistered(bool bSucc)
{
	if (bSucc)
		CommunicatorClient::GetInstance()->TellRegisterResult(bSucc);
	m_pTitleBar->SetRegistered(bSucc);
}

bool FSTitleBarControl::IsRecorderProj(const QString &projfile)
{
	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, projfile))
		return false;
	auto rootElem = doc.documentElement();
	if (rootElem.isNull()) return false;
	auto iseditElem = rootElem.firstChildElement("IsEditor");
	if (iseditElem.isNull()) return false;
	return iseditElem.text() == "0" ? true : false;
}

void FSTitleBarControl::OnActionTriggled(int id, bool checked)
{
	FSMainTitleBar::ActionID aid = (FSMainTitleBar::ActionID)id;
	ProjPreproccessorClient::QuestionSavingRes res;
	switch (aid)
	{
	case FSMainTitleBar::ActionID_File_NewProject:
		res = ProjPreproccessorClient::GetInstance()->DoQuestionSaving();
		if (res == ProjPreproccessorClient::QuestionSavingRes_Saved ||
			res == ProjPreproccessorClient::QuestionSavingRes_UserRefuse)
		{
			ProjPreproccessorClient::GetInstance()->Clear();
		}
		break;
	case FSMainTitleBar::ActionID_File_OpenProject:
		res = ProjPreproccessorClient::GetInstance()->DoQuestionSaving();
		if (res == ProjPreproccessorClient::QuestionSavingRes_Saved ||
			res == ProjPreproccessorClient::QuestionSavingRes_UserRefuse)
		{
			QString strOpenProj = ProjPreproccessorClient::GetInstance()->GetOpenProjFile();
			if (strOpenProj.isEmpty()) return;
			LoadProjFromMenu(strOpenProj);
		}
		break;
	case FSMainTitleBar::ActionID_File_SaveProject:
		ProjPreproccessorClient::GetInstance()->SaveAtMenu();
		break;
	case FSMainTitleBar::ActionID_File_SaveProjectAs:
		ProjPreproccessorClient::GetInstance()->SaveNewAtMenu();
		break;
	case FSMainTitleBar::ActionID_File_AchiveProject:
		break;
	case FSMainTitleBar::ActionID_File_ImportMediaFiles:
		m_pMatterView->GetMatterViewManager()->ImportMediaFiles();
		break;
	case FSMainTitleBar::ActionID_File_ImportMediaFolder:
		m_pMatterView->GetMatterViewManager()->ImportMediaFolder();
		break;
    case FSMainTitleBar::ActionID_File_ProjectSettings:
    {
        ProjectSettingDialog dlg(m_pEditorView);
        dlg.DoModal();
        break;
    }
	case FSMainTitleBar::ActionID_File_Settings:
	{
		SettingDialog dlg(m_pEditorView);
		dlg.DoModal();
		break;
	}
	case FSMainTitleBar::ActionID_File_Close:
		emit sigClose();
		break;
	case FSMainTitleBar::ActionID_Edit_Undo:
		m_pTimelineView->GetTitleBarManager()->Undo();
		break;
	case FSMainTitleBar::ActionID_Edit_Redo:
		m_pTimelineView->GetTitleBarManager()->Redo();
		break;
	case FSMainTitleBar::ActionID_Edit_Cut:
		m_pTimelineView->GetTitleBarManager()->Cut();
		break;
	case FSMainTitleBar::ActionID_Edit_Copy:
		m_pTimelineView->GetTitleBarManager()->Copy();
		break;
	case FSMainTitleBar::ActionID_Edit_Paste:
		m_pTimelineView->GetTitleBarManager()->Paste();
		break;
	case FSMainTitleBar::ActionID_Edit_Delete:

		if (m_bOnlyDeleteTimeline)
		{
			if (m_pTimelineView->GetTitleBarManager()->CanDelete())
				m_pTimelineView->GetTitleBarManager()->Delete();
		}
		else
		{
			if (m_bFocusInTimeline)
			{
				if (m_pTimelineView->GetTitleBarManager()->CanDelete())
					m_pTimelineView->GetTitleBarManager()->Delete();
			}
			else
			{
				if (m_pMatterView->GetMatterViewManager()->CanDelete())
					m_pMatterView->GetMatterViewManager()->Delete();
			}
		}
		
		break;
    case FSMainTitleBar::ActionID_Edit_SnapToGuides:
        SettingDataMgr::GetInstance()->SetSnapToGuides(checked);
        break;
	case FSMainTitleBar::ActionID_Export_CreateVideo:
		emit sigCreateVideo();
		break;
	case FSMainTitleBar::ActionID_Help_OnlineHelp:
	{
		QString res = m_pCBSUrlReader->ReadCustomerUrl(SettingDataMgr::GetInstance()->GetLangInfo().simplename1, "HelpOnlinePage");
		QDesktopServices::openUrl(QUrl(res));
	}
		break;
	case FSMainTitleBar::ActionID_Help_ProductFAQ:
	{
		QString res = m_pCBSUrlReader->ReadCustomerUrl(SettingDataMgr::GetInstance()->GetLangInfo().simplename1, "ProductFAQPage");
		QDesktopServices::openUrl(QUrl(res));
	}
		break;
	case FSMainTitleBar::ActionID_Help_SupportCenter:
	{
		QString res = m_pCBSUrlReader->ReadCustomerUrl(SettingDataMgr::GetInstance()->GetLangInfo().simplename1, "SupportPage");
		QDesktopServices::openUrl(QUrl(res));
	}
		break;
	case FSMainTitleBar::ActionID_Help_NewFeatureRequest:
	{
		QString res = m_pCBSUrlReader->ReadCustomerUrl(SettingDataMgr::GetInstance()->GetLangInfo().simplename1, "NewFeatureRequest");
		QDesktopServices::openUrl(QUrl(res));
	}
		break;
	case FSMainTitleBar::ActionID_Help_ProductPage:
	{
		QString res = m_pCBSUrlReader->ReadCustomerUrl(SettingDataMgr::GetInstance()->GetLangInfo().simplename1, "ProductPage");
		QDesktopServices::openUrl(QUrl(res));
	}
		break;
	case FSMainTitleBar::ActionID_Help_CheckForUpdate:
		OnCheckUpdate();
		break;
	case FSMainTitleBar::ActionID_Help_About:
	{
		AboutDialog dlg(g_FSEditor);
		dlg.setStyleSheet(gGetStyleSheet(":/FSEditor/Resources/styles_light.css"));
		dlg.DoModal();
	}	break;
	}
}

void FSTitleBarControl::OnMenuEditAbout2Show(bool bOnlyTimeline/* = false*/)
{
	m_bOnlyDeleteTimeline = bOnlyTimeline;
	QTimer::singleShot(0, [&, bOnlyTimeline]{
		auto titleBarMgr = m_pTimelineView->GetTitleBarManager();
		m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Edit_Undo, titleBarMgr->CanUndo());
		m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Edit_Redo, titleBarMgr->CanRedo());
		m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Edit_Cut, titleBarMgr->CanCut());

		bool bCanCopy = titleBarMgr->CanCopy();
		m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Edit_Copy, bCanCopy);
		bool bCanPaste = titleBarMgr->CanPaste();
		m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Edit_Paste, bCanPaste);

		if (bOnlyTimeline)
		{
			m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Edit_Delete, titleBarMgr->CanDelete());
		}
		else
		{
			m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Edit_Delete, titleBarMgr->CanDelete() || m_pMatterView->GetMatterViewManager()->CanDelete());
		}
	});
}

void FSTitleBarControl::OnMenuFileAbout2Show()
{
	auto bChange = ProjPreproccessorClient::GetInstance()->IsContentChanged();
	auto titleBarMgr = m_pTimelineView->GetTitleBarManager();
	m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_File_SaveProject, bChange);

	bool bSaveAsEnable = !(ProjPreproccessorClient::GetInstance()->IsNewProject() && !bChange);
	m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_File_SaveProjectAs, bSaveAsEnable);

}

void FSTitleBarControl::OnMenuRecentOpenAbout2Show()
{
	QStringList lst = ProjPreproccessorClient::GetInstance()->GetRecentOpens();

	auto menu = m_pTitleBar->GetMenuRecentOpen();
	menu->clear();
	int menuWidth = 0;

	if (lst.isEmpty())
	{
		auto act = menu->addAction("(" + tr("Empty") + ")");
		act->setEnabled(false);
		menuWidth = 50;
	}
	else
	{
		for (auto file : lst)
		{
			menuWidth = qMax(menuWidth, StringOperation::GetTextTrueLenght(m_pTitleBar->GetMenuFile()->font(), file));
			auto act = menu->addAction(file);
			connect(act, &QAction::triggered, this, [&, file]{

				auto isExsit = FileOperation::IsFileExist(file);
				if (!isExsit)
				{
					FSMessageBox::informationOk(tr("FimoraScrn"), tr("The project file %1 can not be found").arg(file) + "." , m_pEditorView);
					return;
				}

				ProjPreproccessorClient::QuestionSavingRes  res = ProjPreproccessorClient::GetInstance()->DoQuestionSaving();
				if (res == ProjPreproccessorClient::QuestionSavingRes_Saved ||
					res == ProjPreproccessorClient::QuestionSavingRes_UserRefuse)
				{
                    LoadProjFromMenu(file);
				}
			});
		}
	}
	m_pTitleBar->GetMenuRecentOpen()->setFixedWidth(menuWidth + 120);
}

void FSTitleBarControl::OnMenuExportAbout2Show()
{
	m_pTitleBar->SetActionEnabled(FSMainTitleBar::ActionID_Export_CreateVideo, g_FSEditor->CanExport());
}

void FSTitleBarControl::OnRegister()
{ 
	m_pRegisterMain->show();
}

void FSTitleBarControl::OnPuschase()
{
	auto url = m_pCBSUrlReader->ReadCustomerUrl(SettingDataMgr::GetInstance()->GetLangInfo().simplename1, "OrderPage");
	QDesktopServices::openUrl(QUrl(url));
}

void FSTitleBarControl::OnCheckUpdate()
{
	m_pUpdateMain->LiveUpdate();
}

void FSTitleBarControl::OnBeginInstall()
{
	//做保存工作
	ProjPreproccessorClient::GetInstance()->DoQuestionSaving();
	while (CommunicatorClient::GetInstance()->IsSaving())
		QApplication::processEvents();
	m_pUpdateMain->Install();
}

void FSTitleBarControl::OnInstallStarted()
{
	//关闭程序
	CommunicatorClient::GetInstance()->Quit();
}

void FSTitleBarControl::RebuildRegisterMain(bool bTellRecorder /*= false*/)
{
	if (m_pRegisterMain)
	{
		delete m_pRegisterMain;
		m_pRegisterMain = nullptr;
	}

    m_pRegisterMain = new RegisterMain();
	m_pRegisterMain->hide();

	connect(m_pRegisterMain, &RegisterMain::signalCheckOnRegist, this, &FSTitleBarControl::OnRegistered);

	connect(m_pRegisterMain, &RegisterMain::singalStartRegister, this, [&](const QDateTime time){
		GAThread::getInstance()->setStartRegisterTime(time);
	});
	connect(m_pRegisterMain, &RegisterMain::signalCheckOnRegist, this, [&](bool bResult, QDateTime time){
		QDateTime iStatrtTime = GAThread::getInstance()->getStartRegisterTime();
		int iCost = iStatrtTime.secsTo(time);
		GAThread::getInstance()->insertEventItem(RegisterTimer, "RegsterFinished", QString::number(iCost * 1000));
		if (bResult)
		{
			//获得安装的时间
			QDateTime installTime = GAThread::getInstance()->getAppInstallTime();
			QDateTime CurTime = QDateTime::currentDateTime();
			int iTotalCost = installTime.daysTo(CurTime);
			GAThread::getInstance()->insertEventItem(RegisterTimer, "RegisterDate", QString::number((iTotalCost + 1) * 1000));
		}
		//上传注册接口耗时
		QDateTime CurTime = QDateTime::currentDateTime();
		int iCostCount = time.secsTo(CurTime);
		GAThread::getInstance()->insertEventItem(RegisterTimer, "RegsterFinished", QString::number(iCostCount * 1000));
	});
	//QTimer::singleShot(0, [&]{m_pEditorView->RetranslateUi(); });
	if (bTellRecorder) OnRegistered(m_pRegisterMain->CheckRegister());
}
