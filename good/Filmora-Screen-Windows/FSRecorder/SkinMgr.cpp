#include "stdafx.h"
#include "SkinMgr.h"
#include "inc_CommonLib/externalapi.h"
#include "inc_FSCommonlib/ExternalAPI.h"



SkinMgr::SkinMgr()
{
	m_type = SKIN_TYPE_LIGHT;
	m_ResMap[SKIN_TYPE_LIGHT] = ":/FSRecorder/Resources/styles_light.css";
	//m_ResMap[SKIN_TYPE_DARK] = ":/FSRecorder/Resources/styles_dark.css";
}

SkinMgr::~SkinMgr()
{
}

SkinMgr* SkinMgr::GetInstance()
{
	static SkinMgr ins;
	return &ins;
}

void SkinMgr::SetSkin(SKIN_TYPE type)
{
	QString qss = gGetStyleSheet(m_ResMap[type]);
    QString MediaPlayerQss = gGetStyleSheet(":/FSMediaPlayerView/Resources/styles_light.qss");
	QString FSExportQss = gGetStyleSheet(":/FSExport/Resources/styles_light.qss");
    QString CommonQss = gGetCommonStyleSheet();
	qApp->setStyleSheet("");
	qApp->setStyleSheet(CommonQss+MediaPlayerQss+FSExportQss+qss);
}

void SkinMgr::SetInitType(SKIN_TYPE type)
{
	m_type = type;
}

SkinMgr::SKIN_TYPE SkinMgr::GetInitType()
{
	return m_type;
}
