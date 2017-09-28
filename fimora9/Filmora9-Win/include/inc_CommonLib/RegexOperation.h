#pragma once
#include "commonlib_global.h"
#include <QString>

class COMMONLIB_EXPORT RegexOperation
{
public:
	RegexOperation();
	~RegexOperation();

	static bool RegexFindString(const QString &strSrc, const QString &strPattern, QString &strFinded);
};

