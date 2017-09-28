#include "stdafx.h"
#include "GAUploadWorkThread.h"

//定义静态变量
GAUpload* GAUploadWorkThread::m_pWorkThread = nullptr;
QMutex GAUploadWorkThread::m_Mutex;

GAUpload* GAUpload::GetInstance()
{
	if (GAUploadWorkThread::m_pWorkThread == NULL)
	{   //double check  
		GAUploadWorkThread::m_Mutex.lock();
		//用lock实现线程安全，用资源管理类，实现异常安全  
		//使用资源管理类，在抛出异常的时候，资源管理类对象会被析构，析构总是发生的无论是因为异常抛出还是语句块结束。  
		if (GAUploadWorkThread::m_pWorkThread == NULL)
		{
			GAUploadWorkThread::m_pWorkThread = new GAUploadWorkThread(nullptr);
		}
		GAUploadWorkThread::m_Mutex.unlock();
	}
	return GAUploadWorkThread::m_pWorkThread;
}

void GAUploadWorkThread::Start()
{
	m_pThread->start();
}

void GAUploadWorkThread::SuspendThread()
{
	m_pThread->suspendThread();
}

void GAUploadWorkThread::ResumThread()
{
	m_pThread->resumThread();
}

void GAUploadWorkThread::SendEventinfo(QString strCategory, QString strAction, QString strLabel)
{
	m_pThread->sendEventinfo(strCategory, strAction, strLabel);
}

void GAUploadWorkThread::SendAppSpeedInfo(QString strCategery, QString strVariable, int iValue)
{
	m_pThread->sendAppSpeedInfo(strCategery, strVariable, iValue);
}

void GAUploadWorkThread::SendScreenInfo(QString strPageName)
{
	m_pThread->sendScreenInfo(strPageName);
}

void GAUploadWorkThread::SetDimensioninfo(int iIndex, QString strValue)
{
	m_pThread->setDimensioninfo(iIndex, strValue);
}

void GAUploadWorkThread::SendAppVersion(QString strVersion)
{
	m_pThread->sendAppVersion(strVersion);
}

void GAUploadWorkThread::SendLanguageInfo(QString strLanguage)
{
	m_pThread->sendLanguageInfo(strLanguage);
}

void GAUploadWorkThread::SendViewSize(int Width, int Height)
{
	m_pThread->sendViewSize(Width, Height);
}

void GAUploadWorkThread::SendUserMetric(int iIndex, int iValue)
{
	m_pThread->sendUserMetric(iIndex, iValue);
}

void GAUploadWorkThread::SendSocialNetWorkInfo(QString strNetWork, QString strAction, QString strTarget)
{
	m_pThread->SendSocialNetWorkInfo(strNetWork, strAction, strTarget);
}

void GAUploadWorkThread::SendExceptionInfo(QString strDescription, int iFatal)
{
	m_pThread->SendExceptionInfo(strDescription, iFatal);
}

void GAUploadWorkThread::SetWaitRunTime(int iWaitTime)
{
	m_pThread->setWaitRunTime(iWaitTime);
}

GAUploadWorkThread::GAUploadWorkThread(QObject *parent)
{
	m_pThread = GAThread::getInstance();
}
