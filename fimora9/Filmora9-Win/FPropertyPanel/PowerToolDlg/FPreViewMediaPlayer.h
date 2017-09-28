#ifndef FPREVIEWMEDIAPLAYER_H
#define FPREVIEWMEDIAPLAYER_H

#include <QMap>
#include "..\include\inc_FCommonLib\FBaseWidget.h"
#include "..\include\inc_FPropertyPanel\fpropertypanel_global.h"
#include "FMediaPlayerBarBase.h"

class FPowerToolPlayer;
class FPreViewMediaPlayer : public FBaseWidget
{
	Q_OBJECT

public:
	FPreViewMediaPlayer(QWidget *parent);
	~FPreViewMediaPlayer();

private:
	
};

class MediaPlayerView;
class FLabel;
class ToolBarSlider;
class FBaseWidget;
class FCommonPlayerBase : public FMediaPlayerBarBase
{
	Q_OBJECT
public:
	FCommonPlayerBase(IMediaPlayerControl *control, QWidget *parent);
	~FCommonPlayerBase(){};
	void SetPlayer(QWidget* player);
	void SetSliderWidth(int iWidth);
	void SetSliderHeight(int iHeight);
	void SetMediaToSliderSpace(int iSpace);
	FBaseWidget* GetPlayerWidget(){ return m_playerWidget; };
private:
	void InitUI();
	void InitConnections();

private:
	FBaseWidget* m_playerWidget;
	QWidget* m_Player;
	FLabel* m_PastTimeLabel;
	FLabel* m_TotalTimeLabel;
	FLabel* m_SeparatorLabel;
	ToolBarSlider* m_ToolBarSlider;
	QMap<int, BaseButton*> m_AllBtns;
};

class FLabel;
class MediaPlayerView;
class  FPowerToolPlayer : public FCommonPlayerBase
{
	Q_OBJECT

public:
	FPowerToolPlayer(IMediaPlayerControl *control, QWidget *parent);
	~FPowerToolPlayer();
protected:
	void resizeEvent(QResizeEvent *event)override;
};


class FLabel;
class MediaCropView;
class CropPlayerView;
class FNLETimeline;
class  FPROPERTYPANEL_EXPORT FCropPlayer : public FCommonPlayerBase
{
	Q_OBJECT

public:
	FCropPlayer(IMediaPlayerControl *control, QWidget *parent);
	~FCropPlayer(){};
	void SetTimeline(FNLETimeline* timeLine);

protected:
	void resizeEvent(QResizeEvent *event)override;
Q_SIGNALS:
	void signalOK();
	void signalCancel();
private:
	CropPlayerView* m_CropPlayer;
};

#endif // FPREVIEWMEDIAPLAYER_H
