#include "stdafx.h"
#include "SettingDataMgr.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_FSCommonlib/MultiLangMgr.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "Interface/C++/cudacheck/wsCUDA.h"
#include <QFile>
#include <QDir>

#include "CommunicatorClient.h"
#include "inc_FSCommonlib/DesktopRectangleMgr.h"

#define tag_name_savepath		"SavePath"
#define tag_name_language		"Language"
#define tag_name_autoupgrade	"AutoUpgrade"
#define tag_name_gpuacceleration "GpuAcceleration"
#define tag_name_snaptoguides "SnapToGuides"


SettingDataMgr::SettingDataMgr(QObject *parent)
	: QObject(parent)
{
    m_projectSettingMgr = new ProjectSettingsMgr(this);
	Reset();
	Load(GetPath());
}

SettingDataMgr::~SettingDataMgr()
{

}

SettingDataMgr* SettingDataMgr::GetInstance()
{
	static SettingDataMgr ins;
	return &ins;
}

void SettingDataMgr::Load(const QString &settingfile)
{
	QDomDocument doc;
	if (!XmlOperation::OpenXmlFile(doc, GetPath()))
		return;
	QDomElement rootElem = doc.documentElement();
	QDomElement subElem = rootElem.firstChildElement(tag_name_savepath);
	if (!subElem.isNull())
	{
		m_strProjSavePath =  subElem.text();
	}
	subElem = rootElem.firstChildElement(tag_name_language);
	if (!subElem.isNull())
	{
		XmlOperation::SetElemmentText(doc, subElem, m_langinfo.fullname);  // 写入语言配置
	}
	subElem = rootElem.firstChildElement(tag_name_autoupgrade);
	if (!subElem.isNull())
	{
		m_bAutoUpgrade = subElem.text() == "0" ? false : true;
	}
	subElem = rootElem.firstChildElement(tag_name_gpuacceleration);
	if (!subElem.isNull())
	{
		m_bGPUAcceleration = subElem.text() == "0" ? false : true;
	}
    subElem = rootElem.firstChildElement(tag_name_snaptoguides);
    if (!subElem.isNull())
    {
        m_bSnapToGuides = subElem.text() == "0" ? false : true;
    }

}

void SettingDataMgr::SetProjSavePath(const QString &path)
{
	m_strProjSavePath = path;
	QDomDocument doc;
	XmlOperation::OpenXmlFile(doc,GetPath());
	auto rootElem = doc.documentElement();
	auto savePathElem = rootElem.firstChildElement(tag_name_savepath);
	if (!savePathElem.isNull())
	{
		XmlOperation::SetElemmentText(doc, savePathElem, path);
	}
	XmlOperation::SaveXmlContent2File(doc, GetPath());
}

QString SettingDataMgr::GetProjSavePath()
{
	return m_strProjSavePath;
}

void SettingDataMgr::SetLang(const QString &fullname)
{
	if (m_langinfo.fullname != fullname){
		m_langinfo = GetLangInfo(fullname);
		MultiLangMgr::GetInstance()->SetLangLocale(m_langinfo.ll);
	}
}

QString SettingDataMgr::GetLangFullName()
{
	return m_langinfo.fullname;
}

LangConstInfo SettingDataMgr::GetLangInfo()
{
	return m_langinfo;
}

bool SettingDataMgr::CanGpuAccOn()
{
	AutoGetGpuCap gpu;
	ACCELERATETYPE type = (ACCELERATETYPE)gpu.GetGpuCap();
	bool bOn = (type == HARDWARE_ACCELERATE_CUDA
		|| type == HARDWARE_ACCELERATE_ATI
		|| type == HARDWARE_ACCELERATE_INTEL
		|| type == HARDWARE_ACCELERATE_NVENC
		|| type == HARDWARE_ACCELERATE_VCE);
	return bOn;
}

QString SettingDataMgr::GetConfigLangName()
{
	QString iniPath = FSGetFilmoraScreenDirectory() + "\\FilmoraScrn.ini";
	QSettings setting(iniPath, QSettings::IniFormat);
	setting.beginGroup("System");
	auto langname = setting.value("DefInstallLang").toString();
	setting.endGroup();
	auto info = GetLangInfoWithSimpleName(langname);
	return info.fullname;
}

void SettingDataMgr::SetAutoUpgrade(bool b)
{
	m_bAutoUpgrade = b;
}

bool SettingDataMgr::IsAutoUpgrade()
{
	return m_bAutoUpgrade;
}

void SettingDataMgr::SetGpuAcceleration(bool b)
{
	m_bGPUAcceleration = b;
}

bool SettingDataMgr::IsGpuAcceleration()
{
	return m_bGPUAcceleration;
}

void SettingDataMgr::SetSnapToGuides(bool b)
{
    m_bSnapToGuides = b;
    emit sigSnapToGuides(m_bSnapToGuides);
    Save();
}

bool SettingDataMgr::GetSnapToGuides()
{
    return m_bSnapToGuides;
}


ProjectSettingsMgr* SettingDataMgr::GetProejctSettingsMgr()
{
    return m_projectSettingMgr;
}

void SettingDataMgr::Reset()
{
	m_bAutoUpgrade = false;
	m_bGPUAcceleration = false;
	m_strProjSavePath = GetDefaultPath();
	m_langinfo = MultiLangMgr::GetInstance()->GetLangConstInfo();
    m_bSnapToGuides = true;
}

void SettingDataMgr::Save()
{
	QDomDocument doc;
    if (!XmlOperation::OpenXmlFile(doc, GetPath()))
    {
        //文件存在，但是不是xml文件或数据损坏将文件删除再重新创建
        if (!QFile::exists(GetPath()))
            return;
        else
        {
            QFile::remove(GetPath());
            if (!XmlOperation::OpenXmlFile(doc, GetPath()))
            {
                return;
            }
        }           
    }		
	QDomElement rootElem = doc.documentElement();
	QDomElement subElem = rootElem.firstChildElement(tag_name_savepath);
	if (subElem.isNull())
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_savepath, m_strProjSavePath);
	else
		XmlOperation::SetElemmentText(doc, subElem, m_strProjSavePath);

	subElem = rootElem.firstChildElement(tag_name_language);
	if (subElem.isNull())
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_language, m_langinfo.fullname);
	else 
		XmlOperation::SetElemmentText(doc, subElem, m_langinfo.fullname);

	subElem = rootElem.firstChildElement(tag_name_autoupgrade);
	if (subElem.isNull())
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_autoupgrade, m_bAutoUpgrade ? "1" : "0");
	else
		XmlOperation::SetElemmentText(doc, subElem, m_bAutoUpgrade ? "1" : "0");

	subElem = rootElem.firstChildElement(tag_name_gpuacceleration);
	if (subElem.isNull())
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_gpuacceleration, m_bGPUAcceleration ? "1" : "0");
	else
		XmlOperation::SetElemmentText(doc, subElem, m_bGPUAcceleration ? "1" : "0");

    subElem = rootElem.firstChildElement(tag_name_snaptoguides);
    if (subElem.isNull())
        XmlOperation::AddXmlElement(doc, rootElem, tag_name_snaptoguides, GetSnapToGuides() ? "1" : "0");
    else
        XmlOperation::SetElemmentText(doc, subElem, GetSnapToGuides() ? "1" : "0");



	XmlOperation::SaveXmlContent2File(doc, GetPath());
}

QString SettingDataMgr::GetDefaultPath()
{
	QString path = PathOperation::GetDocumentPath() + "/FilmoraScreen";
	QDir dir(path);
	if (!dir.exists())
	{
		dir.mkpath(path);
	}
	return path;
}

QString SettingDataMgr::GetPath()
{
	QString path = FSGetFilmoraScreenDirectory()+ "/settingdatas.xml";

	QFile file(path);
	QDomDocument doc;
	if (!file.exists())
	{
		XmlOperation::CreateXmlFile(path, "SettingDatas");
		if (!XmlOperation::OpenXmlFile(doc, path))
			return "";
		QDomElement rootElem = doc.documentElement();
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_savepath, GetDefaultPath());
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_language, GetConfigLangName());
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_autoupgrade, "1");
		XmlOperation::AddXmlElement(doc, rootElem, tag_name_gpuacceleration, CanGpuAccOn() ?  "1" : "0");

		XmlOperation::SaveXmlContent2File(doc, path);
	}
	else
	{
		if (!XmlOperation::OpenXmlFile(doc, path)) //损坏的xml文件
		{
			QFile::remove(path);
			XmlOperation::CreateXmlFile(path, "SettingDatas");
			if (!XmlOperation::OpenXmlFile(doc, path))
				return "";
			QDomElement rootElem = doc.documentElement();
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_savepath, GetDefaultPath());
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_language, GetConfigLangName());
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_autoupgrade, "1");
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_gpuacceleration, CanGpuAccOn() ? "1" : "0");
			return path;
		}
		QDomElement rootElem = doc.documentElement();
		auto elem = rootElem.firstChildElement(tag_name_savepath);
		if (elem.isNull())
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_savepath, GetDefaultPath());

		elem = rootElem.firstChildElement(tag_name_language);
		if (elem.isNull())
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_language, GetConfigLangName());

		elem = rootElem.firstChildElement(tag_name_autoupgrade);
		if (elem.isNull())
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_autoupgrade, "1");

		elem = rootElem.firstChildElement(tag_name_gpuacceleration);
		if (elem.isNull())
			XmlOperation::AddXmlElement(doc, rootElem, tag_name_gpuacceleration, CanGpuAccOn() ? "1" : "0");

		XmlOperation::SaveXmlContent2File(doc, path);
	}
	return path;
}

LangConstInfo SettingDataMgr::GetLangInfo(const QString &fullname)
{
	for (int i = 0; i < sizeof(g_LangInfos) / sizeof(LangConstInfo);i++)
	{
		if (g_LangInfos[i].fullname == fullname)
		{
			return g_LangInfos[i];
		}
	}
	return LangConstInfo();
}

LangConstInfo SettingDataMgr::GetLangInfoWithSimpleName(const QString &simplename)
{
	for (int i = 0; i < sizeof(g_LangInfos) / sizeof(LangConstInfo); i++)
	{
		if (g_LangInfos[i].simplename1 == simplename ||
			g_LangInfos[i].simplename2 == simplename)
		{
			return g_LangInfos[i];
		}
	}
	return LangConstInfo();
}

//////////////////////////////////////////////////////////////////////////


#define tag_name_exportname "ExportName"
#define tag_name_resolutionwidth "ResolutionWidth"
#define tag_name_resolutionheight "ResolutionHeight"
#define tag_name_playbackresolution "PlaybackResolution"
#define tag_name_backgroundcolor "BackgroundColor"


ProjectSettingsMgr::ProjectSettingsMgr(QObject *parent)
:QObject(parent)
{
    Reset();
    connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigProjectSettingsLoaded, this, &ProjectSettingsMgr::Load);

}

void ProjectSettingsMgr::NewProject()
{
    Reset();
    sigResolutionChanged(GetResolution());
    sigPlaybackResoultionChagned(GetPlaybackResoultion());
    sigExportNameChagned(GetExportName());
    sigBackgroundColorChanged(GetBackgroundColor());
}

void ProjectSettingsMgr::Reset()
{
    m_projectName = "My Video";
    m_resoultion = DesktopRectangleMgr::GetInstance()->PrimaryScreen().size();

    m_playback = 0.5f;
    m_lockAspect = true;
    m_backgroundColor = Qt::black;
}

QString ProjectSettingsMgr::GetExportName()
{
    return m_projectName;
}

void ProjectSettingsMgr::SetExportName(QString name)
{
    m_projectName = name;
}

QSize ProjectSettingsMgr::GetResolution()
{
    return m_resoultion;
}

void ProjectSettingsMgr::SetResoultion(QSize size)
{
    m_resoultion = size;
}

float ProjectSettingsMgr::GetPlaybackResoultion()
{
    return m_playback;
}

void ProjectSettingsMgr::SetPlaybackResoultion(float rate)
{
    m_playback = rate;
}

bool ProjectSettingsMgr::GetLockAspect()
{
    return m_lockAspect;
}

void ProjectSettingsMgr::SetLockAspect(bool lock)
{
    m_lockAspect = lock;
}



void ProjectSettingsMgr::SetBackgroundColor(QColor color)
{
    m_backgroundColor = color;
}

QColor ProjectSettingsMgr::GetBackgroundColor()
{
    return m_backgroundColor;
}

void ProjectSettingsMgr::Load(QString xml)
{
    Reset();
    QDomDocument doc("");
    if (!doc.setContent(xml))
    {
        return;
    }

    auto docElem = doc.documentElement();

    auto projectNameElem = docElem.firstChildElement(tag_name_exportname);
    if (!projectNameElem.isNull())
    {
        SetExportName(projectNameElem.toElement().text());
    }
    auto resoultonWidthElem = docElem.firstChildElement(tag_name_resolutionwidth);
    auto resoultonHeightElem = docElem.firstChildElement(tag_name_resolutionheight);

    if (!resoultonWidthElem.isNull() && !resoultonHeightElem.isNull())
    {
        QSize temp = QSize(resoultonWidthElem.text().toInt(), resoultonHeightElem.text().toInt());
        if (!temp.isValid())
            temp = QSize(1920, 1080);
        SetResoultion(temp);
    }

    auto playbackElem = docElem.firstChildElement(tag_name_playbackresolution);
    if (!playbackElem.isNull())
    {
        SetPlaybackResoultion(playbackElem.text().toFloat());
    }
    auto backgroundColorElem = docElem.firstChildElement(tag_name_backgroundcolor);
    if (!backgroundColorElem.isNull())
    {
        SetBackgroundColor(QColor(backgroundColorElem.text()));
    }

}

QString ProjectSettingsMgr::GetProjectString()
{
    QString settingsXml("<ProjectSettings>");

    settingsXml += QString("<%1>%2</%1>").arg(tag_name_exportname).arg(GetExportName());
    settingsXml += QString("<%1>%2</%1>").arg(tag_name_resolutionwidth).arg(GetResolution().width());
    settingsXml += QString("<%1>%2</%1>").arg(tag_name_resolutionheight).arg(GetResolution().height());
    settingsXml += QString("<%1>%2</%1>").arg(tag_name_playbackresolution).arg(GetPlaybackResoultion());
    settingsXml += QString("<%1>%2</%1>").arg(tag_name_backgroundcolor).arg(GetBackgroundColor().name());

    settingsXml += QString("</ProjectSettings>");

    return settingsXml;
}
