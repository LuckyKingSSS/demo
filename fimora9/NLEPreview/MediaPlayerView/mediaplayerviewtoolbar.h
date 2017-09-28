#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>

#include <QtWidgets/QLabel>

#include <QtCore/QMap>
#include <QtCore/QString>

class MediaPlayerControl;
class BaseButton;
class ToolBarSlider;
class ToolBarSlider;
class CurrentFrameLabel;
class CAudioPillarView;

class ToolBarBase : public QWidget
{
	Q_OBJECT
public:
	enum ToolBar
	{
		ToolBar_Play = 1,
		ToolBar_Stop,
		ToolBar_Pause,
		ToolBar_Resume,
		ToolBar_Seek,
		ToolBar_Forward,
		ToolBar_Backward,
	};

public:
	ToolBarBase(MediaPlayerControl *control, QWidget *parent = nullptr);
	~ToolBarBase();

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

protected:
	MediaPlayerControl *m_mediaPlayerControl;
	ToolBarSlider* m_slider;
	QMap<int, BaseButton*> m_toolbarButtons;

	bool m_isPlaying;
	bool m_seekManual;
	int m_lastSeekFrames;
};

class AsepctRatioLabel;
class MainMediaPlayerToolBar : public ToolBarBase
{
	Q_OBJECT
public:
	enum MainToolBar
	{
		MainToolBar_RecordVoice = 0x10,
		MainToolBar_SnapShot,
		MainToolBar_Volume,
		MainToolBar_FullScreen,
	};
public:
	MainMediaPlayerToolBar(MediaPlayerControl *control, QWidget *parent);

	void Open() override;
	void Close() override;
	void Play() override;
	void Stop() override;
	void Pause() override;
	void SeekCallback(int frames, int accurate) override;
	void SeekManual(int frames, int accurate) override;
	void SetDuration(int duration) override;
	void SetFPS(float fps) override;
	void SetRatio(float ratio) override;

	void DblMouseButtonFullScreen();
protected:
	void resizeEvent(QResizeEvent *event);


	void EnableButton(bool enable);
private:
	AsepctRatioLabel *m_aspectRatio;
	CurrentFrameLabel *m_currentFrame;
	CurrentFrameLabel *m_TotalFrame;
	CAudioPillarView *m_pAudioPillar;	
};



