#include "stdafx.h"
#include "FSExport.h"
#include "FSExportBar.h"
#include "FSExportLocalPanel.h"
#include "inc_FSCommonlib/FSCommonWidget.h"
#include "inc_FSCommonlib/ExternalAPI.h"
#include "FSExportHelperManager.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "FSExportProgressDialog.h"
#include "inc_FSCommonlib/ExportWarringDlg.h"
#include <QtConcurrent/QtConcurrent>
#include "inc_FSCommonlib/gathread.h"
#include "FSExportSettingData.h"
#include "inc_CommonLib/externalapi.h"
#include "FSExportHelper.h"

FSExport::FSExport(QList<FSNLETimeline *>pTimelinelist, QWidget *parent)
    :QWidget(parent)
    , mbRecorderState(true)
    , mpNLETimelineList(pTimelinelist)
	, m_strFileType("")
{
    mpExportBar     = new FSExportBar(this);   
    mpStackedWidget = new QStackedWidget(this);
    mpExportBtn     = new FSButton(this);
    mpExportLocal   = new FSExportLocalPanel(this);  
    mpStackedWidget->addWidget(mpExportLocal);
    mpStackedWidget->setFrameShape(QFrame::NoFrame);
    mpExportBtn->setText(tr("Export"));
    mpExportBtn->setObjectName("Export_mpExportBtn");
    mpStackedWidget->setObjectName("FSExport_mpStackedWidget");
    setObjectName("FSExport");
    mpStackedWidget->setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_StyledBackground);
    ConnectSignals();
}

FSExport::~FSExport()
{

}

void FSExport::SetRecorderExportState(bool bRecorderState)
{
    mbRecorderState = bRecorderState;
    mpExportLocal->SetRecorderExportState(bRecorderState);
}

void FSExport::TransfromGaInfo(FSExportHelperManager *pHelp)
{
	//上传GA数据
	bool bIsGpuAccelerate = FSExportSettingDataMgr::GetInstance()->IsEnabledGPU();
	QList<FSExportHelper *>  helpList = pHelp->getExportHelper();
	QList<FSExportHelper *>::iterator iter = helpList.begin();
	for (; iter != helpList.end(); ++iter)
	{
		(*iter)->TransfromGaInfo(m_strFileType, bIsGpuAccelerate);
	}
}

void FSExport::SetDefaultExportRsolution(QSize resolution)
{
    mpExportLocal->SetDefaultExportRsolution(resolution);
}

void FSExport::SetExportFileName(QString name)
{
    mpExportLocal->SetExportFileName(name);
}

void FSExport::slotExportProfileChange(int iIndex)
{
	//上传GA数据
	if (iIndex == 0)
	{
		m_strFileType = "MP4";
	}
	else if (iIndex == 1)
	{
		m_strFileType = "MOV";
	}
	else
	{
		m_strFileType = "GIF";
	}
}

void FSExport::resizeEvent(QResizeEvent *e)
{
    int btnWidth = 100;
    int btnHeight = 30;
    int bottomHeight = 56;
    mpExportBar->setGeometry(0, 0, mpExportBar->width(), height() - bottomHeight);
    mpStackedWidget->setGeometry(mpExportBar->geometry().right(), 0, width() - mpExportBar->width(), height() - bottomHeight);
    mpExportBtn->setGeometry(width() - btnWidth - 19, height() - btnHeight - 13, btnWidth, btnHeight);
}

void FSExport::ConnectSignals()
{
	connect(mpExportBtn, &FSButton::clicked, this, &FSExport::OnExportClickedSlot); 
	connect(mpExportLocal, &FSExportLocalPanel::signalPageChange, this, &FSExport::slotExportProfileChange);
}

void FSExport::OnExportClickedSlot()
{
    ExportWarringDlg dlg(this);
    FSExportHelperManager *pHelper = new FSExportHelperManager(mpNLETimelineList, mbRecorderState, mpExportLocal->GetExportSettingInfo(), this);  
    int reg = 0;
    if (!dlg.IsRegister())
    {        
        reg=dlg.exec();
        if (reg != 1)
            return;
        emit RegisterResaultSig(dlg.IsRegister());
    }
    pHelper->SetRegisterFlag(dlg.IsRegister());
    if (mpStackedWidget->currentWidget() == mpExportLocal)
    {      
        mpExportLocal->CheckUserInput();
		//上传GA数据
		TransfromGaInfo(pHelper);
		 QFileInfo info(mpExportLocal->GetExportSettingInfo().filename);
         if (!pHelper->IsEnoughDiskSpace(info.absolutePath()))
         {
             FSMessageBox::informationOk(tr("Filmora Scrn"), tr("There isn't enough space for exporting, please delete unnecessary files to free enough space."), this);
			 //上传导出失败
			 GAThread::getInstance()->insertEventItem(Export_Fail, "FileFormat", m_strFileType);
			 GAThread::getInstance()->insertEventItem(Export_Fail, "ExportFailureReason", "There isn't enough space for exporting");
             pHelper->deleteLater();
             return;
         } 
        emit SigHide();
        FSExportProgressDialog dlg1(this);
		connect(&dlg1, &FSExportProgressDialog::singalExportFail, this, [&](){
			GAThread::getInstance()->insertEventItem(Export_Fail, "FileFormat", m_strFileType);
			GAThread::getInstance()->insertEventItem(Export_Fail, "ExportFailureReason", "Encode failed");
		});
        dlg1.SetExportHelperMgr(pHelper);    
		//上传GA数据
		GAThread::getInstance()->startTime();
        int reg = dlg1.exec();
        pHelper->deleteLater();
        emit SigDone(reg);        
    }
}


