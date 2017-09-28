
#pragma once

/*
*Author:		zhy
*Date:			2016.04.29
*Description:	播放器窗口
*/

#ifndef MEDIAPLAYERVIEW_H_H
#define MEDIAPLAYERVIEW_H_H


#include "MediaPlayerControl.h"
#include <QtWidgets/QWidget>



class MediaPlayerViewPrivate;

// 只有画面的播放器
class MEDIAPLAYVIEW_EXT MediaPlayerView : public QWidget
{
	Q_OBJECT
public:
	MediaPlayerView(QWidget *parent);
	IMediaPlayerControl* GetMediaPlayerControl();
	void CompareSideBySide(bool bEnable);
    void SetBorderColor(QColor color);
    void SetBorderWidth(int width);
protected:
	MediaPlayerView(MediaPlayerViewPrivate *p, QWidget *parent);
	MediaPlayerViewPrivate *m_p;
};


// 主界面播放器，包含一个控制栏
class MEDIAPLAYVIEW_EXT MainMediaPlayerView : public MediaPlayerView
{
	Q_OBJECT
public:
	MainMediaPlayerView(QWidget *parent);

protected:
	MainMediaPlayerView(MediaPlayerViewPrivate *p, QWidget *parent);

};

class NLETimeline;
class MEDIAPLAYVIEW_EXT MediaCropView : public MediaPlayerView
{
	Q_OBJECT
public:
	MediaCropView(NLETimeline *pDestTimeLine, QWidget *parent);

Q_SIGNALS:
	void signalOK();
	void signalCancel();
};

class MEDIAPLAYVIEW_EXT CaptionPlayerView : public MediaPlayerView
{
	Q_OBJECT
public:
	CaptionPlayerView(QWidget *parent);

Q_SIGNALS:
	void RulerSeekCallback(int frames);
	void StartRulerSeekCallback(bool isPlaying);
	void EndRulerSeekCallback(bool isPlaying);

	void Forward();
	void Backward();
};

class BaseButton;
class MEDIAPLAYVIEW_EXT ActionCamPlayerView : public MediaPlayerView
{
	Q_OBJECT
public:
	ActionCamPlayerView(QWidget *parent);
	void EnableWidgets(bool bEnable);
	void Forward();
	void Backward();

Q_SIGNALS:
	void RulerSeekCallback(int frames);
	void StartRulerSeekCallback(bool isPlaying);
	void EndRulerSeekCallback(bool isPlaying);
};

class NLETimeline;
class MEDIAPLAYVIEW_EXT MainMediaPlayerViewEx : public MainMediaPlayerView
{
	Q_OBJECT
public:
	MainMediaPlayerViewEx(QWidget *parent);
	void SetLoadType(bool bTimelimeLoaded);
	void SetSeletedTimeline(NLETimeline *pSelected);
};


// 本模块的所有stylesheet,
MEDIAPLAYVIEW_EXT QString gGetMediaPlayerStyles();

#endif