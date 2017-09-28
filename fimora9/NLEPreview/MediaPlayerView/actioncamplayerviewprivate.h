#ifndef ACTIONCAMPLAYERVIEWPRIVATE_H
#define ACTIONCAMPLAYERVIEWPRIVATE_H


#include "mediaplayerviewprivate.h"
#include "mediaplayerviewtoolbar.h"

class BaseButton;
class ActionCamMediaPlayerToolBar;
class ToolBarBase;
class ActionCamPlayerviewPrivate : public MediaPlayerViewPrivate
{
	Q_OBJECT
public:
	ActionCamPlayerviewPrivate(QWidget *parent = nullptr);
	~ActionCamPlayerviewPrivate();

	void Forward();
	void Backward();


	void EnableWidgets(bool bEnable);

	ToolBarBase* GetToolBar();
private:
	void ButtonClicked(int id);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	ActionCamMediaPlayerToolBar *m_toolbar;
	
};


class CurrentFrameLabel;
class ActionCamMediaPlayerToolBar : public ToolBarBase
{
	Q_OBJECT
public:
	ActionCamMediaPlayerToolBar(MediaPlayerControl *control, QWidget *parent);

	void Open() override;
	void Close() override;
	//void Play() override;
	void Stop() override;
	//void Pause() override;
	void SeekCallback(int frames, int accurate) override;
	void SeekManual(int frames, int accurate) override;
	void SetDuration(int duration) override;

	void Forward() override;
	void Backward() override;

	void EnableWidgets(bool bEnable);

protected:
	void ButtonClicked(int id, QPoint p) override;

	void resizeEvent(QResizeEvent *event);

private:
	CurrentFrameLabel *m_currentFrame;

	QList<QFrame*> m_separatorWidgets;


};

#endif // ACTIONCAMPLAYERVIEWPRIVATE_H
