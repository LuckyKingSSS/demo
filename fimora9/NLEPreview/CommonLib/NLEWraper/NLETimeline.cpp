#include "stdafx.h"
#include "NLETimeline.h"
#include "INLETimelineBase.h"
#include "NLECommon.h"
#include "NLEWraper.h"
#include "PlayerMaterProvider.h"
#include <QPoint>
#include <QRect>
#include <QImage>
#include "NLEKey.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
NLETimeline::NLETimeline(NLEComPtr<INLETimelineBase> pSrcTimeline, ExactType exacttype, QObject *parent)
	: QObject(parent)
{
	m_ExactType = exacttype;
	m_pSrcTimeLine = pSrcTimeline;
	m_pProp = NLEWraper::GetInstance().GetPlayerMaterProvider()->NLEGetProperties(m_pSrcTimeLine);//NLECommon::NLEGetProperties(m_pSrcTimeLine);
}

NLETimeline::~NLETimeline()
{
}

NLETimeline* NLETimeline::Clone()
{
	NLEComPtr<INLETimelineBase> pCloned = NLEWraper::GetInstance().GetPlayerMaterProvider()->CloneTimeline(m_pSrcTimeLine);
	NLETimeline* pNewOne = new NLETimeline(pCloned,m_ExactType);
	pNewOne->m_strAncestralGuid = m_strAncestralGuid;
	return pNewOne;
}

void NLETimeline::SetName(const QString &strName)
{
	const wchar_t* pName = reinterpret_cast<const wchar_t*>(strName.utf16());
	m_pProp->SetName(pName);
}

QString NLETimeline::GetName() const
{
	wchar_t name[256];
	m_pProp->GetName(name,256);
	return QString::fromWCharArray(name);
}

void NLETimeline::SetLevel(int nLevel)
{
	m_pProp->SetLevel(nLevel);
}

void NLETimeline::SetPosition(int nPos)
{
	m_pProp->SetPosition(nPos);
}

int NLETimeline::GetPosition()
{
	int pos = 0;
	m_pProp->GetPosition(&pos);
	return pos;
}

void NLETimeline::SetRange(int nTrimin, int nTrimout)
{
	NLERange range(nTrimin, nTrimout);
	m_pProp->Render()->SetRenderRange(range);
}

QPoint NLETimeline::GetRange()
{
	NLERange range;
	if (SUCCEEDED(m_pProp->GetRenderRange(&range)))
	{
		return QPoint(range.Start, range.End);
	}
	return QPoint(0, 0);
}

int NLETimeline::GetTotalLength()
{
	int nCnt;

	NLEPropComPtr pProp = NLECommon::NLEGetProperties(m_pSrcTimeLine);
	return SUCCEEDED(pProp->Render()->GetRenderTotalFrameCount(&nCnt)) ? nCnt : 0;
}

GUID NLETimeline::GetGuid()
{
	GUID guid;
	m_pSrcTimeLine->GetGUID(&guid);

	return guid;
}

QString NLETimeline::GetGuidString()
{
	GUID guid = GetGuid();
	QUuid uuid(guid);
	return uuid.toString();
}

void NLETimeline::SetTrimin(int nTrimin)
{
	NLERange range;
	if (!SUCCEEDED(m_pProp->GetRenderRange(&range))) return;
	range.Start = nTrimin;
	m_pProp->Render()->SetRenderRange(range);
}

int NLETimeline::GetTrimin()
{
	NLERange range;
	return SUCCEEDED(m_pProp->GetRenderRange(&range)) ? range.Start : 0;
}

void NLETimeline::SetTrimout(int nTrimout)
{
	NLERange range;
	if (!SUCCEEDED(m_pProp->GetRenderRange(&range))) return;
	range.End = nTrimout;
	m_pProp->Render()->SetRenderRange(range);
}

int NLETimeline::GetTrimout()
{
	NLERange range;
	return SUCCEEDED(m_pProp->GetRenderRange(&range)) ? range.End : 0;
}

int NLETimeline::GetPlayLen()
{
	NLERange range;
	return SUCCEEDED(m_pProp->Render()->GetRenderRange(&range)) ? range.End - range.Start + 1 : 0;
}

int NLETimeline::GetSrcLen()
{
	NLERange range;
	return SUCCEEDED(m_pProp->GetSourceRange(&range)) ? range.Count() : 0;
}

bool NLETimeline::GetRender(INLERender** ppVideoRender, INLERender** ppAudioRender)
{
	return SUCCEEDED(m_pSrcTimeLine->GetRender(ppAudioRender, ppVideoRender));
}

void NLETimeline::SetRenderSize(int nDx, int nDy)
{
	m_pProp->SetRenderSize(NLESize(nDx, nDy));
}

QSize NLETimeline::GetRenderSize()
{
	NLESize size;
	m_pProp->GetRenderSize(&size);
	return QSize(size.Width, size.Height);
}

QRectF NLETimeline::GetRenderRegion()
{
    NLERectF rf;
    m_pProp->GetRectF(kRender_Region, &rf);
    return QRectF(rf.X, rf.Y, rf.Width, rf.Height);
}

void NLETimeline::SetRenderRegion(const QRectF  &rc)
{
    m_pProp->SetRectF(kRender_Region, NLERectF(rc.left(), rc.top(), rc.width(), rc.height()));
}

QRectF NLETimeline::GetRealRenderRegion(QRectF  baseRc)
{
    QRectF realRc, realBaseRc;
    if (baseRc.isNull())
        return realRc;
    QRectF rc = GetRenderRegion();
    QSizeF  size = GetRenderSize();
    QSizeF baseSize = baseRc.size();
    if (rc.width() < 1e-8 || rc.height() < 1e-8)
        return realRc;
    double offsetx = 0;
    double offsety = 0;
    if (size.width()*baseSize.height() > size.height()*baseSize.width())
    {
        offsety = (baseRc.height() - baseRc.width()*size.height() / size.width()) / 2;
        realBaseRc.setTopLeft(QPointF(0.0, 0));
        realBaseRc.setWidth(baseRc.width());
        realBaseRc.setHeight(baseRc.width()*size.height() / size.width());
    }
    else
    {
        offsetx = (baseRc.width() - baseRc.height()*size.width() / size.height()) / 2;
        realBaseRc.setTopLeft(QPointF(0, 0.0));
        realBaseRc.setWidth(baseRc.height()*size.width() / size.height());
        realBaseRc.setHeight(baseRc.height());
    }
    return QRectF(rc.left()*realBaseRc.width(), rc.top()*realBaseRc.height(), rc.width()*realBaseRc.width(), rc.height()*realBaseRc.height());
}


void NLETimeline::SetRenderResizeScale(QSizeF scale)
{
    m_pProp->SetSizeF(kRender_ResizeScale, NLESizeF(scale.width(), scale.height()));
}

QSizeF NLETimeline::GetRenderResizeScale()
{
    NLESizeF NLEsize;
    m_pProp->GetSizeF(kRender_ResizeScale, &NLEsize);
    return QSizeF(NLEsize.Width, NLEsize.Height);
}

void NLETimeline::SetRenderMode(NLERenderMode mode)
{
    m_pProp->SetInt(kRender_Mode, mode);

}

void NLETimeline::SetHiden(bool bHide)
{
	m_pProp->SetInt(kRender_Hidden, bHide ? 1 : 0);
}

bool NLETimeline::IsRenderScaleEnable()
{
	int enable;
	m_pProp->GetInt(NLEKey::Render::kRender_Scale_Enable, &enable);
	return enable == 1;
}

void NLETimeline::SetRenderScaleEnable(bool bEnable)
{
	m_pProp->SetInt(NLEKey::Render::kRender_Scale_Enable, bEnable ? 1 : 0);
}

float NLETimeline::GetRenderScale()
{
	NLERational scale;
	m_pProp->GetRational(NLEKey::Render::kRender_Scale, &scale);
	return scale.Value();
}

void NLETimeline::SetRenderScale(float scale)
{
	NLERational s(scale * 100000000, 100000000);
	m_pProp->SetRational(NLEKey::Render::kRender_Scale, s);
}

void NLETimeline::SetRenderBackgroundColor(QColor color)
{
    auto c = qRgba(color.red(), color.green(), color.blue(), color.alpha());
    m_pProp->SetInt(NLEKey::Render::kRender_BackgroundColor, c);
}

QColor NLETimeline::GetRenderBackgroundColor()
{
    INT c;
    m_pProp->GetInt(NLEKey::Render::kRender_BackgroundColor, &c);
    return QColor::fromRgba(c);
}

void NLETimeline::SetCropRegion(const QRectF  &rc)
{
	m_pProp->SetSourceRegion(NLERectF(rc.left(),rc.top(),rc.width(),rc.height()));
}

QRectF NLETimeline::GetCropRegion()
{
	NLERectF rc;
	m_pProp->GetSourceRegion(&rc);
	return QRectF(rc.X, rc.Y, rc.Width, rc.Height);
}

void NLETimeline::GetTimelineBase(INLETimelineBase **ppTimelineBase)
{
	*ppTimelineBase = m_pSrcTimeLine;
}

NLEComPtr<INLETimelineBase> NLETimeline::GetSrc()
{
	return m_pSrcTimeLine;
}

void NLETimeline::SetAncestralGuid(const QString &strGuid)
{
	m_strAncestralGuid = strGuid;
}

QString NLETimeline::GetAncestralGuid() const
{
	return m_strAncestralGuid;	
}

float NLETimeline::GetSourceFramerate()
{
	NLERational rational;
	m_pProp->GetSourceFramerate(&rational);

	if (rational.den == 0) return 0.0;
	return 1.0 * rational.num / rational.den;
}

float NLETimeline::GetRenderFramerate()
{
	NLERational rational;
	m_pProp->GetRenderFrameRate(&rational);
	if (rational.den == 0) return 0.0;
	return 1.0 * rational.num / rational.den;
}

bool NLETimeline::IsFrameIn(int nFrame)
{
	int nStart = 0;
	HRESULT hr = m_pProp->GetPosition(&nStart);
	if (!SUCCEEDED(hr)) return false;
	
	int nLen = GetPlayLen();
	if (nFrame >= nStart && nFrame < nStart + nLen);
	{
		return true;
	}
	return false;
}

int NLETimeline::GetCurrentFrame()
{

	INLERender *pVideoRender = NULL;
	INLERender *pAudioRender = NULL;
	m_pSrcTimeLine->GetRender(&pAudioRender, &pVideoRender);
	if (!pVideoRender)
		return -1;

	return pVideoRender->GetOffset();
}

QImage NLETimeline::GetCurrentFrameImg(int nFrameW,int nFrameH)
{
	INLERender *pVideoRender = NULL;
	INLERender *pAudioRender = NULL;
	HRESULT hr = m_pSrcTimeLine->GetRender(&pAudioRender, &pVideoRender);
	if (!SUCCEEDED(hr)) return QImage();

	if (!pVideoRender) return QImage();

	NLEComPtr<INLEFrame> pVideoFrame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFrame(nFrameW, nFrameH);
	if (!pVideoFrame) return QImage();

	int nPl = GetPlayLen();
	int nSl = GetSrcLen();
	int offset = pVideoRender->GetOffset();

	bool bRender = true;
	bRender &= SUCCEEDED(pVideoRender->Prepare(pVideoFrame));
	bRender &= SUCCEEDED(pVideoRender->Render(pVideoFrame));

	if (!bRender) return QImage();

	NLEMediaInfo Info;
	pVideoFrame->GetMediaInfo(&Info);
	QImage img((uchar*)pVideoFrame->GetData(), Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
	return img;
}

QPointF NLETimeline::GetRegionCenter()
{
	NLEPointF p;
	HRESULT hr = m_pProp->GetPointF(kRender_AnchorPointF, &p);
	if (!SUCCEEDED(hr)) return QPointF();
	return QPointF(p.X, p.Y);
}

void NLETimeline::SetRegionCenter(QPointF center)
{
	NLEPointF p(center.x(),center.y());
	HRESULT hr = m_pProp->SetPointF(kRender_AnchorPointF, p);
}

double NLETimeline::GetRegionRotateAngle()
{
	double angle;
	HRESULT hr = m_pProp->GetDouble(kRender_RotationAngle, &angle);
	if (!SUCCEEDED(hr)) return 0;
	return angle;
}

void NLETimeline::SetRegionRotateAngle(double angle)
{
	HRESULT hr = m_pProp->SetDouble(kRender_RotationAngle, angle);
}

QSizeF NLETimeline::GetRegionScale()
{
	NLESizeF size;
	HRESULT hr = m_pProp->GetSizeF(kRender_ResizeScale, &size);
	if (!SUCCEEDED(hr)) QSizeF(0.0,0.0);
	return QSizeF(size.Width, size.Height);
}

void NLETimeline::SetRegionScale(QSizeF size)
{
	HRESULT hr = m_pProp->SetSizeF(kRender_ResizeScale, NLESizeF(size.width(), size.height()));
}

QPixmap NLETimeline::GetFramePixmap(int nFrame, int nW, int nH)
{
	INLERender *pVideoRender = NULL;
	INLERender *pAudioRender = NULL;
	NLEWraper::GetInstance().GetPlayerMaterProvider()->GetMediaRender(GetSrc(), &pVideoRender, &pAudioRender);
	if (!pVideoRender) return QPixmap();

	NLEComPtr<INLEFrame> pFrame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFrame(nW, nH);
	if (!pFrame) return QPixmap();


	int nPlayLen = GetPlayLen();

	pVideoRender->Seek(nFrame);
	pVideoRender->Prepare(pFrame);
	pVideoRender->Render(pFrame);
	if (!pFrame) return QPixmap();

	void* pData = pFrame->GetData();
	int nDataLen = pFrame->GetDataSize();

	NLEMediaInfo Info;
	pFrame->GetMediaInfo(&Info);

	QImage img((uchar*)pData, Info.video.nWidth, Info.video.nHeight, QImage::Format_ARGB32);
	QPixmap pix = QPixmap::fromImage(img).scaled(nW, nH);

	return pix;
}

void NLETimeline::SetRenderFrameRate(float frameRate)
{
	NLERational rational(frameRate);
	m_pProp->SetRenderFrameRate(rational);
}

float NLETimeline::GetRenderFrameRate()
{
	NLERational rational;
	m_pProp->GetRenderFrameRate(&rational);
	return rational;
}

ExactType NLETimeline::GetExactType() const
{
	return m_ExactType;
}

int NLETimeline::GetDuration()
{
	int frames;
	m_pProp->GetInt(NLEKey::Render::kRender_TotalFrameCount, &frames);

	return frames / GetRenderFramerate();
}

void NLETimeline::AddClip(NLETimeline* pClip, int nStartPos, int nLevel)
{
	//auto d = pClip->GetTotalLength();
	m_pSrcTimeLine->AddClip(pClip->m_pSrcTimeLine);
	pClip->SetLevel(nLevel);
	pClip->SetPosition(nStartPos);

	//auto dd = pClip->GetTotalLength();
}

void NLETimeline::RemoveClip(NLETimeline* pClip)
{
	m_pSrcTimeLine->RemoveClip(pClip->GetSrc());
}

void NLETimeline::SetClipLevel(NLETimeline* pClip, int nLevel)
{
	if (!IsClipExist(pClip)) return;
	pClip->SetLevel(nLevel);
}

void NLETimeline::SetClipPos(NLETimeline* pClip, int nStartPos)
{
	if (!IsClipExist(pClip)) return;
	pClip->SetPosition(nStartPos);
}

void NLETimeline::SetClipRange(NLETimeline* pClip, int nTrimin, int nTrimout)
{
	if (!IsClipExist(pClip)) return;
	pClip->SetRange(nTrimin, nTrimout);
}

int NLETimeline::GetClipCount()
{
	int nCnt = 0;
	m_pSrcTimeLine->GetClipCount(&nCnt);
	return nCnt;
}

int NLETimeline::GetVolume()
{
    int volume=0;
    m_pProp->GetInt(NLEKey::PostProcess::kVolume, &volume);
    return volume;
}

void NLETimeline::SetVolume(int volume)
{
    volume = qBound(-100, volume, 100);
    m_pProp->SetInt(NLEKey::PostProcess::kVolume, volume);
}

NLEPropComPtr NLETimeline::GetProp()
{
    return m_pProp;
}

int NLETimeline::GetLevel()
{
    int level;
    m_pProp->GetInt(NLEKey::Base::kLevel, &level);
    return level;
}

QSizeF NLETimeline::GetSourceSize()
{
    NLESize NLEsize;
    m_pProp->GetSize(kSource_Size, &NLEsize);
    return QSizeF(NLEsize.Width, NLEsize.Height);
}

bool NLETimeline::IsClipExist(NLETimeline* pClip)
{
	if (!pClip) return false;
	NLEComPtr<INLETimelineBase> pTimeline = NULL;
	m_pSrcTimeLine->GetClip(pClip->GetGuid(), &pTimeline);
	return pTimeline != NULL;
}


