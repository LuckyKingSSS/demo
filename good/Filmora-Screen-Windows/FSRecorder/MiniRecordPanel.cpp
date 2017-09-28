#include "stdafx.h"
#include "MiniRecordPanel.h"

#include "RecordPanel.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "RecorderViewsMgr.h"
#include "SettingsDataManager.h"
#include "FSRecorder.h"
#include "PrimaryScreenManager.h"

MiniRecordPanel::MiniRecordPanel(QWidget *parent)
: FSBaseWidget(parent)
{

    m_pTitleBar = new TopLevelWindowTitleBar(this);
    m_pTitleBar->SetSystemBorderVisible(false);
    m_pTitleBar->SetButtons(TopLevelWindowTitleBar::None);
    m_pTitleBar->SetResizeble(false);
    m_pTitleBar->SetDragMaximum(false);
    m_pTitleBar->GetControl()->SetBorderColor(QColor(102, 102, 102));
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);


    m_pBtnRecord = new BaseStateButton(0, "MiniRecordPanel_RecordButton", this);
    m_pBtnRecord->AddButton(0, "MiniRecordPanel_RecordButton_Start");
    m_pBtnRecord->AddButton(1, "MiniRecordPanel_RecordButton_Stop");

    m_pBtnPauseResume = new BaseStateButton(0, "MiniRecordPanel_PauseResumeButton", this);
    m_pBtnPauseResume->AddButton(0, "MiniRecordPanel_PauseButton");
    m_pBtnPauseResume->AddButton(1, "MiniRecordPanel_ResumeButton");

    m_pCountBackwardCtrl = new CountBackwardCtrl("RecordPanel_CountBackwardCtrl", this);
    m_pBtnDelete = new BaseButton("MiniRecordPanel_DeleteButton", this);

    m_pBtnMinimized = new BaseButton("FSRecorder_TitleBar_MinimizeButton", this);
    //m_pBtnMaxPanel = new BaseButton("MiniRecordPanel_MaxPanelButton", this);
    m_pBtnMaxPanel = new BaseButton("FSRecorder_TitleBar_MaxipanelButton", this);

    m_volumeBar = new MiniVolumeBar(this);
    connect(SettingsDataManager::GetInstance()->GetAudioData(), &AudioData::signalVolumeChanged, this, [&](int value){
        if (isVisible())
        {
            m_volumeBar->SetValue(value);
        }
    });

    m_attachMgr = new MiniPanelAttachmentManager(this);
    resize(300, 44);

    // 防止被任务栏盖住
    QTimer *timer = new QTimer(this);
    timer->start(500);
    connect(timer, &QTimer::timeout, this, [&](){
        if (this->isVisible())
            this->raise();
    });
}

MiniRecordPanel::~MiniRecordPanel()
{

}

void MiniRecordPanel::resizeEvent(QResizeEvent *event)
{
    const int titleBorderWidth = m_pTitleBar->GetControl()->GetBorderWidth();
    m_pTitleBar->setGeometry(titleBorderWidth, 0, width() - 2 * titleBorderWidth, height());
    const int margins = 10;
    int left = 20;
    int top = 0;

    const int btnRecordWidth = height() - 18;
    top = (height() - btnRecordWidth) / 2;
    m_pBtnRecord->setGeometry(left, top, btnRecordWidth, btnRecordWidth);
    left = m_pBtnRecord->geometry().right() + margins;
    m_pBtnPauseResume->setGeometry(left, top, btnRecordWidth, btnRecordWidth);
    left = m_pBtnPauseResume->geometry().right() + margins;

    const int countBackwardCtrlWidth = 90;
    const int countBackwardCtrlHeight = height() - 10;
    top = (height() - countBackwardCtrlHeight) / 2;
    m_pCountBackwardCtrl->setGeometry(left, top, countBackwardCtrlWidth, countBackwardCtrlHeight);
    left = m_pCountBackwardCtrl->geometry().right() + margins;

    const int btnDeleteHeight = 20;
    top = (height() - btnDeleteHeight) / 2;
    m_pBtnDelete->setGeometry(left, top, btnDeleteHeight, btnDeleteHeight);
    left = m_pBtnDelete->geometry().right() + margins;


    top = (height() - btnRecordWidth) / 2;
    m_pBtnMinimized->setGeometry(left, top, btnRecordWidth, btnRecordWidth);
    left = m_pBtnMinimized->geometry().right() + margins;

    m_pBtnMaxPanel->setGeometry(left, top, btnRecordWidth, btnRecordWidth);

    m_volumeBar->setGeometry(titleBorderWidth, height() - 2, width() - 2 * titleBorderWidth, height());
}

void MiniRecordPanel::showEvent(QShowEvent *event)
{
    // 解决m_pBtnRecord状态贴图无法刷新过来的问题
    QEvent myEvent(QEvent::Leave);
    QApplication::sendEvent(m_pBtnRecord->GetButton(0), &myEvent);
    FSBaseWidget::showEvent(event);
}

void MiniRecordPanel::SetToolTip()
{
    m_pBtnRecord->GetButton(0)->setToolTip(tr("Start  capturing "));
    m_pBtnRecord->GetButton(1)->setToolTip(tr("Stop "));

    m_pBtnDelete->setToolTip(tr("Delete recording file"));
    m_pBtnPauseResume->GetButton(0)->setToolTip(tr("Pause capturing"));
    m_pBtnPauseResume->GetButton(1)->setToolTip(tr("Resume capturing"));

    m_pBtnMinimized->setToolTip(tr("Minimize"));
    m_pBtnMaxPanel->setToolTip(tr("Maximize"));

}

void MiniRecordPanel::Attach()
{
    m_attachMgr->Attach();
}

//

SyncRecordPanelAndMiniPanelState::SyncRecordPanelAndMiniPanelState(RecordPanel *panel, MiniRecordPanel *miniPanel, QObject *parent)
:QObject(parent)
, m_pRecordPanel(panel)
, m_pMiniPanel(miniPanel)
, m_currentPanelType(Type_Panel)
, m_startRecordPanelType(Type_Panel)
{

    m_pRecordPanel->installEventFilter(this);
    m_pMiniPanel->installEventFilter(this);

    connect(m_pRecordPanel->m_pBtnRecord, &RecordButton::sigClicked, this, [&](RecordButton::ButtonId id){
        emit sigButtonRecordClicked(id);
    });

    connect(m_pMiniPanel->m_pBtnRecord, &BaseStateButton::Clicked, this, [&](int id){
        emit sigButtonRecordClicked(id);
    });

    connect(m_pRecordPanel->m_pBtnPauseResume, &BaseStateButton::Clicked, this, [&](int id){
        emit sigButtonPauseResumeClicked(id);
    });

    connect(m_pMiniPanel->m_pBtnPauseResume, &BaseStateButton::Clicked, this, [&](int id){
        emit sigButtonPauseResumeClicked(id);
    });

    connect(m_pRecordPanel->m_pBtnDelete, &BaseButton::Clicked, this, [&]{
        emit sigButtonDeleteClicked(m_pRecordPanel);
    });
    connect(m_pMiniPanel->m_pBtnDelete, &BaseButton::Clicked, this, [&]{
        emit sigButtonDeleteClicked(m_pMiniPanel);
    });


    connect(m_pRecordPanel->m_pBtnMinimized, &BaseButton::Clicked, this, [&]{
        if (RecorderCenter::GetInstance()->IsRecording())
        {
            m_pRecordPanel->setVisible(false);
        }
        else
        {
            RecorderViewsMgr::GetInstance()->HideAllView();

        }
    });
    connect(m_pMiniPanel->m_pBtnMinimized, &BaseButton::Clicked, this, [&]{
        m_pMiniPanel->setVisible(false);
        if (RecorderCenter::GetInstance()->IsRecording())
        {
        }
        else
        {
            RecorderViewsMgr::GetInstance()->HideAllView();

        }

    });


    // panel间的切换
    connect(m_pRecordPanel->m_pBtnMiniPanel, &BaseButton::Clicked, this, [&]{
        m_currentPanelType = Type_MiniPanel;
        m_pRecordPanel->setVisible(false);
        //m_pMiniPanel->move(m_globalPos);
        m_pMiniPanel->Attach();
        m_pMiniPanel->setVisible(true);
    });
    connect(m_pMiniPanel->m_pBtnMaxPanel, &BaseButton::Clicked, this, [&]{
        m_currentPanelType = Type_Panel;
        //m_pRecordPanel->move(m_globalPos);
        m_pRecordPanel->setVisible(true);
        m_pMiniPanel->setVisible(false);
    });


}

void SyncRecordPanelAndMiniPanelState::StartRecorder()
{
    m_startRecordPanelType = m_currentPanelType;   
   
    if (m_currentPanelType == Type_Panel)
    {
        if (SettingsDataManager::GetInstance()->GetAdvancedData()->IsAutoHideRecorder())
        {
            if (!m_pRecordPanel->GetLock())
            {
                // 设置了自动隐藏
                m_pRecordPanel->setVisible(false);
            }

        }
        else
        {
            // 在录制区，而且未Lock, 开始录制就变成minipanel
            if (PrimaryScreenManager::GetInstance()->PanelInRecordingRectangle(m_pRecordPanel) && !m_pRecordPanel->GetLock())
            {
                m_pRecordPanel->m_pBtnMiniPanel->Clicked(0, QPoint());
            }
        }
        
    }
    else
    {
        m_pMiniPanel->setVisible(true);
    }

    // 及时显示minipanel
    qApp->processEvents();
}

void SyncRecordPanelAndMiniPanelState::StopRecorder()
{
    // 复原录制前是哪个panelType, 并隐藏临时那个
    if (m_currentPanelType != m_startRecordPanelType)
    {
        GetRealRecordWidget()->setVisible(false);
    }
    m_currentPanelType = m_startRecordPanelType;
}

void SyncRecordPanelAndMiniPanelState::ButtonRecordSetCurrentId(int id)
{
    m_pRecordPanel->m_pBtnRecord->SetCurrentId((RecordButton::ButtonId)id);
    m_pMiniPanel->m_pBtnRecord->SetCurrentButtonId(id);
}

void SyncRecordPanelAndMiniPanelState::ButtonRecordSetEnabled(bool enabled)
{
    m_pRecordPanel->m_pBtnRecord->setEnabled(enabled);
    m_pMiniPanel->m_pBtnRecord->setEnabled(enabled);
}

void SyncRecordPanelAndMiniPanelState::ButtonPauseResumeSetCurrentId(int id)
{
    m_pRecordPanel->m_pBtnPauseResume->SetCurrentButtonId(id);
    m_pMiniPanel->m_pBtnPauseResume->SetCurrentButtonId(id);
}

void SyncRecordPanelAndMiniPanelState::ButtonPauseResumeSetEnabled(bool enabled)
{
    m_pRecordPanel->m_pBtnPauseResume->setEnabled(enabled);
    m_pMiniPanel->m_pBtnPauseResume->setEnabled(enabled);
}

void SyncRecordPanelAndMiniPanelState::ButtonDeleteSetEnabled(bool enabled)
{
    m_pRecordPanel->m_pBtnDelete->setEnabled(enabled);
    m_pMiniPanel->m_pBtnDelete->setEnabled(enabled);
}

void SyncRecordPanelAndMiniPanelState::ButtonDeleteSetToolTip(QString tooltip)
{
    m_pRecordPanel->m_pBtnDelete->setToolTip(tooltip);
    m_pMiniPanel->m_pBtnDelete->setToolTip(tooltip);
}

void SyncRecordPanelAndMiniPanelState::CountBackwardCtrlStart()
{
    m_pRecordPanel->m_pCountBackwardCtrl->Start();
    m_pMiniPanel->m_pCountBackwardCtrl->Start();
}

void SyncRecordPanelAndMiniPanelState::CountBackwardCtrlPause()
{
    m_pRecordPanel->m_pCountBackwardCtrl->Pause();
    m_pMiniPanel->m_pCountBackwardCtrl->Pause();

}

void SyncRecordPanelAndMiniPanelState::CountBackwardCtrlResume()
{
    m_pRecordPanel->m_pCountBackwardCtrl->Resume();
    m_pMiniPanel->m_pCountBackwardCtrl->Resume();

}

void SyncRecordPanelAndMiniPanelState::CountBackwardCtrlStop()
{
    m_pRecordPanel->m_pCountBackwardCtrl->Stop();
    m_pMiniPanel->m_pCountBackwardCtrl->Stop();

}

void SyncRecordPanelAndMiniPanelState::setVisible(bool enabled)
{
    QWidget *curPanel = nullptr;
   if (m_currentPanelType == Type_Panel)
    {
        curPanel = m_pRecordPanel;
    }
    else
    {
        curPanel = m_pMiniPanel;
    }

    curPanel->setVisible(enabled);
    if (enabled)
        curPanel->activateWindow();


}

QWidget* SyncRecordPanelAndMiniPanelState::GetRealRecordWidget()
{
    if (m_currentPanelType == Type_Panel)
        return m_pRecordPanel;
    else
        return m_pMiniPanel;
}



bool SyncRecordPanelAndMiniPanelState::eventFilter(QObject *watcher, QEvent *event)
{
    if (event->type() == QEvent::Hide)
    {
        if ((watcher == m_pRecordPanel && m_currentPanelType == Type_Panel) || 
            (watcher == m_pMiniPanel && m_currentPanelType == Type_MiniPanel)
            )
        {

        }
    }
    else if (event->type() == QEvent::Show)
    {
        if (watcher == m_pMiniPanel)
        {

        }
    }
    else if (event->type() == QEvent::Move)
    {
        QMoveEvent *pEvent = static_cast<QMoveEvent*>(event);

        m_globalPos = pEvent->pos();
        

    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
#include "PreviewAttachmentManager.h"

MiniPanelAttachmentManager::MiniPanelAttachmentManager(MiniRecordPanel *parent)
:QObject(parent)
, m_attach(true)
, m_miniPanel(parent)
{
    //QRect availableRect = PrimaryScreenManager::GetInstance()->ScreenAvailableGeometry(PrimaryScreenManager::GetInstance()->PrimaryScreenIndex());
    //m_recordRectangle = availableRect.adjusted(10,10,-10,-10);

    m_pAttachHintDashline = new DashlineWidget(m_miniPanel);
    m_pAttachHintDashline->SetColor(QColor(220, 220, 220, 100));

    m_miniPanel->installEventFilter(this);
    connect(RecordHelper::GetInstance(), &RecordHelper::sigResolutionChangedByDrag, this, &MiniPanelAttachmentManager::RecorderSizeChanged);
    connect(RecordHelper::GetInstance(), &RecordHelper::sigResolutionChangedByEdit, this, &MiniPanelAttachmentManager::RecorderSizeChanged);

    connect(m_miniPanel->m_pTitleBar, &TopLevelWindowTitleBar::StartDashedMove, this, [&](QPoint point){
        m_pAttachHintDashline->resize(m_miniPanel->size());

    });
    connect(m_miniPanel->m_pTitleBar, &TopLevelWindowTitleBar::DashedMove, this, [&](QPoint point){
        auto rettuple = GetCanAttach(point - PrimaryScreenManager::GetInstance()->PrimaryScreen().topLeft());
        if (rettuple)
        {
            m_pAttachHintDashline->MovePosition(GetAttachPosition());
        }
        else
        {
            m_pAttachHintDashline->HideDashline();
        }

    });
    connect(m_miniPanel->m_pTitleBar, &TopLevelWindowTitleBar::EndDashedMove, this, [&](QPoint point){
        auto rettuple = GetCanAttach(m_miniPanel->geometry().topLeft() - PrimaryScreenManager::GetInstance()->PrimaryScreen().topLeft());
        if (rettuple)
        {
            //SetAttach(true);
            //m_miniPanel->move(GetAttachPosition());
        }
        m_pAttachHintDashline->HideDashline();
    });
}

bool MiniPanelAttachmentManager::GetAttach()
{
    return m_attach && SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled();
}

void MiniPanelAttachmentManager::SetAttach(bool attach)
{
    m_attach = attach;
}

void MiniPanelAttachmentManager::Attach()
{
    if (GetAttach())
    {
        m_miniPanel->move(GetAttachPosition());
    }
}

void MiniPanelAttachmentManager::RecorderSizeChanged(QRect rc)
{
    m_recordRectangle = rc;

    Attach();
}

QPoint MiniPanelAttachmentManager::GetAttachPosition()
{
    const int margins = 5;
    QRect availableRect = PrimaryScreenManager::GetInstance()->PrimaryScreen();
    QPoint offsetScreen = availableRect.topLeft();
    //QPoint position = m_recordRectangle.bottomRight() - QPoint(m_miniPanel->width(), margins);
    int x = qMax(0, m_recordRectangle.right() - m_miniPanel->width());
    int y = m_recordRectangle.bottom() + margins;
    // 右下
    if (y + m_miniPanel->height() > availableRect.height())
    {

        // 上边
        x = qMax(0, m_recordRectangle.right() - m_miniPanel->width());
        y = m_recordRectangle.top() - m_miniPanel->height() - margins;
        if (y < 0)
        {
            // 右边
            x = m_recordRectangle.right() + margins;
            y = m_recordRectangle.bottom() - m_miniPanel->height() - margins;

            if (x + m_miniPanel->width() > availableRect.width())
            {
                // 左边
                x = m_recordRectangle.left() - m_miniPanel->width() - margins;
                y = m_recordRectangle.bottom() - m_miniPanel->height() - margins;
                if (x < 0)
                {
                    x = qMax(0, m_recordRectangle.right() - m_miniPanel->width());
                    y = m_recordRectangle.bottom() - m_miniPanel->height() - 10;
                }

            }
        }



    }

    return QPoint(x, y) + offsetScreen;
}

QRect MiniPanelAttachmentManager::GetAttachRectangle()
{
    return QRect();
}


bool MiniPanelAttachmentManager::GetCanAttach(QPoint position)
{
    const int margins = 0;

    QList<QRect> rectlist;

    int left = m_recordRectangle.right() - m_miniPanel->width() - margins;
    int width = m_miniPanel->width() + margins;
    QRect tempRect;
    //tempRect = QRect(left, m_recordRectangle.bottom(), width, margins);
    //rectlist.push_back(tempRect);

    //tempRect = QRect(left, m_recordRectangle.bottom() - m_miniPanel->height() - margins, width, margins);
    //rectlist.push_back(tempRect);

    //tempRect = QRect(left, m_recordRectangle.top() - m_miniPanel->height() - margins, width, margins);
    //rectlist.push_back(tempRect);

    // 右下角
    tempRect = QRect(left, m_recordRectangle.bottom() - m_miniPanel->height() - margins, width, 2*(m_miniPanel->height()+margins));
    rectlist.push_back(tempRect);

    //// 右上角
    //tempRect = QRect(left, m_recordRectangle.top() - m_miniPanel->height() - margins, width, 2 * (m_miniPanel->height() + margins));
    //rectlist.push_back(tempRect);


    position += QPoint(m_miniPanel->width()/2, m_miniPanel->height()/2);
    for (QRect temp : rectlist)
    {
        if (temp.contains(position))
        {
            return true;
        }
    }

    return false;
}

bool MiniPanelAttachmentManager::eventFilter(QObject *watcher, QEvent *event)
{
    if (watcher == m_miniPanel)
    {
        if (event->type() == QEvent::Move)
        {

            //QMoveEvent *pEvent = static_cast<QMoveEvent*>(event);
            //m_globalPos = pEvent->pos();
            //if (!RecordHelper::GetInstance()->IsDraging() && m_miniPanel->isVisible())
            //{
            //    SetAttach(GetCanAttach(pEvent->pos()));
            //}
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {

            QMoveEvent *pEvent = static_cast<QMoveEvent*>(event);
            if (!RecordHelper::GetInstance()->IsDraging() && m_miniPanel->isVisible())
            {
                SetAttach(GetCanAttach(m_miniPanel->geometry().topLeft() - PrimaryScreenManager::GetInstance()->PrimaryScreen().topLeft()));
                if (GetAttach())
                {
                    m_miniPanel->move(GetAttachPosition());
                }
            }

        }
    }



    return false;
}
