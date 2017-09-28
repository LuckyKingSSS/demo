#ifndef FEDITPlAYER_H
#define FEDITPlAYER_H

#include "inc_FCommonLib/FBaseWidget.h"
#include "inc_FPropertyPanel/fpropertypanel_global.h"

class FEditPlayerMgr;
class FEditRectangle;
class FEditPlayerImpl;
class FNLETimeline;

class FPROPERTYPANEL_EXPORT FEditPlayer : public FBaseWidget
{
	Q_OBJECT

public:
	FEditPlayer(QWidget *parent = 0);
	~FEditPlayer();

	// 播放器操作
	bool Open(FNLETimeline *timeline);
	bool Close();
	void Play();
	void Stop();
	void Pause();
	void Seek(int frames);
	void SetDuration(int duration);
	void Forward();
	void Backward();
	void Restore();
	int  Duration();
	bool HaveSource();
	int  CurrentFrame();
	bool IsPlaying();

	// 矩形框操作
	void SetRectangleVisible(bool);

protected:
	void resizeEvent(QResizeEvent *) override;

private:
	FEditPlayerImpl *m_pImpl;
};

#endif // FEDITPlAYER_H
