#include "stdafx.h"

#include "inc_CommonLib/NLEWraper.h"
#include <QtWidgets/QVBoxLayout>
#include "mediaplayerviewprivate.h"

#include "mediaplayercontrolprivate.h"
#include "mediaplayerviewtoolbar.h"

#include "inc_CommonLib/NLEBaseWidget.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/NLETimeline.h"
#include "INLEMarkdown.h"


MediaPlayerView::MediaPlayerView(QWidget *parent)
:QWidget(parent)
{
	
    setMouseTracking(true);
    m_p = new MediaPlayerViewPrivate(this, new MediaPlayerControl);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_p);
	setLayout(layout);
}


MediaPlayerView::MediaPlayerView(MediaPlayerViewPrivate *p, QWidget *parent)
:QWidget(parent)
{
	m_p = p;

    setMouseTracking(true);
    QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_p);
	setLayout(layout);
}


IMediaPlayerControl* MediaPlayerView::GetMediaPlayerControl()
{
	return m_p->GetMediaPlayerControl();
}

void MediaPlayerView::CompareSideBySide(bool bShow)
{
	m_p->CompareSideBySide(bShow);
}
void MediaPlayerView::SetBorderColor(QColor color)
{
    m_p->SetBorderColor(color);
}

void MediaPlayerView::SetBorderWidth(int width)
{
    m_p->SetBorderWidth(width);
}

//////////////////////////////////////////////////////////////////////////

MediaPlayerViewPrivate::MediaPlayerViewPrivate(QWidget *parent,IMediaPlayerControl* control)
:QWidget(parent)
{
    setMouseTracking(true);
    m_mediaPlayerControl = static_cast<MediaPlayerControl*>(control);
	Q_ASSERT(m_mediaPlayerControl);
	bool ret = m_mediaPlayerControl->Init();
	Q_ASSERT(ret);
	m_mediaPlayerControl->SetMediaPlayerView(this);
	m_displayView = new DisplayView(m_mediaPlayerControl, this);
	m_fullscreen = new FullScreenControl(this);

	//setStyleSheet(GetStyles());
}

MediaPlayerViewPrivate::~MediaPlayerViewPrivate()
{
	delete m_mediaPlayerControl;
}


IMediaPlayerControl* MediaPlayerViewPrivate::GetMediaPlayerControl()
{
	return m_mediaPlayerControl;
}



void MediaPlayerViewPrivate::resizeEvent(QResizeEvent *event)
{
	m_displayView->setGeometry(rect());
	
}



void MediaPlayerViewPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
	event->ignore();
}

void MediaPlayerViewPrivate::FullScreen()
{
	m_fullscreen->FullScreen();
}

void MediaPlayerViewPrivate::Restore()
{
	m_fullscreen->Restore();
}

void MediaPlayerViewPrivate::SnapShot(const QString &path)
{

}

void MediaPlayerViewPrivate::SetRatio(float ratio)
{
	m_displayView->setRatio(ratio);
}

void MediaPlayerViewPrivate::CompareSideBySide(bool bEnable)
{
	NLEComPtr<INLEMarkBook> markBook = nullptr;

		
	if (m_mediaPlayerControl->GetTimelineBase())
	{
		if (m_mediaPlayerControl->GetTimelineBase()->GetSrc())
		{
			m_mediaPlayerControl->GetTimelineBase()->GetSrc()->QueryInterface(IID_INLEMarkBook, (LPVOID*)&markBook);
			if (markBook)
			{
				markBook->SetRenderMode(bEnable ? 1 : 0);
			}
		}
	}
		
	m_displayView->ShowCompareLabel(bEnable);
	m_mediaPlayerControl->Update();
}

void MediaPlayerViewPrivate::SetBorderColor(QColor color)
{
    m_displayView->SetBorderColor(color);
}

void MediaPlayerViewPrivate::SetBorderWidth(int width)
{
    m_displayView->SetBorderWidth(width);
}

//////////////////////////////////////////////////////////////////////////
MainMediaPlayerView::MainMediaPlayerView(QWidget *parent)
:MediaPlayerView(new MainMediaPlayerViewPrivate, parent)
{
	
}


MainMediaPlayerView::MainMediaPlayerView(MediaPlayerViewPrivate *p, QWidget *parent)
:MediaPlayerView(p, parent)
{

}

//////////////////////////////////////////////////////////////////////////

MainMediaPlayerViewPrivate::MainMediaPlayerViewPrivate(QWidget *parent /*= nullptr*/)
:MediaPlayerViewPrivate(parent, new MediaPlayerControl)
{
	setMouseTracking(true);
	m_toolbar = new MainMediaPlayerToolBar(m_mediaPlayerControl, this);

	connect(m_toolbar, &MainMediaPlayerToolBar::Clicked, this, &MainMediaPlayerViewPrivate::ButtonClicked);
	m_mediaPlayerControl->SetToolBar(m_toolbar);
	m_fullscreen->SetToolbar(m_toolbar);

	// 全局空格，播放
	connect(NLEGlobalShortcut::GetInstance(), &NLEGlobalShortcut::actived, this, [&](int id){
		if (id == NLEGlobalShortcut::Key_Space)
		{
			if (m_mediaPlayerControl->HaveSource() && m_mediaPlayerControl->Duration() > 0)
			{
				if (m_mediaPlayerControl->IsPlaying())
					m_mediaPlayerControl->Pause();
				else
					m_mediaPlayerControl->Play();
			}
		}
			
	});


}

void MainMediaPlayerViewPrivate::resizeEvent(QResizeEvent *event)
{
	const int toolbarH = 60;
	m_displayView->setGeometry(0, 0, width(), height() - toolbarH);
	int top = m_displayView->geometry().bottom();
	m_toolbar->setGeometry(0, top, width(), toolbarH + 1);

	m_fullscreen->ResizeEvent(m_displayView, rect());
}

void MainMediaPlayerViewPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
	m_toolbar->DblMouseButtonFullScreen();
	MediaPlayerViewPrivate::mouseDoubleClickEvent(event);
}

void MainMediaPlayerViewPrivate::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		if (m_fullscreen->IsFullScreen())
		{
			m_toolbar->DblMouseButtonFullScreen();
		}
	}
	else if (event->modifiers() & Qt::AltModifier)
	{
		//Restore();
	}
	
	return MediaPlayerViewPrivate::keyPressEvent(event);
}

void MainMediaPlayerViewPrivate::mouseMoveEvent(QMouseEvent *event)
{
	m_fullscreen->MayShowToolbar(rect(), event->pos());
	MediaPlayerViewPrivate::mouseMoveEvent(event);
}

//bool MainMediaPlayerViewPrivate::event(QEvent *event)
//{
//	if (event->type() == QEvent::KeyPress)
//	{
//		auto pKeyEvent = static_cast<QKeyEvent*>(event);
//		//qDebug() <<( pKeyEvent->modifiers() & Qt::AltModifier) << (pKeyEvent->key() == Qt::Key_Tab);
//	}
//	return MediaPlayerViewPrivate::event(event);
//}

void MainMediaPlayerViewPrivate::ButtonClicked(int id, QPoint p)
{
	switch (id)
	{
	case MainMediaPlayerToolBar::MainToolBar_FullScreen:
		FullScreen();
		break;
	case MainMediaPlayerToolBar::MainToolBar_Volume:
	{
		
		AudioVolumeSlider::GetInstance()->DoModal(p);
	}
		
		break;
	case MainMediaPlayerToolBar::MainToolBar_SnapShot:
		m_mediaPlayerControl->SnapShot(L"c:\\snapshot.jpg");
		break;
	default:
		;//Q_ASSERT(false);
	}
}

QString gGetMediaPlayerStyles()
{
	return gGetStyleSheet(QString(":/MediaPlayerView/Resources/styles.qss"));
}
