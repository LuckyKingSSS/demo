#include "stdafx.h"
#include "inc_FSCommonlib/FSNLETimeline.h"
#include "inc_CommonLib/NLEWraper.h"
#include "inc_CommonLib/PlayerMaterProvider.h"
#include "Include/NLEComPtr.h"
#include "Interface/INLECaption.h"

#include "INLETimelineBase.h"
#include "ICaptureSource.h"


MarkPoint::MarkPoint(QObject *parent) 
	: QObject(parent)
	, m_frame(-1)
	, m_selected(false)
	, m_bkcolor(-1,-1,-1)
{
}

MarkPoint::MarkPoint(int frame, QColor color, QObject *parent /*= nullptr*/)
	: QObject(parent)
	, m_frame(frame)
	, m_selected(false)
	, m_bkcolor(color)
{

}

MarkPoint& MarkPoint::operator=(const MarkPoint &other)
{
	if (&other != this)
	{
		m_frame = other.m_frame;
		m_selected = other.m_selected;
		m_bkcolor = other.m_bkcolor;
	}
	return *this;
}

void MarkPoint::SetFrame(int nFrame)
{
	m_frame = nFrame;
}

int MarkPoint::GetFrame()
{
	return m_frame;
}

void MarkPoint::SetColor(QColor clr)
{
	m_bkcolor = clr;
}

QColor MarkPoint::GetColor()
{
	return m_bkcolor;
}

void MarkPoint::SetSelected(bool bSelected)
{
	m_selected = bSelected;
}

bool MarkPoint::IsSelected()
{
	return m_selected;
}

//////////////////////////////////////////////////////////////////////////

FSNLETimeline::FSNLETimeline(NLEComPtr<INLETimelineBase> pSrcTimeline, ExactType exacttype /*= ExactType_Unknown*/, QObject *parent /*= nullptr*/)
	:NLETimeline(pSrcTimeline, exacttype, parent)
{
	m_pDependency = nullptr;
	m_selectedMarker = -1;
	//m_type = TimelineType_Unknown;
}

FSNLETimeline::~FSNLETimeline()
{
    emit sigBeforeDelete();
}

FSNLETimeline* FSNLETimeline::Clone()
{
	NLEComPtr<INLETimelineBase> pCloned = NLEWraper::GetInstance().GetPlayerMaterProvider()->CloneTimeline(m_pSrcTimeLine);
	FSNLETimeline* pNewOne = new FSNLETimeline(pCloned);
	pNewOne->m_strAncestralGuid = m_strAncestralGuid;
	//pNewOne->m_type = m_type;
	pNewOne->m_thumbnail = m_thumbnail;
	pNewOne->m_pDependency = m_pDependency;
	return pNewOne;
}



QString FSNLETimeline::GetFilePath()
{
    wchar_t name[512] = {'0'};
    m_pProp->GetString(NLEKey::Base::kStrFileFullPath, name, 512);
    return QString::fromWCharArray(name);
}

void FSNLETimeline::SetFilePath(QString path)
{
    const wchar_t* pName = reinterpret_cast<const wchar_t*>(path.utf16());
    m_pProp->SetString(NLEKey::Base::kStrFileFullPath, pName);
}




void FSNLETimeline::SetAnchorPointF(QPointF anchor)
{
    m_pProp->SetPointF(kRender_AnchorPointF, NLEPointF(anchor.x(), anchor.y()));
}

void FSNLETimeline::SetRenderMode(NLERenderMode mode)
{
    m_pProp->SetInt(kRender_Mode, mode);
}

NLERenderMode FSNLETimeline::GetRenderMode()
{
    int mode;
    m_pProp->GetInt(kRender_Mode, &mode);
    return (NLERenderMode)mode;
}

bool FSNLETimeline::IsContain(QPoint pt, QRectF  baseRc)
{

    NLEMouseButton mb = NLE_MB_LEFT_TYPE;
    NLEShiftState  ss = NLE_SS_NONE_TYPE;
  
    if (m_ExactType == ExactType_Caption)
    {
        NLEComPtr <INLECaption> pCaption;
        GetSrc()->QueryInterface(IID_INLECaption, (LPVOID*)&pCaption);
        Q_ASSERT(pCaption);       
        if (pCaption->MouseDownEvent(pt.x(), pt.y(), ss, mb))
        {
            return true;
        }            
    }
    else if (m_ExactType==ExactType_Image||m_ExactType==ExactType_Video)
    {
        QRectF renderRc = GetRealRenderRegion(baseRc);
        QPainterPath path;
        path.addRect(renderRc);
        path=QTransform().rotate(GetRegionRotateAngle()).map(path);
        return path.contains(pt);
    }
    return false;
}


void FSNLETimeline::SetMaskPath(const QString &pathPoint)
{
    const wchar_t* path = reinterpret_cast<const wchar_t*>(pathPoint.utf16());
    m_pProp->SetString(kRender_MaskFile, path);
}

QString FSNLETimeline::GetMaskPath()
{
    wchar_t path[1024] = {'0'};
    m_pProp->GetString(kRender_MaskFile, path, 1024);
    return QString::fromWCharArray(path);
}

void FSNLETimeline::SetMix2Mono(bool bEnable)
{
    INT value = bEnable ? 1 : 0;
    m_pProp->SetInt(NLEKey::PostProcess::kMix2Mono, value);
}

bool FSNLETimeline::GetMix2Mono()
{
    INT value=-1;
    m_pProp->GetInt(NLEKey::PostProcess::kMix2Mono, &value);
    return value == 1;
}

void FSNLETimeline::SetDenoiseLevel(int level)
{
    INT value = qBound(0, level, 3);
    m_pProp->SetInt(NLEKey::Render::kRender_Denoise, value);
}

int FSNLETimeline::GetDenoiseLevel()
{
    INT value = -1;
    m_pProp->GetInt(NLEKey::Render::kRender_Denoise, &value);
    if (value == -1)
    {
        SetDenoiseLevel(0);
        return 0;
    }
    return value;
}

TimelineType FSNLETimeline::GetType()
{ 
	int type;
	m_pProp->GetInt(Key_TimelineType, &type);
	return (TimelineType)type;
}

void FSNLETimeline::SetType(TimelineType type,bool bGetThumbnail /*= true*/)
{
	m_pProp->SetInt(Key_TimelineType, (int)type);
	if (type == TimelineType_Video ||
		type == TimelineType_VideoAudio ||
		type == TimelineType_Image ||
		type == TimelineType_Complex)
	{
		if (!bGetThumbnail) return;
		_FetchThumbnail(0);
	}
}

void FSNLETimeline::SetThumbnail(QPixmap px)
{
	if (GetType() == TimelineType_Annotaions_Caption ||GetType() == TimelineType_Annotaions_Overlay)
	{
		if (!px.isNull())
		{
			QImage img = px.toImage();
			QByteArray ba;
			QBuffer buffer(&ba);
			buffer.open(QIODevice::WriteOnly);
			img.save(&buffer, "PNG");
			NLEBlob thumbnail((LPBYTE)buffer.buffer().constData(), buffer.buffer().length(), true);
			m_pProp->SetBlob(Key_CaptionThumnnail, thumbnail);
		}
	}
	m_thumbnail = px;
}

QPixmap FSNLETimeline::GetThumbnail()
{
	if (m_thumbnail.isNull() && 
		(GetType() == TimelineType_Annotaions_Caption || GetType() == TimelineType_Annotaions_Overlay))
	{
		NLEBlob thumbnail;
		m_pProp->GetBlob(Key_CaptionThumnnail, &thumbnail);
		QImage img = QImage::fromData(thumbnail.pData, thumbnail.uSize, "PNG");
		m_thumbnail = QPixmap::fromImage(img);
	}
	return m_thumbnail;
}

QPixmap FSNLETimeline::GetThumbnail(int nWidth, int nHeight)
{
	return ExtractImage(qBound(0, 25 * 5, GetPlayLen()/2), nWidth, nHeight);
}


QPixmap FSNLETimeline::GetThumbnail(int frames, int nWidth, int nHeight)
{
    return ExtractImage(frames, nWidth, nHeight);

}

void FSNLETimeline::_FetchThumbnail(int nFrame /*= 5 * 25*/)
{
	NLESize size;
	m_pProp->GetRenderSize(&size);
	auto rationSize = GetRatioSize(QSize(size.Width, size.Height), QSize(144, 80));
	SetThumbnail(GetThumbnail(nFrame,rationSize.width(), rationSize.height()));
	emit sigFetchedThumbnail(this);
}

QPixmap FSNLETimeline::ExtractImage(int nFrame, int nWidth, int nHeight)
{
#ifdef _DEBUG
	//return QPixmap();
#endif // _DEBUG

	NLEComPtr<INLERender> pVideoRender = NULL;
	NLEComPtr<INLERender> pAudioRender = NULL;

	NLEWraper::GetInstance().GetPlayerMaterProvider()->GetMediaRender(m_pSrcTimeLine, &pVideoRender, nullptr);
	if (!pVideoRender) return QPixmap();

	auto originRenderSize = GetRenderSize();
	SetRenderSize(GetSourceSize().width(), GetSourceSize().height());

	NLESize sz;
	m_pProp->SetInt(NLEKey::Render::kRender_RawEnabled, 1);
	m_pProp->GetRenderSize(&sz);
	QSize renderSize = QSize(sz.Width, sz.Height);

	NLEComPtr<INLEFrame> pFrame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateVideoFrame(renderSize.width(), renderSize.height());
	if (!pFrame)
	{
		m_pProp->SetInt(NLEKey::Render::kRender_RawEnabled, 0);
        SetRenderSize(originRenderSize.width(), originRenderSize.height());
		return QPixmap();
	}
		

	int nPlayLen = GetPlayLen();
	int nSrcLen = GetSrcLen();

	pVideoRender->Seek(nFrame);
	pVideoRender->Prepare(pFrame);
	pVideoRender->Render(pFrame);
	if (!pFrame)
	{
		m_pProp->SetInt(NLEKey::Render::kRender_RawEnabled, 0);
		return QPixmap();
	}
		
	void* pData = pFrame->GetData();
	int nDataLen = pFrame->GetDataSize();
	QImage img((uchar*)pData, renderSize.width(), renderSize.height(), QImage::Format_ARGB32);

	QPixmap pixSrc = QPixmap::fromImage(img);
	if (nWidth <=0 || nHeight <=0)
	{
		m_pProp->SetInt(NLEKey::Render::kRender_RawEnabled, 0);
        SetRenderSize(originRenderSize.width(), originRenderSize.height());
		return pixSrc;
	}

	auto ratioSize = GetRatioSize(QSize(sz.Width, sz.Height),QSize(nWidth,nHeight));
	int w = ratioSize.width();
	int h = ratioSize.height();

	QPixmap pix = pixSrc.scaled(w, h);

	QPixmap backPix(QSize(nWidth,nHeight));
	QPainter p(&backPix);
	p.fillRect(backPix.rect(), Qt::black);
	p.drawPixmap((backPix.width() - w) * 0.5, (backPix.height() - h)*0.5, w, h,pix);
	m_pProp->SetInt(NLEKey::Render::kRender_RawEnabled, 0);

	pVideoRender->Cleanup();
	//pAudioRender->Cleanup();
    SetRenderSize(originRenderSize.width(), originRenderSize.height());
	return backPix;
}


QString FSNLETimeline::GetDurationString()
{
	int d = GetDuration();
	int sec = d % 60;
	int min = (d % (60 * 60)) / 60;
	int h = d / (60 * 60);
	QString strTime = QString("%1:%2:%3").arg(h, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
	return strTime;
}

int FSNLETimeline::GetFrameAudio(int nFrame)
{
	INLERender *pVideoRender = NULL;
	INLERender *pAudioRender = NULL;

	NLEWraper::GetInstance().GetPlayerMaterProvider()->GetMediaRender(GetSrc(), &pVideoRender, &pAudioRender);

	if (!pAudioRender) return -1;

	NLEComPtr<INLEFrame> pFrame = NLEWraper::GetInstance().GetPlayerMaterProvider()->CreateAudioFrame(this->GetRenderFrameRate());
	if (!pFrame) return -1;

	HRESULT hr = pAudioRender->Seek(nFrame);
	hr = pAudioRender->Prepare(pFrame);
	hr = pAudioRender->Render(pFrame);
	if (!pFrame) return -1;

	void* pData = pFrame->GetData();
	int nDataLen = pFrame->GetDataSize();
	Q_ASSERT(pData);
	Q_ASSERT(nDataLen > 0);
	if (!pData) return -1;

	short* pShortData = (short*)pData;
	int nShortLen = nDataLen / 2;
	int max = 0;
	for (int i = 0; i < nShortLen; ++i)
	{
		int value = pShortData[i] + (65535 / 2);
		if (max < value)
		{
			max = value;
		}
	}
	return 1.0 * max / 65535 * 255;
}


TimelineType FSNLETimeline::GetNLETimelineType()
{
	TimelineType type = TimelineType_Unknown;
	if (GetClipCount() > 0)
	{
		type = TimelineType_Complex;
	}
	else
	{
		NLEFileType nletype = _GetNLEFileType();
		if (nletype == NLE_FILE_TYPE_VIDEO || nletype == NLE_FILE_TYPE_AUDIO)
		{
			INT nVideo = 0;
			m_pProp->GetInt(kSource_HasVideoStream, &nVideo);
			INT nAudio = 0;
			m_pProp->GetInt(kSource_HasAudioStream, &nAudio);

			if (nVideo && nAudio)
			{
				type = TimelineType_VideoAudio;
			}
			else if (nVideo && !nAudio)
			{
				type = TimelineType_Video;
			}
			else
			{
				type = TimelineType_Audio;
			}
		}
		else if (nletype == NLE_FILE_TYPE_AUDIO)
		{
			type = TimelineType_Audio;
		}
		else if (nletype == NLE_FILE_TYPE_IMAGE)
		{
			type = TimelineType_Image;
		}
	}
	return type;
}

NLEFileType FSNLETimeline::_GetNLEFileType()
{
	NLEFileType fileType = NLE_FILE_TYPE_UNKNOWN;
	m_pProp->GetInt(NLEKey::Source::kSource_FileType, (int*)&fileType);
	return fileType;
}



bool FSNLETimeline::IsRecordTimeline()
{
	NLEScreenRecordingStreamType renderType = NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN;
	HRESULT hr = m_pProp->GetInt(NLEKey::Source::kSource_ScreenRecordingStreamType, (int*)&renderType);
	if (renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO ||
		renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO ||
		renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO ||
		renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO ||
		renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO)
	{
		return true;
	}
	return false;
}

bool FSNLETimeline::IsDesktopVideo()
{
	if (GetType() == TimelineType_VideoAudio || GetType() == TimelineType_Video)
	{
		NLEScreenRecordingStreamType renderType = NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN;
		HRESULT hr = m_pProp->GetInt(NLEKey::Source::kSource_ScreenRecordingStreamType, (int*)&renderType);
		if (renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO)
		{
			return true;
		}
	}
	return false;
}

bool FSNLETimeline::IsGameVideo()
{
    if (GetType() == TimelineType_VideoAudio || GetType() == TimelineType_Video)
    {
        NLEScreenRecordingStreamType renderType = NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN;
        HRESULT hr = m_pProp->GetInt(NLEKey::Source::kSource_ScreenRecordingStreamType, (int*)&renderType);
        if (renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_GAME_VIDEO)
        {
            return true;
        }
    }
    return false;
}

bool FSNLETimeline::IsCameraVideo()
{
    if (GetType() == TimelineType_VideoAudio || GetType() == TimelineType_Video)
    {
        NLEScreenRecordingStreamType renderType = NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN;
        HRESULT hr = m_pProp->GetInt(NLEKey::Source::kSource_ScreenRecordingStreamType, (int*)&renderType);
        if (renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_WEBCAM_VIDEO)
        {
            return true;
        }
    }
    return false;
}

bool FSNLETimeline::IsMicrophoneAudio()
{
	if (GetType() == TimelineType_Audio)
	{
		NLEScreenRecordingStreamType renderType = (NLEScreenRecordingStreamType)GetRecordingStreamType();
		if (renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_MICROPHONE_AUDIO)
		{
			return true;
		}
	}
	return false;
}

bool FSNLETimeline::IsSystemAudio()
{
	if(GetType() == TimelineType_Audio)
	{
		NLEScreenRecordingStreamType renderType = (NLEScreenRecordingStreamType)GetRecordingStreamType();
		if (renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_SYSTEM_AUDIO || renderType == NLE_SCREEN_RECORDING_STREAM_TYPE_DESKTOP_VIDEO)
		{
			return true;
		}
	}
	return false;
}

bool FSNLETimeline::IsPureAudio()
{
	auto type = GetType();
	if (type == TimelineType_Audio)
		return true;
	if (type == TimelineType_Complex)
	{
		bool bHasVideo = false;
		QList<NLEComPtr<INLETimelineBase>> timelines = NLEWraper::GetInstance().GetPlayerMaterProvider()->GetSubTimelines(GetSrc());
		for (auto srctl : timelines)
		{
			FSNLETimeline fstl(srctl);
			fstl.SetType(fstl.GetNLETimelineType(), false);
			if (fstl.IsDesktopVideo() || fstl.IsGameVideo() || fstl.IsCameraVideo())
			{
					bHasVideo = true;
					break;
			}
		}
		return !bHasVideo;
	}
	return false;
}

int FSNLETimeline::GetRecordingStreamType()
{
    NLEScreenRecordingStreamType renderType = NLE_SCREEN_RECORDING_STREAM_TYPE_UNKNOWN;
    HRESULT hr = m_pProp->GetInt(NLEKey::Source::kSource_ScreenRecordingStreamType, (int*)&renderType);
    return renderType;
}

QString FSNLETimeline::GetAncestralGuidString()
{
	wchar_t guid[256] = { '0' };
	m_pProp->GetString(Key_AncestralGuid, guid, 256);
	return QString::fromWCharArray(guid);
}

void FSNLETimeline::SetAncestralGuidString(const QString &str)
{
	std::wstring wstr = str.toStdWString();
	m_pProp->SetString(Key_AncestralGuid, wstr.c_str());
}

QString FSNLETimeline::GetRecordAncestralGuidString()
{
	wchar_t guid[256] = { '0' };
	m_pProp->GetString(Key_RecordAncestralGuid, guid, 256);
	return QString::fromWCharArray(guid);
}

void FSNLETimeline::SetRecordAncestralGuidString(const QString &str)
{
	std::wstring wstr = str.toStdWString();
	m_pProp->SetString(Key_RecordAncestralGuid, wstr.c_str());
}

void FSNLETimeline::SetDisplayName(const QString &name)
{
	QString sname = name;

	if (sname.size() > 255)
	{
		sname = sname.left(255);
	}
	std::wstring wname = sname.toStdWString();
	m_pProp->SetString(Key_DisplayName, wname.c_str());
}

QString FSNLETimeline::GetDisplayName()
{
	wchar_t name[256] = { '0' };
	m_pProp->GetString(Key_DisplayName, name, 256);
	return QString::fromWCharArray(name);
}

void FSNLETimeline::SetDependency(FSNLETimeline *pDen)
{
	m_pDependency = pDen;
}

FSNLETimeline* FSNLETimeline::GetDependency()
{
	return m_pDependency;
}


QSize FSNLETimeline::GetRatioSize(QSize srcSize, QSize dstSize)
{
	double dRatio = 1.0 * srcSize.width() / srcSize.height();
	double dActualRatio = 1.0 * dstSize.width() / dstSize.height();
	int w, h;
	if (dRatio > dActualRatio)
	{
		w = dstSize.width();
		h = w / dRatio;
	}
	else
	{
		h = dstSize.height();
		w = h * dRatio;
	}
	return QSize(w, h);
}

QString FSNLETimeline::GetSourceFilePath()
{
	wchar_t filename[256] = { '0' };
	m_pProp->GetString(L"Absolute.FilePath", filename, 256);
	return QString::fromStdWString(filename);
}

void FSNLETimeline::SetBackgroundColor(QColor clr,bool bDefaultSet /*= true*/)
{
	QString strClr = QString("%1,%2,%3").arg(clr.red()).arg(clr.green()).arg(clr.blue());
	std::wstring wstrclr = strClr.toStdWString();
	m_pProp->SetString(Key_BackGroundColor, wstrclr.c_str());

	if (!bDefaultSet)
		m_pProp->SetInt(Key_BackGroundColorChangerByUser, 1);
}

QColor FSNLETimeline::GetBackgroundColor()
{
	wchar_t filename[256] = { '0' };
	m_pProp->GetString(Key_BackGroundColor, filename, 256);
	QString strClr = QString::fromStdWString(filename);
	QStringList res = strClr.split(",");
	//Q_ASSERT(res.size() >= 3);
	if (res.size() < 3)
		return QColor();
	return QColor(res[0].toInt(), res[1].toInt(), res[2].toInt());
}

bool FSNLETimeline::IsBkColorChangeByUser()
{
	int res;
	m_pProp->GetInt(Key_BackGroundColorChangerByUser, &res);
	return res == 1;
}

void FSNLETimeline::SetMarkers(const QList<int> &markers)
{
	QString res;
	for (int i = 0; i < markers.size(); ++i)
	{
		if (i == 0)
		{
			res += QString("%1").arg(markers[i]);
		}
		else
		{
			res += QString(";%1").arg(markers[i]);
		}
	}
	std::wstring wsmks = res.toStdWString();
	m_pProp->SetString(Key_ClipMarkers, wsmks.c_str());
}

QList<int> FSNLETimeline::GetMarkers()
{
	wchar_t markers[1024 * 3] = { '0' };
	m_pProp->GetString(Key_ClipMarkers, markers, 1024 * 3);
	QString strRes = QString::fromStdWString(markers);
	QList<int> lst;
	if (strRes.isEmpty())
		return lst;
	QStringList res = strRes.split(";");
	
	for (auto mk : res)
		if(!mk.isEmpty()) lst.push_back(mk.toInt());
	return lst;
}

void FSNLETimeline::AddMarker(int frame)
{
	if (!CanAddMarker()) return;
	frame = TrackMarkPos2ClipMarkPos(frame);
	wchar_t markers[1024 * 3] = { '0' };
	m_pProp->GetString(Key_ClipMarkers, markers, 1024 * 3);
	QString strRes = QString::fromStdWString(markers);
	strRes += QString(";%1").arg(frame);

	std::wstring wsmks = strRes.toStdWString();
	m_pProp->SetString(Key_ClipMarkers, wsmks.c_str());
}

void FSNLETimeline::RemoveMarker(int frame)
{
	frame = TrackMarkPos2ClipMarkPos(frame);
	if (m_selectedMarker == frame)
		m_selectedMarker = -1;
	auto markers = GetMarkers();
	markers.removeOne(frame);
	SetMarkers(markers);
}

void FSNLETimeline::SelectMarker(int frame, bool bSelected)
{
	frame = TrackMarkPos2ClipMarkPos(frame);
	if (bSelected)
		m_selectedMarker = frame;
	else if (m_selectedMarker == frame)
		m_selectedMarker = -1;
}

int FSNLETimeline::GetSelectedMarker()
{
	return m_selectedMarker;
}

void FSNLETimeline::UnselectMarker()
{
	m_selectedMarker = -1;
}

bool FSNLETimeline::CanAddMarker()
{
	auto type = GetType();
	if (type == TimelineType_Video ||
		type == TimelineType_Audio || 
		type == TimelineType_VideoAudio)
	{
		return true;
	}
	return false;
}

int FSNLETimeline::ClipMarkPos2TrackMarkPos(int pos)
{
	return pos + (GetPosition() - GetTrimin());
}

int FSNLETimeline::TrackMarkPos2ClipMarkPos(int pos)
{
	return pos - (GetPosition() - GetTrimin());
}

QList<int> FSNLETimeline::GetTrackMarkPos()
{
	QList<int> lst;
	auto markers = GetMarkers();
	for (auto mk : markers)
	{
		lst.push_back(ClipMarkPos2TrackMarkPos(mk));
	}
	return lst;
}

void FSNLETimeline::UpdateTrackMarks(QList<MarkPointPtr> mps)
{
	QString strRes;
	for (auto mp : mps)
	{
		strRes += (QString("%1,%2,%3,%4").arg(mp->GetFrame()).arg(mp->GetColor().red()).arg(mp->GetColor().green()).arg(mp->GetColor().blue()) + ";");
	}
	std::wstring wsmks = strRes.toStdWString();
	m_pProp->SetString(Key_TrackMarkers, wsmks.c_str());
}

QList<MarkPointPtr> FSNLETimeline::CreateTrackMarks()
{
	wchar_t markers[1024 * 16] = { '0' };
	m_pProp->GetString(Key_TrackMarkers, markers, 1024 * 16);
	QString strRes = QString::fromStdWString(markers);
	QList<MarkPointPtr> lst;
	if (strRes.isEmpty())
		return lst;
	QStringList res = strRes.split(";");
	for (auto oneMark : res)
	{
		if (oneMark.count(",")!= 3)
			continue;

		auto markinfos = oneMark.split(",");
		if (markinfos.size() != 4)
			continue;
		MarkPointPtr ptr(new MarkPoint(markinfos[0].toInt(), QColor(markinfos[1].toInt(), markinfos[2].toInt(), markinfos[3].toInt())));
		lst.push_back(ptr);
	}
	return lst;
}

void FSNLETimeline::SetHidenTracks(QList<int> tracks)
{
	QString res;
	for (int i = 0; i < tracks.size(); ++i)
	{
		if (i == 0)
			res += QString("%1").arg(tracks[i]);
		else
			res += QString(";%1").arg(tracks[i]);
	}
	std::wstring wsmks = res.toStdWString();
	m_pProp->SetString(Key_HidenTracks, wsmks.c_str());
}

QList<int> FSNLETimeline::GetHidenTracks()
{
	wchar_t hidens[1024 * 2] = { '0' };
	m_pProp->GetString(Key_HidenTracks, hidens, 1024 * 2);
	QString strRes = QString::fromStdWString(hidens);
	QList<int> lst;
	if (strRes.isEmpty())
		return lst;
	QStringList res = strRes.split(";");

	for (auto one : res)
		if (!one.isEmpty()) lst.push_back(one.toInt());
	return lst;
}

void FSNLETimeline::SetFreezenTracks(QList<int> tracks)
{
	QString res;
	for (int i = 0; i < tracks.size(); ++i)
	{
		if (i == 0)
			res += QString("%1").arg(tracks[i]);
		else
			res += QString(";%1").arg(tracks[i]);
	}
	std::wstring wsmks = res.toStdWString();
	m_pProp->SetString(Key_FreezenTracks, wsmks.c_str());
}

QList<int> FSNLETimeline::GetFreezenTracks()
{
	wchar_t freezens[1024 * 2] = { '0' };
	m_pProp->GetString(Key_FreezenTracks, freezens, 1024 * 2);
	QString strRes = QString::fromStdWString(freezens);
	QList<int> lst;
	if (strRes.isEmpty())
		return lst;
	QStringList res = strRes.split(";");

	for (auto one : res)
		if (!one.isEmpty()) lst.push_back(one.toInt());
	return lst;
}

