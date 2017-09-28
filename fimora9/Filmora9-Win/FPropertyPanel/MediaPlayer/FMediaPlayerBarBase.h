#ifndef FMEDIAPLAYERBASEBAR_H
#define FMEDIAPLAYERBASEBAR_H

#include <QMap>
#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "inc_FPropertyPanel\fpropertypanel_global.h"

class ToolBarSlider;
class IMediaPlayerControl;
class BaseButton;
class FPROPERTYPANEL_EXPORT FMediaPlayerBarBase : public FBaseWidget
{
	Q_OBJECT

public:
	enum ToolBarButton
	{
		ToolBar_Play = 1,
		ToolBar_Stop = 2,
		ToolBar_Pause = 4,
		ToolBar_Resume = 8,
		ToolBar_Forward = 16,
		ToolBar_Backward = 32,
	};
	FMediaPlayerBarBase(IMediaPlayerControl *control, QWidget *parent);
	~FMediaPlayerBarBase();
	void SetShowButton(int iType);
	ToolBarSlider* GetToolBarSlider(){ return m_PlaySlider; };
	QMap<int, BaseButton*> GetButtons(){ return m_toolbarButtons; };

	virtual void Open();
	virtual void Close();
	virtual void Play();
	virtual void Stop();
	virtual void Pause();
	virtual void SeekCallback(int frames, int accurate);
	virtual void SeekManual(int frames, int accurate);

	virtual void SetDuration(int duration);
	virtual void SetFPS(float fps){}
	virtual void SetRatio(float ratio){}

	virtual void Forward();
	virtual void Backward();

	int CurrentFrames();
	int Duration();
	bool GetSliderButtonPressDown();
	bool IsSeeking();
	BaseButton* GetButton(int id);
	virtual void EnableWidgets(bool bEnable);


Q_SIGNALS:
	void Clicked(int id, QPoint p);
	void RulerSeekCallback(int frames);
	void StartRulerSeekCallback(bool isPlaying);
	void EndRulerSeekCallback(bool isPlaying);

private:
	Q_SLOT void SeekCallbackSlot(int frames);

protected:
	void CreateToolBarButton(int id, const QString objectName = "");
	void CreateToolBarStateButton(int firstId, QString firstObjectName, int secondId, QString secondObjectName);
	void ToolBarPlayButtonStateChanged(int id);
	virtual void ButtonClicked(int id, QPoint p);

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	ToolBarSlider* m_PlaySlider;
	IMediaPlayerControl *m_mediaPlayerControl;
	QMap<int, BaseButton*> m_toolbarButtons;

	bool m_isPlaying;
	bool m_seekManual;
	int m_lastSeekFrames;
};

#endif // FMEDIAPLAYERBASEBAR_H
