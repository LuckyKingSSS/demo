///////////////////////////////////////////////////////////////////////////
// Log.h
//==========================================================================
// 功能    ： 创建并写日志
// 模块	   ： PSB项目WSLibrary.dll模块 
// 创建时间： 2005-08-01 
// 创建者  ： 付玮
// 修订    ： 
//==========================================================================

#pragma once


#ifdef __cplusplus
extern "C"
{
#endif

void __stdcall SetGpuEncodeType(int nIsUseGpuEncode, int nGpuType,int nUseSystemEncode);
void __stdcall GetGpuEncodeType(int* nIsUseGpuEncode, int* nGpuType, int* nUseSystemEncode);

void __stdcall SetAppPath(const wchar_t* pAppPath); //  设置应用程序路径 最后一个字符非斜线

void __stdcall GetAppPath(wchar_t* pAppPath, int len); 

void __stdcall SetAppResourcePath(const wchar_t* pAppResourcePath,int len);//  设置应用程序资源存放路径例如XML.图片等
void __stdcall GetAppResourcePath(wchar_t* pAppResourcePath,int len);

void __stdcall SetFontCachePath(const wchar_t* pFontCachePath); // 设置字体缓存路径

void __stdcall GetFontCachePath(wchar_t* pFontCachePath, int len);

void __stdcall  SetDocumentPath(const wchar_t* path);

void __stdcall  GetDocumentPath(wchar_t* path, int len);

void __stdcall SetAppTmpPath(const wchar_t* pAppTempPath,int len); //  设置应用程序临时路径 最后一个字符非斜线 最大长度1024 mac下
void __stdcall GetAppTmpPath(wchar_t* pAppTempPath); 
void __stdcall SetWinAtiVideoCardType(int ntype);
int __stdcall GetWinAtiVideoCardType(void);
void __stdcall SetVideoCardType(int ntype); //
void __stdcall GetVideoCardType(int* nType); 

// 设置日志文件名，不包括路径 默认为"log.txt" 全局日志均写入到{app}\log目录
// bUseHDInfo - 是否包含硬件信息
// bClearLog - 是否清空旧信息
// WSLogInit是必须调用的
void __stdcall WSLogInit(const wchar_t* pFileName = 0, int bUseHDInfo = TRUE, int bClearLog = FALSE, DWORD dwReserved = 0);

#ifndef WIN32
void __stdcall WSLogInitEx(const wchar_t* pFilePath,const wchar_t* pFileName, int bUseHDInfo, int bClearLog, DWORD dwReserved);
#endif
	
void __stdcall WSLogUninit();

void __stdcall WSLog(const wchar_t* pText);//外调用写日志函数(C++及Delphi)

void __cdecl WSLogEx(const wchar_t* lpszFormat, ...);//外调用写可变参数的日志函数(仅C++可用)

void __stdcall WSOutput(const wchar_t* lpszFormat); // 输出调试信息 使用XTraceMonitor.exe查看

void __stdcall WSOutputEx(COLORREF crText, const wchar_t* lpszFormat); // 输出调试信息 crText为文本颜色

void __stdcall WSOutputImg(LPBITMAPINFO lpbi, LPVOID lpBits); //输出图像


// 以下函数可以使用 LogFile类代替
typedef void* HLOGFILE;
//创建Log文件 pPathName日志文件路径名 bUseHDInfo为是否需要包含硬件信息 bClearLog是否要清空原来的Log文件 dwReserve为保留字
HLOGFILE __stdcall LogFileCreate(const wchar_t* pPathName, int bUseHDInfo, int bClearLog, DWORD dwReserve);

void __stdcall LogFileDestroy(HLOGFILE hLog);//销毁Log文件

void __stdcall LogFileLog(HLOGFILE hLog, const wchar_t* pText);//写日志文件

const wchar_t* __stdcall ReadLogFileBuffer(const wchar_t* pFileName); //读取文件，返回字符串指针 

void __stdcall ReleaseLogFileBuffer(const wchar_t* pLogBuffer);//释放所返回字符串指针

#ifdef WIN32
LONG __stdcall SEHExceptionFilter(LPEXCEPTION_POINTERS lpEr);
#endif

void __stdcall CreateReportWithoutCrash();

typedef LONG  (__stdcall *BUGSPLAT_CALLBACK)();

void __stdcall SetBugSplatCallBack(BUGSPLAT_CALLBACK fn);

typedef LONG  (__stdcall *MULTI_CALLBACK)(void* pUserObj, UINT uMsg, WPARAM wParam, LPARAM lParam);

void __stdcall SetErrorCallBack(MULTI_CALLBACK fnCallback);

void __stdcall WS_Log_Error_CallBackToUser();

#define TRY_BEGIN  try{         


#define TRY_END(return_value)   }		\
	catch(...)                          \
{                               \
	CreateReportWithoutCrash();	\
	WS_Log_Error_CallBackToUser();\
	return return_value;        \
}                              \

#ifdef __cplusplus
}
#endif



#ifdef NO_LOG

#define WSLOG
#define WSLOGEX

#else

#define WSLOG WSLog
#define WSLOGEX WSLogEx

#endif






