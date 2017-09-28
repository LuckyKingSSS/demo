#ifndef PROJECTAUTOSAVEMGR_H
#define PROJECTAUTOSAVEMGR_H

#include <QObject>
#include <QTimer>

#include "ProjectFilesMgr.h"
#include "inc_FSCommonlib/FSCommonWidget.h"

class ProjectAutoSaveBase;
class ProjectAutoSaveMgr : public QObject
{
    Q_OBJECT
public:
    enum MgrType{ Mgr_Unknow, Mgr_UntitleSave, Mgr_TitleSave };
public:
    static ProjectAutoSaveMgr* GetInstance();

    void LoadProject(ProjectFileInfo *info);
    void LoadEditorProjectFromEditor(QString projectPath);
    void LoadRecorderProjectFromEditor(QString projectPath);
    bool SaveAsNew(const QString newpath);
    bool Save();
    void QuitDontSave();

    ProjectFileInfo* GetUntitleProjectInfo();

public:
    ProjectAutoSaveMgr(QObject *parent);
    ~ProjectAutoSaveMgr();

    ProjectAutoSaveBase* GetMgr(MgrType type);
    ProjectAutoSaveBase* GetCurrentMgr();
    MgrType GetCurrentMgrType();
    QString ProjectPath2Directory(QString path);
    FSMessageBox::Button LoadNewProject(QString projectPath, bool recorderProjectFromEditor);
    void _LoadProjectFromEditor(QString projectPath, bool recorderProjectFromEditor);

private:
    QMap<int, ProjectAutoSaveBase*> m_autoSaveMgrMap;
};

class ProjectAutoSaveBase : public QObject
{
    Q_OBJECT
public:
    ProjectAutoSaveBase(ProjectAutoSaveMgr::MgrType type, QObject *parent);
    ~ProjectAutoSaveBase();

    virtual void Start();
    virtual void Finish();
    virtual ProjectFileInfo*  GetUntitleProjectInfo();

    void LoadProjectInfo(QString dir);
    void ClearProjectInfo();
    QString GetProjectInfoDirectory();
    QString GetProjectInfoXmlPath();
    QString GetUnititleProjectFilePath();

    bool IsStart();
    int GetType();

    bool IsUntitle();
    void SetUntitle(bool untitle);

    void Load(QString path);

    void Save(QString path);

protected:
    virtual void AutoSave();

    void AddNewAutoProject(ProjectFileInfo *info);
    void RemoveAutoProject(ProjectFileInfo *info);
private:

protected:
    ProjectFileInfos m_datas;
    ProjectFileInfo* m_currentData;
    int m_mgrType;
    bool m_bUntitle;
    QString m_projectInfoDirectory;
    QString m_projectInfoXmlPath;
    QString m_untitleProjectFilePath;

private:
    QTimer *m_timer;
    int m_maxProjectFileCount;
};

class UntitleAutoSave : public ProjectAutoSaveBase
{
    Q_OBJECT
public:
    UntitleAutoSave(QObject *parent);

private:


};

class TitleAutoSaver : public ProjectAutoSaveBase
{
    Q_OBJECT
public:
    TitleAutoSaver(QObject *parent);

};


#endif // PROJECTAUTOSAVEMGR_H
