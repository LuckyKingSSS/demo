
#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#ifndef _WS_LOG_H_E414DE25_D129_4c3a_8012_9D15A8DF5DA2
#define _WS_LOG_H_E414DE25_D129_4c3a_8012_9D15A8DF5DA2


#ifdef NO_LOG
	#define WSLOG
	#define WSLOGEX
#else
	#define WSLOG	WSLog
	#define WSLOGEX WSLogEx
#endif


// 设置日志文件名，不包括路径 默认为"log.txt" 全局日志均写入到{app}\log目录
// bUseHDInfo - 是否包含硬件信息
// bClearLog - 是否清空旧信息
// WSLogInit是必须调用的
inline void WSLogInit(const wchar_t* pFileName = 0, BOOL bUseHDInfo = TRUE, BOOL bClearLog = FALSE, DWORD dwReserved = 0);

inline void WSLogUninit();

//inline void WSLog(const wchar_t* pText);				//写日志函数(C++及Delphi)

inline void WSLog(const wchar_t* lpszFormat, ...);		//写可变参数的日志函数(C++及Delphi)

inline void WSLogEx(const wchar_t* lpszFormat, ...);	//写可变参数的日志函数(仅C++可用)



// 以下函数可以使用 LogFile类代替
typedef void* HLOGFILE;

//创建Log文件 pPathName日志文件路径名 bUseHDInfo为是否需要包含硬件信息 bClearLog是否要清空原来的Log文件 dwReserve为保留字
inline HLOGFILE LogFileCreate(const wchar_t* pPathName, BOOL bUseHDInfo, BOOL bClearLog, DWORD dwReserve);

inline void LogFileDestroy(HLOGFILE hLog);						//销毁Log文件

//inline void LogFileLog(HLOGFILE hLog, const wchar_t* pText);	//写日志文件

inline void LogFileLog(HLOGFILE hLog, const wchar_t* lpszFormat, ...);	//写日志文件（可变参数）

//const wchar_t* __stdcall ReadLogFileBuffer(const wchar_t* pFileName); //读取文件，返回字符串指针 
//
//void __stdcall ReleaseLogFileBuffer(const wchar_t* pLogBuffer);//释放所返回字符串指针



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DllLoad
{
public:
	DllLoad(LPCTSTR lpszName)
	{
		m_hModule = ::LoadLibrary(lpszName);
	}

	~DllLoad()
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}

	operator  HMODULE() 
	{ 
		return m_hModule;
	}

protected:

	HMODULE m_hModule;
};


struct WSLOGPROC
{
	void (__stdcall* fnInit		)(const wchar_t*, BOOL, BOOL, DWORD);
	void (__stdcall* fnUninit	)();
	void (__stdcall* fnLog		)(const wchar_t*);	

	struct
	{
		HLOGFILE	(__stdcall* fnCreate	)(const wchar_t*, BOOL, BOOL, DWORD);
		void		(__stdcall* fnDestroy	)(HLOGFILE);	
		void		(__stdcall* fnLog		)(HLOGFILE, const wchar_t*);	
	} LogFile;
};


inline WSLOGPROC* GetLogProc()
{
	static DllLoad logdl(L"WS_Log.dll");
	static WSLOGPROC log = 
	{
		( void (__stdcall*)(const wchar_t*, BOOL, BOOL, DWORD) )	::GetProcAddress(HMODULE(logdl), "WSLogInit"),
		( void (__stdcall*)() )										::GetProcAddress(HMODULE(logdl), "WSLogUninit"),
		( void (__stdcall*)(const wchar_t*) )						::GetProcAddress(HMODULE(logdl), "WSLog"),

		{
			( HLOGFILE	(__stdcall*)(const wchar_t*, BOOL, BOOL, DWORD) )	::GetProcAddress(HMODULE(logdl), "LogFileCreate"),
			( void		(__stdcall*)(HLOGFILE) )							::GetProcAddress(HMODULE(logdl), "LogFileDestroy"),
			( void		(__stdcall*)(HLOGFILE, const wchar_t*) )			::GetProcAddress(HMODULE(logdl), "LogFileLog"),
		}
	};

	return &log;
}

inline void WSLogInit(const wchar_t* pFileName, BOOL bUseHDInfo, BOOL bClearLog, DWORD dwReserved)
{
	WSLOGPROC* log = GetLogProc();
	if (log && log->fnInit)
	{
		log->fnInit(pFileName, bUseHDInfo, bClearLog, dwReserved);
	}
}

inline void WSLogUninit()
{
	WSLOGPROC* log = GetLogProc();
	if (log && log->fnUninit)
	{
		log->fnUninit();
	}
}

inline void WSLog(const wchar_t* lpszFormat, ...)
{
	WSLOGPROC* log = GetLogProc();
	if (log && log->fnLog)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf = 0;
		wchar_t szBuffer[1024];
		nBuf = vswprintf(szBuffer, 1024 - 1, lpszFormat, args);
		if (nBuf) 
			log->fnLog(szBuffer);

		va_end(args);
	}
}

inline void WSLogEx(const wchar_t* lpszFormat, ...)
{
	WSLOGPROC* log = GetLogProc();
	if (log && log->fnLog)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf = 0;
		wchar_t szBuffer[1024];
		nBuf = vswprintf(szBuffer, 1024 - 1, lpszFormat, args);
		if (nBuf) 
			log->fnLog(szBuffer);

		va_end(args);
	}
}

inline HLOGFILE LogFileCreate(const wchar_t* pPathName, BOOL bUseHDInfo, BOOL bClearLog, DWORD dwReserve)
{
	WSLOGPROC* log = GetLogProc();
	if (log && log->LogFile.fnCreate)
	{
		return log->LogFile.fnCreate(pPathName, bUseHDInfo, bClearLog, dwReserve);
	}

	return 0;
}

inline void LogFileDestroy(HLOGFILE hLog)
{
	WSLOGPROC* log = GetLogProc();
	if (log && log->LogFile.fnDestroy)
	{
		log->LogFile.fnDestroy(hLog);
	}
}

inline void LogFileLog(HLOGFILE hLog, const wchar_t* lpszFormat, ...)
{
	WSLOGPROC* log = GetLogProc();
	if (log && log->LogFile.fnLog)
	{
		va_list args;
		va_start(args, lpszFormat);

		int nBuf = 0;
		wchar_t szBuffer[1024];
		nBuf = vswprintf(szBuffer, 1024 - 1, lpszFormat, args);
		if (nBuf) 
			log->LogFile.fnLog(hLog, szBuffer);

		va_end(args);
	}
}

#endif



#if 0
void __stdcall SetAppPath(const wchar_t* pAppPath); //  设置应用程序路径 最后一个字符非斜线

void __stdcall GetAppPath(wchar_t* pAppPath, int len); 


// 设置日志文件名，不包括路径 默认为"log.txt" 全局日志均写入到{app}\log目录
// bUseHDInfo - 是否包含硬件信息
// bClearLog - 是否清空旧信息
// WSLogInit是必须调用的
void __stdcall WSLogInit(const wchar_t* pFileName = 0, BOOL bUseHDInfo = TRUE, BOOL bClearLog = FALSE, DWORD dwReserved = 0);

void __stdcall WSLogUninit();

void __stdcall WSLog(const wchar_t* pText);//外调用写日志函数(C++及Delphi)

void WSLogEx(const wchar_t* lpszFormat, ...);//外调用写可变参数的日志函数(仅C++可用)

void __stdcall WSOutput(const wchar_t* lpszFormat); // 输出调试信息 使用XTraceMonitor.exe查看

void __stdcall WSOutputEx(COLORREF crText, const wchar_t* lpszFormat); // 输出调试信息 crText为文本颜色

void __stdcall WSOutputImg(LPBITMAPINFO lpbi, LPVOID lpBits); //输出图像


// 以下函数可以使用 LogFile类代替
typedef void* HLOGFILE;
//创建Log文件 pPathName日志文件路径名 bUseHDInfo为是否需要包含硬件信息 bClearLog是否要清空原来的Log文件 dwReserve为保留字
HLOGFILE __stdcall LogFileCreate(const wchar_t* pPathName, BOOL bUseHDInfo, BOOL bClearLog, DWORD dwReserve);

void __stdcall LogFileDestroy(HLOGFILE hLog);//销毁Log文件

void __stdcall LogFileLog(HLOGFILE hLog, const wchar_t* pText);//写日志文件

const wchar_t* __stdcall ReadLogFileBuffer(const wchar_t* pFileName); //读取文件，返回字符串指针 

void __stdcall ReleaseLogFileBuffer(const wchar_t* pLogBuffer);//释放所返回字符串指针

#endif

