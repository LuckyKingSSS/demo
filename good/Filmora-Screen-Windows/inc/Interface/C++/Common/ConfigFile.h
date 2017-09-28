///////////////////////////////////////////////////////////////////////////
// ConfigFile.h
//==========================================================================
// 功能    ： 个人创建并读写配置文件
// 模块	   ： PSB项目WSLibrary.dll模块 
// 创建时间： 2005-08-10 
// 创建者  ： 付玮
// 修订    ： 
//==========================================================================

//注意:
// 1.个人配置文件类建立时会对文件进行核对,存在该文件就打开,不存在则创建;
//   读文件时也会这样,所以读时大家要注意打开的文件名是否正确,否则将一无所获;
// 2.释放时会自动保存,大家可以放心使用释放功能.
// 3.大家注意写文件时有大小写之分哦!

#pragma once

#ifdef WIN32

#ifdef WSCONFIG_EXPORTS
#define WSCONFIG_API __declspec(dllexport)
#else
#define WSCONFIG_API __declspec(dllimport)
#endif

#else

#define WSCONFIG_API

#endif

#include <string>
using namespace std;


//class ConfigFileImpl;
class XMLConfig;
class WSCONFIG_API ConfigFile
{
public:
	ConfigFile(const wchar_t*, const wchar_t* pRootNode = L"config_root");//构造函数
	virtual ~ConfigFile();//析构函数


	void SetConfigStr(const wchar_t* , const wchar_t*);//写配置,string类型
	void SetConfigInt(const wchar_t* , int);//写配置,int类型
	void SetConfigDbl(const wchar_t* , double);//写配置,double类型

	void SetAttribute(const wchar_t* , const wchar_t* lpAttribName, const wchar_t* lpAttribValue);

	int GetConfigStr(const wchar_t*, wstring&);//读配置信息,string类型
	const wchar_t* GetConfigStrPtr(const wchar_t*);//读配置信息,string类型
	int GetConfigInt(const wchar_t*, int*);//读配置信息,int类型
	int GetConfigDbl(const wchar_t*, double*);//读配置信息,double类型

	const wchar_t* GetAttribute(const wchar_t*, const wchar_t* lpAttribName);

	int Flush();//存盘

private:
	//ConfigFileImpl* m_pzImpl;

	XMLConfig*	m_pzImpl;
	wchar_t *			m_pszFileName; //保存配置文件路径
	
};
