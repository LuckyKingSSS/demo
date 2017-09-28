#ifndef FMOTIONPANNEL_H
#define FMOTIONPANNEL_H

#include "FPropertyPanel.h"
#include "inc_FPropertyPanel\fpropertypanel_global.h"

class FButton;
class QListWidget;
class FMotionPannelMgr;
class MediaPlayerView;
class FPROPERTYPANEL_EXPORT FMotionPanel : public FPropertyPanel
{
	Q_OBJECT

public:
	FMotionPanel(QWidget *parent);
	~FMotionPanel();
	// 窗口初始化
	virtual void Init();
	// 窗口将显示
	virtual void Show();
	// 窗口将隐藏
	virtual void Hide();
	// 窗口enable操作
	virtual void SetEnabled(bool enabled);
	// 窗口将销毁
	virtual void Destory();
	// 窗口更新数据
	virtual void Update() override;

protected:
	void resizeEvent(QResizeEvent *event)override;

Q_SIGNALS:
	void  sigConfirmSetting();


private:
	void onPlayMotion(QString strFilePath);


private:
	void InitUI();
	void InitData();
	void InitConnections();

private:

	FButton* m_AdvanceBtn;
	FButton* m_OkBtn;
	QListWidget* m_ListWidget;
	FMotionPannelMgr* m_PannelMgr;
	MediaPlayerView* m_Player;
};

#endif // FMOTIONPANNEL_H
