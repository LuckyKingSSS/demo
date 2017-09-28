#include "stdafx.h"
#include "Communicator.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_FSCommonlib/GlobalDefine.h"
#include "ProjectFilesMgr.h"
#include "inc_CommonLib/FileOperation.h"
#include "FSRecorder.h"
#include "RecorderViewsMgr.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "RecorderPreview.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_FSCommonlib/MultiLangMgr.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_FSCommonLib/ExternalAPI.h"
#include "RecorderViewsMgr.h"
#include "RecordPanel.h"
#include "ProjectAutoSaveMgr.h"

extern QString GetSaveProjPath();
Communicator::Communicator(QObject *parent)
	: QObject(parent)
{
	m_bSaving = false;
	m_bEditorShowed = false;
	m_pEditorProc = new QProcess(this);
	QString editorExe = "FSEditor.exe";
    m_bProcessStartError = false;
	m_pFSLoadingEditorWin = new FSLoadingEditorWin(0);
#ifdef _DEBUG
	editorExe = "FSEditorD.exe";
#endif 
	m_pEditorProc->setProgram(PathOperation::MakeWorkPath(editorExe));
	connect(m_pEditorProc, &QProcess::readyReadStandardOutput, this, &Communicator::OnRecvMsg);
    connect(m_pEditorProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(OnEditorExit(int, QProcess::ExitStatus)));
    connect(m_pEditorProc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(OnProcessStartError(QProcess::ProcessError)));
}

Communicator* Communicator::GetInstance()
{
	static Communicator ins;
	return &ins;
}

bool Communicator::IsEditorRunning()
{
	return ProjectMgr::GetInstance()->IsEditorProject();
	//return m_pEditorProc->state() == QProcess::Running;
}

bool Communicator::IsEditShowed()
{
	return m_bEditorShowed;
}

bool Communicator::GotoEditor()
{
	QProcess::ProcessState state = m_pEditorProc->state();
	if (state == QProcess::Starting)
	{
		auto bStarted = m_pEditorProc->waitForStarted();
		ProjectMgr::GetInstance()->SetEditorProject(true);
		return bStarted;	
	}
	else if (state == QProcess::NotRunning)
	{
		m_pEditorProc->setArguments(QStringList() << "starteditor");
        m_bProcessStartError = false;
		m_pEditorProc->start();
		auto curProj = ProjectFilesMgr::GetInstance()->GetCurrent();
		QString projName = curProj ? curProj->GetBaseName() : "";
		m_pFSLoadingEditorWin->Show(projName);
        while (!m_bEditorShowed && !m_bProcessStartError)
        {
            QApplication::processEvents();
        }
		ProjectMgr::GetInstance()->SetEditorProject(true);

        if (!m_bProcessStartError)
        {
            //如果Edit显示上传数据
            bool bSwitch = GAThread::getInstance()->getIsStartFromStartUp();
            if (bSwitch)
            {   //只上传从startup界面到editor 的时间性能信息
                int iCost = GAThread::getInstance()->getElapse();
                GAThread::getInstance()->insertEventItem(VideoEditorLaunchFromStartup, "VideoEditorTime", QString::number(iCost));
                GAThread::getInstance()->setIsStartFromStartup(false);//还原开关
            }
            m_pFSLoadingEditorWin->Hide();
            TellIsNewProject(ProjectMgr::GetInstance()->IsNewProject());
            TellRecentOpens();
            TellProjectName(ProjectMgr::GetInstance()->GetProjectName());
			TellContentChanged(ProjectMgr::GetInstance()->IsContentChanged());
        }
        else
        {
            m_pFSLoadingEditorWin->Hide();
            FSMessageBox::informationOk(sFilmoraScreen, tr("Unknown Error detected, please restart the application."), 0);
            emit sigShowRecorder();

        }
	}
	else if (state == QProcess::Running)
	{
		SendMsg(command_GotoEditor);
	}
	return true;
}

void Communicator::HideEditor()
{
	QProcess::ProcessState state = m_pEditorProc->state();
	if (state == QProcess::Running)
	{
		SendMsg(command_HideEditor);
	}
}

void Communicator::TranslteRecordFiles(QStringList files)
{
	QString cmd;
	for (int i = 0; i < files.size(); ++i)
	{
		if (i == files.size() -1)
			cmd += files[i];
		else
			cmd += files[i] + ",";
	}
	SendMsg(command_TranslateFiles + cmd);
}

void Communicator::EditerLoadProjectString(const QString &projstring)
{
	SendMsg(command_LoadProjectString + projstring);
}

std::tuple<QString, QString, QString> Communicator::GetEditorProjectString()
{
	QString res = SendMsgWaitForResponse(command_GetProjectString);
	QString strProjStrs = GetCmdContent(res, response2Recorder_PushProjectString);
	QStringList lst = strProjStrs.split(',');
	Q_ASSERT(lst.size() == 3);
    if (lst.size() != 3) return std::tuple<QString, QString, QString>();
	QString strMatter = FileOperation::ReadFile(lst[0],true);
    QString strTimeline = FileOperation::ReadFile(lst[1], true);
    QString strProjectSettings = FileOperation::ReadFile(lst[2], true);
	QFile::remove(lst[0]);
    QFile::remove(lst[1]);
    QFile::remove(lst[2]);
    return std::tuple<QString, QString, QString>(strMatter, strTimeline, strProjectSettings);
}


QString Communicator::GetEditorThumbnail()
{
	auto res = SendMsgWaitForResponse(command_GetEditorThumbnail);
	auto path = GetCmdContent(res, response2Recorder_ThumbnailPath);
	return path;
}

bool Communicator::IsSaving()
{
    return m_bSaving;
}

void Communicator::TellContentChanged(bool bChanged)
{
	SendMsg(command_TellContentChanged + QString(bChanged ? "1" : "0"));
}

void Communicator::TellRecentOpens()
{
	auto recentInfos = ProjectFilesMgr::GetInstance()->GetRecentInfos();
	QString response;
	for (auto info : recentInfos)
		response += (info->filepath + ",");
	if (!response.isEmpty())
		response = response.left(response.length() - 1);
	SendMsg(command_RecentOpens + response);
}

void Communicator::TellIsNewProject(bool isNewProj)
{
	SendMsg(command_TellIsNewProject + QString(isNewProj ? "1" : "0"));
}

void Communicator::TellProjectName(QString prjName)
{
	SendMsg(command_TellProjectName + prjName);
}

void Communicator::TellSaveProjectFinish()
{
	SendMsg(command_TellSaveProjectFinish);
}

void Communicator::TellRegisterResult(bool bSucc)
{
	QString ssucc = (bSucc ? "1" : "0");
	SendMsg(command_TellRegisterResult + ssucc);
}

int Communicator::ShowMessageBox(QString title, QString text, int buttons)
{
    int ret = -1;
    if (!ProjPreproccessor::GetInstance()->GetLoadFromEditor())
    {
        ret = FSMessageBox::information(title, text, buttons, RecorderViewsMgr::GetInstance()->GetFSRecorder());
    }
    else
    {
        QString ssucc = QString("%1^%2^%3").arg(title).arg(text).arg(buttons);
        auto result = SendMsgWaitForResponse(command_EditorMessagebox + ssucc);
        ret = GetCmdContent(result, response2Recorder_EditorMessagebox).toInt();
    }
    return ret;
}

void Communicator::OnEditorExit(int ec, QProcess::ExitStatus ps)
{
	if (ps == QProcess::CrashExit)
		qApp->quit();
}

void Communicator::OnProcessStartError(QProcess::ProcessError err)
{
    m_bProcessStartError = true;
}

bool Communicator::SendMsg(QString msg)
{
	std::string stdMsg = msg.toStdString() + "|";
	int nSended = m_pEditorProc->write(stdMsg.c_str(), stdMsg.length());
	return nSended = msg.length();
}

QString Communicator::SendMsgWaitForResponse(QString msg)
{
	m_strResponse = "";
	SendMsg(msg);
	while (m_strResponse.isEmpty())
    {
        //qDebug() << "In while loop";
        QApplication::processEvents();
    }
	return m_strResponse;
}

void Communicator::OnRecvMsg()
{
	QByteArray res = m_pEditorProc->readAllStandardOutput();
	QString strRes(res.data());
	
	if (strRes.contains("response2"))
	{
		m_strResponse = res;
	}

	if (strRes.contains(request_GotoRecorder))
	{
		MultiLangMgr::GetInstance()->SetLangLocale();
		emit sigShowRecorder();
	}
	if (strRes.contains(request_SetContentChanged))
	{
		QString content = GetCmdContent(strRes, request_SetContentChanged);
		ProjectMgr::GetInstance()->SetContentChanged(content == "0" ? false : true);
		//ProjectMgr::GetInstance()->SetEditorProject(content == "0" ? false : true);
	}
	if (strRes.contains(request_SaveThumbnailFile))
	{
		QString content = GetCmdContent(strRes, request_SaveThumbnailFile);
		ProjectMgr::GetInstance()->SetThumbnailPath(content);
	}
	if (strRes.contains(request_IsNewProject))
	{
		bool bNewProj = ProjectMgr::GetInstance()->IsNewProject();
		SendMsg(response2Editor_IsNewProject + QString(bNewProj ? "1" : "0"));
	}
	if (strRes.contains(request_SaveProject))
	{
		m_bSaving = true;
		if (ProjectMgr::GetInstance()->IsContentChanged())
		{
			ProjectMgr::GetInstance()->Save();
		}
		m_bSaving = false;
	}
	if (strRes.contains(request_SaveNewProject))
	{
		m_bSaving = true;
		QString content = GetCmdContent(strRes, request_SaveNewProject);
		ProjectMgr::GetInstance()->SaveNew(content);
		m_bSaving = false;
	}
	if (strRes.contains(request_Quit))
	{
        auto timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, [&,timer]{
			if (!m_bSaving)
			{
				timer->stop();
				qApp->quit();
			}
		});
		timer->start(200);
	}
    if (strRes.contains(request_QuitDontSave))
    {
        ProjectAutoSaveMgr::GetInstance()->QuitDontSave();
    }
	if (strRes.contains(request_ClearRecorder))
	{
		//auto timer = new QTimer(this);
		//connect(timer, &QTimer::timeout, this, [&, timer]{
		//	if (!m_bSaving)
		//	{
		//		timer->stop();
				ProjectMgr::GetInstance()->Clear();
				TellContentChanged(false);
				emit sigClearRecorder();
		//	}
		//});
		//timer->start(200);
	}
	if (strRes.contains(request_LoadProject))
	{
		QString projfile = GetCmdContent(strRes, request_LoadProject);
		emit sigLoadProject(projfile);
	}
	if (strRes.contains(request_LoadImportProject))
	{
		QString projfile = GetCmdContent(strRes, request_LoadImportProject);
		emit sigLoadImportProject(projfile);
	}
	if (strRes.contains(request_EditorShowed))
	{
		m_bEditorShowed = true;
	}
	if (res.contains(request_TellRegisterResult))
	{
		QString bSucce = GetCmdContent(res, request_TellRegisterResult);
		emit sigRegisterResult(bSucce == "1" ? true : false);
	}
}


//////////////////////////////////////////////////////////////////////////
ProjPreproccessor::ProjPreproccessor(QObject *parent)
	:QObject(parent)
    , m_bLoadFromEditor(false)
{

}

ProjPreproccessor* ProjPreproccessor::GetInstance()
{
	static ProjPreproccessor  ins;
	return &ins;
}

void ProjPreproccessor::LoadProject(QString projFile, bool loadImport /*= false*/)
{
	CheckFiles(projFile);
	ProjectMgr::GetInstance()->Load(projFile);
	if (ProjectMgr::GetInstance()->IsEditorProject())
	{
		RecorderViewsMgr::GetInstance()->GotoEditor();
		QString content = ProjectMgr::GetInstance()->GetMatterString2File() + "," + ProjectMgr::GetInstance()->GetTimelineString2File() + "," + ProjectMgr::GetInstance()->GetProjectSettingsString2File();
		Communicator::GetInstance()->EditerLoadProjectString(content);
		//projecttoEdit 信息统计
		GAThread::getInstance()->insertEventItem(Start_Up, "ProjecttoEditor", "");	
	}
	else
	{
		if (!loadImport)
		{
			RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_Preview);
			Communicator::GetInstance()->HideEditor();
			//上传GA数据
			GAThread::getInstance()->insertEventItem(Change_Page, "YourClips", "");
			GAThread::getInstance()->insertEventItem(Start_Up, "ProjecttoYourClips", "");	
			if (GAThread::getInstance()->getLoadProjectType())
			{	
				double iCost = GAThread::getInstance()->getElapse();
				GAThread::getInstance()->insertEventItem(YourClipsLaunchFromStartup, "YourClipsTime", QString::number(iCost));
				GAThread::getInstance()->setLoadProjectType(false);
			}	
		}
		else
		{
			auto paths = RecordingPreviewDataManager::GetInstance()->GetAllPaths();
			Communicator::GetInstance()->TranslteRecordFiles(paths);
		}
	}
}

void ProjPreproccessor::LoadProjectFromEditor(QString projFile)
{
	LoadProject(projFile);
	AddProj2Recent(projFile);
}


void ProjPreproccessor::LoadImportProject(QString projFile)
{
	LoadProject(projFile,true);
	AddProj2Recent(projFile);
}

ProjectFileInfo* ProjPreproccessor::AddProj2Recent(QString projFile)
{
	auto allInfos = ProjectFilesMgr::GetInstance()->GetInfos();
	ProjectFileInfo *pInfo = nullptr;
	for (auto info : allInfos)
	{
		if (info->filepath == projFile)
		{

			pInfo = info;
			break;
		}
	}

	if (!pInfo)
	{
		pInfo = new ProjectFileInfo;
		pInfo->filepath = projFile;
		pInfo->imgpath = "";
        pInfo->savetime = pInfo->CurrentTimeString();
		ProjectFilesMgr::GetInstance()->Add(pInfo);
	}
	ProjectFilesMgr::GetInstance()->Add2Recent(pInfo);

    return pInfo;
}


bool ProjPreproccessor::CheckFiles(QString projFile)
{
	if (!QFile::exists(projFile)) 
		return false;
	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, projFile))
		return false;

	auto rootElem = doc.documentElement();
	if (rootElem.isNull()) return false;

	bool bChanged = false;
	QDomElement tmpElem;
	QString path;
	QList<QDomElement> lstRemove;
	auto previewElem = rootElem.firstChildElement("PreviewMessage");
	if (!previewElem.isNull())
	{
		auto previewItem = previewElem.firstChildElement("Item");
		while (!previewItem.isNull())
		{
			tmpElem = previewItem.firstChildElement("Path");
			if (!tmpElem.isNull())
			{
				path = tmpElem.text();
				if (!QFile::exists(path))
				{
					bChanged = true;
					lstRemove.push_back(previewItem);
					//previewElem.removeChild(previewItem);
				}
			}
			previewItem = previewItem.nextSiblingElement();
		}
		for (auto elem : lstRemove)
		{
			previewElem.removeChild(elem);
		}
	}
	lstRemove.clear();
	auto matterElem = rootElem.firstChildElement("MatterMessage");
	if (!matterElem.isNull())
	{
		auto mediaElem = matterElem.firstChildElement("MediaInfo");
		while (!mediaElem.isNull())
		{
			path = mediaElem.attribute("Path");
			if (!QFile::exists(path))
			{
				bChanged = true;
				lstRemove.push_back(mediaElem);
				//matterElem.removeChild(mediaElem);
			}
			mediaElem = mediaElem.nextSiblingElement();
		}
		for (auto elem : lstRemove)
		{
			matterElem.removeChild(elem);
		}
	}

	lstRemove.clear();
	auto timelineElem = rootElem.firstChildElement("TimelineMessage");
	if (!timelineElem.isNull())
	{
		auto rootTimeline = timelineElem.firstChildElement("TimeLine");
		if (!rootTimeline.isNull())
		{
			auto subTimeline = rootTimeline.firstChildElement("TimeLine");
			while (!subTimeline.isNull())
			{
				auto propertiesElem = subTimeline.firstChildElement("Properties");
				if (!propertiesElem.isNull())
				{
					auto propertieElem = propertiesElem.firstChildElement("Property");
					while (!propertieElem.isNull())
					{
						if (propertieElem.attribute("key") == "Absolute.FilePath")
						{
							path = propertieElem.attribute("value");
							if (!QFile::exists(path))
							{
								bChanged = true;
								lstRemove.push_back(subTimeline);
								//rootTimeline.removeChild(subTimeline);
								break;
							}
						}
						propertieElem = propertieElem.nextSiblingElement();
					}
				}
				subTimeline = subTimeline.nextSiblingElement();
			}
		}
		for (auto elem : lstRemove)
		{
			rootTimeline.removeChild(elem);
		}
	}

	if (bChanged)
	{
        Communicator::GetInstance()->ShowMessageBox(sFilmoraScreen, tr("Some file(s) in your project have been moved or missing."), FSMessageBox::OK);
		XmlOperation::SaveXmlContent2File(doc, projFile);
	}
		
	return true;
}

ProjPreproccessor::QuestionSavingRes ProjPreproccessor::DoQuestionSaving()
{
	QuestionSavingRes res = QuestionSavingRes_UserCancel;
	if (ProjectMgr::GetInstance()->IsContentChanged())
	{
		int ret = FSMessageBox::questionYesNoCancel(tr(sFilmoraScreen),
			QObject::tr("The project has been modified,Do you want to save the changes?"),
			0);

		if (ret == FSMessageBox::Yes)
		{
			if (ProjectMgr::GetInstance()->IsNewProject())
			{
				QString newpath = QFileDialog::getSaveFileName(0,
					tr("Please select directory"),
					GetSaveProjPath(),
					"Project Fils(*.wfsproj)");
				if (!newpath.isEmpty())
				{
					ProjectMgr::GetInstance()->SaveNew(newpath);
					res = QuestionSavingRes_Saved;
				}
				else
				{
					res = QuestionSavingRes_UserNoSavePath;
				}
			}
			else
			{
				ProjectMgr::GetInstance()->Save();
				res = QuestionSavingRes_Saved;
			}
		}
		if (ret == FSMessageBox::No)
		{
			res = QuestionSavingRes_UserRefuse;
            ProjectAutoSaveMgr::GetInstance()->QuitDontSave();
		}
		if (ret == FSMessageBox::Cancel)
		{
			res = QuestionSavingRes_UserNoSavePath;
		}
	}
	else
		res = QuestionSavingRes_Saved;

	return res;
}

void ProjPreproccessor::SetLoadFromEditor(bool b)
{
    m_bLoadFromEditor = b;
}

bool ProjPreproccessor::GetLoadFromEditor()
{
    return m_bLoadFromEditor;
}

//////////////////////////////////////////////////////////////////////////
FSLoadingEditorWin::FSLoadingEditorWin(QWidget *parent)
	:MediaImportDlg(QStringList(""),parent)
{
	m_titlebar->SetButtons(TopLevelWindowTitleBar::None);
	m_pLbProgress->hide();
	m_pBtnCancel->hide();
	SetRange(0, 0);
	RetranslateUi();
}

void FSLoadingEditorWin::Show(QString projName)
{
	if (projName.isEmpty())
	{
		m_pLbName->setText(tr("Loading Editor..."));
	}
	else
	{
		m_pLbName->setText(tr("Loading Project") + QString(" %1.wfsproj ...").arg(projName));
	}
	show();
}

void FSLoadingEditorWin::Hide()
{
	hide();
}
