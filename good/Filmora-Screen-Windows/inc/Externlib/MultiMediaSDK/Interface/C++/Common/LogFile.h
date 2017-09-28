///////////////////////////////////////////////////////////////////////////
// LogFile.h
//==========================================================================
// 功能    ： 个人创建并写日志文件
// 模块	   ： PSB项目WSLibrary.dll模块 
// 创建时间： 2005-08-01 
// 创建者  ： 付玮
// 修订    ： 
//==========================================================================


#ifndef _LOGFILE_H
#define _LOGFILE_H


#ifdef WIN32

#ifdef LOG_EXPORTS
	#define LOG_API __declspec(dllexport)
#else
	#define LOG_API __declspec(dllimport)
#endif

#else

#define LOG_API

#endif

class LogFileImpl;
class LOG_API LogFile
{
public:
	//设定日志文件名szFileName 及是否要清空原来的Log文件bClearLog，是否需要包含硬件信息bUseHDInfo 和保留字dwReserve。
	LogFile(const wchar_t *szFileName = 0, int bUseHDInfo = TRUE, int bClearLog = FALSE, DWORD dwReserve = 0);
	virtual ~LogFile();//析构函数

	int Open(const wchar_t *szFileName = 0, int bUseHDInfo = TRUE, int bClearLog = FALSE, DWORD dwReserve = 0); //设定日志文件名
	int IsOpen() const; // 是否成功创建文件

	void Log(const wchar_t *szText);//调用写日志,支持多线程

	void Close();

private:

	LogFileImpl*	m_pImpl;

};


#endif
