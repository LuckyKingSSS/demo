/*
*Author:		zhy
*Date:			2016.03.21
*Description:	FilmoraScreen设置面板
*/


#ifndef OPTIONWIDGETT_H
#define OPTIONWIDGETT_H

#include "inc_FSCommonlib/FSCommonWidget.h"

#include "NLEComPtr.h"
#include "cVideoStreamCtrl.h"
#include "Interface/INLEPlayer.h"
#include "ICapturePlayer.h"


class SettingsWidgetBar;
class OptionBasePanel;
class FSButton;
class SettingsBasePanel;

// 设置窗口
class FSSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	FSSettingsWidget(QWidget *parent);
	~FSSettingsWidget();

	bool GetCaptureEnabled();

Q_SIGNALS:
	void signalCaptureEnabled(bool enabled);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

private:
	void CreatePanel(QString iconObjectName, QString trtext, const char *text, SettingsBasePanel *panel);
	void PanelChanged(SettingsBasePanel *panel);

private:
	SettingsWidgetBar *m_bar;
	QStackedWidget *m_stackWidget;

};


class SettingsBasePanel : public FSBaseWidget
{
	Q_OBJECT
public:
	SettingsBasePanel(QWidget *parent):FSBaseWidget(parent){ setObjectName("SettingsBasePanel"); }
	virtual void SetPanelEnabled(bool enabled, bool manual){}

Q_SIGNALS:
	// 设置面板无效，如没有摄像头
	void signalPanelEnabled(bool enabled);

	void signalPanelSetChekced(bool checked);
};

class SettingsScreenBasePanel;
// Screen窗口
class SettingsScreenPanel : public SettingsBasePanel
{
	Q_OBJECT
public:
	SettingsScreenPanel(QWidget *parent);

private:
	void CreatePanel(SettingsScreenBasePanel *panel);
	
	void SetPanelEnabled(bool enabled, bool manual) override;

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

private:
	QList<SettingsScreenBasePanel*> m_panels;

};


// 摄像头设置窗口
class SettingsCameraPanel : public SettingsBasePanel
{
	Q_OBJECT
public:
	SettingsCameraPanel(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

	void SetPanelEnabled(bool enabled, bool manual) override;

	void RetranslateUi() override;
	bool RefreshDevices();
	void RefreshResolutions(bool cmbChanged = true);
	void RefreshFramerates();
	void NewDevices();

private slots:
	void ChooseCameraIndexChanged(int index);
	void ResolutionIndexChanged(int index);
	void FramerateIndexChanged(int index);

	void _ChooseCameraIndexChanged(int index);
	void _ResolutionIndexChanged(int index);
	void _FramerateIndexChanged(int index);

private:
	FSLabel *m_lbChooseCamera;
    FSComboBox *m_cmbChooseCamera;
	FSLabel *m_lbResolution;
    FSComboBox *m_cmbResolution;
	FSLabel *m_lbFrameRate;
    FSComboBox *m_cmbFrameRate;

	QPushButton *m_btnText;
};

// 麦克风设置窗口
class SettingsAudioPanel : public SettingsBasePanel
{
	Q_OBJECT
public:
	SettingsAudioPanel(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

	void SetPanelEnabled(bool enabled, bool manual) override;
	bool RefreshDevices();
	void NewDevices();

	void RetranslateUi() override;

private slots:
	void MicrophoneIndexChanged(int index);
	void ComputerAudioIndexChanged(int index);

	void _MicrophoneIndexChanged(int index);
	void _ComputerAudioIndexChanged(int index);
private:
	FSLabel *m_lbMicrophone;
    FSComboBox *m_cmbMicrophone;
	FSLabel *m_lbComputerAudio;
    FSComboBox *m_cmbComputerAudio;

	// 选中audio checkbox时回复最后一次切换的动作
	class MQueue
	{
	public:
		void Push(int i){
			if (i != i1){
			i2 = i1; i1 = i;
			}
		}
		int Pop(){
			return qMax(0, i2);
		}
		MQueue(){ i1 = i2 = 0; }
	private:
		int i1;
		int i2;
	};
	MQueue m_computerAudioLast;
	MQueue m_microphoneLast;
	bool m_bMicrophoneLast;

};

// 高级设置窗口
class SettingsAdvancedPanel : public SettingsBasePanel
{
	Q_OBJECT
public:
	SettingsAdvancedPanel(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	void RetranslateUi() override;
private:

    FSButton *m_btnGeneral;
    FSButton *m_btnHotKeys;
	//FSButton *m_btnGeneral;
	//FSButton *m_btnHotKeys;
	QWidget *m_line;

	FSBaseWidget *m_General;
	FSBaseWidget *m_HotKeys;
};

class SettingBaseData;
class SettingsScreenBasePanel : public SettingsBasePanel
{
	Q_OBJECT
public:
	SettingsScreenBasePanel(QString iconObjectName, const char *text, QWidget *parent);

	virtual void SetRadioButtonChecked(bool checked);
	virtual bool GetRadioButtonChecked();
	int Height();

	bool GetDeviceSelected();
	virtual SettingBaseData *GetData() = 0;

Q_SIGNALS:
	// 桌面和游戏切换
	void signalToggled(bool checked);

protected:
	virtual int ShowHeight(){ Q_ASSERT(false); return 200; }
protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
    void RetranslateUi();

protected:
	//SettingBaseData *m_data;

	QRadioButton *m_radioButton;
    FSButton *m_icon;
    FSButton *m_title;

	QList<QWidget*> m_hideWidget;

	int m_cmbLeft;
	int m_cmbWidth;
	int m_titleLeft;
	int m_titleWidth;
	int m_height;

	int m_top;
	int m_spacing;
	int m_lineSpacing;

	bool m_bPreState;

    QColor m_normalColor;
    QColor m_grayColor;
};


class SettingsScreenScreenPanel : public SettingsScreenBasePanel
{
	Q_OBJECT
public:
	SettingsScreenScreenPanel(QString iconObjectName, const char *text,  QWidget *parent);

protected:
	void RetranslateUi() override;

	void SetPanelEnabled(bool enabled, bool manual) override;
	void SetRadioButtonChecked(bool checked) override;
    bool GetRadioButtonChecked() override;
    SettingBaseData *GetData() override;
private slots:
	void ScreenCaptureIndexChanged(int index);
	void FramerateIndexChanged(int index);

	void _ScreenCaptureIndexChanged(int index);
	void _FramerateIndexChanged(int index);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

	int ShowHeight() override;


protected:
    FSComboBox *m_cmbScreenCapture;
    FSButton *m_lbFrameRate;
    FSComboBox *m_cmbFrameRate;


};


class SettingsScreenGamePanel : public SettingsScreenBasePanel
{
	Q_OBJECT
public:
	SettingsScreenGamePanel(QString iconObjectName, const char *text, QWidget *parent);

	bool HaveChooseGame();
    bool GetRadioButtonChecked() override;

protected:
	void RetranslateUi() override;
	SettingBaseData *GetData() override;
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
	bool eventFilter(QObject *watcher, QEvent *event);
private:
	int ShowHeight() override;
	void RefreshGame();
	void ResetFrameratesComboBox();
	void SetPanelEnabled(bool enabled, bool manual) override;
	void SetRadioButtonChecked(bool checked) override;

	void NoAvialbleGame();


private slots:
	void GameIndexChanged(int index);
	void FramerateIndexChanged(int index);

	void _GameIndexChanged(int index, bool emitPanelSetChekced);
	void _FramerateIndexChanged(int index);
protected:
    FSComboBox *m_cmbGame;
    FSButton *m_lbFrameRate;
    FSComboBox *m_cmbFrameRate;

	FSButton *m_btnRefresh;
};


class SettingsAdvancedGeneralPanel : public FSBaseWidget
{
	Q_OBJECT
public:
	SettingsAdvancedGeneralPanel(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
	void RetranslateUi() override;

protected:
	FSLabel *m_lbRecordFile;
	QLineEdit *m_leRecordFile;
	FSBaseButton *m_btnRecordFile;
    QCheckBox *m_ckbGPU;
    QCheckBox *m_ckbAutoHideRecorder;


	FSLabel *m_lbGPU1;
	FSLabel *m_lbGPU2;
	FSLabel *m_lbGPU3;
	FSLabel *m_lbGPU4;
};


class SettingsAdvancedHotKeysPanel : public FSBaseWidget
{
	Q_OBJECT
public:
	SettingsAdvancedHotKeysPanel(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *watcher, QEvent *event);
	void showEvent(QShowEvent *event);
	void RetranslateUi() override;
private:
	bool m_enabled;

	FSLabel *m_lbStartStop;
	QLineEdit *m_leStartStop;

	FSLabel *m_lbPauseResume;
	QLineEdit *m_lePauseResume;

	FSLabel *m_lbFullscreen;
	QLineEdit *m_leFullscreen;

    FSLabel *m_lbAddMarker;
    QLineEdit *m_leAddMarker;

};


class SettingsWidgetBarItem : public FSBaseWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor selectedItemColor READ GetSelectedItemColor WRITE SetSelectedItemColor)

public:
	SettingsWidgetBarItem(QString iconObjectName, const char *text, SettingsBasePanel *widget, bool haveCheckbox, QWidget *parent);

	bool IsChecked();
	void SetChecked(bool checked);
	void SetEnabled(bool enabled);

	void SetSelected(bool selected);

	SettingsBasePanel* GetPanel();

public:
	QColor GetSelectedItemColor();
	void SetSelectedItemColor(QColor color);

Q_SIGNALS:
	void signalClicked(SettingsBasePanel *panel);
	void signalCheckBoxChanged(bool checked);
protected:
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

	void RetranslateUi() override;

private:
	SettingsBasePanel *m_panel;

	QCheckBox *m_ckb;
    FSButton *m_icon;
    FSButton *m_title;

	bool m_selected;
	QColor m_selectedColor;
};

class SettingsWidgetBar : public FSBaseWidget
{
	Q_OBJECT
public:
	SettingsWidgetBar(QWidget *parent);

	void AddWidget(QString iconObjectName, QString trtext, const char *text, SettingsBasePanel *widget, bool haveCheckbox = true);
	SettingsBasePanel* GetCurrentWidget();
	//void SetCurrentWidget(SettingsBasePanel *widget);
	bool GetCaptureEnabled();

    void SetItemHeight(int height);

	void SetChecked(int index, bool checked);
	void SetSelected(int index, bool selected);
	void SetEnabled(int index, bool enabled);
Q_SIGNALS:
	void signalPanelChanged(SettingsBasePanel *widget);
	void signalCaptureEnabled(bool enabled);

private:
	void resizeEvent(QResizeEvent *event);

private:
	QList<SettingsWidgetBarItem*> m_items;
	SettingsBasePanel *m_curWidget;
    int                m_height;
};

// 摄像头预览
class BaseStateButton;
class PreviewBase : public FSBaseDialog
{
	Q_OBJECT
public:
	PreviewBase(QWidget *parent);
	~PreviewBase();

    bool GetLock();
    void setVisible(bool visible, bool force = true);

    bool IsManualClose();
    void SetManualClose(); // 手动关闭preview

	void SetResolution(QSize size);
	HWND GetHWND();
Q_SIGNALS:
	void signalHide();
	void signalShow();
    void signalLock(bool lock);
    void signalManulClose();

    void sigDashedMove(QPoint point);
    void sigStartDashedMove(QPoint point);
    void sigEndDashedMove(QPoint point);

protected:
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);

protected:
	QWidget *m_display;
	QSize m_resolution;
    BaseStateButton *m_btnLock;
    bool m_bManualClose;
    int m_titleNameOffset;
};


// 摄像头预览
class CameraPreview : public PreviewBase
{
	Q_OBJECT
public:
	CameraPreview(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);

};

// 游戏预览
class GamePreview : public PreviewBase
{
	Q_OBJECT
public:
	GamePreview(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);

    QSize GetSize();

private:
    QTimer *m_timer;
    float m_ratio;
};



// 麦克风音量预览
class MidSlider;
class MicrophonePreview : public PreviewBase
{
	Q_OBJECT
public:
	MicrophonePreview(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);

private:
	ToolBarSlider *m_slider;

    VolumeBar *m_volume1;
	

};


void gSetContentChanged();

#endif // OPTIONWIDGETT_H
