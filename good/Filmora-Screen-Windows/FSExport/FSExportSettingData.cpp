#include "stdafx.h"
#include "FSExportSettingData.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/XmlOperation.h"




QString sGetDefaultLocalFilePath()
{
    QString filePath = QApplication::applicationDirPath() + "/Data/ExportFiles";

    QStringList dirs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if (dirs.size() > 0)
    {
        filePath = dirs[0] + "/FilmoraScreen";
        QDir ddd(filePath);
        if (!ddd.exists())
        {
            ddd.mkpath(filePath);
        }
        filePath = filePath + "/ExportFiles";
        QDir ddd1(filePath);
        if (!ddd1.exists())
        {
            ddd1.mkpath(filePath);
        }
    }
    return filePath;
}

QDomElement sExportGetChildElementByTag(QDomElement parent, QString tag)
{
    auto elems = parent.elementsByTagName(tag);
    if (!elems.isEmpty())
    {
        return elems.at(0).toElement();
    }
    return QDomElement();
}


bool sString2Bool(const QString & str)
{
    return str.toInt() != 0;
}

QString sBool2String(const bool & flag){
    return flag ? "1" : "0";
}

static FSExportSettingDataMgr *s_pSettingsDataManager = NULL;
FSExportSettingDataMgr * FSExportSettingDataMgr::GetInstance()
{
    if (!s_pSettingsDataManager)
    {
        s_pSettingsDataManager = new FSExportSettingDataMgr(NULL);
    }
    return s_pSettingsDataManager;
}

void FSExportSettingDataMgr::Release()
{
    delete s_pSettingsDataManager;
    s_pSettingsDataManager = NULL;
}

FSExportLocalSettingData * FSExportSettingDataMgr::GetLocalSettingData()
{
    return m_pLocalData;
}

FSExportSettingDataMgr::FSExportSettingDataMgr(QObject *parent)
    : QObject(parent)
    , m_RootTagName("ExportSetting")
    , m_GpuName("GpuAcceleration")
    , m_bEnableGpu(true)
{
    m_pLocalData = new FSExportLocalSettingData("ExportLocal",this);
    m_SettingDataList.push_back(m_pLocalData);
}

FSExportSettingDataMgr::~FSExportSettingDataMgr()
{

}

void FSExportSettingDataMgr::LoadingExportSetting(const QString &rootStr)
{
    QDomDocument doc;
    doc.setContent(rootStr);
    QDomElement rootElement = doc.documentElement();
    rootElement = rootElement.firstChildElement(m_RootTagName);
    QDomElement GpuElement = doc.documentElement().firstChildElement(m_GpuName);
    QDomElement Elem;
    if (!rootElement.isNull()){
        for (auto item:m_SettingDataList)
        {
            Elem = sExportGetChildElementByTag(rootElement, item->RootTagName());
            item->LoadingSetting(Elem);
        }
    }
    if (!GpuElement.isNull())
    {
        m_bEnableGpu = sString2Bool(GpuElement.text());
    }
}


void FSExportSettingDataMgr::SaveExportSettingData(const QString filepath)
{
    QDomDocument doc;
    if (QFile::exists(filepath))
    {
        if (!XmlOperation::OpenXmlFile(doc, filepath))
        {
            QFile::remove(filepath);
            XmlOperation::CreateXmlFile(filepath, "SettingDatas");
            if (!XmlOperation::OpenXmlFile(doc, filepath))
                return;
        }
    }
    else
    {
        XmlOperation::CreateXmlFile(filepath, "SettingDatas");
        if (!XmlOperation::OpenXmlFile(doc, filepath))
            return;
    }
    QDomElement  item = doc.documentElement();
    item = item.firstChildElement(RootTagName());
    if (!item.isNull())
    {
        doc.documentElement().removeChild(item);
    }
    item = doc.createElement(RootTagName());
    doc.documentElement().appendChild(item);
    for (auto itemTEMP:m_SettingDataList)
    {
        QDomElement  itemSub;

        itemTEMP->GetXmlSetting(itemSub);
        item.appendChild(itemSub);
    }
    XmlOperation::SaveXmlContent2File(doc, filepath);
}

QString FSExportSettingDataMgr::RootTagName()
{
    return m_RootTagName;
}

bool FSExportSettingDataMgr::IsEnabledGPU()
{
    return m_bEnableGpu;
}

////////////////////////////////////////////////////////////////////////////////////
IFSExportSettingData::IFSExportSettingData(QString rootTagName, QObject *pParent)
    :QObject(pParent)
    , m_RootTagName(rootTagName)
{

}

IFSExportSettingData::~IFSExportSettingData()
{

}

QString IFSExportSettingData::RootTagName()
{
    return m_RootTagName;
}

////////////////////////////////////////////////////////////////////////////////////
FSExportLocalSettingData::FSExportLocalSettingData(QString rootTagName, QObject *pParent)
    :IFSExportSettingData(rootTagName,pParent),
    m_bShowMessageBox(true),
    m_LocalFilePath(sGetDefaultLocalFilePath())
{
    m_LocalFileTabName = "LocalFilePath";
    m_bShowMessageBoxTagName = "bShowMessageBox";
}

FSExportLocalSettingData::~FSExportLocalSettingData()
{

}

void FSExportLocalSettingData::LoadingSetting(const QDomElement& xmlElement)
{
    QDomElement elem;
    if (!xmlElement.isNull())
    {
        elem = xmlElement.firstChildElement(m_LocalFileTabName);
        if (!elem.isNull())
        {
            m_LocalFilePath = elem.text();
        }
        elem = xmlElement.firstChildElement(m_bShowMessageBoxTagName);
        if (!elem.isNull())
        {
            m_bShowMessageBox = sString2Bool(elem.text());
        }
    }
}

void FSExportLocalSettingData::GetXmlSetting(QDomElement &elementValue)
{
    QDomDocument doc;
    QDomElement element = doc.createElement(RootTagName());
    doc.appendChild(element);
    QDomElement elementFilePath = doc.createElement(m_LocalFileTabName);
    QDomText LocalFilePath = doc.createTextNode(m_LocalFilePath);
    elementFilePath.appendChild(LocalFilePath);

    QDomElement elementMessageBox = doc.createElement(m_bShowMessageBoxTagName);
    QDomText bMessageBoxShow = doc.createTextNode(sBool2String(m_bShowMessageBox));
    elementMessageBox.appendChild(bMessageBoxShow);

    element.appendChild(elementFilePath);
    element.appendChild(elementMessageBox);
    elementValue = element;
}

bool FSExportLocalSettingData::GetShowMessageBoxFlag()
{
    return m_bShowMessageBox;
}

void FSExportLocalSettingData::SetShowMessageBoxFlag(const bool& bShow)
{
    m_bShowMessageBox = bShow;
}

QString FSExportLocalSettingData::GetLocalFilePath()
{
    return m_LocalFilePath;
}

void FSExportLocalSettingData::SetLocalFilePath(const QString &filePath)
{
    m_LocalFilePath = filePath;
}

