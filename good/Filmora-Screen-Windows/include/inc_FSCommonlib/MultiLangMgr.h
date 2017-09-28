#ifndef MULTILANGMGR_H
#define MULTILANGMGR_H
#include "inc_FSCommonlib/fscommonlib_global.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_FSCommonlib/CBSUrlReader.h"
#include <QObject>
#include <QTranslator>

enum Language_Locale{
    Language_Locale_none,
	Language_Locale_en,
	Language_Locale_ja,
	Language_Locale_de,
	Language_Locale_fr,
	Language_Locale_pt,
	Language_Locale_es,
	Language_Locale_it,
	Language_Locale_zh_tw,
	Language_Locale_ru,
	Language_Locale_nl,
	Language_Locale_ar,
};

struct LangConstInfo
{
	Language_Locale ll;
	QString dirname;
	QString simplename1;
	QString simplename2;
	QString fullname;
};

const LangConstInfo g_LangInfos[] = {
	{ Language_Locale_en, "en", "ENG", "", "English" },
	{ Language_Locale_de, "de", "DEU", "", "German" },
	{ Language_Locale_es, "es", "ESP", "ESM", "Spanish" },
	{ Language_Locale_fr, "fr", "FRA", "", "French" },
	{ Language_Locale_it, "it", "ITA", "", "Italian" },
	{ Language_Locale_pt, "pt", "PTG", "", "Portuguese" },
	{ Language_Locale_ja, "ja", "JPN", "", "Japenese" },
	//{ nguage_Locale_zh_tw, "zh_tw", "ZHH", "CHT", "" },
	//{ Language_Locale_ru, "ru", "RUS", "", "" },
	//{ Language_Locale_nl, "nl", "NLD", "", "" }
	//{ Language_Locale_ar, "ar", "ARG", "", "" }
};

struct OneLang
{
	Language_Locale m_lang;
	QList<QTranslator*> m_lstTranslators;
	OneLang(Language_Locale lang);
	OneLang(QString simpleName);
	void Init(Language_Locale lang);
	~OneLang();
};

class FSCOMMONLIB_EXPORT MultiLangMgr : public QObject
{
	Q_OBJECT

public:
	static MultiLangMgr *GetInstance();
	~MultiLangMgr();

	void SetLangLocale();
	void SetLangLocale(Language_Locale lang);
	void SetLangLocale(QString fullName);
	Language_Locale GetLangLocale();
	LangConstInfo GetLangConstInfo();
	QString GetSimpleName1();
	QString GetSimpleName2();

protected:
	MultiLangMgr(QObject *parent = nullptr);
	void Init();
	void RemoveCurrentLang();
	void LoadCurrentLang();

private:
	QList<OneLang*> m_lstLangs;
	Language_Locale m_curLang;
	CBSUrlReader *m_CBSUrlReader;
};

#endif // MULTILANGMGR_H
