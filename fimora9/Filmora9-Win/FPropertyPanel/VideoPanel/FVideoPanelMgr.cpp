#include "stdafx.h"
#include "FVideoPanelMgr.h"
#include "inc_FCommonLib/FCommonWidgets.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_FProject/FNLETimelineFactory.h"

FVideoPanelMgr::FVideoPanelMgr(QObject *parent /*= 0*/)
: FPanelMgr(parent)
, m_pFilter(0)
{
	
}

FVideoPanelMgr::~FVideoPanelMgr()
{
	
}

void FVideoPanelMgr::RotateReset()
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

void FVideoPanelMgr::SetRotate(FPropertyRotate::RotateMode mode)
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

int FVideoPanelMgr::GetContrastValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kContrast, &value);
	return value;
}

void FVideoPanelMgr::SetContrastValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kContrast, value);
}

int FVideoPanelMgr::GetSaturationValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kSaturation, &value);
	return value;
}

void FVideoPanelMgr::SetSaturationValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kSaturation, value);
}

int FVideoPanelMgr::GetBrightnessValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kBrightness, &value);
	return value;
}

void FVideoPanelMgr::SetBrightnessValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kBrightness, value);
}

int FVideoPanelMgr::GetTintValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	INT value;
	pProp->GetInt(NLEKey::PostProcess::kHue, &value);
	return value;
}

void FVideoPanelMgr::SetTintValue(int value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetInt(NLEKey::PostProcess::kHue, value);
}

bool FVideoPanelMgr::Enhanced()
{
	return true;
}

void FVideoPanelMgr::EnableEnhance(bool enhanced)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
}

float FVideoPanelMgr::GetSpeedValue()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	NLERational value;
	pProp->GetRenderScale(&value);
	return value;
}

void FVideoPanelMgr::SetSpeedValue(float value)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetRenderScaleEnable(true);
	pProp->SetRenderScale(value);
}

bool FVideoPanelMgr::PlayInReverse()
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	BOOL res;
	pProp->GetRenderBackplay(&res);
	return res;
}

void FVideoPanelMgr::EnablePlayInReverse(bool enabled)
{
	auto pTimeline = GetTimeline();
	Q_ASSERT(pTimeline);
	auto pProp = pTimeline->GetProp();
	pProp->SetRenderBackplay(enabled);
}

void FVideoPanelMgr::RotateFlipHorizontal()
{
    m_pFilter = FNLETimelineFactory::GetInstance()->CreateTimeline(NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_Mirror)));
	m_pFilter->SetRange(0, GetTimeline()->GetTotalLength());
	GetTimeline()->AddClip(m_pFilter, 0, 1);
}

void FVideoPanelMgr::RotateFlipVertical()
{
    m_pFilter = FNLETimelineFactory::GetInstance()->CreateTimeline(NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_FlipUp)));
	m_pFilter->SetRange(0, GetTimeline()->GetTotalLength());
	GetTimeline()->AddClip(m_pFilter, 0, 1);
}




