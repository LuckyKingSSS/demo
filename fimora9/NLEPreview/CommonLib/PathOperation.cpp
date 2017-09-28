#include "stdafx.h"
#include "PathOperation.h"
#include <QCoreApplication>
#include "windows.h"
#include "shlobj.h"

PathOperation::PathOperation()
{
}


PathOperation::~PathOperation()
{
}

QString PathOperation::MakeWorkPath(const QString &strSubPath)
{
	QString strRespath = QCoreApplication::applicationDirPath()+ "/" + strSubPath;
	return strRespath;
}

QString PathOperation::GetUserSelectDir(QString title, WId wid)
{
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = (HWND)wid;
	TCHAR szTitle[MAX_PATH] = { 0 };
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szDisplay[MAX_PATH] = { 0 };
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
	bi.pidlRoot = pidl;
	std::wstring wstitle = title.toStdWString();
	bi.lpszTitle = wstitle.c_str();
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS /*| BIF_DONTGOBELOWDOMAIN*/;
	LPITEMIDLIST res = SHBrowseForFolder(&bi);
	if (!res) return "";
	wchar_t path[MAX_PATH] = { 0 };
	SHGetPathFromIDList(res, path);
	return QString::fromWCharArray(path);
}
QString PathOperation::GetTempDir()
{
	QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
	if (dirs.size() > 0)
	{
		return dirs[0];
	}
	return "";
}

QString PathOperation::GetDocumentPath()
{
	QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
	if (dirs.size() > 0)
	{
		return dirs[0];
	}
	return "";
}

