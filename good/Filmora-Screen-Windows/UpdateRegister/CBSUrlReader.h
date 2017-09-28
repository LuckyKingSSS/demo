#ifndef CBSURLREADER_H
#define CBSURLREADER_H

#include <QObject>

class CBSUrlReader : public QObject
{
	Q_OBJECT

public:
	CBSUrlReader(QObject *parent);
	~CBSUrlReader();
	QString ReadCustomerUrl(const QString &langName, const QString &key);

private:
	bool LoadUtilitiesLib();
	bool LoadXmlFile();
	void Init();

private:
	HMODULE m_LibModual;
};

#endif // CBSURLREADER_H
