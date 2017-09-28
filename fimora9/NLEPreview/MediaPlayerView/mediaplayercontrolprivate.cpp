#include "stdafx.h"
#include "mediaplayercontrolprivate.h"

#include <assert.h>


#include "mediaplayerviewprivate.h"
#include "mediaplayerviewtoolbar.h"
#include "cropmediaplayercontrol.h"

#include "Interface/INLEFactory.h"
#include "Interface/INLETimelineBase.h"
//#include "Shared/NLECommon.h"
#include "Include/NLEPropComPtr/NLEKeyShortcut.h"

#include "inc_CommonLib/WSCreateInstance.h"
#include "inc_CommonLib/NLETimeline.h"


MediaPlayerControl::MediaPlayerControl()
{
	m_IMediaPlayer = nullptr;
	m_IPlayerViewMgr = nullptr;
	m_IPlayerView = nullptr;

	m_TimelineBase = nullptr;

	m_toolbar = nullptr;
	m_mediaPlayer = nullptr;
	m_haveSource = false;

	m_close = false;
	m_isSeeking = false;
	m_seekFrames = 0;
	m_isStop = false;
	m_curFrames = 0;
	m_playStartCallbackFun = nullptr;
    m_bLoadFinish = true;
    m_bPausedFinish = true;
}


MediaPlayerControl::~MediaPlayerControl()
{
	SAFE_RELEASE(m_IPlayerView);
}


IMediaPlayerControl* IMediaPlayerControl::CreateInstance()
{
	MediaPlayerControl *control = new MediaPlayerControl;
	if (!control->Init())
	{
		control->Release();
		control = nullptr;
	}
	return control;
}



bool MediaPlayerControl::Open(int bSource, INLERender *pVideoRender, INLERender* pAudioRender, float fps)
{
	m_fps = fps;
	m_haveSource = true;
    m_bLoadFinish = false;

	bool ret = SUCCEEDED(m_IPlayerView->SetVideoInput(bSource, pVideoRender));
	m_IMediaPlayer->SetAudioInput(pAudioRender);
	m_IMediaPlayer->Load();

    while (!m_bLoadFinish)
	{
        QThread::msleep(20);
       // QApplication::processEvents();
    }

	if (m_toolbar)
	{
		m_toolbar->Open();
		m_toolbar->SetFPS(m_fps);
		m_toolbar->SetDuration(Duration());
	}

	return ret;
}

bool MediaPlayerControl::Open(int bSource, NLETimeline *timeline)
{

	Q_ASSERT(timeline);

	m_TimelineBase = timeline;


	NLEComPtr<INLERender> pVideoRender;
	NLEComPtr<INLERender> pAudioRender;


	bool ret = SUCCEEDED(m_TimelineBase->GetSrc()->GetRender(&pAudioRender, &pVideoRender));
	if (ret)
	{
		float rate = m_TimelineBase->GetRenderFramerate();
		ret = Open(bSource, pVideoRender, pAudioRender, rate);

		UpdateRatio();

	}
	
	return ret;
}

void MediaPlayerControl::SetRatio(float ratio)
{
	m_ratio = ratio;
	m_mediaPlayer->SetRatio(ratio);
	if (m_toolbar)
		m_toolbar->SetRatio(ratio);
}

float MediaPlayerControl::GetRatio()
{
	return m_ratio;
}

void MediaPlayerControl::UpdateRatio()
{
	QSize size = m_TimelineBase->GetRenderSize();

	if (!size.isNull())
	{
		SetRatio(1.0 * size.width() / size.height());
	}
}



NLETimeline* MediaPlayerControl::GetTimelineBase()
{
	return m_TimelineBase;
}

QImage MediaPlayerControl::GetCurrentImage()
{
	NLEComPtr<INLEFrame> pframe = m_IMediaPlayer->CaptureCurrentImageEx();
	Q_ASSERT(pframe);
	NLEMediaInfo Info;
	pframe->GetMediaInfo(&Info);

	return QImage((const uchar*)pframe->GetData(), Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
}

bool MediaPlayerControl::Close()
{
	if (HaveSource())
	{
		m_close = true;
		Stop();

		m_IPlayerView->SetVideoInput(0, nullptr);
		m_IMediaPlayer->SetAudioInput(nullptr);

		if (m_toolbar)
			m_toolbar->Close();

		m_haveSource = false;
		m_TimelineBase = nullptr;
	}

	return true;
}

void MediaPlayerControl::SetHwnd(HVIEW hview, const RECT &rect)
{

	if (m_IPlayerView == nullptr)
	{
		m_IPlayerView = m_IPlayerViewMgr->CreatePlayView(hview, rect);
	}
	else
	{
		m_IPlayerView->SetParentWND(hview);
		auto hwnd = m_IPlayerView->GetWND();
#ifdef WIN32
		SetWindowPos(m_IPlayerView->GetWND(), 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE);
#endif
    }
}

bool MediaPlayerControl::Play()
{
	bool ret = true;
	if (HaveSource())
	{
		if (m_toolbar)
		{
			// Slider在seeking中时不播放
			if (!IsPlaying() && !m_toolbar->IsSeeking())
			{
				if (m_playStartCallbackFun && !m_playStartCallbackFun())
					return false;

				emit PlayStartCallback();
				ret = SUCCEEDED(m_IMediaPlayer->Play());
				if (m_toolbar)
					m_toolbar->Play();

			}
		}
		else
		{
			if (!IsPlaying())
			{
				if (m_playStartCallbackFun && !m_playStartCallbackFun())
					return false;

				emit PlayStartCallback();
				ret = SUCCEEDED(m_IMediaPlayer->Play());
				

			}
		}
	}
	else
	{
		if (m_toolbar)
			m_toolbar->Pause();
	}

	return ret;
}


#include <QtWidgets/QApplication>

bool MediaPlayerControl::Stop()
{
	bool ret = true;
	//if (HaveSource())
	//{
		m_isStop = false;
		ret = SUCCEEDED(m_IMediaPlayer->Stop());

		while (!m_isStop)
		{
			QThread::msleep(20);
			//qApp->processEvents();
		}

		if (m_toolbar)
			m_toolbar->Stop();



	//}
	
	return ret;
}

bool MediaPlayerControl::Pause()
{
	bool ret = true;
	if (HaveSource() && IsPlaying())
	{
        m_bPausedFinish = false;
		ret = SUCCEEDED(m_IMediaPlayer->Pause());
		if (m_toolbar)
			m_toolbar->Pause();
		// pause底层不会发送停止信号，在此模拟停止播放信号

        while (!m_bPausedFinish)
		{
            QThread::msleep(20);
			//qApp->processEvents();
		}
	}
	
	return ret;
}

void MediaPlayerControl::SetVolume(int volume)
{
	if (m_TimelineBase)
		m_TimelineBase->SetVolume(volume);
}

int MediaPlayerControl::GetVolume(){
    return m_TimelineBase->GetVolume();
}

bool MediaPlayerControl::IsPause()
{
	return m_IMediaPlayer->GetPlayerState() == PS_PAUSE;
}



bool MediaPlayerControl::Seek(int frames, int accurate)
{
	bool ret = true;
	if (HaveSource())
	{
		m_isSeeking = true;
		m_seekFrames = frames;
		m_curFrames = frames;
		ret = SUCCEEDED(m_IMediaPlayer->Seek(frames, accurate));
		if (m_toolbar)
			m_toolbar->SeekCallback(frames, 0);
	}
	return ret;
}


bool MediaPlayerControl::Forward()
{
	bool ret = true;
	if (HaveSource())
	{
		Pause();
		ret = SUCCEEDED(m_IMediaPlayer->Forward());
		if(m_toolbar)
			m_toolbar->Forward();
	}
	return ret;
}

bool MediaPlayerControl::Backward()
{
	bool ret = true;
	if (HaveSource())
	{
		Pause();
		ret = SUCCEEDED(m_IMediaPlayer->BackForward());
		if (m_toolbar)
			m_toolbar->Backward();
	}
	return ret;
	
}

int MediaPlayerControl::Duration()
{
	int duration = 0;
	if (HaveSource())
	{
		if (m_TimelineBase)
		{
			QPoint range = m_TimelineBase->GetRange();
			duration = range.y() - range.x() + 1;
		}
		else
			duration = m_IMediaPlayer->GetMovieLength();

	}

	return duration;
}

int MediaPlayerControl::CurrentFrames()
{
	return m_curFrames;
}

bool MediaPlayerControl::HaveSource()
{
	return m_haveSource;
}

bool MediaPlayerControl::IsSeeking()
{
	return m_isSeeking;
}

float MediaPlayerControl::FPS()
{
	return m_fps;
}

void MediaPlayerControl::DurationChanged()
{
	if (m_toolbar)
	{
		m_toolbar->SetDuration(Duration());
	}
	UpdateRatio();
}

INLEFrame* MediaPlayerControl::CaptureCurrentImage()
{
	return m_IMediaPlayer->CaptureCurrentImageEx();
}

void MediaPlayerControl::Update()
{
	int ret = m_IMediaPlayer->Update(true);
}

NLETimeline* MediaPlayerControl::GetTimeline()
{
	return m_TimelineBase;
}

void MediaPlayerControl::RegisterPlayStartCallback(std::function<bool()> playStartCallbackFun)
{
	m_playStartCallbackFun = playStartCallbackFun;
}

bool MediaPlayerControl::IsPlaying()
{
	return m_IMediaPlayer->GetPlayerState() == PS_PLAYING;
}

//EXTERN_C const IID IID_IMediaPlayer;

bool MediaPlayerControl::Init()
{

	INLEFactory	*pFactory = NULL;
	WSCreateInstance::Instance()->wsCreateInstance(CLSID_NLEFactory, nullptr, 0, IID_INLEFactory, (void**)&pFactory);
	if (!pFactory) 
		return false;
	pFactory->Init(NULL);
	m_IMediaPlayer = pFactory->CreateNLEPlayer();
	if (!m_IMediaPlayer) 
		return false;

	m_IMediaPlayer->GetPlayerViewMgr(&m_IPlayerViewMgr);
	assert(m_IPlayerViewMgr);

	// 获取播放时间回调
	m_IMediaPlayer->SetCallBack([](void* pUserObj, UINT uMsg, WPARAM_PLAYER wParam, LPARAM_PLAYER lParam)->HRESULT{

		auto mediaPlayerControl = static_cast<MediaPlayerControl*>(pUserObj);
		if (uMsg == WM_USER + 2001)
		{
			// 播放中
			if (wParam == PLAYER_MSG_PROGRESS)
			{
				if (mediaPlayerControl->IsPlaying() && !mediaPlayerControl->IsSeeking())
				{
					long long frames = *(long long*)(lParam);
					QMetaObject::invokeMethod(mediaPlayerControl, "QueuePlayingCallback", Qt::QueuedConnection, Q_ARG(int, frames));
				}

			}
			// 停止播放
			else if (wParam == PLAYER_MSG_FINISH )
			{
				QMetaObject::invokeMethod(mediaPlayerControl, "QueuePlayerFinish", Qt::QueuedConnection);
	
			}
			else if (wParam == PLAYER_MSG_STOP)
			{


                mediaPlayerControl->m_close = false;
                mediaPlayerControl->m_isStop = true;
                mediaPlayerControl->m_curFrames = 0;

				QMetaObject::invokeMethod(mediaPlayerControl, "QueuePlayerStop", Qt::QueuedConnection);
			}
			else if (wParam == PLAYER_MSG_PAUSE)
			{
                mediaPlayerControl->m_bPausedFinish = true;
                QMetaObject::invokeMethod(mediaPlayerControl, "QueuePauseFinish", Qt::QueuedConnection);

			}
			else if (wParam == PLAYER_MSG_SEEK || wParam == PLAYER_MSG_SEEK_PLAYING)
			{
				long long frames = *(long long*)(lParam);
				QMetaObject::invokeMethod(mediaPlayerControl, "QueuePlayerSeekFinish", Qt::QueuedConnection, Q_ARG(int, frames));

			}	
			else if (wParam == PLAYER_MSG_LOAD)
			{
                mediaPlayerControl->m_bLoadFinish = true;
                mediaPlayerControl->m_curFrames = 0;
				QMetaObject::invokeMethod(mediaPlayerControl, "QueuePlayerLoad", Qt::QueuedConnection);

			}
			else if (wParam == PLAYER_MSG_PROGRESSFORMARKUP)
			{
				long long frames = *(long long*)(lParam);
				QMetaObject::invokeMethod(mediaPlayerControl, "QueuePlayingMarkdownCallback", Qt::QueuedConnection, Q_ARG(int, frames));
			}

		}

		return 0;
	}, this);


	// 获取音频数据
	m_IMediaPlayer->SetAudioVolumeCallBack([](void* pUserObj, UINT uMsg, WPARAM_PLAYER wParam, LPARAM_PLAYER lParam)->HRESULT{

		auto mediaPlayerControl = static_cast<MediaPlayerControl*>(pUserObj);
		if (uMsg == WM_AudioVolume)
		{
			DWORD l = *(DWORD*)wParam;
			DWORD r = *(DWORD*)lParam;
			QMetaObject::invokeMethod(mediaPlayerControl, "QueueAudioVolumesCallback", Qt::QueuedConnection, Q_ARG(int, l), Q_ARG(int, r));
		}
		return 0;
	}, this);



	return true;
}
Q_INVOKABLE void MediaPlayerControl::QueuePlayingCallback(int frames)
{
	m_curFrames = frames;
	emit PlayingCallback(frames);
}


void MediaPlayerControl::QueuePlayerFinish()
{
	m_curFrames = 0;
	m_IMediaPlayer->Load();
	emit PlayFinishCallback();
}


Q_INVOKABLE void MediaPlayerControl::QueuePlayerStop()
{
	if (!m_close)
	{
		m_IMediaPlayer->Load();
	}

 //   m_close = false;
 //   m_isStop = true;
 //   m_curFrames = 0;

	emit PlayStopCallback();
}

Q_INVOKABLE void MediaPlayerControl::QueuePlayerSeekFinish(int frames)
{
	if (frames == m_seekFrames)
		m_isSeeking = false;

	emit SeekFinishCallback(frames);
}

Q_INVOKABLE void MediaPlayerControl::QueuePlayerLoad()
{
 //   m_bLoadFinish = true;
	//m_curFrames = 0;
	m_IMediaPlayer->Update(1);
	emit PlayLoadFinishCallback();
}

Q_INVOKABLE void MediaPlayerControl::QueuePauseFinish()
{
    emit PlayPauseCallback();

}

Q_INVOKABLE void MediaPlayerControl::QueueAudioVolumesCallback(int lvolume, int rvolume)
{
	emit AudioVolumesCallback(lvolume, rvolume);
}

Q_INVOKABLE void MediaPlayerControl::QueuePlayingMarkdownCallback(int frames)
{
	if (IsPlaying())
	{
		emit PlayingCallbackMarkdown(frames);
	}
}

void MediaPlayerControl::SetToolBar(ToolBarBase *toolbar)
{
	m_toolbar = toolbar;
	if (m_toolbar)
	{
		connect(m_toolbar, &ToolBarBase::RulerSeekCallback, this, &IMediaPlayerControl::RulerSeekCallback);
	}
}

ToolBarBase* MediaPlayerControl::GetToolBar()
{
	return m_toolbar;
}

void MediaPlayerControl::SetMediaPlayerView(MediaPlayerViewPrivate *w)
{
	m_mediaPlayer = w;
}

void MediaPlayerControl::Release()
{
	delete this;
}

bool MediaPlayerControl::SnapShot(const wchar_t *path)
{
	bool ret = false;
	if (HaveSource())
	{
		CaptureImageInfo captureImageInfo;
		captureImageInfo.width = 720;
		captureImageInfo.height = 576;
		captureImageInfo.uImageType = IMAGE_JPG;
		
		ret = SUCCEEDED(m_IMediaPlayer->CaptureCurrentImage(&captureImageInfo, path));

	}
	
	return ret;
}
