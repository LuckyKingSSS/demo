#ifndef TVDRAW_H
#define TVDRAW_H

#include <QObject>

class TimelineTrack;
class TimelineRuler;
class TrackWidget;
class TimelineManager;
class TVTrackWidgetDraw : public QObject
{
	Q_OBJECT

public:
	TVTrackWidgetDraw(QObject *parent);
	void SetTrackWidget(TrackWidget *pWidget);
	void SetRuler(TimelineRuler *pRuler);
	void Draw(QPainter *painter,QRect rc);

	QSize GetMarkerSize();
	int GetMarkerTop();
private:
	TrackWidget *m_pTrackWidget;
	TimelineRuler *m_pRuler;
	TimelineManager *m_pTimelineMgr;
	TimelineTrack* m_pTrackData;
	int m_nCurFrame;

	static const int m_nMarkerW = 7;
	static const int m_nMarkerH = 11;
	int m_nMarkTop;
};

#endif // TVDRAW_H
