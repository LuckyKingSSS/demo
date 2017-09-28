#ifndef FMOTIONLISTITEM_H
#define FMOTIONLISTITEM_H

#include <QListWidgetItem>

class FNLETimeline;
class FMotionListItem : public QListWidgetItem
{

public:
	FMotionListItem(const QIcon & icon, const QString & text,QString strFilePath);
	~FMotionListItem();

private:
	void InitData();
private:
	QString m_strFilePath;
	NLEComPtr<FNLETimeline> m_Timeline;
};

#endif // FMOTIONLISTITEM_H
