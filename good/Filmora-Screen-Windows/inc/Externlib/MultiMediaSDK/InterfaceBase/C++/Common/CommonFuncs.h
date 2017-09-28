// CommonFuncs.h

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include "math_utility.h"

//******************************
// 读取扩展名


#ifdef UNICODE
	#define GetFileExtendName GetFileExtendNameW
#else
	#define GetFileExtendNameGetFileExtendNameA
#endif


//#include <tchar.h>
//#pragma warning(disable: 4996)
#include <wchar.h>

__inline const char* GetFileExtendNameA(const char* lpPathName)
{
	const char* pTemp = strchr(lpPathName, '.');
	if (pTemp)
		return pTemp + 1;

	return "";
}

__inline const wchar_t* GetFileExtendNameW(const wchar_t* lpPathName)
{
	const wchar_t* pTemp = wcsrchr(lpPathName, L'.');
	if (pTemp)
		return pTemp + 1;

	return L"";
}


//*******************************
// FourCC转字符串

__inline char* FourCCToString(DWORD dwFourCC, char szFourCC[5])
{
	memset(szFourCC, 0, sizeof(szFourCC));
	memcpy(szFourCC, &dwFourCC, 4);

	return szFourCC;
}

// 去除字符串末尾反斜杠'\\'
__inline void RemoveBackslash(wchar_t* szPath)
{
	int nPathLen = (int)wcslen(szPath);
	if (L'\\' == szPath[nPathLen - 1])
	{
		szPath[nPathLen - 1] = L'\0';
	}
}

//__inline bool CheckPath(const wchar_t* szPath, const wchar_t* szErrorInfo = 0)
//{
//	if (!szPath || (wcslen(szPath) == 0))
//	{
//		if (!szPath && szErrorInfo)
//		{
//			WSLogEx(L"%ls : pointer == NULL.", szErrorInfo);
//		}
//		else if (szErrorInfo)
//		{
//			WSLogEx(L"%ls : length == 0.", szErrorInfo);
//		}
//
//		return false;
//	}
//
//	return true;
//}

#ifdef WIN32

//__inline wchar_t *Format(wchar_t t[], wchar_t *sFormat, ...)
//{
//	va_list arglist;
//	va_start(arglist, sFormat);
//	StringCchVPrintf(t, MAX_PATH, sFormat, arglist);
//	va_end(arglist);
//	return t;
//}

__inline BOOL IsDirectory(LPCTSTR lpPathName)
{
	WIN32_FIND_DATA wfd = { 0 };
	FindClose(FindFirstFile(lpPathName, &wfd));

	return (FILE_ATTRIBUTE_DIRECTORY & wfd.dwFileAttributes);
}

__inline BOOL IsFileExists(LPCTSTR lpPathName)
{
	WIN32_FIND_DATA wfd = { 0 };
	FindClose(FindFirstFile(lpPathName, &wfd));

	return (FILE_ATTRIBUTE_ARCHIVE & wfd.dwFileAttributes);
}

#endif // WIN32

