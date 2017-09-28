#ifndef FPANNELMEDIAPLAYER_H
#define FPANNELMEDIAPLAYER_H

#include "FMediaPlayerBarBase.h"
#include "inc_FCommonLib\FBaseWidget.h"
#include "..\..\include\inc_FPropertyPanel\fpropertypanel_global.h"

class FPannelMediaPlayerBar;
class MediaPlayerView;
class IMediaPlayerControl;
class FNLETimeline;
class  FPROPERTYPANEL_EXPORT FPannelMediaPlayer : public FBaseWidget
{
	Q_OBJECT

public:
	FPannelMediaPlayer(QWidget *parent);
	~FPannelMediaPlayer();
	IMediaPlayerControl* GetMediaPlayerControl();
	void SetDuration(int iDurationTime);
	void SetCurPlayTime(int iCurTime);
protected:
	void resizeEvent(QResizeEvent *event)override;

private:
	void InitUI();
	void InitConnections();
private:
	MediaPlayerView*     m_Player;
	FPannelMediaPlayerBar* m_PlayToolbar;
};


class FLabel;
class IMediaPlayerControl;
class  FPannelMediaPlayerBar : public  FMediaPlayerBarBase
{
	Q_OBJECT
public:
	FPannelMediaPlayerBar(IMediaPlayerControl *control, QWidget *parent);
	~FPannelMediaPlayerBar(){};
	void SetDuration(int iDurationTime);
	void SetCurPlayTime(int iCurTime);

private:
	void InitUI();
	void InitConnections();
private:
	FLabel* m_PastTimeLabel;
	FLabel* m_TotalTimeLabel;
	FLabel* m_SeparateLabel;
};

#endif // FPANNELMEDIAPLAYER_H
