#include "stdafx.h"
#include "RecorderViewsMgr.h"
#include "FSRecorder.h"
#include "RecordPanel.h"
#include "SettingsDataManager.h"
#include "FSSettingsWidget.h"
#include "Communicator.h"
#include "inc_MKCollecter/MKCollecter.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "PrimaryScreenManager.h"

RecorderViewsMgr::RecorderViewsMgr(QObject *parent)
	: QObject(parent)
{
	SetCurrentViewPage(ViewPage_None);
    m_pRecordLoadingCtrl = new RecordLoadingCtrl;
}

RecorderViewsMgr::~RecorderViewsMgr()
{
    CurrentViewPageMgr::GetInstance()->Release();
    if (m_pRecordLoadingCtrl)
        delete m_pRecordLoadingCtrl;
}

RecorderViewsMgr* RecorderViewsMgr::GetInstance()
{
	static RecorderViewsMgr ins;
	return &ins;
}

void RecorderViewsMgr::SetCurrentViewPage(ViewPage vp)
{
	m_currentView = vp;
	switch (m_currentView)
	{
	case RecorderViewsMgr::ViewPage_None:
	case RecorderViewsMgr::ViewPage_Main:
	case RecorderViewsMgr::ViewPage_Loading:
		MKCollecter::GetInstance()->SetKeyboardHook(false);
		break;
	case RecorderViewsMgr::ViewPage_Record:
	case RecorderViewsMgr::ViewPage_Recording:
		MKCollecter::GetInstance()->SetKeyboardHook(true);
		break;
	}

    CurrentViewPageMgr::GetInstance()->SetCurrentViewPage(vp);
}

RecorderViewsMgr::ViewPage RecorderViewsMgr::GetCurrentViewPage()
{
	return m_currentView;
}

void RecorderViewsMgr::ResetWidgetPos()
{
	int margin = 80;
	int w = m_pRecordPanel->width();
	int h = m_pRecordPanel->height();

	HWND hWnd = FindWindow(L"Shell_TrayWnd", NULL);
	RECT rc;
	::GetWindowRect(hWnd, &rc);
	int nTaskbarH = rc.bottom - rc.top;

    //QRect rcDsk = RecordHelper::GetInstance()->DesktopRect();
    QRect rcDsk = PrimaryScreenManager::GetInstance()->SecondScreen();
	int l = rcDsk.right() - margin - w + 1;
	int t = rcDsk.bottom() - nTaskbarH - h + 1;
    if (PrimaryScreenManager::GetInstance()->ScreenCount() > 1 )
    {
        l = rcDsk.left() + margin;
    }
	m_pRecordPanel->move(l, t);
}


void RecorderViewsMgr::ShowMainView(int page)
{
    m_pRecordPanel->SetVisible(false, true);
	m_pRecordMask->setVisible(false);
	m_pRecordWidget->setVisible(false);
	m_pRecordLoadingCtrl->setVisible(false);
	m_pActionWidget->HideWidget();

    m_pCameraPreview->setVisible(false);
    m_pGamePreview->setVisible(false);
    m_pAudioPreview->setVisible(false);

	m_pFSRecorder->setVisible(true);
	m_pFSRecorder->raise();
    m_pFSRecorder->activateWindow();
	m_pFSRecorderMainView->ShowPage((FSRecorderMainView::Page)page);
	SetCurrentViewPage(ViewPage_Main);
}

void RecorderViewsMgr::ShowRecordWidget()
{

    if (!RecorderCenter::GetInstance()->IsRecording())
    {
        m_pFSRecorder->setVisible(false);
        if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
        {
            m_pRecordMask->setVisible(true);
            m_pRecordMask->raise();
            m_pRecordWidget->setVisible(true);
            m_pRecordWidget->raise();
        }
        m_pRecordPanel->SetVisible(true);
        m_pRecordPanel->raise();
        m_pRecordLoadingCtrl->setVisible(false);
        m_pActionWidget->HideWidget();

    }
    else
    {
        m_pFSRecorder->setVisible(false);

        m_pRecordMask->setVisible(false);
        m_pRecordWidget->setVisible(false);
        
        m_pRecordPanel->SetVisible(true);
        m_pRecordPanel->raise();
        m_pRecordLoadingCtrl->setVisible(false);
        m_pActionWidget->HideWidget();
    }
    auto ReStorePreview = [&](PreviewBase *preview, bool enabed){
        if (enabed && !preview->IsManualClose())
            preview->setVisible(true);
    };

    QList<std::pair<PreviewBase*,bool>> previews;
    previews.push_back(std::make_pair(m_pCameraPreview, SettingsDataManager::GetInstance()->GetCameraData()->GetDeviceEnabled()));
    previews.push_back(std::make_pair(m_pGamePreview, SettingsDataManager::GetInstance()->GetGameData()->GetDeviceEnabled()));
    previews.push_back(std::make_pair(m_pAudioPreview, SettingsDataManager::GetInstance()->GetAudioData()->GetDeviceEnabled()));

    // 排序，让y()最小的preview显示，进行attach
    std::sort(previews.begin(), previews.end(), [&](std::pair<PreviewBase*, bool> &l, std::pair<PreviewBase*, bool> &r){
        return l.first->geometry().y() > r.first->geometry().y();
    });
    for (auto preview : previews)
    {
        ReStorePreview(preview.first, preview.second);
    }


    SetCurrentViewPage(ViewPage_Record);
    ::SetFocus((HWND)m_pRecordPanel->winId());
	//上传GA数据
	GAThread::getInstance()->insertEventItem(Change_Page, "RecorderWidget","");

}

void RecorderViewsMgr::ShowLoadingView()
{
	m_pFSRecorder->setVisible(false);
	m_pRecordMask->setVisible(false);
	m_pRecordWidget->setVisible(false);
    m_pRecordPanel->SetVisible(false, false);
    m_pRecordPanel->ShowLoadingView();
    // 每次创建新的loadingctrol，防止再次显示时位置闪烁
    if (m_pRecordLoadingCtrl)
        delete m_pRecordLoadingCtrl;
    m_pRecordLoadingCtrl = new RecordLoadingCtrl;
    connect(m_pRecordLoadingCtrl, &RecordLoadingCtrl::sigLoadingFinish, this, &RecorderViewsMgr::sigLoadingFinish);
    connect(m_pRecordLoadingCtrl, &RecordLoadingCtrl::sigLoadingAbort, this, &RecorderViewsMgr::sigLoadingAbort);

	m_pRecordLoadingCtrl->Start();
    if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
        m_pActionWidget->ShowWidget();

    m_pCameraPreview->setVisible(false, false);
    m_pGamePreview->setVisible(false, false);
    m_pAudioPreview->setVisible(false, false);

	SetCurrentViewPage(ViewPage_Loading);
}

void RecorderViewsMgr::ShowRecordingView()
{
	m_pFSRecorder->setVisible(false);
	m_pRecordMask->setVisible(false);
	m_pRecordWidget->setVisible(false);
    //m_pRecordPanel->SetVisible(false, false);
	m_pRecordLoadingCtrl->setVisible(false);

	if (SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled()) 
        m_pActionWidget->ShowWidget();

	//if (m_pRecordPanel->IsCameraPreviewEnabled()) m_pCameraPreview->setVisible(true);
	//if (m_pRecordPanel->IsGamePreviewEnabled()) m_pGamePreview->setVisible(true);
	//if (m_pRecordPanel->IsAudioPreviewEnabled()) m_pAudioPreview->setVisible(true);

	SetCurrentViewPage(ViewPage_Recording);
}

void RecorderViewsMgr::GotoEditor()
{
	HideAllView();
	SysTrayIcon::GetInstance()->SetVisible(false);
	Communicator::GetInstance()->GotoEditor();
	SetCurrentViewPage(ViewPage_None);
}

void RecorderViewsMgr::ShowPreviewWidgets(bool show)
{
	m_pCameraPreview->setVisible(show);
	m_pGamePreview->setVisible(show);
	m_pAudioPreview->setVisible(show);
}

void RecorderViewsMgr::HideAllView(bool force)
{
	m_pFSRecorder->setVisible(false);
	m_pRecordMask->setVisible(false);
	m_pRecordWidget->setVisible(false);
    m_pRecordPanel->SetVisible(false);
	m_pRecordLoadingCtrl->setVisible(false);
	m_pActionWidget->HideWidget();
    m_pCameraPreview->setVisible(false, force);
    m_pGamePreview->setVisible(false, force);
    m_pAudioPreview->setVisible(false, force);
}

void RecorderViewsMgr::SetFSRecorder(FSRecorder *pView)
{
	m_pFSRecorder = pView;
}

FSRecorder* RecorderViewsMgr::GetFSRecorder()
{
	return m_pFSRecorder;
}

void RecorderViewsMgr::SetFSRecorderMainView(FSRecorderMainView *pView)
{
	m_pFSRecorderMainView = pView;
}

FSRecorderMainView* RecorderViewsMgr::GetFSRecorderMainView()
{
	return m_pFSRecorderMainView;
}

void RecorderViewsMgr::SetRecordPanel(RecordPanel *pView)
{
	m_pRecordPanel = pView;
}

RecordPanel* RecorderViewsMgr::GetRecordPanel()
{
	return m_pRecordPanel;
}

void RecorderViewsMgr::SetRecordMask(RecordMask *pView)
{
	m_pRecordMask = pView;
}

RecordMask* RecorderViewsMgr::GetRecordMask()
{
	return m_pRecordMask;
}

void RecorderViewsMgr::SetRecordWidget(RecordWidget *pView)
{
	m_pRecordWidget = pView;
}

RecordWidget* RecorderViewsMgr::GetRecordWidget()
{
	return m_pRecordWidget;
}

void RecorderViewsMgr::SetRecordLoadingCtrl(RecordLoadingCtrl *pView)
{
	m_pRecordLoadingCtrl = pView;
}

RecordLoadingCtrl* RecorderViewsMgr::GetRecordLoadingCtrl()
{
	return m_pRecordLoadingCtrl;
}

void RecorderViewsMgr::SetActionWidget(ActionWidget *pView)
{
	m_pActionWidget = pView;
}

ActionWidget* RecorderViewsMgr::GetActionWidget()
{
	return m_pActionWidget;
}

void RecorderViewsMgr::SetCameraPreview(PreviewBase *pView)
{
	m_pCameraPreview = pView;
}

PreviewBase * RecorderViewsMgr::GetCameraPreview()
{
	return m_pCameraPreview;
}

void RecorderViewsMgr::SetGamePreview(PreviewBase *pView)
{
	m_pGamePreview = pView;
}

PreviewBase * RecorderViewsMgr::GetGamePreview()
{
	return m_pGamePreview;
}

void RecorderViewsMgr::SetAudioPreview(MicrophonePreview *pView)
{
	m_pAudioPreview = pView;
}

MicrophonePreview* RecorderViewsMgr::GetAudioPreview()
{
	return m_pAudioPreview;
}

void RecorderViewsMgr::Clear()
{
	SettingsDataManager::GetInstance()->Clear();
	((PreviewPage*)m_pFSRecorderMainView->GetPageView(FSRecorderMainView::Page_Preview))->Reset();
}

//////////////////////////////////////////////////////////////////////////
const char *RecordPanelMemory = "SharedMemory_RecordPanel";
static CurrentViewPageMgr *s_RecordPanelWindowShow = nullptr;
CurrentViewPageMgr* CurrentViewPageMgr::GetInstance()
{
    if (s_RecordPanelWindowShow == nullptr)
    {
        s_RecordPanelWindowShow = new CurrentViewPageMgr;

    }
    return s_RecordPanelWindowShow;
}

CurrentViewPageMgr::CurrentViewPageMgr()
{
    memory = new QSharedMemory(RecordPanelMemory);
    if (!memory->attach())
    {
        bool fff = memory->create(1);
        int i = 0;
    }
}

CurrentViewPageMgr::~CurrentViewPageMgr()
{
    if (memory->attach())
    {
        memory->detach();
    }
    delete memory;
}

void CurrentViewPageMgr::Release()
{
    if (s_RecordPanelWindowShow)
    {
        delete s_RecordPanelWindowShow;
        s_RecordPanelWindowShow = nullptr;
    }
}

static void sShowWindow(QString wndName)
{

}

void CurrentViewPageMgr::ShowNeedShowWindow()
{
    auto page = GetCurrentViewPage();
    if (page == RecorderViewsMgr::ViewPage_Main)
    {
        //已经开启一个recorder, 弹出已经开启的recorder
        QString wndName = GetFSRecorderTitleName();
        OneApplication::ShowWindow(wndName);
    }
    else if (page == RecorderViewsMgr::ViewPage_Record)
    {
        OneApplication::ShowWindow("FSRecordPanel");
    }
    else if (page == RecorderViewsMgr::ViewPage_None)
    {
        QString wndName = GetFSEditTitleName();
        OneApplication::ShowWindow(wndName);
    }
}

void CurrentViewPageMgr::SetCurrentViewPage(RecorderViewsMgr::ViewPage page)
{
    QSharedMemory memory(RecordPanelMemory);
    if (memory.attach())
    {
        auto byteArray = QString::number((int)page).toLatin1();
        memory.lock();
        char *to = static_cast<char*>(memory.data());
        memcpy(to, byteArray.data(), 1);
        memory.unlock();
    }

}

RecorderViewsMgr::ViewPage CurrentViewPageMgr::GetCurrentViewPage()
{
    QSharedMemory memory(RecordPanelMemory);
    int page = 0;
    if (memory.attach())
    {
        char to[2] = { 0 };
        memory.lock();
        char *from = static_cast<char*>(memory.data());
        memcpy(to, from, 1);
        page = atoi(to);
        memory.unlock();
    }

    return (RecorderViewsMgr::ViewPage)page;
}

bool CurrentViewPageMgr::TerminateFSEditor()
{
    BOOL ret = FALSE;
#ifdef _DEBUG
    QString fseditorString("fseditord.exe");
#else
    QString fseditorString("fseditor.exe");
#endif
    auto processID = ProcessManager::FindProcessID(fseditorString);
    if (processID)
    {
        auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
        ret = ::TerminateProcess((HANDLE)hProcess, 0);
    }

    return ret;
}

