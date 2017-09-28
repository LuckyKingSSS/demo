#ifndef FSRECORDER_H
#define FSRECORDER_H

#include "inc_FSCommonlib/FSCommonWidget.h"

class RecordPanel;
class RecordingPanel;
class RecordLoadingCtrl;
class ActionWidget;
class RecordHelper;
class RecordMask;
class RecordWidget;
class FSRecorderMainView;
class TopLevelWindowTitleBar;
class FSRecorderMainView;
class PreviewBase;
class MicrophonePreview;
class ButtonBar;
class FSLabel;
class FSRecorder : public FSBaseWidget
{
	Q_OBJECT

public:
	FSRecorder(QWidget *parent = 0);
	~FSRecorder();
	void RetranslateUi() override;

protected:
	
	void resizeEvent(QResizeEvent *e) override;
	void closeEvent(QCloseEvent *e) override;
	//bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
private:
	//上传PID 信息到GA
	void transfromPidInfo();
	void transfromRegisterInfo();

private:
	FSRecorderMainView *m_pRecorderMainView;
	TopLevelWindowTitleBar *m_pTitleBar;
	RecordHelper *m_pHelper;
	RecordPanel *m_pRecordPanel;
	RecordMask *m_pMaskWidget;
	RecordWidget *m_pRecordWidget;
	ActionWidget *m_pActionWidget;
	PreviewBase *m_pCameraPriview;
	PreviewBase *m_pGamePriview;
	MicrophonePreview *m_pAudioPreivew;
    QPushButton *m_pBtnMinimize;
    QPushButton *m_pBtnClose;
    QLabel      *m_pLabelIcon;
    ButtonBar *m_pButtonBar;
};

class GettingStartPage;
class SetttingPage;
class PreviewPage;
class PageHeader;
class FSIconButton;
class FSRecorderMainView : public FSBaseWidget
{
	Q_OBJECT

public:
	enum Page
	{
		Page_GettingStart = 0,
		Page_Setting,
		Page_Preview,
	};
	FSRecorderMainView(QWidget *parent = 0);
	~FSRecorderMainView();
	QWidget* GetPageView(Page page);
	void ShowPage(Page page);
    Page GetCurrentPage();
Q_SIGNALS:
    void signalShowPage(Page page);

protected:
	void showEvent(QShowEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
private:
	QList<FSBaseWidget*> m_lstPages;
	PageHeader *m_pPageHeader;
};

class ButtonBar : public FSBaseWidget
{
	Q_OBJECT
public:
    ButtonBar(QWidget *titleBar, QWidget *parent );
	void SetCurrentPage(FSRecorderMainView::Page page);
	int Width();
	void TransFromBuyGaInfo();
	void TransFromRegisterGaInfo();
protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *e) override;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QString GetSaveProjectDirectory();
    void SaveProjectDirectory(QString path);
    void InitSaveProjectDirectory();
private:
	FSIconButton *m_pBtnRegistration;
	FSIconButton *m_pBtnPurchase;
	FSIconButton *m_pBtnMenu;

	QMenu *m_pMenu;
	QAction *m_pActPreviewWindow;
	QAction *m_pActCatureSetup;
	QAction *m_pActGotoEditor;

    QAction *m_pActSaveProject;
    QAction *m_pActSaveAsProject;
    QAction *m_pActExit;

    QString m_strSaveProjectDirectory;

	FSRecorderMainView::Page m_curPage;

    QWidget *m_titleBar;
};

class PageHeader : public FSBaseWidget
{
	Q_OBJECT
public:
	PageHeader(QWidget *parent = 0);
	void SetCurrentPage(FSRecorderMainView::Page page);
    FSRecorderMainView::Page GetCurrentPage();

protected:
	void RetranslateUi() override;
	void resizeEvent(QResizeEvent *e) override;
private:
	QLabel *m_pLbTitle;
	FSIconButton *m_pBtnRegistration;
	FSIconButton *m_pBtnPurchase;
	FSIconButton *m_pBtnMenu;

	QMenu *m_pMenu;
	QAction *m_pActPreviewWindow;
	QAction *m_pActCatureSetup;
	QAction *m_pActGotoEditor;

	FSRecorderMainView::Page m_curPage;

};

class ProjectListWidget;
class GettingStartPage : public FSBaseWidget
{
	Q_OBJECT
public:
	GettingStartPage(QWidget *parent = 0);
protected:
	void ConnectSignals();
	void showEvent(QShowEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
	void AppearStatus(int nState); //nState 0:没有工程  1:有工程
	void RetranslateUi() override;

private:

	FSNormalButton *m_pBtnGetStart;
	QPushButton *m_pBtnQuickCapture;
	QPushButton *m_pBtnVideoEditor;
	ProjectListWidget *m_pProjectList;

	int m_nAppreaState;
};

class FSSettingsWidget;
class SetttingPage : public FSBaseWidget
{
	Q_OBJECT
public:
	SetttingPage(QWidget *parent = 0);

protected:
	void ConnectSignals();
	void resizeEvent(QResizeEvent *e) override;
	void showEvent(QShowEvent *e) override;
	
	void RetranslateUi() override;
private:
    FSButton *m_pbtnCapture;
	FSSettingsWidget *m_pSettingWidget;
};
class RecorderPreview;
class PreviewPage : public FSBaseWidget
{
	Q_OBJECT
public:
	PreviewPage(QWidget *parent = 0);
	void Reset();
	QPixmap GetFirstItemThumbnail();
public slots:
	void slotRecordStop(QString path);
	void slotCaptureMore();
	void slotExport2Editor(QStringList paths);
	void slotExport2Local(QStringList paths);
protected:
	void ConnectSignals();
	void showEvent(QShowEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;
private:
	RecorderPreview *m_preview;
};


class RecordHelper : public QObject
{
	Q_OBJECT

public:
	enum DragableRectIndex
	{
		DragableRectIndex_Left = 0,
		DragableRectIndex_LeftTop,
		DragableRectIndex_Top,
		DragableRectIndex_RightTop,
		DragableRectIndex_Right,
		DragableRectIndex_RightBottom,
		DragableRectIndex_Bottom,
		DragableRectIndex_LeftBottom,
		DragableRectIndex_Center
	};
public:
	static RecordHelper* GetInstance();
	void DragAndMove(DragableRectIndex index, QPoint pos);

	void SetRecordRect(QRect rc);
    void SetPreviousRect(QRect rc);
	void SetRecordRectLeft(int l);
	void SetRecordRectRight(int r);
	void SetRecordRectTop(int t);
	void SetRecordRectBottom(int b);
	void StartMove(QPoint pt);
    void StopMove();
	void MoveRecordRect(QPoint pt); //x,y :正值表示向右向下移动，负值表示向左向上移动
    void SetDesktopRect(QRect rc);

	QRect DesktopRect();
	QRect RecordRect();
	QRect PreviousRect();
	QRect CustomRect();
	QRect LeftMaskRect();
	QRect TopMaskRect();
	QRect RightMaskRect();
	QRect BottomMaskRect();
	QRect DragableRect(DragableRectIndex index);

	bool IsLock();
	void SetLock(bool bLock);
	bool GetShiftLock();
	void SetShiftLock(bool bLock);
	int GetMinW();
	void SetMinW(int w);
	int GetMinH();
	void SetMinH(int h);
	int GetMaxW();
	void SetMaxW(int w);
	int GetMaxH();
	void SetMaxH(int h);
	double GetRatio();
	void UpdateRatio();

	bool IsRecordRectModified();
    bool IsDraging();

Q_SIGNALS:
	void Update();
	void sigResolutionChangedByEdit(QRect rc);
	void sigResolutionChangedByDrag(QRect rc);
	void sigShiftLock(bool b);
protected:
	void ManualDrag(DragableRectIndex index, QPoint pos);
	void RestrictDrag(DragableRectIndex index, QPoint pos);
	void CalcDragableRects();

private:
	RecordHelper(QObject *parent = 0);
	QList<QRect> m_dragableRects;
	QRect m_rcDesktop;


	QRect m_rcRecord;
	QRect m_rcPrevious;
	QRect m_rcStartMove;
	QPoint m_ptStartMove;

	QRect m_rcCustom;

	float m_fLockRatio;
	bool m_Lock;
	bool m_ShiftLock;
	int m_minW;
	int m_minH;
	int m_maxW;
	int m_maxH;
	bool m_bRecordRectModified;
    bool m_bDraging;
};

class RecordWidget : public QWidget
{
	Q_OBJECT

public:
	RecordWidget(RecordHelper *helper, QWidget *parent = 0);
	~RecordWidget();

protected:
	void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent *e) override;
	void showEvent(QShowEvent *e) override;

	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
private:
	RecordHelper *m_pHelper;
	RecordHelper::DragableRectIndex m_PressIndex;

	QPoint m_ptPre;
};

class RecordMask : public QWidget
{
	Q_OBJECT
public:
	RecordMask(RecordHelper *helper, QWidget *parent = 0);
protected:
	void showEvent(QShowEvent *e) override;
	void paintEvent(QPaintEvent *e) override;
private:
	RecordHelper *m_pHelper;
};

class RecorderCenter : public QObject
{
	Q_OBJECT
public:
	enum RecordState
	{
		RecordState_None = 0,
		RecordState_Recording,
		RecordState_Pause,
	};
	static RecorderCenter* GetInstance();
	void Start();
	void Stop();
	void Abort();
	void Pause();
	void Resume();
	RecordState GetReordState();
	QString GetOutputFile();
    bool IsRecording();
	void EnableMouseHook(bool bEnable);
	void EnableKeyboardHook(bool bEnable);
	
	void PrepareCameraStream();
	void StopCameraStream();
Q_SIGNALS:
	void sigRecordStateChange();
    void sigStartFail(QString errString);
protected:
	QString GenerateOutputFileName();
	void SetState(RecordState state);
private:
	RecorderCenter(QObject *parent = 0);
	QString m_strOutputFile;
	RecordState m_recorState;
};


class ActionWidget : public QWidget
{
	Q_OBJECT
public:
	ActionWidget(QWidget *parent = 0);

	void ShowWidget();
	void HideWidget();
	void SetPaused(bool bPause);
protected:
	void paintEvent(QPaintEvent *e) override;
private:
	QColor m_clrCorner;
	QTimer m_timer;
	bool m_bPause;
};

#endif // FSRECORDER_H
