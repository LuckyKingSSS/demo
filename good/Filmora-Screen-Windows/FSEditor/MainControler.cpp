#include "MainControler.h"
#include "inc_FSMatterView/FSMatterView.h"
#include "inc_FSTimelineView/fstimelineview.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "INLEMouseEffectManager.h"
#include "INLEKeyboardStreamManager.h"
#include "CommunicatorClient.h"
#include "FSEditor.h"
#include <QDesktopWidget>
#include "FSMainTitleBar.h"
#include "inc_Commonlib/DesktopMgr.h"
#include "inc_FSCommonlib/ExternalAPI.h"

MainControler::MainControler(FSMatterView *matterview,
	FSTimelineView *timelineview, 
	IFSEditMediaPlayer *mediaplayerview, 
	IFSPropertyView *propertyview, 
    FSMeidaPlayerBar *pMediaPayerBar,
	FSMainTitleBar *pMainTitleBar,
	QObject *parent)
	: QObject(parent)
{
	m_pMatterView = matterview;
	m_pTimelineView = timelineview;
	m_pEditMediaPlayerView = mediaplayerview;
	m_pPropertyView = propertyview;
    m_pMediaPayerBar = pMediaPayerBar;
	m_pMainTitleBar = pMainTitleBar;
	m_pTimelineViewManager = m_pTimelineView->GetTimelineViewManager();
	m_pPlayerEditorManager = m_pTimelineView->GetPlayerEditManager();

	connect(m_pMatterView->GetMatterViewManager(), &IFSMatterViewManager::sigContentChanged, this, [&](bool bChanged){
		CommunicatorClient::GetInstance()->SetContentChanged(bChanged);
	});
	connect(m_pTimelineViewManager, &ITimelineViewManager::sigContentChanged, this, [&](bool bChanged){
		CommunicatorClient::GetInstance()->SetContentChanged(bChanged);
	});
	connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigMatterStringLoaded, m_pMatterView->GetMatterViewManager(), &IFSMatterViewManager::LoadProjectString);
	connect(CommunicatorClient::GetInstance(), &CommunicatorClient::sigTimelineStringLoaded, m_pTimelineViewManager, &ITimelineViewManager::LoadProjectString);
	ConnectSignals();
}

MainControler::~MainControler()
{

}

void MainControler::ConnectSignals()
{
    m_pEditMediaPlayerView->SetTimelineViewManager(m_pTimelineViewManager);
    m_pEditMediaPlayerView->SetPlayerEditManager(m_pPlayerEditorManager);
    m_pEditMediaPlayerView->SetTitleBarMannager(m_pTimelineView->GetTitleBarManager());
    m_pPropertyView->SetEditorHelperManager(m_pEditMediaPlayerView->GetEditHelperManarger());
    m_pPropertyView->SetPlayerEditManager(m_pPlayerEditorManager);

	connect(m_pMatterView->GetMatterViewManager(), &IFSMatterViewManager::sigLoad, this, [&](FSNLETimeline *timeline){
		MVPlayer player(tr("Filmora Scrn"), "Filmora Scrn", GetEditorMainView());
		player.SetSrcTimeline(timeline);
		player.DoModal();
	});
	connect(m_pMatterView->GetMatterViewManager(), &IFSMatterViewManager::sigAdd, this, [&](FSNLETimeline *timeline){
		m_pPlayerEditorManager->AddTimelines(QList<FSNLETimeline*>() << timeline);
	});

	connect(m_pMatterView->GetMatterViewManager(), &IFSMatterViewManager::sigMouseEffectDraging, this, [&](bool bDraging){
		m_pEditMediaPlayerView->GetEditHelperManarger()->SetMouseEffectDraging(bDraging);
		m_pTimelineViewManager->SetMouseEffectDraging(bDraging);
	});

	connect(m_pEditMediaPlayerView->GetEditHelperManarger(), &IFSEditHelperManager::TextChanged, this, [&](QString text){
		m_pTimelineView->GetTimelineViewManager()->UpdateView();
	});
	connect(m_pEditMediaPlayerView, &IFSEditMediaPlayer::sigEnableEditShotCut, this, [&](bool bEnable){
		m_pMainTitleBar->EnableEditShortCut(bEnable);
		m_pTimelineViewManager->EnableShortcuts(bEnable);
	});
	connect(m_pTimelineViewManager, &ITimelineViewManager::sigStartSeek, m_pPlayerEditorManager, &IPlayerEditorManager::sigPause);
    connect(m_pPlayerEditorManager, &IPlayerEditorManager::sigTimelineAdded, this, [&](FSNLETimeline *pTimeline){
        m_pMediaPayerBar->OnAddTimeLine();
    });
    connect(m_pTimelineViewManager, &ITimelineViewManager::sigDurationChanged, this, [&](int d){
        m_pMediaPayerBar->OnDurationChangedSlot(d);
    });
	connect(m_pMatterView->GetMatterViewManager(), &IFSMatterViewManager::sigDisplayNameChanged, this, [&](QString ancestralGuid, QString displayname){
		m_pPlayerEditorManager->SetDisplayName(ancestralGuid, displayname);
	});
	connect(m_pPropertyView, &IFSPropertyView::sigSpeedChanged, this, [&](FSNLETimeline *timeline,float scale){
		m_pTimelineViewManager->SetSpeedScale(timeline,scale); 
	});
	connect(m_pPropertyView, &IFSPropertyView::sigDenoise, this, [&](FSNLETimeline *timeline){
		m_pTimelineViewManager->DenoiseLevelChanged(timeline);
	});
}


//////////////////////////////////////////////////////////////////////////
MVPlayer::MVPlayer(const QString &trtitle, const char *title, QWidget *parent)
	:FSBaseDialog(trtitle,title,parent)
{
    m_pIFSMediaPlayer = new IFSMediaPlayer(this);
    m_pIFSMediaPlayer->SetChunkHeight(16);
	m_pTitleBar = new TopLevelWindowTitleBar(this);
	m_pTitleBar->SetSystemBorderVisible(false);
	m_pTitleBar->SetButtons(TopLevelWindowTitleBar::Close);
	m_pTitleBar->GetButton(TopLevelWindowTitleBar::Close)->setObjectName("FSEditor_TitleBar_CloseButton");
    m_pTitleBar->setObjectName("FSEditor_SettingDialog_TitleBar");
    m_pTitleBar->GetControl()->SetBorderColor(QColor(102,102,102));
    m_pTitleBar->GetControl()->SetBorderWidth(1);
	resize(800, 624);
	setMinimumSize(600, 400);
	auto sc = new QShortcut(this);
	sc->setKey(QKeySequence("Space"));
	sc->setContext(Qt::ApplicationShortcut);
	connect(sc, &QShortcut::activated, this, [&]{
		if (m_pIFSMediaPlayer->IsPlaying())
		{
			m_pIFSMediaPlayer->Pause();
		}
		else
		{
			m_pIFSMediaPlayer->Play();
		}

	});
}

void MVPlayer::SetSrcTimeline(FSNLETimeline *timeline)
{
	Q_ASSERT(timeline);
	m_pTimeline = timeline;
    QSizeF srcSize = m_pTimeline->GetSourceSize();
    QSize dstSize = m_pTimeline->GetRatioSize(QSize(srcSize.width(), srcSize.height()), size());
    dstSize.setHeight(dstSize.height() + 34 + 37); // ¼ÓÉÏtitlebarºÍtoolbar
    resize(dstSize);
}

void MVPlayer::closeEvent(QCloseEvent *e)
{
	m_pIFSMediaPlayer->close();
	e->accept();
}

void MVPlayer::showEvent(QShowEvent *e)
{
	int l = (DesktopMgr::GetInstance()->MainScreen()->width() - width()) / 2;
	int t = (DesktopMgr::GetInstance()->MainScreen()->height() - height()) / 2;
	move(l, t);
	m_pIFSMediaPlayer->Open(m_pTimeline);
	m_pIFSMediaPlayer->Play();
	QTimer::singleShot(0, [&]{
		resize(width() + 2, height());
	});
	
}

void MVPlayer::resizeEvent(QResizeEvent *e)
{
	static const int titleH = 34;
	m_pTitleBar->setGeometry(1, 1, width()-2, titleH);
	m_pIFSMediaPlayer->setGeometry(1, titleH+1, width()-2, height() - titleH-2);
}