#include "stdafx.h"
#include "PrimaryScreenManager.h"

#include "RecorderViewsMgr.h"
#include "FSRecorder.h"
#include "RecordPanel.h"
#include "FSSettingsWidget.h"
#include "SettingsDataManager.h"
#include "PreviewAttachmentManager.h"


PrimaryScreenManager* PrimaryScreenManager::GetInstance()
{
    static PrimaryScreenManager instance(0);
    return &instance;
}

PrimaryScreenManager::PrimaryScreenManager(QObject *parent)
: DesktopRectangleMgr(parent)
{
    connect(this, &DesktopRectangleMgr::sigPrimaryScreenChanged, this, &PrimaryScreenManager::PrimaryScreenChanged);

    m_recorderViewsMgr = RecorderViewsMgr::GetInstance();
    m_pHelper = RecordHelper::GetInstance();

    // 主窗口
    int dw = PrimaryScreenWidth();
    int dh = PrimaryScreenHeight();
    auto wwww = m_recorderViewsMgr->GetFSRecorder()->width();
    m_recorderViewsMgr->GetFSRecorder()->move((dw - m_recorderViewsMgr->GetFSRecorder()->width()) * 0.5, (dh - m_recorderViewsMgr->GetFSRecorder()->height()) * 0.5);


    m_pHelper->SetDesktopRect(PrimaryScreen());
    m_pHelper->SetRecordRect(DefaultRecordRect());

}



int PrimaryScreenManager::PrimaryScreenWidth()
{
    return PrimaryScreen().width();
}

int PrimaryScreenManager::PrimaryScreenHeight()
{
    return PrimaryScreen().height();

}


int PrimaryScreenManager::SecondScreenWidth()
{
    return SecondScreen().width();
}

int PrimaryScreenManager::SecondScreenHeight()
{
    return SecondScreen().height();
}




bool PrimaryScreenManager::eventFilter(QObject *watcher, QEvent *event)
{
    return false;
}

void PrimaryScreenManager::PrimaryScreenChanged()
{
    // 
    QRect primaryScreen = PrimaryScreen();

    QRect primaryRect = QRect(0, 0, primaryScreen.width(), primaryScreen.height());

    m_pHelper->SetDesktopRect(primaryRect);

    // 录制区在主屏
    m_recorderViewsMgr->GetRecordMask()->setGeometry(primaryScreen);
    m_recorderViewsMgr->GetRecordWidget()->setGeometry(primaryScreen);
    m_recorderViewsMgr->GetActionWidget()->setGeometry(primaryScreen);


    QRect recordRect = m_pHelper->RecordRect();
    bool bRecordRectChanged = false;

    if (SettingsDataManager::GetInstance()->GetDesktopData()->GetType() == DesktopData::CaputerType_Fullscreen)
    {
        recordRect = primaryRect;
        bRecordRectChanged = true;
    }
    else
    {
        if (recordRect.width() <= primaryRect.width() && recordRect.height() <= primaryRect.height())
        {
            if (!primaryRect.contains(recordRect))
            {
                recordRect.moveTopLeft(QPoint(primaryRect.width() - recordRect.width() + 1, primaryRect.bottom() - recordRect.height() + 1));
                bRecordRectChanged = true;
            }
        }
        else
        {
            recordRect = QRect(0, 0, primaryRect.width(), primaryRect.height());
            bRecordRectChanged = true;

        }
    }


    if (bRecordRectChanged)
    {
        m_pHelper->SetRecordRect(recordRect);
        RecordHelper::GetInstance()->sigResolutionChangedByDrag(recordRect);
    }
    

    //// 其它窗口切换逻辑
    auto NeedSwitchToSecondScreen = [&](QWidget *w)->bool{
        return PrimaryScreen().intersects(w->geometry());
    };
    auto NeedSwitchToPrimaryScreen = [&](QWidget *w)->bool{
        return PrePrimaryScreen().intersects(w->geometry());
    };

    auto SwitchToSecondScreen = [&](QWidget *panel){
        auto offset = panel->geometry().topLeft() - PrimaryScreen().topLeft();
        offset = SecondScreen().topLeft() + offset;
        panel->move(offset);
    };

    auto SwitchToPrimaryScreen = [&](QWidget *panel){
        auto offset = panel->geometry().topLeft() - PrePrimaryScreen().topLeft();
        offset = PrimaryScreen().topLeft() + offset;
        panel->move(offset);
    };

    auto SwitchPanel = [&](QWidget *panel){
        if (NeedSwitchToSecondScreen(panel))
        {
            SwitchToSecondScreen(panel);
            return false;
        }
        else if (NeedSwitchToPrimaryScreen(panel))
        {
            SwitchToPrimaryScreen(panel);
            return true;
        }
        return false;
    };


    //// 其它窗口切换逻辑
    //auto NeedSwitchToSecondScreen = [&](QWidget *w)->bool{
    //    return PrimaryScreen().intersects(w->geometry());
    //    //return PrimaryScreen().contains(w->geometry());
    //};
    //auto NeedSwitchToPrimaryScreen = [&](QWidget *w)->bool{
    //    return PrePrimaryScreen().intersects(w->geometry());
    //    //return PrePrimaryScreen().contains(w->geometry());
    //};
    //auto SwitchToSecondScreen = [&](QWidget *panel){
    //    auto offset = panel->geometry().topLeft() - PrimaryScreen().topLeft() /*+ QPoint(panel->width() / 2, 0)*/;
    //    float scaleX = 1.0 * offset.x() / PrimaryScreen().width();
    //    float scaleY = 1.0 * offset.y() / PrimaryScreen().height();
    //    //offset.setX(SecondScreen().right() - offset.x());
    //    offset.setX(SecondScreen().right() - SecondScreen().width() * scaleX);
    //    offset.setY(SecondScreen().top() + SecondScreen().height() * scaleY);
    //    panel->move(offset - QPoint(panel->width() / 2, 0));
    //};
    //auto SwitchToPrimaryScreen = [&](QWidget *panel){
    //    auto offset = panel->geometry().topLeft() - PrePrimaryScreen().topLeft();
    //    float scaleX = 1.0 * offset.x() / PrePrimaryScreen().width();
    //    float scaleY = 1.0 * offset.y() / PrePrimaryScreen().height();
    //    //offset = PrimaryScreen().topLeft() + offset;
    //    offset.setX(PrimaryScreen().left() + PrimaryScreen().width() * scaleX);
    //    offset.setY(PrimaryScreen().top() + PrimaryScreen().height() * scaleY);
    //    panel->move(offset);
    //};
    //auto SwitchPanel = [&](QWidget *panel){
    //    if (NeedSwitchToSecondScreen(panel))
    //    {
    //        SwitchToSecondScreen(panel);
    //    }
    //    else if (NeedSwitchToPrimaryScreen(panel))
    //    {
    //        SwitchToPrimaryScreen(panel);
    //    }
    //};



    bool bRecordPanelInPrimaryScreen = SwitchPanel(m_recorderViewsMgr->GetRecordPanel());
    SwitchPanel(m_recorderViewsMgr->GetCameraPreview());
    SwitchPanel(m_recorderViewsMgr->GetGamePreview());
    SwitchPanel(m_recorderViewsMgr->GetAudioPreview());


    auto AdjustPanel = [&](QWidget *panel, bool inPrimaryScreen){
        QRect rect = inPrimaryScreen ? ScreenAvailableGeometry(PrimaryScreenIndex()) : ScreenAvailableGeometry(SecondScreenIndex());
        QRect panelNeedRect = QRect(panel->geometry().topLeft(), QSize(panel->width(), 100));
        if (!rect.intersects(panelNeedRect))
        {
            panel->move(rect.right() - panel->width(), rect.bottom() - panel->height());
            PreviwAttachmentManager::GetInstance()->PrimaryScreenSwitchMoveAttachPreviews();
        }
    };

    AdjustPanel(m_recorderViewsMgr->GetRecordPanel(), bRecordPanelInPrimaryScreen);


     //更新界面数据

    m_recorderViewsMgr->GetRecordPanel()->showEvent(nullptr);
}

void PrimaryScreenManager::SwitchRecordScreen()
{
    SwitchPrimary2Screen(SecondScreenIndex());
}

QRect PrimaryScreenManager::DefaultRecordRect()
{
    QRect dsk = PrimaryScreen();
    int customW = 1280;
    int customH = 720;
    return QRect((dsk.width() - customW) * 0.5, (dsk.height() - customH) * 0.5, customW, customH);
}

void PrimaryScreenManager::SetDefaultRecordPanelPosition()
{
    static bool bFirstShow = true;
    if (bFirstShow)
    {
        bFirstShow = false;
        auto recordPanel = m_recorderViewsMgr->GetRecordPanel();
        int margin = 80;
        int w = recordPanel->width();
        int h = recordPanel->height();

        QRect rcDsk = PrimaryScreenManager::GetInstance()->ScreenAvailableGeometry(PrimaryScreenManager::GetInstance()->SecondScreenIndex());
        int l = rcDsk.right() - margin - w + 1;
        int t = rcDsk.bottom() - h + 1;

        if (PrimaryScreenManager::GetInstance()->ScreenCount() > 1)
        {
            // 如果多屏，调整panel近主录制屏
            if (PrimaryScreen().left() < SecondScreen().left())
            {
                l = rcDsk.left() + margin;
            }
            else
            {
                l = rcDsk.right() - margin - w;
            }
        }
        recordPanel->move(l, t);
    }
}




bool PrimaryScreenManager::PanelInRecordingRectangle(QWidget *panel)
{
    if (SettingsDataManager::GetInstance()->GetGameData()->GetDeviceEnabled())
    {
        return false;
    }
    else if (!SettingsDataManager::GetInstance()->GetDesktopData()->GetDeviceEnabled())
    {
        return false;
    }
    else
    {
        QRect cropRect = SettingsDataManager::GetInstance()->GetDesktopData()->GetCropRect();
        QPoint primaryScreenTopLeft = PrimaryScreen().topLeft();
        cropRect.moveTopLeft(QPoint(cropRect.x() + primaryScreenTopLeft.x(), cropRect.y() + primaryScreenTopLeft.y()));
        return cropRect.intersects(panel->geometry());
    }     
}
