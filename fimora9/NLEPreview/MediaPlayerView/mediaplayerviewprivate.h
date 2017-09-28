#ifndef MEDIAPLAYVIEW_H
#define MEDIAPLAYVIEW_H


#include "inc_MediaPlayerView/MediaPlayerView.h"

#include <QtGui/QKeyEvent>
#include <QtCore/QString>

class MediaPlayerControl;
class FullScreenControl;
class DisplayView;

class  MediaPlayerViewPrivate : public QWidget
{
	Q_OBJECT
public:
	MediaPlayerViewPrivate(QWidget *parent, IMediaPlayerControl* control);
	~MediaPlayerViewPrivate();

	IMediaPlayerControl* GetMediaPlayerControl();

	void FullScreen();
	void Restore();
	void SnapShot(const QString &path);
	virtual void SetRatio(float ratio);
	void CompareSideBySide(bool bEnable);
    void SetBorderColor(QColor color);
    void SetBorderWidth(int width);
protected:
	void resizeEvent(QResizeEvent *event);
	//virtual int MediaControlType();
	void mouseDoubleClickEvent(QMouseEvent *event);

protected:
	DisplayView *m_displayView;
	MediaPlayerControl *m_mediaPlayerControl;
	FullScreenControl *m_fullscreen;
};


class MainMediaPlayerToolBar;
class  MainMediaPlayerViewPrivate : public MediaPlayerViewPrivate
{
	Q_OBJECT
public:
	MainMediaPlayerViewPrivate(QWidget *parent = nullptr);
	

protected:
	virtual void resizeEvent(QResizeEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void ButtonClicked(int id, QPoint p);

private:
	MainMediaPlayerToolBar *m_toolbar;
};



//QString GetStyles();




#endif // MEDIAPLAYVIEW_H
