#ifndef FEDITPLAYERIMPL_H
#define FEDITPLAYERIMPL_H

#include "inc_FCommonLib/FBaseWidget.h"

class FNLETimeline;
class FEditPlayerMgr;
class FEditRectangle;
class FEditRectangleView;
class IMediaPlayerControl;
class MediaPlayerView;

class FEditPlayerImpl : public FBaseWidget
{
	Q_OBJECT

public:
	FEditPlayerImpl(QWidget *parent = 0);
	~FEditPlayerImpl();

	// 播放器操作
	bool Open(FNLETimeline *timeline);
	bool Close();
	void Play();
	void Stop();
	void Pause();
	void Seek(int frames);
	void SetDuration(int duration);
	void Forward();
	void Backward();
	void Restore();
	int  Duration();
	bool HaveSource();
	int  CurrentFrame();
	bool IsPlaying();

	// 设置矩形区域可见性
	void SetRectangleVisible(bool);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	void slotRectChanged(const QRectF&);
	QRectF GetPlayerRegion();

private:
	FEditPlayerMgr *m_pMgr;
	FEditRectangleView *m_pRectangleView;
	MediaPlayerView *m_pPlayerView;
	IMediaPlayerControl *m_pPlayerCtrl;
};

#endif // FEDITPLAYERIMPL_H
