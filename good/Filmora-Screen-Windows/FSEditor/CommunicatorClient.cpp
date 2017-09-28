#include "stdafx.h"
#include "CommunicatorClient.h"
#include <QSocketNotifier>
#include "inc_FSCommonlib/GlobalDefine.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "FSEditor.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "inc_FSTimelineView/fstimelineview.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "SettingDataMgr.h"
#include <QThread>
#include <QMessageBox>
#include <QApplication>
#include "inc_FSCommonlib/ProjectSavingMgr.h"

#include "io.h"

extern FSEditor* g_FSEditor;
CommunicatorClient::CommunicatorClient(QObject *parent)
	: QObject(parent)
{
	m_pEditorView = nullptr;
	m_bSaving = false;
	m_Filein.open(stdin, QIODevice::ReadOnly);
	auto thrd = new QThread(this);
	moveToThread(thrd);
	thrd->start();
	QMetaObject::invokeMethod(this, "_OnReceiveMsg", Qt::QueuedConnection);
}
CommunicatorClient* CommunicatorClient::GetInstance()
{
	static CommunicatorClient ins;
	return &ins;
}

void CommunicatorClient::SetEditorView(QWidget *pEditorView)
{
	m_pEditorView = pEditorView;
}

void CommunicatorClient::GotoRecorder()
{
	SendMsg(request_GotoRecorder);
}

void CommunicatorClient::SetContentChanged(bool bChanged)
{
    // 修改内容改变时才进行通信
    bool bStateChanged = (bChanged != ProjPreproccessorClient::GetInstance()->IsContentChanged());
    if (bStateChanged)
    {
        ProjPreproccessorClient::GetInstance()->SetContentChanged(bChanged);
        SendMsg(request_SetContentChanged + QString(bChanged ? "1" : "0"));
    }

}

//bool CommunicatorClient::IsContentChanged()
//{
//	QString res = SendMsgWaitForResponse(request_IsContentChanged);
//	QString content = GetCmdContent(res, response2Editor_IsContentChanged);
//	return content == "0" ? false : true;
//}

bool CommunicatorClient::IsNewProj()
{
	QString res = SendMsgWaitForResponse(request_IsNewProject);
	QString content = GetCmdContent(res, response2Editor_IsNewProject);
	return content == "0" ? false : true;
}

void CommunicatorClient::Save()
{
    ProjectSavingMgr::GetInstance()->WaitFinishSave();
    ProjectSavingMgr::GetInstance()->SetSaving(true);
	m_bSaving = true;
	SaveThumbnailFile();
	SendMsg(request_SaveProject, false); 
	while (m_bSaving)
    {
        //qDebug() << "In while loop";
        ::Sleep(20);
        //QApplication::processEvents();
    }

    ProjectSavingMgr::GetInstance()->SetSaving(false);

}

void CommunicatorClient::SaveNew(QString newProjfile)
{
    ProjectSavingMgr::GetInstance()->WaitFinishSave();
    ProjectSavingMgr::GetInstance()->SetSaving(true);
	m_bSaving = true;
	SaveThumbnailFile();
	SendMsg(request_SaveNewProject + newProjfile, false); 
    while (m_bSaving)
    {
        ::Sleep(20);
        //qDebug() << "In while loop";
        //QApplication::processEvents();
    }
    ProjectSavingMgr::GetInstance()->SetSaving(false);

}

void CommunicatorClient::QuitDontSave()
{
    SendMsg(QString(request_QuitDontSave));

}

void CommunicatorClient::Quit()
{
    SendMsg(request_Quit);
}

void CommunicatorClient::ClearRecorder()
{
	SendMsg(request_ClearRecorder);
}

void CommunicatorClient::LoadProject(QString projFile)
{
	SendMsg(request_LoadProject + projFile);
}

void CommunicatorClient::LoadImportPorject(QString projFile)
{
	SendMsg(request_LoadImportProject + projFile);
}

void CommunicatorClient::SaveThumbnailFile()
{
	if (!m_pEditorView)
		return;
	QPixmap px = ((FSEditor*)m_pEditorView)->GetUpperView()->GetMatterView()->GetMatterViewManager()->Get1stItemPixmap();
	auto path = FSGetProjectFileThumbnailPath() + QString("/%1.png").arg(::GetTickCount());
	px.save(path);
	SendMsg(request_SaveThumbnailFile + path, false);
}

bool CommunicatorClient::IsSaving()
{
	return m_bSaving;
}

void CommunicatorClient::EditorShowed()
{
	SendMsg(request_EditorShowed);
}

void CommunicatorClient::TellRegisterResult(bool bSucc)
{
	QString ssucc = (bSucc ? "1" : "0");
	SendMsg(request_TellRegisterResult + ssucc);
}

void CommunicatorClient::Test()
{
	auto timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [&,timer]{
		int a = 0;
		qDebug() << "Test Timer running";
	});
	timer->start(100);
	qDebug() << "Test Timer Start";
}

//QStringList CommunicatorClient::GetRecentOpens()
//{
//	QString res = SendMsgWaitForResponse(request_GetRecentOpens);
//	QString content = GetCmdContent(res, response2Editor_GetRecentOpens);
//
//	QStringList lst;
//	if (!content.isEmpty())
//	{
//		lst = content.split(",");
//	}
//	return lst;
//}

bool CommunicatorClient::SendMsg(const QString msg, bool waitFinishSave )
{
    if (waitFinishSave)
    {
        ProjectSavingMgr::GetInstance()->WaitFinishSave();
    }

	QString smd = msg + "|";

	QFile fileout;
	fileout.open(stdout, QIODevice::WriteOnly);
	
	auto utfStr = smd.toUtf8();
	int nSended = fileout.write(utfStr.constData(),utfStr.size());
	fileout.close();
	return nSended == utfStr.size();
}

void CommunicatorClient::_OnReceiveMsg()
{
	while (true)
	{
		char buffer[4096] = { 0 };
		int nRead = read(m_Filein.handle(), buffer, 4096);
		QString res = QString(buffer).left(nRead);
		if (res.contains("response2"))
		{
			m_strResponse = res;
		}

		if (res.contains(command_GotoEditor))
		{
			emit sigShowEditor();
		}
		if (res.contains(command_HideEditor))
		{
			emit sigHideEditor();
		}
		if (res.contains(command_TranslateFiles))
		{
			QString strFiles = GetCmdContent(res,command_TranslateFiles);
			QStringList lst = strFiles.split(',');
			emit sigImportFiles(lst);
		}
		if (res.contains(command_LoadProjectString))
		{
			QString strProjStrs = GetCmdContent(res, command_LoadProjectString);
			QStringList lst = strProjStrs.split(',');
			Q_ASSERT(lst.size() == 3);
            if (lst.size() != 3)
                continue;
			QString strMatter = FileOperation::ReadFile(lst[0],true);
            QString strTimeline = FileOperation::ReadFile(lst[1], true);
            QString strProjectSettings = FileOperation::ReadFile(lst[2], true);
			QFile::remove(lst[0]);
            QFile::remove(lst[1]);
            QFile::remove(lst[2]);
			emit sigMatterStringLoaded(strMatter);
            emit sigTimelineStringLoaded(strTimeline);
            emit sigProjectSettingsLoaded(strProjectSettings);
            emit sigProjectFinishLoaded();
		}
		if (res.contains(command_GetProjectString))
		{
			if (!m_pEditorView)
				return;
			QString strMatter = (qobject_cast<FSEditor*>(m_pEditorView))->GetUpperView()->GetMatterView()->GetMatterViewManager()->GetProjectString();
			if (strMatter.isEmpty()) strMatter = " ";
			QString strTimeline = (qobject_cast<FSEditor*>(m_pEditorView))->GetTimelineView()->GetTimelineViewManager()->GetProjectString();
			if (strTimeline.isEmpty()) strTimeline = " ";
            QString strProjectSettings = SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->GetProjectString();
            if (strProjectSettings.isEmpty()) strProjectSettings = " ";

			QString file1 = PathOperation::GetTempDir() +"/" + QString("%1%2").arg(::GetTickCount()).arg(qrand());
            QString file2 = PathOperation::GetTempDir() + "/" + QString("%1%2").arg(::GetTickCount()).arg(qrand());
            QString file3 = PathOperation::GetTempDir() + "/" + QString("%1%2").arg(::GetTickCount()).arg(qrand());
			FileOperation::WriteFile(file1, strMatter,true);
            FileOperation::WriteFile(file2, strTimeline, true);
            FileOperation::WriteFile(file3, strProjectSettings, true);
			SendMsg(response2Recorder_PushProjectString + file1 + "," + file2 + "," + file3, false);
		}
		if (res.contains(command_GetEditorThumbnail))
		{
			if (!m_pEditorView)
				return;
            auto thumbnail = (qobject_cast<FSEditor*>(m_pEditorView))->GetUpperView()->GetMatterView()->GetMatterViewManager()->Get1stItemPixmap();
			auto path = FSGetProjectFileThumbnailPath() + QString("/%1.png").arg(::GetTickCount());
			thumbnail.save(path);
			SendMsg(response2Recorder_ThumbnailPath + path, false);
		}
		if (res.contains(command_TellContentChanged))
		{
            QString changed = GetCmdContentFromEnd(res, command_TellContentChanged);
			ProjPreproccessorClient::GetInstance()->SetContentChanged(changed == "0" ? false:true);
		}
		if (res.contains(command_TellIsNewProject))
		{
			QString isnew = GetCmdContent(res, command_TellIsNewProject);
			ProjPreproccessorClient::GetInstance()->SetNewProject(isnew == "0" ? false : true);
		}
		if (res.contains(command_RecentOpens))
		{
			QString content = GetCmdContent(res, command_RecentOpens);
			QStringList lst;
			if (!content.isEmpty())
			{
				lst = content.split(",");
			}
			ProjPreproccessorClient::GetInstance()->SetRecentOpens(lst);
		}
		if (res.contains(command_TellProjectName))
		{
            QString prjname = GetCmdContentFromEnd(res, command_TellProjectName);
			emit sigGetProjectName(prjname);
		}
		if (res.contains(command_TellSaveProjectFinish))
		{
			m_bSaving = false;
		}
		if (res.contains(command_TellRegisterResult))
		{
			QString bSucce = GetCmdContent(res, command_TellRegisterResult);
			emit sigRegisterResult(bSucce == "1" ? true : false);
		}
        if (res.contains(command_EditorMessagebox))
        {
            QString content = GetCmdContent(res, command_EditorMessagebox);
            QStringList lst;
            if (!content.isEmpty())
            {
                lst = content.split("^");
                if (lst.size() == 3)
                {
                    QTimer::singleShot(0, m_pEditorView, [&, lst](){
						if (!m_pEditorView)
							return;
                        auto button = FSMessageBox::information(lst[0], lst[1], lst[2].toInt(), m_pEditorView);
                        SendMsg(response2Recorder_EditorMessagebox + QString::number(button));

                    });
                    
                }
            }
        }
		QThread::msleep(1);
	}
}

QString CommunicatorClient::SendMsgWaitForResponse(const QString msg)
{
	m_strResponse = "";
	SendMsg(msg);
	while (m_strResponse.isEmpty())
	{
        // qDebug() << "In while loop";
        ::Sleep(20);
        //QApplication::processEvents();
	}
	return m_strResponse;
}

//////////////////////////////////////////////////////////////////////////
ProjPreproccessorClient::ProjPreproccessorClient(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_bContentChanged = false;
	m_bNewProject = true;
}

ProjPreproccessorClient* ProjPreproccessorClient::GetInstance()
{
	static ProjPreproccessorClient ins;
	return &ins;
}

void ProjPreproccessorClient::SetEditorView(FSEditor *pEditor)
{
	m_pFSEditor = pEditor;
}

void ProjPreproccessorClient::SaveAtClose()
{
	QuestionSavingRes res = DoQuestionSaving();
	if (res == QuestionSavingRes_Saved || res == QuestionSavingRes_UserRefuse)
	{
        CommunicatorClient::GetInstance()->Quit();
	}
}

bool ProjPreproccessorClient::SaveAtMenu()
{
	CommunicatorClient *pCom = CommunicatorClient::GetInstance();
	if (!IsContentChanged()) return true;
	if (pCom->IsNewProj())
	{
        QString newpath = QFileDialog::getSaveFileName(m_pFSEditor, tr("Please select directory"), GetProjectSavePath(), "Project Files(*.wfsproj)");
		if (!newpath.isEmpty())
		{
			pCom->SaveNew(newpath);
			return true;
		}
		else
			return false;
	}
	else
	{
		pCom->Save();
		return true;
	}
	return true;
}

bool ProjPreproccessorClient::SaveNewAtMenu()
{
	CommunicatorClient *pCom = CommunicatorClient::GetInstance();
    QString newpath = QFileDialog::getSaveFileName(m_pFSEditor, tr("Please select directory"), GetProjectSavePath(), "Project Files(*.wfsproj)");
	if (!newpath.isEmpty())
	{
		pCom->SaveNew(newpath);
		return true;
	}
	return false;
}

void ProjPreproccessorClient::Clear()
{
	//auto timer = new QTimer;
	//connect(timer, &QTimer::timeout, this,[&, timer]{
	//	if (!CommunicatorClient::GetInstance()->IsSaving())
	//	{
	//		timer->stop();
			SetContentChanged(false);
			m_pFSEditor->GetUpperView()->GetMatterView()->GetMatterViewManager()->Clear();
			m_pFSEditor->GetTimelineView()->GetTimelineViewManager()->Clear();
			emit m_pFSEditor->GetTimelineView()->GetTimelineViewManager()->sigClear();
            SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->NewProject();
			CommunicatorClient::GetInstance()->ClearRecorder();
	//		delete timer;
	//	}

	//});
	//timer->start(100);
}

void ProjPreproccessorClient::Load(QString projFile)
{
	Clear();
	CommunicatorClient::GetInstance()->LoadProject(projFile);
}

void ProjPreproccessorClient::ImportLoad(const QString projFile)
{
	Clear();
	CommunicatorClient::GetInstance()->LoadImportPorject(projFile);
}

void ProjPreproccessorClient::SetRecentOpens(QStringList lst)
{
	m_lstRecentOpens = lst;
}

QStringList ProjPreproccessorClient::GetRecentOpens()
{
	return m_lstRecentOpens;
}

QString ProjPreproccessorClient::GetOpenProjFile()
{
	QString file = QFileDialog::getOpenFileName(m_pFSEditor, tr("Please select a project file to open"), SettingDataMgr::GetInstance()->GetProjSavePath(), "project files(*.wfsproj)");
	return file;
}

ProjPreproccessorClient::QuestionSavingRes ProjPreproccessorClient::DoQuestionSaving()
{
	QuestionSavingRes res = QuestionSavingRes_UserCancel;
	CommunicatorClient *pCom = CommunicatorClient::GetInstance();
	if (IsContentChanged())
	{
		int ret = FSMessageBox::questionYesNoCancel(tr(sFilmoraScreen),
			QObject::tr("The project has been modified,Do you want to save the changes?"),
			m_pFSEditor);

		if (ret == FSMessageBox::Yes)
		{
			if (pCom->IsNewProj())
			{
				QString newpath = QFileDialog::getSaveFileName(m_pFSEditor, 
					tr("Please select directory"),
                    GetProjectSavePath(),
					"Project Fils(*.wfsproj)");
				if (!newpath.isEmpty())
				{
					pCom->SaveNew(newpath);
					res = QuestionSavingRes_Saved;
				}
				else
				{
					res = QuestionSavingRes_UserNoSavePath;
				}
			}
			else
			{
				pCom->Save();
				res = QuestionSavingRes_Saved;
			}
		}
		if (ret == FSMessageBox::No)
		{
			res = QuestionSavingRes_UserRefuse;
            CommunicatorClient::GetInstance()->QuitDontSave();
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

void ProjPreproccessorClient::SetContentChanged(bool bChanged)
{
	if (m_bContentChanged == bChanged) return;
	m_bContentChanged = bChanged;
	emit sigContentChanged();
}

bool ProjPreproccessorClient::IsContentChanged()
{
	return m_bContentChanged;
}

void ProjPreproccessorClient::SetNewProject(bool bNew)
{
	if (m_bNewProject == bNew) return;
	m_bNewProject = bNew;
	emit sigIsNewProjectChanged();
}

bool ProjPreproccessorClient::IsNewProject()
{
	return m_bNewProject;
}

QString ProjPreproccessorClient::GetProjectSavePath()
{
    return SettingDataMgr::GetInstance()->GetProjSavePath() + "/My Project";

}

