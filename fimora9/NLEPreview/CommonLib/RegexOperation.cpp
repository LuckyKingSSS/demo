#include "stdafx.h"
#include "RegexOperation.h"
#include <QRegExp>

RegexOperation::RegexOperation()
{
}


RegexOperation::~RegexOperation()
{
}

bool RegexOperation::RegexFindString(const QString &strSrc, const QString &strPattern, QString &strFinded)
{
	QRegExp rx(strPattern);
	int nPos = strSrc.indexOf(rx);
	if (nPos >= 0)
	{
		strFinded = rx.cap(1); 
		return true;
	}
	return false;
}