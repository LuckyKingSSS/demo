#pragma  once

#include "inc_CommonLib/GlobalResources.h"

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include <NLEComPtr.h>
#include "INLEFontLibrary.h"

class  CaptionEnvironment : public ICaptionEnvironment
{
public:
	//static CaptionEnvironment* GetInstance();
	void Release() override;

	INLEFactory* GetNLEFactory() override;
	CaptionFonts GetNLEFonts() override;

	CaptionEnvironment();
private:
	INLEFactory *m_pFactory;
	NLEComPtr<INLEFontLibrary> m_pFontLibrary;

	void InitSystemFont();
	QList<std::tuple<QString, QString>> m_fonts;
};




