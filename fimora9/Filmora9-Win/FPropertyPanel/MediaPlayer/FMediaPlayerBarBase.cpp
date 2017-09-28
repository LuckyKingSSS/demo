#include "stdafx.h"
#include "CommonWidgets.h"
#include "FMediaPlayerBarBase.h"
#include "inc_MediaPlayerView\MediaPlayerControl.h"


FMediaPlayerBarBase::FMediaPlayerBarBase(IMediaPlayerControl *control, QWidget *parent)
	: FBaseWidget(parent)
	, m_mediaPlayerControl(control)
	, m_isPlaying(false)
	, m_lastSeekFrames(-1)
	, m_seekManual(false)
{
	setAttribute(Qt::WA_StyledBackground);
	setObjectName("MediaPlayerView_ToolBarBase");

	CreateToolBarStateButton(ToolBarButton::ToolBar_Play, "MediaPlayerView_ToolBarBase_Play", ToolBarButton::ToolBar_Pause, "MediaPlayerView_ToolBarBase_Pause");
	CreateToolBarButton(ToolBarButton::ToolBar_Stop, "MediaPlayerView_ToolBarBase_Stop");
	CreateToolBarButton(ToolBarButton::ToolBar_Forward, "MediaPlayerView_ToolBarBase_Forward");
	CreateToolBarButton(ToolBarButton::ToolBar_Backward, "MediaPlayerView_ToolBarBase_Backward");


	m_PlaySlider = new ToolBarSlider(this);
	m_PlaySlider->setDuration(0);
	connect(m_PlaySlider, &ToolBarSlider::valueChanged, this, [&](double value){
		SeekManual(value, 0);
	});

	connect(m_PlaySlider, &ToolBarSlider::startValueChanged, this, [&](){
		m_seekManual = true;
		m_isPlaying = m_mediaPlayerControl->IsPlaying();
		m_mediaPlayerControl->Pause();
		emit StartRulerSeekCallback(m_isPlaying);
	});
	connect(m_PlaySlider, &ToolBarSlider::endValueChanged, this, [&](){
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
				if (m_lastSeekFrames == frames && !m_PlaySlider->ButtonPressDown())
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

FMediaPlayerBarBase::~FMediaPlayerBarBase()
{

}

void FMediaPlayerBarBase::SetShowButton(int iType)
{
	if (iType | ToolBarButton::ToolBar_Backward == 0)
	{
		if (GetButton(ToolBarButton::ToolBar_Backward) != nullptr)
		{
			GetButton(ToolBarButton::ToolBar_Backward)->hide();
		}	
	}
	if (iType | ToolBarButton::ToolBar_Forward == 0)
	{
		if (GetButton(ToolBarButton::ToolBar_Forward) != nullptr)
		{
			GetButton(ToolBarButton::ToolBar_Forward)->hide();
		}	
	}
	if (iType | ToolBarButton::ToolBar_Pause == 0)
	{
		if (GetButton(ToolBarButton::ToolBar_Pause) != nullptr)
		{
			GetButton(ToolBarButton::ToolBar_Pause)->hide();
		}
	}
	if (iType | ToolBarButton::ToolBar_Play == 0)
	{
		if (GetButton(ToolBarButton::ToolBar_Play) != nullptr)
		{
			GetButton(ToolBarButton::ToolBar_Play)->hide();
		}
	}
	if (iType | ToolBarButton::ToolBar_Resume == 0)
	{
		if (GetButton(ToolBarButton::ToolBar_Resume) != nullptr)
		{
			GetButton(ToolBarButton::ToolBar_Resume)->hide();
		}	
	}
	if (iType | ToolBarButton::ToolBar_Stop == 0)
	{
		if (GetButton(ToolBarButton::ToolBar_Stop) != nullptr)
		{
			GetButton(ToolBarButton::ToolBar_Stop)->hide();
		}
	}
}

void FMediaPlayerBarBase::Open()
{

}

void FMediaPlayerBarBase::Close()
{
	ToolBarPlayButtonStateChanged(ToolBarButton::ToolBar_Play);
	m_PlaySlider->setValue(0);
	m_PlaySlider->setDuration(0);
	RulerSeekCallback(0);
}

void FMediaPlayerBarBase::Play()
{
	ToolBarPlayButtonStateChanged(ToolBarButton::ToolBar_Pause);
}

void FMediaPlayerBarBase::Stop()
{
	ToolBarPlayButtonStateChanged(ToolBarButton::ToolBar_Play);
	m_PlaySlider->setValue(0);
	RulerSeekCallback(0);
}

void FMediaPlayerBarBase::Pause()
{
	ToolBarPlayButtonStateChanged(ToolBarButton::ToolBar_Play);
}

void FMediaPlayerBarBase::SeekCallback(int frames, int accurate)
{
	m_PlaySlider->setValue(frames);
}

void FMediaPlayerBarBase::SeekManual(int frames, int accurate)
{
	m_mediaPlayerControl->Seek(frames, accurate);
	emit RulerSeekCallback(frames);
	if (!m_mediaPlayerControl->IsPlaying() || m_seekManual)
		m_lastSeekFrames = frames;
}

void FMediaPlayerBarBase::SetDuration(int duration)
{
	m_PlaySlider->setDuration(duration);
}

void FMediaPlayerBarBase::Forward()
{
	int frames = qMin(CurrentFrames() + 1, Duration());
	SeekCallback(frames, 1);
	RulerSeekCallback(frames);
}

void FMediaPlayerBarBase::Backward()
{
	int frames = qMax(CurrentFrames() - 1, 0);
	SeekCallback(frames, 1);
	RulerSeekCallback(frames);
}

int FMediaPlayerBarBase::CurrentFrames()
{
	return m_PlaySlider->value();
}

int FMediaPlayerBarBase::Duration()
{
	return m_PlaySlider->duration();
}

bool FMediaPlayerBarBase::GetSliderButtonPressDown()
{
	if (!m_PlaySlider->isVisible())
		return false;
	return m_PlaySlider->ButtonPressDown();
}

bool FMediaPlayerBarBase::IsSeeking()
{
	return !m_PlaySlider->canSeek();
}

BaseButton* FMediaPlayerBarBase::GetButton(int id)
{
	auto btn = m_toolbarButtons[id];
	//Q_ASSERT(btn);
    //btn->show();
	return btn;
}

void FMediaPlayerBarBase::EnableWidgets(bool bEnable)
{
	for (auto btn : m_toolbarButtons)
	{
		btn->SetEnabled(bEnable);
	}
	m_PlaySlider->setEnabled(bEnable);
}

Q_SLOT void FMediaPlayerBarBase::SeekCallbackSlot(int frames)
{
	SeekCallback(frames, 0);
}

void FMediaPlayerBarBase::CreateToolBarButton(int id, const QString objectName /*= ""*/)
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

void FMediaPlayerBarBase::CreateToolBarStateButton(int firstId, QString firstObjectName, int secondId, QString secondObjectName)
{
	auto btn = new BaseStateButton(firstId, "", this);
	btn->AddButton(firstId, firstObjectName);
	btn->AddButton(secondId, secondObjectName);
	connect(btn, &BaseStateButton::Clicked, this, [&](int id, QPoint p){
		ButtonClicked(id, p);
	});
	m_toolbarButtons.insert(firstId, btn);
}

void FMediaPlayerBarBase::ToolBarPlayButtonStateChanged(int id)
{
	qobject_cast<BaseStateButton*>(GetButton(ToolBarButton::ToolBar_Play))->SetCurrentButtonId(id);
}

void FMediaPlayerBarBase::ButtonClicked(int id, QPoint p)
{
	switch (id)
	{
	case ToolBarButton::ToolBar_Play:
	{
		if (!m_mediaPlayerControl->Play())
		{
			ToolBarPlayButtonStateChanged(ToolBar_Play);
		}
		break;
	}
	case ToolBarButton::ToolBar_Pause:
		m_mediaPlayerControl->Pause();
		break;
	case ToolBarButton::ToolBar_Stop:
		m_mediaPlayerControl->Stop();
		break;
	case ToolBarButton::ToolBar_Backward:

		m_mediaPlayerControl->Backward();
		break;
	case ToolBarButton::ToolBar_Forward:

		m_mediaPlayerControl->Forward();

		break;
	default:
		;//Q_ASSERT(false);
	}
	emit Clicked(id, p);
}

void FMediaPlayerBarBase::mouseDoubleClickEvent(QMouseEvent *event)
{
	event->accept();
}
