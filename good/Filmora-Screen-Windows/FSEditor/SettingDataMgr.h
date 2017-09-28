#ifndef SETTINGDATAMGR_H
#define SETTINGDATAMGR_H

#include <QObject>
#include <QSize>
#include <QColor>
#include "inc_FSCommonlib/MultiLangMgr.h"

class ProjectSettingsMgr;
class SettingDataMgr : public QObject
{
	Q_OBJECT

public:
	static SettingDataMgr* GetInstance();
	~SettingDataMgr();
	void Load(const QString &settingfile);
	void Save();

	void SetProjSavePath(const QString &path);
	QString GetProjSavePath();

	void SetLang(const QString &lang);
	QString GetLangFullName();
	LangConstInfo GetLangInfo();

	void SetAutoUpgrade(bool b);
	bool IsAutoUpgrade();

	void SetGpuAcceleration(bool b);
	bool IsGpuAcceleration();

    void SetSnapToGuides(bool b);
    bool GetSnapToGuides();



    ProjectSettingsMgr* GetProejctSettingsMgr();

Q_SIGNALS:
    void sigOk();
    void sigCancel();

    void sigSnapToGuides(bool enabled);
protected:
	void Reset();
	QString GetDefaultPath();
	QString GetPath();
	LangConstInfo GetLangInfo(const QString &fullname);
	LangConstInfo GetLangInfoWithSimpleName(const QString &simplename);
	bool CanGpuAccOn();
	QString GetConfigLangName();
private:
	SettingDataMgr(QObject *parent = 0);
	QString m_strProjSavePath;
	bool m_bAutoUpgrade;
	bool m_bGPUAcceleration;
	LangConstInfo m_langinfo;
    bool m_bSnapToGuides;

    ProjectSettingsMgr *m_projectSettingMgr;
};


class ProjectSettingsMgr : public QObject
{
    Q_OBJECT
public:

    QString GetExportName();
    void SetExportName(QString name);

    QSize GetResolution();
    void SetResoultion(QSize size);

    float GetPlaybackResoultion();
    void SetPlaybackResoultion(float rate);

    bool GetLockAspect();
    void SetLockAspect(bool lock);

    void SetBackgroundColor(QColor color);
    QColor GetBackgroundColor();

    void Load(QString xml);
    QString GetProjectString();

Q_SIGNALS:
    void sigResolutionChanged(QSize size);
    void sigPlaybackResoultionChagned(float ratio);
    void sigExportNameChagned(QString projectName);
    void sigBackgroundColorChanged(QColor color);

public:
    ProjectSettingsMgr(QObject *parent);
    void NewProject();
private:
    void Reset();

private:
    QString m_projectName;
    QSize m_resoultion;
    float m_playback;
    bool m_lockAspect;
    QColor m_backgroundColor;

};

#endif // SETTINGDATAMGR_H
