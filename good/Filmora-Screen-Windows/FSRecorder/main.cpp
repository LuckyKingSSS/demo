#include "stdafx.h"
#include <QtWidgets/QApplication>
#include <windows.h> 
#include "AppStartController.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_FSCommonlib/MultiLangMgr.h"

#include "RecorderViewsMgr.h "
#include "RecordPanel.h"
#include "FSRecorder.h"
#include "../include/inc_FSCommonlib/gathread.h"
#include "WSLog.h"
#include "inc_CommonLib/PathOperation.h"

LONGLONG getAppStartTime();
void writeTimeLogFile(QString strStartTime);
void UploadStartGAInfo();
void UploadExitGAInfo();


int main(int argc, char *argv[])
{
	qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
	QString strStartTime = QString::number(QDateTime::currentDateTime().toTime_t());
	QApplication a(argc, argv);
	if (!AppStartController::GetInstance()->CheckStartDuration())
		return 0;
	//上传程序正常启动的信息
	UploadStartGAInfo();
	WSLogInit(L"log1.txt", 1, 1);
	
    OneApplication oneApp("Wondershare Filmora Scrn");
	if (argc == 1)
	{
		if (!oneApp.IsOneApp())
		{
			CurrentViewPageMgr::GetInstance()->ShowNeedShowWindow();
			return 0;
		}
		CurrentViewPageMgr::GetInstance()->TerminateFSEditor();
		
	}
	if (argc != 1)
	{
		//如果用户双击工程进入则记录时间
		writeTimeLogFile(strStartTime);
	}
    AppStartController::GetInstance()->Init(&a, argc, argv);

	MultiLangMgr::GetInstance()->SetLangLocale();

	QFont font(loadFontFamilyFromFiles(":/FSCommonLib/Resources/Lato-Regular.ttf"), a.font().pointSize());
	qApp->setFont(font);

	int iRec = AppStartController::GetInstance()->Start();
	//上传程序正常退出的信息
	UploadExitGAInfo();
	return iRec;
}

LONGLONG getAppStartTime()
{
	LARGE_INTEGER litmp;
	LONGLONG Qpart1;
	double dfMinus, dfFreq, dfTime;
	//获得CPU计时器的时钟频率  
	QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s） 
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值  
	Qpart1 = litmp.QuadPart; //开始计时  
	return Qpart1;
}

void writeTimeLogFile(QString strStartTime)
{
	//参数不为空，则为双击工程文件操作
	QString strFilePath = FSGetFilmoraScreenDirectory();
	strFilePath.append("\\timeLog.txt");
	QFile file(strFilePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "open timelog file failed";
	}
	else
	{
		QTextStream out(&file);
		out << "time:" << strStartTime;
		out.flush();
		file.close();
	}
}

void UploadStartGAInfo()
{
	GAThread::getInstance()->setAppStartTime(getAppStartTime());
	GAThread::getInstance()->insertEventItem(AppStartAndExitCount,"AppSatart","");
}

void UploadExitGAInfo()
{
	GAThread::getInstance()->insertEventItem(AppStartAndExitCount, "AppExit");
}