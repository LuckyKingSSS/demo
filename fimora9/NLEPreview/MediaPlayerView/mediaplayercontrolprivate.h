#pragma once

#include "inc_MediaPlayerView/MediaPlayerControl.h"
#include "Interface/INLEPlayer.h"
#include "include/NLEComPtr.h"


#define MEDIAPLAYER_RATIO_43 4.0 / 3
#define MEDIAPLAYER_RATIO_169 16.0 / 9

NLEInterface INLEPlayer;
NLEInterface IPlayerViewMgr;
NLEInterface IPlayerView;

class ToolBarBase;
class MediaPlayerViewPrivate;
class MediaPlayerControl : public IMediaPlayerControl
{
	Q_OBJECT
public:
	void SetHwnd(HVIEW hview, const RECT &rect) override;
	bool Open(int bSource, INLERender *pVideoRender, INLERender*  pAudioRender, float fps);
	bool Open(int bSource, NLETimeline *timeline) override;
	void SetRatio(float ratio) override;
	float GetRatio() override;
	bool Close() override;
	bool Play() override;
	bool IsPlaying() override;
	bool Stop() override;
	bool Pause() override;
    void SetVolume(int volume) override;
    int GetVolume() override;
	bool IsPause() override;
	bool Seek(int frames, int accurate) override;
	bool IsSeeking() override;
	bool Forward() override;
	bool Backward() override;
	int Duration() override;
	int CurrentFrames() override;
	bool HaveSource() override;
	float FPS() override;
	void DurationChanged() override;
	INLEFrame* CaptureCurrentImage() override;
	void Update() override;
	NLETimeline* GetTimeline() override;
	void RegisterPlayStartCallback(std::function<bool()> playStartCallbackFun) override;

	void Release() override; 

	bool SnapShot(const wchar_t *path);

	bool Init();
	void SetToolBar(ToolBarBase *toolbar);
	ToolBarBase* GetToolBar();
	void SetMediaPlayerView(MediaPlayerViewPrivate *w);
	void UpdateRatio();

	Q_INVOKABLE void QueuePlayingCallback(int frames);
	Q_INVOKABLE void QueuePlayerFinish();
	Q_INVOKABLE void QueuePlayerStop();
	Q_INVOKABLE void QueuePlayerSeekFinish(int frames);
	Q_INVOKABLE void QueuePlayerLoad();
    Q_INVOKABLE void QueuePauseFinish();
	Q_INVOKABLE void QueueAudioVolumesCallback(int lvolume, int rvolume);
	Q_INVOKABLE void QueuePlayingMarkdownCallback(int frames);
	

	NLETimeline* GetTimelineBase();

	QImage GetCurrentImage() override;

	MediaPlayerControl();
	~MediaPlayerControl();

	NLEComPtr<INLEPlayer> m_IMediaPlayer;
protected:
	
	NLEComPtr<IPlayerViewMgr> m_IPlayerViewMgr;

	IPlayerView* m_IPlayerView;
	NLETimeline *m_TimelineBase;

	ToolBarBase *m_toolbar;
	MediaPlayerViewPrivate *m_mediaPlayer;

	bool m_haveSource;
	float m_fps;
	float m_ratio;

	bool m_close;

	bool m_isSeeking;
	int m_seekFrames;
	bool m_isStop;

	int m_curFrames;
    bool m_bLoadFinish;
    bool m_bPausedFinish;

	std::function<bool()> m_playStartCallbackFun;
};

