#ifndef FMOTIONPANNELMGR_H
#define FMOTIONPANNELMGR_H

#include <QObject>
#include <QList>

typedef struct
{
	QString strMotionName;
	QString strPicPath;
	QString strXmlPath;
}*pMotionInfo, MotionInfo;

class FMotionPannelMgr : public QObject
{
	Q_OBJECT

public:
	FMotionPannelMgr(QObject *parent);
	~FMotionPannelMgr();
	QList<pMotionInfo> GetAllResource(QString strFilePath);
	 
private:
	QList<pMotionInfo> m_AllResouce;
};

#endif // FMOTIONPANNELMGR_H
