#ifndef FSETTINGDATACONTR_H
#define FSETTINGDATACONTR_H

#include <QObject>
#include <QList>

class FSettingDataContr : public QObject
{
	Q_OBJECT

public:
	FSettingDataContr(QObject *parent);
	~FSettingDataContr();
	void initConfig();
	const QList<QString>& getDeviceConfig(){ return m_DeviceList; };
private:
	QList<QString> m_DeviceList;
};

#endif // FSETTINGDATACONTR_H
