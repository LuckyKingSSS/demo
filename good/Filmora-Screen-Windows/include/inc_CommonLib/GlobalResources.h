#pragma once

#include "inc_CommonLib/commonlib_global.h"

#include <QList>
#include <QString>

// 环境管理类
class INLEFactory;
class COMMONLIB_EXPORT ICaptionEnvironment/* : public QObject*/
{
public:
	typedef QList<std::tuple<QString, QString>> CaptionFonts;
public:
	static ICaptionEnvironment* GetInstance();
	virtual void Release() = 0;
	virtual INLEFactory* GetNLEFactory() = 0;
	virtual CaptionFonts GetNLEFonts() = 0;

//	CaptionEnvironment(QObject *parent);
//private:
//	INLEFactory *m_pFactory;
//	NLEComPtr<INLEFontLibrary> m_pFontLibrary;
//
//	void InitSystemFont();
//	QList<std::tuple<QString, QString>> m_fonts;
};
