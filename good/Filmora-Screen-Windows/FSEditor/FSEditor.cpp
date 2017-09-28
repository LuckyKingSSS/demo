#include "stdafx.h"
#include "FSEditor.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "inc_FSTimelineView/fstimelineview.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "inc_FSExport/IFSExport.h"
#include "MainControler.h"
#include "FSMainTitleBar.h"
#include "FSTitleBarControl.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "CommunicatorClient.h"
#include "inc_FSCommonlib/GlobalDefine.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_FSCommonlib/VolumeSlider.h"
#include "inc_MediaPlayerView/MediaPlayerControl.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_FSCommonlib/gathread.h"
#include "inc_CommonLib/StringOperation.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_FSCommonlib/MultiLangMgr.h"
#include "inc_FSCommonlib/FSTipWidget.h"
#include "SettingDataMgr.h"
#include "ProjectSettingDialog.h"
#include "inc_FSCommonlib/ProjectSavingMgr.h"

FSEditor* g_FSEditor = nullptr;
FSEditor::FSEditor(QWidget *parent)
	: FSBaseWidget(parent)
	, m_pMainControler(nullptr)
	, m_bCanExport(false)
{
	SetEditorMainView(this);
	ProjPreproccessorClient::GetInstance()->SetEditorView(this);
	CommunicatorClient::GetInstance()->SetEditorView(this);
	const QString appName = GetFSEditTitleName();
	QIcon icon = QIcon(":/FSEditor/Resources/light/filmora scrn.ico");
	setWindowIcon(icon);
	setWindowTitle(appName);
	g_FSEditor = this;
	m_pTitleBar = new FSMainTitleBar(this);
	m_pTitleBar->setObjectName("FSEditor_MainTitleBar");
	setObjectName("FSEditor");
	m_pTitleBar->SetSystemBorderVisible(false);
	m_pTitleBar->SetButtons(TopLevelWindowTitleBar::Minimized | TopLevelWindowTitleBar::Normal | TopLevelWindowTitleBar::Maximized | TopLevelWindowTitleBar::Close);
	m_pTitleBar->GetButton(TopLevelWindowTitleBar::Minimized)->setObjectName("FSEditor_TitleBar_MinimizedButton");
	((BaseStateButton*)m_pTitleBar->GetButton(TopLevelWindowTitleBar::Normal))->GetButton(TopLevelWindowTitleBar::Normal)->setObjectName("FSEditor_TitleBar_NormalButton");
	((BaseStateButton*)m_pTitleBar->GetButton(TopLevelWindowTitleBar::Normal))->GetButton(TopLevelWindowTitleBar::Maximized)->setObjectName("FSEditor_TitleBar_MaximizedButton");
	m_pTitleBar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("FSEditor_TitleBar_CloseButton");
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	m_pTitleBar->GetControl()->SetBorderColor(QColor(102, 102, 102));
	m_pTitleBar->GetControl()->SetBorderWidth(1);
	m_pBtnRecorder = new QPushButton(this);
	m_PBtnExport = new FSIconButton(this);
    m_pBtnRecorder->setFocusPolicy(Qt::NoFocus);
    m_PBtnExport->setFocusPolicy(Qt::NoFocus);
	m_pBtnRecorder->setObjectName("FSEditor_m_pBtnRecorder");
	m_PBtnExport->setObjectName("FSEditor_m_PBtnExport");
	m_PBtnExport->setEnabled(false);
	m_pUpperView = new UpperView(this);
	m_pUpperView->setMinimumHeight(200);
    m_pDownView = new DownView(m_pUpperView->GetMediaPlayerView(), this);
    m_pTimelineView = m_pDownView->GetFSTimelineView();

	m_pSplitter = new QSplitter(Qt::Vertical, this);
	m_pSplitter->setOpaqueResize(false);
	m_pSplitter->setHandleWidth(6);
	m_pSplitter->addWidget(m_pUpperView);
    m_pSplitter->addWidget(m_pDownView);
	m_pSplitter->setChildrenCollapsible(false);
	m_pSplitter->setObjectName("FSEditor_m_pSplitter");
  

    m_pMainControler = new MainControler(m_pUpperView->GetMatterView(), m_pTimelineView, m_pUpperView->GetMediaPlayerView(), m_pUpperView->GetPropertyView(), m_pDownView->GetFSMediaPlayerBar(), m_pTitleBar, this);
	m_pTitleBarControl = new FSTitleBarControl(m_pTitleBar, m_pUpperView->GetMatterView(), m_pUpperView->GetMediaPlayerView(), m_pTimelineView, this);
	m_pTitleBarControl->SetEditorView(this);

	connect(m_PBtnExport, &FSIconButton::clicked, this, &FSEditor::OnExport);
	connect(m_pTitleBarControl, &FSTitleBarControl::sigCreateVideo, this, &FSEditor::OnExport);
	connect(m_pBtnRecorder, &QPushButton::clicked, this, &FSEditor::OnGotoRecorder);
	connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigShowEditor, this, &FSEditor::OnShowEditor, Qt::QueuedConnection);
	connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigHideEditor, this, &FSEditor::OnHideEditor, Qt::QueuedConnection);
	connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigImportFiles, this, &FSEditor::OnImportFiles, Qt::QueuedConnection);
	connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigGetProjectName, this, &FSEditor::OnGetProjectName, Qt::QueuedConnection);
	connect(m_pTitleBarControl, &FSTitleBarControl::sigClose, this, [&]{close(); });
	connect(m_pTimelineView->GetTimelineViewManager(), &ITimelineViewManager::sigClear, this, [&]{
		m_bCanExport = false;
		m_PBtnExport->SetEnable(m_bCanExport);
	});
	connect(m_pTimelineView->GetPlayerEditManager(), &IPlayerEditorManager::signalTimelineUpdate, this, [&]{
		m_bCanExport = m_pTimelineView->GetPlayerEditManager()->HasTimelines();
		m_PBtnExport->SetEnable(m_bCanExport);
	});
	connect(m_pUpperView->GetMediaPlayerView()->GetMediaPlayerControl(), &IMediaPlayerControl::SeekFinishCallback, this, [&](){
		GAThread::getInstance()->increaseSeekCount();
	});

	RetranslateUi();
	setMinimumSize(1200, 670);
}

FSEditor::~FSEditor()
{
}

void FSEditor::ShowMaximized()
{
	m_pTitleBar->ShowMaximized();

	QTimer::singleShot(0, this, [&](){
		//m_pUpperView->SetBaseSize();
        m_pDownView->GetFSMediaPlayerBar()->ResizePlayer();
    });

}

UpperView * FSEditor::GetUpperView()
{
	return m_pUpperView;
}

FSTimelineView * FSEditor::GetTimelineView()
{
	return m_pTimelineView;
}

bool FSEditor::CanExport()
{
	return m_bCanExport;
}

void FSEditor::OnExport()
{
	QList<FSNLETimeline*> timelines;
	FSNLETimeline * pTemp = m_pTimelineView->GetTimelineViewManager()->GetRootTimeline()->Clone();
	if (pTemp->GetClipCount() > 0)
	{
		emit m_pTimelineView->GetPlayerEditManager()->sigPause();
		timelines << pTemp;
		IFSExport exportDlg(timelines, this);
		exportDlg.SetRecorderExportState(false);
        exportDlg.SetDefaultExportRsolution(SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->GetResolution());
        exportDlg.SetExportFileName(SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->GetExportName());
		connect(&exportDlg, &IFSExport::RegisterResaultSig, this, [&](bool bRegistered){
			if (bRegistered)
			{
				m_pTitleBarControl->RebuildRegisterMain(true);
			}
		});
		//上传GA数据
		GAThread::getInstance()->insertEventItem(Change_Page, "Export", "");
		exportDlg.DoModal(); 
	}
	pTemp->deleteLater();
}

void FSEditor::OnGotoRecorder()
{
	hide();
	m_pUpperView->GetMediaPlayerView()->Pause();
	CommunicatorClient::GetInstance()->GotoRecorder();
}

void FSEditor::OnShowEditor()
{
	show();
    activateWindow();
}

void FSEditor::OnHideEditor()
{
	hide();
}

void FSEditor::OnImportFiles(QStringList lst)
{
	m_pUpperView->GetMatterView()->AddRecordFils(lst);
}

void FSEditor::OnGetProjectName(QString projName)
{
	m_pTitleBar->SetProjectName(projName);
}

void FSEditor::RetranslateUi()
{
	setWindowTitle(GetFSEditTitleName());
	m_pBtnRecorder->setText(tr("+Recorder"));
	m_pBtnRecorder->setToolTip(tr("Go to recorder"));
	m_PBtnExport->setText(tr("Export"));
	m_PBtnExport->setToolTip(tr("Export to local file"));
	((BaseStateButton*)m_pTitleBar->GetButton(TopLevelWindowTitleBar::Normal))->GetButton(TopLevelWindowTitleBar::Normal)->setToolTip(tr("Restore Down"));
	((BaseStateButton*)m_pTitleBar->GetButton(TopLevelWindowTitleBar::Normal))->GetButton(TopLevelWindowTitleBar::Maximized)->setToolTip(tr("Maximize"));
	m_pTitleBar->GetButton(TopLevelWindowTitleBar::Minimized)->setToolTip(tr("Minimize"));
	m_pTitleBar->GetButton(TopLevelWindowTitleBar::Close)->setToolTip(tr("Close"));

    resizeEvent(nullptr);
}

void FSEditor::resizeEvent(QResizeEvent *e)
{
	static const int titleBarH = 34;
	static const int TopHeaderH = 42;
	static const int ButtonW = 74;
	static const int ButtonH = 20;
	auto rc = rect();
	auto w = width();
	m_pTitleBar->setGeometry(0, 0, width(), titleBarH);

	QRect rcClient = rect().adjusted(1, 1, -1, -1);

    int btnRecorderWidth = GetRetranslateUiTextLenght(m_pBtnRecorder->font(), m_pBtnRecorder->text(), 72) + 10;
    m_pBtnRecorder->setGeometry(12, 37, btnRecorderWidth, ButtonH);

    int btnExportNeedWidth = GetRetranslateUiTextLenght(m_PBtnExport->font(), m_PBtnExport->text(), ButtonW - 32, 0) + 32;
    m_PBtnExport->setGeometry(rcClient.width() - 27 - btnExportNeedWidth, 37, btnExportNeedWidth, ButtonH);

	m_pSplitter->setGeometry(rcClient.left(), titleBarH + TopHeaderH, rcClient.width(), rcClient.height() - titleBarH - TopHeaderH);

	int UpperViewH = height() * 0.6+4;
	m_pSplitter->setSizes(QList<int>() << UpperViewH << height() - UpperViewH);

}

void FSEditor::closeEvent(QCloseEvent *e)
{
	//上传GA数据
	m_pTimelineView->TransFromGaInfo();
	ProjPreproccessorClient::GetInstance()->SaveAtClose();
	e->ignore();
}

void FSEditor::showEvent(QShowEvent *e)
{
	CommunicatorClient::GetInstance()->EditorShowed();
}

//////////////////////////////////////////////////////////////////////////
UpperView::UpperView(QWidget *parent /*= 0*/)
	: FSBaseWidget(parent)
{
    setObjectName("UpperView");
    
    m_pMatterView = new FSMatterView(this);
    m_pMatterView->resize(300, 100);
	m_pMediaEditView = new IFSEditMediaPlayer(this);
	m_pPropertyView = new IFSPropertyView(this);

	m_pSplitter = new QSplitter(Qt::Horizontal, this);
	m_pPropertyView->setMaximumWidth(500);
	m_pPropertyView->setMinimumWidth(299);

	m_pSplitter->addWidget(m_pMediaEditView);
	m_pSplitter->addWidget(m_pPropertyView);
	//显示split handler 
	m_pSplitter->setHandleWidth(1);
	m_pSplitter->handle(1)->setObjectName("SplitHandle");
	m_pSplitter->setChildrenCollapsible(true);
	m_pSplitter->setCollapsible(1, false);
    m_pMediaEditView->GetEditHelperManarger()->EnableSnap(SettingDataMgr::GetInstance()->GetSnapToGuides());

    auto MediaEditorViewProperyInitFun = [&](){
        m_pMediaEditView->SetBaseRenderSize(SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->GetResolution());
        m_pMediaEditView->SetPlayBackResolution(SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->GetPlaybackResoultion());
        m_pMediaEditView->SetPlayerBackgroundColor(SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->GetBackgroundColor());

    };

    QTimer::singleShot(0, [&, MediaEditorViewProperyInitFun](){
        MediaEditorViewProperyInitFun();
    });
    connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigProjectFinishLoaded, this, [&, MediaEditorViewProperyInitFun](){
        MediaEditorViewProperyInitFun();
    });

    connect(m_pPropertyView, &IFSPropertyView::sigShowPropertyView, this, &UpperView::OnPropertyViewShowSlot);

    connect(SettingDataMgr::GetInstance(), &SettingDataMgr::sigSnapToGuides, this, [&](){
        m_pMediaEditView->GetEditHelperManarger()->EnableSnap(SettingDataMgr::GetInstance()->GetSnapToGuides());
    });

    connect(SettingDataMgr::GetInstance()->GetProejctSettingsMgr(), &ProjectSettingsMgr::sigResolutionChanged, this, [&](QSize size){
        m_pMediaEditView->SetBaseRenderSize(size);
    });
    connect(SettingDataMgr::GetInstance()->GetProejctSettingsMgr(), &ProjectSettingsMgr::sigPlaybackResoultionChagned, this, [&](float raito){
        m_pMediaEditView->SetPlayBackResolution(raito);
    });
    connect(SettingDataMgr::GetInstance()->GetProejctSettingsMgr(), &ProjectSettingsMgr::sigBackgroundColorChanged, this, [&](QColor color){
        m_pMediaEditView->SetPlayerBackgroundColor(color);
    });

    connect(m_pMediaEditView, &IFSEditMediaPlayer::sigProjectSettingTriggered, this, [&](){
        ProjectSettingDialog dlg(this);
        dlg.DoModal();
    });

    connect(m_pMediaEditView, &IFSEditMediaPlayer::sigResolutionChanged, this, [&](float resolutionScale){
        SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->SetPlaybackResoultion(resolutionScale);
    });


    connect(m_pMatterView->GetMatterViewManager(), &IFSMatterViewManager::sigShowMaximum, this, &UpperView::OnMatterViewShowSlot);
}

UpperView::~UpperView()
{

}

FSMatterView * UpperView::GetMatterView()
{
	return m_pMatterView;
}

IFSEditMediaPlayer * UpperView::GetMediaPlayerView()
{
	return m_pMediaEditView;
}

IFSPropertyView * UpperView::GetPropertyView()
{
	return m_pPropertyView;
}




//void UpperView::SetBaseSize()
//{
//    QSize size(width() - m_pPropertyView->width() - m_pMatterView->width() - m_pSplitter->handleWidth(), height());
//    QSize temp;
//    temp.setWidth(size.width());
//    temp.setHeight(size.height() - 37);
//    m_pMediaEditView->SetBaseSize(temp);
//}


int GetMatterViewW()
{
    return 300;
}

void UpperView::resizeEvent(QResizeEvent *e)
{
    //int MatterViewW = GetMatterViewW();
    int MatterViewW = m_pMatterView->width();
	int PropertyViewW = 299;
    m_pMatterView->setGeometry(0, 1, MatterViewW, height());
	m_pSplitter->setGeometry(m_pMatterView->geometry().right() + 1, 1, width() - MatterViewW, height());
	//m_pMeidaPlayerBar->setGeometry(0, m_pSplitter->geometry().bottom(), width(), barHeigth);

	m_pSplitter->setSizes(QList<int>() << width() - MatterViewW - PropertyViewW << PropertyViewW);
}



void UpperView::OnPropertyViewShowSlot(bool bshow)
{
    int MatterViewW = m_pMatterView->width();
	int PropertyViewW = bshow ? 299 : 24;
	if (PropertyViewW == 24)
	{
		m_pPropertyView->setMinimumWidth(PropertyViewW);
		m_pPropertyView->setMaximumWidth(PropertyViewW);
	}
	else{
		m_pPropertyView->setMinimumWidth(299);
		m_pPropertyView->setMaximumWidth(500);
	}
	m_pSplitter->setSizes(QList<int>() << width() - MatterViewW - PropertyViewW << PropertyViewW);
}



void UpperView::OnMatterViewShowSlot(bool bshow)
{
    int matterViewW = bshow ? GetMatterViewW() : 24;
    QRect matterRect = m_pMatterView->geometry();
    m_pMatterView->setGeometry(matterRect.left(), matterRect.top(), matterViewW, matterRect.height());
    m_pSplitter->setGeometry(m_pMatterView->geometry().right() + 1, 1, width() - matterViewW, height());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
FSMeidaPlayerBar::FSMeidaPlayerBar(IFSEditMediaPlayer *pPlayer, QWidget *pParent)
	:FSBaseWidget(pParent)
	, m_pMediaPlayer(pPlayer)
	, m_CurrentVoluem(0)
	, m_bSeek(false)
{
	CreateUI();
	RetranslateUi();
	InitTimeText();
	ConnectSignals();
}

FSMeidaPlayerBar::~FSMeidaPlayerBar()
{

}

void FSMeidaPlayerBar::ToolBarPlayButtonStateChanged(int id)
{
	qobject_cast<FSStateBaseButton*>(m_ToolBarBtnMap[ToolBar_Play])->SetCurrentButtonId(id);
}

void FSMeidaPlayerBar::OnPlayed()
{
	ToolBarPlayButtonStateChanged(ToolBar_Pause);
    m_pComBoxScale->EnableWheelEvent(false);
}

void FSMeidaPlayerBar::OnPaused()
{
	ToolBarPlayButtonStateChanged(ToolBar_Play);
    m_pComBoxScale->EnableWheelEvent(true);
}

void FSMeidaPlayerBar::OnStoped()
{
	ToolBarPlayButtonStateChanged(ToolBar_Play);
	m_pProgressSlider->setValue(0);
	UpdateTimeText(m_pMediaPlayer->GetMediaPlayerControl()->Duration(), 0, m_pMediaPlayer->GetMediaPlayerControl()->FPS());
	m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(false);
    m_pComBoxScale->EnableWheelEvent(true);
}

void FSMeidaPlayerBar::OnLoaded()
{
	if (m_pMediaPlayer->HaveSource())
		m_CurrentVoluem = m_pMediaPlayer->GetMediaPlayerControl()->GetVolume();
	m_pProgressSlider->setDuration(m_pMediaPlayer->GetMediaPlayerControl()->Duration());
	m_pSliderEditVolume->SetValue(m_CurrentVoluem);
	UpdateTimeText(m_pMediaPlayer->GetMediaPlayerControl()->Duration(), m_pMediaPlayer->CurrentFrame(), m_pMediaPlayer->GetMediaPlayerControl()->FPS());
	EnableWidgets(m_pMediaPlayer->GetMediaPlayerControl()->GetTimeline()->GetClipCount() > 0);
	OnVolumeValueChanged(m_CurrentVoluem);
}

void FSMeidaPlayerBar::OnPlaying(int frames)
{
	m_pProgressSlider->setValue(frames);
	UpdateTimeText(m_pMediaPlayer->GetMediaPlayerControl()->Duration(), frames, m_pMediaPlayer->GetMediaPlayerControl()->FPS());
    m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(frames != 0); 
    if (m_pMediaPlayer->IsPlaying())
        OnPlayed();
    else
        OnPaused();
}

void FSMeidaPlayerBar::OnSeeked(int frames)
{
	m_pProgressSlider->setValue(frames);
	UpdateTimeText(m_pMediaPlayer->GetMediaPlayerControl()->Duration(), frames, m_pMediaPlayer->GetMediaPlayerControl()->FPS());
	m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(frames != 0);
}

void FSMeidaPlayerBar::EnableWidgets(bool bEnable)
{
	QList<BaseButton* > list = m_ToolBarBtnMap.values();
	m_pProgressSlider->setEnabled(bEnable);
	m_pSliderEditVolume->setEnabled(bEnable);
	m_pLabelPreViewDuration->setEnabled(bEnable);
	m_pLabelEditDuration->setEnabled(bEnable);
    m_pButtonViewCut->setEnabled(bEnable);
    m_pButtonViewScale->setEnabled(bEnable);
	for (auto item : list)
	{
		item->SetEnabled(bEnable);
	}
	m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(m_pMediaPlayer->CurrentFrame() != 0);
}

void FSMeidaPlayerBar::UpdateTimeText(int duration, int currentFrame, float fps)
{
	QString timeText, timeText1;
	timeText = gFrame2StringSecond(qMin(currentFrame, duration), fps);
	timeText1 = gFrame2StringSecond(duration, fps);
	m_pLabelPreViewDuration->setText(timeText.mid(0, 8));
	m_pLabelEditDuration->setText(timeText1.mid(0, 8));
}

void FSMeidaPlayerBar::OnDurationChangedSlot(int duration)
{
	SetDuration(m_pMediaPlayer->GetMediaPlayerControl()->Duration());
	EnableWidgets(m_pMediaPlayer->GetMediaPlayerControl()->GetTimeline()->GetClipCount() > 0);
    if (m_pMediaPlayer->GetMediaPlayerControl()->GetTimeline()->GetClipCount() == 0)
    {
        m_pProgressSlider->setValue(0);
    }       
}

void FSMeidaPlayerBar::SetDuration(int duration)
{
	m_pProgressSlider->setDuration(duration);
	if (duration == 0)
	{
		m_pProgressSlider->setValue(0);
		m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(false);
	}
	UpdateTimeText(duration, m_pMediaPlayer->CurrentFrame(), m_pMediaPlayer->GetMediaPlayerControl()->FPS());
}

void FSMeidaPlayerBar::OnAddTimeLine()
{
	EnableWidgets(m_pMediaPlayer->GetMediaPlayerControl()->GetTimeline()->GetClipCount() > 0);
}


void FSMeidaPlayerBar::RetranslateUi()
{
	static_cast<FSBaseButton*>(m_ToolBarBtnMap[ToolBar_Stop])->setToolTip(tr("Stop(Ctrl+/)"));
	static_cast<FSBaseButton*>(m_ToolBarBtnMap[ToolBar_Forward])->setToolTip(tr("Forward(Left)"));
	static_cast<FSBaseButton*>(m_ToolBarBtnMap[ToolBar_Backward])->setToolTip(tr("Backward(Right)"));
	static_cast<FSBaseButton*>(m_ToolBarBtnMap[ToolBar_Volume])->setToolTip(tr("Volume"));
    m_pButtonViewScale->setToolTip(tr("Scale"));
    m_pButtonViewCut->setToolTip(tr("Crop"));
    static_cast<FSBaseButton*>(m_ToolBarBtnMap[ToolBar_SnapShort])->setToolTip(tr("SnapShot"));
	FSStateBaseButton *pStateBtn1 = static_cast<FSStateBaseButton*>(m_ToolBarBtnMap[ToolBar_Play]);
	static_cast<FSBaseButton*>(pStateBtn1->GetButton(ToolBar_Play))->setToolTip(tr("Play(Space)"));
	static_cast<FSBaseButton*>(pStateBtn1->GetButton(ToolBar_Pause))->setToolTip(tr("Pause(Space)"));
	static_cast<FSBaseButton*>(m_ToolBarBtnMap[ToolBar_FullScreen])->setToolTip(tr("FullScreen"));
	m_pComBoxScale->setToolTip(tr("Zoom in/out preview window"));
    m_pComBoxResolution->setToolTip(tr("Playback resolution"));
        
    QStringList list;
    list << "10%" << "25%" << "35%" << "50%" << tr("Fit") << "60%" << "75%" << "85%" << "100%" << "125%" << "150%" << "175%" << "200%";
    m_pComBoxScale->blockSignals(true);
    m_pComBoxScale->clear();
    m_pComBoxScale->addItems(list);
    m_pComBoxScale->blockSignals(false);
    list.clear();
    list << tr("Full") << "1/2" << "1/4" << "1/8" << "1/16";
    m_pComBoxResolution->blockSignals(true);
    m_pComBoxResolution->clear();
    m_pComBoxResolution->addItems(list);
    m_pComBoxResolution->blockSignals(false);

}

void FSMeidaPlayerBar::CreateUI()
{
	CreateBtn(ToolBar_Stop, "FSMeidaPlayerBar_Stop", this);
	CreateBtn(ToolBar_Backward, "FSMeidaPlayerBar_Backward", this);
	CreateBtn(ToolBar_Forward, "FSMeidaPlayerBar_Forward", this);
	CreateBtn(ToolBar_Volume, "FSMeidaPlayerBar_Volume", this);
    CreateBtn(ToolBar_SnapShort, "FSMeidaPlayerBar_SnapShort", this);
	CreateStateBtn(ToolBar_Play, "FSMeidaPlayerBar_Play", ToolBar_Pause, "FSMeidaPlayerBar_Pause");
	CreateBtn(ToolBar_FullScreen, "FSMeidaPlayerBar_FullScreen", this);
	m_pProgressSlider = new ToolBarSlider(this);
	m_pSliderEditVolume = new AudioVolumeSlider(this);
	m_pLabelPreViewDuration = new FSLabel(this);
	m_pLabelEditDuration = new FSLabel(this);
    m_pButtonViewScale = new FSButton(this);
    m_pButtonViewCut = new FSButton(this);
	m_pComBoxScale = new FSComboBox(this);
    m_pComBoxResolution = new FSComboBox(this);
    m_pComBoxScale->setFocusPolicy(Qt::NoFocus);
    m_pComBoxResolution->setFocusPolicy(Qt::NoFocus);
	setFixedHeight(37);
	m_pProgressSlider->setMinimumWidth(254);
	m_pProgressSlider->SetChunkHeight(10);
	m_pSliderEditVolume->SetRange(-100, 100);
	m_pLabelPreViewDuration->setFixedSize(58, 17);
	m_pLabelEditDuration->setFixedSize(58, 17);
	m_pComBoxScale->setFixedSize(65, 37);
    m_pComBoxResolution->setFixedSize(65, 37);
    m_pButtonViewCut->setCheckable(true);
    m_pButtonViewScale->setCheckable(true);
	QHBoxLayout *pHL = new QHBoxLayout(this);

	pHL->addStretch(2);
	pHL->addWidget(m_ToolBarBtnMap.value(ToolBar_Backward));
	pHL->addWidget(m_ToolBarBtnMap.value(ToolBar_Forward));
	pHL->addWidget(m_ToolBarBtnMap.value(ToolBar_Play));
	pHL->addWidget(m_ToolBarBtnMap.value(ToolBar_Stop));
    pHL->addWidget(m_ToolBarBtnMap.value(ToolBar_Volume));
    pHL->addWidget(m_ToolBarBtnMap.value(ToolBar_SnapShort));
	pHL->addSpacing(9);
	pHL->addWidget(m_pLabelPreViewDuration);
	pHL->addSpacing(9);
	pHL->addWidget(m_pProgressSlider, 5);
	pHL->addSpacing(9);
	pHL->addWidget(m_pLabelEditDuration);
	pHL->addSpacing(9);	
    pHL->addWidget(m_ToolBarBtnMap.value(ToolBar_FullScreen));
    pHL->addWidget(m_pComBoxScale);
    pHL->addWidget(m_pComBoxResolution);
    pHL->addWidget(m_pButtonViewScale);
	pHL->addWidget(m_pButtonViewCut);		
	pHL->addStretch(2);
	pHL->setMargin(0);
	pHL->setSpacing(0);

    QButtonGroup *pButtonGroup = new QButtonGroup(this);
    pButtonGroup->addButton(m_pButtonViewScale,ToolBar_ViewScale);
    pButtonGroup->addButton(m_pButtonViewCut, ToolBar_ViewCut);
    m_pButtonViewScale->setChecked(true);
    m_pButtonViewScale->setFixedSize(37, 37);
    m_pButtonViewCut->setFixedSize(37, 37);
	m_pLabelPreViewDuration->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_pLabelEditDuration->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	setObjectName("FSMeidaPlayerBar");
	m_pLabelPreViewDuration->setObjectName("FSMeidaPlayerBar_m_pLabelPreViewDuration");
	m_pLabelEditDuration->setObjectName("FSMeidaPlayerBar_m_pLabelEditDuration");
	m_pComBoxScale->setObjectName("FSMeidaPlayerBar_m_pComBoxScale");
    m_pComBoxResolution->setObjectName("FSMeidaPlayerBar_m_pComBoxScale");
	m_pProgressSlider->setObjectName("FSMeidaPlayerBar_m_pProgressSlider");
    m_pButtonViewCut->setObjectName("FSMeidaPlayerBar_View");
    m_pButtonViewScale->setObjectName("FSMeidaPlayerBar_Scale");
	
	connect(m_pComBoxScale, &QComboBox::currentTextChanged, this, [&](QString text){
		QString temp = text.left(text.length() - 1);
		if (temp.toInt() >= 10)
		{
            m_pMediaPlayer->ResetMartPoint();
			m_pMediaPlayer->SetScale(false,1.0*temp.toInt() / 100);
        }
        else
        {
            m_pMediaPlayer->SetScale(true,1.0);
        }
	});

	connect(m_pMediaPlayer, &IFSEditMediaPlayer::sigChangeScale, this, [&](bool bZoom){
        int index = qBound(0, m_pComBoxScale->currentIndex() + (bZoom ? -1 : 1), m_pComBoxScale->count() - 1);
        m_pComBoxScale->blockSignals(true);
        m_pComBoxScale->setCurrentIndex(index);
        QString text = m_pComBoxScale->currentText();
        QString temp = text.left(text.length() - 1);
        if (temp.toInt() >= 10)
        {
            m_pMediaPlayer->SetScale(false, 1.0*temp.toInt() / 100);
        }
        else
        {
            int index = qBound(0, m_pComBoxScale->currentIndex() + (bZoom ? -1 : 1), m_pComBoxScale->count() - 1);
            m_pComBoxScale->setCurrentIndex(index);
            text = m_pComBoxScale->currentText();
            temp = text.left(text.length() - 1);
            m_pMediaPlayer->SetScale(false, 1.0*temp.toInt() / 100);
        }
        m_pComBoxScale->blockSignals(false);
	});

    connect(m_pMediaPlayer, &IFSEditMediaPlayer::sigMenuChangeScale, this, [&](QString scale){
        m_pComBoxScale->blockSignals(true);
        m_pComBoxScale->setCurrentText(scale);
        QString text = m_pComBoxScale->currentText();
        QString temp = text.left(text.length() - 1);
        if (temp.toInt() >= 10)
        {
            m_pMediaPlayer->SetScale(false, 1.0*temp.toInt() / 100);
        }
        else
        {
            m_pMediaPlayer->SetScale(true, 1.0);
        }
        m_pComBoxScale->blockSignals(false);
    });

    connect(m_pComBoxResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(slotResolutionChanged(int)));

    connect(m_pMediaPlayer, &IFSEditMediaPlayer::sigResolutionChanged, this, [&](float resouion){
        m_pComBoxResolution->blockSignals(true);
        int index = qRound(qLn(qRound(1.0 / resouion)) / qLn(2));
        m_pComBoxResolution->setCurrentIndex(index);
        m_pComBoxResolution->blockSignals(false);
    });

    connect(pButtonGroup, SIGNAL(buttonClicked(int)), this,SLOT(slotButtonGroupClicked(int)));
	m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(false);
}

void FSMeidaPlayerBar::ConnectSignals()
{
	connect(m_pProgressSlider, &ToolBarSlider::valueChanged, [this](double value){
		m_pMediaPlayer->OnMediaPlayerBarChange(value);
		UpdateTimeText(m_pMediaPlayer->GetMediaPlayerControl()->Duration(), value, m_pMediaPlayer->GetMediaPlayerControl()->FPS());
	});
	connect(m_pProgressSlider, &ToolBarSlider::startValueChanged, [this](){
		m_bSeek = true;
		m_pMediaPlayer->OnMediaPlayerBarStartChange();
	});
	connect(m_pProgressSlider, &ToolBarSlider::endValueChanged, [this](){
		m_pMediaPlayer->OnMediaPlayerBarEndChange();
		m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(((int)m_pProgressSlider->value()) != 0);
		QTimer::singleShot(1000, [this](){m_bSeek = false; });
	});

	connect(m_pMediaPlayer->GetMediaPlayerControl(), &IMediaPlayerControl::PlayingCallback, [this](int frames){
		OnPlaying(frames);
	});
	connect(m_pMediaPlayer->GetMediaPlayerControl(), &IMediaPlayerControl::PlayStopCallback, [this](){
		OnStoped();
        
	});
	connect(m_pMediaPlayer->GetMediaPlayerControl(), &IMediaPlayerControl::PlayFinishCallback, this, [&](){
		//OnStoped();
        ToolBarPlayButtonStateChanged(ToolBar_Play);
        m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(false);
        m_pComBoxScale->EnableWheelEvent(true);
	});
	connect(m_pMediaPlayer->GetMediaPlayerControl(), &IMediaPlayerControl::PlayStartCallback, this, [&](){
		OnPlayed();
        
	});
	connect(m_pMediaPlayer->GetMediaPlayerControl(), &IMediaPlayerControl::PlayLoadFinishCallback, this, [&]()
	{
		OnLoaded();
	});
	connect(m_pMediaPlayer->GetMediaPlayerControl(), &IMediaPlayerControl::PlayPauseCallback, this, [&]()
	{
		OnPaused();
	});
	connect(m_pMediaPlayer->GetMediaPlayerControl(), &IMediaPlayerControl::SeekFinishCallback, this, [&](int frame)
	{
		if (!m_bSeek)
			OnSeeked(frame);
	});

    connect(m_pMediaPlayer, &IFSEditMediaPlayer::sigBackForwedShortCutTriged, this, [&](int frame)
    {
        m_pProgressSlider->setValue(frame);
        UpdateTimeText(m_pMediaPlayer->GetMediaPlayerControl()->Duration(), frame, m_pMediaPlayer->GetMediaPlayerControl()->FPS());
    });

	connect(m_pSliderEditVolume, &AudioVolumeSlider::ValueChanged, this, [&](int iValue){
		OnVolumeValueChanged(iValue - 100);
	});

    connect(m_pMediaPlayer, &IFSEditMediaPlayer::sigEditModeChanged, this, [&](bool bScaleMode){
        if (bScaleMode)
            m_pButtonViewScale->setChecked(true);
        else
            m_pButtonViewCut->setChecked(true);
    });

}

void FSMeidaPlayerBar::CreateBtn(int Id, const QString &objName, QWidget *parent)
{
	auto pBtn = new FSBaseButton(Id, objName, parent);
	pBtn->setVisible(true);
	pBtn->setFixedSize(37, 37);
	connect(pBtn, &FSBaseButton::Clicked, this, &FSMeidaPlayerBar::BtnClicked);
	m_ToolBarBtnMap.insert(Id, pBtn);
}

void FSMeidaPlayerBar::CreateStateBtn(int firstId, const QString &objName1, int secId, const QString &objName2)
{
	auto pStateBtn = new FSStateBaseButton(firstId, "", this);
	pStateBtn->setVisible(true);
	pStateBtn->setFixedSize(37, 37);
	auto pBtn1 = new FSBaseButton(firstId, objName1, pStateBtn);
	auto pBtn2 = new FSBaseButton(secId, objName2, pStateBtn);
	pStateBtn->AddButton(pBtn1);
	pStateBtn->AddButton(pBtn2);
	connect(pStateBtn, &BaseStateButton::Clicked, this, &FSMeidaPlayerBar::BtnClicked);
	m_ToolBarBtnMap.insert(firstId, pStateBtn);
}

void FSMeidaPlayerBar::OnVolumeValueChanged(int value)
{
	if (m_pMediaPlayer->GetMediaPlayerControl()->HaveSource())
	{
		m_pMediaPlayer->GetMediaPlayerControl()->SetVolume(value);
		m_CurrentVoluem = value;
	}

}

void FSMeidaPlayerBar::OnVolumeClicked(QPoint pos)
{
	//加上100的原因是 m_pSliderEditVolume的取值范围是 0 - 200 跟volume对应值相差100
	m_pSliderEditVolume->SetValue(m_pMediaPlayer->GetMediaPlayerControl()->GetVolume() + 100);
	m_pSliderEditVolume->DoModal(pos);
}

void FSMeidaPlayerBar::slotButtonGroupClicked(int id)
{
    switch (id)
    {
    case FSMeidaPlayerBar::ToolBar_ViewScale:
        if (m_pMediaPlayer->IsPlaying())
            m_pMediaPlayer->Pause();
        m_pMediaPlayer->OnMediaPlayerBarCropView(true);
        break;
    case FSMeidaPlayerBar::ToolBar_ViewCut:
        if (m_pMediaPlayer->IsPlaying())
            m_pMediaPlayer->Pause();
        m_pMediaPlayer->OnMediaPlayerBarCropView(false);
        //上传GA数据
        GAThread::getInstance()->insertEventItem(Clip_Edit, "Crop", "");
    }
}

void FSMeidaPlayerBar::slotResolutionChanged(int index)
{
    m_pMediaPlayer->SetPlayBackResolution(1.0 / qPow(2, index));
 
    SettingDataMgr::GetInstance()->GetProejctSettingsMgr()->SetPlaybackResoultion(1.0 / qPow(2, index));
}

void FSMeidaPlayerBar::ResizePlayer()
{
	m_pComBoxScale->setCurrentIndex(5);
	QTimer::singleShot(0, this, [&]{m_pComBoxScale->setCurrentIndex(4); });
}

void FSMeidaPlayerBar::BtnClicked(int id, QPoint leftTop)
{
	switch (id)
	{
	case FSMeidaPlayerBar::ToolBar_Play:

		if (m_pMediaPlayer->HaveSource())
		{
			m_pMediaPlayer->Play();
			m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(true);
		}
		else
		{
			ToolBarPlayButtonStateChanged(ToolBar_Play);
		}

		break;
	case FSMeidaPlayerBar::ToolBar_Stop:
		m_pMediaPlayer->Stop();
		break;
	case FSMeidaPlayerBar::ToolBar_Pause:
		m_pMediaPlayer->Pause();
		break;
	case FSMeidaPlayerBar::ToolBar_Forward:
	{
		m_pMediaPlayer->Forward();
		m_pProgressSlider->setValue(m_pMediaPlayer->CurrentFrame());
		m_pMediaPlayer->GetTimelineViewManager()->SetCurrentFrame(m_pMediaPlayer->CurrentFrame());
		UpdateTimeText(m_pMediaPlayer->Duration(), m_pMediaPlayer->CurrentFrame(), m_pMediaPlayer->GetMediaPlayerControl()->FPS());
		m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(m_pMediaPlayer->CurrentFrame() > 0);
	}
		break;
	case FSMeidaPlayerBar::ToolBar_Backward:
	{
		m_pMediaPlayer->Backward();
		m_pProgressSlider->setValue(m_pMediaPlayer->CurrentFrame());
		m_pMediaPlayer->GetTimelineViewManager()->SetCurrentFrame(m_pMediaPlayer->CurrentFrame());
		UpdateTimeText(m_pMediaPlayer->Duration(), m_pMediaPlayer->CurrentFrame(), m_pMediaPlayer->GetMediaPlayerControl()->FPS());
		m_ToolBarBtnMap[ToolBar_Backward]->SetEnabled(m_pMediaPlayer->CurrentFrame() > 0);
	}
		break;
    case FSMeidaPlayerBar::ToolBar_SnapShort:
    {
        QPoint pt = mapToGlobal(m_ToolBarBtnMap[ToolBar_SnapShort]->geometry().topLeft()) + QPoint(-20,-40);
        QString formatFile = QDir::toNativeSeparators(PathOperation::GetDocumentPath());
        QImage img;
        m_pMediaPlayer->GetSnapShortImg(img);  
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(formatFile + QDir::separator());
        formatFile.append(QDir::separator() + tr("snapshot.png"));
        FSTipWidget::Instance().SetMesseage(formatFile, &pt);
        img.save(formatFile);        
        QTimer::singleShot(0, [&](){m_ToolBarBtnMap.value(ToolBar_SnapShort)->repaint();});
    }
        break;
    case FSMeidaPlayerBar::ToolBar_Volume:
    {
        OnVolumeClicked(leftTop);
        QTimer::singleShot(0, [&](){m_ToolBarBtnMap.value(ToolBar_Volume)->repaint(); });
    }		
		break;
	case FSMeidaPlayerBar::ToolBar_FullScreen:
		m_pMediaPlayer->OnMediaPlayerBarFullScreen();
		break;
	default:
		Q_ASSERT(false);
		break;
	}
}


void FSMeidaPlayerBar::InitTimeText()
{
	m_pLabelPreViewDuration->setText("00:00:00");
	m_pLabelEditDuration->setText("00:00:00");
}


//////////////////////////////////////////////////////////////////////////
DownView::DownView(IFSEditMediaPlayer *pPlayer, QWidget *parent)
:FSBaseWidget(parent)
{
    m_pMeidaPlayerBar = new FSMeidaPlayerBar(pPlayer, this);
    m_pTimelineView = new FSTimelineView(this);
}

FSTimelineView * DownView::GetFSTimelineView()
{
    return m_pTimelineView;
}

FSMeidaPlayerBar * DownView::GetFSMediaPlayerBar()
{
    return m_pMeidaPlayerBar;
}

void DownView::resizeEvent(QResizeEvent *event)
{
    m_pMeidaPlayerBar->setGeometry(0, 0, width(), 40);
    int top = m_pMeidaPlayerBar->geometry().bottom() + 1;
    m_pTimelineView->setGeometry(0, top, width(), height() - top);
}
