#ifndef GAUPLOADWORKTHREAD_H
#define GAUPLOADWORKTHREAD_H

#include <stdafx.h>
#include "GAUploadThread.h"

class GAUploadWorkThread : public GAUpload
{
	Q_OBJECT

public:
	//启动线程
	virtual void Start();
	//挂起当前的线程
	virtual void SuspendThread();
	//恢复线程执行
	virtual void  ResumThread();
	//发送事件类信息
	virtual void SendEventinfo(QString strCategory, QString strAction, QString strLabel);
	//发送速度类信息
	virtual void SendAppSpeedInfo(QString strCategery, QString strVariable, int iValue);
	//发送屏幕信息
	virtual void SendScreenInfo(QString strPageName);
	//发送用户自定义维度信息
	virtual void SetDimensioninfo(int iIndex, QString strValue);
	//发送程序版本信息
	virtual void SendAppVersion(QString strVersion);
	//发送程序语言信息
	virtual void SendLanguageInfo(QString strLanguage);
	//发送应用程序窗口大小
	virtual void SendViewSize(int Width, int Height);
	//发送自定义信息
	virtual void SendUserMetric(int iIndex, int iValue);
	//传送用户社交行为
	virtual void SendSocialNetWorkInfo(QString strNetWork, QString strAction, QString strTarget);
	//传动软件异常信息
	virtual void SendExceptionInfo(QString strDescription, int iFatal);
	//设置启动线程等待的时间
	virtual void SetWaitRunTime(int iWaitTime);
	//构造函数
	GAUploadWorkThread(QObject *parent);
public:
	static GAUpload* m_pWorkThread;
	static QMutex m_Mutex;
protected:
	GAThread* m_pThread;
};
#endif // GAUPLOADWORKTHREAD_H