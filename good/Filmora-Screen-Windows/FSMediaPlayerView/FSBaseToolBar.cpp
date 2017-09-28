#include "stdafx.h"

#include "FSBaseToolBar.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSCommonlib/VolumeSlider.h"
#include "inc_MediaPlayerView/MediaPlayerControl.h"
#include "inc_CommonLib/NLETimeline.h"
#include "inc_CommonLib/externalapi.h"

#include <QImage>
#include "inc_FSCommonlib/gathread.h"


FSBaseToolBar::FSBaseToolBar(IMediaPlayerControl *control, QWidget *parent)
    : QWidget(parent)
    , mpMediaPlayerControl(nullptr)
    , mpProgressSlider(nullptr)
    , mpSliderEditVolume(nullptr)
    , mCurrentVoluem(0)
    , m_pHL(NULL)
{
    Q_ASSERT(control);
    mpMediaPlayerControl = control;
    CreateUI();
    ConnectSignals();
}

FSBaseToolBar::~FSBaseToolBar()
{

}

bool FSBaseToolBar::Close()
{
    bool reg;
    if (reg=mpMediaPlayerControl->Close())
    {
        OnStoped();
        InitTimeText();
        EnableWidgets(false);
    }
    return reg;
}

bool FSBaseToolBar::Open(int bSource, NLETimeline *timeline)
{
    Q_ASSERT(timeline);
    return mpMediaPlayerControl->Open(bSource, timeline);
}

void FSBaseToolBar::Play()
{
    if (!mpMediaPlayerControl->IsPlaying())
    {
        if (mpMediaPlayerControl->HaveSource())
            mpMediaPlayerControl->Play();
        else
            OnPaused();
    }
}

void FSBaseToolBar::Stop()
{
    mpMediaPlayerControl->Stop();
}

void FSBaseToolBar::Pause()
{
    mpMediaPlayerControl->Pause();
}

void FSBaseToolBar::OnSeekCallback(int frames, int accurate)
{
    Q_UNUSED(accurate);
    mpProgressSlider->setValue(frames);
}

bool FSBaseToolBar::SeekManual(int frames, int accurate)
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->Seek(frames, accurate);
        emit RulerSeekCallback(frames);
        if (mpMediaPlayerControl->IsPlaying() || mbseekManual)
        {
            mlastSeekFrames = frames;
        }
    }
    return mpMediaPlayerControl->HaveSource();
}

void FSBaseToolBar::SetDuration(int duration)
{  
    if (duration <= 0)
    {
        mpProgressSlider->setValue(0);
    }
    mpProgressSlider->setDuration(duration);
    UpdateTimeText(duration);
}


void FSBaseToolBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    e->accept();
}

void FSBaseToolBar::OnStoped()
{
    ToolBarPlayButtonStateChanged(ToolBar::ToolBar_Play);
    mpProgressSlider->setValue(0);
    RulerSeekCallback(0);
}

void FSBaseToolBar::OnPlayed()
{
    ToolBarPlayButtonStateChanged(ToolBar_Pause);
}

void FSBaseToolBar::OnPaused()
{
    ToolBarPlayButtonStateChanged(ToolBar_Play);
}

void FSBaseToolBar::OnForwarded()
{
    int frames = qMin(CurrentFrames() + 1, Duration());
    OnSeekCallback(frames, 1);
    UpdateTimeText(Duration());
    emit RulerSeekCallback(frames);
}

void FSBaseToolBar::OnBackwarded()
{
    int frames = qMax(CurrentFrames() - 1, 0);
    OnSeekCallback(frames, 1);
    emit RulerSeekCallback(frames);
}

void FSBaseToolBar::CreateUI()
{
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("FSCommonLib_FSBaseToolBar");
    CreateBtn(ToolBar_Stop, "FSBaseToolBar_Stop", this);
    CreateBtn(ToolBar_Backward, "FSBaseToolBar_Backward", this);
    CreateBtn(ToolBar_Forward, "FSBaseToolBar_Forward", this);
    CreateBtn(ToolBar_Volume, "FSBaseToolBar_Volume", this);

    CreateStateBtn(ToolBar_ViewCut, "FSBaseToolBar_View", ToolBar_ViewScale, "FSBaseToolBar_Scale");
    CreateStateBtn(ToolBar_Play, "FSBaseToolBar_Play", ToolBar_Pause, "FSBaseToolBar_Pause");
    CreateStateBtn(ToolBar_FullScreen, "FSBaseToolBar_FullScreen", ToolBar_FullScreenExit, "FSBaseToolBar_FullScreenExit");



    mpProgressSlider = new ToolBarSlider(this);
    mpSliderEditVolume = new AudioVolumeSlider(this);
    mpLabelPreViewDuration = new FSLabel(this);
    mpLabelEditDuration = new FSLabel(this);

    mpSliderEditVolume->SetRange(-100, 0);
    mpSliderEditVolume->hide();
    mpSliderEditVolume->SetValue(mCurrentVoluem);
    mpLabelEditDuration->setFixedSize(58, 17);
    mpLabelPreViewDuration->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mpLabelEditDuration->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mpProgressSlider->setVisible(true);
    SetUiTipText();
    setFixedHeight(37);
    if (mpLabelPreViewDuration->text().isEmpty())
    {
        InitTimeText();
    }
    EnableWidgets(false);
    mpProgressSlider->setObjectName("mpProgressSlider");
    mpLabelPreViewDuration->setObjectName("FSBaseToolBar_mpLabelPreViewDuration");
    mpLabelEditDuration->setObjectName("FSBaseToolBar_mpLabelEditDuration");
    m_pHL = new QHBoxLayout(this);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_Backward]);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_Forward]);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_Play]);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_Stop]);
    m_pHL->addWidget(mpLabelPreViewDuration);
    m_pHL->addSpacing(10);
    m_pHL->addWidget(mpProgressSlider, 2);
    m_pHL->addSpacing(10);
    m_pHL->addWidget(mpLabelEditDuration);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_Volume]);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_ViewCut]);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_FullScreen]);
    m_pHL->setSpacing(0);
    m_pHL->setMargin(0);
    UpdateTimeText(mpMediaPlayerControl->Duration());
    GetButton(ToolBar_ViewCut)->setVisible(false);
}

void FSBaseToolBar::ConnectSignals()
{
    connect(this, &FSBaseToolBar::RulerSeekCallback, mpMediaPlayerControl, &IMediaPlayerControl::RulerSeekCallback);
    connect(mpProgressSlider, &ToolBarSlider::valueChanged, [this](double value){
        SeekManual(value, 100);
        UpdateTimeText(mpMediaPlayerControl->Duration());
    });
    connect(mpProgressSlider, &ToolBarSlider::startValueChanged, [this](){
        mbseekManual = true;
        mbPlaying = mpMediaPlayerControl->IsPlaying();
        mpMediaPlayerControl->Pause();
        emit StartRulerSeekCallback(mbPlaying);
    });
    connect(mpProgressSlider, &ToolBarSlider::endValueChanged, [this](){
        if (mbPlaying)
        {
            mbseekManual = false;
            mpMediaPlayerControl->Play();
            OnPlayed();
            emit EndRulerSeekCallback(mbPlaying);
            mbPlaying = false;
            mlastSeekFrames = -1;
        }
    });


    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayingCallback, [this](int frames){
        mpProgressSlider->setValue(frames);
        UpdateTimeText(mpMediaPlayerControl->Duration());
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayStopCallback, [this](){
        OnStoped();
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayFinishCallback, [this](){
        OnStoped();
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayStartCallback, [this](){
        OnPlayed();
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayLoadFinishCallback, [this]()
    {
        mpProgressSlider->setDuration(mpMediaPlayerControl->Duration());
        mpSliderEditVolume->SetValue(mCurrentVoluem);

        UpdateTimeText(mpMediaPlayerControl->Duration());
        EnableWidgets(mpMediaPlayerControl->GetTimeline()->GetClipCount() > 0);
        OnVolumeValueChanged(mpSliderEditVolume->GetValue());
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayPauseCallback, [this]()
    {
        OnPaused();
    });

    connect(mpSliderEditVolume, &AudioVolumeSlider::ValueChanged, [this](int value){
        OnVolumeValueChanged(value);
    });

    connect(mpMediaPlayerControl, &IMediaPlayerControl::SeekFinishCallback, [this](int frames){
        if (!mbPlaying)
        {
            if (mbseekManual)
            {
                if (mlastSeekFrames == frames && !mpProgressSlider->ButtonPressDown())
                {
                    emit EndRulerSeekCallback(false);
                    mlastSeekFrames = -1;
                    mbseekManual = false;
                }
            }
            else
            {
                if (mlastSeekFrames == frames)
                {
                    mlastSeekFrames = -1;
                }
            }
        }

    });
}

void FSBaseToolBar::CreateBtn(int Id, const QString &objName, QWidget *parent)
{
    auto pBtn = new FSBaseButton(Id, objName, parent);
    pBtn->setVisible(true);
    pBtn->setFixedSize(37, 37);
    connect(pBtn, &FSBaseButton::Clicked, this, &FSBaseToolBar::BtnClicked);
    if (Id == ToolBar_Forward)
    {
        connect(pBtn, &FSBaseButton::pressed, [this](){
            Forward();          
        });
    }
    else if (Id==ToolBar_Backward)
    {
        connect(pBtn, &FSBaseButton::pressed, [this](){
            Backward();          
        });
    }
    mToolBarBtnMap.insert(Id, pBtn);
}

void FSBaseToolBar::CreateStateBtn(int firstId, const QString &objName1, int secId, const QString &objName2)
{
    auto pStateBtn = new FSStateBaseButton(firstId, "", this);
    pStateBtn->setVisible(true);
    pStateBtn->setFixedSize(37, 37);
    auto pBtn1 = new FSBaseButton(firstId, objName1, pStateBtn);
    auto pBtn2 = new FSBaseButton(secId,   objName2, pStateBtn);
    pStateBtn->AddButton(pBtn1);
    pStateBtn->AddButton(pBtn2);
    connect(pStateBtn, &BaseStateButton::Clicked, this, &FSBaseToolBar::BtnClicked);
    mToolBarBtnMap.insert(firstId, pStateBtn);
}

void FSBaseToolBar::ToolBarPlayButtonStateChanged(int id)
{
    qobject_cast<FSStateBaseButton*>(GetButton(ToolBar::ToolBar_Play))->SetCurrentButtonId(id);
}

void FSBaseToolBar::SetUiTipText()
{
    static_cast<FSBaseButton*>(mToolBarBtnMap[ToolBar_Stop])->setToolTip(tr("stop(Ctrl+/)"));
    static_cast<FSBaseButton*>(mToolBarBtnMap[ToolBar_Forward])->setToolTip(tr("forward(Left)"));
    static_cast<FSBaseButton*>(mToolBarBtnMap[ToolBar_Backward])->setToolTip(tr("backward(Right)"));
    static_cast<FSBaseButton*>(mToolBarBtnMap[ToolBar_Volume])->setToolTip(tr("Volume"));

    FSStateBaseButton *pStateBtn = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_ViewCut]);
    static_cast<FSBaseButton*>(pStateBtn->GetButton(ToolBar_ViewScale))->setToolTip(tr("Scale"));
    static_cast<FSBaseButton*>(pStateBtn->GetButton(ToolBar_ViewCut))->setToolTip(tr("Cut"));

    FSStateBaseButton *pStateBtn1 = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_Play]);
    static_cast<FSBaseButton*>(pStateBtn1->GetButton(ToolBar_Play))->setToolTip(tr("play(Space)"));
    static_cast<FSBaseButton*>(pStateBtn1->GetButton(ToolBar_Pause))->setToolTip(tr("pause(Space)"));
    FSStateBaseButton *pStateBtn2 = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_FullScreen]);
    static_cast<FSBaseButton*>(pStateBtn2->GetButton(ToolBar_FullScreen))->setToolTip(tr("fullscreen"));
    static_cast<FSBaseButton*>(pStateBtn2->GetButton(ToolBar_FullScreenExit))->setToolTip(tr("exit"));
}

void FSBaseToolBar::BtnClicked(int id, QPoint leftTop)
{
    switch (id)
    {
    case FSBaseToolBar::ToolBar_Play:
        Play();
        break;
    case FSBaseToolBar::ToolBar_Stop:
        Stop();
        break;
    case FSBaseToolBar::ToolBar_Pause:
        Pause();
        break;
    case FSBaseToolBar::ToolBar_Forward:
        emit EndBackForword();
        break;
    case FSBaseToolBar::ToolBar_Backward:
        emit EndBackForword();
        break;
    case FSBaseToolBar::ToolBar_Volume:
        OnVolumeClicked(leftTop);
        break;
    case FSBaseToolBar::ToolBar_ViewScale:
        emit VeiwCutSignal(true);
        break;
    case FSBaseToolBar::ToolBar_ViewCut:
        emit VeiwCutSignal(false);
        break;
    case FSBaseToolBar::ToolBar_FullScreen:
    case FSBaseToolBar::ToolBar_FullScreenExit:
        emit FullScreenSignal();
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void FSBaseToolBar::Forward()
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->Forward();
        OnForwarded();
    }
}

void FSBaseToolBar::Backward()
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->Backward();
        OnBackwarded();
    }
}

void FSBaseToolBar::Seek(int frames)
{
    bool bPlaying = mpMediaPlayerControl->IsPlaying();
    if (SeekManual(frames, 100))
    {
        mpProgressSlider->setValue(frames);
    }        
    if (bPlaying)
    {
        if(mpMediaPlayerControl->Play())
            OnPlayed();
    }
    UpdateTimeText(mpMediaPlayerControl->Duration());
}

void FSBaseToolBar::Restore()
{
    (qobject_cast<FSStateBaseButton*>(GetButton(ToolBar_FullScreen)))->SetCurrentButtonId(ToolBar_FullScreen);
}

bool FSBaseToolBar::HaveSource()
{
    return mpMediaPlayerControl->HaveSource();
}

bool FSBaseToolBar::IsPlaying()
{
    return mpMediaPlayerControl->IsPlaying();
}


void FSBaseToolBar::OnVolumeClicked(QPoint pos)
{    
    mpSliderEditVolume->SetValue(mpMediaPlayerControl->GetVolume());
    mpSliderEditVolume->DoModal(pos);    
}

void FSBaseToolBar::OnVolumeValueChanged(int value)
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->SetVolume(value);
        mCurrentVoluem = value;
    }
        
}


void FSBaseToolBar::InitTimeText()
{   
     mpLabelPreViewDuration->setText("00:00:00");
     mpLabelEditDuration->setText("00:00:00");
}

void FSBaseToolBar::EnableWidgets(bool bEnable)
{
    setEnabled(bEnable);   
}

void FSBaseToolBar::UpdateTimeText(int duration)
{
    QString timeText, timeText1;
    if (!mpMediaPlayerControl->HaveSource())
    {
        InitTimeText();
        return;
    }
   
    timeText = gFrame2StringSecond(qMin(CurrentFrames(), duration), mpMediaPlayerControl->FPS());
    timeText1 = gFrame2StringSecond(duration, mpMediaPlayerControl->FPS());
    mpLabelPreViewDuration->setText(timeText.left(8));
    mpLabelEditDuration->setText(timeText1.left(8));
}

int FSBaseToolBar::CurrentFrames()
{
    return mpProgressSlider->value();
}

int FSBaseToolBar::Duration()
{
    return mpMediaPlayerControl->Duration();
}


bool FSBaseToolBar::IsSeeking()
{
    return mpMediaPlayerControl->IsSeeking();
}

BaseButton* FSBaseToolBar::GetButton(int id)
{
    if (mToolBarBtnMap.contains(id))
    {
        return mToolBarBtnMap[id];
    }
    return nullptr;
}

BaseButton* FSBaseToolBar::GetStateBtn(int id, int id1)
{
    FSStateBaseButton * pBtn = qobject_cast<FSStateBaseButton *>(GetButton(id));
    if (pBtn)
    {
        return pBtn->GetButton(id1);
    }
    return NULL;
}

void FSBaseToolBar::DblMouseBtnFullScreen()
{
    (qobject_cast<FSStateBaseButton*>(GetButton(ToolBar_FullScreen)))->ClickCurrentButton();
}

bool FSBaseToolBar::IsScaleState()
{
    FSStateBaseButton *pStateBtn = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_ViewCut]);
    return (pStateBtn->CurrentButtonId() == ToolBar_ViewCut);
}

void FSBaseToolBar::SetFullScreenBtnVisible(bool bVisible)
{
    FSStateBaseButton *pStateBtn = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_FullScreen]);
    if (bVisible)
        pStateBtn->SetCurrentButtonId(ToolBar_FullScreen);
    else{
        pStateBtn->SetCurrentButtonId(ToolBar_FullScreenExit);
    }
}

void FSBaseToolBar::SetScaleBtnVisible(bool bVisible)
{
    FSStateBaseButton *pStateBtn = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_ViewCut]);
    if (bVisible)
        pStateBtn->SetCurrentButtonId(ToolBar_ViewScale);
    else{
        pStateBtn->SetCurrentButtonId(ToolBar_ViewCut);
    }
}

void FSBaseToolBar::SetChunkHeight(int height)
{
    mpProgressSlider->SetChunkHeight(height);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FSRecorderBaseToolBar::FSRecorderBaseToolBar(IMediaPlayerControl *control, QWidget *parent )
    : QWidget(parent)
    , mpMediaPlayerControl(nullptr)
    , mpProgressSlider(nullptr)
    , mCurrentVoluem(0)
    , m_pHL(NULL)
{
    Q_ASSERT(control);
    mpMediaPlayerControl = control;
    CreateUI();
    ConnectSignals();
}

FSRecorderBaseToolBar::~FSRecorderBaseToolBar()
{

}

bool FSRecorderBaseToolBar::Close()
{
    bool reg;
    if (reg = mpMediaPlayerControl->Close())
    {
        OnStoped();
        InitTimeText();
        EnableWidgets(false);
    }
    return reg;
}

bool FSRecorderBaseToolBar::Open(int bSource, NLETimeline *timeline)
{
    Q_ASSERT(timeline);
    return mpMediaPlayerControl->Open(bSource, timeline);
}

void FSRecorderBaseToolBar::Play()
{
    if (!mpMediaPlayerControl->IsPlaying())
    {
        if (mpMediaPlayerControl->HaveSource())
            mpMediaPlayerControl->Play();
        else
            OnPaused();
    }
}

void FSRecorderBaseToolBar::Stop()
{
    mpMediaPlayerControl->Stop();
}

void FSRecorderBaseToolBar::Pause()
{
    mpMediaPlayerControl->Pause();
}

void FSRecorderBaseToolBar::Forward()
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->Forward();
        OnForwarded();
    }
}

void FSRecorderBaseToolBar::Backward()
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->Backward();
        OnBackwarded();
    }
}

void FSRecorderBaseToolBar::Seek(int frames)
{
    bool bPlaying = mpMediaPlayerControl->IsPlaying();
    if (SeekManual(frames, 100))
    {
        mpProgressSlider->setValue(frames);
    }
    if (bPlaying)
    {
        if (mpMediaPlayerControl->Play())
            OnPlayed();
    }
    UpdateTimeText(mpMediaPlayerControl->Duration());
}

void FSRecorderBaseToolBar::Restore()
{
    (qobject_cast<FSStateBaseButton*>(GetButton(ToolBar_FullScreen)))->SetCurrentButtonId(ToolBar_FullScreen);
}

bool FSRecorderBaseToolBar::HaveSource()
{
    return mpMediaPlayerControl->HaveSource();
}

bool FSRecorderBaseToolBar::IsPlaying()
{
    return mpMediaPlayerControl->IsPlaying();
}

void FSRecorderBaseToolBar::EnableWidgets(bool bEnable)
{
    setEnabled(bEnable);
}

void FSRecorderBaseToolBar::UpdateTimeText(int duration)
{
    QString timeText;
    if (!mpMediaPlayerControl->HaveSource())
    {
        InitTimeText();
        return;
    }
    timeText = QString("<font color = #ffffff > %1< / font><font color = #7F9CAF >/%2< / font>").arg(gFrame2StringSecond(qMin(CurrentFrames(), duration), mpMediaPlayerControl->FPS())).arg(gFrame2StringSecond(duration, mpMediaPlayerControl->FPS()));
    mpLabelPreViewDuration->setText(timeText);
   
}

void FSRecorderBaseToolBar::SetDuration(int duration)
{
    mpProgressSlider->setDuration(duration);
    if (duration == 0)
    {
        mpProgressSlider->setValue(0);
    }
    UpdateTimeText(duration);
}

int FSRecorderBaseToolBar::CurrentFrames()
{
    return mpProgressSlider->value();
}

int FSRecorderBaseToolBar::Duration()
{
    return mpMediaPlayerControl->Duration();
}

bool FSRecorderBaseToolBar::IsSeeking()
{
    return mpMediaPlayerControl->IsSeeking();
}

BaseButton* FSRecorderBaseToolBar::GetButton(int id)
{
    if (mToolBarBtnMap.contains(id))
    {
        return mToolBarBtnMap[id];
    }
    return nullptr;
}

BaseButton* FSRecorderBaseToolBar::GetStateBtn(int id, int id1)
{
    FSStateBaseButton * pBtn = qobject_cast<FSStateBaseButton *>(GetButton(id));
    if (pBtn)
    {
        return pBtn->GetButton(id1);
    }
    return NULL;
}

void FSRecorderBaseToolBar::DblMouseBtnFullScreen()
{
    (qobject_cast<FSStateBaseButton*>(GetButton(ToolBar_FullScreen)))->ClickCurrentButton();
    mpMediaPlayerControl->Update();
}

void FSRecorderBaseToolBar::SetFullScreenBtnVisible(bool bVisible)
{
    FSStateBaseButton *pStateBtn = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_FullScreen]);
    if (bVisible)
        pStateBtn->SetCurrentButtonId(ToolBar_FullScreen);
    else{
        pStateBtn->SetCurrentButtonId(ToolBar_FullScreenExit);
    }
}

void FSRecorderBaseToolBar::SetChunkHeight(int height)
{
    mpProgressSlider->SetChunkHeight(height);
}

void FSRecorderBaseToolBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    e->accept();
}


void FSRecorderBaseToolBar::OnStoped()
{
    ToolBarPlayButtonStateChanged(ToolBar_Play);
    mpProgressSlider->setValue(0);
    RulerSeekCallback(0);
}

void FSRecorderBaseToolBar::OnPlayed()
{
    ToolBarPlayButtonStateChanged(ToolBar_Pause);
}

void FSRecorderBaseToolBar::OnPaused()
{
    ToolBarPlayButtonStateChanged(ToolBar_Play);
}

void FSRecorderBaseToolBar::OnForwarded()
{
    int frames = qMin(CurrentFrames() + 1, Duration());
    OnSeekCallback(frames, 1);
    UpdateTimeText(Duration());
    emit RulerSeekCallback(frames);
}

void FSRecorderBaseToolBar::OnBackwarded()
{
    int frames = qMax(CurrentFrames() - 1, 0);
    OnSeekCallback(frames, 1);
    emit RulerSeekCallback(frames);
}

void FSRecorderBaseToolBar::OnSeekCallback(int frames, int accurate)
{
    Q_UNUSED(accurate);
    mpProgressSlider->setValue(frames);
}

bool FSRecorderBaseToolBar::SeekManual(int frames, int accurate)
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->Seek(frames, accurate);
        emit RulerSeekCallback(frames);
        if (mpMediaPlayerControl->IsPlaying() || mbseekManual)
        {
            mlastSeekFrames = frames;
        }
    }
    return mpMediaPlayerControl->HaveSource();
}

void FSRecorderBaseToolBar::CreateUI()
{
    setObjectName("RecorderToolBar");
    setAttribute(Qt::WA_StyledBackground);    
    CreateStateBtn(ToolBar_VolumeOn, "RecorderFSBaseToolBar_VolumeOn", ToolBar_VolumeOff, "RecorderFSBaseToolBar_VolumeOff");
    CreateStateBtn(ToolBar_Play, "RecorderFSBaseToolBar_Play", ToolBar_Pause, "RecorderFSBaseToolBar_Pause");
    CreateStateBtn(ToolBar_FullScreen, "RecorderFSBaseToolBar_FullScreen", ToolBar_FullScreenExit, "RecorderFSBaseToolBar_FullScreenExit");
    GetButton(ToolBar_VolumeOn)->setFixedSize(25, 25);
    GetButton(ToolBar_Play)->setFixedSize(15, 15);
    GetButton(ToolBar_FullScreen)->setFixedSize(15, 15);
    mpProgressSlider = new ToolBarSlider(this);
    mpLabelPreViewDuration = new FSLabel(this);
    mpSliderPreviewVolume = new ToolBarSlider(this,Qt::Horizontal);
    mpSliderPreviewVolume->setDuration(100);
    mpSliderPreviewVolume->setValue(mCurrentVoluem);
    mpSliderPreviewVolume->SetChunkHeight(16);
    mpSliderPreviewVolume->setFixedWidth(97);
    mpLabelPreViewDuration->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    SetUiTipText();
    setFixedHeight(37);
    if (mpLabelPreViewDuration->text().isEmpty())
    {
        InitTimeText();
    }
    EnableWidgets(false);
    
    mpSliderPreviewVolume->setObjectName("RecorderFSBaseToolBar_pToolBarSlider");
    mpProgressSlider->setObjectName("RecorderFSBaseToolBar_pToolBarSlider");
    mpLabelPreViewDuration->setObjectName("RecorderFSBaseToolBar_mpLabelPreViewDuration");
    mpProgressSlider->setVisible(true);
    m_pHL = new QHBoxLayout(this);
    m_pHL->addSpacing(6);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_Play]);
    m_pHL->addSpacing(6);
    m_pHL->addWidget(mpLabelPreViewDuration);
    m_pHL->addSpacing(8);
    m_pHL->addWidget(mpProgressSlider,2);
    m_pHL->addSpacing(17);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_VolumeOn]);
    m_pHL->addSpacing(7);
    m_pHL->addWidget(mpSliderPreviewVolume);
    m_pHL->addSpacing(10);
    m_pHL->addWidget(mToolBarBtnMap[ToolBar_FullScreen]);
    m_pHL->addSpacing(10);
    m_pHL->setSpacing(0);
    m_pHL->setMargin(0);
}

void FSRecorderBaseToolBar::ConnectSignals()
{
    connect(this, &FSRecorderBaseToolBar::RulerSeekCallback, mpMediaPlayerControl, &IMediaPlayerControl::RulerSeekCallback);
    connect(mpProgressSlider, &ToolBarSlider::valueChanged, [this](double value){
		m_iStartSeekTime = QDateTime::currentDateTime().toTime_t();
        SeekManual(value, 100);
        UpdateTimeText(mpMediaPlayerControl->Duration());
    });
    connect(mpProgressSlider, &ToolBarSlider::startValueChanged, [this](){
  
        mbseekManual = true;
        mbPlaying = mpMediaPlayerControl->IsPlaying();
        mpMediaPlayerControl->Pause();
        emit StartRulerSeekCallback(mbPlaying);
    });
    connect(mpProgressSlider, &ToolBarSlider::endValueChanged, [this](){
        if (mbPlaying)
        {
            mbseekManual = false;
            mpMediaPlayerControl->Play();
            OnPlayed();
            emit EndRulerSeekCallback(mbPlaying);
            mbPlaying = false;
            mlastSeekFrames = -1;	
        }
		//上传GA信息
		m_iEndSeekTime = QDateTime::currentDateTime().toTime_t();
		GAThread::getInstance()->insertSeekInfo(m_iEndSeekTime - m_iStartSeekTime);
    });

    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayingCallback, [this](int frames){
        mpProgressSlider->setValue(frames);
        UpdateTimeText(mpMediaPlayerControl->Duration());
        BaseButton *pBtn = GetStateBtn(ToolBar_Play, ToolBar_Play);
        if (pBtn&&pBtn->isVisible())
        {
            OnPlayed();
        }
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayStopCallback, [this](){
        OnStoped();
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayFinishCallback, [this](){
        OnStoped();
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayStartCallback, [this](){
        OnPlayed();
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayLoadFinishCallback, [this]()
    {
        mpProgressSlider->setDuration(mpMediaPlayerControl->Duration());
        mpSliderPreviewVolume->setValue(0.5*mCurrentVoluem+50);
        if (mpSliderPreviewVolume->isEnabled())
        {
            mpMediaPlayerControl->SetVolume(mCurrentVoluem);
        }
        else
        {
            mpMediaPlayerControl->SetVolume(-100);
        }

        UpdateTimeText(mpMediaPlayerControl->Duration());
        OnVolumeValueChanged(mpSliderPreviewVolume->value());
    });
    connect(mpMediaPlayerControl, &IMediaPlayerControl::PlayPauseCallback, [this]()
    {
        OnPaused();
    });

    connect(mpSliderPreviewVolume, &ToolBarSlider::valueChanged, this, &FSRecorderBaseToolBar::OnVolumeValueChanged);


    connect(mpMediaPlayerControl, &IMediaPlayerControl::SeekFinishCallback, [this](int frames){
        if (!mbPlaying)
        {
            if (mbseekManual)
            {
                if (mlastSeekFrames == frames && !mpProgressSlider->ButtonPressDown())
                {
                    emit EndRulerSeekCallback(false);
                    mlastSeekFrames = -1;
                    mbseekManual = false;
                }
            }
            else
            {
                if (mlastSeekFrames == frames)
                {
                    mlastSeekFrames = -1;
                }
            }
        }

    });
}

void FSRecorderBaseToolBar::CreateBtn(int Id, const QString &objName, QWidget *parent)
{
    auto pBtn = new FSBaseButton(Id, objName, parent);
    pBtn->setVisible(true);
    pBtn->setFixedSize(37, 37);
    connect(pBtn, &FSBaseButton::Clicked, this, &FSRecorderBaseToolBar::BtnClicked);
    mToolBarBtnMap.insert(Id, pBtn);
}

void FSRecorderBaseToolBar::CreateStateBtn(int firstId, const QString &objName1, int secId, const QString &objName2)
{
    auto pStateBtn = new FSStateBaseButton(firstId, "", this);
    pStateBtn->setVisible(true);
    pStateBtn->setFixedSize(37, 37);
    auto pBtn1 = new FSBaseButton(firstId, objName1, pStateBtn);
    auto pBtn2 = new FSBaseButton(secId, objName2, pStateBtn);
    pStateBtn->AddButton(pBtn1);
    pStateBtn->AddButton(pBtn2);
    connect(pStateBtn, &BaseStateButton::Clicked, this, &FSRecorderBaseToolBar::BtnClicked);
    mToolBarBtnMap.insert(firstId, pStateBtn);
}

void FSRecorderBaseToolBar::ToolBarPlayButtonStateChanged(int id)
{
    qobject_cast<FSStateBaseButton*>(GetButton(ToolBar_Play))->SetCurrentButtonId(id);
}

void FSRecorderBaseToolBar::SetUiTipText()
{
    FSStateBaseButton *pStateBtn1 = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_Play]);
    static_cast<FSBaseButton*>(pStateBtn1->GetButton(ToolBar_Play))->setToolTip(tr("play(Space)"));
    static_cast<FSBaseButton*>(pStateBtn1->GetButton(ToolBar_Pause))->setToolTip(tr("pause(Space)"));
    FSStateBaseButton *pStateBtn2 = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_FullScreen]);
    static_cast<FSBaseButton*>(pStateBtn2->GetButton(ToolBar_FullScreen))->setToolTip(tr("fullscreen(Alt+Enter)"));
    static_cast<FSBaseButton*>(pStateBtn2->GetButton(ToolBar_FullScreenExit))->setToolTip(tr("exit"));
    FSStateBaseButton *pStateBtn3 = static_cast<FSStateBaseButton*>(mToolBarBtnMap[ToolBar_VolumeOn]);
    static_cast<FSBaseButton*>(pStateBtn3->GetButton(ToolBar_VolumeOn))->setToolTip(tr("VolumeOn"));
    static_cast<FSBaseButton*>(pStateBtn3->GetButton(ToolBar_VolumeOff))->setToolTip(tr("VolumeOff"));
}

void FSRecorderBaseToolBar::BtnClicked(int id, QPoint leftTop)
{
    switch (id)
    {
    case ToolBar_Play:
        Play();
        break;
        break;
    case ToolBar_Pause:
        Pause();
        break;
    case ToolBar_VolumeOn:
        OnVolumeOnClicked();
        break;
    case ToolBar_VolumeOff:
        OnVolumeOffClicked();
        break;
    case ToolBar_FullScreen:
    case ToolBar_FullScreenExit:
        emit FullScreenSignal();
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

void FSRecorderBaseToolBar::OnVolumeValueChanged(int value)
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->SetVolume(2*value-100);
        mCurrentVoluem = 2 * value - 100;
    }
}

void FSRecorderBaseToolBar::OnVolumeOnClicked()
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpMediaPlayerControl->SetVolume(-100);
        mpSliderPreviewVolume->setEnabled(false);
    }
}

void FSRecorderBaseToolBar::OnVolumeOffClicked()
{
    if (mpMediaPlayerControl->HaveSource())
    {
        mpSliderPreviewVolume->setValue(0.5*mCurrentVoluem+50);
        mpSliderPreviewVolume->setEnabled(true);
        OnVolumeValueChanged(mpSliderPreviewVolume->value());
    }
}

void FSRecorderBaseToolBar::InitTimeText()
{

    mpLabelPreViewDuration->setText("<font color = #ffffff >00:00:00< / font><font color = #7F9CAF >/00:00:00< / font>");
}
