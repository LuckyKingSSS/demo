unit WSLogFile; // 提供日志记录 功能

interface

uses Windows, SysUtils;

{$IFNDEF UNICODE}
type
  UnicodeString = WideString;
{$ENDIF}

{$WARN SYMBOL_PLATFORM OFF}

type
  HLOGFILE = pointer;

// 如果默认的日志已经在使用，则修改文件指向新的日志，避免多个进程同时写一个日志
procedure WSLogInitUnUse(const AFileName: UnicodeString; bUseHDInfo : BOOL = TRUE; bClearLog : BOOL = FALSE; dwReserved : DWORD = 0); overload;
// 设置日志文件名，不包括路径 默认为"log.txt" 全局日志均写入到{app}\log目录
// bUseHDInfo - 是否包含硬件信息
// bClearLog - 是否清空旧信息
// WSLogInit不是必须调用的
procedure WSLogInit(const pFileName: PWideChar; bUseHDInfo : BOOL = TRUE; bClearLog : BOOL = FALSE; dwReserved : DWORD = 0); stdcall;
// 功能： 记录软件日志
// 返回:  无返回值
procedure WSLog(const pText: PWideChar); stdcall; overload;
procedure WSLog(const pText: WideString); overload;
procedure WSLog(const pText: string); overload;
// 输出调试信息 使用XTraceMonitor.exe查看
procedure WSOutput(const lpszFormat: PWideChar); stdcall;
// 输出调试信息 crText为文本颜色
procedure WSOutputEx(crText : COLORREF; const lpszFormat: PWideChar); stdcall;
// 输出图像
procedure WSOutputImg(lpbi: PBitmapInfo; lpBits: Pointer); stdcall;
//创建Log文件 pFileName为创建位置 bUseHDInfo为是否需要包含硬件信息 bClearLog是否要清空原来的Log文件 dwReserve为保留字
function LogFileCreate(const pFileName : PWideChar; bUseHDInfo : BOOL; bClearLog : BOOL; dwReserve : DWORD) : HLOGFILE; stdcall;
//销毁Log文件
procedure LogFileDestroy(hLog : HLOGFILE); stdcall;
// 写日志文件
procedure LogFileLog(hLog : HLOGFILE; const pText : PWideChar); stdcall;
// 读取文件，返回字符串指针
function ReadLogFileBuffer(const pFileName: PWideChar): PWideChar; stdcall;
// 释放所返回字符串指针
procedure ReleaseLogFileBuffer(const pLogBuffer: PWideChar); stdcall;
// Base64 编解码
// 编码后的长度一般比原文多占用1/3的存储空间，请保证base64code有足够的空间
//int __stdcall WSBase64Encode(char * base64code, const unsigned char * src, int src_len);
function WSBase64Encode(base64code : PAnsiChar; const src : Pointer; src_len : Integer) : Integer; stdcall;

//int __stdcall WSBase64Decode(unsigned char * buf, const char * base64code, int src_len = -1);
function WSBase64Decode(buf : Pointer; const base64code : PAnsiChar; src_len : Integer = -1) : Integer; stdcall;

function StartCheckMD5(const pFileName: PWideChar; const pMD5: PChar): BOOL; stdcall;

implementation

uses WideFileUtils;

const
  DLLNAME = 'WS_Log.dll';
  SLogFileDir = 'Log';

procedure WSLogInit             ; external DLLNAME Name 'WSLogInit';
procedure WSLog(const pText: PWideChar); stdcall;               external DLLNAME Name 'WSLog';
procedure WSOutput              ; external DLLNAME Name 'WSOutput';
procedure WSOutputEx            ; external DLLNAME Name 'WSOutputEx';
procedure WSOutputImg           ; external DLLNAME Name 'WSOutputImg';

function LogFileCreate          ; external DLLNAME Name 'LogFileCreate';
procedure LogFileDestroy        ; external DLLNAME Name 'LogFileDestroy';
procedure LogFileLog            ; external DLLNAME Name 'LogFileLog';

function ReadLogFileBuffer      ; external DLLNAME Name 'ReadLogFileBuffer';
procedure ReleaseLogFileBuffer  ; external DLLNAME Name 'ReleaseLogFileBuffer';
function  WSBase64Encode        ; external DLLNAME Name 'WSBase64Encode';
function  WSBase64Decode        ; external DLLNAME Name 'WSBase64Decode';

function StartCheckMD5          ; external DLLNAME Index 111;

procedure WSLogInitUnUse(const AFileName: UnicodeString; bUseHDInfo : BOOL = TRUE; bClearLog : BOOL = FALSE; dwReserved : DWORD = 0); overload;
var
  n: Integer;
  szLogExt,
  szLogFile, szNewFile, szFullLogDir: UnicodeString;
begin
  n:=0;
  szLogFile:=AFileName;
  if szNewFile='' then
    szNewFile:='Log.txt';
  szLogFile:=WideChangeFileExt(szNewFile, '');
  szLogExt:=WideExtractFileExt(szNewFile);
  if szLogExt='' then
    szLogExt:='.txt';
  while WideIsFileLocked(WideExpandAppDirFileName(SLogFileDir, szNewFile, szFullLogDir)) do
  begin
    Inc(n);
    szNewFile:=WideFormat('%s(%d)%s', [szLogFile, n, szLogExt]);
  end;
  WSLogInit(PWideChar(szNewFile), bUseHDInfo, bClearLog, dwReserved);
end;

procedure WSLog(const pText: WideString); overload;
begin
  WSLog(PWideChar(pText));
end;

procedure WSLog(const pText: string); overload;
begin
  WSLog(PWideChar(WideString(pText)));
end;

end.
