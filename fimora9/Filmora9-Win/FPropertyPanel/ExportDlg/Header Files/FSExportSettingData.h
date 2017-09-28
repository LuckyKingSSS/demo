#ifndef FSEXPORTSETTINGDATA_H
#define FSEXPORTSETTINGDATA_H

#include <QObject>
#include <QtXml/QDomDocument>

class IFSExportSettingData :public QObject{
    Q_OBJECT
public:
    IFSExportSettingData(QString rootTagName,QObject *pParent);
    ~IFSExportSettingData();
    virtual void LoadingSetting(const QDomElement &xmlElement){ return; };
    virtual void GetXmlSetting(QDomElement &element){};
    QString RootTagName();
private:
    QString m_RootTagName;
};

class FSExportLocalSettingData :public IFSExportSettingData{
    Q_OBJECT
public:
    FSExportLocalSettingData(QString rootTagName, QObject *pParent);
    ~FSExportLocalSettingData();

    virtual void LoadingSetting(const QDomElement & xmlElement) override;
    virtual void GetXmlSetting(QDomElement &element) override;
    bool GetShowMessageBoxFlag();
    void SetShowMessageBoxFlag(const bool &bShow);
    QString GetLocalFilePath();
    void    SetLocalFilePath(const QString &filePath);

private:
    bool    m_bShowMessageBox;
    QString m_LocalFilePath;
    QString m_LocalFileTabName;
    QString m_bShowMessageBoxTagName;
};


class FSExportSettingDataMgr : public QObject
{
    Q_OBJECT

public:
    static FSExportSettingDataMgr *GetInstance();
    void Release();
    FSExportLocalSettingData * GetLocalSettingData();
    void LoadingExportSetting(const QString &rootStr);
    void SaveExportSettingData(const QString filepath);
    QString RootTagName();
    bool IsEnabledGPU();

private:
    FSExportSettingDataMgr(QObject *parent);
    ~FSExportSettingDataMgr();


private:
    QString                        m_RootTagName;
    QString                        m_GpuName;
    QList<IFSExportSettingData *>  m_SettingDataList;
    FSExportLocalSettingData      *m_pLocalData;
    bool                           m_bEnableGpu;  
};

#endif // FSEXPORTSETTINGDATA_H
