#ifndef FPREVIEWPLAYER_H
#define FPREVIEWPLAYER_H
#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FPreviewPlayerHeader;
class FButton;
class FPannelMediaPlayer;
class FNLETimeline;
class MediaPlayerView;

class FPROPERTYPANEL_EXPORT FPreviewPlayer : public FBaseWidget
{
	Q_OBJECT

public:
	FPreviewPlayer(QWidget *parent);
	~FPreviewPlayer();
	void SetTimeline(FNLETimeline* timeline);

public Q_SLOTS:
    void slotOnTimelineChange();

private: 
	void InitUI();
	void InitConnections();
	void InitData();

private:
	FPreviewPlayerHeader* m_pheader;
	FPannelMediaPlayer* m_MediaPlayer;
	FButton* m_SaveBtn;
	FButton* m_OkBtn;
	FButton* m_CancelBtn;
	FNLETimeline* m_Timeline;
};

#endif // FPREVIEWPLAYER_H
