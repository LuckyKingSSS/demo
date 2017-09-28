#include "stdafx.h"
#include "inc_FSExport/IFSExport.h"
#include "inc_CommonLib/TopLevelWindowTitleBar.h"
#include "FSExport.h"
#include "inc_CommonLib/CommonWidgets.h"
#include "inc_CommonLib/XmlOperation.h"
#include "inc_FSCommonlib/ExternalAPI.h"

#include "inc_MediaPlayerView/MediaPlayerControl.h"
#include "inc_FSCommonLib/FSNLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/externalapi.h"
#include "NLEType.h"
#include "FSExportSettingData.h"

IFSExport::IFSExport(QList<FSNLETimeline *>pTimelinelist, QWidget *parent /*= 0*/)
    :FSBaseDialog(tr("Export"), "Export", parent)
    , m_pFSExport(NULL)
{
    Q_ASSERT(parent);
    QString settingPath = FSGetFilmoraScreenDirectory() + "/settingdatas.xml";
    QFile f(settingPath);
    if (f.open(QIODevice::ReadOnly))
    {
        FSExportSettingDataMgr::GetInstance()->LoadingExportSetting(f.readAll());
    }
    setAttribute(Qt::WA_StyledBackground);
    m_pFSExport = new FSExport(pTimelinelist, this);
    connect(m_pFSExport, &FSExport::SigHide, this, &IFSExport::hide);
    connect(m_pFSExport, &FSExport::SigDone, this, [&](int ret){
        parentWidget()->activateWindow();
        done(ret);
    });
    connect(m_pFSExport, &FSExport::RegisterResaultSig, this,&IFSExport::RegisterResaultSig);
    //connect(m_pFSExport, &FSExport::SigDone, this, &IFSExport::done);
    m_titlebar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_titlebar->SetButtons(TopLevelWindowTitleBar::Button::Close);
    m_titlebar->SetResizeble(false);
    m_titlebar->setFixedHeight(34);
    m_titlebar->GetButton(TopLevelWindowTitleBar::Button::Close)->setObjectName("IFSExport_mCloseBtn");
    m_titlebar->setObjectName("IFSExport_m_titlebar");
    setMinimumSize(788, 622);

}

IFSExport::~IFSExport()
{
    QString settingPath = FSGetFilmoraScreenDirectory() + "/settingdatas.xml";
    settingPath = QDir::toNativeSeparators(settingPath);
    FSExportSettingDataMgr::GetInstance()->SaveExportSettingData(settingPath);
    FSExportSettingDataMgr::GetInstance()->Release();
}

void IFSExport::SetRecorderExportState(bool bRecorderState)
{
    m_pFSExport->SetRecorderExportState(bRecorderState);
}

void IFSExport::SetExportFileName(QString name)
{
    m_pFSExport->SetExportFileName(name);
}

void IFSExport::SetDefaultExportRsolution(QSize resolution)
{
    m_pFSExport->SetDefaultExportRsolution(resolution);
}

void IFSExport::resizeEvent(QResizeEvent *event)
{
	const int margins = 1;
	m_titlebar->setGeometry(margins, margins, width() - 2 * margins, m_titleHeight);
    m_pFSExport->setGeometry(margins, m_titlebar->geometry().bottom() + 1, width() - 2 * margins, height() - m_titlebar->height() - 2 * margins);
}

void IFSExport::RetranslateUi()
{
    SetTitle(tr("Export"));
}

QString gGetExportStyleSheet()
{
    return gGetStyleSheet(":/FSExport/Resources/styles_light.qss");
}
