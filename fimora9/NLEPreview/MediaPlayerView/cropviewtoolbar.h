#ifndef CROPVIEWTOOLBAR_H
#define CROPVIEWTOOLBAR_H

#include "mediaplayerviewtoolbar.h"
class QRadioButton;
class FrameLabel;
class CropViewToolBar : public ToolBarBase
{
	Q_OBJECT

public:
	enum CropView_BtnID
	{
		CropView_BtnID_RadioManually = 0x1000,
		CropView_BtnID_Radio169,
		CropView_BtnID_Radio43,
		CropView_BtnID_Reset,
		CropView_BtnID_OK,
		CropView_BtnID_Cancel
	};

	CropViewToolBar(MediaPlayerControl *control, QWidget *parent);
	~CropViewToolBar();

	void Open() override;
	void Close() override;
	void Play() override;
	void Stop() override;
	void Pause() override;
	void SeekCallback(int frames, int accurate) override;
	void SeekManual(int frames, int accurate) override;
	void SetDuration(int duration) override;
	void SetFPS(float fps) override;

public Q_SLOTS:
	void slotButtonToggled(int,bool);

Q_SIGNALS:
	void signalButtonClicked(int nId);
	

protected:
	void resizeEvent(QResizeEvent *event) override;

protected:
	QRadioButton *m_pRadioManually;
	QRadioButton *m_pRadio169;
	QRadioButton *m_pRadio43;

	QLabel *m_pLabelConstrain;
	FrameLabel *m_pCurrentFrame;

	QPushButton *m_pBtnReset;
	QPushButton *m_pBtnOK;
	QPushButton *m_pBtnCancel;
};

class ToolBarLabel;
class FrameLabel : public QWidget
{
	Q_OBJECT
public:
	FrameLabel(QWidget*parent = 0);
	~FrameLabel();

	void SetFPS(float fps);
	void SetCurrentFrame(int frames);
	void SetDuration(int duration);
protected:
	void resizeEvent(QResizeEvent *event);
private:
	ToolBarLabel *m_curFrame;
	float m_fps;
	int m_duration;

	QString m_strDerationStr;
};



#endif // CROPVIEWTOOLBAR_H
