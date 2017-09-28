#include "stdafx.h"
#include "commonapi.h"

#include "mediaplayercontrolprivate.h"
#include "mediaplayerviewprivate.h"
#include "mediaplayerviewtoolbar.h"

#include "inc_CommonLib/externalapi.h"
#include "inc_CommonLib/CommonWidgets.h"

FullScreenControl::FullScreenControl( QWidget *w)
:QObject(w)
{

	m_mediaPlayerViewPrivate = w;
	SetFullScreen(false);


	m_hideAnimation = new QPropertyAnimation(this);
	m_hideAnimation->setDuration(500);

	m_hideAnimTimer = new QTimer(this);
	m_hideAnimTimer->setSingleShot(true);
	m_hideAnimTimer->setInterval(500);
	connect(m_hideAnimTimer, &QTimer::timeout, this, [&](){
		QPoint start = m_toolbar->pos();
		QPoint end = start + QPoint(0, m_toolbar->height());
		m_hideAnimation->setTargetObject(m_toolbar);
		m_hideAnimation->setPropertyName(QByteArray("pos"));
		m_hideAnimation->setStartValue(start);
		m_hideAnimation->setEndValue(end);
		m_hideAnimation->start();
	});

	m_showAnimation = new QPropertyAnimation(this);
	m_showAnimation->setDuration(100);

}


void FullScreenControl::FullScreen()
{
	if (IsFullScreen())
	{
		SetFullScreen(false);
		m_mediaPlayerViewPrivate->setWindowFlags(Qt::Widget);
		m_mediaPlayerViewPrivate->showNormal();
		StopAnimation();
	}
	else
	{
		// 重构不要放if外面
		//必须放在最前面，m_mediaPlayerViewPrivate->setGeometry(fullscreen)会马上触发resizeEvent
		SetFullScreen(true); 

		// 使用此方法全屏会有边框
		//m_mediaPlayerViewPrivate->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
		//m_mediaPlayerViewPrivate->showFullScreen();
		// 使用此方法播放器工具栏无法及时收到paint事件，导致工具没实时响应事件
		//m_mediaPlayerViewPrivate->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		//m_mediaPlayerViewPrivate->showFullScreen();


		// 手动设置全屏
		auto fullscreen = QApplication::desktop()->screenGeometry();
		m_mediaPlayerViewPrivate->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
		m_mediaPlayerViewPrivate->setGeometry(fullscreen);
		m_mediaPlayerViewPrivate->setVisible(true);
		m_mediaPlayerViewPrivate->activateWindow();

	}
	
}

void FullScreenControl::Restore()
{
	if (IsFullScreen())
	{
		auto mainToolbar = qobject_cast<MainMediaPlayerToolBar*>(m_toolbar);
		if (mainToolbar)
			mainToolbar->DblMouseButtonFullScreen();
		else
		{
			m_mediaPlayerViewPrivate->setWindowFlags(Qt::SubWindow);
			m_mediaPlayerViewPrivate->showNormal();
			StopAnimation();
			SetFullScreen(false);
		}


	}
}



void FullScreenControl::ResizeEvent(QWidget *displayView, QRect rect)
{
	// 是全屏时进行隐藏动画
	if (IsFullScreen() && m_toolbar)
	{
		displayView->setGeometry(rect);

		m_hideAnimTimer->start();
	}
}

void FullScreenControl::MayShowToolbar(QRect rect, QPoint p)
{
	// 是全屏时进行显示动画
	if (IsFullScreen() && m_toolbar)
	{
		m_curPoint = p;
		m_dispplayRect = rect;
		const int hitHeith = m_toolbar->height() * 2;
		QRect hitRect(0, m_dispplayRect.height() - hitHeith, m_dispplayRect.width(), hitHeith);
		if (hitRect.contains(m_curPoint))
		{
			m_hideAnimTimer->stop();
			if (!IsAnimationRunning() && !IsToolBarShow())
			{
				QPoint start = m_toolbar->pos();
				QPoint end = start - QPoint(0, m_toolbar->height());
				m_showAnimation->setTargetObject(m_toolbar);
				m_showAnimation->setPropertyName(QByteArray("pos"));
				m_showAnimation->setStartValue(start);
				m_showAnimation->setEndValue(end);
				m_showAnimation->start();
			}
		}
		else
		{
			if (!IsAnimationRunning() && IsToolBarShow())
			{
				m_hideAnimTimer->start();
			}
			
		}
	}
}

void FullScreenControl::StopAnimation()
{
	m_hideAnimation->stop();
	m_hideAnimTimer->stop();
}

bool FullScreenControl::IsToolBarShow()
{
	if (m_toolbar)
	{
		auto r1 = m_toolbar->geometry();
		auto r2 = m_dispplayRect;
		auto ret = r2.contains(r1);
		return m_dispplayRect.contains(m_toolbar->geometry());
	}
	return false;
}

bool FullScreenControl::IsAnimationRunning()
{
	return m_hideAnimation->state() == QAbstractAnimation::Running ||
		m_showAnimation->state() == QAbstractAnimation::Running;
}

void FullScreenControl::SetFullScreen(bool fullscreen)
{
	m_mediaPlayerViewPrivate->setProperty("FullScreen", fullscreen);
}

bool FullScreenControl::IsFullScreen()
{
	return m_mediaPlayerViewPrivate->property("FullScreen").toBool();
}

//////////////////////////////////////////////////////////////////////////
DisplayView::DisplayView(IMediaPlayerControl *control, QWidget *parent)
:QWidget(parent), m_control(control)
, m_borderWidth(0)
{
	setMouseTracking(true);
	setAttribute(Qt::WA_StyledBackground);
	setObjectName("MediaPlayerView_DisplayView");
	
    m_display = new QWidget(this);
	//
	//m_display->setStyleSheet("QWidget{background-color:red;}");
    m_displayBorder = new DisplayView_p(this);

	m_ratio = MEDIAPLAYER_RATIO_169;
	m_DisplayViewMouseEvent = new DisplayViewMouseEvent(this);

	m_pLbBefore = new QLabel(tr("Before"), this);
	m_pLbBefore->setAlignment(Qt::AlignCenter);
	m_pLbBefore->setStyleSheet("background-color:black;color:white;");
	m_pLbAfter = new QLabel(tr("After"), this);
	m_pLbAfter->setAlignment(Qt::AlignCenter);
	m_pLbAfter->setStyleSheet("background-color:black;color:white;");
	ShowCompareLabel(false);

	RECT r = { 0, 0, 100,100 };
	SetPlayerViewHwnd(r);
}

DisplayView::~DisplayView()
{
	SAFE_DELETE(m_DisplayViewMouseEvent);
}


void DisplayView::setRatio(double ratio)
{
	m_ratio = ratio;
	resizeEvent(NULL);
}

HWND DisplayView::getHwnd()
{
    return (HWND)m_display->winId();
}

QWidget* DisplayView::GetDisplayWidget()
{
	return m_display;
}

void DisplayView::ShowCompareLabel(bool bShow)
{
	m_pLbBefore->setVisible(bShow);
	m_pLbAfter->setVisible(bShow);
}

void DisplayView::SetBorderColor(QColor color)
{
    m_displayBorder->SetBorderColor(color);
}

void DisplayView::SetBorderWidth(int width)
{
    m_borderWidth = width;
    resizeEvent(nullptr);
}

void DisplayView::resizeEvent(QResizeEvent *event)
{
	double ratioTemp = 1.0 * width() / height();
	int displayWidth = 0;
	int displayHeight = 0;
    int h = height() - m_borderWidth * 2;
    int w = width() - m_borderWidth * 2;
	if (ratioTemp > m_ratio)
	{

        displayHeight = h;
        displayWidth = qRound(h * m_ratio);
	}
	else
	{
		displayWidth = w;
		displayHeight = qRound(1.0* w / m_ratio);
	}

	m_display->setGeometry((width() - displayWidth)/2, (height() - displayHeight) / 2, displayWidth, displayHeight);

    m_displayBorder->setGeometry(m_display->geometry().adjusted(-m_borderWidth, -m_borderWidth, m_borderWidth, m_borderWidth));
    m_displayBorder->lower();
    m_displayBorder->hide();

	static const int nCmpLabelW = 100;
	static const int nCmpLabelH = 14;
	static const int margins = 6;
	int nHaftW = (m_display->geometry().width() - margins) * 0.5;
	int left = m_display->geometry().left() + (nHaftW - nCmpLabelW) * 0.5;
	int top = m_display->geometry().top() + m_display->geometry().height() * 0.5 * 0.5 - 1;
	m_pLbBefore->setGeometry(left, top, nCmpLabelW, nCmpLabelH);
	left = m_display->geometry().left() + nHaftW + margins + (nHaftW - nCmpLabelW) * 0.5;
	m_pLbAfter->setGeometry(left, top, nCmpLabelW, nCmpLabelH);

	
	RECT r = { 0, 0, (LONG)qMax(1.0f, displayWidth * gGetDevicePixelRatio()), (LONG)qMax(1.0f, displayHeight * gGetDevicePixelRatio()) };
    if ((r.right - r.left) > 10)
    {
        //RECT rTemp = {r.left + m_borderWidth, r.top + m_borderWidth, r.right - m_borderWidth, r.bottom - m_borderWidth};
        SetPlayerViewHwnd(r);

		QPoint topLeft = mapToParent(m_display->geometry().topLeft());
		emit signalDisplayRect(QRect(topLeft, QSize(displayWidth, displayHeight)));

	}

}

void DisplayView::SetPlayerViewHwnd(const RECT &r)
{
	m_control->SetHwnd((HVIEW)m_display->winId(), r);
}

void DisplayView::mouseDoubleClickEvent(QMouseEvent *event)
{
	event->ignore();
	//QWidget::mouseDoubleClickEvent(event);
}

void DisplayView::showEvent(QShowEvent *event)
{
	//resizeEvent(nullptr);
}

//////////////////////////////////////////////////////////////////////////

DisplayViewMouseEvent::DisplayViewMouseEvent(DisplayView *view)
{
	m_displayView = view;
	m_elapsedTimer.start();
	qApp->installNativeEventFilter(this);
}

DisplayViewMouseEvent::~DisplayViewMouseEvent()
{
	qApp->removeNativeEventFilter(this);
}

static int PointDPIRatio(int x)
{
	return x / gGetDevicePixelRatio();
}

bool DisplayViewMouseEvent::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
{
#ifdef WIN32

	MSG *msg = (MSG*)(message);
	if (msg->message == WM_LBUTTONDOWN )
	{
		auto hwnd = ::GetParent(msg->hwnd);
		if (hwnd == m_displayView->getHwnd())
		{
			QWidget *widget = QWidget::find((WId)hwnd);
			QMouseEvent mouseEvent(QEvent::MouseButtonPress, QPoint((LOWORD(msg->lParam)), (HIWORD(msg->lParam))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(widget, &mouseEvent);

			if (m_elapsedTimer.elapsed() < QApplication::doubleClickInterval())
			{
				QMouseEvent doublieMouseEvent(QEvent::MouseButtonDblClick, QPoint((LOWORD(msg->lParam)), (HIWORD(msg->lParam))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
				QApplication::sendEvent(widget, &doublieMouseEvent);

			}
			else
				m_elapsedTimer.restart();
		}
	}
	else if (msg->message == WM_LBUTTONUP)
	{
		auto hwnd = ::GetParent(msg->hwnd);
		if (hwnd == m_displayView->getHwnd())
		{
			QWidget *widget = QWidget::find((WId)hwnd);
			QMouseEvent mouseEvent(QEvent::MouseButtonRelease, QPoint((LOWORD(msg->lParam)), (HIWORD(msg->lParam))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(widget, &mouseEvent);
		}
	}
	else if (msg->message == WM_MOUSEMOVE)
	{
		auto hwnd = ::GetParent(msg->hwnd);
		if (hwnd == m_displayView->getHwnd())
		{
			QWidget *widget = QWidget::find((WId)hwnd);
			QMouseEvent mouseEvent(QEvent::MouseMove, QPoint(PointDPIRatio(short(msg->lParam)), PointDPIRatio(short(msg->lParam >> 16))), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);


			QApplication::sendEvent(widget, &mouseEvent);
		}
	}
	//else if (msg->message == WM_SETCURSOR)
	//{
	//	auto hwnd = ::GetParent(msg->hwnd);
	//	if (hwnd == m_displayView->getHwnd())
	//	{
	//		HCURSOR newCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL));
	//		HCURSOR oldCursor = ::SetCursor(newCursor);
	//	}
	//}

#endif 
	return false;
}


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
AsepctRatioLabel::AsepctRatioLabel(QWidget *parent)
:QLabel(parent)
{
	setText("ASPECT RATIO: 4:3");
}



void AsepctRatioLabel::SetRatio(float ratio)
{
	QString text("4:3");
	if (ratio > MEDIAPLAYER_RATIO_43 + 0.01)
	{
		text = "16:9";
	}
	setText(QString("ASPECT RATIO:%1").arg(text));
}

//////////////////////////////////////////////////////////////////////////

CurrentFrameLabel::CurrentFrameLabel(QWidget *parent)
:QWidget(parent)
{
	m_duration = 0;
	m_fps = 25.0;
	m_preciseFrames = true;
	m_curFrame = new ToolBarLabel(this);
	m_curFrame->setText("00:00:00.00");
}

void CurrentFrameLabel::SetFPS(float fps)
{
	m_fps = fps;
}

void CurrentFrameLabel::SetCurrentFrame(int frames)
{
	frames = qBound(0, frames, m_duration);
	m_curFrame->setText(m_preciseFrames ? gFrame2String(frames, m_fps) : gFrame2StringSecond(frames, m_fps));
}

void CurrentFrameLabel::SetDuration(int duration)
{
	m_duration = duration;
}

void CurrentFrameLabel::SetPreciseFrames(bool enable)
{
	m_preciseFrames = enable;
}

void CurrentFrameLabel::resizeEvent(QResizeEvent *event)
{
	m_curFrame->setGeometry(rect());
}

AudioVolumeSlider* AudioVolumeSlider::GetInstance()
{
	static AudioVolumeSlider *sAudioVolumeSlider = nullptr;
	if (sAudioVolumeSlider == nullptr)
	{
		sAudioVolumeSlider = new AudioVolumeSlider(gGetMainWindow());
	}
	return sAudioVolumeSlider;
}

//////////////////////////////////////////////////////////////////////////


AudioVolumeSlider::AudioVolumeSlider(QWidget *parent)
:QDialog(parent)
{
	setWindowFlags(Qt::Popup);

	m_show = false;
	m_slider = new ToolBarSlider(this, Qt::Vertical);
	m_slider->setDuration(100);
	m_slider->setValue(100);
	resize(20, 100);

}



void AudioVolumeSlider::DoModal(QPoint p)
{
	if (m_show)
	{
		hide();
	}
	else
	{
		m_show = true;

		p.setY(p.y() - height());
		move(p);
		show();

	}

}

void AudioVolumeSlider::resizeEvent(QResizeEvent *event)
{
	const int space = 5;
	m_slider->setGeometry(space, space, width() - 2 * space, height() - 2 * space);
}




void AudioVolumeSlider::hideEvent(QHideEvent *event)
{
	// 200ms后才设置成已隐藏，解决点一次显示，再点一次隐藏问题
	QTimer::singleShot(200, [&](){
		m_show = false;
	});
}
