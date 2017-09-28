unit
	Config;
    // 提供配置记录 功能

//注意:
// 1.全局配置文件类一定要先建立,再使用,最后要记得释放;
//   目前只能初始化一次；
// 2.个人配置文件类建立时会对文件进行核对,存在该文件就打开,不存在则创建;
//   读文件时也会这样,所以读时大家要注意打开的文件名是否正确,否则将一无所获;
// 3.释放时会自动保存,大家可以放心使用释放功能.
// 4.大家注意写文件时有大小写之分哦!
    
interface

uses
    Windows;
type
   HCONFIGFILE = pointer;

// 初始化全局配置，应用程序初始化时调用
// pFileName - 全局配置文件名，不包括路径 默认为"Config.xml" 全局配置均写入到{app}目录
//BOOL __stdcall InitGlobalConfig(const wchar_t* pFileName = L"Config.xml", const wchar_t* pRootNode = L"config_root", const wchar_t* pVersion = L"1.0", DWORD dwResered = 0);
function InitGlobalConfig(pFileName : PWideChar = nil; pRootNode: PWideChar = nil; pVersion: PWideChar = nil; dwReserved : DWORD = 0) : BOOL; stdcall;

//void __stdcall ShutdownGlobalConfig();	// 关闭全局配置，应用程序退出时调用自动Flush
procedure ShutdownGlobalConfig(); stdcall;

//void __stdcall WSSetConfigStr(const wchar_t*, const wchar_t*);//设置配置 传入字符串
procedure WSSetConfigStr(const pNodeName : PWideChar; const pContent : PWideChar); stdcall;

//void __stdcall WSSetConfigInt(const wchar_t*, int);//设置配置  传入整型类型值
procedure WSSetConfigInt(const pNodeName : PWideChar; nValue : Integer); stdcall;

//void __stdcall WSSetConfigDbl(const wchar_t*, double);//设置配置  传入浮点类型值
procedure WSSetConfigDbl(const pNodeName : PWideChar; dValue : Double); stdcall;

//void __stdcall WSSetAttribute(const wchar_t* , const wchar_t* lpAttribName, const wchar_t* lpAttribValue);
procedure WSSetConfigAttribute(const pNodeName : PWideChar; const lpAttribName : PWideChar; const lpAttribValue : PWideChar); stdcall;

//BOOL __stdcall WSConfigFlush();//写配置文件
function WSConfigFlush() : BOOL; stdcall;

//BOOL __stdcall WSGetConfigStrEx(const wchar_t*, const wchar_t*, int);//读配置文件，内容为字符型 ,Delphi使用
function WSGetConfigStrEx(const pNodeName : PWideChar; pstrValue : PWideChar; nBufLen : Integer) : BOOL; stdcall;

//const wchar_t* __stdcall WSGetConfigStrPtr(const wchar_t*);//读配置文件， 内容为字符型  如果配置不存在则返回0
function WSGetConfigStrPtr(const pNodeName : PWideChar) : PWideChar; stdcall;


//BOOL __stdcall WSGetConfigInt(const wchar_t*, int&);//读配置文件  读出整型值
function WSGetConfigInt(const pNdeName : PWideChar; pnValue : PInteger) : BOOL; stdcall;

//BOOL __stdcall WSGetConfigDbl(const wchar_t*, double&);//读配置文件  读出浮点值
function WSGetConfigDbl(const pNodeName : PWideChar; pdValue : PDouble) : BOOL; stdcall;

//const wchar_t* WSGetConfigAttribute(const wchar_t*, const wchar_t* lpAttribName);
function WSGetConfigAttribute(const pNodeName : PWideChar; const lpAttribName : PWideChar) : PWideChar; stdcall;


//创建或打开配置文件，pFileName为创建或打开文件路径名 不会自动Flush
//HCONFIGFILE __stdcall ConfigFileInit(const wchar_t* pPathName, const wchar_t* pRootNode = L"config_root", const wchar_t* pVersion = L"1.0", DWORD dwResered = 0);
function ConfigFileInit(const pFileName : PWideChar; pRootNode: PWideChar = nil; pVersion: PWideChar = nil; dwReserved : DWORD = 0) : HCONFIGFILE; stdcall;

//void __stdcall ConfigFileDestroy(HCONFIGFILE hconfig);//关闭Config文件 不会自动Flush
procedure ConfigFileDestroy(hconfig : HCONFIGFILE); stdcall;

//void __stdcall ConfigSetStr(HCONFIGFILE hconfig, const wchar_t*, const wchar_t*);//设置配置， 内容为字符型
procedure ConfigSetStr(hconfig : HCONFIGFILE; const pNodeName : PWideChar; const pValue : PWideChar); stdcall;

//const wchar_t* __stdcall ConfigGetStrPtr(HCONFIGFILE hconfig, const wchar_t*);//读配置文件， 内容为字符型  如果配置不存在则返回0
function ConfigGetStrPtr(hconfig : HCONFIGFILE; const pNodeName : PWideChar) : PWideChar; stdcall;


//void __stdcall ConfigSetInt(HCONFIGFILE hconfig, const wchar_t*, int);//设置配置，内容为整型
procedure ConfigSetInt(hconfig : HCONFIGFILE; const pNodeName : PWideChar; nValue : Integer); stdcall;

//void __stdcall ConfigSetDbl(HCONFIGFILE hconfig, const wchar_t*, double);//设置配置，  内容为浮点型
procedure ConfigSetDbl(hconfig : HCONFIGFILE; const pNodeName : PWideChar; dValue : Double); stdcall;

//void __stdcall ConfigSetAttribute(HCONFIGFILE hconfig, const wchar_t* , const wchar_t* lpAttribName, const wchar_t* lpAttribValue);
procedure ConfigSetAttribute(hconfig : HCONFIGFILE; const pNodeName : PWideChar; const lpAttribName : PWideChar; const lpAttribValue : PWideChar); stdcall;

//BOOL __stdcall ConfigFlush(HCONFIGFILE hconfig);//写配置文件
function ConfigFlush(hconfig : HCONFIGFILE) : BOOL; stdcall;

//BOOL __stdcall ConfigGetStrEx(HCONFIGFILE hconfig, const wchar_t*, const wchar_t*, int)；//读配置文件， 内容为字符型,Delphi使用
function ConfigGetStrEx(hconfig : HCONFIGFILE; const pNodeName : PWideChar; const pValue : PWideChar; nBufLen : Integer) : BOOL; stdcall;

//BOOL __stdcall ConfigGetInt(HCONFIGFILE hconfig, const wchar_t*, int*);//读配置文件， 内容为整型
function ConfigGetInt(hconfig : HCONFIGFILE; const pNodeName : PWideChar; pnValue : PInteger) : BOOL; stdcall;

//BOOL __stdcall ConfigGetDbl(HCONFIGFILE hconfig, const wchar_t*, double*);//读配置文件，  内容为浮点型
function ConfigGetDbl(hconfig : HCONFIGFILE; const pNodeName : PWideChar; pdValue : PDouble) : BOOL; stdcall;

//const wchar_t* ConfigGetAttribute(HCONFIGFILE hconfig, const wchar_t*, const wchar_t* lpAttribName);
function ConfigGetAttribute(hconfig : HCONFIGFILE; const pNodeName : PWideChar; const lpAttribName : PWideChar) : PWideChar; stdcall;


// Base64 编解码
// 编码后的长度一般比原文多占用1/3的存储空间，请保证base64code有足够的空间
//int __stdcall WSBase64Encode(char * base64code, const unsigned char * src, int src_len); 
function WSBase64Encode(base64code : PChar; const src : Pointer; src_len : Integer) : Integer; stdcall;

//int __stdcall WSBase64Decode(unsigned char * buf, const char * base64code, int src_len = -1);
function WSBase64Decode(buf : Pointer; const base64code : PChar; src_len : Integer = -1) : Integer; stdcall;



implementation

const
    DLLNAME = 'WS_Log.dll';

function  InitGlobalConfig	    ; external DLLNAME Name 'InitGlobalConfig';
procedure ShutdownGlobalConfig	; external DLLNAME Name 'ShutdownGlobalConfig';
procedure WSSetConfigStr        ; external DLLNAME Name 'WSSetConfigStr';
procedure WSSetConfigInt	      ; external DLLNAME Name 'WSSetConfigInt';
procedure WSSetConfigDbl	      ; external DLLNAME Name 'WSSetConfigDbl';
function  WSConfigFlush		      ; external DLLNAME Name 'WSConfigFlush';
function  WSGetConfigStrEx		  ; external DLLNAME Name 'WSGetConfigStrEx';
function  WSGetConfigInt		    ; external DLLNAME Name 'WSGetConfigInt';
function  WSGetConfigDbl		    ; external DLLNAME Name 'WSGetConfigDbl';
function  WSGetConfigStrPtr     ; external DLLNAME Name 'WSGetConfigStrPtr';
function  ConfigFileInit		    ; external DLLNAME Name 'ConfigFileInit';
procedure ConfigFileDestroy		  ; external DLLNAME Name 'ConfigFileDestroy';
procedure ConfigSetStr	        ; external DLLNAME Name 'ConfigSetStr';
procedure ConfigSetInt	        ; external DLLNAME Name 'ConfigSetInt';
procedure ConfigSetDbl	        ; external DLLNAME Name 'ConfigSetDbl';
function  ConfigGetStrEx		    ; external DLLNAME Name 'ConfigGetStrEx';
function  ConfigGetInt		      ; external DLLNAME Name 'ConfigGetInt';
function  ConfigGetDbl		      ; external DLLNAME Name 'ConfigGetDbl';
function  ConfigFlush           ; external DLLNAME Name 'ConfigFlush';
function  ConfigGetStrPtr       ; external DLLNAME Name 'ConfigGetStrPtr';

function  WSBase64Encode		    ; external DLLNAME Name 'WSBase64Encode';
function  WSBase64Decode        ; external DLLNAME Name 'WSBase64Decode';

function  WSGetConfigAttribute     ; external DLLNAME Name 'WSGetConfigAttribute';
function  ConfigGetAttribute       ; external DLLNAME Name 'ConfigGetAttribute';
procedure  WSSetConfigAttribute     ; external DLLNAME Name 'WSSetConfigAttribute';
procedure  ConfigSetAttribute       ; external DLLNAME Name 'ConfigSetAttribute';

end.