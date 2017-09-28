#pragma once
#include <QMutex>
/************************************************************************/
/* 作者：wangbiao
   功能：封装Goolgle 分析上传数据接口，方便调用
   说明：因为内部需要上传的数据比较多，如果每次上传都new 一个对象会造成比较大的
         资源消耗，故此类设计成为单例模式
   时间：2017/06/19
*/
/************************************************************************/
typedef void* ua_tracker;
class TUATracker
{
public:
	~TUATracker();
	void createTracker(const char* tracking_id, const char* client_id, const char* app_name);
	//发送程序版本信息
	void setAppVerion(QString strVersion);
	//发送应用程序语言信息
	void SetLanguageAndLocate(QString string);
	//发送应用程序窗口大小信息
	void  SetViewSize(int Width, int Height);
	//发送自定义维度信息
	void SetDimension(int iIndex, QString strValue);
	//发送自定义指标信息
	void SetMetric(int iIndex, int iValue);
	//传送用户事件
	void SendEvent(QString  strCategory, QString strAction, QString strLabel,int iValue = 0);
	//发送用户切换界面信息
	void SendScreenName(QString strName);
	//传送应用速度的参数
	void SendAppSpeed(QString  strCategory, QString strVariable, int iValue, QString strLable = "");
	//传送用户社交行为
	void SendSocialNetWorkInfo(QString strNetWork,QString strAction,QString strTarget);
	//传动软件异常信息
	void SendExceptionInfo(QString strDescription,int iFatal);
	//QString 转换为 char* 类型
	char* QStr2Char(QString strInput);
public:
	static TUATracker *getInstance();
	static TUATracker *m_pInstance;
	static QMutex  m_Mutex;

private:
	ua_tracker m_Tracker;
private:
	QString getClientId();
private:
	TUATracker();
	//避免赋值操作影响单例，赋值操作设置为私有，封闭赋值操作
	TUATracker(const TUATracker &);
	TUATracker& operator = (const TUATracker &);
};

