#include "stdafx.h"
//#include "FSProjectMgrPrivate.h"
//#include "inc_CommonLib/FileOperation.h"
//#include "inc_CommonLib/XmlOperation.h"
//
//FSProjectMgrPrivate::FSProjectMgrPrivate(QObject *parent)
//	: FSProjectMgr(parent)
//{
//	m_bLoad = false;
//}
//
//FSProjectMgrPrivate::~FSProjectMgrPrivate()
//{
//
//}
//
//void FSProjectMgrPrivate::SetSettingString(const QString &str)
//{
//	m_strSettings = str;
//}
//
//QString FSProjectMgrPrivate::GetSettingString()
//{
//	return m_strSettings;
//}
//
//void FSProjectMgrPrivate::SetPreviewString(const QString &str)
//{
//	m_strPreview = str;
//}
//
//QString FSProjectMgrPrivate::GetPreviewString()
//{
//	return m_strPreview;
//}
//
//void FSProjectMgrPrivate::SetMatterString(const QString &str)
//{
//	m_strMatter = str;
//}
//
//QString FSProjectMgrPrivate::GetMatterString()
//{
//	return m_strMatter;
//}
//
//void FSProjectMgrPrivate::SetTimelineString(const QString &str)
//{
//	m_strTimeline = str;
//}
//
//QString FSProjectMgrPrivate::GetTimelineString()
//{
//	return m_strTimeline;
//}
//
//QString FSProjectMgrPrivate::GetEditorTempFile()
//{
//	QStringList list = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
//	if (list.isEmpty()) return "";
//	QString file = list[0] + "\\FilmoraScreen";
//	QDir dir(file);
//	if (!dir.exists())
//	{
//		dir.mkpath(file);
//	}
//	file += "\\editorfile.xml";
//	return file;
//}
//
//void FSProjectMgrPrivate::SaveEditor2TempFile()
//{
//	QString file = GetEditorTempFile();
//	if (QFile::exists(file))
//	{
//		QFile::remove(file);
//	}
//	FileOperation::CreateNewFile(file);
//	FileOperation::WriteFile(file, m_strMatter + "\r\n" + m_strTimeline);
//}
//
//bool FSProjectMgrPrivate::Save(bool bRecorderOnly)
//{
//	if (m_bLoad)
//	{
//		SaveNew(m_strPath, bRecorderOnly);
//		return true;
//	}
//	return false;
//}
//
//bool FSProjectMgrPrivate::SaveNew(const QString &path, bool bRecorderOnly)
//{
//	if (QFile::exists(path))
//	{
//		QFile::remove(path);
//	}
//	QString strContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
//	strContent += "<ProjectMessage>\r\n";
//	strContent += m_strSettings + "\r\n";
//	strContent += m_strPreview + "\r\n";
//	if (!bRecorderOnly)
//	{
//		strContent += FileOperation::ReadFile(GetEditorTempFile());
//		QFile::remove(GetEditorTempFile());
//	}
//	strContent += "</ProjectMessage>";
//	FileOperation::CreateNewFile(path);
//	//FileOperation::WriteFile(path,strContent);
//	QFile file(path);
//	if (!file.open(QIODevice::WriteOnly)) return false;
//	file.write(strContent.toUtf8());
//	return true;
//}
//
//bool FSProjectMgrPrivate::Load(const QString &path)
//{
//	if (!FileOperation::IsFileExist(path)) return false;
//	m_bLoad = true;
//	m_strPath = path;
//
//	QString strContent = FileOperation::ReadFile(path);
//
//	QString begin = "<SettingMessage>";
//	QString end = "</SettingMessage>";
//	int pos1 = strContent.indexOf(begin);
//	int pos2 = strContent.indexOf(end);
//	if (pos1 != -1 && pos2 != -1)
//		m_strSettings = strContent.mid(pos1, pos2 - pos1 + end.length());
//
//	begin = "<PreviewMessage>";
//	end = "</PreviewMessage>";
//	pos1 = strContent.indexOf(begin);
//	pos2 = strContent.indexOf(end);
//	if (pos1 != -1 && pos2 != -1)
//		m_strPreview = strContent.mid(pos1, pos2 - pos1 + end.length());
//
//	begin = "<MatterMessage>";
//	end = "</MatterMessage>";
//	pos1 = strContent.indexOf(begin);
//	pos2 = strContent.indexOf(end);
//	if (pos1 != -1 && pos2 != -1)
//		m_strPreview = strContent.mid(pos1, pos2 - pos1 + end.length());
//
//	begin = "<TimelineMessage>";
//	end = "</TimelineMessage>";
//	pos1 = strContent.indexOf(begin);
//	pos2 = strContent.indexOf(end);
//	if (pos1 != -1 && pos2 != -1)
//		m_strPreview = strContent.mid(pos1, pos2 - pos1 + end.length());
//
//	emit sigLoaded();
//	return true;
//}
