#ifndef RECORDPANEL_H
#define RECORDPANEL_H
#include "inc_FSCommonlib/FSCommonWidget.h"

class FSTitleBar;
class DesktopGameRecord;
class BaseButton;
class TopLevelWindowTitleBar;
class PreviewCtrlWidget;
class RecordButton;
class CountBackwardCtrl;
class SeperaterLine;
class FreeSpaceChecker;
class MiniRecordPanel;
class SyncRecordPanelAndMiniPanelState;
class BaseStateButton;


class RecordPanel : public FSBaseWidget
{
	Q_OBJECT

public:
	enum UiState
	{
		UiState_Stop = 0,
		UiState_Pause,
		UiState_Recording,
	};
	RecordPanel(QWidget *parent = 0);
	~RecordPanel();
	void Resize();
	void SetCommentTip(int state); //0: 锁定比例提示  1：全屏快捷键提示
	void SetUiState(UiState s);
	void EnableStopButton(bool bEnable);
	void SetCommentVisible(bool bVisible);
	bool IsCameraPreviewEnabled();
	bool IsGamePreviewEnabled();
	bool IsAudioPreviewEnabled();
	void SetPreviewWindowShowed();
	PreviewCtrlWidget *GetPreviewCtrlWidget();
	bool CanGo2PreviewWin();

    void SetVisible(bool visible, bool force = true);
    bool GetLock();

    QWidget* GetRealRecordWidget();
    void ShowLoadingView();
    void LoadingAbort();
Q_SIGNALS:
	void sigNormalStop(QString path);
    void sigDashedMove(QPoint point);
    void sigStartDashedMove(QPoint point);
    void sigEndDashedMove(QPoint point);
protected:
    void setVisible(bool visible);

	void ConnectSignals();
	void DoStart();
	void DoStop();
	void DoPause();
	void DoResume();
	void DoLoading();
	void resizeEvent(QResizeEvent *e) override;
	void showEvent(QShowEvent *e) override;
	//void hideEvent(QHideEvent *e) override;
	void RetranslateUi();
	//void closeEvent(QCloseEvent *e) override;
	void OnStartButtonClicked();
    void OnRecordStateChanged();

private:
	TopLevelWindowTitleBar *m_pTitleBar;
    QPushButton *m_pBtnSettings;
    QPushButton *m_pBtnPreview;
    QPushButton *m_pBtnEditor;

	RecordButton *m_pBtnRecord;
	BaseStateButton *m_pBtnPauseResume;
	CountBackwardCtrl *m_pCountBackwardCtrl;
	BaseButton *m_pBtnDelete;
	PreviewCtrlWidget *m_pPreviewCtrlWidget;
	QLabel *m_pLbComments;

	SeperaterLine *m_pLine1;
	SeperaterLine *m_pLine2;
	SeperaterLine *m_pLine3;
	SeperaterLine *m_pLine5;
	QWidget *m_pBlackMask;

	//QMenu *m_pMenu;
	//QAction *m_pActCaptureSetup;
	//QAction *m_pActPreviewWindow;
	//QAction *m_pActGotoEditor;

    BaseButton *m_pBtnMiniPanel;
    BaseButton *m_pBtnMinimized;
	BaseButton *m_pBtnClosed;

	UiState m_UiState;
	int m_nCommontlabelHeight;
	int m_nCommontTipState;
	bool m_bPreviewShowed;

	FreeSpaceChecker *m_pFreeSpaceChecker;
	QString m_strNoEnoughSpace;

    BaseStateButton *m_pBtnLock;

private:
    MiniRecordPanel *m_pMiniRecordPanel;
    SyncRecordPanelAndMiniPanelState *m_pSyncPanelState;
    friend class SyncRecordPanelAndMiniPanelState;
    friend class PrimaryScreenManager;
};

class SeperaterLine : public FSBaseWidget
{
	Q_OBJECT
public:
	SeperaterLine(QWidget *parent = 0);
};

class PreviewItemWidgetBase;
class DesktopPreviewItemWidget;
class GamePreviewItemWidget;
class CameraPreviewItemWidget;
class MicrophonePreviewItemWidget;
class SystemAudioPreviewItemWidget;
class PreviewCtrlWidget : public FSBaseWidget
{
	Q_OBJECT
public:
	PreviewCtrlWidget(const QString &objname="",QWidget *parent = 0);
	int Height();
	void Reflesh(bool bResizeParentWidget = true);
	void SetIndicatorOn(bool bOn);
	void SetGamePreviewButtonOff();
	void SetCameraPreviewButtonOff();
	void SetMicrophonePreviewButtonOff();

	bool IsCameraPreviewOn();
	bool IsGamePreviewOn();
	bool IsMicrophonePreviewOn();

	void Resize();
	DesktopPreviewItemWidget* GetDesktopPreviewWidget();
protected:
	void resizeEvent(QResizeEvent *e) override;
Q_SIGNALS:
	void sigGamePreviewOn(bool bOn);
	void sigCameraPreviewOn(bool bOn);
	void sigMicrophonePreviewOn(bool bOn);
private:
	DesktopPreviewItemWidget *m_pDesktopItem;
	GamePreviewItemWidget *m_pGameItem;
	CameraPreviewItemWidget *m_pCameraItem;
	MicrophonePreviewItemWidget *m_pMicrophoneItem;
	SystemAudioPreviewItemWidget *m_pSystemAudioItem;

	QList<PreviewItemWidgetBase*> m_listEnbleItems;

};

class Flicker;
class PreviewItemWidgetBase : public FSBaseWidget
{
	Q_OBJECT
public:
	PreviewItemWidgetBase(const QString &objname = "", QWidget *parent = 0);
	void SetIndicatorOn(bool bOn);
	void SetPreviewButtonOff();
	bool IsPreviewOn();


	BaseStateButton *GetPreviewButton();
protected:
	virtual void resizeEvent(QResizeEvent *e) override;
    void RetranslateUi() override;

protected:
	Flicker *m_pIndicator;
	QLabel *m_pIcon;
	QLabel *m_pTitle;
	BaseStateButton *m_pBtnPreview;
};

class DesktopPreviewItemWidget : public PreviewItemWidgetBase
{
	Q_OBJECT
public:
	DesktopPreviewItemWidget(const QString &objname = "", QWidget *parent = 0);
	void SetDesktopWH();
	void SetWidthEdit(int w);
	void SetHeightEdit(int h);
protected:
	void resizeEvent(QResizeEvent *e) override;
    void RetranslateUi() override;
protected:
    FSLineEdit *m_pEditWidth;
    FSLineEdit *m_pEditHeight;
	QLabel *m_pLbX;
	BaseStateButton *m_pBtnLock;
    BaseStateButton *m_pBtnSwitchScreen;
};

class SystemAudioPreviewItemWidget : public PreviewItemWidgetBase
{
	Q_OBJECT
public:
	SystemAudioPreviewItemWidget(const QString &objname = "", QWidget *parent = 0);
protected:
	void RetranslateUi() override;
    void resizeEvent(QResizeEvent *e) override;

};

class MicrophonePreviewItemWidget : public PreviewItemWidgetBase
{
	Q_OBJECT
public:
	MicrophonePreviewItemWidget(const QString &objname = "", QWidget *parent = 0);
	void RetranslateUi() override;
protected:
	void resizeEvent(QResizeEvent *e) override;
};

class CameraPreviewItemWidget : public PreviewItemWidgetBase
{
	Q_OBJECT
public:
	CameraPreviewItemWidget(const QString &objname = "", QWidget *parent = 0);
	void RetranslateUi() override;

protected:
	void resizeEvent(QResizeEvent *e) override;
};

class GamePreviewItemWidget : public PreviewItemWidgetBase
{
	Q_OBJECT
public:
	GamePreviewItemWidget(const QString &objname = "", QWidget *parent = 0);
	void RetranslateUi() override;
protected:
	void resizeEvent(QResizeEvent *e) override;
};

class Flicker : public FSBaseWidget
{
	Q_OBJECT
public:
	Flicker(const QString &objname = "", QWidget *parent = 0);
	void SetOn(bool bOn);
protected:
	void resizeEvent(QResizeEvent *e) override;
protected:
	QLabel *m_pLbOn;
	QLabel *m_pLbOff;
};



class RecordButton : public FSBaseWidget
{
	Q_OBJECT
public:
	enum ButtonId
	{
		ButtonId_Start = 0,
		ButtonId_Stop,
		//ButtonId_Resume,
	};
	RecordButton(const QString &objname = "",QWidget *parent = 0);
	void SetCurrentId(ButtonId id);
protected:
	void showEvent(QShowEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
	void RetranslateUi() override;
Q_SIGNALS:
	void sigClicked(ButtonId id);
private:
	QList<BaseButton*> m_lstButtons;
	QLabel *m_pLbTip;
	QLabel *m_pLbShortcut;
	ButtonId m_CurrentID;
};

class CountBackwardCtrl : public FSBaseWidget
{
	Q_OBJECT
public:
	CountBackwardCtrl(const QString &objname = "",QWidget *parent = 0);
	void Reset();
	void Start();
	void Pause();
	void Resume();
	void Stop();
protected:
	void Update();
	void resizeEvent(QResizeEvent *e) override;
private:
	QLabel *m_pLbTime;
	QTimer m_timer;
	int m_nCount;
	bool m_bPause;
};

class CountCtrl;
class RecordLoadingCtrlRecordPanelMask;
class RecordLoadingCtrl : public FSBaseWidget
{
	Q_OBJECT

public:
	RecordLoadingCtrl(QWidget *parent = 0);
	void Start();
Q_SIGNALS:
	void sigLoadingFinish();
	void sigLoadingAbort();
protected:
	void RetranslateUi() override;
	void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
	void paintEvent(QPaintEvent *e) override;
    void closeEvent(QCloseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
private:
	QLabel *m_pLbContainer;
	CountCtrl *m_pCountCtrl;
	QLabel *m_pLbTitle;
	QLabel *m_pLbStopHotkey;
	QLabel *m_pLbPauseHotkey;
	QLabel *m_pLbStopTip;
	QLabel *m_pLbPauseTip;
	QPushButton *m_pBtnCancel;

	QTimer m_timer;
	int m_nCount;
    QSize m_containerSize;
    QRect m_recordRect;

    // 倒计时将recordpanel盖住不给操作，主要在多屏时
    RecordLoadingCtrlRecordPanelMask *m_recordPanelMark;
};

class CountCtrl : public FSBaseWidget
{
	Q_OBJECT
public:
	CountCtrl(QWidget *parent = 0);
	void SetCount(int cnt);
	void Start();
	void Abort();
protected:
	void Finish();
	void paintEvent(QPaintEvent *e) override;
Q_SIGNALS:
	void sigFinish();
private:
	QTimer m_timer;
	QTimer m_timer2;
	int m_nCount;
	int m_nCount2;
	int m_nCount3;

	static const int m_timerspan = 50;
};

class SysTrayIcon : public QObject
{
	Q_OBJECT
public:
	static SysTrayIcon* GetInstance();
	~SysTrayIcon();
	void SetVisible(bool bVisible);
Q_SIGNALS:
	void sigOpen();
	void sigQuit();
protected:
	SysTrayIcon(QObject *parent = 0);
	void RetranslateUi();
	void Show();
	void Hide();

    void IgnoreDoubleClick();

private:
	QSystemTrayIcon *m_pTrayIcon;

	QMenu *m_pMenu;
	QAction *m_pActOpen;
	QAction *m_pActQuit;
	QIcon *m_icoNormal;
	QIcon *m_icoRecording;
	QIcon *m_icoPause;

    QTimer *m_doublieTimer;
};

#include "inc_FSCommonlib/MediaImportDlg.h"

class StopRecordMediaImportDlg : public QDialog
{
    Q_OBJECT
public:
    StopRecordMediaImportDlg(RecordPanel *panel, QWidget *parent);

    void DoModal();

protected:
    void showEvent(QShowEvent *event);  
    void paintEvent(QPaintEvent *event);

    void Run();

    Q_INVOKABLE void NormalStop(QString path);
protected:
    RecordPanel *m_panel;
    int m_arcAngle;
};


class GlobalMenu : public QMenu
{
    Q_OBJECT
public:
    GlobalMenu(QWidget *parent);
    ~GlobalMenu();

protected:
    void TestClose(QPoint pt);

    bool m_bMouseHook;
};


class FreeSpaceChecker : public QObject
{
	Q_OBJECT
public:
	FreeSpaceChecker(QObject *parent);

	bool DoCheck();
	void StartCheck();
	void StopCheck();
Q_SIGNALS:
	void sigSpaceNotEnough();
private:
	QTimer m_timer;

};

class RecordLoadingCtrlRecordPanelMask : public FSBaseWidget
{
    Q_OBJECT

public:
    RecordLoadingCtrlRecordPanelMask(QWidget *parent = 0);
private:
    void paintEvent(QPaintEvent *e) override;

};

#endif // RECORDPANEL_H
