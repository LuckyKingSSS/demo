#include "stdafx.h"
#include "StringOperation.h"

StringOperation::StringOperation()
{
}


StringOperation::~StringOperation()
{
}

QString StringOperation::GetElidedString(QFont font, int len, const QString &text)
{
	QFontMetrics metrics(font);
	return metrics.elidedText(text, Qt::ElideRight, len);
}

int StringOperation::GetTextTrueLenght(QFont font, const QString &text)
{
	QFontMetrics metrics(font);
	return metrics.width(text);
}

QString StringOperation::GetMenuString(QFont menuFont, const QString &titleText, const QString &hotkeyText, int showlen)
{
	if (hotkeyText.isEmpty()) return titleText;
	int len1 = GetTextTrueLenght(menuFont,titleText);
	int len2 = GetTextTrueLenght(menuFont, hotkeyText);

	QString midStr = "";
	int midLen = 0;

	while (len1 + len2 + midLen < showlen)
	{
		midStr += " ";
		midLen = GetTextTrueLenght(menuFont, midStr);
	}
	return titleText + midStr + hotkeyText;
}
