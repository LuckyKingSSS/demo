#include "stdafx.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtCore/QTimer>
#include <QtGui/QMouseEvent>

#include "mediaplayerviewtoolbar.h"
#include "mediaplayercontrolprivate.h"


#include "inc_CommonLib/CommonWidgets.h"
#include "AudioPillarView.h"

ToolBarBase::ToolBarBase(MediaPlayerControl *control, QWidget *parent /*= nullptr*/)
:QWidget(parent)
, m_mediaPlayerControl(control)
, m_isPlaying(false)
, m_lastSeekFrames(-1)
, m_seekManual(false)
{
	setAttribute(Qt::WA_StyledBackground);
	setObjectName("MediaPlayerView_ToolBarBase");

	CreateToolBarStateButton(ToolBar::ToolBar_Play, "MediaPlayerView_ToolBarBase_Play", ToolBar::ToolBar_Pause, "MediaPlayerView_ToolBarBase_Pause");
	CreateToolBarButton(ToolBar::ToolBar_Stop, "MediaPlayerView_ToolBarBase_Stop");
	CreateToolBarButton(ToolBar::ToolBar_Forward, "MediaPlayerView_ToolBarBase_Forward");
	CreateToolBarButton(ToolBar::ToolBar_Backward, "MediaPlayerView_ToolBarBase_Backward");


	m_slider = new ToolBarSlider(this);
	m_slider->setDuration(0);
	connect(m_slider, &ToolBarSlider::valueChanged, this, [&](double value){
		SeekManual(value, 0);
		
	});

	connect(m_slider, &ToolBarSlider::startValueChanged, this, [&](){
		m_seekManual = true;
		m_isPlaying = m_mediaPlayerControl->IsPlaying();
		m_mediaPlayerControl->Pause();
		emit StartRulerSeekCallback(m_isPlaying);
	});
	connect(m_slider, &ToolBarSlider::endValueChanged, this, [&](){
		if (m_isPlaying)
		{
			m_seekManual = false;
			m_mediaPlayerControl->Play();
			emit EndRulerSeekCallback(m_isPlaying);
			m_isPlaying = false;
			m_lastSeekFrames = -1;
		}
	});

	connect(m_mediaPlayerControl, &IMediaPlayerControl::PlayingCallback, this, [&](int frames){
		SeekCallbackSlot(frames);
	});

	connect(m_mediaPlayerControl, &IMediaPlayerControl::PlayFinishCallback, this, [&](){
		Stop();
	});


	connect(m_mediaPlayerControl, &IMediaPlayerControl::SeekFinishCallback, this, [&](int frames){
		if (!m_isPlaying)
		{
			if (m_seekManual)
			{
				if (m_lastSeekFrames == frames && !m_slider->ButtonPressDown())
				{
					emit EndRulerSeekCallback(false);
					m_lastSeekFrames = -1;
					m_seekManual = false;
				}
			}
			else
			{
				if (m_lastSeekFrames == frames)
				{
					m_lastSeekFrames = -1;
				}
			}
		}

	});
}

ToolBarBase::~ToolBarBase()
{

}

void ToolBarBase::Open()
{
	//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(false);
}
 
void ToolBarBase::Close()
{
	ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Play);
	m_slider->setValue(0);
	m_slider->setDuration(0);
	//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(false);
	RulerSeekCallback(0);
}

void ToolBarBase::Play()
{
	ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Pause);
	//QMetaObject::invokeMethod(this, "PlayQueue", Qt::QueuedConnection);
}
//void ToolBarBase::PlayQueue()
//{
//	ToolBarStateChanged(ToolBar::ToolBar_Pause);
//}

#include <QtWidgets/QApplication>

void ToolBarBase::Stop()
{
	ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Play);
	m_slider->setValue(0);
	//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(false);
	RulerSeekCallback(0);
	// 去除停止播放后的底层回调的播放停止事件PM_Finish
	//QApplication::processEvents();
}


void ToolBarBase::Pause()
{
	ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Play);
}

void ToolBarBase::SeekCallback(int frames, int accurate)
{
	m_slider->setValue(frames);
	//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(frames > 0);
}

void ToolBarBase::SeekManual(int frames, int accurate)
{
	m_mediaPlayerControl->Seek(frames, accurate);
	//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(frames > 0);
	emit RulerSeekCallback(frames);
	if (!m_mediaPlayerControl->IsPlaying() || m_seekManual)
		m_lastSeekFrames = frames;
}




void ToolBarBase::SetDuration(int duration)
{
	m_slider->setDuration(duration);
	
}


int ToolBarBase::CurrentFrames()
{
	return m_slider->value();
}



int ToolBarBase::Duration()
{
	return m_slider->duration();
}

bool ToolBarBase::GetSliderButtonPressDown()
{
	if (!m_slider->isVisible())
		return false;
	
	return m_slider->ButtonPressDown();

}

bool ToolBarBase::IsSeeking()
{
	return !m_slider->canSeek();
}

void ToolBarBase::Forward()
{
	int frames = qMin(CurrentFrames() + 1, Duration());
	SeekCallback(frames, 1);
	RulerSeekCallback(frames);
}

void ToolBarBase::Backward()
{
	int frames = qMax(CurrentFrames() - 1, 0);
	SeekCallback(frames, 1);
	RulerSeekCallback(frames);
}

BaseButton* ToolBarBase::GetButton(int id)
{
	auto btn = m_toolbarButtons[id];
	Q_ASSERT(btn);
	btn->show();
	return btn;
}

void ToolBarBase::EnableWidgets(bool bEnable)
{
	for (auto btn : m_toolbarButtons)
	{
		btn->SetEnabled(bEnable);
	}
	m_slider->setEnabled(bEnable);
}

void ToolBarBase::mouseDoubleClickEvent(QMouseEvent *event)
{
	event->accept();
}

void ToolBarBase::ToolBarPlayButtonStateChanged(int id)
{
	qobject_cast<BaseStateButton*>(GetButton(ToolBar::ToolBar_Play))->SetCurrentButtonId(id);

}



void ToolBarBase::ButtonClicked(int id, QPoint p)
{
	switch (id)
	{
	case ToolBarBase::ToolBar_Play:
	{
		if (!m_mediaPlayerControl->Play())
		{
			ToolBarPlayButtonStateChanged(ToolBar_Play);
		}
		break;
	}
	case ToolBarBase::ToolBar_Pause:
		m_mediaPlayerControl->Pause();
		break;
	case ToolBarBase::ToolBar_Stop:
		m_mediaPlayerControl->Stop();
		break;
	case ToolBarBase::ToolBar_Backward:

		m_mediaPlayerControl->Backward();
		break;
	case ToolBarBase::ToolBar_Forward:

		m_mediaPlayerControl->Forward();

		break;
	default:
		;//Q_ASSERT(false);
	}
	


	emit Clicked(id, p);
}

Q_SLOT void ToolBarBase::SeekCallbackSlot(int frames)
{
	SeekCallback(frames, 0);
}

void ToolBarBase::CreateToolBarButton(int id, const QString objectName)
{
	if (!m_toolbarButtons.contains(id))
	{
		auto btn = new BaseButton(id, objectName, this);
		btn->hide();
		connect(btn, &BaseButton::Clicked, this, [&](int id, QPoint p){
			ButtonClicked(id, p);
		});
		m_toolbarButtons.insert(id, btn);
	}

}


void ToolBarBase::CreateToolBarStateButton(int firstId, QString firstObjectName, int secondId, QString secondObjectName)
{
	auto btn = new BaseStateButton(firstId, "", this);
	btn->AddButton(firstId, firstObjectName);
	btn->AddButton(secondId, secondObjectName);
	connect(btn, &BaseStateButton::Clicked, this, [&](int id, QPoint p){
		ButtonClicked(id, p);
	});
	m_toolbarButtons.insert(firstId, btn);
}

//////////////////////////////////////////////////////////////////////////


#include <QtWidgets/QApplication>
//////////////////////////////////////////////////////////////////////////
MainMediaPlayerToolBar::MainMediaPlayerToolBar(MediaPlayerControl *control, QWidget *parent)
: ToolBarBase(control, parent)
{
	setObjectName("MediaPlayerView_MainMediaPlayerToolBar");

	CreateToolBarStateButton(MainToolBar::MainToolBar_FullScreen, "MediaPlayerView_MainMediaPlayerToolBar_FullScreen", 
		MainToolBar::MainToolBar_FullScreen, "MediaPlayerView_MainMediaPlayerToolBar_RestoreFullScreen");
	
	CreateToolBarButton(MainToolBar::MainToolBar_RecordVoice, "MediaPlayerView_MainMediaPlayerToolBar_RecordVoice");
	CreateToolBarButton(MainToolBar::MainToolBar_SnapShot, "MediaPlayerView_MainMediaPlayerToolBar_SnapShot");
	CreateToolBarButton(MainToolBar::MainToolBar_Volume, "MediaPlayerView_MainMediaPlayerToolBar_Volume");
	
	m_aspectRatio = new AsepctRatioLabel(this);
	m_currentFrame = new CurrentFrameLabel(this);
	m_TotalFrame = new CurrentFrameLabel(this);
	m_pAudioPillar = new CAudioPillarView(this);
	m_pAudioPillar->SetPallarRange(0, 100);
	connect(m_mediaPlayerControl, &MediaPlayerControl::AudioVolumesCallback, this, [&](int leftVolume, int rightVolume)
	{
		m_pAudioPillar->SetPillarValue(leftVolume, rightVolume);
	});
	connect(m_mediaPlayerControl, &MediaPlayerControl::PlayFinishCallback, this, [&]()
	{
		m_pAudioPillar->SetPillarValue(0, 0);
	});
	connect(m_mediaPlayerControl, &MediaPlayerControl::PlayPauseCallback, this, [&]()
	{
		m_pAudioPillar->SetPillarValue(0, 0);
	});

	EnableButton(false);

}



void MainMediaPlayerToolBar::Open()
{
	EnableButton(true);
	ToolBarBase::Open();
	
}


void MainMediaPlayerToolBar::Close()
{
	ToolBarBase::Close();
	m_currentFrame->SetCurrentFrame(0);
	m_currentFrame->SetDuration(0);
	m_TotalFrame->SetCurrentFrame(0);
	m_TotalFrame->SetDuration(0);
	EnableButton(false);
}

void MainMediaPlayerToolBar::Play()
{
	ToolBarBase::Play();
}

void MainMediaPlayerToolBar::Stop()
{
	ToolBarBase::Stop();
	m_currentFrame->SetCurrentFrame(0);
}


void MainMediaPlayerToolBar::Pause()
{
	ToolBarBase::Pause();
}


void MainMediaPlayerToolBar::SeekCallback(int frames, int accurate)
{
	if (m_slider->canSeek())
	{
		ToolBarBase::SeekCallback(frames, accurate);
		m_currentFrame->SetCurrentFrame(frames);
		//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(frames > 0);
	}
	
}

void MainMediaPlayerToolBar::SeekManual(int frames, int accurate)
{
	if (m_mediaPlayerControl->HaveSource())
	{
		ToolBarBase::SeekManual(frames, accurate);
		m_currentFrame->SetCurrentFrame(frames);
		//GetButton(ToolBar::ToolBar_Backward)->SetEnabled(frames > 0);
	}
	
}

void MainMediaPlayerToolBar::SetDuration(int duration)
{
	ToolBarBase::SetDuration(duration);
	m_currentFrame->SetDuration(duration);
	m_TotalFrame->SetDuration(duration);
	m_TotalFrame->SetCurrentFrame(duration);
}


void MainMediaPlayerToolBar::SetFPS(float fps)
{
	m_currentFrame->SetFPS(fps);
	m_TotalFrame->SetFPS(fps);
}

void MainMediaPlayerToolBar::SetRatio(float ratio)
{
	m_aspectRatio->SetRatio(ratio);
}

void MainMediaPlayerToolBar::DblMouseButtonFullScreen()
{
	(qobject_cast<BaseStateButton*>(GetButton(MainToolBar_FullScreen)))->ClickCurrentButton();
}

void MainMediaPlayerToolBar::resizeEvent(QResizeEvent *event)
{
	const int margins = 5;
	const int space = margins;
	int left = margins;	
	int top = margins;
	const int btnw = 14;
	const int btnh = 14;

	GetButton(ToolBar_Backward)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Backward)->geometry().right() + space;
	GetButton(ToolBar_Play)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Play)->geometry().right() + space;
	GetButton(ToolBar_Forward)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Forward)->geometry().right() + space;
	GetButton(ToolBar_Stop)->setGeometry(left, top, btnw, btnh);
	left = GetButton(ToolBar_Stop)->geometry().right() + space;

	const int rightbtnw = 17;
	int sliderw = width() - left - 4 * (rightbtnw + space);
	m_slider->setGeometry(left, top, sliderw, m_slider->Height());

	
	left = m_slider->geometry().right() + space;
	GetButton(MainToolBar_RecordVoice)->setGeometry(left, top, rightbtnw, btnh);
	left = GetButton(MainToolBar_RecordVoice)->geometry().right() + space;
	GetButton(MainToolBar_SnapShot)->setGeometry(left, top, rightbtnw, btnh);
	left = GetButton(MainToolBar_SnapShot)->geometry().right() + space;
	GetButton(MainToolBar_Volume)->setGeometry(left, top, rightbtnw, btnh);
	left = GetButton(MainToolBar_Volume)->geometry().right() + space;
	GetButton(MainToolBar_FullScreen)->setGeometry(left, top, rightbtnw, btnh);
	left = GetButton(MainToolBar_FullScreen)->geometry().right() + space;


	top = m_slider->geometry().bottom() + 10;
	left = margins;
	m_aspectRatio->setGeometry(left, top, 200, btnh);


	m_pAudioPillar->setGeometry(width() - rightbtnw, top, rightbtnw, 30);
	
	
	m_currentFrame->setGeometry(width() - 75 - rightbtnw, top, 90, 14);
	top = m_currentFrame->geometry().bottom();
	m_TotalFrame->setGeometry(width() - 75 - rightbtnw, top, 90, 14);


}

void MainMediaPlayerToolBar::EnableButton(bool enable)
{
	GetButton(ToolBar::ToolBar_Play)->SetEnabled(enable);
	GetButton(ToolBar::ToolBar_Stop)->SetEnabled(enable);
	GetButton(ToolBar::ToolBar_Backward)->SetEnabled(enable);
	GetButton(ToolBar::ToolBar_Forward)->SetEnabled(enable);

	GetButton(MainToolBar::MainToolBar_SnapShot)->SetEnabled(enable);
	//GetButton(MainToolBar::MainToolBar_FullScreen)->SetEnabled(enable);
}

