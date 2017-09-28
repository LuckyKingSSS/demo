#include "stdafx.h"
#include "XmlOperation.h"
#include "FileOperation.h"
#include <QFile>

XmlOperation::XmlOperation()
{
}


XmlOperation::~XmlOperation()
{
} 

void XmlOperation::CreateXmlFile(const QString &filePath, const QString &rootElemName)
{
	if (FileOperation::IsFileExist(filePath)) return;

	QDomDocument doc;
	QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);

	QDomElement root = doc.createElement(rootElemName);
	doc.appendChild(root);

	SaveXmlContent2File(doc, filePath);
}

void XmlOperation::SaveXmlContent2File(QDomDocument &doc, const QString &filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return;
	QTextStream outStream(&file);
	outStream.setCodec("UTF-8");
	doc.save(outStream, 4);
	file.close();
}

bool XmlOperation::OpenXmlFile(QDomDocument &doc, const QString &strFile)
{
	QFile file(strFile);
	QString strError = "";
	int nRow = 0, nColumn = 0;
	if (!file.open(QIODevice::ReadWrite)) return false;
	if (!doc.setContent(&file, false, &strError, &nRow, &nColumn))
	{
		file.close();
		return false;
	}
	file.close();
	return true;
}

QDomElement XmlOperation::AddXmlElement(QDomDocument &doc, QDomElement& parentElem, const QString &strTagName, const QString &strText)
{
	QDomElement domElem = doc.createElement(strTagName);
	 
	QDomText domText = doc.createTextNode(strText);
	domElem.appendChild(domText);
	parentElem.appendChild(domElem);

	return domElem;
}

void XmlOperation::SetElemmentText(QDomDocument &doc, QDomElement& elem, const QString &strText)
{
	auto childs = elem.childNodes();
	for (int i = 0; i < childs.size(); ++i)
	{
		auto c = childs.at(i);
		if (c.isText())
		{
			elem.removeChild(c);
		}
	}
	QDomText domText = doc.createTextNode(strText);
	elem.appendChild(domText);
}
