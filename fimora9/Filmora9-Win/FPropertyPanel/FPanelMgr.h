#ifndef FPANELMGR_H
#define FPANELMGR_H

#include <QObject>

class FNLETimeline;

class FPanelMgr : public QObject
{
	Q_OBJECT

public:
	FPanelMgr(QObject *parent);
	~FPanelMgr();

	void SetTimeline(FNLETimeline *);
	FNLETimeline* GetTimeline();
	void SetRootTimeline(FNLETimeline *);
	FNLETimeline* GetRootTimeline();

private:
	FNLETimeline *m_pTimeline;
	FNLETimeline *m_pRootTimeline;
};

#endif // FPANELMGR_H
