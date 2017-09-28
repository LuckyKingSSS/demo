#include "stdafx.h"
#include "AppStartController.h"
#include <QApplication>
#include "SkinMgr.h"
#include "WSLog.h"
#include "RecorderPreview.h"
#include "RecorderViewsMgr.h"
#include "FSRecorder.h"
#include "Communicator.h"
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>
#include "inc_FSCommonlib/TUATracker.h"
#include "inc_FSCommonlib/gathread.h"
#include "ProjectFilesMgr.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_Commonlib/DesktopMgr.h"

#include "FSSplashScreen.h"

//#define  RECORDER_WINDOW_NAME		L"Wondershare Filmora Scrn\r"
#define  LOCALSERVER_NAME			"FSRecorderServer"

#define  CMD_REQUEST_QUIT			"request_quit"
#define  CMD_REFUSE_QUIT			"refuse_quit"
#define  CMD_FINISH_QUIT			"finish_quit"

#define  CMD_IS_SAME_PROJECT		"is_same_project"
#define  CMD_RESPONE_SAME_PROJECT		"respone_same_project"


extern bool DoClose(QWidget* parentWidget);
AppStartController::AppStartController(QObject *parent)
	: QObject(parent)
{
	m_bProjStart = false;
	m_bAnotherRefuse = false;
	m_bAnotherFinishQuit = false;
	m_bSameProject = false;
	m_bResponed = false;
}

AppStartController* AppStartController::GetInstance()
{
	static AppStartController ins;
	return &ins;
}

AppStartController::~AppStartController()
{
	if (m_pResponeSocket)
		m_pResponeSocket->write(CMD_FINISH_QUIT);
}

void AppStartController::Init(QApplication *app, int argc, char *argv[])
{
	//发送GA数据
	GAThread::getInstance()->insertEventItem(Sys_Info, "", "");
	GAThread::getInstance()->start();
	m_appObj = app;
	ParseCmdLine(argc, argv);
	m_pResponeSocket = nullptr;
	m_pServer = new QLocalServer(this);
	m_pConnectSocket = new QLocalSocket(this);
	connect(m_pServer, &QLocalServer::newConnection, this, &AppStartController::OnNewConnect);
	connect(m_pConnectSocket, &QLocalSocket::readyRead, this, [&]{
		QString res = QString::fromUtf8(m_pConnectSocket->readAll());
		if (res.contains(CMD_REFUSE_QUIT))
		{
			m_bAnotherRefuse = true;
			return;
		}
		if (res.contains(CMD_FINISH_QUIT))
		{
			m_bAnotherFinishQuit = true;
			return;
		}
		if (res.contains(CMD_RESPONE_SAME_PROJECT))
		{
			int pos1 = res.indexOf("$");
			int pos2 = res.indexOf("@");
			QString strSame = res.mid(pos1 + 1, pos2 - pos1 - 1);
			m_bSameProject = strSame == "1" ? true : false;
			m_bResponed = true;
			return;
		}
	});
}

void AppStartController::OnNewConnect()
{
	m_pResponeSocket = m_pServer->nextPendingConnection();
	connect(m_pResponeSocket, &QLocalSocket::readyRead, this, &AppStartController::OnServerRead);
}

void AppStartController::OnServerRead()
{
	QString res = QString::fromUtf8(m_pResponeSocket->readAll());
	if (res.contains(CMD_REQUEST_QUIT))
	{
		bool bCancel = DoClose(0);
		if (bCancel)
			m_pResponeSocket->write(QString(CMD_REFUSE_QUIT).toUtf8());
	}
	if (res.contains(CMD_IS_SAME_PROJECT))
	{
		int pos1 = res.indexOf("$");
		int pos2 = res.indexOf("@");
		QString proj = res.mid(pos1 + 1, pos2 - pos1 - 1);
		bool bSameProj = proj == ProjectMgr::GetInstance()->GetProjectPath();
		QString respone = QString(CMD_RESPONE_SAME_PROJECT) + "$" + (bSameProj ? "1" : "0") + "@";
		m_pResponeSocket->write(respone.toUtf8());
	}
}


void AppStartController::ParseCmdLine(int argc, char *argv[])
{
	m_bProjStart = (argc == 2);
	if (m_bProjStart)
		m_strProjFile = argv[1];
}

bool AppStartController::QuitAnother()
{
	m_pConnectSocket->write(QString(CMD_REQUEST_QUIT).toUtf8());
	while (!m_bAnotherFinishQuit)
	{
		if (m_bAnotherRefuse)
		{
			return false;
		}
		QApplication::processEvents();
	}
	return true;
}

bool AppStartController::IsSameProject(QString projFile)
{
	QString cmd = QString(CMD_IS_SAME_PROJECT) + "$" + projFile + "@";
	m_pConnectSocket->write(cmd.toUtf8());
	while (!m_bResponed)
	{
		QApplication::processEvents();
	}
	return m_bSameProject;
}

int AppStartController::Start()
{
	m_pConnectSocket->connectToServer(LOCALSERVER_NAME);
	m_pConnectSocket->waitForConnected();

	auto state = m_pConnectSocket->state();
	bool bHasOne = (state == QLocalSocket::ConnectedState);

	if (!m_bProjStart && bHasOne)
	{
        return 0;
	}
		
	if (!m_bProjStart && !bHasOne)
	{
        FSSplashScreen splash;
        splash.show();

		FSRecorder w(NULL);
        //sSetFont(&w);
        RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_GettingStart);
        splash.finish(&w);
        //上传GA数据
        GAThread::getInstance()->insertEventItem(Change_Page, "Setup", "");
		//上传工程数量信息	
		int iProjectCount = ProjectFilesMgr::GetInstance()->GetRecentInfos().length();
		GAThread::getInstance()->insertEventItem(Start_Up, "ProjectListCount", QString::number(iProjectCount));

		m_pServer->listen(LOCALSERVER_NAME);
		return m_appObj->exec();
	}
	//m_bProjStart == true，双击工程文件启动

	//快速重复双击同一个工程文件，上一个recorder的server还没来得及建立，这个record已经起来的情况，这里重新再去做连接
	OneApplication oneApp("Wondershare Filmora Scrn");
	if (!oneApp.IsOneApp())
	{
		int cnt = 0;
		while (!bHasOne && cnt++ < 5)
		{
			QThread::msleep(200);
			m_pConnectSocket->connectToServer(LOCALSERVER_NAME);
			m_pConnectSocket->waitForConnected();
			state = m_pConnectSocket->state();
			bHasOne = (state == QLocalSocket::ConnectedState);
		}
	}

	if (bHasOne)
	{
		if (IsSameProject(m_strProjFile))
		{
			return 0;
		}
		if (!QuitAnother())
		{
			return 0;
		}
	}

    FSSplashScreen splash;
    splash.show();
	FSRecorder w(NULL);
	RecorderViewsMgr::GetInstance()->ShowMainView(FSRecorderMainView::Page_GettingStart);
    splash.finish(&w);
    QTimer::singleShot(0, [&]{ProjPreproccessor::GetInstance()->LoadProject(m_strProjFile); });
	m_pServer->listen(LOCALSERVER_NAME);
	return m_appObj->exec();
}

bool AppStartController::CheckStartDuration()
{
	auto curTime = ::GetTickCount();
	QString tmpFSDir = PathOperation::GetTempDir() + "/FilmoraScrn";
	QDir dir(tmpFSDir);
	if (!dir.exists())
		dir.mkpath(tmpFSDir);
	QString fileName = tmpFSDir + "/appstart.ini";

	bool bRet = false;
	if (!FileOperation::IsFileExist(fileName))
	{
		FileOperation::CreateNewFile(fileName);
		QSettings setting(fileName, QSettings::IniFormat);
		setting.beginGroup("StartTime");
		setting.setValue("time", (unsigned int)curTime);
		setting.endGroup();
		bRet = true;
	}
	else
	{
		QSettings setting(fileName, QSettings::IniFormat);
		setting.beginGroup("StartTime");
		auto startTime = setting.value("time").toUInt();
		setting.setValue("time", (unsigned int)curTime);
		setting.endGroup();
		bRet = ((curTime - startTime) > 3000);
	}
	return bRet;
}
