#include "stdafx.h"
#include "FSEditor.h"
#include <QtWidgets/QApplication>
#include <QSharedMemory>
#include "inc_CommonLib/externalapi.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "inc_FSExport/IFSExport.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "CommunicatorClient.h"
#include "inc_UpdateRegister/RegisterMain.h"
#include "inc_UpdateRegister/UpdateMain.h"
#include "WSLog.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_FSCommonlib/MultiLangMgr.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "qtbuiltinhook.h"

bool IsRecorderStarted();
void TransFromSpeedInfo();

int main(int argc, char *argv[])
{
	qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
	QApplication a(argc, argv);
	Squish::allowAttaching(11233);
#ifdef _DEBUG
	//QMessageBox::information(0, "for debug", "");
#endif // _DEBUG
    OneApplication oneApp("Filmora Screen Editor");
	//上传GA数据
	TransFromSpeedInfo();
    if (!oneApp.IsOneApp())
    {
		oneApp.ShowWindow(GetFSEditTitleName());
		return 0;
    }
#ifndef _DEBUG
	if (/*IsRecorderStarted()*/ argc < 2)
		return 0;
#endif // !_DEBUG

    WSLogInit(L"log0.txt", 1, 1);
    qApp->setStyleSheet("");
    qApp->setStyleSheet(gGetCommonStyleSheet() + GetPlayerStyleSheet() + gGetExportStyleSheet() + gGetStyleSheet(":/FSEditor/Resources/styles_light.css"));

	ProjPreproccessorClient::GetInstance();
	MultiLangMgr::GetInstance()->SetLangLocale();
	FSEditor w;
	w.ShowMaximized();
	//sSetFont(&w);
	QFont font(loadFontFamilyFromFiles(":/FSCommonLib/Resources/Lato-Regular.ttf"),a.font().pointSize());
	qApp->setFont(font);
	//上传GA数据
	GAThread::getInstance()->insertEventItem(Change_Page,"VideoEditor","");
	GAThread::getInstance()->start();
	return a.exec();
}


bool IsRecorderStarted()
{
#ifdef _DEBUG
	QString fsrecorderString("fsrecorderD.exe");
#else
	QString fsrecorderString("fsrecorder.exe");
#endif
	auto processID = ProcessManager::FindProcessID(fsrecorderString);
	return processID != 0;
}

void TransFromSpeedInfo()
{
	QString strFilePath = FSGetFilmoraScreenDirectory();
	strFilePath.append("\\timeLog.txt");
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//如果文件不存在，则不是用户双击工程文件后进入Edit
		qDebug() << "timeLog.txt didn't exist";
		return;
	}
	QString strEndTime = QString::number(QDateTime::currentDateTime().toTime_t()*1000);
	//上传用户时间参数
	GAThread::getInstance()->insertEventItem(VideoEditorLaunchFromLocalFile, "VideoEditorTime", strEndTime);
	//上传完成后删除文件
	QFile::remove(strFilePath);
}