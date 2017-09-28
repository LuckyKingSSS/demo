#include "stdafx.h"
#include "FSExportProgressDialog.h"
#include "inc_CommonLib/StringOperation.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "FSExportSettingData.h"
#include <QTimer>
#include "inc_FSCommonlib/gathread.h"
#include "inc_CommonLib/externalapi.h"

FSExportProgressDialog::FSExportProgressDialog(QWidget *parent)
    : FSBaseDialog(tr("Filmora Scrn"),"Filmora Scrn",parent)
    , mpExportHelperMgr(nullptr)
    , m_bErrorOccur(false)
{
    setAttribute(Qt::WA_StyledBackground);
    CreateUI();
    TranslateUI();
    ConnectSignals();
    setFixedSize(660, 242);
    m_titlebar->setFixedHeight(34);
    mpFPSHelper = new FPSHelper(this);
}

FSExportProgressDialog::~FSExportProgressDialog()
{

}


void FSExportProgressDialog::SetExportHelperMgr(FSExportHelperManager *pExportHelperMgr)
{
    Q_ASSERT(pExportHelperMgr);
    mpExportHelperMgr = pExportHelperMgr;
    mpProgressBar->setRange(0, mpExportHelperMgr->GetTotalFrames());
    connect(mpExportHelperMgr, &FSExportHelperManager::sigExportState, this, &FSExportProgressDialog::OnExportHelperStatesChanagedSlot);
    //connect(mpExportHelperMgr, &FSExportHelperManager::sigThumbnail, this, &FSExportProgressDialog::OnThumbnail);
    connect(mpExportHelperMgr, &FSExportHelperManager::sigFinished, this, &FSExportProgressDialog::OnFinished);
    mpExportHelperMgr->Start();
    mpFPSHelper->Start(mpExportHelperMgr->GetTotalFrames());
}

QRect adaptiveRect(const QRect& r, int w, int h)
{
    if (w * 1.0 / r.width() < h * 1.0 / r.height())
    {
        double d = h * 1.0 / r.height();
        w = w / d;
        h = r.height();
    }
    else
    {
        double d = w * 1.0 / r.width();
        h = h / d;
        w = r.width();
    }
    return QRect(r.left() + (r.width() - w) / 2, r.top() + (r.height() - h) / 2, w, h);
}


void FSExportProgressDialog::resizeEvent(QResizeEvent *e)
{
    m_titleHeight = m_titlebar->height();   
    m_titlebar->setGeometry(1, 1, width() - 2, m_titleHeight);
    int spacingW = 38;
    int spacingH = 10;
    int t = m_titleHeight + 26;
    int l = 20;
    int w = 610;
    int h = 24;
    mpLabelStep->setGeometry(l, t, w , h);
    mpLabelElapsedTime->setGeometry(25, height() - 75, 400, 25);
    mpLabelLeaveTime->setGeometry(25, mpLabelElapsedTime->geometry().bottom(), 400, 25);
    t = mpLabelStep->geometry().bottom() + 23;
    h = 4;
    mpProgressBar->setGeometry(l, t, w, h);
    spacingW = 10;
    l = width()-280;
    t = mpProgressBar->geometry().bottom()+60;
    mpButtonPauseResume->setGeometry(l, t, 120, 40);
    mpButtonFindTarget->setGeometry(l, t, 120, 40);
    mpButtonCancel->setGeometry(width()-150, t, 120, 40);
}

void FSExportProgressDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();   
    OnCancelCilckedSlot();
    
}

void FSExportProgressDialog::CreateUI()
{
    //mpLabelMovie           = new FSLabel(this);
    mpLabelStep            = new FSLabel(this);
    mpLabelLeaveTime = new FSLabel(this);
    mpLabelElapsedTime = new ElapsedTimeLabel(this);
    mpButtonCancel         = new FSButton(this);
    mpButtonPauseResume    = new FSButton(this);
    mpButtonFindTarget     = new FSButton(this);
    mpProgressBar          = new QProgressBar(this);
    
    mpButtonPauseResume->setCheckable(true);
    mpProgressBar->setTextVisible(false);
    mpButtonFindTarget->setVisible(false);
    mpButtonPauseResume->setVisible(false);
    mpProgressBar->setAttribute(Qt::WA_StyledBackground);
    m_titlebar->setObjectName("FSExportProgressDialog_m_titlebar");
    m_titlebar->GetButton(TopLevelWindowTitleBar::Button::Close)->setObjectName("FSExportProgressDialog_mCloseBtn");
    mpProgressBar->setObjectName("FSExportProgressDialog_mpProgressBar");
    mpLabelStep->setObjectName("FSExportProgressDialog_mpLabelStep");
    mpLabelLeaveTime->setObjectName("FSExportProgressDialog_mpLabelStep");
    mpLabelElapsedTime->setObjectName("FSExportProgressDialog_mpLabelStep");
    mpButtonPauseResume->setObjectName("FSExportProgressDialog_mpButtonPauseResume");
    mpButtonCancel->setObjectName("FSExportProgressDialog_mpButtonCancel");
    mpButtonFindTarget->setObjectName("FSExportProgressDialog_mpButtonFindTarget");
    setObjectName("FSExportProgressDialog");

}

void FSExportProgressDialog::ConnectSignals()
{
    connect(mpButtonCancel, &FSButton::clicked, this, &FSExportProgressDialog::OnCancelCilckedSlot);
    connect(mpButtonPauseResume, &FSButton::clicked, this, &FSExportProgressDialog::OnPauseResumeClickedSlot);
    connect(mpButtonFindTarget, &FSButton::clicked, this, &FSExportProgressDialog::OnFindTargetClickedSlot);
}

void FSExportProgressDialog::TranslateUI()
{
    //mpLabelStep->setText(tr("Step: Converting..."));
    mpLabelStep->setText("");
    mpLabelStep->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    mpButtonCancel->setText(tr("Cancel"));

    mpButtonPauseResume->setText(tr("Pause"));
    mpButtonFindTarget->setText(tr("Find Target"));

    mpButtonPauseResume->setProperty("pause", tr("Pause"));
    mpButtonPauseResume->setProperty("Resume", tr("Resume"));

}

void FSExportProgressDialog::OnCancelCilckedSlot()
{
    mpExportHelperMgr->Pause();
    if (mpExportHelperMgr->IsConverting())
    {
         int reg = FSMessageBox::questionYesNo(tr("Filmora Scrn"), tr("File is still creating, do you want to abort?"), this);
        if (reg == FSMessageBox::Yes)
        {
            mpExportHelperMgr->Stop();
            QFile::remove(mpExportHelperMgr->GetFileName());
            done(ExitCode_Cancel);
            return;
        }
        else
        {
            mpExportHelperMgr->Resume();
        }
        return;
    }  
    mpExportHelperMgr->Stop();
    done(ExitCode_Cancel);
}

void FSExportProgressDialog::OnPauseResumeClickedSlot()
{
    if (mpButtonPauseResume->isChecked())
    {
        mpExportHelperMgr->Pause();
        mpButtonPauseResume->setText(tr("Resume"));
    }
    else
    {        
        mpExportHelperMgr->Resume();
        mpButtonPauseResume->setText(tr("Pause"));
    }
}

void FSExportProgressDialog::OnFindTargetClickedSlot()
{
    FSFindTargetFile(mpExportHelperMgr->GetFileName());
    done(ExitCode_Finish);
}

void FSExportProgressDialog::OnExportHelperStatesChanagedSlot(FSExportHelper::FSEXPORT_STATE state, int progress)
{
    switch (state)
    {
    case FSExportHelper::EXPORT_STATE_PROGRESS:
    {
        QString stepText = QString(tr("Converting file %1")).arg(QFileInfo(mpExportHelperMgr->GetFileName()).fileName());
        QString stepTextElide = StringOperation::GetElidedString(mpLabelStep->font(), mpLabelStep->width(), stepText);
        mpLabelStep->setText(stepTextElide);
        mpLabelStep->setToolTip(stepTextElide);
        mpProgressBar->setValue(progress);
        mpLabelLeaveTime->setText(mpFPSHelper->GetLeaveTime(progress));
        mpLabelElapsedTime->Start();

        //mpFPSHelper->GetLeaveTime(progress);
    }
        break;
    case FSExportHelper::EXPORT_STATE_OCCUR_ERROR:
    {
        m_bErrorOccur = true;
        if (mpExportHelperMgr->HaveUnFinsihedTask())
        {
            if (FSExportSettingDataMgr::GetInstance()->GetLocalSettingData()->GetShowMessageBoxFlag())
            {
                auto reg = FSConfirmDlg::question(tr("Filmora Scrn"), QString(tr("Export file %1 failed, do you want to continue exporting other files?")).arg(QFileInfo(mpExportHelperMgr->GetFileName()).fileName()), this);
                FSExportSettingDataMgr::GetInstance()->GetLocalSettingData()->SetShowMessageBoxFlag(!reg.first);
                if (reg.second == FSMessageBox::Button::Yes)
                {
                    mpExportHelperMgr->Start();
                    break;
                }
                else
                {
                    mpButtonPauseResume->setVisible(false);
                    mpButtonFindTarget->setVisible(true);
                    mpLabelStep->setText(QString(tr("Completed,success: %1 files, failure: %2 files.")).arg(mpExportHelperMgr->GetSuccessFileNum()).arg(mpExportHelperMgr->GetFailedFileNum()+mpExportHelperMgr->TaskNum()));
                    resizeEvent(NULL);
                    break;
                }
            }
            else
            {
                mpExportHelperMgr->Start();
            }
        }
        else
        {
            OnFinished();
        }
    }
        break;
    default:
        break;
    }
}

//void FSExportProgressDialog::OnThumbnail(QPixmap  pix)
//{
//    //mpLabelMovie->setPixmap(pix.scaled(mpLabelMovie->width(), mpLabelMovie->height()));
//}

void FSExportProgressDialog::OnFinished()
{
    mpButtonPauseResume->setVisible(false);
    mpButtonFindTarget->setVisible(mpExportHelperMgr->GetSuccessFileNum()>0?true:false);
    mpButtonCancel->setText(tr("Close"));
    mpLabelStep->setText(m_bErrorOccur ? QString(tr("Completed,success: %1 files, failure: %2 files.")).arg(mpExportHelperMgr->GetSuccessFileNum()).arg(mpExportHelperMgr->GetFailedFileNum()) : QString(tr("Completed.")));
    //mpLabelStep->setText("");
    mpLabelLeaveTime->setText("");
    mpLabelElapsedTime->End();
    resizeEvent(NULL);
	if (m_bErrorOccur)
	{
      //如果导出过程中编码错误，则发信息
		emit singalExportFail();
	}
	//上传导出文件所消耗的时间
	int iLoadElaspse = GAThread::getInstance()->getElapse();
	GAThread::getInstance()->insertEventItem(ExportTimer, "ExportTime", QString::number(iLoadElaspse));
}


////////////////////////////////////////////////////////////////////////////////////////////

FSConfirmDlg::FSConfirmDlg(const QString &title, const QString &text, int buttons, QWidget *parent)
    :FSMessageBox(title, text,Qt::AlignRight , buttons, parent)
{
    m_pCbDontShow = new QCheckBox(this);
    m_pLbDonShow = new QLabel(this);
    m_pLbDonShow->setText(tr("Don't show it next time."));
    m_pLbDonShow->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_pLbDonShow->setObjectName("FSConfirmDlg_m_pLbDonShow");
    m_pCbDontShow->setObjectName("FSConfirmDlg_m_pCbDontShow");
}

QPair <bool, FSMessageBox::Button> FSConfirmDlg::question(const QString & title, const QString & text, QWidget * parent)
{
    FSConfirmDlg dlg(title,text,Button::Yes|Button::No,parent);
    int button = dlg.DoModal();
    bool bCheck = dlg.IsCheck();
    return {bCheck, (FSMessageBox::Button)button};
}

bool FSConfirmDlg::IsCheck()
{
    return m_pCbDontShow->isChecked();
}

void FSConfirmDlg::resizeEvent(QResizeEvent *e)
{
    FSMessageBox::resizeEvent(e);
    QPoint lb = mapFromGlobal(m_lbText->mapToGlobal(m_lbText->geometry().bottomLeft()));
    int l = m_lbIcon->width() + 53;
    int t = m_lbIcon->height()+ 28;
    m_pCbDontShow->setGeometry(l, t, 20, 20);
    m_pLbDonShow->setGeometry(m_pCbDontShow->geometry().right() + 5, t, width() - m_pCbDontShow->geometry().right() - 5, 20);
}

//////////////////////////////////////////////////////////////////////////
ElapsedTimeLabel::ElapsedTimeLabel(QWidget *parent)
:FSLabel(parent)
, m_second(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [&](){
        SetElapsedTime(++m_second);
    });
}

void ElapsedTimeLabel::Start()
{
    if (!m_timer->isActive())
    {
        m_second = 0;
        SetElapsedTime(m_second);
        m_timer->start(1000);
    }

}

void ElapsedTimeLabel::End()
{
    m_timer->stop();
}

void ElapsedTimeLabel::SetElapsedTime(int second)
{
    setText(QString("%1 %2").arg(tr("Elapsed time:"), -20, QChar(' ')).arg(gFrame2StringSecond(second*25)));

}
