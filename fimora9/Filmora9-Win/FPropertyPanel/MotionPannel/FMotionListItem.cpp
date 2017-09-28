#include "stdafx.h"
#include "..\include\inc_CommonLib\NLEWraper.h"
#include "FMotionListItem.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_FProject/FNLETimelineFactory.h"

FMotionListItem::FMotionListItem(const QIcon & icon, const QString & text, QString strFilePath) : QListWidgetItem(icon, text), m_strFilePath(strFilePath)
{
	InitData();
}

FMotionListItem::~FMotionListItem()
{

}

void FMotionListItem::InitData()
{
	NLEComPtr<INLETimelineBase> pTimeline = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateMediaTimeline(m_strFilePath);
	Q_ASSERT(pTimeline);
	//m_Timeline = new FNLETimeline(pTimeline);
    m_Timeline = FNLETimelineFactory::GetInstance()->CreateTimeline(pTimeline);
}
