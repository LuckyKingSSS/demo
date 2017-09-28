#ifndef FAUDIOPROPERPANEL_H
#define FAUDIOPROPERPANEL_H

#include "FPropertySlider.h"
#include "FPropertyPanel.h"
#include "inc_FCommonLib\FCommonWidgets.h"
#include "FPropertyCommon.h"
#include "inc_FPropertyPanel\fpropertypanel_global.h"

class FEqualizerCtl;
class FSpeedCtl;
class FDenoiseCtl;
class FPROPERTYPANEL_EXPORT FAudioProperPanel : public FPropertyPanel
{
	Q_OBJECT

public:
	FAudioProperPanel(QWidget *parent);
	~FAudioProperPanel();
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

public Q_SLOTS:

	void slotVolumeChanged(int iVolume);
	void slotFadeInChanged(float fValue);
	void slotFadeOutChanged(float fValue);
	void slotPitchChanged(int iValue);
	void slotConfirmConfig();
	void slotSpeedChanged(float fValue);
	void slotEqualizerChanged(QString strType);
	void slotDenoiseChnaged(QString strType);

protected:
	void resizeEvent(QResizeEvent *e)override;

Q_SIGNALS:
	void sigPausePlay();
	void sigRefreshTimelinePro();

private:
	void initUI();
	void initConnections();

private:
	FSpeedCtl*      m_SpeedCtl;
	FPropertySlider* m_VolumSlider;
	FPropertySlider* m_FadeinSlider;
	FPropertySlider* m_FadeoutSlider;
	FPropertySlider* m_PitchSlider;
	FEqualizerCtl* m_EqualizerCtl;
	FDenoiseCtl*   m_DenoiseCtl;
	BottomButtonBar* m_BottomBar;
};

class FSpeedCtl : public FBaseWidget
{
	Q_OBJECT
public:
	FSpeedCtl(QWidget *parent);
	~FSpeedCtl(){};
Q_SIGNALS:
	void sigSpeedChanged(float fValue);

protected:
	void resizeEvent(QResizeEvent *e)override;

private:
	void initUI();
	void initData();
	void InitConnections();

private:
	FLabel*  m_SpeedLabel;
	FNonLinearSlider* m_SpeedSlider;
};


class FFEqualizerCtlMgr;
class FEqualizerCtl :public FBaseWidget
{
	Q_OBJECT
public:
	FEqualizerCtl(QWidget *parent);
	~FEqualizerCtl(){};

Q_SIGNALS:
	void sigEqualizerChanged(QString strType);

protected:
	void resizeEvent(QResizeEvent *e)override;

private:
	void initUI();
	void initData();

private:
	FLabel*   m_EqualizerLabel;
	FComboBox* m_EqualizerCombo;
	FButton* m_CustomizeBtn;
	FFEqualizerCtlMgr* m_DataMgr;
};

class FFEqualizerCtlMgr :public QObject
{
	Q_OBJECT
public:
	FFEqualizerCtlMgr(QObject* parent);
	~FFEqualizerCtlMgr(){};
	QList<QString>& getTotalType(){ return m_TotalType; };

private:
	QList<QString> m_TotalType;
};


class FDenoiseCtl :public FBaseWidget
{
    Q_OBJECT
public:
	FDenoiseCtl(QWidget *parent);
	~FDenoiseCtl(){};
	bool bIsDenoiseEnable(){ return  m_bEnable; };

Q_SIGNALS:
	void sigRemoveBackgroundNoise(bool bState);
	void sigDenoiseTypeChanged(QString strType);

protected:
	void resizeEvent(QResizeEvent *e)override;
private: 
	void initUI();
	void initData();
	void initConnections();

private:
	FLabel* m_DenoiseLabel;
	FCheckBox* m_DenoiseCheckBox;
	FNonLinearSlider* m_DenoiseSlider;
	bool m_bEnable;
};


#endif // FAUDIOPROPERPANEL_H
