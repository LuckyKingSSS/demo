#include "stdafx.h"
#include "InternalAPI.h"

int InternalAPI::GetRetranslateUiTextLength(QFont font, QString text, int minWidth, int extraWidth)
{
	const int btnGoWidth = minWidth;
	int btnExportNeedWidth = GetTextTrueLength(font, text);
	if (btnExportNeedWidth > btnGoWidth)
		btnExportNeedWidth += extraWidth;
	else
		btnExportNeedWidth = btnGoWidth;

	return btnExportNeedWidth;
}

int InternalAPI::GetTextTrueLength(QFont font, const QString &text)
{
	QFontMetrics metrics(font);
	return metrics.width(text);
}

QString InternalAPI::GetProgramDataPath()
{
	QString filmoraScrnDir = QProcessEnvironment::systemEnvironment().value("ProgramData");
	filmoraScrnDir += QString("/Wondershare Filmora Scrn");
	filmoraScrnDir = QDir::toNativeSeparators(filmoraScrnDir);
	QDir dir(filmoraScrnDir);
	if (!dir.exists()){
		dir.mkpath(filmoraScrnDir);
	}
	return filmoraScrnDir;
}





