#include "stdafx.h"
#include "FMotionPannelMgr.h"

FMotionPannelMgr::FMotionPannelMgr(QObject *parent)
	: QObject(parent)
{

}

FMotionPannelMgr::~FMotionPannelMgr()
{

}

QList<pMotionInfo> FMotionPannelMgr::GetAllResource(QString strFilePath)
{
	QList<pMotionInfo> Allinfo;
	pMotionInfo info = new MotionInfo();
	info->strMotionName = "test";
	info->strPicPath = "G:\\Code\\Filmora-Screen-Windows\\bin\\x64\\Release\\MotionTemplate\\Bounce to zoom in 1.JPG";
	info->strXmlPath = "G:\\Code\\Filmora-Screen-Windows\\bin\\x64\\Release\\MotionTemplate\\MotionTemplate.xml";

	for (int i = 0; i < 20;i++)
	{
		Allinfo.push_back(info);
	}
	return Allinfo;
}
