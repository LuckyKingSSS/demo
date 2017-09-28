#ifndef CROPMEDIAPLAYERCONTROL_H
#define CROPMEDIAPLAYERCONTROL_H
#include <QObject>
#include "mediaplayercontrolprivate.h"
class NLETimeline;
class CropMediaPlayerControl : public MediaPlayerControl
{
	Q_OBJECT

public:
	CropMediaPlayerControl();
	~CropMediaPlayerControl();

	void SetDestViewHwnd(HVIEW hviewDestParent, const RECT &rect);
	bool Open(NLETimeline *pDestTimeline);

protected:
	NLEComPtr<IPlayerView> m_pDestPlayerView;
	NLETimeline *m_pDestTimeline;
};

#endif // CROPMEDIAPLAYERCONTROL_H
