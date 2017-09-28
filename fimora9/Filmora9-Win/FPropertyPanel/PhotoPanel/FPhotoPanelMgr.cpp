#include "stdafx.h"
#include "FPhotoPanelMgr.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_FProject/FNLETimelineFactory.h"

FPhotoPanelMgr::FPhotoPanelMgr(QObject *parent)
	: FPanelMgr(parent)
	, m_pFilter(0)
{

}

FPhotoPanelMgr::~FPhotoPanelMgr()
{

}

void FPhotoPanelMgr::RotateReset()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	if (m_pFilter){  // È¥³ýÂË¾µ
		pTimeline->RemoveClip(m_pFilter);
		delete m_pFilter;
		m_pFilter = 0;
	}
	auto pProp = pTimeline->GetProp();
	pProp->SetDouble(NLEKey::Render::kRender_RotationAngle, 0);  // Ðý×ª½Ç¶È¹éÁã
}

void FPhotoPanelMgr::SetRotate(FPropertyRotate::RotateMode mode)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	DOUBLE pAngle[10];
	switch (mode)
	{
	case FPropertyRotate::Rotate90Clockwise:
		pProp->GetDouble(NLEKey::Render::kRender_RotationAngle, pAngle);
		pProp->SetDouble(NLEKey::Render::kRender_RotationAngle, 90 + *pAngle);
		break;
	case FPropertyRotate::Rotate90AntiClockwise:
		pProp->GetDouble(NLEKey::Render::kRender_RotationAngle, pAngle);
		pProp->SetDouble(NLEKey::Render::kRender_RotationAngle, -90 + *pAngle);
		break;
	case FPropertyRotate::FlipHorizontal:
		RotateFlipHorizontal();
		break;
	case FPropertyRotate::FlipVertical:
		RotateFlipVertical();
		break;
	default:
		break;
	}
}

int FPhotoPanelMgr::GetContrastValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kContrast, &value);
	return value;
}

void FPhotoPanelMgr::SetContrastValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kContrast, value);
}

int FPhotoPanelMgr::GetSaturationValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kSaturation, &value);
	return value;
}

void FPhotoPanelMgr::SetSaturationValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kSaturation, value);
}

int FPhotoPanelMgr::GetBrightnessValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kBrightness, &value);
	return value;
}

void FPhotoPanelMgr::SetBrightnessValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kBrightness, value);
}

int FPhotoPanelMgr::GetTintValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kHue, &value);
	return value;
}

void FPhotoPanelMgr::SetTintValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kHue, value);
}

bool FPhotoPanelMgr::Enhanced()
{
	return true;
}

void FPhotoPanelMgr::EnableEnhance(bool enabled)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
}

int FPhotoPanelMgr::GetDuration()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	return pTimeline->GetDuration();
}

void FPhotoPanelMgr::SetDuration(INT64 value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetStreamLength(value);
	//pProp->SetInt64(NLEKey::Video::kDuration, value);
}

void FPhotoPanelMgr::DurationApplyToAll()
{
	auto timeline = GetTimeline();
	auto rootTimeline = GetRootTimeline();
}

void FPhotoPanelMgr::RotateFlipHorizontal()
{
	m_pFilter =  FNLETimelineFactory::GetInstance()->CreateTimeline((NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_Mirror))));
	m_pFilter->SetRange(0, GetTimeline()->GetTotalLength());
	GetTimeline()->AddClip(m_pFilter, 0, 1);
}

void FPhotoPanelMgr::RotateFlipVertical()
{
    m_pFilter = FNLETimelineFactory::GetInstance()->CreateTimeline(NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_FlipUp)));
	m_pFilter->SetRange(0, GetTimeline()->GetTotalLength());
	GetTimeline()->AddClip(m_pFilter, 0, 1);
}



