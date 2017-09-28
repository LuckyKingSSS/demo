#ifndef CAPTIONPLAYERVIEWPRIVATE_H
#define CAPTIONPLAYERVIEWPRIVATE_H

#include <QtWidgets/QWidget>
#include "mediaplayerviewprivate.h"
#include "mediaplayerviewtoolbar.h"

class CaptionMediaPlayerToolBar;
class CaptionPlayerViewPrivate : public MediaPlayerViewPrivate
{
	Q_OBJECT

public:
	CaptionPlayerViewPrivate(QWidget *parent = nullptr);
	~CaptionPlayerViewPrivate();

	ToolBarBase* GetToolBar();
private:
	void ButtonClicked(int id);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	CaptionMediaPlayerToolBar *m_toolbar;
};

class CaptionMediaPlayerToolBar : public ToolBarBase
{
	Q_OBJECT
public:
	CaptionMediaPlayerToolBar(MediaPlayerControl *control, QWidget *parent);

	void Open() override;
	void Close() override;
	//void Play() override;
	void Stop() override;
	//void Pause() override;
	void SeekCallback(int frames, int accurate) override;
	void SeekManual(int frames, int accurate) override;
	void SetDuration(int duration) override;
	void SetFPS(float fps) override;
	//void SetRatio(float ratio) override;

protected:
	void resizeEvent(QResizeEvent *event);

private:
	QLabel *m_lb;
	CurrentFrameLabel *m_currentFrame;
	CurrentFrameLabel *m_TotalFrame;

};

#endif // CAPTIONPLAYERVIEWPRIVATE_H
