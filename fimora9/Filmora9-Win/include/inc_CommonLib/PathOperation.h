#pragma once
#include "commonlib_global.h"
class COMMONLIB_EXPORT PathOperation
{
public:
	PathOperation();
	~PathOperation();

	static QString MakeWorkPath(const QString &strSubPath);  //在工作目录后面拼接strSubPath路劲

	static QString GetUserSelectDir(QString title,WId wid);

	static QString GetTempDir();  
	static QString GetDocumentPath();
	
};

