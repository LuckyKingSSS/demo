#ifndef GAINNERTHREAD_H
#define GAINNERTHREAD_H

#include <QThread>

class GAThread;
class TUATracker;

//接口类型
enum interfaceType
{
	type_AppVersion,  //上传app版本
	type_AppLanguage, //上传app语言
	type_AppViewSize, //上传app界面大小
	type_UserDimension,//上传用户自定义维度
	type_UserMetic,   //上传用户自定义
	type_Event,       //上传事件信息
	type_Page,         //上传页面切换
	type_AppSpeed,    //上传时间类参数
	type_UserSocial,  //上传用户的社交行为类数据
	type_Exception    //上传app使用过程中的异常信息
};

//上传的数据结构体
typedef struct
{
	interfaceType eventTypeInfo; // 接口类别
	QString strCategory;  // 类别
	QString strAction;    //具体行为
	QString strLabel;    //标签
	QString strPageName;//页面名称
	QString strVariable;//变量名称
	int iValue;        //数值
	int iIndex;       //序号
	QString strValue;
	QString strVersion;
	QString strLanguage;
    int Width;
    int Height;
	QString strNetWork;
	QString strTarget;
	QString strDescription;
	int iFatal;
	void gaInfo()
	{
		strCategory = "";
		strCategory = "";
		strAction = "";
		strLabel = "";
		strPageName = "";
		strVariable = "";
		iValue = 0;        
		iIndex = 0;
		strValue = "";
		strVersion = "";
		strLanguage = "";
		Width = 0;
		Height = 0;
		strNetWork = "";
		strAction = "";
		strTarget = "";
		strDescription = "";
		iFatal = 0;
	};
}*pGaInfo, gaInfo;

class  GAThread : public QThread
{
	Q_OBJECT

public:
	//设置为单例模式，方便全局获取线程对象
	static GAThread* getInstance();
	//启动线程
	void start();
	//挂起当前的线程
	void suspendThread();
	//恢复线程执行
	void  resumThread();
	//发送事件类信息
	void sendEventinfo(QString strCategory,QString strAction,QString strLabel);
	//发送速度类信息
	void sendAppSpeedInfo(QString strCategery,QString strVariable,int iValue);
	//发送屏幕信息
	void sendScreenInfo(QString strPageName);
	//发送用户自定义维度信息
	void setDimensioninfo(int iIndex, QString strValue);
	//发送程序版本信息
	void sendAppVersion(QString strVersion);
	//发送程序语言信息
	void sendLanguageInfo(QString strLanguage);
	//发送应用程序窗口大小
	void sendViewSize(int Width, int Height);
	//发送自定义信息
	void sendUserMetric(int iIndex, int iValue);
	//传送用户社交行为
	void SendSocialNetWorkInfo(QString strNetWork, QString strAction, QString strTarget);
	//传动软件异常信息
	void SendExceptionInfo(QString strDescription, int iFatal);
	//设置启动线程等待的时间
	void setWaitRunTime(int iWaitTime){ m_iWaitRunTime = iWaitTime; };

protected:
	//线程执行体
	void run();
private:
	GAThread(QObject *parent);
	bool IsOn();
public:
	~GAThread();
private:
	QQueue <gaInfo> m_EventList;   //事件队列
	QMutex m_SyncMutext;//用于对队列做同步访问的互斥对象
	QMutex m_TheadSuspendMutext;//用于线程悬停的互斥对象
	TUATracker* m_DataTraker;
	static QMutex m_Mutex;
	static GAThread* m_sGaThread;
	static QWaitCondition m_sWaitCondition;
	QSettings* m_Setting;
	bool m_bOn;
	int  m_iWaitRunTime; //等待启动的时间，单位为ms
};
#endif // GATHREAD_H
