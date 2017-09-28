#pragma once
#include "commonlib_global.h"
class COMMONLIB_EXPORT FileOperation
{
public:
	FileOperation();
	~FileOperation();

	static bool IsFileExist(const QString &strFile);
	static QString ReadFile(const QString &strUrl, bool bUtf8 = false);
	static void WriteFile(const QString &strFile, QString strContent,bool bUtf8 = false);
	
	static QString GetFileName(const QString &strFullName);
	static QString GetFileBaseName(const QString &strFullName);
	static QString GetFileSuffixName(const QString &strFullName);
	static QString GetFileDir(const QString &strFullName);
	static void CreateNewFile(const QString &path);
};

