#ifndef GATHREAD_H
#define GATHREAD_H

#include "inc_FSCommonlib/fscommonlib_global.h"
#include <QThread>
#include "FSNLETimeline.h"

class GAThread;
class FSNLETimeline;
class TimelineManager;
//系统信息结构体
typedef struct
{
	QString strCpuMode;  //cpu型号
	QString strGpuMode;  //Gpu型号
	QString strSysRamInfo;//系统内存大小
	QString strOSBit;     //操作系统的位数
	QString strSysResolution;//系统分辨率
	QString strSysVersion;  //操作系统的版本
	QString strSysLanguage;//系统语言
	QString strProductVersion;//产品的版本号
}*pSysInfo, sysInfo;

//视频文件属性
typedef struct
{
	QString strFileType;      //文件类型
	QString strFileFormat;    //文件格式
	QString strFileSizeRange; // 视频文件的大小
	QString strFileDurationRange;//视频时间长度
	QString strVedioResolutionRange;//视频分辨率
	QString strVedioCodec;         //视频解码器
	QString strVedioFramerate;//视频帧率
	QString strVedioBitrate; // 视频的比特率
}*pVedioInfo, VedioInfo;

//音频文件属性
typedef struct
{
	QString strAudioBitrate;//音频比特率
	QString strAudioCodec; //音频解码器
	QString strAudioChannel; //音频通道
	QString strAudioSampleRate;//音频样品率
}*pAudioInfo, AudioInfo;

enum eventType
{
	Sys_Info,       //上传系统信息事件
	Start_Up,       //上传启动页面操作事件
	Recorder_Device,//上传录屏设备信息事件
	Recorder_Widget,//上传录屏的视频信息事件
	Your_Clips,     //上传操作的clip信息的事件
	Import,         //上传导入操作信息的事件
	Import_Media,   //上传导入的多媒体文件的信息事件
	Import_Failed,  //上传导入失败信息事件
	Clip_Edit,      //上传用户的Clip操作事件
	Annotation_Edit,//上传用户的Annotation Edit操作事件
	Export_Profile, //上传用户的Export Profile 操作事件
	Export_Fail,    //上传用户的Export  Fail 操作事件
	Project_Management, //上传用户的Project  Management操作事件
	Register,            //上传用户的Register操作事件
	Buy,                 //上传用户的Buy操作事件
	Change_Page,        //用户页面切换事件
	App_Launch,         //app启动时长统计
	VideoEditorLaunchFromStartup,//VideoEditor页面切换时间
	ClipsSeekSpeed, //Seek时间
	YourClipsLaunchFromStartup,//yourclips页面切换时间
	YourClipsLaunchFromRecorder,//yourclips页面切换时间
	ImportTimer,  //导入时间类型
	ExportTimer, //导出文件时间类型
	RegisterTimer,//注册时间类型
	VideoEditorLaunchFromLocalFile, //从本地双击Editor工程文件
	YourClipsLaunchFromLocalFile,  //从本地双击Recorder工程文件
	UserDefineDimension,            //用户自定义维度
	AppStartAndExitCount            //用户正常进入程序和退出程序次数统计
};

//上传的数据结构体
typedef struct
{
	eventType eventTypeInfo;//事件的类型信息
	//action 的值,分3种情况
	//1：如果是计时器则是计时器的名称,
	//2: 如果是切换页面则是新页面的名称
	//3：如果是Event 则是具体的事件名称
	QString strAction;
	QString strLabel;//标签的值，如果是计时器则该参数为NULL
	int iValue;//如果是计时器信息，则是所经历的毫秒数，如果是其他则为0
}*pGaInfo, gaInfo;

//配置文件分割类型
enum partitionType
{
	EqualType,//等号分割
	ColonType,//冒号分割
};

class FSCOMMONLIB_EXPORT GAThread : public QThread
{
	Q_OBJECT

public:
	~GAThread();
public:
	void setCurrentEventType(eventType curType){ m_CurrentEventType = curType; };
	void TransformSysInfo();
	void TransformStartUpInfo();
	void TransformRecorderDeviceInfo();
	void TransFromSpeedInfo();
	//上传自定义维度信息
	void TransfromDimensionInfo();
	//通过获取到的视频时长，得到视频所在的时间范围
	QString getRecordDurarionRange(int iCounts);
	//获取系统配置信息
	pSysInfo getSysInfo();
	//宽字符转窄字符
	BOOL WStringToString(const std::wstring &wstr, std::string &str);
	//读取注册表中特定项的值
	std::string  ReadRegValue(wchar_t* wcRegPath, wchar_t* wcRegKey);
	//挂起当前的线程
	void suspendThread();
	//恢复线程执行
	void  resumThread();
	//在队列中插入一个事件信息项
	void insertEventItem(gaInfo& eventInfo);
	//插入事件信息项, 重载形式
	void insertEventItem(eventType eventTypeInfo, QString strAction, QString strLabel= "", int iValue = 0);
	//设置为单例模式，方便全局获取线程对象
	static GAThread* getInstance();
	//将枚举型的变量做转换成字符串
	QString convertEventTypeToString(eventType tyepInfo);
	//插入视频文件的属性
	VedioInfo insertVedioFileInfo(FSNLETimeline *timeline);
	//插入音频文件的属性信息
	AudioInfo insertAudioInfo(FSNLETimeline *timeline);
	//
	QString sGetFourCCString(int fourcc);
	//获取图像分辨率所在的范围
	QString getPicResolutionRange(NLESize picSize);
	//插入ClipCount信息
	void insetClipCountInfo(NLETimeline* timeline);
	//插入ClipsCount信息
	void insetClipCountInfo(TimelineManager*);
	//获取导出文件的大小范围
	QString getExportFileRange(qint64 fileSize);
	//获取导出时间的范围 注意传入的时间单位是秒
	QString getExportFileDurationRange(qint64 durationRange);
	//获取导出文件的分辨率的范围
	QString getFileResolutionRange(int  iArea);
	//插入当前的seek操作信息
	void insertSeekInfo(int iCurCost){ iTotalSeekCost += iCurCost; iTotalSeekCount++; };
	QString getAverageTime(){ return  QString::number((float)(iTotalSeekCost * 1000) / (float)qMax(1, iTotalSeekCount)); };
	void startTime(){ m_Time.restart(); };
	int getElapse(){return m_Time.elapsed();};
	void setLoadProjectType(bool bType){ m_bIsLoadFromRecorder = bType; };
	bool getLoadProjectType(){ return m_bIsLoadFromRecorder; };
	void increaseSeekCount(){ iTotalSeekCount++; };
	int getSeekCount(){ return iTotalSeekCount; };
	void  setStartRegisterTime(QDateTime time){ m_StartRegTime = time; };
	QDateTime getStartRegisterTime(){ return m_StartRegTime; };
	long long getAppStartTime(){ return m_lAppStartTime; };
	void setAppStartTime(long long time){ m_lAppStartTime = time; };
	QDateTime getAppInstallTime();
	void  setIsStartFromStartup(bool bIsFromStartUp){ bIsStartFromStartUp = bIsFromStartUp; };
	bool getIsStartFromStartUp(){ return bIsStartFromStartUp; };
	//启动线程
	void start();

protected:
	//线程执行体
	void run();
	std::string getGpuMode();
	std::wstring getSysLanguage();
	std::string getSysRamSize();
	std::string getSysResolution();
	std::string getProductVersion();
	//获取操作系统的版本号（兼容win10）
	std::string getOsVersion();
	//在配置文件中通过key值读取到对应的配置值
	std::string getConifgByKey(QString strConfigFilePath, QString strKey, partitionType divType = EqualType);
	QString getFileType(TimelineType type);
	//获取文件大小范围
	QString getFileSizeRange(QString strFilePath);
	//获取视频文件的时间范围
	QString getFileDurationRange(FSNLETimeline * timeline);
	//获取文件的分辨率范围
	QString getFileResolutionRange(QSizeF size);
private:
	GAThread(QObject *parent);
	void TranfromPageInfo();
	void TransfromStartAndExitCount();
	bool IsOn();
private:
	eventType m_CurrentEventType; //当前操作的事件类型
	QQueue <gaInfo> m_EventList;   //事件队列
	QMutex m_SyncMutext;//用于对队列做同步访问的互斥对象
	QMutex m_TheadSuspendMutext;//用于线程悬停的互斥对象
	gaInfo m_CurrentInfo;//当前节点的信息
	static QMutex m_Mutex;
	static GAThread* m_sGaThread;
	static QWaitCondition m_sWaitCondition;
	int iTotalSeekCost;//当前所有的seek操作的总耗时
	int iTotalSeekCount;//当前seek操作的总的次数
	QTime m_Time;//用于计时
	bool m_bIsLoadFromRecorder;//工程文件是否是从Recorder加载
	QDateTime m_StartRegTime;//启动注册的时间
	long long m_lAppStartTime;//app 启动的时间
	QDateTime m_AppInstallTime;//app的安装时间
	QSettings* m_Setting;
	bool m_bOn;
	bool bIsStartFromStartUp;
};
#endif // GATHREAD_H
