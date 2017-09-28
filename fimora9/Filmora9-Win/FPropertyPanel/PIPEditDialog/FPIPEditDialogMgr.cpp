#include "stdafx.h"
#include "FPIPEditDialogMgr.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_FProject/FNLETimeline.h"
#include "inc_FProject/FNLETimelineFactory.h"

FPIPEditDialogMgr::FPIPEditDialogMgr(QObject *parent)
	: FPanelMgr(parent)
	, m_MaskPath(QApplication::applicationDirPath() + "/" + "Mask/Masks/")
	, m_pFilter(0)
{
	Init();
}

FPIPEditDialogMgr::~FPIPEditDialogMgr()
{

}

void FPIPEditDialogMgr::SetMaskPath(QString path)
{
	m_MaskPath = path;
	Init();
}

QStringList FPIPEditDialogMgr::GetMaskThumbFile()
{
	return m_lstMaskThumbFile;
}

QStringList FPIPEditDialogMgr::GetMaskFile()
{
	return m_lstMaskFile;
}
#include <QDebug>
void FPIPEditDialogMgr::SetMaskEffect(int id)
{
	if (id == 0) {
		GetTimeline()->GetProp()->SetString(NLEKey::Render::kRender_MaskFile, L"");
		return;
	}
	wchar_t pWchar[256];
	QString file = m_lstMaskFile[id - 1];
	m_lstMaskFile[id - 1].toWCharArray(pWchar);
	qDebug() << file;
	GetTimeline()->GetProp()->SetString(NLEKey::Render::kRender_MaskFile, pWchar);
}

void FPIPEditDialogMgr::SetMaskWidth(float w)
{

}

void FPIPEditDialogMgr::SetMaskHeight(float h)
{

}

void FPIPEditDialogMgr::SetMaskLeft(float l)
{

}

void FPIPEditDialogMgr::SetMaskTop(float t)
{

}

void FPIPEditDialogMgr::SetCurrentColor(QColor c)
{
	m_CurrentColor = c;
	
}

QColor FPIPEditDialogMgr::GetCurrentColor()
{
	return m_CurrentColor;
}

void FPIPEditDialogMgr::SetIntensityLevel(int intensity)
{
	m_IntensityLevel = intensity;

}

int FPIPEditDialogMgr::GetIntensityLevel()
{
	return m_IntensityLevel;
}

void FPIPEditDialogMgr::SetShadowDistance(int distance)
{
	m_ShadowDistance = distance;

}

int FPIPEditDialogMgr::GetShadowDistance()
{
	return m_ShadowDistance;
}

void FPIPEditDialogMgr::SetShadowBlur(int blur)
{
	m_ShadowBlur = blur;

}

int FPIPEditDialogMgr::GetShadowBlur()
{
	return m_ShadowBlur;
}

void FPIPEditDialogMgr::SetShadowTransparency(int transparency)
{
	m_ShadowTransparency = transparency;

}

int FPIPEditDialogMgr::GetShadowTransparency()
{
	return m_ShadowTransparency;
}

void FPIPEditDialogMgr::SetShadowColor(QColor c)
{
	m_ShadowColor = c;
}

QColor FPIPEditDialogMgr::GetShadowColor()
{
	return m_ShadowColor;
}

void FPIPEditDialogMgr::SetShadowDirection(DIRECTION d)
{
	m_ShadowDirection = d;
}

DIRECTION FPIPEditDialogMgr::GetShadowDirection()
{
	return m_ShadowDirection;
}

void FPIPEditDialogMgr::SetBorderDistance(int bd)
{
	m_BorderDistance = bd;
}

int FPIPEditDialogMgr::GetBorderDistance()
{
	return m_BorderDistance;
}

void FPIPEditDialogMgr::SetBorderBlur(int bb)
{
	m_BorderBlur = bb;
}

int FPIPEditDialogMgr::GetBorderBlur()
{
	return m_BorderBlur;
}

void FPIPEditDialogMgr::SetBorderTransparency(int bt)
{
	m_BorderTransparency = bt;
}

int FPIPEditDialogMgr::GetBorderTransparency()
{
	return m_BorderTransparency;
}

void FPIPEditDialogMgr::SetBorderGradientColorStart(QColor c)
{
	m_GradientColorStart = c;
}

QColor FPIPEditDialogMgr::GetBorderGradientColorStart()
{
	return m_GradientColorStart;
}

void FPIPEditDialogMgr::SetBorderGradientColorEnd(QColor c)
{
	m_GradientColorEnd = c;
}

QColor FPIPEditDialogMgr::GetBorderGradientColorEnd()
{
	return m_GradientColorEnd;
}

void FPIPEditDialogMgr::SetBorderGradientDirection(DIRECTION d)
{
	m_GradientDirection = d;
}

DIRECTION FPIPEditDialogMgr::GetBorderGradientDirection()
{
	return m_GradientDirection;
}

void FPIPEditDialogMgr::SetRotate(FPropertyRotate::RotateMode mode)
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

void FPIPEditDialogMgr::Init()
{
	QDir maskdir(m_MaskPath);
	if (maskdir.exists()){
		QFileInfoList fileInfos = maskdir.entryInfoList(QDir::Files);
		for (auto fileInfo : fileInfos){
			QString file = fileInfo.absoluteFilePath();
			if (file.contains("Thumb")){
				m_lstMaskThumbFile.append(file);
			}
			else{
				m_lstMaskFile.append(file);
			}
		}
	}
}

void FPIPEditDialogMgr::RotateFlipHorizontal()
{
    m_pFilter = FNLETimelineFactory::GetInstance()->CreateTimeline(NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_Mirror)));
	m_pFilter->SetRange(0, GetTimeline()->GetTotalLength());
	GetTimeline()->AddClip(m_pFilter, 0, 1);
}

void FPIPEditDialogMgr::RotateFlipVertical()
{
    m_pFilter = FNLETimelineFactory::GetInstance()->CreateTimeline(NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFilterTimeline(QString::fromWCharArray(NLEKey::Filter::kFilterID_FlipUp)));
	m_pFilter->SetRange(0, GetTimeline()->GetTotalLength());
	GetTimeline()->AddClip(m_pFilter, 0, 1);
}






