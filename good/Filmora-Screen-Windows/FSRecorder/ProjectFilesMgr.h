#ifndef PROJECTFILESMGR_H
#define PROJECTFILESMGR_H
#include <QObject>

class ProjectMgr : public QObject
{
	Q_OBJECT

public:
	static ProjectMgr* GetInstance();

	void SetSettingString(const QString &str);
	QString GetSettingString();

	void SetPreviewString(const QString &str);
	QString GetPreviewString();

	void SetMatterString(const QString &filepath);
	QString GetMatterString();
	QString GetMatterString2File();

    void SetProjectSettingsString(const QString &filepath);
    QString GetProjectSettingsString();
    QString GetProjectSettingsString2File();

	void SetTimelineString(const QString &filepath);
	QString GetTimelineString();
	QString GetTimelineString2File();

	void SetContentChanged(bool bChanged);
	bool IsContentChanged();
	QString GetContentString();

	void SetProjectPath(QString prjFile);
	QString GetProjectPath();
	QString GetProjectName();

	void SetThumbnailPath(const QString &thumbnailpath);
	QString GetThumbnailPath();

	bool IsEditorProject();
	void SetEditorProject(bool bEdit);

	void SetNewProject(bool bNew);
	bool IsNewProject();

	bool Load(const QString &path, bool loadSettingOnly = false);  //loadSettingOnly: 用户点击QuitRecord时为true
	bool Save();
    bool SaveNew(const QString newpath);
    bool AutoSaveNew(const QString newpath);

	void GetAllSavingString();
	void Clear();
	
Q_SIGNALS:
	void sigLoaded();
protected:
	ProjectMgr(QObject *parent = 0);
    bool _SaveNew(const QString &newpath);
protected:
	bool m_bContentChanged;
	bool m_bNewProj;
	bool m_bEditProj;
	QString m_strSettings;
	QString m_strPreview;
    QString m_strMatter;
    QString m_strProjectSettings;
	QString m_strTimeline;

	QString m_strThumbnailPath;
	QString m_strProjFile;
};



struct ProjectFileInfo;
class ProjectFilesMgr : public QObject
{
	Q_OBJECT

public:
	static ProjectFilesMgr* GetInstance();
	~ProjectFilesMgr();
	
	void Add(ProjectFileInfo* info);
	void Add2Recent(ProjectFileInfo* info);
	void Remove(ProjectFileInfo* info);
	void Remove(const QString &filepath);

	bool IsExist(const QString filepath);
	bool IsExist(ProjectFileInfo* info);

	QList<ProjectFileInfo*>& GetInfos();
	QList<ProjectFileInfo*>& GetRecentInfos();

	ProjectFileInfo* GetCurrent();
	void SetCurrent(ProjectFileInfo *info); 

protected:
	ProjectFilesMgr(QObject *parent = 0);
	void Load();
	void RemoveUnExist(QStringList lost);
	QString GetPath();
	void LoadRecentOpens();
	QString GetRecentsConfigPath();
	
	ProjectFileInfo* GetProjectFileInfo(const QString filepaht);
	void Recents2File();
private:
	QList<ProjectFileInfo*> m_lstInfos;
	QList<ProjectFileInfo*> m_lstRecents;

	ProjectFileInfo* m_pCurrent;
};

struct ProjectFileInfo
{

public:
	QString		filepath;
	QString		imgpath;
	QString		savetime;

	QString GetBaseName()
	{
		QFileInfo file(filepath);
		if (!file.exists()) return "";
		return file.baseName();
	}
    static QString CurrentTimeString()
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
    }
    static QString AdjustTimeString(QString timeString)
    {
        QRegExp regExp("[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}-[0-9]{2}-[0-9]{2}");
        bool bTime = regExp.exactMatch(timeString);
        if (!bTime)
            timeString = "0000-00-00 00-00-00";
        return timeString;
    }
    ProjectFileInfo* Clone()
    {
        ProjectFileInfo *newInfo = new ProjectFileInfo;
        newInfo->filepath = filepath;
        newInfo->imgpath = imgpath;
        newInfo->savetime = savetime;
        return newInfo;
    }
};
typedef QList<ProjectFileInfo*> ProjectFileInfos;


#define  ElemNodeText_ProjectFileAutoSave		"ProjectFileAutoSave"
#define  ElemNodeText_ProjectFile		"ProjectFile"
#define  ElemNodeText_FilePath			"FilePath"
#define  ElemNodeText_ThumbnailPath		"ThumbnailPath"
#define  ElemNodeText_SaveTime			"SaveTime"


#endif // PROJECTFILESMGR_H
