#include "stdafx.h"
#include "FSBaseToolBar.h"
#include "FSCropWidget.h"
#include "FSFullScreenControl.h"
#include "FSMediaPlayer.h"
#include "FSEditHelperManager.h"
#include "FSPropertyView.h"
#include "FSEditHelper.h"
#include "FSPlayerShortCutContrl.h"
#include "inc_MediaPlayerView/MediaPlayerView.h"
#include "inc_CommonLib/NLEBaseWidget.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/FileOperation.h"
#include "inc_CommonLib/PathOperation.h"
#include "inc_CommonLib/GlobalResources.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_FSTimelineView/fstimelineview.h"
#include "INLEMouseEffectManager.h"
#include "FSUnDoReDoCommand.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "inc_FSCommonlib/gathread.h"


#define CaptionFileName "Test_1.xml"
#define VideoFileName "RecordFile_343681435.mov"


FSEditPlayerView::FSEditPlayerView(QWidget *parent)
    : FSBaseWidget(parent)
    , mpMediaPlayerView(nullptr)
    , mpPlayerToolBar(nullptr)
    , mpFullScreenControl(nullptr)
    , mpMainWindow(nullptr)
    , mpPlayerEditorManager(nullptr)
    , mpTimelineViewManager(nullptr)
    , mpControl(nullptr)
    , mbSeekManuel(false)
    , m_PlayResolutionScale(1.0)
    , m_BaseRenderSize(1920,1080)
{  
    CreateUI();
    RetranslateUi();
    ConnectSignals();
    PlayerViewChange2EditView(false);
    setObjectName("FSCommonLib_IFSMediaPlayer");
    setAttribute(Qt::WA_StyledBackground);
    setAcceptDrops(true);
    SetPlayerBorderWidth(2);
    SetPlayerBorderColor(QColor("#27313C"));
}

FSEditPlayerView::~FSEditPlayerView()
{

}

bool FSEditPlayerView::Open(int bSource, FSNLETimeline *timeline)
{
    return mpPlayerToolBar->Open(bSource, timeline);
}

bool FSEditPlayerView::Close()
{
    return mpPlayerToolBar->Close();
}

void FSEditPlayerView::Play()
{
    mpPlayerToolBar->Play();
}

void FSEditPlayerView::Stop()
{
    mpPlayerToolBar->Stop();
}

void FSEditPlayerView::Pause()
{
	if (mpPlayerToolBar->IsPlaying())
	{
		mpPlayerToolBar->Pause();
	}
}


void FSEditPlayerView::Seek(int frames)
{
	//frames = qMin(mpPlayerToolBar->Duration(), frames);
    mpPlayerToolBar->Seek(frames);
}

void FSEditPlayerView::SetDuration(int duration)
{
    mpPlayerToolBar->SetDuration(duration);
}

void FSEditPlayerView::Forward()
{
    mpPlayerToolBar->Forward();
    if (mpPreviewEditView->isVisible())
    {
        mpPreviewEditView->update();
    }
}

void FSEditPlayerView::Backward()
{
    mpPlayerToolBar->Backward();
    if (mpPreviewEditView->isVisible())
    {
        mpPreviewEditView->update();
    }
}

void FSEditPlayerView::Restore()
{
    if (mpFullScreenControl->Restore())
    {
        mpPlayerToolBar->Restore();
        mpPlayerToolBar->setVisible(false);
    }
}

int FSEditPlayerView::Duration()
{
    return mpPlayerToolBar->Duration();
}


int FSEditPlayerView::CurrentFrame()
{
    return mpPlayerToolBar->CurrentFrames();
}

bool FSEditPlayerView::HaveSource()
{
   return mpPlayerToolBar->HaveSource();
}

void FSEditPlayerView::PlayerViewChange2EditView(bool bShowEdit)
{
    if (bShowEdit)
    {
        mpMediaPlayerView->setVisible(false);   
        mpPreviewEditView->SetRenderSize(size());
        mpPreviewEditView->ShowEditView(true);           
        emit ShowEditViewSig(true);
        mpMediaPlayerView->lower(); 
        bool reg = static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->isEditTextHelper();
        EnableShortCut(!reg);
    }
    else
    {   
        emit ShowEditViewSig(false);  
        EnableShortCut(true,true); 
        mpPreviewEditView->ShowEditView(false);
        if (mpControl->HaveSource())
            mpControl->GetTimeline()->SetRenderSize(m_BaseRenderSize.width()*m_PlayResolutionScale, m_BaseRenderSize.height()*m_PlayResolutionScale);
        mpControl->Update();
        mpMediaPlayerView->setVisible(true);
    }   
}

void FSEditPlayerView::OnPlayerViewMousePress(QMouseEvent *e)
{
    if (mpMediaPlayerView->GetMediaPlayerControl()->HaveSource()&&!mpFullScreenControl->IsFullScreen())
    {
        Pause();
        PlayerViewChange2EditView(true);
        mpPreviewEditView->mousePressEvent(e);
        QMouseEvent e1(QMouseEvent::MouseButtonRelease, e->localPos(), e->button(), e->buttons(), e->modifiers());
        mpPreviewEditView->mouseReleaseEvent(&e1);
    }
    else if (mpFullScreenControl->IsFullScreen())
    {
        if (IsPlaying())
            Pause();
        else
            Play();
    }
    
}

bool FSEditPlayerView::IsPlaying()
{
    return mpControl->IsPlaying();
}

void FSEditPlayerView::SetPlayerEditManager(IPlayerEditorManager *pPlayerManager)
{
    Q_ASSERT(pPlayerManager);
    mpPlayerEditorManager = pPlayerManager;
    connect(mpPlayerEditorManager, &IPlayerEditorManager::signalSelectionChanged, this, &FSEditPlayerView::OnTimeLineSelectionChangedSlot, Qt::UniqueConnection);
    connect(mpPlayerEditorManager, &IPlayerEditorManager::signalTimelineUpdate, this, &FSEditPlayerView::OnTimeLineUpdataSlot, Qt::UniqueConnection);
    connect(mpPlayerEditorManager, &IPlayerEditorManager::sigMouseEffectAdded, this, &FSEditPlayerView::OnMouseEffectAddedSlot, Qt::UniqueConnection);
    connect(mpPlayerEditorManager, &IPlayerEditorManager::sigMouseEffectRemove, this, &FSEditPlayerView::OnMouseEffectRemoveSlot, Qt::UniqueConnection);
    connect(mpPlayerEditorManager, &IPlayerEditorManager::sigPause, this, &FSEditPlayerView::Pause, Qt::UniqueConnection);
    connect(mpPlayerEditorManager,&IPlayerEditorManager::sigTimelineAdded, this, &FSEditPlayerView::OnTimeLineAddedSlot, Qt::UniqueConnection);
    static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->SetPlayerEditorManager(mpPlayerEditorManager);
}

void FSEditPlayerView::SetTimelineViewManager(ITimelineViewManager *pTimeLineManager)
{
    Q_ASSERT(pTimeLineManager);
    mpTimelineViewManager = pTimeLineManager;
    Open(1, mpTimelineViewManager->GetRootTimeline());
    //SetBaseRenderSize(mpControl->GetTimeline()->GetRenderSize());
    connect(mpTimelineViewManager, &ITimelineViewManager::sigStartSeek, this, &FSEditPlayerView::OnTimeLineViewSeekStartSlot,Qt::UniqueConnection);
    connect(mpTimelineViewManager, &ITimelineViewManager::sigSeek, this, &FSEditPlayerView::OnTimeLineViewSeekSlot, Qt::UniqueConnection);
    connect(mpTimelineViewManager, &ITimelineViewManager::sigEndSeek, this, &FSEditPlayerView::OnTimeLineViewSeekEndSlot, Qt::UniqueConnection);
    connect(mpTimelineViewManager, &ITimelineViewManager::sigDurationChanged, this, &FSEditPlayerView::OnDurationChangedSlot, Qt::UniqueConnection);
    connect(mpTimelineViewManager, &ITimelineViewManager::sigClear, this, &FSEditPlayerView::OnTimelineClear, Qt::UniqueConnection);
    
}

void FSEditPlayerView::SetTitleBarMannager(ITitleBarManager *pTitleBarMannager)
{
    Q_ASSERT(pTitleBarMannager);
    mpTitleBarMannager = pTitleBarMannager;
}

ITimelineViewManager* FSEditPlayerView::GetTimelineViewManager()
{
    return mpTimelineViewManager;
}

IFSEditHelperManager * FSEditPlayerView::GetEditHelperManager()
{
    return mpPreviewEditView->GetEditHelperManager();
}

ITitleBarManager * FSEditPlayerView::GetTitleBarMannager()
{
    return mpTitleBarMannager;
}

IMediaPlayerControl * FSEditPlayerView::GetMediaPlayerControl()
{
    return mpControl;
}


IPlayerEditorManager * FSEditPlayerView::GetPlayerEditorManager()
{
    return mpPlayerEditorManager;
}

FSPlayerMenuControl * FSEditPlayerView::GetMenuControl()
{
    return mpMenuControl;
}

FSPlayerShortCutContrl * FSEditPlayerView::GetShortCutControl()
{
    return mpShortCutContrl;
}

void FSEditPlayerView::SetChunkHeight(int height)
{
    mpPlayerToolBar->SetChunkHeight(height);
}

void FSEditPlayerView::OnMediaPlayerBarStartChange()
{
    OnTimeLineViewSeekStartSlot();
}

void FSEditPlayerView::OnMediaPlayerBarEndChange()
{
    OnTimeLineViewSeekEndSlot();
}

void FSEditPlayerView::OnMediaPlayerBarChange(int frame)
{
    OnTimeLineViewSeekSlot(frame);
}

void FSEditPlayerView::OnMediaPlayerBarFullScreen()
{
    OnFullScreen();
    mpPlayerToolBar->SetFullScreenBtnVisible(!mpFullScreenControl->IsFullScreen());
}

void FSEditPlayerView::OnMediaPlayerBarCropView(bool bScaleMode)
{
    OnCropView(bScaleMode);
    mpPlayerToolBar->SetScaleBtnVisible(!bScaleMode);
}

void FSEditPlayerView::OnFullScreen()
{
    if (!mpFullScreenControl->IsFullScreen() && !mpPlayerToolBar->IsPlaying())
    {
        //修改暂停时全屏闪烁问题，先将编辑界面隐藏再将全屏切换加到事件循环最后。
        PlayerViewChange2EditView(false);
    }
    QTimer::singleShot(100, [&](){
        SetPlayerBorderWidth(mpFullScreenControl->IsFullScreen()?2:0);
        mpFullScreenControl->FullScreen();
        if (!mpFullScreenControl->IsFullScreen())
        {
            mpPlayerToolBar->setVisible(false);
            Q_ASSERT(parentWidget());
            parentWidget()->setGeometry(QRect(parentWidget()->geometry().topLeft(), parentWidget()->geometry().size() + QSize(1, 1)));
            parentWidget()->setGeometry(QRect(parentWidget()->geometry().topLeft(), parentWidget()->geometry().size() + QSize(-1, -1)));
            if (!mpPlayerToolBar->IsPlaying())
            {
                PlayerViewChange2EditView(true);
            }           
        }
        else
        {
            mpPlayerToolBar->setVisible(true);
            PlayerViewChange2EditView(false);
        }
    }
    );    
}

void FSEditPlayerView::OnCropView(bool bScaleMode)
{
    static_cast< FSEditHelperManager * >(mpPreviewEditView->GetEditHelperManager())->SetScaleMode(bScaleMode);
}


void FSEditPlayerView::OnShortCutActived(int id)
{
    switch (id)
    {
        case FSPlayerShortCutContrl::ShortCutId::Space_ShortCut:
        {
            if (!HaveSource() || !isVisible() || mpControl->GetTimeline()->GetClipCount() == 0)
                return;
            if (IsPlaying())
            {
                Pause();
            }
            else
            {
                Play();
            }
        }
        break;
        case FSPlayerShortCutContrl::ShortCutId::Stop_ShortCut:
        {
            if (!HaveSource() || !isVisible())
                return;
            Stop();
        }
        break;
        case FSPlayerShortCutContrl::ShortCutId::Forward_ShortCut:
        {
            if (!HaveSource() || !isVisible())
                return;
            Backward();
            mpTimelineViewManager->SetCurrentFrame(mpPlayerToolBar->CurrentFrames());
            emit sigBackForwedShortCutTriged(mpPlayerToolBar->CurrentFrames());
        }
        break;
        case FSPlayerShortCutContrl::ShortCutId::Bacward_ShortCut:
        {
            if (!HaveSource() || !isVisible())
                return;
            Forward();
            mpTimelineViewManager->SetCurrentFrame(mpPlayerToolBar->CurrentFrames());
            emit sigBackForwedShortCutTriged(mpPlayerToolBar->CurrentFrames());
        }
        break;
        case FSPlayerShortCutContrl::ShortCutId::FullScreen_ShortCut:
        {
            if (!HaveSource() || !isVisible() || !mpPlayerToolBar->isEnabled())
                return;
            if (!mpFullScreenControl->IsFullScreen())
                mpPlayerToolBar->DblMouseBtnFullScreen();
        }
        break;
        case FSPlayerShortCutContrl::ShortCutId::Alt_ShortCut:
            break;
        default:
            break;
    }
}

void FSEditPlayerView::OnLongPressedShortCut(int id,bool bPress)
{
    switch (id)
    {
    case FSPlayerShortCutContrl::ShortCutId::Space_ShortCut:
        {
            emit sigSpaceLongPress(bPress);
        }
        break;
    case FSPlayerShortCutContrl::ShortCutId::Stop_ShortCut:   
    case FSPlayerShortCutContrl::ShortCutId::Forward_ShortCut:   
    case FSPlayerShortCutContrl::ShortCutId::Bacward_ShortCut:   
    case FSPlayerShortCutContrl::ShortCutId::FullScreen_ShortCut:
        break;
    case FSPlayerShortCutContrl::ShortCutId::Alt_ShortCut:
    {       
        if (mpPlayerToolBar->IsScaleState())
        {
            if (bPress)
            {
                OnMediaPlayerBarCropView(false);
                emit sigEditModeChanged(mpPlayerToolBar->IsScaleState());
            }
        }
        else
        {
            if (!bPress)
            {
                OnMediaPlayerBarCropView(true);
                emit sigEditModeChanged(mpPlayerToolBar->IsScaleState());
            }
        }                 
    }
        break;
    default:
        break;
    }
}

void FSEditPlayerView::resizeEvent(QResizeEvent *e)
{  
    mpMediaPlayerView->setGeometry(0, 0, width(), height());
    mpPreviewEditView->setGeometry(0, 0, width(), height());
    mpPlayerToolBar->setGeometry(0, height()-mpPlayerToolBar->height(), width(), mpPlayerToolBar->height());
    mpFullScreenControl->ResizeEvent(mpMediaPlayerView,rect()); 
    mpPlayerToolBar->raise();
    if (!mpControl->IsPlaying())
        mpPreviewEditView->SetRenderSize({ width(), height() });
}


FSPreviewEditView * FSEditPlayerView::GetPreviewEditView()
{
    return mpPreviewEditView;
}

void FSEditPlayerView::AddDropTimeLineList(QList<FSNLETimeline *> list, QPointF DragPos)
{   
    if (!list.isEmpty())
    {
        mpPlayerEditorManager->AddTimelines(list);
    }
}

void FSEditPlayerView::AddDropMouseClickSound(QString filePath, QPointF DragPos, QPointF offset)
{
    IFSEditHelper * pEditHelper = static_cast<FSEditHelperManager *>(GetEditHelperManager())->GetTopLevelCanAddMouseEffcetEditHelper(DragPos, offset);
   
    if (pEditHelper)
    {
        QList<FSNLETimeline *> Sublist;
        Sublist << pEditHelper->GetTimeLine();
        pEditHelper->SetSelected(true);
        mpPlayerEditorManager->SetSelected(Sublist);
        mpPlayerEditorManager->AddMouseSound(pEditHelper->GetTimeLine(), filePath);      
    }
}

void FSEditPlayerView::AddDropMouseFliter(FSNLETimeline * pTimeLine, QPointF DragPos, QPointF offset)
{
    IFSEditHelper * pEditHelper = static_cast<FSEditHelperManager *>(GetEditHelperManager())->GetTopLevelCanAddMouseEffcetEditHelper(DragPos, offset);
    if (pEditHelper)
    {
        QList<FSNLETimeline *> Sublist;
        Sublist << pEditHelper->GetTimeLine();
        pEditHelper->SetSelected(true);
        mpPlayerEditorManager->SetSelected(Sublist);
        mpPlayerEditorManager->AddMouseFilter(pEditHelper->GetTimeLine(), pTimeLine->Clone());       
    }
    pTimeLine->deleteLater();
}

void FSEditPlayerView::SetPlayerBorderWidth(int w)
{
    mpPreviewEditView->SetPlayerBorderWidth(w);
    mpMediaPlayerView->SetPlayerBorderWidth(w);
}

int FSEditPlayerView::GetPlayerBorderWidth()
{
    return mpPreviewEditView->GetPlayerBorderWidth();
}

void FSEditPlayerView::SetPlayerBorderColor(QColor color)
{
    mpPreviewEditView->SetPlayerBorderColor(color);
    mpMediaPlayerView->SetPlayerBorderColor(color);
}


void FSEditPlayerView::GetSnapShortImg(QImage & image)
{
    Pause();
    mpControl->GetTimeline()->SetRenderSize(m_BaseRenderSize.width(), m_BaseRenderSize.height());
    mpControl->Update();
    mpMediaPlayerView->GetSnapShortImg(image);
    mpPreviewEditView->SetRenderSize(size());   
}

bool FSEditPlayerView::isPressingSpace()
{
    return mpShortCutContrl->isPressingSpace();
}

void FSEditPlayerView::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        if (mpFullScreenControl->IsFullScreen())
        {
            mpPlayerToolBar->DblMouseBtnFullScreen();
            return;
        }
    }
    return QWidget::keyPressEvent(e);
}

void FSEditPlayerView::mouseMoveEvent(QMouseEvent *e)
{
    mpFullScreenControl->MayShowToolBar(rect(), e->pos());
    QWidget::mouseMoveEvent(e);
}


void FSEditPlayerView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::MouseButton::RightButton)
    {   
        mpMenuControl->EnableAction(FSPlayerMenuControl::Action_Reset, static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->GetResetableEditHelpers().size() > 0);
        mpMenuControl->EnableAction(FSPlayerMenuControl::Action_Paste, mpTitleBarMannager->CanPaste());
        mpMenuControl->EnableAction(FSPlayerMenuControl::Action_Copy, mpTitleBarMannager->CanCopy());
        mpMenuControl->EnableAction(FSPlayerMenuControl::Action_Delete, mpTitleBarMannager->CanDelete());
        mpMenuControl->EnableAction(FSPlayerMenuControl::Action_Cut, mpTitleBarMannager->CanCut());
        mpMenuControl->PopUp(e->globalPos());
    }
    return QWidget::mouseReleaseEvent(e);
}

void FSEditPlayerView::RetranslateUi()
{
    mpMenuControl->RetranslateUi();
}


void FSEditPlayerView::ConnectSignals()
{
    connect(mpPlayerToolBar, &FSBaseToolBar::StartRulerSeekCallback, [this](bool isPlaying){
        mbSeekManuel = true;
        PlayerViewChange2EditView(false);
    });
    connect(mpPlayerToolBar, &FSBaseToolBar::EndRulerSeekCallback, [this](bool isPlaying){
        mbSeekManuel = false;
        if (mpFullScreenControl->IsFullScreen())
            PlayerViewChange2EditView(false);
        else
            PlayerViewChange2EditView(!isPlaying);
    });

    connect(mpPlayerToolBar, &FSBaseToolBar::EndBackForword, [this](){
        if (mpPreviewEditView->isVisible())
            mpPreviewEditView->update();
        if (!mpFullScreenControl->IsFullScreen())
            PlayerViewChange2EditView(true);
        mpTimelineViewManager->SetCurrentFrame(mpPlayerToolBar->CurrentFrames());
    });

    connect(mpPlayerToolBar, &FSBaseToolBar::FullScreenSignal, this, &FSEditPlayerView::OnFullScreen);
    connect(mpPlayerToolBar, &FSBaseToolBar::VeiwCutSignal, this, &FSEditPlayerView::OnCropView);

    connect(mpMediaPlayerView, &FSMediaPlayerView::MousePreesEvent, [this](QMouseEvent *e){
        OnPlayerViewMousePress(e);
    });

    connect(mpControl, &IMediaPlayerControl::PlayPauseCallback, [this](){
        if (!mbSeekManuel&&!mpFullScreenControl->IsFullScreen())
        {
            PlayerViewChange2EditView(true);
        }
    });
    

    connect(mpControl, &IMediaPlayerControl::PlayStopCallback, [this](){
        mpTimelineViewManager->SetCurrentFrame(0);
        Seek(0);
        static_cast<FSEditHelperManager *>(GetEditHelperManager())->sigRepaintWidget();
    });
    connect(mpControl, &IMediaPlayerControl::SeekFinishCallback, [this](int frames){
        mpTimelineViewManager->SetCurrentFrame(frames);
    });
    connect(mpControl, &IMediaPlayerControl::PlayingCallback, [this](int frames){
        mpTimelineViewManager->SetCurrentFrame(frames);
    });

    connect(mpControl, &IMediaPlayerControl::PlayStartCallback, [this](){
        PlayerViewChange2EditView(false);
    });

    connect(mpControl, &IMediaPlayerControl::PlayLoadFinishCallback, [this](){
        PlayerViewChange2EditView(true);
    });


    connect(mpMediaPlayerView, &FSMediaPlayerView::MouseDoubleClicked, [this](){
        if (mpControl->HaveSource())
            mpPlayerToolBar->DblMouseBtnFullScreen();
    });

    connect(mpShortCutContrl, &FSPlayerShortCutContrl::ShortCutActivated, this, &FSEditPlayerView::OnShortCutActived);
    connect(mpShortCutContrl, &FSPlayerShortCutContrl::LongPressedShortCut, this, &FSEditPlayerView::OnLongPressedShortCut);

    connect(static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager()), &FSEditHelperManager::SelectionChanged, this, &FSEditPlayerView::OnSelectionChanged, Qt::QueuedConnection);

    connect(static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager()), &FSEditHelperManager::TextReDoUnDo, this, &FSEditPlayerView::OnTextReDoUnDoSlot);
    connect(static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager()), &FSEditHelperManager::MousePressedSig, this, &FSEditPlayerView::OnMousePressedSlot);

    connect(mpMenuControl, &FSPlayerMenuControl::ActionTriggered, [&](int id){
        bool bFit = false;
        float fZoomScale = 1.0;
        switch (id)
        {
        case FSPlayerMenuControl::Action_Reset:
        {
            QList<IFSEditHelper *> list = static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->GetResetableEditHelpers();
            if (!list.isEmpty())
            {
                mpPlayerEditorManager->BeginComposite();
                for (auto item : list)
                {
                    if (item->GetEditType() == IFSEditHelper::Edit_Text)
                    {
                        double scalex, scaley, transx, transy, angle;
                        double defaultscalex, defaultscaley, defaulttransx, defaulttransy, defaultangle;
                        item->GetDefaultAngle(defaultangle);
                        item->GetDefaultScale(defaultscalex, defaultscaley);
                        item->GetDefaultPosiont(defaulttransx, defaulttransy);
                        angle = item->GetAngle();
                        item->GetPosition(transx, transy);
                        item->GetScale(scalex, scaley);
                        if (qAbs(angle - defaultangle) > 1e-4)
                            item->SetAngle(defaultangle);
                        if (qAbs(scalex - defaultscalex) > 1e-4 || qAbs(scaley - defaultscaley) > 1e-4)
                            item->SetScale(defaultscalex, defaultscaley);
                        if (qAbs(transx - defaulttransx) > 1e-4 || qAbs(transy - defaulttransy) > 1e-4)
                            item->SetPosition(defaulttransx, defaulttransy);

                    }
                    else if (item->GetEditType() != IFSEditHelper::Edit_Audio)
                    {
                        double scalex, scaley, transx, transy, angle;
                        item->GetDefaultAngle(angle);
                        item->GetDefaultScale(scalex, scaley);
                        item->GetDefaultPosiont(transx, transy);
                        UndoCommand *pCmd = new FSVisaulUndoCommand(item, { scalex, scaley }, { transx, transy }, angle, false);
                        mpPlayerEditorManager->PushCommand(pCmd);
                    }
                }
                mpPlayerEditorManager->EndComposite();
                static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->sigRepaintWidget();
            }
        }
            break;
        case FSPlayerMenuControl::Action_Copy:
            if (mpTitleBarMannager->CanCopy())
                mpTitleBarMannager->Copy();
            break;
        case FSPlayerMenuControl::Action_Paste:
            if (mpTitleBarMannager->CanPaste())
                mpTitleBarMannager->Paste();
            break;
        case FSPlayerMenuControl::Action_Cut:
            if (mpTitleBarMannager->CanCut())
                mpTitleBarMannager->Cut();
            break;
        case FSPlayerMenuControl::Action_Delete:
            if (mpTitleBarMannager->CanDelete())
                mpTitleBarMannager->Delete();
            break;
        case FSPlayerMenuControl::Action_ProjectSetting:
            emit sigProjectSettingTrigged();
            break;
        case FSPlayerMenuControl::Action_PlayResolution_Full:
            m_PlayResolutionScale = 1.0;
            emit sigResolutionChanged(m_PlayResolutionScale);
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_2:
            m_PlayResolutionScale = 0.5;
            emit sigResolutionChanged(m_PlayResolutionScale);
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_4:
            m_PlayResolutionScale = 0.25;
            emit sigResolutionChanged(m_PlayResolutionScale);
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_8:
            m_PlayResolutionScale = 0.125;
            emit sigResolutionChanged(m_PlayResolutionScale);
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_16:
            m_PlayResolutionScale = 0.0625;
            emit sigResolutionChanged(m_PlayResolutionScale);
            break;
        case FSPlayerMenuControl::Action_Zoom_Fix:
            bFit = true;
            emit sigZoom(true, tr("Fit"));
            break;
        case FSPlayerMenuControl::Action_Zoom_200:
            emit sigZoom(false, "200%");
            break;
        case FSPlayerMenuControl::Action_Zoom_175:
            emit sigZoom(false, "175%");
            break;
        case FSPlayerMenuControl::Action_Zoom_150:
            emit sigZoom(false, "150%");
            break;
        case FSPlayerMenuControl::Action_Zoom_125:
            emit sigZoom(false, "125%");
            break;
        case FSPlayerMenuControl::Action_Zoom_100:
            emit sigZoom(false, "100%");
            break;
        case FSPlayerMenuControl::Action_Zoom_85:
            emit sigZoom(false, "85%");
            break;
        case FSPlayerMenuControl::Action_Zoom_75:
            emit sigZoom(false, "75%");
            break;
        case FSPlayerMenuControl::Action_Zoom_60:
            emit sigZoom(false, "60%");
            break;
        case FSPlayerMenuControl::Action_Zoom_50:
            emit sigZoom(false, "50%");
            break;
        case FSPlayerMenuControl::Action_Zoom_35:
            emit sigZoom(false, "35%");
            break;
        case FSPlayerMenuControl::Action_Zoom_25:
            emit sigZoom(false, "25%");
            break;
        case FSPlayerMenuControl::Action_Zoom_10:
            emit sigZoom(false, "10%");
            break;
        default:
            break;
        }
    });
}

void FSEditPlayerView::CreateUI()
{  
    setMouseTracking(true);
    mpMediaPlayerView = new FSMediaPlayerView(this);
    mpControl = mpMediaPlayerView->GetMediaPlayerControl();
    Q_ASSERT(mpControl);   
    mpPreviewEditView = new FSPreviewEditView(mpControl, this);
    mpPlayerToolBar = new FSBaseToolBar(mpControl, this);
    mpFullScreenControl = new FSFullScreenControl(this);
    mpShortCutContrl = new FSPlayerShortCutContrl(this);
    mpFullScreenControl->SetToolBar(mpPlayerToolBar);
    mpMenuControl = new FSPlayerMenuControl(this);
    setAttribute(Qt::WA_StyledBackground);  
    PlayerViewChange2EditView(true);
    mpPlayerToolBar->setVisible(false);
}

void FSEditPlayerView::OnTimeLineUpdataSlot()
{
    if (!mpPlayerToolBar->IsPlaying() && !mbSeekManuel&&!mpFullScreenControl->IsFullScreen())
    {
        if (mpControl->HaveSource())
            mpControl->Update();
        PlayerViewChange2EditView(true);
        IFSEditHelperManager *pEdithelperMg = mpPreviewEditView->GetEditHelperManager();
        pEdithelperMg->CurrentEditHelperChanged(static_cast<FSEditHelperManager *>(pEdithelperMg)->GetCurrentEditHelper(), false);
        static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->sigRepaintWidget();
    }
    else if (mpPreviewEditView->isVisible())
    {
        PlayerViewChange2EditView(false);
    }
    
}

void FSEditPlayerView::OnTimeLineSelectionChangedSlot()
{
    mpPreviewEditView->SetTimeLineList(mpPlayerEditorManager->GetTimelines(), mpPlayerEditorManager->GetSelected(), mpPlayerEditorManager->GetOtherSelected(), mpPlayerToolBar->IsScaleState());
    bool reg = static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->isEditTextHelper();
    EnableShortCut(!reg);
}

void FSEditPlayerView::OnDurationChangedSlot(int duration)
{
    SetDuration(mpControl->Duration());  
	if (!mpPlayerToolBar->IsPlaying())
	{
		//auto curFrame = qBound(0,mpPlayerToolBar->CurrentFrames(), mpControl->Duration());
		//mpPlayerToolBar->Seek(curFrame);
        //mpTimelineViewManager->SetCurrentFrame(curFrame);
	}
    if (mpControl->GetTimeline()->GetClipCount() == 0)
    {
        Seek(0);
        mpTimelineViewManager->SetCurrentFrame(0);
    }
    mpPlayerToolBar->EnableWidgets(mpControl->GetTimeline()->GetClipCount() > 0);
}

void FSEditPlayerView::OnTimeLineAddedSlot(FSNLETimeline *pTimeLine)
{
    if (pTimeLine->GetType() == TimelineType::TimelineType_Image
        || pTimeLine->GetType() == TimelineType::TimelineType_Video
        || pTimeLine->GetType() == TimelineType::TimelineType_VideoAudio
        || pTimeLine->GetType() == TimelineType::TimelineType_Annotaions_Overlay)
    {
        pTimeLine->SetRenderMode(RenderMode_Stretch);
        pTimeLine->SetAnchorPointF(QPointF(0.5, 0.5));
        QSizeF sourceSize = pTimeLine->GetSourceSize();
        QSizeF renderSize = mpControl->GetTimeline()->GetRenderSize();
        renderSize.setHeight(renderSize.width() / mpControl->GetRatio());
        QSizeF renderSize1 = GetTimeLineInitRenderSize(pTimeLine,renderSize);
        
        if (pTimeLine->GetDependency())
        {
            QSizeF renderSizeSource = GetTimeLineInitRenderSize(pTimeLine->GetDependency(), renderSize);
            QSizeF renderSizeTemp = QSizeF(renderSizeSource.width()*0.4,renderSizeSource.height()*0.4);
            QSizeF renderSizeTemp1 = GetTimeLineInitRenderSize(pTimeLine, renderSizeTemp);
            QRectF RC = pTimeLine->GetDependency()->GetRenderRegion();
            float w = renderSizeTemp1.width() / renderSize.width();
            float h = renderSizeTemp1.height() / renderSize.height();
            float x = (renderSizeSource.width() - renderSizeTemp1.width()) / renderSize.width()+RC.topLeft().x();
            float y = RC.topLeft().y();
			pTimeLine->SetRenderResizeScale(QSizeF(w, h));
            pTimeLine->SetRenderRegion(QRectF(x, y,w,h));
            pTimeLine->SetDependency(NULL);

        }
        else
        {
			pTimeLine->SetRenderResizeScale(QSizeF(renderSize1.width() / renderSize.width(), renderSize1.height() / renderSize.height()));
            pTimeLine->SetRenderSize(renderSize.width(), renderSize.height());
            NLEPoint pt;
            HRESULT reg=pTimeLine->GetProp()->GetPoint(Drag_AddPos, &pt);

            if (!SUCCEEDED(reg))
            {
                QPointF LeftPt((renderSize.width() - renderSize1.width()) / 2 / renderSize.width(), (renderSize.height() - renderSize1.height()) / 2 / renderSize.height());
                QRectF RC = QRectF(LeftPt.x(), LeftPt.y(), renderSize1.width() / renderSize.width(), renderSize1.height() / renderSize.height());
                pTimeLine->SetRenderRegion(RC);
            }
            else
            {
                QPointF pos(pt.X,pt.Y);
                QPointF LeftPt(pos.x() / renderSize.width(), pos.y() / renderSize.height());
                QRectF RC = QRectF(LeftPt.x(), LeftPt.y(), renderSize1.width() / renderSize.width(), renderSize1.height() / renderSize.height());
                pTimeLine->SetRenderRegion(RC);               
            }
            pTimeLine->GetProp()->Delete(Drag_AddPos);
        }
               
        static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->UpdataEidyHelperData(pTimeLine);
        static_cast<FSEditHelperManager *>(mpPreviewEditView, GetEditHelperManager())->InitDefaultParam(pTimeLine);
    }


    if (pTimeLine->GetType() == TimelineType::TimelineType_Audio
        || pTimeLine->GetType() == TimelineType::TimelineType_VideoAudio){
        pTimeLine->SetMix2Mono(false);

    }

    if (pTimeLine->GetType() == TimelineType::TimelineType_Annotaions_Caption||
        pTimeLine->GetType() == TimelineType::TimelineType_Annotaions_Element)
    {
            NLEComPtr<INLECaption> pCaption = NULL;
            pTimeLine->GetSrc()->QueryInterface(IID_INLECaption, (LPVOID*)&pCaption);
            if (pCaption)
            {
                NLECaptionObjData objData;
                pCaption->GetObjParaByIndex(0, &objData);
                NLEPoint pt;
                HRESULT reg = pTimeLine->GetProp()->GetPoint(Drag_AddPos, &pt);
                if (SUCCEEDED(reg))
                {
                    QPointF pos(pt.X, pt.Y);
                    objData.pObj->SetPos(pos.x(), pos.y());
                }              
            }
            static_cast<FSEditHelperManager *>(mpPreviewEditView, GetEditHelperManager())->InitDefaultParam(pTimeLine);
            pTimeLine->GetProp()->Delete(Drag_AddPos);
            mpPreviewEditView->setFocus();
    }

	if (!mpPlayerToolBar->IsPlaying())
		mpControl->Update();
        //mpPlayerToolBar->Seek(mpPlayerToolBar->CurrentFrames());

    mpPlayerToolBar->EnableWidgets(mpControl->GetTimeline()->GetClipCount() > 0);
}

void FSEditPlayerView::OnTimelineClear()
{
    mpPreviewEditView->ClearEditHelper();
}

void FSEditPlayerView::OnTimeLineViewSeekStartSlot()
{
	m_iBegineSeekTime = QDateTime::currentDateTime().toTime_t();
    if (mpControl->GetTimeline()->GetClipCount() > 0)
    {
        mbSeekManuel = true;
        PlayerViewChange2EditView(false);
    }    
}

void FSEditPlayerView::OnTimeLineViewSeekSlot(int frames)
{  
    if (mpControl->GetTimeline()->GetClipCount() > 0)
    {
        Seek(frames);
    }
}

void FSEditPlayerView::OnTimeLineViewSeekEndSlot()
{
    if (mpControl->GetTimeline()->GetClipCount() > 0)
    {
        mbSeekManuel = false;
        QTimer::singleShot(1, this, [&](){
            PlayerViewChange2EditView(!mpControl->IsPlaying()); 
        });
    }
    if (!mpPlayerToolBar->IsPlaying())
        mpPreviewEditView->SetTimeLineList(mpPlayerEditorManager->GetTimelines(), mpPlayerEditorManager->GetSelected(), mpPlayerEditorManager->GetOtherSelected(), mpPlayerToolBar->IsScaleState());
	//上传GA数据
	m_iEndSeekTime = QDateTime::currentDateTime().toTime_t();
	int iCost = m_iEndSeekTime - m_iBegineSeekTime;
	GAThread::getInstance()->insertSeekInfo(iCost);
}

void FSEditPlayerView::OnMouseEffectAddedSlot(FSNLETimeline *pTimeLine, INLETimelineBase *filter)
{
    static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->TimeLineAddedMouseEficet(pTimeLine, filter);
	//上传信息
	if (filter == nullptr)
	{
		GAThread::getInstance()->insertEventItem(Clip_Edit, "AddLeftClickSound", "");
		return;
	}
	int iValue;
	NLEComPtr<INLEProperties> prop = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(filter);
	prop->GetInt(Key_MouseFilterType, &iValue);
	if (iValue == Value_MouseFilterType_Highlight)
	{
		GAThread::getInstance()->insertEventItem(Clip_Edit, "AddCursorHighLight", "");
	}
	else if (iValue == Value_MouseFilterType_Rings)
	{
		GAThread::getInstance()->insertEventItem(Clip_Edit, "AddLeftClickRing", "");
	}

}

void FSEditPlayerView::OnMouseEffectRemoveSlot(FSNLETimeline *pTimeLine, INLETimelineBase *filter)
{
    static_cast<FSEditHelperManager *>(mpPreviewEditView->GetEditHelperManager())->TimeLineRemoveMouseEficet(pTimeLine, filter);
}

void FSEditPlayerView::OnSelectionChanged(QList<IFSEditHelper *> list)
{
    QList<FSNLETimeline *> timeLineList;
    for (auto item : list)
    {
        timeLineList.push_back(item->GetTimeLine());
    }
    mpPlayerEditorManager->SetSelected(timeLineList);
}

void FSEditPlayerView::OnTextReDoUnDoSlot(IFSEditHelper *pHelper)
{
    Q_ASSERT(pHelper);
    if (pHelper)
    {
        mpPlayerEditorManager->PushCommand(new FSCaptionChangeCommand(pHelper));
    }
        
}

void FSEditPlayerView::OnMousePressedSlot()
{
    FSEditHelperManager *pEditManager = static_cast<FSEditHelperManager *>(sender());
    EnableShortCut(!pEditManager->isEditTextHelper());
}

void FSEditPlayerView::EnableShortCut(bool bEnable, bool bPlayerShotcutOnly)
{
    mpShortCutContrl->EnableShortCut(FSPlayerShortCutContrl::Space_ShortCut, bEnable);
    mpShortCutContrl->EnableShortCut(FSPlayerShortCutContrl::Forward_ShortCut, bEnable);
    mpShortCutContrl->EnableShortCut(FSPlayerShortCutContrl::Bacward_ShortCut, bEnable);
    if (!bPlayerShotcutOnly)
        emit sigEnableEditShotCut(bEnable);
}

void FSEditPlayerView::SetBaseRenderSize(QSize size)
{
    if (size.isValid())
    {
        mpControl->SetRatio(1.0*size.width() / size.height());
        m_BaseRenderSize = size;
        mpPreviewEditView->SetBaseRenderSize(size);
    }
    
}

void FSEditPlayerView::SetPlayBackResolution(double resolution)
{
    m_PlayResolutionScale = qBound(0.0625, resolution, 1.0);
    mpMenuControl->UpdataPlayResolutionMenuCheckState(m_PlayResolutionScale);
    emit sigResolutionChanged(m_PlayResolutionScale);
}

float FSEditPlayerView::GetPlayBackResolution()
{
    return m_PlayResolutionScale;
}

void FSEditPlayerView::SetPlayerBackgroundColor(QColor color)
{
    mpPreviewEditView->SetPlayerBackgroundColor(color);
}

QSizeF FSEditPlayerView::GetTimeLineInitRenderSize(FSNLETimeline *pTimeLine, QSizeF renderSize)
{
    QSizeF sourceSize = pTimeLine->GetSourceSize();
    QSizeF renderSize1;
    if (sourceSize.width() > renderSize.width() || sourceSize.height() > renderSize.height())
    {
        if (sourceSize.width() * renderSize.height() > sourceSize.height() * renderSize.width())
        {
            renderSize1.setWidth(renderSize.width());
            renderSize1.setHeight(renderSize1.width()*sourceSize.height() / sourceSize.width());
        }
        else
        {
            renderSize1.setHeight(renderSize.height());
            renderSize1.setWidth(renderSize.height()*sourceSize.width() / sourceSize.height());
        }
    }
    else
    {
        renderSize1 = sourceSize;
    }
    return renderSize1;
}

///////////////////////////////////////FSMediaPlayer////////////////////////////////////////////////

FSMediaPlayer::FSMediaPlayer(QWidget *parent)
    : QWidget(parent)
    , mpMediaPlayerView(nullptr)
    , mpPlayerToolBar(nullptr)
    , mpFullScreenControl(nullptr)
    , mpMainWindow(nullptr)
    , mpControl(nullptr)
{   
    setMouseTracking(true);
    mpMediaPlayerView = new FSMediaPlayerView(this);
    mpControl = mpMediaPlayerView->GetMediaPlayerControl();
    Q_ASSERT(mpControl);
    mpPlayerToolBar = new FSRecorderBaseToolBar(mpControl, this);
    mpFullScreenControl = new FSFullScreenControl(this);

    mpFullScreenControl->SetToolBar(mpPlayerToolBar);
    connect(mpPlayerToolBar, &FSRecorderBaseToolBar::RulerSeekCallback, this, &FSMediaPlayer::RulerSeekCallback);
    connect(mpPlayerToolBar, &FSRecorderBaseToolBar::StartRulerSeekCallback, [this](bool isPlaying){
        emit StartRulerSeekCallback(isPlaying);
    });
    connect(mpPlayerToolBar, &FSRecorderBaseToolBar::EndRulerSeekCallback, [this](bool isPlaying){
        emit EndRulerSeekCallback(isPlaying);
    });

    connect(mpPlayerToolBar, &FSRecorderBaseToolBar::FullScreenSignal, this, &FSMediaPlayer::OnFullScreen);
    connect(mpMediaPlayerView, &FSMediaPlayerView::MouseDoubleClicked, [this](){
        if (mpControl->HaveSource())
        {
            mpPlayerToolBar->DblMouseBtnFullScreen();
        }            
    });
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("FSCommonLib_IFSMediaPlayer");
    SetShortCut();
    

}

FSMediaPlayer::~FSMediaPlayer()
{

}

bool FSMediaPlayer::Open(int bSource, FSNLETimeline *timeline)
{
    bool reg = mpPlayerToolBar->Open(bSource, timeline);
    mpPlayerToolBar->EnableWidgets(reg);
    return reg;
}

bool FSMediaPlayer::Close()
{
    return mpPlayerToolBar->Close();
}

void FSMediaPlayer::Play()
{
    mpPlayerToolBar->Play();
}

void FSMediaPlayer::Stop()
{
    mpPlayerToolBar->Stop();
}

void FSMediaPlayer::Pause()
{
    mpPlayerToolBar->Pause();
}

void FSMediaPlayer::Seek(int frames)
{
    mpPlayerToolBar->Seek(frames);
}

void FSMediaPlayer::SetDuration(int duration)
{
    mpPlayerToolBar->SetDuration(duration);
}

void FSMediaPlayer::Forward()
{
    mpPlayerToolBar->Forward();
}

void FSMediaPlayer::Backward()
{
    mpPlayerToolBar->Backward();
}

void FSMediaPlayer::Restore()
{
    if (mpFullScreenControl->Restore())
    {
        mpPlayerToolBar->Restore();
    }
}

int FSMediaPlayer::Duration()
{
    return mpPlayerToolBar->Duration();
}

bool FSMediaPlayer::HaveSource()
{
    return mpPlayerToolBar->HaveSource();
}

bool FSMediaPlayer::IsPlaying()
{
	return mpControl->IsPlaying();
}



void FSMediaPlayer::SetChunkHeight(int height)
{
    mpPlayerToolBar->SetChunkHeight(height);
}

void FSMediaPlayer::Update()
{
    mpControl->Update();
}

void FSMediaPlayer::GetSnapShortImg(QImage & image)
{
    mpMediaPlayerView->GetSnapShortImg(image);
}

void FSMediaPlayer::OnFullScreen()
{
    mpFullScreenControl->FullScreen();
    if (!mpFullScreenControl->IsFullScreen())
    {
        Q_ASSERT(parentWidget());
        parentWidget()->setGeometry(QRect(parentWidget()->geometry().topLeft(), parentWidget()->geometry().size() + QSize(1, 1)));
        parentWidget()->setGeometry(QRect(parentWidget()->geometry().topLeft(), parentWidget()->geometry().size() + QSize(-1, -1)));

    }
    else
    {
        mpPlayerToolBar->setVisible(true);
        mpPlayerToolBar->raise();
    }
}

void FSMediaPlayer::resizeEvent(QResizeEvent *e)
{
    mpMediaPlayerView->setGeometry(0, 0, width(), height() - mpPlayerToolBar->height());
    mpPlayerToolBar->setGeometry(0, height() - mpPlayerToolBar->height(), width(), mpPlayerToolBar->height());
    mpFullScreenControl->ResizeEvent(mpMediaPlayerView, rect());
    mpPlayerToolBar->raise();
}

void FSMediaPlayer::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        if (mpFullScreenControl->IsFullScreen())
        {
            mpPlayerToolBar->DblMouseBtnFullScreen();
            return;

        }
    }
    return QWidget::keyPressEvent(e);
}

void FSMediaPlayer::mouseMoveEvent(QMouseEvent *e)
{
    mpFullScreenControl->MayShowToolBar(rect(), e->pos());
    QWidget::mouseMoveEvent(e);
}



void FSMediaPlayer::SetShortCut()
{
    m_pScStop = new QShortcut(QKeySequence(Qt::Key_Slash + Qt::ControlModifier), this);
    m_pScStop->setContext(Qt::WindowShortcut);
    connect(m_pScStop, &QShortcut::activated, [this]{
        if (!HaveSource() || !isVisible())
            return;
        Stop();
    });

    m_pScFullScrn = new QShortcut(QKeySequence("Alt+Return"), this);
    m_pScFullScrn->setContext(Qt::WindowShortcut);
    connect(m_pScFullScrn, &QShortcut::activated, [this]{
        if (!HaveSource() || !isVisible() || !mpPlayerToolBar->isEnabled())
            return;
        if (!mpFullScreenControl->IsFullScreen())
            mpPlayerToolBar->DblMouseBtnFullScreen();
    });
}

///////////////////////////////////////FSMediaPlayerView////////////////////////////////////////////////
FSMediaPlayerView::FSMediaPlayerView(QWidget *pParent)
    :QWidget(pParent)
    , m_bClicked(false)
    , m_PlayBorderWidth(1)
    , m_BorderColor(Qt::black)
    , m_MouseEvent(QEvent::Type::MouseButtonPress, QPointF(0, 0), Qt::MouseButton::LeftButton,
    Qt::MouseButton::LeftButton, Qt::KeyboardModifier::ControlModifier)
{
    setMouseTracking(true);
    mpMediaPlayerView = new MediaPlayerView(this);
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, [this]{
        m_pTimer->stop();
        if (m_bClicked)
        {
            m_bClicked = false;
            emit MousePreesEvent(&m_MouseEvent);
        }
    });
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("FSCommonLib_IFSMediaPlayer");
    
}

FSMediaPlayerView::~FSMediaPlayerView()
{

}

IMediaPlayerControl * FSMediaPlayerView::GetMediaPlayerControl()
{
    return mpMediaPlayerView->GetMediaPlayerControl();
}

void FSMediaPlayerView::mousePressEvent(QMouseEvent *e)
{
    if (!m_bClicked)
    {           
        m_pTimer->start(300);  
        m_bClicked = true;
        m_MouseEvent = *e;
    }    
    return QWidget::mousePressEvent(e);
}

void FSMediaPlayerView::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_bClicked)
    {
        m_bClicked = false;
        m_pTimer->stop();
        emit MouseDoubleClicked();
    }
}

void FSMediaPlayerView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(m_PlayBorderWidth);
    pen.setColor(m_BorderColor);
    painter.setPen(pen);
    painter.drawRect(m_PlayBorderWidth / 2, m_PlayBorderWidth / 2, width() - m_PlayBorderWidth, height() - m_PlayBorderWidth);
}

void FSMediaPlayerView::resizeEvent(QResizeEvent *e)
{
    QRect rc = rect();
    rc.adjust(m_PlayBorderWidth, m_PlayBorderWidth, -m_PlayBorderWidth, -m_PlayBorderWidth);
    mpMediaPlayerView->setGeometry(rc);
}

void FSMediaPlayerView::SetPlayerBorderWidth(int w)
{
    m_PlayBorderWidth = w;
    mpMediaPlayerView->SetBorderWidth(0);
}

void FSMediaPlayerView::SetPlayerBorderColor(QColor color)
{
    m_BorderColor = color;
}

void FSMediaPlayerView::GetSnapShortImg(QImage & image)
{
    if (mpMediaPlayerView->GetMediaPlayerControl()->HaveSource())
    {
        auto tempFrame = mpMediaPlayerView->GetMediaPlayerControl()->CaptureCurrentImage();
        Q_ASSERT(tempFrame);
        NLEMediaInfo Info;
        tempFrame->GetMediaInfo(&Info);
        image=QImage((uchar*)tempFrame->GetData(), Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
    }
}

///////////////////////////////////////FSPreviewEditView////////////////////////////////////////////////

FSPreviewEditView::FSPreviewEditView(IMediaPlayerControl *pMediaPlayerControl,QWidget *pParent /*= 0*/)
    :FSBaseWidget(pParent)
    ,m_BaseRenderSize(1920,1080)
    , m_PlayerBorderWidth(1)
    , m_PlayerBorderColor(Qt::black)
    , m_PlayerBackgroundColor(Qt::black)
{
    Q_ASSERT(pMediaPlayerControl);
    mpMediaPlayerControl = pMediaPlayerControl;
    mpEditHelperManager = new FSEditHelperManager(mpMediaPlayerControl, this);
    CreateUI();
    RetranslateUi();
    setAttribute(Qt::WA_InputMethodEnabled);
    setMouseTracking(true);    
    ConnectSignals();

    mpCaptionEnv = ICaptionEnvironment::GetInstance();
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("FSPreviewEditView");
}


FSPreviewEditView::~FSPreviewEditView()
{
    mpCaptionEnv->Release();
}

void FSPreviewEditView::SetTimeLineList(QList<FSNLETimeline*> &list, QList<FSNLETimeline*> &selectList, QList<FSNLETimeline*> &OherselectList, bool ScaleState)
{
    mTimeLineList = list;
    mpEditHelperManager->UpdateEidtHelperList(mTimeLineList,selectList,OherselectList,ScaleState);
    mpEditHelperManager->SetDocState(true);
    update();
}



IFSEditHelperManager * FSPreviewEditView::GetEditHelperManager()
{
    return mpEditHelperManager;
}

void FSPreviewEditView::ShowEditView(bool bShow)
{
    mpEditHelperManager->SetDocState(bShow);
    setVisible(bShow);   
}

bool FSPreviewEditView::IsSelectEditHelper()
{
    return mpEditHelperManager->isSelectEditHelper();
}

QList<IFSEditHelper*> FSPreviewEditView::GetEditHelperList(bool bSelecet)
{
    return mpEditHelperManager->GetEditHelpers(bSelecet);
}

void FSPreviewEditView::RemoveSelecetEditHelper()
{
    mpEditHelperManager->RemoveSelecetEditHelper();
}

void FSPreviewEditView::mouseMoveEvent(QMouseEvent *event)
{    
    mpEditHelperManager->DoMouseMoveEvent(event,GetMouseEventOffSet(event));
    QWidget::mouseMoveEvent(event);
}

void FSPreviewEditView::mousePressEvent(QMouseEvent *event)
{
    
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        mpEditHelperManager->DoMousePressEvent(event, GetMouseEventOffSet(event));
    }
    QWidget::mousePressEvent(event);
}

void FSPreviewEditView::mouseReleaseEvent(QMouseEvent *event)
{ 
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        mpEditHelperManager->DoMouseUpEvent(event, GetMouseEventOffSet(event));
    }
    QWidget::mouseReleaseEvent(event);
}

void FSPreviewEditView::mouseDoubleClickEvent(QMouseEvent *e)
{
    mpEditHelperManager->DoMouseDoubleClickEvent(e, GetMouseEventOffSet(e));
}

void FSPreviewEditView::keyPressEvent(QKeyEvent *event)
{
    mpEditHelperManager->DoKeyPressEvent(event);
    QWidget::keyPressEvent(event);
}

void FSPreviewEditView::paintEvent(QPaintEvent *e)
{
    if (!isVisible())
        return;
    QPainter painter(this);   
    double offsetx = 1.0*(width() - m_Width) / 2.0;
    double offsety = 1.0*(height() - m_Height) / 2.0;
    int nStart;
    if (mpMediaPlayerControl->HaveSource())
    {
        nStart = ::GetTickCount();
        //QImage img=GetImage();
        mpMediaPlayerControl->Update();
        qDebug() << "Update:" << ::GetTickCount() - nStart;
        //QThread::msleep(10);
        nStart = ::GetTickCount();
        NLEComPtr<INLEFrame> pFrame = mpMediaPlayerControl->CaptureCurrentImage();
        qDebug() << "Get:"<<::GetTickCount() - nStart;
        nStart = ::GetTickCount();
        NLEMediaInfo Info;
        pFrame->GetMediaInfo(&Info);
        QImage img = QImage((uchar*)pFrame->GetData(), Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
		
        qDebug() <<"Get:"<< ::GetTickCount() - nStart;
        nStart = ::GetTickCount();
        if (mpMediaPlayerControl->GetTimeline()->GetClipCount() == 0)
        {
            QSize  sz = mpMediaPlayerControl->GetTimeline()->GetRenderSize();
            img = QImage(sz.width(), sz.height(), QImage::Format_ARGB32);
            img.fill(m_PlayerBackgroundColor);
        }           
        painter.drawImage(QPoint(offsetx,offsety), img);
        QPen pen;
        pen.setWidth(m_PlayerBorderWidth);
        pen.setColor(m_PlayerBorderColor);
        painter.setPen(pen);
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.drawRect(QRect(m_PlayerBorderWidth/2,m_PlayerBorderWidth/2, width() - m_PlayerBorderWidth, height() - m_PlayerBorderWidth));
    }
    mpEditHelperManager->DoPaintEvent(painter, QPoint(offsetx,offsety));  
    //qDebug() << "Paint:" << ::GetTickCount() - nStart;
}

void FSPreviewEditView::inputMethodEvent(QInputMethodEvent *e)
{
    mpEditHelperManager->DoinputMethodEvent(e);
    QWidget::inputMethodEvent(e);
}

void FSPreviewEditView::resizeEvent(QResizeEvent *e)
{   
    if (!mpMediaPlayerControl->IsPlaying())
        SetRenderSize(size());
    int baseH, baseW,w,h;
    if (width()*m_BaseRenderSize.height() > height()*m_BaseRenderSize.width())
    {
        baseH = m_BaseRenderSize.height() - 2 * m_PlayerBorderWidth;
        baseW = 1.0*baseH*m_BaseRenderSize.width() / m_BaseRenderSize.height();
    }
    else
    {
        baseW = m_BaseRenderSize.width() - 2 * m_PlayerBorderWidth;
        baseH = 1.0*baseW*m_BaseRenderSize.height()/m_BaseRenderSize.width();
    }
    h = m_Height;
    w = m_Width;
    mpEditHelperManager->DoResizeEvent(e, QSizeF(1.0*w / baseW, 1.0*h / baseH));
    QWidget::resizeEvent(e);   
}

void FSPreviewEditView::SetBaseRenderSize(QSize size)
{    
     m_BaseRenderSize = size;
}



void FSPreviewEditView::SetRenderSize(QSize widgetSize)
{
    int tt1 = widgetSize.width();
    widgetSize = widgetSize - QSize(2 * m_PlayerBorderWidth, 2 * m_PlayerBorderWidth);
    m_Width = widgetSize.width();
    m_Height = widgetSize.height();
    if (mpMediaPlayerControl->HaveSource())
    {
        if (qAbs(m_Width) >1e-2&& qAbs(m_Height)>1e-2)
        {
            mpMediaPlayerControl->GetTimeline()->SetRenderSize(m_Width,m_Height);
        }
    }
}

void FSPreviewEditView::ClearEditHelper()
{
    mpEditHelperManager->ClearEditHelper();
}


void FSPreviewEditView::GetPreviewPath(QList<FSPainterPath> &pathList)
{
    double offsetx = 1.0*(width() - int(m_Width)) / 2;
    double offsety = 1.0*(height() - int(m_Height)) / 2;
    mpEditHelperManager->GetControlPath(pathList, QPointF(offsetx, offsety));
}


void FSPreviewEditView::GetPreviewGuidesPath(QList<FSPainterPath > &GuidesPath, int Width, int Height,QPointF offSet)
{
    double offsetx = 1.0*(width() - int(m_Width)) / 2;
    double offsety = 1.0*(height() - int(m_Height)) / 2;
    QList<QPointF> hPointList, hPointList1, vPointList, vPointList1;
    mpEditHelperManager->GetGuidesPointList(hPointList, vPointList, QPointF(offsetx, offsety));
    mpEditHelperManager->GetMainGuidesPointList(hPointList1, vPointList1, QPointF(offsetx, offsety));
    FSPainterPath guidesPath, mainGuidesPath;
    guidesPath.SetNeedFill(false);
    guidesPath.SetPathColor(QColor("#7f9caf"));
    mainGuidesPath.SetNeedFill(false);
    mainGuidesPath.SetPathColor(QColor("#00ebff"));
    for (int i = 0; i < hPointList.size(); i++)
    { 
        guidesPath.GetPath().moveTo(0, hPointList.at(i).y() + offSet.y());
        guidesPath.GetPath().lineTo(QPointF(Width, hPointList.at(i).y() + offSet.y()));
    }

    for (int i = 0; i < hPointList1.size(); i++)
    {
        mainGuidesPath.GetPath().moveTo(0, hPointList1.at(i).y() + offSet.y());
        mainGuidesPath.GetPath().lineTo(QPointF(Width, hPointList1.at(i).y() + offSet.y()));
    }
    for (int i = 0; i < vPointList.size(); i++)
    {
        guidesPath.GetPath().moveTo(vPointList.at(i).x() + offSet.x(), 0);
        guidesPath.GetPath().lineTo(QPointF(vPointList.at(i).x() + offSet.x(), Height));
    }
    for (int i = 0; i < vPointList1.size(); i++)
    {
        mainGuidesPath.GetPath().moveTo(vPointList1.at(i).x() + offSet.x(), 0);
        mainGuidesPath.GetPath().lineTo(QPointF(vPointList1.at(i).x() + offSet.x(), Height));
    }
    GuidesPath << guidesPath << mainGuidesPath;
}

QPoint FSPreviewEditView::GetPreiewOffsetPoint()
{
    double offsetx = 1.0*(width() - int(m_Width)) / 2;
    double offsety = 1.0*(height() - int(m_Height)) / 2;
    return  QPointF(offsetx, offsety).toPoint();
}


void FSPreviewEditView::SetPlayerBorderWidth(int w)
{
    m_PlayerBorderWidth = w;
    mpEditHelperManager->SetBorderWidth(m_PlayerBorderWidth);
}

int FSPreviewEditView::GetPlayerBorderWidth()
{
    return m_PlayerBorderWidth;
}

void FSPreviewEditView::SetPlayerBorderColor(QColor color)
{
    m_PlayerBorderColor = color;
}

void FSPreviewEditView::SetPlayerBackgroundColor(QColor color)
{
    m_PlayerBackgroundColor = color;
    auto c = qRgba(color.red(), color.green(), color.blue(), color.alpha());
    mpMediaPlayerControl->GetTimeline()->GetPropW()->SetInt(NLEKey::Render::kRender_BackgroundColor, c);
    mpEditHelperManager->sigRepaintWidget();
}

int FSPreviewEditView::GetSelectItemNum()
{
    return mpEditHelperManager->GetSelecetItemNum();
}

void FSPreviewEditView::MultiSelectByRect(QRect rc)
{
    double offsetx = 1.0*(width() - int(m_Width)) / 2;
    double offsety = 1.0*(height() - int(m_Height)) / 2;
    mpEditHelperManager->MultiSelectByRect(QRect(rc.topLeft() - QPoint(offsetx, offsety), rc.size()));
}

QPointF  FSPreviewEditView::GetMouseEventOffSet(QMouseEvent *event)
{
    double offsetx = m_PlayerBorderWidth;
    double offsety = m_PlayerBorderWidth;
    if (width() *m_Height> height()*m_Width)
    {
        offsetx = 1.0*(width() - int(m_Width)) / 2;
    }
    else
    {
        offsety = 1.0*(height() - int(m_Height)) / 2;
    }
    return QPointF(offsetx, offsety);
}

void FSPreviewEditView::RetranslateUi()
{
    
}



QImage FSPreviewEditView::GetImage()
{
    INLERender *pVideoRender = NULL;
    HRESULT hr = mpMediaPlayerControl->GetTimeline()->GetRender(&pVideoRender, NULL);

    QSize  sz = mpMediaPlayerControl->GetTimeline()->GetRenderSize();
    QImage img(sz.width(), sz.height(), QImage::Format_ARGB32);
    img.fill(m_PlayerBackgroundColor);

    if (!SUCCEEDED(hr)) return img;

    if (!pVideoRender) return img;

    NLEComPtr<INLEFrame> pVideoFrame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFrame(sz.width(), sz.height());
    if (!pVideoFrame) return img;
 
    bool bRender = true;
    bRender &= SUCCEEDED(pVideoRender->Prepare(pVideoFrame));
    bRender &= SUCCEEDED(pVideoRender->Render(pVideoFrame));

    if (!bRender) return img;


    NLEMediaInfo Info;
    pVideoFrame->GetMediaInfo(&Info);
    img=QImage((uchar*)pVideoFrame->GetData(), Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
    return img;
}

void FSPreviewEditView::ConnectSignals()
{
    
}


void FSPreviewEditView::CreateUI()
{

}



///////////////////////////////////////FSPropertyEditPlayerView////////////////////////////////////////////////
FSPropertyEditPlayerView::FSPropertyEditPlayerView(QWidget *pParent)
    :QWidget(pParent)
{
    CreateUI();
    ConnectSignals();
    resize(750, 400);
}

FSPropertyEditPlayerView::~FSPropertyEditPlayerView()
{

}

FSEditPlayerView * FSPropertyEditPlayerView::GetEditPlayerView()
{
    return m_pEditPlayerView;
}

void FSPropertyEditPlayerView::CreateUI()
{
    m_pEditPlayerView = new FSEditPlayerView(this);
    m_pPropertyView   = new FSPropertyView(this);
}

void FSPropertyEditPlayerView::ConnectSignals()
{
    
}

void FSPropertyEditPlayerView::resizeEvent(QResizeEvent *e)
{
    m_pEditPlayerView->setGeometry(0, 0, width() - 260, height());
    m_pPropertyView->setGeometry(m_pEditPlayerView->geometry().right(), 0, 260, height());
}



//////////////////////////////////////////////////////////////////////////////////////
FSMediaScrollArea::FSMediaScrollArea(FSEditPlayerView *pWidget, QWidget *pParent)
    :QScrollArea(pParent)
    , m_BaseSize(1920, 1080)
    , m_pEditPlayerView(pWidget)
    , m_PointLast(0,0)
    , m_bReset(true)
    , m_bFit(true)
{
    Q_ASSERT(pWidget);
    m_pViewPort = new FSViewPort(this,m_pEditPlayerView);
    m_pWidget = new QWidget(this);    
    setWidget(m_pWidget);
    setFrameShape(QFrame::NoFrame);
    m_pVL = new QVBoxLayout(m_pWidget);
    m_pVL->addWidget(pWidget);
    m_pVL->setMargin(0);
    m_pVL->setSpacing(0);

    m_pWidget->setObjectName("FSScrollArea_m_pWidget");
    verticalScrollBar()->setObjectName("FSMediaScrollArea_verticalScrollBar");
    horizontalScrollBar()->setObjectName("FSMediaScrollArea_horizontalScrollBar");
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pWidget->setFixedSize(m_BaseSize.width(), m_BaseSize.height());
    setObjectName("FSMediaScrollArea");
    connect(m_pEditPlayerView, &FSEditPlayerView::ShowEditViewSig, this, [&](bool bShow){m_pViewPort->setVisible(bShow); });
    connect(m_pEditPlayerView->GetMediaPlayerControl(), &IMediaPlayerControl::PlayingCallback, this, [&](){m_pViewPort->setVisible(false); });
    connect(m_pEditPlayerView, &FSEditPlayerView::sigZoom, this, &FSMediaScrollArea::slotZoom);
    connect(m_pViewPort, &FSViewPort::sigMouseMoved, this, &FSMediaScrollArea::slotMouseMoved);
    connect(m_pViewPort, &FSViewPort::sigMousePressed, this, &FSMediaScrollArea::slotMousePressed);
    connect(m_pViewPort, &FSViewPort::sigMouseReleased, this, &FSMediaScrollArea::slotMouseReleased);
    ConnectUpdateViewPort(true);
    m_PonitMarkPoint = m_pWidget->geometry().center();
    setWidgetResizable(true);
}

FSMediaScrollArea::~FSMediaScrollArea()
{

}

void FSMediaScrollArea::SetScaleSize(bool bFit, float scale)
{
    m_bFit = bFit;
    blockSignals(true);
    if (!m_bFit)
    {
        QSizeF size = QSizeF(m_BaseSize.width()*scale, m_BaseSize.height()*scale);
        QSize temp = size.toSize();
        temp.setWidth(size.width() + 2 * m_pEditPlayerView->GetPlayerBorderWidth());
        temp.setHeight(size.height() + 2 * m_pEditPlayerView->GetPlayerBorderWidth());
        QSize temp1 = rect().size();
        QTransform transform = QTransform().scale(1.0*size.width() / m_pWidget->width(), 1.0*size.height() / m_pWidget->height());
        QPointF PT = transform.map(m_pWidget->pos());
        takeWidget();
        m_pWidget->setFixedSize(temp.width(), temp.height());
        setWidget(m_pWidget);
        QPoint pt = transform.map(m_PonitMarkPoint.toPoint());
        if (m_bReset)
        {
            transform = QTransform().translate((temp1.width() - m_pWidget->width()) / 2, (temp1.height() - m_pWidget->height()) / 2);
            m_pWidget->move(transform.m31(), transform.m32());
            m_bReset = false;
        }
        else
        {
            pt = m_PonitMarkPoint.toPoint() - pt;
            m_pWidget->move(PT.x() + pt.x(), PT.y() + pt.y());
        }
    }
    else
    {
        takeWidget();
        int w, h;
        int w1 = width() - 12 * m_pEditPlayerView->GetPlayerBorderWidth();
        int h1 = height() - 12 * m_pEditPlayerView->GetPlayerBorderWidth();
        if (w1*m_BaseSize.height() > h1*m_BaseSize.width())
        {
            h = h1 - m_pEditPlayerView->GetPlayerBorderWidth() * 2;
            w = 1.0*h*m_BaseSize.width() / m_BaseSize.height();
        }
        else
        {
            w = w1 - m_pEditPlayerView->GetPlayerBorderWidth() * 2;
            h = 1.0*w*m_BaseSize.height() / m_BaseSize.width();
        }
        w += m_pEditPlayerView->GetPlayerBorderWidth() * 2;
        h += m_pEditPlayerView->GetPlayerBorderWidth() * 2;
        m_pWidget->setFixedSize(w,h);
        setWidget(m_pWidget);
        m_pWidget->move((width()-w)/2, (height()-h)/2);
    }
    m_pEditPlayerView->GetMenuControl()->UpdataZoomMenuCheckState(bFit, scale);
    blockSignals(false);
    UpdateViewPort(0);
}

void FSMediaScrollArea::SetBaseRenderSize(QSize size)
{
    if ((!size.isNull()) && (size.isValid()))
    {
        m_pEditPlayerView->SetBaseRenderSize(size);
        if (m_bFit)
        {
            m_BaseSize = size;
            emit sigMenuChangeScale("Fit");
        }
        else
        {
            QSize Size = this->size() - QSize(2 * m_pEditPlayerView->GetPlayerBorderWidth(), 2 * m_pEditPlayerView->GetPlayerBorderWidth());
            double scale = 1.0*Size.width() / m_BaseSize.width();
            m_BaseSize = size;
            emit sigMenuChangeScale(QString("%1%2").arg((int)qRound(scale*100)).arg("%"));
        }
        
    }
}

void FSMediaScrollArea::ResetMarkPoint()
{
    m_PonitMarkPoint = rect().center();
    m_bReset = true;
}

void FSMediaScrollArea::wheelEvent(QWheelEvent *e)
{
    QPointF offsetPt = m_pEditPlayerView->GetPreviewEditView()->GetPreiewOffsetPoint();
    QRect rc = m_pWidget->geometry();
    rc.adjust(qRound(offsetPt.x()), qRound(offsetPt.y()), -qRound(offsetPt.x()), -qRound(offsetPt.y()));
    if (m_pEditPlayerView->IsPlaying())
        return;
    if (qAbs(1.0*rc.width() / m_BaseSize.width() - 0.1) < 1e-4&&e->delta() < 0)
        return;
    if (qAbs(1.0*rc.width() / m_BaseSize.width() - 2) < 1e-4&&e->delta() > 0)
        return;
    m_PonitMarkPoint = mapFromGlobal(e->globalPos());   
    if (!rc.contains(m_PonitMarkPoint.toPoint()))
    {

        if (m_pViewPort->rect().intersects(rc))
        {
            if (m_PonitMarkPoint.x() < rc.left())
            {
                m_PonitMarkPoint.setX(rc.left());
            }
            else if (m_PonitMarkPoint.x() > rc.right())
            {
                m_PonitMarkPoint.setX(rc.right());
            }

            if (m_PonitMarkPoint.y() < rc.top())
            {
                m_PonitMarkPoint.setY(rc.top());
            }
            else if (m_PonitMarkPoint.y() > rc.bottom())
            {
                m_PonitMarkPoint.setY(rc.bottom());
            }
            m_bReset = false;
        }
        else
        {
            ResetMarkPoint();
        }           
    }
    emit sigChangeScale(e->delta() < 0);
}



void FSMediaScrollArea::resizeEvent(QResizeEvent *e)
{
    m_pViewPort->setGeometry(this->rect());
    if (m_bFit)
    {
        int w, h;
        if (width()*m_BaseSize.height() > height()*m_BaseSize.width())
        {
            h = height() - m_pEditPlayerView->GetPlayerBorderWidth() * 12 ;
            w = 1.0*h*m_BaseSize.width() / m_BaseSize.height();
        }
        else
        {
            w = width() - m_pEditPlayerView->GetPlayerBorderWidth() * 12;
            h = 1.0*w*m_BaseSize.height() / m_BaseSize.width();
        }
        w += m_pEditPlayerView->GetPlayerBorderWidth() * 2;
        h += m_pEditPlayerView->GetPlayerBorderWidth() * 2;
        m_pWidget->setFixedSize(w, h);
        setWidget(m_pWidget);
        m_pWidget->move((width() - w) / 2, (height() - h) / 2);
    }
}

void FSMediaScrollArea::slotMousePressed(QPointF pos)
{
    m_PointLast = pos;
}

void FSMediaScrollArea::slotMouseReleased(QPointF pos)
{

}

void FSMediaScrollArea::slotMouseMoved(QPointF pos)
{
    QPointF offSet = pos - m_PointLast;
    m_PointLast = pos;
    QPoint pt = m_pWidget->pos() + offSet.toPoint();
    m_pWidget->move(pt.x(),pt.y());
    UpdateViewPort(0);

}

void FSMediaScrollArea::scrollContentsBy(int x, int y)
{
    return;
}

bool FSMediaScrollArea::event(QEvent *e)
{
    if (e->type() == QEvent::StyleChange || e->type() == QEvent::LayoutRequest) {
        return true;
    }
   return QScrollArea::event(e);
}

void FSMediaScrollArea::UpdateViewPort(int value)
{
    if (m_pEditPlayerView->GetPreviewEditView()->isVisible())
        m_pViewPort->update(0, 0, m_pViewPort->width(), m_pViewPort->height());
}

void FSMediaScrollArea::ConnectUpdateViewPort(bool bConnect)
{
    if (bConnect)
    {
        connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FSMediaScrollArea::UpdateViewPort);
        connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &FSMediaScrollArea::UpdateViewPort);
    }
    else
    {
        disconnect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FSMediaScrollArea::UpdateViewPort);
        disconnect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &FSMediaScrollArea::UpdateViewPort);
    }
}

void FSMediaScrollArea::slotZoom(bool bFit, QString scale)
{
    m_bFit = bFit;
    emit sigMenuChangeScale(scale);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
FSViewPort::FSViewPort(QWidget *pParent, FSEditPlayerView *pEditPlayerView)
    :QLabel(pParent)
    , m_pPreviewWidget(pEditPlayerView->GetPreviewEditView())
    , m_pEditPlayerView(pEditPlayerView)
    , m_BoxFillColor({255,255,255})
    , m_MultiSelectRect(QPoint(0,0),QSize(0,0))
    , m_bEnableDrawMultiSelectRect(false)
    , m_bPressed(false)
    , m_bRightPressed(false)
    , m_bRightLongPressed(false)
{    
    setMouseTracking(true);
    setAcceptDrops(true);
    setAttribute(Qt::WA_InputMethodEnabled);
    connect(m_pEditPlayerView, &FSEditPlayerView::sigSpaceLongPress, this, [&](bool bPress)
    {
        QPoint pt=mapFromGlobal(QCursor::pos());
        QRect rc = rect();
        if (!rect().contains(pt))
        {
            setCursor(Qt::CustomCursor);
            m_bPressed = false;
            m_pEditPlayerView->GetShortCutControl()->SetSpacePressing(false);
        }
        else
        {
            setCursor(bPress ? Qt::ClosedHandCursor : m_bRightLongPressed ? Qt::ClosedHandCursor : Qt::CustomCursor);
            if (!bPress)
            {
                m_bPressed = bPress;
            }
        }        
    });
    connect(static_cast<FSEditHelperManager *>(m_pPreviewWidget->GetEditHelperManager()), &FSEditHelperManager::sigRepaintWidget, this, [&](){update();});
}

FSViewPort::~FSViewPort()
{

}

void FSViewPort::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int start = ::GetTickCount();
    QList<FSPainterPath> list;
    m_pPreviewWidget->GetPreviewPath(list);
    QPoint pt = m_pPreviewWidget->mapToGlobal(m_pPreviewWidget->pos());
    QPoint pt1 = mapToGlobal(pos());
    QPoint pt2 = pt - pt1;
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen p;
    for (auto item:list)
    {
        item.GetPath().translate(pt2);
        p.setWidthF(item.GetPathWidth());
        p.setColor(item.GetPathColor());
        p.setStyle(item.GetStyle());
        painter.setPen(p);
        painter.drawPath(item.GetPath());
        if (item.GetRichColor())
        {
            p.setWidthF(1);
            p.setColor(Qt::black);
            painter.setPen(p);
            painter.drawPath(item.GetPath());
        }       
        if (item.IsNeedFill())
        {
            painter.fillPath(item.GetPath(), item.GetPathFillColor());
        }
    }
    list.clear();
    m_pPreviewWidget->GetPreviewGuidesPath(list, width(), height(),pt2);
    painter.setRenderHint(QPainter::Antialiasing, false);
    for (auto item : list)
    {
        p.setWidth(1);
        p.setColor(item.GetPathColor());
        p.setStyle(item.GetStyle());
        painter.setPen(p);
        painter.drawPath(item.GetPath());
        if (item.IsNeedFill())
        {
            painter.fillPath(item.GetPath(), item.GetPathFillColor());
        }
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (m_bEnableDrawMultiSelectRect)
    {
        p.setColor(QColor(252, 252, 252));
        p.setStyle(Qt::DashLine);
        p.setWidthF(1);
        painter.setPen(p);
        painter.drawRect(m_MultiSelectRect);
    }
    
    //qDebug() << "paintCon:" << GetTickCount() - start;
}

void FSViewPort::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint pt = m_pPreviewWidget->mapFromGlobal(ev->globalPos());
    QMouseEvent event(ev->type(),pt,ev->button(),ev->buttons(),ev->modifiers());
    if (m_pEditPlayerView->isPressingSpace())
    {
        if (m_bPressed)
            emit sigMouseMoved(ev->pos());
        else if (m_bRightLongPressed)
        {
            if (m_bRightPressed)
            {
                emit sigMousePressed(ev->pos());
                m_bRightPressed = false;
            }
            emit sigMouseMoved(ev->pos());
        }
    }
    else
    {
        if (!m_bRightLongPressed)
        {
            QApplication::sendEvent(m_pPreviewWidget, &event);
            if (m_pPreviewWidget->cursor().shape() == Qt::CursorShape::CrossCursor)
            {
                QPixmap pix(":/FSCommonLib/Resources/light/Cursor.png");
                setCursor(QCursor(pix.scaled(pix.width()*gGetDevicePixelRatio(), pix.height()*gGetDevicePixelRatio(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
            }
            else
            {
                setCursor(m_pPreviewWidget->cursor());
            }
            if (m_bEnableDrawMultiSelectRect)
            {
                m_MultiSelectRect.setBottomRight(ev->pos());
                m_pPreviewWidget->MultiSelectByRect(QRect(m_pPreviewWidget->mapFromGlobal(mapToGlobal(m_MultiSelectRect.topLeft())), m_MultiSelectRect.size()));
            }
        }
        else
        {
            if (m_bRightPressed)
            {                
                emit sigMousePressed(ev->pos());
                m_bRightPressed = false;
            }
            emit sigMouseMoved(ev->pos());
        }        
    }     
}

void FSViewPort::mousePressEvent(QMouseEvent *ev)
{
    QPoint pt = m_pPreviewWidget->mapFromGlobal(ev->globalPos());
    QMouseEvent event(ev->type(), pt, ev->button(), ev->buttons(), ev->modifiers());    
    if (ev->button() == Qt::MouseButton::LeftButton)
    {
        if (m_pEditPlayerView->isPressingSpace())
        {
            m_bPressed = true;
            emit sigMousePressed(ev->pos());
        }
        else
        {
            QApplication::sendEvent(m_pPreviewWidget, &event);
            m_bEnableDrawMultiSelectRect = m_pPreviewWidget->GetSelectItemNum() == 0;
            if (m_bEnableDrawMultiSelectRect)
            {
                m_MultiSelectRect.setTopLeft(ev->pos());
                m_MultiSelectRect.setBottomRight(ev->pos());
                m_pEditPlayerView->GetPlayerEditorManager()->UnselectAll();
            }
        }
    }
    else if (ev->button() == Qt::MouseButton::RightButton || ev->button() == Qt::MouseButton::MidButton)
    {
        m_bRightPressed = true;
        QApplication::sendEvent(m_pPreviewWidget, &event);
        if (ev->button() == Qt::MouseButton::RightButton)
        {
            QTimer::singleShot(200, this, [&](){
                if (m_bRightPressed)
                {
                    m_bRightLongPressed = true;
                    setCursor(Qt::ClosedHandCursor);
                }
                else
                {
                    m_bRightLongPressed = false;
                }
            });
        }
        else if (ev->button() == Qt::MouseButton::MidButton){
            QTimer::singleShot(210, this, [&](){
                if (m_bRightPressed)
                {
                    m_bRightLongPressed = true;
                    setCursor(Qt::ClosedHandCursor);
                }
                else
                {
                    m_bRightLongPressed = false;         
                    m_pEditPlayerView->sigZoom(true, "Fit");
                    m_pEditPlayerView->GetMenuControl()->UpdataZoomMenuCheckState(true, 1);
                }
            });
        }       
    }
}

void FSViewPort::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint pt = m_pPreviewWidget->mapFromGlobal(ev->globalPos());
    QMouseEvent event(ev->type(), pt, ev->button(), ev->buttons(), ev->modifiers());
    if (ev->button() == Qt::MouseButton::LeftButton)
    {
        if (!m_pEditPlayerView->isPressingSpace()||!rect().contains(ev->pos()))
        {
            QApplication::sendEvent(m_pPreviewWidget, &event);
            if (m_bEnableDrawMultiSelectRect)
            {
                IPlayerEditorManager *pPlayerManager = m_pEditPlayerView->GetPlayerEditorManager();
                QList<IFSEditHelper *> list = m_pPreviewWidget->GetEditHelperList(true);
                QList<FSNLETimeline *> timeLineList;
                for (auto item : list)
                {
                    timeLineList.push_back(item->GetTimeLine());
                }
                pPlayerManager->SetSelected(timeLineList);
            }
        }
        else
        {
            emit sigMouseReleased(ev->pos());
        }
    }
    else if (ev->button() == Qt::MouseButton::RightButton || ev->button() == Qt::MouseButton::MidButton)
    {
        if (m_bRightLongPressed)
            emit sigMouseReleased(ev->pos());
        else
        {          
            QApplication::sendEvent(m_pPreviewWidget, &event);
        }
    }
    m_bRightLongPressed = false;
    m_bRightPressed = false;
    m_bEnableDrawMultiSelectRect = false;
    m_bPressed = false;
    update();
    setCursor(Qt::CustomCursor);
}

void FSViewPort::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QPoint pt = m_pPreviewWidget->mapFromGlobal(ev->globalPos());
    QMouseEvent event(ev->type(), pt, ev->button(), ev->buttons(), ev->modifiers());
    QApplication::sendEvent(m_pPreviewWidget, &event);
}

void FSViewPort::keyPressEvent(QKeyEvent *e)
{
    QApplication::sendEvent(m_pPreviewWidget,e);
}


void FSViewPort::inputMethodEvent(QInputMethodEvent *e)
{
    QApplication::sendEvent(m_pPreviewWidget, e);
}

void FSViewPort::wheelEvent(QWheelEvent *e)
{
    QApplication::sendEvent(m_pPreviewWidget, e);
}

void FSViewPort::dragEnterEvent(QDragEnterEvent *e)
{
    auto mimeData = e->mimeData();
    if (mimeData->hasFormat("MatterView_BaseItem"))
    {
        e->accept();
    }
    else if (mimeData->hasFormat("MatterView_MouseEffect"))
    {
        if(m_pPreviewWidget->GetEditHelperManager()->CanAddMouseEffect())
        {
            e->accept();
        }
    }
   
}

void FSViewPort::dragMoveEvent(QDragMoveEvent *e)
{
    QPoint qPt = m_pPreviewWidget->mapFromGlobal(mapToGlobal(e->pos()));
    auto mimeData = e->mimeData();
    if (mimeData->hasFormat("MatterView_MouseEffect"))
    {
        static_cast<FSEditHelperManager *>(m_pPreviewWidget->GetEditHelperManager())->UpdataDragEnterPathColor(qPt, m_pPreviewWidget->GetPreiewOffsetPoint());
        update();
    }

}

void FSViewPort::dropEvent(QDropEvent *e)
{
    QList<FSNLETimeline *> list;
    auto mimeData = e->mimeData();
    if (mimeData->hasFormat("MatterView_BaseItem"))
    {
        QList< FSNLETimeline *> *ptimelineList = reinterpret_cast<QList< FSNLETimeline *>*>(mimeData->property("MatterView_BaseItem").value < qptrdiff >());
        Q_ASSERT(ptimelineList);
        for (auto ptimeline:*ptimelineList)
        {
            if (ptimeline->GetType() == TimelineType_Complex)  //录制一路流的timeline的情况
            {
				FSNLETimeline *ptimelineTemp = ptimeline;
                list.push_back(ptimelineTemp);
                QList<NLEComPtr<INLETimelineBase>> timelines = NLEWraper::GetInstance().GetPlayerMaterProvider()->GetSubTimelines(ptimelineTemp->GetSrc());
                FSNLETimeline *pCameraTl = nullptr;
                FSNLETimeline *pDesktopGameTl = nullptr;
                for (auto pitem : timelines)
                {
                    NLEComPtr<INLEProperties> pProp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(pitem);
                    auto tl=new FSNLETimeline(pitem);
                    if (tl->GetType() == TimelineType_VideoAudio || tl->GetType() == TimelineType_Video)
                    {
                        NLEPoint pt;
                        QPoint qPt = m_pPreviewWidget->mapFromGlobal(mapToGlobal(e->pos())) - m_pPreviewWidget->GetPreiewOffsetPoint();
                        pt.X = qPt.x();
                        pt.Y = qPt.y();
                        pProp->SetPoint(Drag_AddPos, pt);
                    }
                }
            }
            else if (ptimeline->GetType() == TimelineType_Audio ||
                ptimeline->GetType() == TimelineType_VideoAudio ||
                ptimeline->GetType() == TimelineType_Annotaions_Caption ||
                ptimeline->GetType() == TimelineType_Annotaions_Element ||
                ptimeline->GetType() == TimelineType_Video ||
                ptimeline->GetType() == TimelineType_Image ||
                ptimeline->GetType() == TimelineType_Annotaions_Overlay)
            {
				FSNLETimeline *ptimelineTemp = ptimeline;
                if (ptimelineTemp->GetType() != TimelineType_Audio)
                {
                    NLEPoint pt;
                    QPoint qPt = m_pPreviewWidget->mapFromGlobal(mapToGlobal(e->pos())) - m_pPreviewWidget->GetPreiewOffsetPoint();
                    pt.X = qPt.x();
                    pt.Y = qPt.y();
                    ptimelineTemp->GetProp()->SetPoint(Drag_AddPos, pt);
                }
                list.push_back(ptimelineTemp);
            }
        }
        m_pEditPlayerView->AddDropTimeLineList(list, mapToGlobal(e->pos()));
    }
    else if (mimeData->hasFormat("MatterView_MouseEffect"))
    {
        QPoint qPt = m_pPreviewWidget->mapFromGlobal(mapToGlobal(e->pos()));
        if (mimeData->hasFormat("MatterView_MouseEffect_ClickSound"))
        {
            QString soundFile = mimeData->property("MatterView_MouseEffect_ClickSound_File").value<QString>();
            m_pEditPlayerView->AddDropMouseClickSound(soundFile, qPt, m_pPreviewWidget->GetPreiewOffsetPoint());
        }
        else if (mimeData->hasFormat("MatterView_MouseEffect_Filter"))
        {
            FSNLETimeline *timeline = reinterpret_cast<FSNLETimeline*>(mimeData->property("MatterView_MouseEffect_FilterTimeline").value<qptrdiff>());
            Q_ASSERT(timeline);
            m_pEditPlayerView->AddDropMouseFliter(timeline, qPt, m_pPreviewWidget->GetPreiewOffsetPoint());
        }
        else
        {
            Q_ASSERT(false);
        }
    }
	e->accept();
}


void FSViewPort::enterEvent(QEvent *)
{
    bool reg = m_pEditPlayerView->isPressingSpace();
    setCursor(reg? Qt::ClosedHandCursor : Qt::CustomCursor);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
FSPlayerMenuControl::FSPlayerMenuControl(QObject *pParent)
    :QObject(pParent)
{
    InitAction();
    RetranslateUi();
}

FSPlayerMenuControl::~FSPlayerMenuControl()
{

}

void FSPlayerMenuControl::EnableAction(int id, bool bEnable)
{
    m_ActionList.at(qBound(0, id, m_ActionList.size() - 1))->setEnabled(bEnable);
}

void FSPlayerMenuControl::CheckAction(int id)
{
    m_ActionList.at(qBound(0, id, m_ActionList.size() - 1))->setChecked(true);
}

void FSPlayerMenuControl::PopUp(QPointF pt)
{
    m_pMenuEdit->popup(pt.toPoint());
}

void FSPlayerMenuControl::RetranslateUi()
{
    QString text;
    for (auto item: m_ActionList)
    {
        switch (item->data().toInt())
        {
        case FSPlayerMenuControl::Action_Reset:
            text = QString(tr("Reset"));
            break;
        case FSPlayerMenuControl::Action_Copy:
            text = QString(tr("Copy"));
            break;
        case FSPlayerMenuControl::Action_Paste:
            text = QString(tr("Paste"));
            break;
        case FSPlayerMenuControl::Action_Cut:
            text = QString(tr("Cut"));
            break;
        case FSPlayerMenuControl::Action_Delete:
            text = QString(tr("Delete"));
            break;
        case FSPlayerMenuControl::Action_ProjectSetting:
            text = QString(tr("Project Settings"));
            break;
        case FSPlayerMenuControl::Action_PlayResolution_Full:
            text = QString(tr("Full"));
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_2:
            text = QString(("1/2"));
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_4:
            text = QString(("1/4"));
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_8:
            text = QString(("1/8"));
            break;
        case FSPlayerMenuControl::Action_PlayResolution_1_16:
            text = QString(("1/16"));
            break;
        case FSPlayerMenuControl::Action_Zoom_Fix:
            text = QString(tr("Fit"));
            break;
        case FSPlayerMenuControl::Action_Zoom_200:
            text = QString(("200%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_175:
            text = QString(("175%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_150:
            text = QString(("150%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_125:
            text = QString(("125%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_100:
            text = QString(("100%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_85:
            text = QString(("85%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_75:
            text = QString(("75%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_60:
            text = QString(("60%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_50:
            text = QString(("50%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_35:
            text = QString(("35%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_25:
            text = QString(("25%"));
            break;
        case FSPlayerMenuControl::Action_Zoom_10:
            text = QString(("10%"));
            break;
        default:
            break;
        }
        item->setText(text);
    }
    m_pMenuPlayResolution->setTitle(tr("Playback resolution"));
    m_pMenuZoom->setTitle(tr("Zoom"));
}

void FSPlayerMenuControl::UpdataZoomMenuCheckState(bool bfit, double scale)
{
    if (bfit)
        CheckAction(Action_Zoom_Fix);
    else{
        QString str = QString("%1%2").arg((int)(qRound(scale * 100))).arg("%");
        for (int i = Action_Zoom_100; i <= Action_Zoom_200; i++)
        {
            if (m_ActionList.at(i)->text() == str)
            {
                m_ActionList.at(i)->setChecked(true);
                return;
            }
        }
    }
}

void FSPlayerMenuControl::UpdataPlayResolutionMenuCheckState(double scale)
{
    QString str = QString("%1%2").arg((int)(scale * 100)).arg("%");
    double scaleTemp = 1.0;
    for (int i = Action_PlayResolution_Full; i <= Action_PlayResolution_1_16; i++)
    {
        if (qAbs(scaleTemp - scale) < 1e-5)
        {
            m_ActionList.at(i)->setChecked(true);
            return;
        }
        else
        {
            scaleTemp = scaleTemp / 2;
        }
    }
}

void FSPlayerMenuControl::InitAction()
{
    m_pMenuEdit = new QMenu(static_cast<QWidget *>(parent()));
    m_pMenuEdit->setFixedWidth(180);
    m_pMenuPlayResolution = new QMenu();
    m_pMenuZoom = new QMenu();
    for (int i = Action_Reset; i <= Action_ProjectSetting; i++)
    {
        QAction *pAction = new QAction(m_pMenuEdit);
        connect(pAction, &QAction::triggered, this, &FSPlayerMenuControl::OnActionTriggered);
        pAction->setData(i);
        m_pMenuEdit->addAction(pAction);
        if (i == Action_Delete)
        {
            m_pMenuEdit->addSeparator();
        }
        m_ActionList.push_back(pAction);
    }
    m_pMenuEdit->addMenu(m_pMenuPlayResolution);
    QActionGroup *pMenuPlayResolutionGroup = new QActionGroup(this);
    for (int i = Action_PlayResolution_Full; i <= Action_PlayResolution_1_16; i++)
    {
        QAction *pAction = new QAction(m_pMenuPlayResolution);
        pAction->setCheckable(true);
        pMenuPlayResolutionGroup->addAction(pAction);
        connect(pAction, &QAction::triggered, this, &FSPlayerMenuControl::OnActionTriggered);
        pAction->setData(i);
        m_pMenuPlayResolution->addAction(pAction);
        m_ActionList.push_back(pAction);
    }
    m_pMenuEdit->addMenu(m_pMenuZoom);
    QActionGroup *pMenuZoomGroup = new QActionGroup(this);
    for (int i = Action_Zoom_Fix; i <= Action_Zoom_200; i++)
    {
        QAction *pAction = new QAction(m_pMenuZoom);
        pAction->setCheckable(true);
        pMenuZoomGroup->addAction(pAction);
        connect(pAction, &QAction::triggered, this, &FSPlayerMenuControl::OnActionTriggered);
        pAction->setData(i);
        m_pMenuZoom->addAction(pAction);
        m_ActionList.push_back(pAction);
    }
    m_pMenuEdit->setVisible(false);
    m_pMenuPlayResolution->setVisible(false);
    m_pMenuZoom->setVisible(false);
    m_pMenuZoom->setFixedWidth(126);
    m_pMenuPlayResolution->setFixedWidth(126);
    CheckAction(Action_PlayResolution_Full);
    CheckAction(Action_Zoom_100);
}

void FSPlayerMenuControl::OnActionTriggered()
{
    QAction *pAction= static_cast<QAction *>(sender());
    emit ActionTriggered(pAction->data().toInt());
}
