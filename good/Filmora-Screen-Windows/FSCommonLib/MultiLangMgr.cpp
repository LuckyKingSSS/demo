#include "stdafx.h"
#include "inc_FSCommonlib/MultiLangMgr.h"

/*OneLang*/
OneLang::OneLang(Language_Locale lang)
{
	Init(lang);
}

OneLang::OneLang(QString simpleName)
{
	m_lang = Language_Locale_en;
	if (!simpleName.isEmpty()){
		for (auto langInfo : g_LangInfos){
			if ((simpleName == langInfo.simplename1) || (simpleName == langInfo.simplename2)){
				m_lang = langInfo.ll;
				break;
			}
		}
	}
	Init(m_lang);
}

void OneLang::Init(Language_Locale lang)
{
	QString langDir("");
	for (auto langInfo : g_LangInfos){
		if (lang == langInfo.ll){
			m_lang = lang;
			langDir = langInfo.dirname;
			break;
		}
	}
	if (langDir.isEmpty()) {
		m_lang = Language_Locale_en;
		langDir = "en";
	}
	langDir = qApp->applicationDirPath() + "/" + "Lang/" + langDir + "/";

	QDir dir(langDir);
	QStringList filelist = dir.entryList(QDir::Files);

	for (auto file : filelist)
	{
		QTranslator *pTrans = new QTranslator;
		pTrans->load(langDir + file);
		m_lstTranslators.push_back(pTrans);
	}
}

OneLang::~OneLang()
{
	for (auto translator : m_lstTranslators){
		delete translator;
	}
}

/*MultiLangMgr*/
MultiLangMgr::MultiLangMgr(QObject *parent)
	: QObject(parent)
{
    m_curLang = Language_Locale_none;  // 默认英文
	m_CBSUrlReader = new CBSUrlReader(this);
	Init();
}

void MultiLangMgr::Init()
{
	for (auto langInfo : g_LangInfos){
		OneLang *lang = new OneLang(langInfo.ll);
		m_lstLangs.push_back(lang);
	}
}

void MultiLangMgr::RemoveCurrentLang()
{
	for (auto lang : m_lstLangs){
		if (m_curLang == lang->m_lang){
			for (auto translator : lang->m_lstTranslators)
				qApp->removeTranslator(translator);
		}
	}
}

void MultiLangMgr::LoadCurrentLang()
{
	for (auto lang : m_lstLangs){
		if (m_curLang == lang->m_lang){
			/*写入配置文件*/
			QString configFilePath = FSGetFilmoraScreenDirectory() + "/" + "FilmoraScrn.ini";
			QFile configFile(configFilePath);
			if (!configFile.exists()){
				if (configFile.open(QIODevice::ReadWrite)){  // create file
					configFile.close();
				}
			}
			QSettings settings(configFilePath, QSettings::IniFormat);
			settings.beginGroup("System");
			settings.setValue("DefInstallLang", GetSimpleName1());  // 写入语言
			settings.setValue("ProdID", m_CBSUrlReader->ReadCustomerUrl(GetSimpleName1(), "PID"));  // 写入PID
			settings.endGroup();
			/*安装*/
			for (auto translator : lang->m_lstTranslators)
				qApp->installTranslator(translator);
		}
	}
}

MultiLangMgr * MultiLangMgr::GetInstance()
{
	static MultiLangMgr instance;
	return &instance;
}

MultiLangMgr::~MultiLangMgr()
{
	for (auto lang : m_lstLangs){
		delete lang;
	}
}

void MultiLangMgr::SetLangLocale(Language_Locale lang)
{
	if (m_curLang != lang){
		RemoveCurrentLang();
		m_curLang = lang;
		LoadCurrentLang();
	}
}

void MultiLangMgr::SetLangLocale(QString fullName)
{
	for (auto langInfo : g_LangInfos){
		if (langInfo.fullname == fullName){
			SetLangLocale(langInfo.ll);
			break;
		}
	}
}

void MultiLangMgr::SetLangLocale()
{
	/*从配置文件读入语言设置*/
	QString configFilePath = FSGetFilmoraScreenDirectory() + "/" + "FilmoraScrn.ini";
	QFile configFile(configFilePath);
	if (!configFile.exists()){
		if (configFile.open(QIODevice::ReadWrite)){  // create file
			configFile.close();
		}
	}
	QSettings settings(configFilePath, QSettings::IniFormat);
	settings.beginGroup("System");
	QString simpleName = settings.value("DefInstallLang").toString();
	settings.endGroup();
	Language_Locale lang = Language_Locale_en;
	if (!simpleName.isEmpty()){
		for (auto langInfo : g_LangInfos){
			if ((simpleName == langInfo.simplename1) || (simpleName == langInfo.simplename2)){
				lang = langInfo.ll;
				break;
			}
		}
	}
	SetLangLocale(lang);
}

QString MultiLangMgr::GetSimpleName1()
{
	for (auto lang : g_LangInfos){
		if (lang.ll == m_curLang){
			return lang.simplename1;
		}
	}
	return "";
}

QString MultiLangMgr::GetSimpleName2()
{
	for (auto lang : g_LangInfos){
		if (lang.ll == m_curLang){
			return lang.simplename2;
		}
	}
	return "";
}

Language_Locale MultiLangMgr::GetLangLocale()
{
	return m_curLang;
}

LangConstInfo MultiLangMgr::GetLangConstInfo()
{
	for (auto langInfo : g_LangInfos){
		if (langInfo.ll == m_curLang){
			return langInfo;
		}
	}
	return g_LangInfos[0];  // 英文
}




