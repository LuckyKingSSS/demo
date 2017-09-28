#pragma once
#include <string>
#include <windows.h>
using namespace std;


class CStringUtils
{
public:
	CStringUtils();
	~CStringUtils();

public:
	//字符串处理
	static BOOL WStringToString(const std::wstring &wstr, std::string &str);
	static BOOL StringToWString(const std::string &str, std::wstring &wstr);
	static string string_To_UTF8(const std::string & str);
	static string trim(string& s);
	static wstring trim(wstring& s);
	static string getFileName(string strFilePath);
	static string ws2s(const std::wstring& wstr);
	static char* UnicodeToUtf8(const wchar_t* unicode);
	//string 转 cstring
	static CString stringToCString(string strInput);
	//unicode 转utf8
	static const wchar_t* Utf8ToUnicode(const char* utf8Str);
	//获取特定字体的长度
	//static FontFlag getStringSize(CString strText);
	//优化：替换实体符号
	static CString ReplaceEntitySymbols(CString strText);
	//去除字符串中的换行符
	static wstring RemoveLineBreaks(wstring wstrInput);
};

