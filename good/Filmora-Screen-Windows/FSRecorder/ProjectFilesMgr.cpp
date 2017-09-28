#include "stdafx.h"
#include "ProjectFilesMgr.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "SettingsDataManager.h"
#include "RecorderPreview.h"
#include "Communicator.h"
#include "ProjectAutoSaveMgr.h"

ProjectMgr::ProjectMgr(QObject *parent /*= 0*/)
	:QObject(parent)
{
	m_bContentChanged = false;
	m_bEditProj = false;
	SetNewProject(true);
}



ProjectMgr* ProjectMgr::GetInstance()
{
	static ProjectMgr ins;
	return &ins;
}

void ProjectMgr::SetSettingString(const QString &str)
{
	m_strSettings = str;
}

QString ProjectMgr::GetSettingString()
{
	return m_strSettings;
}

void ProjectMgr::SetPreviewString(const QString &str)
{
	m_strPreview = str;
}

QString ProjectMgr::GetPreviewString()
{
	return m_strPreview;
}

void ProjectMgr::SetMatterString(const QString &filepath)
{
	if (!FileOperation::IsFileExist(filepath)) return;
	m_strMatter = FileOperation::ReadFile(filepath,true);
}

QString ProjectMgr::GetMatterString()
{
	return m_strMatter;
}

QString ProjectMgr::GetMatterString2File()
{
	QString tmpfile = PathOperation::GetTempDir() +"/"+ QString("%1").arg(::GetTickCount() + qrand());
	FileOperation::WriteFile(tmpfile,m_strMatter,true);
	return tmpfile;
}

void ProjectMgr::SetProjectSettingsString(const QString &filepath)
{
    if (!FileOperation::IsFileExist(filepath)) return;
    m_strProjectSettings = FileOperation::ReadFile(filepath, true);
}

QString ProjectMgr::GetProjectSettingsString()
{
    return m_strProjectSettings;

}

QString ProjectMgr::GetProjectSettingsString2File()
{
    QString tmpfile = PathOperation::GetTempDir() + "/" + QString("%1").arg(::GetTickCount() + qrand());
    FileOperation::WriteFile(tmpfile, m_strProjectSettings, true);
    return tmpfile;
}

void ProjectMgr::SetTimelineString(const QString &filepath)
{
	if (!FileOperation::IsFileExist(filepath)) return;
	m_strTimeline = FileOperation::ReadFile(filepath,true);
}

QString ProjectMgr::GetTimelineString()
{
	return m_strTimeline;
}

QString ProjectMgr::GetTimelineString2File()
{
	QString tmpfile = PathOperation::GetTempDir() + "/" + QString("%1").arg(::GetTickCount() + qrand());
	FileOperation::WriteFile(tmpfile, m_strTimeline,true);
	return tmpfile;
}

void ProjectMgr::SetContentChanged(bool bChanged)
{
	if (m_bContentChanged == bChanged) return;
	m_bContentChanged = bChanged;
	Communicator::GetInstance()->TellContentChanged(bChanged);
}

bool ProjectMgr::IsContentChanged()
{
	return m_bContentChanged;
}

QString ProjectMgr::GetContentString()
{
	QString strContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
	strContent += "<ProjectMessage>\r\n";
	strContent += m_bEditProj ? "<IsEditor>1</IsEditor>\r\n" : "<IsEditor>0</IsEditor>\r\n";
	strContent += GetSettingString() + "\r\n";
	strContent += GetPreviewString() + "\r\n";
	strContent += GetMatterString() + "\r\n";
    strContent += GetProjectSettingsString() + "\r\n";
    strContent += GetTimelineString() + "\r\n";
	strContent += "</ProjectMessage>";

	return strContent;
}

void ProjectMgr::SetProjectPath(QString prjFile)
{
	if (m_strProjFile == prjFile) return;
	m_strProjFile = prjFile;
	Communicator::GetInstance()->TellProjectName(GetProjectName());
}

QString ProjectMgr::GetProjectPath()
{
	return m_strProjFile;
}

QString ProjectMgr::GetProjectName()
{
	QString projName = "";
	if (!m_strProjFile.isEmpty())
	{
		QFileInfo info(m_strProjFile);
		projName = info.fileName();
	}
	return projName;
}

void ProjectMgr::SetThumbnailPath(const QString &thumbnailpath)
{
	//QFile::remove(m_strThumbnailPath);
	m_strThumbnailPath = thumbnailpath;
}

QString ProjectMgr::GetThumbnailPath()
{
	return m_strThumbnailPath;
}

bool ProjectMgr::IsEditorProject()
{
	return m_bEditProj;
}

void ProjectMgr::SetEditorProject(bool bEdit)
{
	m_bEditProj = bEdit;
}


void ProjectMgr::SetNewProject(bool bNew)
{
	if (m_bNewProj == bNew) return;
	m_bNewProj = bNew;
	Communicator::GetInstance()->TellIsNewProject(m_bNewProj);
}

bool ProjectMgr::IsNewProject()
{
	return m_bNewProj;
}

bool ProjectMgr::Load(const QString &path, bool loadSettingOnly/* = false*/)
{
	if (!FileOperation::IsFileExist(path)) return false;
	
	QString strContent = FileOperation::ReadFile(path,true);
	QString begin, end;
	int pos1, pos2;
	
	begin = "<SettingMessage>";
	end = "</SettingMessage>";
	pos1 = strContent.indexOf(begin);
	pos2 = strContent.indexOf(end);
	if (pos1 != -1 && pos2 != -1)
		m_strSettings = strContent.mid(pos1, pos2 - pos1 + end.length());
	if (loadSettingOnly)
	{
		emit sigLoaded();
		return true;
	}

	begin = "<PreviewMessage>";
	end = "</PreviewMessage>";
	pos1 = strContent.indexOf(begin);
	pos2 = strContent.indexOf(end);
	if (pos1 != -1 && pos2 != -1)
		m_strPreview = strContent.mid(pos1, pos2 - pos1 + end.length());

	begin = "<MatterMessage>";
	end = "</MatterMessage>";
	pos1 = strContent.indexOf(begin);
	pos2 = strContent.indexOf(end);
	if (pos1 != -1 && pos2 != -1)
		m_strMatter = strContent.mid(pos1, pos2 - pos1 + end.length());

    begin = "<ProjectSettings>";
    end = "</ProjectSettings>";
    pos1 = strContent.indexOf(begin);
    pos2 = strContent.indexOf(end);
    if (pos1 != -1 && pos2 != -1)
        m_strProjectSettings = strContent.mid(pos1, pos2 - pos1 + end.length());


	begin = "<TimelineMessage>";
	end = "</TimelineMessage>";
	pos1 = strContent.indexOf(begin);
	pos2 = strContent.indexOf(end);
	if (pos1 != -1 && pos2 != -1)
		m_strTimeline = strContent.mid(pos1, pos2 - pos1 + end.length());

	begin = "<IsEditor>";
	end = "</IsEditor>";
	pos1 = strContent.indexOf(begin) + begin.length();
	pos2 = strContent.indexOf(end);
 	if (pos1 != -1 && pos2 != -1)
	{
		QString strEdit = strContent.mid(pos1, pos2 - pos1);
		m_bEditProj = strEdit == "0" ? false : true;
	}

	SetNewProject(false);
	//m_bNewProj = false;
	SetProjectPath(path);
	m_bContentChanged = false;
	emit sigLoaded();
	return true;
}
bool ProjectMgr::_SaveNew(const QString &newpath)
{
    if (newpath.isEmpty()) return false;
    if (QFile::exists(newpath))
    {
        QFile::remove(newpath);
        ProjectFilesMgr::GetInstance()->Remove(newpath);
    }
    GetAllSavingString();
    QString strContent = GetContentString();
    FileOperation::CreateNewFile(newpath);
    FileOperation::WriteFile(newpath, strContent, true);

    auto info = new ProjectFileInfo;
    info->filepath = newpath;
    info->imgpath = GetThumbnailPath();
    info->savetime = info->CurrentTimeString();
    ProjectFilesMgr::GetInstance()->Add(info);
    ProjectFilesMgr::GetInstance()->Add2Recent(info);

    SetNewProject(false);
    SetProjectPath(newpath);
    m_bContentChanged = false;
    QTimer::singleShot(0, [&]{
        Communicator::GetInstance()->TellContentChanged(m_bContentChanged);
        Communicator::GetInstance()->TellRecentOpens();
        Communicator::GetInstance()->TellSaveProjectFinish();
    });


    return true;
}


bool ProjectMgr::Save()
{
    bool retvalue = _SaveNew(m_strProjFile);
    ProjectAutoSaveMgr::GetInstance()->Save();
    return retvalue;
}

bool ProjectMgr::SaveNew(const QString newpath)
{
    bool retvalue = _SaveNew(newpath);
    ProjectAutoSaveMgr::GetInstance()->SaveAsNew(newpath);
    return retvalue;
}

bool ProjectMgr::AutoSaveNew(const QString newpath)
{
    if (newpath.isEmpty()) 
        return false;

    if (QFile::exists(newpath))
    {
        QFile::remove(newpath);
        ProjectFilesMgr::GetInstance()->Remove(newpath);
    }
    GetAllSavingString();
    QString strContent = GetContentString();
    FileOperation::CreateNewFile(newpath);
    FileOperation::WriteFile(newpath, strContent, true);

    return true;
}

void ProjectMgr::GetAllSavingString()
{
	m_strSettings = SettingsDataManager::GetInstance()->GetSettingString();
	m_strPreview = RecordingPreviewDataManager::GetInstance()->GetSettingString();
	if (Communicator::GetInstance()->IsEditorRunning())
	{
		std::tuple<QString, QString, QString> res = Communicator::GetInstance()->GetEditorProjectString();
        m_strMatter = std::get<0>(res);
        m_strTimeline = std::get<1>(res);
        m_strProjectSettings = std::get<2>(res);
	}
}

void ProjectMgr::Clear()
{
	SetNewProject(true);
	SetProjectPath("");
	m_bContentChanged = false;
}




//////////////////////////////////////////////////////////////////////////


ProjectFilesMgr::ProjectFilesMgr(QObject *parent)
	: QObject(parent)
{
	m_pCurrent = nullptr;
	Load();
	LoadRecentOpens();
}

ProjectFilesMgr::~ProjectFilesMgr()
{
	for (auto info : m_lstInfos)
	{
		SAFE_DELETE(info);
	}
}

ProjectFilesMgr* ProjectFilesMgr::GetInstance()
{
	static ProjectFilesMgr ins;
	return &ins;
}

void ProjectFilesMgr::Add(ProjectFileInfo* info)
{
	if (IsExist(info)) return;
	m_lstInfos.push_back(info);

	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, GetPath()))
		return;
	QList<QString> lstLost;
	QDomElement rootElem = doc.documentElement();

	QDomElement prjElem = XmlOperation::AddXmlElement(doc, rootElem, ElemNodeText_ProjectFile, "");
	XmlOperation::AddXmlElement(doc, prjElem, ElemNodeText_FilePath, info->filepath);
	XmlOperation::AddXmlElement(doc, prjElem, ElemNodeText_ThumbnailPath, info->imgpath);
	XmlOperation::AddXmlElement(doc, prjElem, ElemNodeText_SaveTime, QString("%1").arg(info->savetime));

	XmlOperation::SaveXmlContent2File(doc, GetPath());
}

void ProjectFilesMgr::Add2Recent(ProjectFileInfo* info)
{
	if (m_lstRecents.contains(info))
		m_lstRecents.removeOne(info);
	m_lstRecents.insert(0, info);
	int nLimitCnt = 10;
	if (m_lstRecents.size() > nLimitCnt)
	{
		QList<ProjectFileInfo*> lstTmp;
		for (int i = 0; i < nLimitCnt; ++i)
		{
			lstTmp.push_back(m_lstRecents[i]);
		}
		m_lstRecents.clear();
		m_lstRecents = lstTmp;
	}
	Recents2File();
}

void ProjectFilesMgr::Remove(ProjectFileInfo* info)
{
	if (!info || !IsExist(info)) return;
	
	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, GetPath()))
		return;

	QDomElement rootElem = doc.documentElement();
	QDomElement prjfileElem = rootElem.firstChildElement();
	while (!prjfileElem.isNull())
	{
		if (prjfileElem.tagName() == "ProjectFile")
		{
			QDomElement  tmpElem = prjfileElem.firstChildElement(ElemNodeText_FilePath);
			if (!tmpElem.isNull())
			{
				if (info->filepath == tmpElem.text())
				{
					rootElem.removeChild(prjfileElem);
					XmlOperation::SaveXmlContent2File(doc, GetPath());
					break;
				}
			}
		}
		prjfileElem = prjfileElem.nextSiblingElement();
	}
	delete info;
	m_lstInfos.removeOne(info);
	m_lstRecents.removeOne(info);
	Recents2File();
}

void ProjectFilesMgr::Remove(const QString &filepath)
{
	auto info = GetProjectFileInfo(filepath);
	if (!info) return;
	Remove(info);
}

QList<ProjectFileInfo*>& ProjectFilesMgr::GetInfos()
{
	return m_lstInfos;
}

QList<ProjectFileInfo*>& ProjectFilesMgr::GetRecentInfos()
{
	return m_lstRecents;
}

ProjectFileInfo *ProjectFilesMgr::GetCurrent()
{
	return m_pCurrent;
}

void ProjectFilesMgr::SetCurrent(ProjectFileInfo *info)
{
	m_pCurrent = info;
}

void ProjectFilesMgr::Load()
{
	QString settingfilePath = GetPath();
	QFile file(settingfilePath);
	if (!file.exists()) return;

	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, settingfilePath))
		return;

	QList<QString> lstLost;
	QDomElement rootElem = doc.documentElement();
	QDomElement prjfileElem = rootElem.firstChildElement();
	while (!prjfileElem.isNull())
	{
		if (prjfileElem.tagName() == ElemNodeText_ProjectFile)
		{
			bool bExist = true;
			ProjectFileInfo *info = new ProjectFileInfo;
			QDomElement  tmpElem = prjfileElem.firstChildElement(ElemNodeText_SaveTime);
			if (!tmpElem.isNull())
			{
                info->savetime = info->AdjustTimeString(tmpElem.text());
			}
            else
            {
                // 无此节点 修正时间格式
                info->savetime = info->AdjustTimeString("");
            }

			tmpElem = prjfileElem.firstChildElement(ElemNodeText_ThumbnailPath);
			if (!tmpElem.isNull())
			{
				info->imgpath = tmpElem.text();
			}
			tmpElem = prjfileElem.firstChildElement(ElemNodeText_FilePath);
			if (!tmpElem.isNull())
			{
				info->filepath = tmpElem.text();
				//if (!FileOperation::IsFileExist(info->filepath))
				//{
				//	bExist = false;
				//	lstLost.push_back(info->filepath);
				//	delete info;
				//}
			}
			if (bExist) 
                m_lstInfos.push_back(info);

		}
		prjfileElem = prjfileElem.nextSiblingElement();
	}

    // 增加未保存工程
    ProjectFileInfo *untitleInfo = ProjectAutoSaveMgr::GetInstance()->GetUntitleProjectInfo();
    if (untitleInfo)
    {
        ProjectFileInfo *newUntitleInfo = untitleInfo->Clone();
        m_lstInfos.push_back(newUntitleInfo);
    }

    // 按保存时间排序,最新保存的的放前面
    std::sort(m_lstInfos.begin(), m_lstInfos.end(), [&](ProjectFileInfo *l, ProjectFileInfo *r){
        return l->savetime > r->savetime;
    });
	//RemoveUnExist(lstLost);
}


void ProjectFilesMgr::RemoveUnExist(QStringList lstLost)
{
	//删除不存在的文件
	if (lstLost.isEmpty()) return;
	QString settingfilePath = GetPath();
	QFile file(settingfilePath);
	if (!file.exists()) return;

	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, settingfilePath))
		return;
	QDomElement rootElem = doc.documentElement();

	bool bChange = false;
	for (auto path : lstLost)
	{
		QDomElement prjfileElem = rootElem.firstChildElement();
		while (!prjfileElem.isNull())
		{
			if (prjfileElem.tagName() == ElemNodeText_ProjectFile)
			{
				QDomElement  tmpElem = prjfileElem.firstChildElement(ElemNodeText_FilePath);
				if (!tmpElem.isNull())
				{
					if (path == tmpElem.text())
					{
						rootElem.removeChild(prjfileElem);
						prjfileElem = prjfileElem.nextSiblingElement();
						bChange = true;
						continue;
					}
				}
			}
			prjfileElem = prjfileElem.nextSiblingElement();
		}
	}
	if (bChange) XmlOperation::SaveXmlContent2File(doc, settingfilePath);
}

#define tag_name_paths			"Paths"
#define tag_name_path			"Path"

void ProjectFilesMgr::LoadRecentOpens()
{
	QString  settingfilePath = GetRecentsConfigPath();
	QFile file(settingfilePath);
	if (!file.exists()) return;

	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, settingfilePath))
		return;

	QList<QDomElement> lstLostElems;
	QDomElement rootElem = doc.documentElement();
	QDomElement pathElem = rootElem.firstChildElement();
	while (!pathElem.isNull())
	{
		if (pathElem.tagName() == tag_name_path)
		{
			QString path = pathElem.text();
			auto info = GetProjectFileInfo(path);
			if (info && QFile::exists(path))
			{
				m_lstRecents.push_back(info);
			}
			else
			{
				lstLostElems.push_back(pathElem);
			}

		}
		pathElem = pathElem.nextSiblingElement();
	}

	bool bChange = false;
	for (auto elem : lstLostElems)
	{
		bChange = true;
		rootElem.removeChild(elem);
	}

	if (bChange) XmlOperation::SaveXmlContent2File(doc, settingfilePath);
}

QString ProjectFilesMgr::GetRecentsConfigPath()
{
	QString path = FSGetFilmoraScreenDirectory() + "/recentopenfiles.xml";

	QFile file(path);
	if (!file.exists())
	{
		XmlOperation::CreateXmlFile(path, "Paths");
	}
	return path;
}

QString ProjectFilesMgr::GetPath()
{
	QString path = FSGetFilmoraScreenDirectory() + "/projectfiles.xml";
	
	QFile file(path);
	if (!file.exists())
	{
		XmlOperation::CreateXmlFile(path, "ProjectFiles");
	}
	return path;
}

bool ProjectFilesMgr::IsExist(ProjectFileInfo* info)
{
	for (auto in : m_lstInfos)
	{
		if (in->filepath == info->filepath)
		{
			return true;
		}
	}
	return false;
}

bool ProjectFilesMgr::IsExist(const QString filepath)
{
	for (auto in : m_lstInfos)
	{
		if (in->filepath == filepath)
		{
			return true;
		}
	}
	return false;
}

ProjectFileInfo* ProjectFilesMgr::GetProjectFileInfo(const QString filepaht)
{
	for (auto info : m_lstInfos)
	{
		if (filepaht == info->filepath)
		{
			return info;
		}
	}
	return nullptr;
}

void ProjectFilesMgr::Recents2File()
{
	if (m_lstRecents.isEmpty()) return;

	QString  settingfilePath = GetRecentsConfigPath();
	QFile file(settingfilePath);
	if (file.exists()) file.remove();
	
	XmlOperation::CreateXmlFile(settingfilePath, "Paths");
	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc,settingfilePath))
		return;
	QDomElement rootElem = doc.documentElement();

	for (auto info : m_lstRecents)
	{
		XmlOperation::AddXmlElement(doc, rootElem, "Path", info->filepath);
	}
	XmlOperation::SaveXmlContent2File(doc, GetRecentsConfigPath());
}

