#include "stdafx.h"
#include <QtCore/QtCore>
#include "FileOperation.h"
#include <QFileInfo>

FileOperation::FileOperation()
{
}


FileOperation::~FileOperation()
{ 
}

bool FileOperation::IsFileExist(const QString &strFile)
{
	QFileInfo file(strFile); 
	return file.exists(); 
}

QString FileOperation::ReadFile(const QString &strUrl,bool bUtf8 /*= false*/)
{
	QFile file(strUrl);
	if (file.open(QFile::ReadOnly))
	{
		QTextStream stream(&file);
		if (bUtf8) stream.setCodec("UTF-8");
		QString res = stream.readAll();
		return res;
	}
	return "";
}

void FileOperation::WriteFile(const QString &strFile, QString strContent, bool bUtf8/* = false*/)
{
	QFile file(strFile);
	if (file.open(QIODevice::WriteOnly)) 
	{
		QTextStream stream(&file);
		if(bUtf8) stream.setCodec("UTF-8");
		stream << strContent;
		file.close();
	}
}

QString FileOperation::GetFileName(const QString &strFullName)
{
	QFileInfo file(strFullName);
	return file.fileName();
}

QString FileOperation::GetFileBaseName(const QString &strFullName)
{
	QFileInfo file(strFullName);
	return file.baseName();
}

QString FileOperation::GetFileSuffixName(const QString &strFullName)
{
	QFileInfo file(strFullName);
	return file.suffix();
}

QString FileOperation::GetFileDir(const QString &strFullName)
{
	QFileInfo file(strFullName);
	QString dirPath = file.dir().path();
	if (!dirPath.endsWith("/"))
	{
		dirPath += "/";
	}
	return dirPath;
}

void FileOperation::CreateNewFile(const QString &path)
{
	QFile file(path);
	if (file.exists())
	{
		return;
	}
	file.open(QIODevice::WriteOnly);
	file.close();
}
