#ifndef F3DLUTDATAMGR_H
#define F3DLUTDATAMGR_H

#include <QObject>
#include <QStringList>

class F3DLUTDataMgr : public QObject
{
	Q_OBJECT

public:
	F3DLUTDataMgr(QObject *parent);
	~F3DLUTDataMgr();
	QStringList GetDataToLoad(){ return m_DataList; };
private:
	void GetLocalConfig();

private:
	QStringList m_DataList;
};

#endif // F3DLUTDATAMGR_H
