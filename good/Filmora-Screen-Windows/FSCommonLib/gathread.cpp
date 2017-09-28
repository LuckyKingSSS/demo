#include "stdafx.h"
#include "inc_FSCommonlib\gathread.h"
#include "inc_FSCommonlib\TUATracker.h"
#include <tchar.h> 
#include <string>
#include <QList>
#include "inc_FSCommonlib\FSNLETimeline.h"
#include "QFileInfo"
#include "INLEIterator.h"
#include "NLEComPtr.h"
#include "INLETimelineBase.h"
#include "NLECommon.h"
#include "..\FSTimelineView\TimelineClip.h"
#include "..\FSTimelineView\TimelineManager.h"
#include "..\FSExport\FSExportDataDef.h"
#include "inc_FSCommonlib\ExternalAPI.h"

GAThread* GAThread::m_sGaThread = nullptr;
QMutex GAThread::m_Mutex;
QWaitCondition GAThread::m_sWaitCondition;
GAThread::GAThread(QObject *parent)
	: QThread(parent), m_bIsLoadFromRecorder(false)
{
	//初始化成员变量
	 iTotalSeekCost = 0;
	 iTotalSeekCount = 0;
	 m_lAppStartTime = 0;
	 bIsStartFromStartUp = false;
	 m_bOn = true;
	 m_Time = QTime();
	 //规避启动程序时程序死锁问题--提前获取TUATracker对象
	 TUATracker::getInstance();
	 //m_Setting = new QSettings(FSGetFilmoraScreenDirectory() + "\\FilmoraScrn.ini", QSettings::IniFormat);
	 //m_Setting->beginGroup("DataCollection");
	 //m_Setting->setValue("IsOn", QVariant("false"));
	 //m_Setting->endGroup();
}

void GAThread::TranfromPageInfo()
{
	TUATracker* tracker = TUATracker::getInstance();
	tracker->SendScreenName(m_CurrentInfo.strAction);
}

void GAThread::TransfromStartAndExitCount()
{
	TUATracker* DataTraker = TUATracker::getInstance();
	QString strTypeInfo = convertEventTypeToString(m_CurrentInfo.eventTypeInfo);
	DataTraker->SendEvent(strTypeInfo, m_CurrentInfo.strAction, m_CurrentInfo.strLabel);
}

bool GAThread::IsOn()
{
	//m_bOn = m_Setting->value("IsOn").toBool();
	return m_bOn;
}

GAThread::~GAThread()
{

}

void GAThread::run()
{
	while (true)
	{
		if (!IsOn()|| m_EventList.isEmpty())
		{
			msleep(100);
			continue;
		}
		gaInfo frontInfo = m_EventList.head();
		m_CurrentInfo = frontInfo;
		m_SyncMutext.lock();
		m_EventList.pop_front();
		m_SyncMutext.unlock();
		m_CurrentEventType = frontInfo.eventTypeInfo;
		switch (m_CurrentEventType)
		{
		case Sys_Info:
			TransformSysInfo();
			break;
		case Start_Up:
		case Recorder_Device:
		case Recorder_Widget:
		case Your_Clips:
		case Import:
		case Import_Media:
		case Import_Failed:
		case Clip_Edit:
		case Annotation_Edit:
		case Export_Profile:
		case Export_Fail:
		case Project_Management:
		case Register:
		case Buy:
			TransformStartUpInfo();
			break;
		case Change_Page:
			TranfromPageInfo();
			break;
		case App_Launch:
		case VideoEditorLaunchFromStartup:
		case ClipsSeekSpeed:
		case YourClipsLaunchFromStartup:
		case YourClipsLaunchFromRecorder:
		case ImportTimer:
		case ExportTimer:
		case RegisterTimer:
			TransFromSpeedInfo();
			break;
		case UserDefineDimension:
			TransfromDimensionInfo();
			break;
		case AppStartAndExitCount:
			TransfromStartAndExitCount();
			break;
		default:
			break;
		}
		msleep(100);
	}
}

std::string GAThread::getGpuMode()
{
	QString strAppPath = qApp->applicationDirPath();
	strAppPath.append("\\Log\\Ogre.log");
	return getConifgByKey(strAppPath, "Device Name", ColonType);
}

std::wstring GAThread::getSysLanguage()
{
	LANGID lanId = GetSystemDefaultLangID();
	wchar_t cLan[255] = { 0 };
	DWORD dSize = 254;
	VerLanguageName(lanId, cLan, dSize);
	std::wstring wStrLan = cLan;
	std::string strRect = "";
	return wStrLan;
}

std::string GAThread::getSysRamSize()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	DWORDLONG dRamSize = statex.ullTotalPhys;
	float fKbSize = dRamSize / 1024;
	float fMbSize = fKbSize / 1024;
	float fGbSize = fMbSize / 1024;
	float fTbSize = fGbSize / 1024;
	std::string strRct = "";
	char* cResult = new char[65535];
	if (fTbSize > 1.00)
	{
		sprintf(cResult, "%f", fTbSize);
		strRct = cResult;
		strRct.append("TB");
		return strRct;
	}
	if (fGbSize > 1.00)
	{
		sprintf(cResult, "%f", fGbSize);
		strRct = cResult;
		strRct.append("GB");
		return strRct;
	}
	if (fMbSize > 1.00)
	{
		sprintf(cResult, "%f", fMbSize);
		strRct = cResult;
		strRct.append("MB");
		return strRct;
	}
	if (fKbSize > 1.00)
	{
		sprintf(cResult, "%f", fKbSize);
		strRct = cResult;
		strRct.append("KB");
		return strRct;
	}
	//释放内存
	delete cResult;
	cResult = NULL;
	return strRct;
}

std::string GAThread::getSysResolution()
{
	QVariant iWindowWidth = GetSystemMetrics(SM_CXSCREEN);
	QVariant iWindowHeight = GetSystemMetrics(SM_CYSCREEN);
	QString strSysResolution = "";
	strSysResolution = iWindowWidth.toString().append("*").append(iWindowHeight.toString());
	char* ch;
	QByteArray ba = strSysResolution.toLocal8Bit();
	ch = ba.data();
	return ch;
}

std::string GAThread::getProductVersion()
{
	QString strAppPath = qApp->applicationDirPath();
	strAppPath.append("\\UpdateRegister.ini");
	return getConifgByKey(strAppPath, "Version");
}

std::string GAThread::getOsVersion()
{
	std::string vname;
	//先判断是否为win8.1或win10  
	typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary(L"ntdll.dll");
	DWORD dwMajor, dwMinor, dwBuildNumber;
	NTPROC proc = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");
	proc(&dwMajor, &dwMinor, &dwBuildNumber);
	if (dwMajor == 6 && dwMinor == 3)   //win 8.1  
	{
		vname = "Microsoft Windows 8.1";
		qDebug()<<"此电脑的版本为:"<<vname.c_str();
		return vname;
	}
	if (dwMajor == 10 && dwMinor == 0)  //win 10  
	{
		vname = "Microsoft Windows 10";
		qDebug()<<"此电脑的版本为: "<<vname.c_str();
		return vname;
	}
	//判断win8.1以下的版本  
	SYSTEM_INFO info;                //用SYSTEM_INFO结构判断64位AMD处理器    
	GetSystemInfo(&info);            //调用GetSystemInfo函数填充结构    
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
#pragma warning(disable:4996)  
	if (GetVersionEx((OSVERSIONINFO *)&os))
	{
		//下面根据版本信息判断操作系统名称    
		switch (os.dwMajorVersion)
		{                        //判断主版本号    
		case 4:
			switch (os.dwMinorVersion)
			{                //判断次版本号    
			case 0:
				if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
					vname = "Microsoft Windows NT 4.0";  //1996年7月发布    
				else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
					vname = "Microsoft Windows 95";
				break;
			case 10:
				vname = "Microsoft Windows 98";
				break;
			case 90:
				vname = "Microsoft Windows Me";
				break;
			}
			break;
		case 5:
			switch (os.dwMinorVersion)
			{               //再比较dwMinorVersion的值    
			case 0:
				vname = "Microsoft Windows 2000";    //1999年12月发布    
				break;
			case 1:
				vname = "Microsoft Windows XP";      //2001年8月发布    
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION &&
					info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					vname = "Microsoft Windows XP Professional x64 Edition";
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
					vname = "Microsoft Windows Server 2003";   //2003年3月发布    
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
					vname = "Microsoft Windows Server 2003 R2";
				break;
			}
			break;
		case 6:
			switch (os.dwMinorVersion)
			{
			case 0:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Microsoft Windows Vista";
				else
					vname = "Microsoft Windows Server 2008";   //服务器版本    
				break;
			case 1:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Microsoft Windows 7";
				else
					vname = "Microsoft Windows Server 2008 R2";
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION)
					vname = "Microsoft Windows 8";
				else
					vname = "Microsoft Windows Server 2012";
				break;
			}
			break;
		default:
			vname = "未知操作系统";
		}
		qDebug()<<"此电脑的版本为: "<<vname.c_str();
	}
	else
		qDebug()<< "版本获取失败";
	return vname;
}

std::string GAThread::getConifgByKey(QString strConfigFilePath, QString strKey, partitionType divType)
{
	//读取配置库配置文件的内容
	QString strAllInfo;
	QString strRect;
	QFile data(strConfigFilePath);
	if (data.open(QFile::ReadOnly))
	{
		QTextStream out(&data);
		strAllInfo = out.readAll();
	}
	int iIndex = strAllInfo.indexOf(strKey);
	if (iIndex != -1)
	{
		strAllInfo = strAllInfo.mid(iIndex);
	}
	if (divType == EqualType)
	{
		iIndex = strAllInfo.indexOf("=");
	}
	else
	{
		iIndex = strAllInfo.indexOf(":");
	}
	int iEndIndex = strAllInfo.indexOf("\r\n");
	if (iIndex != -1)
	{
		strRect = strAllInfo.mid(iIndex + 1, iEndIndex - iIndex - 1);
	}
	//返回结果前去除首尾空格
	strRect = strRect.trimmed();
	return strRect.toStdString();
}

QString GAThread::getFileType(TimelineType type)
{
	QString strType = "Video";;
	switch (type)
	{
	case TimelineType_VideoAudio:
		strType = "Video";
		break;
	case TimelineType_Audio:
		strType = "Audio";
		break;
	case TimelineType_Image:
		strType = "Photo";
		break;
	default:
		break;
	}
	return strType;
}

QString GAThread::getFileSizeRange(QString strFilePath)
{
	QString strFileRange;
	QFileInfo fileInfo(strFilePath);
	long fileBitSize = fileInfo.size();
	//将单位转换为MB
	fileBitSize = fileBitSize >> 20;
	fileBitSize /= 50;
	if (fileBitSize <= 1)
	{
		strFileRange = "0M-50M";
		return strFileRange;
	}
	if (fileBitSize <= 2)
	{
		strFileRange = "50M-100M";
		return strFileRange;
	}
	if (fileBitSize <= 3)
	{
		strFileRange = "100M-150M";
		return strFileRange;
	}
	if (fileBitSize <= 4)
	{
		strFileRange = "150M-200M";
		return strFileRange;
	}
	if (fileBitSize <= 6)
	{
		strFileRange = "200M-300M";
		return strFileRange;
	}
	if (fileBitSize <= 10)
	{
		strFileRange = "300M-500M";
		return strFileRange;
	}
	if (fileBitSize <= 16)
	{
		strFileRange = "500M-800M";
		return strFileRange;
	}
	if (fileBitSize <= 20)
	{
		strFileRange = "800M-1G";
		return strFileRange;
	}
	if (fileBitSize <= 40)
	{
		strFileRange = "1G-2G";
		return strFileRange;
	}
	if (fileBitSize > 40)
	{
		strFileRange = ">2G";
		return strFileRange;
	}
	return strFileRange;
}

QString GAThread::getFileDurationRange(FSNLETimeline * timeline)
{
	INT64 fourcc;
	QString strTime = timeline->GetDurationString();
	int iHour = 0,iMinutes = 0,iSecond = 0;
	int iIndex = strTime.indexOf(":");
	iHour = strTime.mid(0, iIndex).toInt();
	strTime = strTime.mid(iIndex + 1);
	iIndex = strTime.indexOf(":");
	iMinutes = strTime.mid(0, iIndex).toInt();
	iIndex = strTime.indexOf(":");
	iSecond = strTime.mid(iIndex + 1).toInt();
	int iTotalMinuts = iHour * 60 + iMinutes + ((iSecond > 0) ? 1 : 0);
	timeline->GetProp()->GetInt64(NLEKey::Source::kSource_Duration, &fourcc);
	QString strFileDurationRange = "0-1min";
	if (iTotalMinuts <= 1)
	{
		strFileDurationRange = "0-1min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 3)
	{
		strFileDurationRange = "1-3min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 5)
	{
		strFileDurationRange = "3-5min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 10)
	{
		strFileDurationRange = "5-10min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 15)
	{
		strFileDurationRange = "10-15min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 30)
	{
		strFileDurationRange = "15-30min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 50)
	{
		strFileDurationRange = "30-50min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 70)
	{
		strFileDurationRange = "50-70min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 100)
	{
		strFileDurationRange = "70-100min";
		return strFileDurationRange;
	}
	if (iTotalMinuts <= 150)
	{
		strFileDurationRange = "10-150min";
		return strFileDurationRange;
	}
	if (iTotalMinuts > 150)
	{
		strFileDurationRange = ">150min";
		return strFileDurationRange;
	}
	return strFileDurationRange;
}

QString GAThread::getFileResolutionRange(QSizeF size)
{
	long lVedioResolution = size.width() * size.height();
	if (lVedioResolution <= 640 * 480)
	{
		return "0- 640 * 680";
	}
	if (lVedioResolution <= 720 * 576)
	{
		return "640 * 680 - 720 * 576";
	}
	if (lVedioResolution <= 1280 * 720)
	{
		return "0- 1280 * 720";
	}
	if (lVedioResolution <= 1920 * 1080)
	{
		return "1280 * 720 - 1920 * 1080";
	}

	if (lVedioResolution <= 3840 * 2160)
	{
		return "1920 * 1080 - 3840 * 2160";
	}
	if (lVedioResolution <= 4096 * 2160)
	{
		return "3840 * 2160- 4096 * 2160";
	}
	if (lVedioResolution > 4096 * 2160)
	{
		return ">4096 * 2160";
	}
	return "0- 640 * 680";
}

QString GAThread::getFileResolutionRange(int iArea)
{
	if (iArea <= 640 * 480)
	{
		return "<640*480";
	}
	if (iArea <= 720 * 576)
	{
		return "640*480~720*576";
	}
	if (iArea <= 1280 * 720)
	{
		return "720*576~1280*720";
	}
	if (iArea <= 1920 * 1080)
	{
		return "1280*720~1920*1080";
	}
	if (iArea <= 4096 * 2160)
	{
		return "1920*1080~4096*2160";
	}
	if (iArea > 4096 * 2160)
	{
		return ">4096*2160";
	}
	return "<640*480";
}

QDateTime GAThread::getAppInstallTime()
{
	if (m_AppInstallTime.isNull())
	{
		//读取软件的安装时间
		HKEY h;
		int iRec = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Wondershare\\Wondershare Filmora Scrn", 0, KEY_ALL_ACCESS, &h);
		if (iRec == 0)
		{
			QSettings regSetting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Wondershare\\Wondershare Filmora Scrn", QSettings::NativeFormat);
			m_AppInstallTime = regSetting.value("InstallInfo", "fail to read").toDateTime();
		}
		else
		{
			QSettings regSetting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wondershare\\Wondershare Filmora Scrn", QSettings::NativeFormat);
			m_AppInstallTime = regSetting.value("InstallInfo", "fail to read").toDateTime();
		}
	}
		return m_AppInstallTime;
}

void GAThread::start()
{
	//延迟10s启动线程
	QTimer::singleShot(10000, this, [&](){
		QThread::start();
	});
}

void GAThread::TransformSysInfo()
{
	//上传数据
	TUATracker* DataTraker = TUATracker::getInstance();
	DataTraker->SetLanguageAndLocate("en");
	DataTraker->setAppVerion("1.0.0");
	DataTraker->SendScreenName("StartUp");
	//获取系统信息
	pSysInfo allSysInfo =  getSysInfo();
	//传送SysInfo信息
	DataTraker->SendEvent("Sys info", "CPUModel", allSysInfo->strCpuMode);
	DataTraker->SendEvent("Sys info", "GPUModel", allSysInfo->strGpuMode);
	DataTraker->SendEvent("Sys info", "SYSRAM ", allSysInfo->strSysRamInfo);
	DataTraker->SendEvent("Sys info", "OSBit", allSysInfo->strOSBit);
	DataTraker->SendEvent("Sys info", "SYSResolution", allSysInfo->strSysResolution);
	DataTraker->SendEvent("Sys info", "SYSVersion ", allSysInfo->strSysVersion);
	DataTraker->SendEvent("Sys info", "SYSLanguage", allSysInfo->strSysLanguage);
	DataTraker->SendEvent("Sys info", "ProductVersion", allSysInfo->strProductVersion);
	delete allSysInfo;
	allSysInfo = NULL;
}

void GAThread::TransformStartUpInfo()
{
	TUATracker* DataTraker = TUATracker::getInstance();
	QString strTypeInfo = convertEventTypeToString(m_CurrentInfo.eventTypeInfo);
	DataTraker->SendEvent(strTypeInfo, m_CurrentInfo.strAction, m_CurrentInfo.strLabel);
	if (m_CurrentInfo.eventTypeInfo == Your_Clips)
	{
		qDebug() << "Your_Clips acion has been done ";
	}
}

void GAThread::TransformRecorderDeviceInfo()
{
	TUATracker* DataTraker = TUATracker::getInstance();
	QString strTypeInfo = convertEventTypeToString(m_CurrentInfo.eventTypeInfo);
	DataTraker->SendEvent(strTypeInfo, m_CurrentInfo.strAction, m_CurrentInfo.strLabel);
}

void GAThread::TransFromSpeedInfo()
{
	TUATracker* DataTraker = TUATracker::getInstance();
	QString strTypeInfo = convertEventTypeToString(m_CurrentInfo.eventTypeInfo);
	DataTraker->SendAppSpeed(strTypeInfo, m_CurrentInfo.strAction, m_CurrentInfo.strLabel.toInt(), "");
}

void GAThread::TransfromDimensionInfo()
{
	TUATracker* DataTraker = TUATracker::getInstance();
	QString strTypeInfo = convertEventTypeToString(m_CurrentInfo.eventTypeInfo);
	DataTraker->SetDimension(m_CurrentInfo.iValue, m_CurrentInfo.strLabel);
}

QString GAThread::getRecordDurarionRange(int iCounts)
{
	int iMinuts = iCounts / 60;
	if (iMinuts <= 1)
	{
		return "0-1 min";
	}
	if (iMinuts <= 3)
	{
		return "1-3 min";
	}
	if (iMinuts <= 5)
	{
		return "3-5 min";
	}
	if (iMinuts <= 10)
	{
		return "5-10 min";
	}
	if (iMinuts <= 15)
	{
		return "10-15 min";
	}
	if (iMinuts <= 30)
	{
		return "15-30 min";
	}
	if (iMinuts <= 50)
	{
		return "30-50 min";
	}
	if (iMinuts <= 70)
	{
		return "50-70 min";
	}
	if (iMinuts <= 100)
	{
		return "70-100 min";
	}
	if (iMinuts <= 150)
	{
		return "100-150 min";
	}
	if (iMinuts > 150)
	{
		return ">150 min";
	}
	return "unknown time range";
}

pSysInfo GAThread::getSysInfo()
{
	QString strAbi = QSysInfo::buildAbi();
	pSysInfo allSysInfo = new sysInfo;
	int iIndex = strAbi.indexOf("-");
	if (iIndex != -1)
	{
		QString strSysArgInfo = strAbi.mid(0, iIndex);
		iIndex = strSysArgInfo.indexOf("_");
		QString strBits = strSysArgInfo.mid(iIndex + 1);
		strBits.append("bit");
		//操作系统的位数
		allSysInfo->strOSBit = strBits;
	}
	//操作系统的版本
	allSysInfo->strSysVersion = getOsVersion().c_str();
	//获取当前程序的版本代号
	allSysInfo->strProductVersion = getProductVersion().c_str();
	//获取Cpu型号
	allSysInfo->strCpuMode = ReadRegValue(_T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), _T("ProcessorNameString")).c_str();
	//获取GPU型号
	allSysInfo->strGpuMode = getGpuMode().c_str();
	//获取系统语言版本
	allSysInfo->strSysLanguage = allSysInfo->strSysLanguage.fromWCharArray(getSysLanguage().c_str());
	//获取系统内存大小
	allSysInfo->strSysRamInfo = getSysRamSize().c_str();
	//获取系统分辨率
	allSysInfo->strSysResolution = getSysResolution().c_str();
	return allSysInfo;
}

BOOL GAThread::WStringToString(const std::wstring &wstr, std::string &str)
{
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	DWORD num = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), -1, NULL, 0, NULL, 0);
	str.resize(num, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), num, NULL, NULL);
	if (nResult == 0)
	{
		return FALSE;
	}
	return TRUE;
}

std::string GAThread::ReadRegValue(wchar_t* wcRegPath, wchar_t* wcRegKey)
{
	const wchar_t* cPath = wcRegPath;//注册表子键路径  
	std::string strValue = "";
	LONG lResult;//LONG型变量－反应结果  
	HKEY hKey = NULL;
	HKEY  hKeyResult = NULL;
	DWORD dwSize = 0;
	DWORD dwDataType = 0;
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, cPath, 0, KEY_QUERY_VALUE, &hKeyResult))
	{
		// 获取缓存的长度dwSize及类型dwDataType
		::RegQueryValueEx(hKeyResult, wcRegKey, 0, &dwDataType, NULL, &dwSize);
		switch (dwDataType)
		{
		case REG_MULTI_SZ:
		{
			//分配内存大小
			BYTE* lpValue = new BYTE[dwSize];
			//获取注册表中指定的键所对应的值
			LONG lRet = ::RegQueryValueEx(hKeyResult, wcRegKey, 0, &dwDataType, lpValue, &dwSize);
			strValue = (char*)lpValue;
			delete[] lpValue;
			break;
		}
		case REG_SZ:
		{
			//分配内存大小
			wchar_t* lpValue = new wchar_t[dwSize];
			memset(lpValue, 0, dwSize * sizeof(wchar_t));
			//获取注册表中指定的键所对应的值
			if (ERROR_SUCCESS == ::RegQueryValueEx(hKeyResult, wcRegKey, 0, &dwDataType, (LPBYTE)lpValue, &dwSize))
			{
				std::wstring wstrValue(lpValue);
				WStringToString(wstrValue, strValue);
			}
			delete[] lpValue;
			break;
		}
		default:
			break;
		}
	}
	//关闭注册表
	::RegCloseKey(hKeyResult);
	return strValue;
}

void GAThread::suspendThread()
{
	m_TheadSuspendMutext.lock();
}

void GAThread::resumThread()
{
	m_TheadSuspendMutext.unlock();
}

void GAThread::insertEventItem(gaInfo& eventInfo)
{
	m_SyncMutext.lock();
	m_EventList.push_back(eventInfo);
	m_SyncMutext.unlock();
}

void GAThread::insertEventItem(eventType eventTypeInfo, QString strAction, QString strLabel, int iValue /*= 0*/)
{
	gaInfo eventInfo;
	eventInfo.eventTypeInfo = eventTypeInfo;
	eventInfo.strAction = strAction;
	eventInfo.strLabel = strLabel;
	eventInfo.iValue = iValue;
	m_SyncMutext.lock();
	m_EventList.push_back(eventInfo);
	m_SyncMutext.unlock();
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

QString GAThread::convertEventTypeToString(eventType tyepInfo)
{
	QString strRect;
	switch (tyepInfo)
	{
	case Sys_Info:
		strRect = "SysInfo";
		break;
	case Start_Up:
		strRect = "StartUp";
		break;
	case Recorder_Device:
		strRect = "RecorderDevice";
		break;
	case Recorder_Widget:
		strRect = "RecorderWidget";
		break;
	case Your_Clips:
		strRect = "YourClips";
		break;
	case Import:
		strRect = "Import";
		break;
	case Import_Media:
		strRect = "ImportMedia";
		break;
	case Import_Failed:
		strRect = "ImportFailed";
		break;
	case Clip_Edit:
		strRect = "ClipEdit";
		break;
	case Annotation_Edit:
		strRect = "AnnotationEdit";
		break;
	case Export_Profile:
		strRect = "ExportProfile";
		break;
	case Export_Fail:
		strRect = "ExportFail";
		break;
	case Project_Management:
		strRect = "ProjectManagement";
		break;
	case Register:
		strRect = "Register";
		break;
	case Buy:
		strRect = "Buy";
		break;
	case App_Launch:
		strRect = "App_Launch";
		break;
	case VideoEditorLaunchFromStartup:
		strRect = "VideoEditorLaunchFromStartup";
		break;
	case ClipsSeekSpeed:
		strRect = "ClipsSeekSpeed";
		break;
	case YourClipsLaunchFromStartup:
		strRect = "YourClipsLaunchFromStartup";
		break;
	case YourClipsLaunchFromRecorder:
		strRect = "YourClipsLaunchFromRecorder";
		break;
	case ImportTimer:
		strRect = "Import";
		break;
	case ExportTimer:
		strRect = "Export";
		break;
	case RegisterTimer:
		strRect = "Register";
		break;
	case VideoEditorLaunchFromLocalFile:
		strRect = "VideoEditorLaunchFromLocalFile";
		break;
	case YourClipsLaunchFromLocalFile:
		strRect = "YourClipsLaunchFromLocalFile";
		break;
	case UserDefineDimension:
		strRect = "UserDefineDimension";
		break;
	case AppStartAndExitCount:
		strRect = "AppStart&ExitCount";
		break;
	default:
		break;
	}
	return strRect;
}

VedioInfo GAThread::insertVedioFileInfo(FSNLETimeline *timeline)
{
	VedioInfo allVedioInfo;
	//视频画面分辨率
	int tempType = timeline->GetNLETimelineType();
	//Q_ASSERT(tempType == TimelineType_Video || tempType == TimelineType_VideoAudio || tempType == TimelineType_Complex);
	QSizeF size = timeline->GetSourceSize();
	QString strDimensions = QString("%1*%2").arg(size.width()).arg(size.height());
	//获取视频文件对应的解码器
	INT fourcc = 0;
	HRESULT hr = timeline->GetProp()->GetInt(NLEKey::Codec::Video::kFourCC, &fourcc);
	allVedioInfo.strVedioCodec = sGetFourCCString(fourcc);
	insertEventItem(Import_Media, "VideoCodec", allVedioInfo.strVedioCodec);
	//帧率
	float framerate = timeline->GetSourceFramerate();
	allVedioInfo.strVedioFramerate = QString::number(framerate);
	insertEventItem(Import_Media, "VideoFramerate", allVedioInfo.strVedioFramerate);
	//比特率
	INT bitrate = 0;
	timeline->GetProp()->GetInt(NLEKey::Codec::Video::kBitrate, &bitrate);
	allVedioInfo.strVedioBitrate = QString::number(bitrate);
	insertEventItem(Import_Media, "VideoBitrate", allVedioInfo.strVedioBitrate);
	//获取文件类型
	TimelineType fileType = timeline->GetType();
	allVedioInfo.strFileType = getFileType(fileType);
	insertEventItem(Import_Media, "FileType", allVedioInfo.strFileType);
	//获取文件格式
	QFileInfo info(timeline->GetFilePath());
	QString strFileFormate = info.suffix();
	allVedioInfo.strFileFormat = strFileFormate;
	insertEventItem(Import_Media, "FileFormat", allVedioInfo.strFileFormat);
	//获取文件大小范围
	allVedioInfo.strFileSizeRange = getFileSizeRange(timeline->GetFilePath());
	insertEventItem(Import_Media, "FileSizeRange", allVedioInfo.strFileSizeRange);
	//获取视频的持续时间范围
	allVedioInfo.strFileDurationRange = getFileDurationRange(timeline);
	insertEventItem(Import_Media, "FileDurationRange", allVedioInfo.strFileDurationRange);
	//获取分辨率范围
	allVedioInfo.strVedioResolutionRange = getFileResolutionRange(size);
	insertEventItem(Import_Media, "VideoResolutionRange", allVedioInfo.strVedioResolutionRange);
	return allVedioInfo;
}

AudioInfo GAThread::insertAudioInfo(FSNLETimeline *timeline)
{
	AudioInfo allAudioInfo;
	//获取音频文件对应的解码器
	INT fourcc = 0;
	HRESULT hr = timeline->GetProp()->GetInt(NLEKey::Codec::Audio::kFourCC, &fourcc);
	allAudioInfo.strAudioCodec = sGetFourCCString(fourcc);
	insertEventItem(Import_Media, "AudioCodec", allAudioInfo.strAudioCodec);
	//获取比特率
	INT bitrate = 0;
	timeline->GetProp()->GetInt(NLEKey::Codec::Audio::kBitrate, &bitrate);
	allAudioInfo.strAudioBitrate = QString::number(bitrate);
	insertEventItem(Import_Media, "AudioBitrate", allAudioInfo.strAudioBitrate);
	//获取通道信息  
	INT iChannel = 0;
	timeline->GetProp()->GetInt(NLEKey::Source::kSource_Channels, &iChannel);
	allAudioInfo.strAudioChannel = QString::number(iChannel);
	insertEventItem(Import_Media, "AudioChannel", allAudioInfo.strAudioChannel);
	//获取样品率  
	INT iSamleRate = 0;
	timeline->GetProp()->GetInt(NLEKey::Source::kSource_SampleRate, &iSamleRate);
	allAudioInfo.strAudioSampleRate = QString::number(iSamleRate);
	insertEventItem(Import_Media, "AudioSampleRate", allAudioInfo.strAudioSampleRate);
	return allAudioInfo;
}

QString GAThread::sGetFourCCString(int fourcc)
{
#define FOURCC_VALUE(fourCC, index) ( 0xff & (fourCC >> (index * 8) ) )

	char szFourcc[5] = { 0 };
	sprintf(szFourcc, "%C%C%C%C",
		FOURCC_VALUE(fourcc, 0),
		FOURCC_VALUE(fourcc, 1),
		FOURCC_VALUE(fourcc, 2),
		FOURCC_VALUE(fourcc, 3)

		);
	return QString(szFourcc);
}

QString GAThread::getPicResolutionRange(NLESize picSize)
{
	int iResolution = picSize.Width * picSize.Height;
	QString strResolution = "<640*480";
	if (iResolution < 640 * 480)
	{
		strResolution = "<640*480";
	}
	else if (iResolution < 1280 * 960)
	{
		strResolution = "640*480-1280*960";
	}
	else if (iResolution < 1600 * 1200)
	{
		strResolution = "1280*960-1600*1200";
	}
	else if (iResolution < 2048 * 1536)
	{
		strResolution = "1600*1200-2048*1536";
	}
	else if (iResolution < 2560 * 1920)
	{
		strResolution = "2048*1536-2560*1920";
	}
	else if (iResolution < 3264 * 2448)
	{
		strResolution = "2560*1920-3264*2448";
	}
	else if (iResolution < 4208 * 3120)
	{
		strResolution = "3264*2448-4208*3120";
	}
	else if (iResolution < 4950 * 3300)
	{
		strResolution = "4208*3120-4950*3300";
	}
	else if (iResolution < 5952 * 3348)
	{
		strResolution = "4950*3300-5952*3348";
	}
	else
	{
		strResolution = ">5952＊3348";
	}
	return strResolution;
}

void GAThread::insetClipCountInfo(NLETimeline* timeline)
{
	//插入使用clip的总的数目
	int iCount = timeline->GetClipCount();
	insertEventItem(Clip_Edit, "ClipCount", QString::number(iCount));
	int iVedioClipCounts = 0;
	NLEComPtr<INLETimelineBase> timelineBase = timeline->GetSrc();
	//遍历clips
	NLEComPtr<INLEIterator> it;
	timelineBase->GetIterator(&it);
	HRESULT hr;
	hr = it->MoveToBegin();
	do
	{
		NLEComPtr<INLETimelineBase> pChild;
		it->Current()->QueryInterface(IID_INLETimelineBase, (LPVOID*)&pChild);
		//NLECommon::NLEGetProperties(*pChild);


	} while (SUCCEEDED(it->MoveToNext()));

}

void GAThread::insetClipCountInfo(TimelineManager* timelineManager)
{
	//TimelineTracks allTracks  = timelineManager->GetTracks();
	//QList<TimelineTrack*>::iterator iter = allTracks.begin();
	//for (; iter != allTracks.end();++iter)
	//{
	//      //遍历每个轨道上的clip信息
	//	Clips clipsInfos = (*iter)->GetClips();

	//}
}

QString GAThread::getExportFileRange(qint64 fileSize)
{
	if (fileSize <= 50)
	{
		return "0M~50M";
	}
	if (fileSize <= 100)
	{
		return "50M~100M";
	}
	if (fileSize <= 150)
	{
		return "100M~150M";
	}
	if (fileSize <= 200)
	{
		return "150M~200M";
	}
	if (fileSize <= 300)
	{
		return "200M~300M";
	}
	if (fileSize <= 500)
	{
		return "300M~500M";
	}
	if (fileSize <= 800)
	{
		return "500M~800M";
	}
	if (fileSize <= 1024)
	{
		return "800M~1G";
	}
	if (fileSize <= 2048)
	{
		return "1G~2G";
	}
	if (fileSize > 2048)
	{
		return ">2G";
	}
	return "0M~50M";
}

QString GAThread::getExportFileDurationRange(qint64 durationRange)
{
	int iMinutsCount = durationRange / 60;
	if (iMinutsCount <= 1)
	{
		return "0~1min";
	}
	if (iMinutsCount <= 3)
	{
		return "1~3min";
	}
	if (iMinutsCount <= 5)
	{
		return "3~5min";
	}
	if (iMinutsCount <= 10)
	{
		return "5~10min";
	}
	if (iMinutsCount <= 15)
	{
		return "10~15min";
	}
	if (iMinutsCount <= 30)
	{
		return "15~30min";
	}
	if (iMinutsCount <= 50)
	{
		return "30~50min";
	}
	if (iMinutsCount <= 70)
	{
		return "50~70min";
	}
	if (iMinutsCount <= 100)
	{
		return "70~100min";
	}
	if (iMinutsCount <= 150)
	{
		return "100~150min";
	}
	if (iMinutsCount > 150)
	{
		return ">150min";
	}
	return "0~1min";
}
