#include "stdafx.h"
#include "PreviewAttachmentManager.h"
#include "RecorderViewsMgr.h"
#include "FSSettingsWidget.h"
#include "RecordPanel.h"
#include "PrimaryScreenManager.h"
#include "SettingsDataManager.h"

PreviwAttachmentManager* PreviwAttachmentManager::GetInstance()
{
    static PreviwAttachmentManager instance(0);
    return &instance;
}

void PreviwAttachmentManager::PrimaryScreenSwitchMoveAttachPreviews()
{
    const int margins = 10;

    m_attachPosition = m_pRecordPanel->geometry().topLeft();
    if (m_pCameraPreview->isVisible())
        SetPreviewAttach(m_pCameraPreview, true);
    if (m_pGamePreview->isVisible())
        SetPreviewAttach(m_pGamePreview, true);
    if (m_pAudioPreview->isVisible())
        SetPreviewAttach(m_pAudioPreview, true);

    AttachPreview(m_pCameraPreview);
    AttachPreview(m_pGamePreview);
    AttachPreview(m_pAudioPreview);

}

PreviwAttachmentManager::PreviwAttachmentManager(QObject *parent)
: QObject(parent)
{

    m_bCamerAttach = true;
    m_bGameAttach = true;
    m_bAudioAttach = true;

    m_recorderViewsMgr = RecorderViewsMgr::GetInstance();

    m_pRecordPanel = m_recorderViewsMgr->GetRecordPanel();
    m_pCameraPreview = m_recorderViewsMgr->GetCameraPreview();
    m_pGamePreview = m_recorderViewsMgr->GetGamePreview();
    m_pAudioPreview = m_recorderViewsMgr->GetAudioPreview();
    InitPanel(m_pCameraPreview);
    InitPanel(m_pGamePreview);
    InitPanel(m_pAudioPreview);

    m_pAttachHintDashline = new DashlineWidget(m_pRecordPanel);
    m_pAttachHintDashline->SetColor(QColor(220, 220, 220, 100));

    connect(m_pRecordPanel, &RecordPanel::sigStartDashedMove, this, [&](QPoint point){
        for (auto panel : m_previews)
        {
            StartAttachPreview(panel.first, panel.second, point);
        }   
    });
    connect(m_pRecordPanel, &RecordPanel::sigDashedMove, this, [&](QPoint point){
        for (auto panel : m_previews)
        {
            MoveAttachPreview(panel.first, panel.second, point);
        }

    });
    connect(m_pRecordPanel, &RecordPanel::sigEndDashedMove, this, [&](QPoint point){
        for (auto panel : m_previews)
        {
            EndAttachPreview(panel.first, panel.second);
        }        
        AttachPreviews();
    });


    auto CurrentDeviceLostFun = [&](){
        AttachPreviews();
    };
    connect(SettingsDataManager::GetInstance()->GetCameraData(), &CameraData::signalCurrentDeviceLost, this, CurrentDeviceLostFun);
    connect(SettingsDataManager::GetInstance()->GetAudioData(), &AudioData::signalCurrentDeviceLost, this, CurrentDeviceLostFun);
    connect(SettingsDataManager::GetInstance()->GetGameData(), &GameData::signalCurrentDeviceLost, this, CurrentDeviceLostFun);

}
QPoint PreviwAttachmentManager::AdjustPanelInAvailableDesktopRectangle(PreviewBase *preview, QPoint point)
{
    QPoint returnPoint = point;
    // 查找point所有的AvailableGeometry，如果都不在
    QRect availableRect = PrimaryScreenManager::GetInstance()->ScreenAvailableGeometry(point);
    if (!availableRect.contains(point))
    {
        const int margins = 2;
        QPoint newAttachPosition = m_pRecordPanel->geometry().topLeft() - QPoint(preview->width() + margins, 0);
        if (newAttachPosition.x() < availableRect.left())
            newAttachPosition.setX(m_pRecordPanel->geometry().right() + margins);

        for (auto temp : m_previews)
        {
            if (temp.first != preview && temp.first->isVisible())
            {
                if (temp.first->geometry().intersects(QRect(newAttachPosition, QSize(preview->width(), preview->height()))))
                {
                    newAttachPosition += QPoint(0, temp.first->height() + margins);
                }
            }
        }
        returnPoint = newAttachPosition;
    }

    return returnPoint;
}

bool PreviwAttachmentManager::eventFilter(QObject *watcher, QEvent *event)
{
    if (event->type() == QEvent::Show)
    {
        PreviewBase *preview = qobject_cast<PreviewBase*>(watcher);
        QPoint topleft = preview->geometry().topLeft();

        if (GetAttack(preview))
        {
            // 计算attachPosition
            m_attachPosition = m_pRecordPanel->geometry().topLeft();       
            for (auto temp : m_previews)
            {
                PreviewBase *panel = temp.first;
                // 已经attach中的
                if (GetPreviewAttach(panel))
                {
                    QPoint offset(0, panel->height());
                    m_attachPosition = m_attachPosition - offset;
                }
            }

            
            m_attachPosition = AdjustPanelInAvailableDesktopRectangle(preview, m_attachPosition);
            
            preview->move(m_attachPosition);
            AttachPreviews(true);

        }
        else
        {
            topleft = AdjustPanelInAvailableDesktopRectangle(preview, topleft);
            preview->move(topleft);

        }
    
    }
    else if (event->type() == QEvent::Hide)
    {
        //AttachPreviews(true);
    }
    else if (event->type() == QEvent::Move)
    {
        //QMoveEvent *pEvent = static_cast<QMoveEvent*>(event);
        //if (watcher == m_pAudioPreview)
        //{
            
        //}
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        AttachPreviews();
    }

    return false;
}



void PreviwAttachmentManager::StartAttachPreview(PreviewBase *preview, DashlineWidget *dashed, QPoint point)
{
    if (GetPreviewAttach(preview))
    {
        QPoint offsetPos = m_pRecordPanel->geometry().topLeft() - preview->geometry().topLeft();
        dashed->resize(preview->size());
        dashed->MovePosition(point - offsetPos);
    }

}

void PreviwAttachmentManager::MoveAttachPreview(PreviewBase *preview, DashlineWidget *dashed, QPoint point)
{
    if (GetPreviewAttach(preview))
    {
        QPoint offsetPos = m_pRecordPanel->geometry().topLeft() - preview->geometry().topLeft();
        dashed->MovePosition(point - offsetPos);
    }

}

void PreviwAttachmentManager::EndAttachPreview(PreviewBase *preview, DashlineWidget *dashed)
{
    if (GetPreviewAttach(preview))
    {
        preview->move(dashed->geometry().topLeft());
        dashed->HideDashline();
    }
}

void PreviwAttachmentManager::ShowAttachPreview(PreviewBase *preview)
{
    if (GetAttack(preview))
    {
        QPoint offset(0, preview->height());
        m_attachPosition = m_attachPosition - offset;
        preview->move(m_attachPosition);
    }
}

bool PreviwAttachmentManager::GetPreviewAttach(PreviewBase *preview)
{
    return GetAttack(preview) && preview->isVisible();
}
bool PreviwAttachmentManager::GetAttack(PreviewBase *preview)
{
    if (preview == m_pCameraPreview)
    {
        return m_bCamerAttach && !preview->GetLock();
    }
    else if (preview == m_pGamePreview)
    {
        return m_bGameAttach && !preview->GetLock();

    }
    else if (preview == m_pAudioPreview)
    {
        return m_bAudioAttach && !preview->GetLock();

    }

    return false;
}

void PreviwAttachmentManager::AttachPreview(PreviewBase *preview)
{
    if (GetPreviewAttach(preview))
    {
        QPoint offset(0, preview->height());
        m_attachPosition = m_attachPosition - offset;
        preview->move(m_attachPosition);
    }
}

void PreviwAttachmentManager::AttachPreviews(bool ignoreRecordPanelVisible)
{
    const int margins = 10;

    m_attachPosition = m_pRecordPanel->geometry().topLeft();
    // 吸附逻辑
    auto AttachPreviewFun = [&](PreviewBase *preview){
        QRect attachRectangle(m_attachPosition + QPoint(m_pRecordPanel->width() / 2, -margins), QSize(m_pRecordPanel->width() / 2, 2 * margins));
        QRect previewRect(preview->geometry().bottomLeft() + QPoint(preview->width() / 2, -margins), QSize(preview->width() / 2, 2 * margins));
        if (!ignoreRecordPanelVisible && !m_pRecordPanel->isVisible())
            previewRect = QRect();
        //QRect previewRect = preview->geometry().adjusted(-margins, preview->height() - margins, margins, margins);
        SetPreviewAttach(preview, attachRectangle.intersects(previewRect));
        AttachPreview(preview);
    };

    std::vector<std::pair<bool, PreviewBase*>> previews;
    for (auto temp : m_previews)
    {
        previews.push_back(std::make_pair(false, temp.first));
    }

    // 循环吸附/循环取消吸附
    for (int count = 0; count < previews.size(); ++count)
    {
        for (int i = 0; i < previews.size(); ++i)
        {
            auto &keyvalue = previews[i];
            if (!keyvalue.first && keyvalue.second->isVisible())
            {
                AttachPreviewFun(keyvalue.second);
                keyvalue.first = GetPreviewAttach(keyvalue.second);
            }
        }
    }
}

void PreviwAttachmentManager::InitPanel(PreviewBase *panel)
{
    panel->installEventFilter(this);
    auto dashed = new DashlineWidget(m_pRecordPanel);
    dashed->HideDashline();
    m_previews.push_back(std::make_pair(panel, dashed));

    connect(panel, &PreviewBase::signalLock, this, &PreviwAttachmentManager::slotPreviewLock);
    connect(panel, &PreviewBase::signalManulClose, this, &PreviwAttachmentManager::slotPreviewLock);

    connect(panel, &PreviewBase::sigStartDashedMove, this, &PreviwAttachmentManager::StartDashedMove);
    connect(panel, &PreviewBase::sigDashedMove, this, &PreviwAttachmentManager::DashedMove);
    connect(panel, &PreviewBase::sigEndDashedMove, this, &PreviwAttachmentManager::EndDashedMove);

}


void PreviwAttachmentManager::slotPreviewLock()
{
    PreviewBase *preview = qobject_cast<PreviewBase*>(sender());
    //SetPreviewAttach(preview, false);
    AttachPreviews(true);
}

void PreviwAttachmentManager::StartDashedMove(QPoint point)
{

    PreviewBase *preview = qobject_cast<PreviewBase*>(sender());
    Q_ASSERT(preview);
    m_pAttachHintDashline->resize(preview->size());
}

void PreviwAttachmentManager::DashedMove(QPoint point)
{

    PreviewBase *preview = qobject_cast<PreviewBase*>(sender());
    Q_ASSERT(preview);

    const int margins = 10;

    m_attachPosition = m_pRecordPanel->geometry().topLeft();
    // 吸附逻辑
    auto AttachPreviewFun = [&](PreviewBase *preview, QPoint tempPoint){
        QRect attachRectangle(m_attachPosition + QPoint(m_pRecordPanel->width() / 2, -margins), QSize(m_pRecordPanel->width() / 2, 2 * margins));
        QRect previewRect(tempPoint + QPoint(0, preview->height()) + QPoint(preview->width() / 2, -margins), QSize(preview->width() / 2, 2 * margins));
        if (!m_pRecordPanel->isVisible())
            previewRect = QRect();
        //QRect previewRect = preview->geometry().adjusted(-margins, preview->height() - margins, margins, margins);
        /*SetPreviewAttach(preview, attachRectangle.intersects(previewRect));
        AttachPreview(preview);*/
        return attachRectangle.intersects(previewRect);
    };

    std::vector<std::pair<bool, PreviewBase*>> previews;
    for (auto temp : m_previews)
    {
        if (temp.first != preview)
            previews.push_back(std::make_pair(false, temp.first));
    }

    // 循环吸附/循环取消吸附
    for (int count = 0; count < previews.size(); ++count)
    {
        for (int i = 0; i < previews.size(); ++i)
        {
            auto &keyvalue = previews[i];
            if (!keyvalue.first && keyvalue.second->isVisible())
            {
                if (AttachPreviewFun(keyvalue.second, keyvalue.second->geometry().topLeft()))
                {
                    QPoint offset(0, keyvalue.second->height());
                    m_attachPosition = m_attachPosition - offset;
                    keyvalue.first = GetPreviewAttach(keyvalue.second);
                }
            }
        }
    }

    if (AttachPreviewFun(preview, point))
    {
        m_pAttachHintDashline->MovePosition(m_attachPosition - QPoint(0, preview->height()));
    }
    else
    {
        m_pAttachHintDashline->HideDashline();

    }
}

void PreviwAttachmentManager::EndDashedMove(QPoint point)
{
    m_pAttachHintDashline->HideDashline();
}

void PreviwAttachmentManager::SetPreviewAttach(QWidget *preview, bool attach)
{
    if (preview == m_pCameraPreview)
    {
        m_bCamerAttach = attach;
    }
    else if (preview == m_pGamePreview)
    {
        m_bGameAttach = attach;
    }
    else if (preview == m_pAudioPreview)
    {
        m_bAudioAttach = attach;

    }
    else
        Q_ASSERT(false);
}

//////////////////////////////////////////////////////////////////////////

DashlineWidget::DashlineWidget(QWidget *parent)
:QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    m_color = QColor(220, 220, 220, 180);

    HideDashline();
}

void DashlineWidget::SetColor(QColor color)
{
    m_color = color;
}

void DashlineWidget::MovePosition(QRect r)
{
    setGeometry(r);
    if (!isVisible())
        ShowDashLine();
}

void DashlineWidget::MovePosition(QPoint point)
{
    move(point);
    if (!isVisible())
        ShowDashLine();
}

void DashlineWidget::HideDashline()
{
    hide();
}

void DashlineWidget::ShowDashLine()
{
    show();
}

void DashlineWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen = painter.pen();
    //pen.setStyle(Qt::DashLine);
    pen.setWidth(8);
    pen.setColor(m_color);
    painter.setPen(pen);

    painter.drawRect(rect());
}
