unit
	Log;
    // 提供日志记录 功能
    
interface

uses
    Windows;

type
   HLOGFILE = pointer;
   
// 设置日志文件名，不包括路径 默认为"log.txt" 全局日志均写入到{app}\log目录
// bUseHDInfo - 是否包含硬件信息
// bClearLog - 是否清空旧信息
// WSLogInit是必须调用的
//void __stdcall WSLogInit(const wchar_t* pFileName, BOOL bUseHDInfo = TRUE, BOOL bClearLog = FALSE, DWORD dwReserved = 0);
procedure WSLogInit(const pFileName: PWideChar; bUseHDInfo : BOOL = TRUE; bClearLog : BOOL = FALSE; dwReserved : DWORD = 0); stdcall;

//void __stdcall WSLogUninit();
procedure WSLogUninit(); stdcall;
// 功能： 记录软件日志
// 返回:  无返回值
//void WSLog(const wchar_t* pText);
procedure WSLog(const pText: PWideChar); stdcall;

//void __stdcall WSOutput(const wchar_t* lpszFormat); // 输出调试信息 使用XTraceMonitor.exe查看
procedure WSOutput(const lpszFormat: PWideChar); stdcall;

//void __stdcall WSOutputEx(COLORREF crText, const wchar_t* lpszFormat); // 输出调试信息 crText为文本颜色
procedure WSOutputEx(crText : COLORREF; const lpszFormat: PWideChar); stdcall;

//void __stdcall WSOutputImg(LPBITMAPINFO lpbi, LPVOID lpBits); //输出图像
procedure WSOutputImg(lpbi: PBitmapInfo; lpBits: Pointer); stdcall;

//创建Log文件 pFileName为创建位置 bUseHDInfo为是否需要包含硬件信息 bClearLog是否要清空原来的Log文件 dwReserve为保留字
//HLOGFILE __stdcall LogFileCreate(const wchar_t* pFileName, BOOL bUseHDInfo, BOOL bClearLog, DWORD dwReserve);
function LogFileCreate(const pFileName : PWideChar; bUseHDInfo : BOOL; bClearLog : BOOL; dwReserve : DWORD) : HLOGFILE; stdcall;

//void __stdcall LogFileDestroy(HLOG hLog);//销毁Log文件
procedure LogFileDestroy(hLog : HLOGFILE); stdcall;

//void __stdcall LogFileLog(HLOG hLog, const wchar_t* pText);//写日志文件
procedure LogFileLog(hLog : HLOGFILE; const pText : PWideChar); stdcall;

//const wchar_t* __stdcall ReadLogFileBuffer(const wchar_t* pFileName); //读取文件，返回字符串指针
function ReadLogFileBuffer(const pFileName: PWideChar): PWideChar; stdcall;

//void __stdcall ReleaseLogFileBuffer(const wchar_t* pLogBuffer);//释放所返回字符串指针
procedure ReleaseLogFileBuffer(const pLogBuffer: PWideChar); stdcall;


implementation

const
    DLLNAME = 'WS_Log.dll';

procedure WSLogInit						  ; external DLLNAME Name 'WSLogInit';
procedure WSLogUninit           ; external DLLNAME Name 'WSLogUninit';
procedure WSLog						      ; external DLLNAME Name 'WSLog';
procedure WSOutput				      ; external DLLNAME Name 'WSOutput';
procedure WSOutputEx			      ; external DLLNAME Name 'WSOutputEx';
procedure WSOutputImg           ; external DLLNAME Name 'WSOutputImg';

function LogFileCreate		      ; external DLLNAME Name 'LogFileCreate';
procedure LogFileDestroy	      ; external DLLNAME Name 'LogFileDestroy';
procedure LogFileLog			      ; external DLLNAME Name 'LogFileLog';

function ReadLogFileBuffer      ; external DLLNAME Name 'ReadLogFileBuffer';
procedure ReleaseLogFileBuffer  ; external DLLNAME Name 'ReleaseLogFileBuffer';

end.