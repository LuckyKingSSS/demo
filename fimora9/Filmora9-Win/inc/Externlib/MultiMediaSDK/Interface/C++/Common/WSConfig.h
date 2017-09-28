///////////////////////////////////////////////////////////////////////////
// Config.h
//==========================================================================
// 功能    ： 创建并读写配置文件
// 模块	   ： PSB项目WSLibrary.dll模块 
// 创建时间： 2005-08-10 
// 创建者  ： 付玮
// 修订    ： 
//==========================================================================

///////////////////////////////////////////////////////////////////////////
//注意:
// 1.全局配置文件类一定要先初始化,再使用,最后要记得释放;
//   目前只能初始化一次
// 2.释放时会自动保存,大家可以放心使用释放功能;
// 3.大家注意写文件时有大小写之分哦!


#pragma once

#include <string>
using namespace std;

#ifdef WIN32
#ifdef __cplusplus
extern "C"
{
#endif
#endif

// 初始化全局配置，应用程序初始化时调用
// pFileName - 全局配置文件名，不包括路径 默认为"Config.xml" 全局配置均写入到{app}目录
int __stdcall InitGlobalConfig(const wchar_t* pFileName = L"Config.xml", const wchar_t* pRootNode = L"config_root", const wchar_t* pVersion = L"1.0", DWORD dwResered = 0);		
void __stdcall ShutdownGlobalConfig();	// 关闭全局配置，应用程序退出时调用 自动Flush


void __stdcall WSSetConfigStr(const wchar_t*, const wchar_t*);//设置配置, 内容为字符型

void __stdcall WSSetConfigInt(const wchar_t*, int);//设置配置， 内容为整型

void __stdcall WSSetConfigDbl(const wchar_t*, double);//设置配置， 内容为浮点型

void __stdcall WSSetAttribute(const wchar_t* , const wchar_t* lpAttribName, const wchar_t* lpAttribValue);

int __stdcall WSConfigFlush();//写配置文件


int __stdcall WSGetConfigStr(const wchar_t*, wstring&);//读配置文件， 内容为字符型

const wchar_t* __stdcall WSGetConfigStrPtr(const wchar_t*);//读配置文件， 内容为字符型 如果配置不存在则返回0

int __stdcall WSGetConfigStrEx(const wchar_t*, const wchar_t*, int);//读配置文件， 内容为字符型

int __stdcall WSGetConfigInt(const wchar_t*, int*);//读配置文件， 内容为整型

int __stdcall WSGetConfigDbl(const wchar_t*, double*);//读配置文件， 内容为浮点型

const wchar_t* __stdcall WSGetConfigAttribute(const wchar_t*, const wchar_t* lpAttribName);

// 以下函数可以使用 ConfigFile类代替
typedef void* HCONFIGFILE;
HCONFIGFILE __stdcall ConfigFileInit(const wchar_t* pPathName, const wchar_t* pRootNode = L"config_root", const wchar_t* pVersion = L"1.0", DWORD dwResered = 0);//创建或打开配置文件，pFileName为创建或打开文件路径名 不会自动Flush

void __stdcall ConfigFileDestroy(HCONFIGFILE hconfig);//关闭Config文件


void __stdcall ConfigSetStr(HCONFIGFILE hconfig, const wchar_t*, const wchar_t*);//设置配置， 内容为字符型

void __stdcall ConfigSetInt(HCONFIGFILE hconfig, const wchar_t*, int);//设置配置，内容为整型

void __stdcall ConfigSetDbl(HCONFIGFILE hconfig, const wchar_t*, double);//设置配置，  内容为浮点型

void __stdcall ConfigSetAttribute(HCONFIGFILE hconfig, const wchar_t* , const wchar_t* lpAttribName, const wchar_t* lpAttribValue);

int __stdcall ConfigFlush(HCONFIGFILE hconfig);//写配置文件


int __stdcall ConfigGetStr(HCONFIGFILE hconfig, const wchar_t*, wstring&);//读配置文件， 内容为字符型

int __stdcall ConfigGetStrEx(HCONFIGFILE hconfig, const wchar_t*, wchar_t*, int);//读配置文件， 内容为字符型,Delphi使用

const wchar_t* __stdcall ConfigGetStrPtr(HCONFIGFILE hconfig, const wchar_t*);//读配置文件， 内容为字符型 如果配置不存在则返回0

int __stdcall ConfigGetInt(HCONFIGFILE hconfig, const wchar_t*, int*);//读配置文件， 内容为整型

int __stdcall ConfigGetDbl(HCONFIGFILE hconfig, const wchar_t*, double*);//读配置文件，  内容为浮点型
	
const wchar_t* __stdcall ConfigGetAttribute(HCONFIGFILE hconfig, const wchar_t*, const wchar_t* lpAttribName);


// 编码后的长度一般比原文多占用1/3的存储空间，请保证base64code有足够的空间
int __stdcall WSBase64Encode(char * base64code, const unsigned char * src, int src_len); 
int __stdcall WSBase64Decode(unsigned char * buf, const char * base64code, int src_len = -1);


#ifdef WIN32
#ifdef __cplusplus
}
#endif
#endif
