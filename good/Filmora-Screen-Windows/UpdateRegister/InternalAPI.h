#pragma once

class InternalAPI
{
public:
	static int GetRetranslateUiTextLength(QFont font, QString text, int minWidth, int extraWidth);
	static int GetTextTrueLength(QFont font, const QString &text);
	static QString GetProgramDataPath();
};


