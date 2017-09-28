#include "stdafx.h"
#include "gathread.h"
#include "GoogleAnalyze/TUATracker.h"
#include <tchar.h> 
#include <string>
#include <QList>
#include "QFileInfo"

GAThread* GAThread::m_sGaThread = nullptr;
QMutex GAThread::m_Mutex;
QWaitCondition GAThread::m_sWaitCondition;
GAThread::GAThread(QObject *parent)
	: QThread(parent)
{
	//初始化成员变量
	m_bOn = true;
	//规避启动程序时程序死锁问题--提前获取TUATracker对象
	m_DataTraker = TUATracker::getInstance();
	m_iWaitRunTime = 0;
}

bool GAThread::IsOn()
{
	return m_bOn;
}

GAThread::~GAThread()
{

}

void GAThread::run()
{
	while (true)
	{
		if (!IsOn() || m_EventList.isEmpty())
		{
			msleep(100);
			continue;
		}
		gaInfo frontInfo = m_EventList.head();
		m_SyncMutext.lock();
		m_EventList.pop_front();
		m_SyncMutext.unlock();
		switch (frontInfo.eventTypeInfo)
		{
		case type_AppVersion:
		{
			m_DataTraker->setAppVerion(frontInfo.strVersion);
			break;
		}
		case type_AppLanguage:
		{
			m_DataTraker->SetLanguageAndLocate(frontInfo.strLanguage);
			break;
		}
		case type_AppViewSize:
		{
			m_DataTraker->SetViewSize(frontInfo.Width, frontInfo.Height);
			break;
		}
		case type_UserDimension:
		{
			m_DataTraker->SetDimension(frontInfo.iIndex, frontInfo.strValue);
			break;
		}
		case type_UserMetic:
		{
			m_DataTraker->SetMetric(frontInfo.iIndex, frontInfo.iValue);
			break;
		}
		case type_Event:
		{
			m_DataTraker->SendEvent(frontInfo.strCategory, frontInfo.strAction, frontInfo.strLabel, frontInfo.iValue);
			break;
		}
		case type_Page:
		{
			m_DataTraker->SendScreenName(frontInfo.strPageName);
			break;
		}
		case type_AppSpeed:
		{
			m_DataTraker->SendAppSpeed(frontInfo.strCategory, frontInfo.strVariable, frontInfo.iValue, frontInfo.strLabel);
			break;
		}
		case type_UserSocial:
		{
			m_DataTraker->SendSocialNetWorkInfo(frontInfo.strNetWork, frontInfo.strAction, frontInfo.strTarget);
			break;
		}
		case type_Exception:
		{
			m_DataTraker->SendExceptionInfo(frontInfo.strDescription, frontInfo.iFatal);
			break;
		}	
		default:
			break;
		}
		msleep(100);
	}
}

void GAThread::start()
{
	//延迟10s启动线程
	QTimer::singleShot(m_iWaitRunTime, this, [&](){
		QThread::start();
	});
}

void GAThread::suspendThread()
{
	m_TheadSuspendMutext.lock();
}

void GAThread::resumThread()
{
	m_TheadSuspendMutext.unlock();
}

void GAThread::sendEventinfo(QString strCategory, QString strAction, QString strLabel)
{
	gaInfo info;
	info.eventTypeInfo = type_Event;
	info.strCategory = strCategory;
	info.strAction = strAction;
	info.strLabel = strLabel;
	m_EventList.push_back(info);
}

void GAThread::sendAppSpeedInfo(QString strCategery, QString strVariable, int iValue)
{
	gaInfo info;
	info.eventTypeInfo = type_AppSpeed;
	info.strCategory = strCategery;
	info.strVariable = strVariable;
	info.iValue = iValue;
	m_EventList.push_back(info);
}

void GAThread::sendScreenInfo(QString strPageName)
{
	gaInfo info;
	info.eventTypeInfo = type_Page;
	info.strPageName = strPageName;
	m_EventList.push_back(info);
}

void GAThread::setDimensioninfo(int iIndex, QString strValue)
{
	gaInfo info;
	info.eventTypeInfo = type_UserDimension;
	info.iIndex = iIndex;
	info.strValue = strValue;
	m_EventList.push_back(info);
}

void GAThread::sendAppVersion(QString strVersion)
{
	gaInfo info;
	info.eventTypeInfo = type_AppVersion;
	info.strVersion = strVersion;
	m_EventList.push_back(info);
}

void GAThread::sendLanguageInfo(QString strLanguage)
{
	gaInfo info;
	info.eventTypeInfo = type_AppLanguage;
	info.strLanguage = strLanguage;
	m_EventList.push_back(info);
}

void GAThread::sendViewSize(int Width, int Height)
{
	gaInfo info;
	info.eventTypeInfo = type_AppViewSize;
	info.Width = Width;
	info.Height = Height;
	m_EventList.push_back(info);
}

void GAThread::sendUserMetric(int iIndex, int iValue)
{
	gaInfo info;
	info.eventTypeInfo = type_UserMetic;
	info.iIndex = iIndex;
	info.iValue = iValue;
	m_EventList.push_back(info);
}

void GAThread::SendSocialNetWorkInfo(QString strNetWork, QString strAction, QString strTarget)
{
	gaInfo info;
	info.eventTypeInfo = type_UserSocial;
	info.strNetWork = strNetWork;
	info.strAction = strAction;
	info.strTarget = strTarget;
	m_EventList.push_back(info);
}

void GAThread::SendExceptionInfo(QString strDescription, int iFatal)
{
	gaInfo info;
	info.eventTypeInfo = type_Exception;
	info.strDescription = strDescription;
	info.iFatal = iFatal;
	m_EventList.push_back(info);
}

GAThread* GAThread::getInstance()
{
	if (m_sGaThread == NULL)
	{   //double check  
		m_Mutex.lock();
		//用lock实现线程安全，用资源管理类，实现异常安全  
		//使用资源管理类，在抛出异常的时候，资源管理类对象会被析构，析构总是发生的无论是因为异常抛出还是语句块结束。  
		if (m_sGaThread == NULL)
		{
			m_sGaThread = new GAThread(nullptr);
		}
		m_Mutex.unlock();
	}
	return m_sGaThread;
}
