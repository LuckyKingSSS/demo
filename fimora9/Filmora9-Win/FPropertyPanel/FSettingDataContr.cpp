#include "stdafx.h"
#include "FSettingDataContr.h"

FSettingDataContr::FSettingDataContr(QObject *parent)
	: QObject(parent)
{
	initConfig();
}

FSettingDataContr::~FSettingDataContr()
{

}

void FSettingDataContr::initConfig()
{
	m_DeviceList = { "WMV", "MP4", "AVI", "MOV", "F4V", "MKV", "TS", "3GP", "MPEG-2", "WEBM", "GIF", "MP3" };
}
