#include "stdafx.h"
#include "inc_FSMediaPlayerView/IFSMediaPlayer.h"
#include "FSPropertyView.h"
#include "FSMediaPlayer.h"
#include "FSEditHelper.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_FSTimelineView/UndoCommandBase.h"
#include "inc_MediaPlayerView/MediaPlayerControl.h"
#include "FSPropertyView.h"



IFSMediaPlayer::IFSMediaPlayer(QWidget *parent)
    :QWidget(parent)
{
    mpFSMediaPlayer = new FSMediaPlayer(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mpFSMediaPlayer);
    mpFSMediaPlayer->resize(500, 500);
    setObjectName("FSCommonLib_IFSMediaPlayer");
    setAttribute(Qt::WA_StyledBackground);
	resize(500, 500);

}

IFSMediaPlayer::~IFSMediaPlayer()
{

}

bool IFSMediaPlayer::Open(FSNLETimeline *timeline)
{
    return mpFSMediaPlayer->Open(1, timeline);
}

bool IFSMediaPlayer::Close()
{
    return mpFSMediaPlayer->Close();
}

void IFSMediaPlayer::Play()
{
    mpFSMediaPlayer->Play();
}

void IFSMediaPlayer::Stop()
{
    mpFSMediaPlayer->Stop();
}

void IFSMediaPlayer::Pause()
{
    mpFSMediaPlayer->Pause();
}

void IFSMediaPlayer::Seek(int frames)
{
    mpFSMediaPlayer->Seek(frames);
}

void IFSMediaPlayer::SetDuration(int duration)
{
    mpFSMediaPlayer->SetDuration(duration);
}

void IFSMediaPlayer::Forward()
{
    mpFSMediaPlayer->Forward();
}

void IFSMediaPlayer::Backward()
{
    mpFSMediaPlayer->Backward();
}

void IFSMediaPlayer::Restore()
{
    mpFSMediaPlayer->Restore();
}

int IFSMediaPlayer::Duration()
{
    return mpFSMediaPlayer->Duration();
}

bool IFSMediaPlayer::HaveSource()
{
    return mpFSMediaPlayer->HaveSource();
}



bool IFSMediaPlayer::IsPlaying()
{
	return mpFSMediaPlayer->IsPlaying();

}

void IFSMediaPlayer::SetChunkHeight(int height)
{
    mpFSMediaPlayer->SetChunkHeight(height);
}

void IFSMediaPlayer::Update()
{
    mpFSMediaPlayer->Update();

}

////////////////////////////////////////////////////////////////////////////////


IFSEditMediaPlayer::IFSEditMediaPlayer(QWidget *parent)
{
    
    m_pEditPlayer = new FSEditPlayerView(this);
    m_pEditPlayer->resize(750, 400);
    m_pEditArea = new FSMediaScrollArea(m_pEditPlayer,this);
    m_pEditArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pEditArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QHBoxLayout *pHL = new QHBoxLayout(this);
    pHL->addWidget(m_pEditArea);
    pHL->setMargin(0);
    pHL->setSpacing(0);
    setObjectName("FSCommonLib_IFSMediaPlayer");
    setAttribute(Qt::WA_StyledBackground);
    resize(750, 400);
    connect(m_pEditArea, &FSMediaScrollArea::sigChangeScale, this, &IFSEditMediaPlayer::sigChangeScale);
    connect(m_pEditArea, &FSMediaScrollArea::sigMenuChangeScale, this, &IFSEditMediaPlayer::sigMenuChangeScale);
    connect(m_pEditPlayer, &FSEditPlayerView::sigEnableEditShotCut, this, &IFSEditMediaPlayer::sigEnableEditShotCut);
    connect(m_pEditPlayer, &FSEditPlayerView::sigBackForwedShortCutTriged, this, &IFSEditMediaPlayer::sigBackForwedShortCutTriged);
    connect(m_pEditPlayer, &FSEditPlayerView::sigEditModeChanged, this, &IFSEditMediaPlayer::sigEditModeChanged);
    connect(m_pEditPlayer, &FSEditPlayerView::sigProjectSettingTrigged, this, &IFSEditMediaPlayer::sigProjectSettingTriggered);
    connect(m_pEditPlayer, &FSEditPlayerView::sigResolutionChanged, this, &IFSEditMediaPlayer::sigResolutionChanged);
}

IFSEditMediaPlayer::~IFSEditMediaPlayer()
{

}

bool IFSEditMediaPlayer::Open(FSNLETimeline *timeline)
{
    return m_pEditPlayer->Open(1, timeline);
}

bool IFSEditMediaPlayer::Close()
{
    return m_pEditPlayer->Close();
}

void IFSEditMediaPlayer::Play()
{
    m_pEditPlayer->Play();
}

void IFSEditMediaPlayer::Stop()
{
    m_pEditPlayer->Stop();
}

void IFSEditMediaPlayer::Pause()
{
    m_pEditPlayer->Pause();
}

void IFSEditMediaPlayer::Seek(int frames)
{
    m_pEditPlayer->Seek(frames);
}

void IFSEditMediaPlayer::SetDuration(int duration)
{
    m_pEditPlayer->SetDuration(duration);
}

void IFSEditMediaPlayer::Forward()
{
    m_pEditPlayer->Forward();
}

void IFSEditMediaPlayer::Backward()
{
    m_pEditPlayer->Backward();
}

void IFSEditMediaPlayer::Restore()
{
    m_pEditPlayer->Restore();
}

int IFSEditMediaPlayer::Duration()
{
    return m_pEditPlayer->Duration();
}

bool IFSEditMediaPlayer::HaveSource()
{
    return m_pEditPlayer->HaveSource();
}


int IFSEditMediaPlayer::CurrentFrame()
{
    return m_pEditPlayer->CurrentFrame();
}

bool IFSEditMediaPlayer::IsPlaying()
{
    return m_pEditPlayer->IsPlaying();
}

void IFSEditMediaPlayer::OnMediaPlayerBarStartChange()
{
    m_pEditPlayer->OnMediaPlayerBarStartChange();
}

void IFSEditMediaPlayer::OnMediaPlayerBarEndChange()
{
    m_pEditPlayer->OnMediaPlayerBarEndChange();
}

void IFSEditMediaPlayer::OnMediaPlayerBarChange(int frame)
{
    m_pEditPlayer->OnMediaPlayerBarChange(frame);
}

void IFSEditMediaPlayer::OnMediaPlayerBarFullScreen()
{
    m_pEditPlayer->OnMediaPlayerBarFullScreen();
}

void IFSEditMediaPlayer::OnMediaPlayerBarCropView(bool bScaleMode)
{
    m_pEditPlayer->OnMediaPlayerBarCropView(bScaleMode);
}

void IFSEditMediaPlayer::SetPlayerEditManager(IPlayerEditorManager* pManager)
{
    return m_pEditPlayer->SetPlayerEditManager(pManager);
}

void IFSEditMediaPlayer::SetTimelineViewManager(ITimelineViewManager*pManager)
{
    return m_pEditPlayer->SetTimelineViewManager(pManager);
}

void IFSEditMediaPlayer::SetTitleBarMannager(ITitleBarManager *pManager)
{
    m_pEditPlayer->SetTitleBarMannager(pManager);
}

ITimelineViewManager* IFSEditMediaPlayer::GetTimelineViewManager()
{
    return m_pEditPlayer->GetTimelineViewManager();
}

IFSEditHelperManager * IFSEditMediaPlayer::GetEditHelperManarger()
{
    return m_pEditPlayer->GetEditHelperManager();
}

IMediaPlayerControl * IFSEditMediaPlayer::GetMediaPlayerControl()
{
    return m_pEditPlayer->GetMediaPlayerControl();
}


void IFSEditMediaPlayer::SetChunkHeight(int height)
{
    m_pEditPlayer->SetChunkHeight(height);
}

void IFSEditMediaPlayer::SetScale(bool bFit, float scale)
{
    m_pEditArea->SetScaleSize(bFit,scale);
}

void IFSEditMediaPlayer::SetBaseRenderSize(QSize size)
{
    m_pEditArea->SetBaseRenderSize(size);
}

void IFSEditMediaPlayer::SetPlayBackResolution(float resulotion)
{
    m_pEditPlayer->SetPlayBackResolution(resulotion);
}

float IFSEditMediaPlayer::GetPlayBackResolution()
{
    return m_pEditPlayer->GetPlayBackResolution();
}

void IFSEditMediaPlayer::SetPlayerBackgroundColor(QColor color)
{
    m_pEditPlayer->SetPlayerBackgroundColor(color);
}

void IFSEditMediaPlayer::GetSnapShortImg(QImage & image)
{
    m_pEditPlayer->GetSnapShortImg(image);
}

void IFSEditMediaPlayer::ResetMartPoint()
{
    m_pEditArea->ResetMarkPoint();
}

//////////////////////////////////////////////////////////////////////////////////////////////
IFSPropertyView::IFSPropertyView(QWidget *pParent) 
    :QWidget(pParent)
{
    m_pPropertyView = new FSPropertyView(pParent);
    QHBoxLayout *pHL = new QHBoxLayout(this);
    pHL->addWidget(m_pPropertyView);
    pHL->setMargin(0);
    connect(m_pPropertyView, &FSPropertyView::sigShowPropertyView, this, &IFSPropertyView::sigShowPropertyView);
    connect(m_pPropertyView, &FSPropertyView::sigSpeedChanged, this, &IFSPropertyView::sigSpeedChanged);
	connect(m_pPropertyView, &FSPropertyView::sigDenoise, this, &IFSPropertyView::sigDenoise);
}



IFSPropertyView::~IFSPropertyView()
{

}

void IFSPropertyView::SetEditorHelperManager(IFSEditHelperManager * pHelperManager)
{
    m_pPropertyView->SetEditHelperManager(pHelperManager);
}



void IFSPropertyView::SetPlayerEditManager(IPlayerEditorManager* pPlayerEditorManager)
{
    m_pPropertyView->SetPlayerEditManager(pPlayerEditorManager);
}

QString GetPlayerStyleSheet()
{
    return gGetStyleSheet(":/FSMediaPlayerView/Resources/styles_light.qss");
}


/////////////////////////////////////////////////////////////
IFSEditHelperManager::IFSEditHelperManager(QObject *pParent)
:QObject(pParent)
{

}

IFSEditHelperManager::~IFSEditHelperManager()
{

}

void IFSEditHelperManager::EnableSnap(bool bEnable)
{

}
