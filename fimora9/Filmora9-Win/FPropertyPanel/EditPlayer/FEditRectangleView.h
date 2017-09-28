#ifndef FEDITRECTANGLEVIEW_H
#define FEDITRECTANGLEVIEW_H

#include "inc_FCommonLib/FBaseWidget.h"
//class FNLETimeline;
class FEditPlayerMgr;

class FEditRectangleView : public FBaseWidget
{
	Q_OBJECT

public:
	FEditRectangleView(FEditPlayerMgr *mgr, QWidget *parent = 0);
	~FEditRectangleView();

	//void SetTimelines(QList<FNLETimeline*>);

signals:
	//void sig

protected:
	void paintEvent(QPaintEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;

private:
	FEditPlayerMgr *m_pMgr;
	//QList<FNLETimeline*> m_TimelinesLst;
};

#endif // FEDITRECTANGLEVIEW_H
