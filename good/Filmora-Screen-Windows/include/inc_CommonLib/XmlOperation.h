#pragma once
#include "commonlib_global.h"
#include <QtXml/QDomDocument>

class COMMONLIB_EXPORT XmlOperation
{
public:
	XmlOperation();
	~XmlOperation();
	
	static void CreateXmlFile(const QString &filePath,const QString &rootElemName);
	static void SaveXmlContent2File(QDomDocument &doc, const QString &filePath);
	static bool OpenXmlFile(QDomDocument &doc, const QString &strFile);
	static QDomElement AddXmlElement(QDomDocument &doc, QDomElement& parentElem, const QString &strTagName, const QString &strText);
	static void SetElemmentText(QDomDocument &doc, QDomElement& elem, const QString &strText);
};

