#ifndef RECORDERVIEWSMGR_H
#define RECORDERVIEWSMGR_H

#include <QObject>


class FSRecorder;
class FSRecorderMainView;
class RecordPanel;
class RecordMask;
class RecordWidget;
class RecordLoadingCtrl;
class ActionWidget;
class PreviewBase;
class MicrophonePreview;
class RecorderViewsMgr : public QObject
{
	Q_OBJECT

public:
	enum ViewPage
	{
		ViewPage_None = 0,
		ViewPage_Main,
		ViewPage_Record,
		ViewPage_Loading,
		ViewPage_Recording,
	};
	static RecorderViewsMgr* GetInstance();
	ViewPage GetCurrentViewPage();
	void ResetWidgetPos();

	void ShowMainView(int page);
	void ShowRecordWidget();
	void ShowLoadingView();
	void ShowRecordingView();
	void ShowPreviewWidgets(bool show);
	void HideAllView(bool force = true);
	void GotoEditor();

	void SetFSRecorder(FSRecorder *pView);
	FSRecorder* GetFSRecorder();
	void SetFSRecorderMainView(FSRecorderMainView *pView);
	FSRecorderMainView* GetFSRecorderMainView();
	void SetRecordPanel(RecordPanel *pView);
	RecordPanel* GetRecordPanel();
	void SetRecordMask(RecordMask *pView);
	RecordMask* GetRecordMask();
	void SetRecordWidget(RecordWidget *pView);
	RecordWidget* GetRecordWidget();
	void SetActionWidget(ActionWidget *pView);
	ActionWidget* GetActionWidget();
	void SetCameraPreview(PreviewBase *);
	PreviewBase *GetCameraPreview();
	void SetGamePreview(PreviewBase *);
	PreviewBase *GetGamePreview();
	void SetAudioPreview(MicrophonePreview *);
	MicrophonePreview *GetAudioPreview();

	void Clear();

Q_SIGNALS:
    void sigLoadingFinish();
    void sigLoadingAbort();

protected:
	RecorderViewsMgr(QObject *parent = 0);
    ~RecorderViewsMgr();
	void SetCurrentViewPage(ViewPage vp);
    void SetRecordLoadingCtrl(RecordLoadingCtrl *pView);
    RecordLoadingCtrl* GetRecordLoadingCtrl();
private:
	FSRecorder *m_pFSRecorder;
	FSRecorderMainView *m_pFSRecorderMainView;
    // 控制面板
	RecordPanel *m_pRecordPanel;
    // 桌面录制框灰色区域
	RecordMask *m_pRecordMask;
    // 桌面录制拖拽框
    RecordWidget *m_pRecordWidget;
    // 倒计时框
	RecordLoadingCtrl *m_pRecordLoadingCtrl;
    // 录制中闪烁框
	ActionWidget *m_pActionWidget;
	PreviewBase *m_pCameraPreview;
	PreviewBase *m_pGamePreview;
	MicrophonePreview *m_pAudioPreview;

	ViewPage m_currentView;
};


class CurrentViewPageMgr
{
public:
    static CurrentViewPageMgr* GetInstance();
    void Release();

    void ShowNeedShowWindow();

    void SetCurrentViewPage(RecorderViewsMgr::ViewPage page);
    RecorderViewsMgr::ViewPage GetCurrentViewPage();

    bool TerminateFSEditor();

protected:
    CurrentViewPageMgr();
    ~CurrentViewPageMgr();

    QSharedMemory *memory;
};

#endif // RECORDERVIEWSMGR_H
