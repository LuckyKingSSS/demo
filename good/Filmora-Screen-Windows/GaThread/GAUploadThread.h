#ifndef GATHREAD_H
#define GATHREAD_H

#include "gathread_global.h"
#include "gathread.h"
#include <QObject>

class GATHREAD_EXPORT GAUpload : public QObject
{
	Q_OBJECT

public:
	//设置为单例模式，方便全局获取线程对象
	static GAUpload* GetInstance();
	//发送事件类信息
	virtual void SendEventinfo(QString strCategory, QString strAction, QString strLabel) = 0;
	//发送速度类信息
	virtual void SendAppSpeedInfo(QString strCategery, QString strVariable, int iValue) = 0;
	//发送屏幕信息
	virtual void SendScreenInfo(QString strPageName) = 0;
	//发送用户自定义维度信息
	virtual void SetDimensioninfo(int iIndex, QString strValue) = 0;
	//发送程序版本信息
	virtual void SendAppVersion(QString strVersion) = 0;
	//发送程序语言信息
	virtual void SendLanguageInfo(QString strLanguage) = 0;
	//发送应用程序窗口大小
	virtual void SendViewSize(int Width, int Height) = 0;
	//发送自定义信息
	virtual void SendUserMetric(int iIndex, int iValue) = 0;
	//传送用户社交行为
	virtual void SendSocialNetWorkInfo(QString strNetWork, QString strAction, QString strTarget) = 0;
	//传动软件异常信息
	virtual void SendExceptionInfo(QString strDescription, int iFatal) = 0;
};
#endif // GATHREAD_H
