#include "stdafx.h"
#include <windows.h> 
#include <fstream>
#include <io.h>  
#include <iostream>
#include "StringUtils.h"
#include "FileFinderHelper.h"

using namespace std;

CFileFinderHelper::CFileFinderHelper()
{
}


CFileFinderHelper::~CFileFinderHelper()
{
}

void CFileFinderHelper::GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//文件句柄    
	long   hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;
	string p;
	path = CStringUtils::trim(path);
	p.assign(path).append("\\*" + format);
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
					GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void CFileFinderHelper::find(const wchar_t* lpPath, std::vector<std::string> &fileList, wchar_t* strFileType)
{
	wchar_t szFind[MAX_PATH] = { 0 };
	WIN32_FIND_DATA FindFileData;
	wcscpy_s(szFind, wcslen(lpPath) + 1, lpPath);
	wcscat_s(szFind, MAX_PATH, L"\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				wchar_t szFile[MAX_PATH];
				wcscpy_s(szFile, wcslen(lpPath) + 1, lpPath);
				wcscat_s(szFile, MAX_PATH, L"\\");
				wcscat_s(szFile, MAX_PATH, (wchar_t*)(FindFileData.cFileName));
				find(szFile, fileList, strFileType);
			}
		}
		else
		{
			wstring strPath(szFind);
			string strRecPath;
			wstring wstrFileName(FindFileData.cFileName);
			string strFileName, strSuffix;
			CStringUtils::WStringToString(wstrFileName, strFileName);
			int iIndex = strFileName.find_last_of(".");
			if (iIndex != string::npos)
			{
				strSuffix = strFileName.substr(iIndex);
				CStringUtils::trim(strSuffix);
				/*TRACE("**********  file suffix begin  *******\n");
				TRACE("the suffix is: %s\n",strSuffix.c_str());
				TRACE("**********  file suffix end  *******\n");*/
				std::cout << strSuffix.c_str() << endl;
				wchar_t buf[MAX_PATH] = { 0 };
				wmemcpy_s(buf, MAX_PATH, szFind, wcslen(szFind) + 2);
				string strFileTypeTmp = CStringUtils::ws2s(strFileType);
				if (strSuffix.compare(strFileTypeTmp.c_str()) == 0)
				{
					//wmemcpy_s(buf, MAX_PATH, wstrFileName.c_str(), wcslen(wstrFileName.c_str()) + 2);
					wstring wstrPath(buf);
					int iIndex = wstrPath.find_last_of(L"\\");
					wstrPath = wstrPath.substr(0, iIndex + 1);
					//swprintf_s(wstrPath, wcslen(wstrPath.c_str()), L"\\%s", wstrFileName);
					wstrPath.append(wstrFileName);
					string strRecPath;
					CStringUtils::WStringToString(wstrPath, strRecPath);
					fileList.push_back(strRecPath);
				}
			}
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}
	FindClose(hFind);
}

std::string CFileFinderHelper::getFileType(wstring strFileName)
{
	int iUnderlineIndex = strFileName.find_last_of(L"_");
	int iDotIndex = strFileName.find_last_of(L".");
	wstring strFileType;
	strFileType = strFileName.substr(iUnderlineIndex + 1, iDotIndex - iUnderlineIndex - 1);
	string strRec;
	CStringUtils::WStringToString(strFileType, strRec);
	return strRec;
}

std::wstring CFileFinderHelper::GetAppPath()
{
	wchar_t chpath[MAX_PATH] = {0};
	GetModuleFileName(NULL, (LPWSTR)chpath, sizeof(chpath));
	wstring strAppPath(chpath);
	int iIndex = strAppPath.find_last_of(L"\\");
	strAppPath = strAppPath.substr(0, iIndex);
	return strAppPath;
}

bool CFileFinderHelper::IsFileExsit(string strFilePath)
{
	fstream _file;
	_file.open(strFilePath, ios::in);
	if (!_file)
	{
		TRACE("*************file :%s\n", strFilePath + "not exsit");
		return false;
	}
	else
	{
		TRACE("*************file :%s\n", strFilePath + "exsit");
		return true;
	}
}
