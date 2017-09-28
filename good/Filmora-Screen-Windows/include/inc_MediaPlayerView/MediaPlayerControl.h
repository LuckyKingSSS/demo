#pragma once

#ifndef MEDIAPLAYERCONTROL_H_H
#define MEDIAPLAYERCONTROL_H_H

/*
*Author:		zhy
*Date:			2016.04.29
*Description:	播放器控制器，包括播放、暂停、前/后一帧等
*/

#include "INLEPlayer.h"

#ifdef WIN32

#ifdef MEDIAPLAYVIEW_EXPORT
#define MEDIAPLAYVIEW_EXT __declspec(dllexport)
#else
#define MEDIAPLAYVIEW_EXT __declspec(dllimport)
#endif

#else
#define MEDIAPLAYVIEW_EXT  
#endif



#include <functional>


#ifndef NLEInterface
#ifdef WIN32
#define NLEInterface interface
#else
#define NLEInterface struct
#endif
#endif

//#ifndef HVIEW
//typedef HWND HVIEW;
//#endif

NLEInterface IMediaStream;
NLEInterface INLERender;
class NLETimeline;

#include <QtCore/QObject>
class INLEFrame;

// 播放控制器
class MEDIAPLAYVIEW_EXT IMediaPlayerControl : public QObject
{
	Q_OBJECT
public:
	static IMediaPlayerControl* CreateInstance();
	virtual void SetHwnd(HVIEW hview, const RECT &rect) = 0;
	virtual bool Open(int bSource, NLETimeline *timeline) = 0;
	virtual void SetRatio(float ratio) = 0;
	virtual float GetRatio() = 0;
	virtual bool Close() = 0;
	virtual bool Play() = 0;
	virtual bool IsPlaying() = 0;
	virtual bool Stop() = 0;
	virtual bool Pause() = 0;
    virtual void SetVolume(int volume) = 0;
    virtual int GetVolume() = 0;
	virtual bool IsPause() = 0;
	virtual bool Seek(int frames, int accurate) = 0;
	virtual bool IsSeeking() = 0;
	virtual bool Forward() = 0;
	virtual bool Backward() = 0;
	virtual int Duration() = 0;
	virtual int CurrentFrames() = 0;
	virtual bool HaveSource() = 0;
	virtual float FPS() = 0;
	virtual void DurationChanged() = 0;
	virtual INLEFrame* CaptureCurrentImage() = 0;
	virtual void Update() = 0;
	virtual NLETimeline* GetTimeline() = 0;

	// 播放前回调控制，返回true播放，false不播放
	virtual void RegisterPlayStartCallback(std::function<bool()>) = 0;
	virtual void Release() = 0;

	virtual QImage GetCurrentImage() = 0;
Q_SIGNALS:
	// 播放中帧回调信号，单位帧
	void PlayingCallback(int frames);
	// 开始播放
	void PlayStartCallback();
	// 播放暂停
	void PlayPauseCallback();
	// 停止
	void PlayStopCallback();
	// 播放停止信号
	void PlayFinishCallback();
	// 加载完成
	void PlayLoadFinishCallback();
	// Seek完成
	void SeekFinishCallback(int frames);

	// 播放中帧回调信号，单位帧，
	void PlayingCallbackMarkdown(int frames);

	// 音柱信号
	void AudioVolumesCallback(int lvolume, int rvolume);

	// 时间尺滑动
	void RulerSeekCallback(int frames);

};


#endif
